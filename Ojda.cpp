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
#include <iostream>
#include <vector>
#include <windows.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include "Cube.h"

const GLfloat ambient_light[] = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 0.0f };
const GLfloat position_light[] = { 0.0f, 0.0f, 1000.0f, 0.0f };
const GLfloat specular_light[] = { 0.4f, 0.4f, 0.4f, 1.0f };

int main()
{
    const int window_width = 1680, window_height = 1050;
    const GLdouble fovy = 40.0;
    const GLdouble zNear = 1.0;
    const GLdouble zFar = 10.0;
    const Eigen::Vector3d eye = { 2.0, 2.5, 5.0 };
    const Eigen::Vector3d center = { 0.0, 0.0, 0.0 };
    const Eigen::Vector3d up = { 0.15, 0.9, 0.45 };

    GLFWwindow* window;
    Cube* cube = new Cube();

    std::cout << "Ojda - OpenGL Viewer\n";
    if (glfwInit() == GL_FALSE) {
        std::cerr << "Could not initialize glfw.\n";
        return -1;
    }

    window = glfwCreateWindow(window_width, window_height, "Ojda - OpenGL Viewer", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Lighting setup
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position_light);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glClearColor((GLclampf)0.1, (GLclampf)0.3, (GLclampf) 0.7, (GLclampf)1.0);

    while (!glfwWindowShouldClose(window)) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fovy, (GLdouble)window_width / (GLdouble)window_height, zNear, zFar);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye.x(), eye.y(), eye.z(), center.x(), center.y(), center.z(), up.x(), up.y(), up.z());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube->glDraw();

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    return 0;
}
