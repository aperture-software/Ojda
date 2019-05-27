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
#include <GLFW/glfw3.h>

int main()
{
    GLFWwindow* window;

    std::cout << "Ojda - OpenGL Viewer\n";
    if (glfwInit() == GL_FALSE) {
        std::cerr << "Could not initialize glfw.\n";
        return -1;
    }

    window = glfwCreateWindow(1680, 1050, "Ojda - OpenGL Viewer", NULL, NULL);
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    return 0;
}
