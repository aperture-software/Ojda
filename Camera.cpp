/*
 * Ojda: OpenGL Viewer
 *
 * Copyright © 2019 Aperture Software
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>
#include <gl/GLU.h>

#include "Camera.h"

Camera::Camera(const BoundingBoxf bbox)
{
    mBoundingSphereRadius = (bbox.min - bbox.max).norm() / 2.0f;
    mCenter = (bbox.min + bbox.max) / 2.0f;
    mDistToCenter = mBoundingSphereRadius / std::sin(mFovY / 360.0f * (float)M_PI);
    mNearDist = mDistToCenter - mBoundingSphereRadius;
    mFarDist = mDistToCenter + mBoundingSphereRadius;
}

void Camera::Perspective(float ar) const
{
    gluPerspective(mFovY, ar, mNearDist, mFarDist);
}

void Camera::LookAt()
{
    const Matrix3f rotation = mRotation.toRotationMatrix();
    // Without rotation applied, our eye sits at mDistToCenter on the Z axis
    // and up is the Y axis. To revolve around the object, we simply apply
    // the rotation to both these vectors.
    mEye = mCenter +  rotation * mDistToCenter * Vector3f::UnitZ();
    mUp = rotation * Vector3f::UnitY();
    gluLookAt(mEye.x(), mEye.y(), mEye.z(), mCenter.x(), mCenter.y(), mCenter.z(), mUp.x(), mUp.y(), mUp.z());
}

void Camera::Zoom(float delta)
{
    // Just change the field of view for zoom
    mFovY += delta;
    if (mFovY < 20.0f)
        mFovY = 20.0f;
    if (mFovY > 179.0f)
        mFovY = 179.0f;
}

void Camera::ProjectToSphere(Vector3f& point) const
{
    // Compute distance to center to find the rotation mode to use.
    // A point within mProjectionSphereRadius produces an X axis rotation.
    // A point outside mProjectionSphereRadius produces a Z axis rotation.
    const float r2 = mProjectionSphereRadius * mProjectionSphereRadius;
    const float d2 = point.x() * point.x() + point.y() * point.y();
    // Now project our point onto the projection sphere by:
    if (d2 < r2) {
        // Adding the missing distance to the surface, in z.
        point.z() = std::sqrt(r2 - d2);
    } else {
        // Multiplying x & y by the relevant factor (r/√d2).
        point *= mProjectionSphereRadius / std::sqrt(d2);
    }
}

void Camera::Rotate(const Vector2f current_pos, const Vector2f target_pos)
{
    // Convert the plannar (x, y) coordinates to a 3D point on the projection sphere.
    Vector3f current_point(current_pos.x(), current_pos.y(), 0.0f);
    Vector3f target_point(target_pos.x(), target_pos.y(), 0.0f);
    ProjectToSphere(current_point);
    ProjectToSphere(target_point);

    // Compute the quaternion that transforms between current and target through a rotation.
    Quaternionf q;
    q.setFromTwoVectors(target_point, current_point);
    mRotation *= q;
}
