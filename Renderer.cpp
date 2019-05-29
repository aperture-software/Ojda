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

Renderer::Renderer(GLFWwindowsizefun fResize)
{
    mModel = new Cube();

    mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "Ojda - OpenGL Viewer", NULL, NULL);
    glfwSetWindowSizeCallback(mWindow, fResize);
    glfwMakeContextCurrent(mWindow);
}

void Renderer::Paint()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fovy, (GLdouble)mWindowWidth / (GLdouble)mWindowHeight, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x(), eye.y(), eye.z(), center.x(), center.y(), center.z(), up.x(), up.y(), up.z());

    glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mModel->glDraw();
    glfwSwapBuffers(mWindow);
}
