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

#include <Eigen/Eigen>

using namespace Eigen;

typedef struct {
    Vector3f min;
    Vector3f max;
} BoundingBoxf;

class Camera
{
private:
    Vector3f mEye = { 2.0f, 2.5f, 5.0f };
    Vector3f mCenter = { 0.0f, 0.0f, 0.0f };
    Vector3f mUp = { 0.15f, 0.9f, 0.45f };
    float fovy = 40.0f;
    float zNear = 1.0f;
    float zFar = 10.0f;
    float mDistToCenter;
    float mBoundingSphereRadius;
public:
    Camera(const BoundingBoxf bbox);
    void Perspective(float ar) const;
    void LookAt() const;
};
