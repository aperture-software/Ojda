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
#include "Dbg.h"
#include "Renderer.h"

Renderer* render;

// GLFW callbacks
void glfw_error(int error, const char* description)
{
    std::cerr << "GLFW error: " << description << "\n";
}

void glfw_resize(GLFWwindow* window, int w, int h)
{
    glfwMakeContextCurrent(window);
    glViewport(0, 0, w, h);
    render->mWindowWidth = w;
    render->mWindowHeight = h;
    render->Paint();
}

int main()
{
    std::cout << "Ojda - OpenGL Viewer\n";
    glfwSetErrorCallback(glfw_error);
    if (glfwInit() == GL_FALSE) {
        std::cerr << "Could not initialize GLFW.\n";
        return -1;
    }

    render = new Renderer("Ojda - OpenGL Viewer", glfw_resize);
    GLFWwindow* window = render->getWindow();

    while (!glfwWindowShouldClose(window)) {
        render->Paint();
        glfwWaitEvents();
    }

    return 0;
}

#if defined(WIN32)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main();
}
#endif
