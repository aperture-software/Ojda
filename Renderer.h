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

#include <windows.h>
#include <GLFW/glfw3.h>

#include "Cube.h"

class Renderer
{
private:
    const GLdouble fovy = 40.0;
    const GLdouble zNear = 1.0;
    const GLdouble zFar = 10.0;
    const Vector3d eye = { 2.0, 2.5, 5.0 };
    const Vector3d center = { 0.0, 0.0, 0.0 };
    const Vector3d up = { 0.15, 0.9, 0.45 };

    Cube* mModel;
    GLFWwindow* mWindow;

public:
    int mWindowWidth = 1680, mWindowHeight = 1050;

    Renderer(GLFWwindowsizefun fResize);
    void Paint();
    inline GLFWwindow* getWindow(void) const { return mWindow; }
};
