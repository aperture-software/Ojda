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
    mBoundingSphereRadius = (bbox.min - bbox.max).norm() * 0.5f;
    mCenter = (bbox.min + bbox.max) * 0.5f;
    mDistToCenter = mBoundingSphereRadius / std::sin(fovy / 360.0f * (float)M_PI);
    zNear = mDistToCenter - mBoundingSphereRadius;
    zFar = mDistToCenter + mBoundingSphereRadius;
}

void Camera::Perspective(float ar) const
{
    gluPerspective(fovy, ar, zNear, zFar);
}

void Camera::LookAt() const
{
    gluLookAt(mEye.x(), mEye.y(), mEye.z(), mCenter.x(), mCenter.y(), mCenter.z(), mUp.x(), mUp.y(), mUp.z());
}
