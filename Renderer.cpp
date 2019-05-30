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

#include <windows.h>
#include <Renderer.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Icon.h"

Renderer::Renderer(const char* title, GLFWwindowsizefun fResize)
{
    int numberOfImages = 1;
    GLFWimage images[1];
    images[0].pixels = stbi_load_from_memory(icon, sizeof(icon), &images[0].width, &images[0].height, 0, 4);

    mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, title, NULL, NULL);
    glfwSetWindowIcon(mWindow, 1, images);
    glfwSetWindowSizeCallback(mWindow, fResize);
    glfwMakeContextCurrent(mWindow);

    BoundingBoxf bbox = { {-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0} };
    mCamera = new Camera(bbox);
    mModel = new Cube();
}

void Renderer::Paint()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    mCamera->Perspective((float)mWindowWidth / (float)mWindowHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    mCamera->LookAt();

    glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mModel->glDraw();
    glfwSwapBuffers(mWindow);
}
