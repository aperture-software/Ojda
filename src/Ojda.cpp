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
#include "Renderer.h"

#define UNUSED(expr) do { (void)(expr); } while (0)

Renderer* render;

// Override std::cout and std::cerr to get messages sent to the Visual Studio output window
#if defined(_WIN32)
class vs_stream : std::basic_streambuf<char, std::char_traits<char>>
{
protected:
    std::streamsize xsputn (const std::char_traits<char>::char_type* s, std::streamsize n)
    {
        OutputDebugStringA(s);
        return n;
    }

    std::char_traits<char>::int_type overflow (std::char_traits<char>::int_type c)
    {
        char str[2]; str[0] = c; str[1] = 0;
        OutputDebugStringA(str);
        return std::char_traits<char>::int_type(c);
    }
};

typedef std::basic_streambuf<char, std::char_traits<char>> sbuf_char;
#endif

// GLFW callbacks
void glfw_error(int error, const char* description)
{
    std::cerr << "GLFW[" << error << "]: " << description << "\n";
}

void glfw_resize(GLFWwindow* window, int w, int h)
{
    glfwMakeContextCurrent(window);
    glViewport(0, 0, w, h);
    render->mWindowWidth = w;
    render->mWindowHeight = h;
    render->Paint();
}

void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    UNUSED(xoffset);
    glfwMakeContextCurrent(window);
    render->Zoom((float)(3.0 * yoffset));
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

    render = new Renderer("Ojda - OpenGL Viewer", glfw_resize, "res/Cube.obj");
    GLFWwindow* window = render->getWindow();
    glfwSetScrollCallback(window, glfw_scroll_callback);

    while (!glfwWindowShouldClose(window)) {
        render->Paint();
        glfwWaitEvents();
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        render->Rotate((float)xpos, (float)ypos,
            (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE));
    }

    return 0;
}

#if defined(_WIN32)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNUSED(hInstance);
    UNUSED(hPrevInstance);
    UNUSED(lpCmdLine);
    UNUSED(nCmdShow);
    vs_stream vs;
    sbuf_char *cout_buf = std::cout.rdbuf();
    sbuf_char *cerr_buf = std::cerr.rdbuf();

    std::cout.rdbuf((sbuf_char *)&vs);
    std::cerr.rdbuf((sbuf_char *)&vs);

    int r = main();

    std::cout.rdbuf(cout_buf);
    std::cerr.rdbuf(cerr_buf);

    return r;
}
#endif
