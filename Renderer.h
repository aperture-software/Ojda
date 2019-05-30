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
#include <Eigen/Eigen>

#include "Camera.h"
#include "Cube.h"

using namespace Eigen;

class Renderer
{
private:
    Cube* mModel;
    Camera* mCamera;
    GLFWwindow* mWindow;
    float mMouseX, mMouseY;

public:
    int mWindowWidth = 1680, mWindowHeight = 1050;

    Renderer(const char* title, GLFWwindowsizefun fResize);
    void Paint();
    void Rotate(float xpos, float ypos, bool update_only);
    void Zoom(float delta);
    inline GLFWwindow* getWindow(void) const { return mWindow; }
};
