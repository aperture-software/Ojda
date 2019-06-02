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

#include <vector>
#if defined(_WIN32)
#include <windows.h>
#include <gl/GLU.h>
#elif defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <Eigen/Eigen>

using namespace Eigen;

class Cube {
private:
    Vector3f getNormal(const size_t index) const;
    std::vector<Matrix3f> mFace;
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Cube();
    void glDraw() const;
};
