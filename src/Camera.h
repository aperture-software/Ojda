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
#pragma once

#include "Mesh.h"

using namespace Eigen;

class Camera
{
private:
    const float mProjectionSphereRadius = 0.8f;

    Vector3f mEye = { 0.0f, 0.0f, 10.0f };
    Vector3f mCenter = { 0.0f, 0.0f, 0.0f };
    Vector3f mUp = { 0.0f, 1.0f, 0.0f };
    float mFovY = 40.0f;
    float mNearDist = 1.0f;
    float mFarDist = 10.0f;
    float mDistToCenter = 1.0f;
    float mBoundingSphereRadius;
    Quaternionf mRotation = Quaternionf(1.0f, 0.0f, 0.0f, 0.0f);

    void ProjectToSphere(Vector3f& point) const;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Camera(const BoundingBox bbox);
    void Perspective(float ar) const;
    void LookAt();
    void Zoom(float delta);
    void Rotate(const Vector2f oldpos, const Vector2f newpos);
};
