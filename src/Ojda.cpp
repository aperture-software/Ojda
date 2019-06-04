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
#include <codecvt>
#include <locale>
#include "Renderer.h"

using namespace std;

#define UNUSED(expr) do { (void)(expr); } while (0)

Renderer* render;

// Override cout and cerr to get messages sent to the Visual Studio output window
#if defined(_WIN32)
class vs_stream : basic_streambuf<char, char_traits<char>>
{
protected:
    streamsize xsputn (const char_traits<char>::char_type* s, streamsize n)
    {
        OutputDebugStringA(s);
        return n;
    }

    char_traits<char>::int_type overflow (char_traits<char>::int_type c)
    {
        char str[2]; str[0] = c; str[1] = 0;
        OutputDebugStringA(str);
        return char_traits<char>::int_type(c);
    }
};

typedef basic_streambuf<char, char_traits<char>> sbuf_char;
#endif

// GLFW callbacks
void glfw_error(int error, const char* description)
{
    cerr << "GLFW[" << error << "]: " << description << "\n";
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

int main(int argc, char* argv[])
{
    cout << "Ojda - OpenGL Viewer\n";

    glfwSetErrorCallback(glfw_error);
    if (glfwInit() == GL_FALSE) {
        cerr << "Could not initialize GLFW.\n";
        return -1;
    }

    render = new Renderer("Ojda - OpenGL Viewer", glfw_resize, (argc >= 2) ? argv[1] : "res/Cube.obj");
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
    sbuf_char *cout_buf = cout.rdbuf();
    sbuf_char *cerr_buf = cerr.rdbuf();

    cout.rdbuf((sbuf_char *)&vs);
    cerr.rdbuf((sbuf_char *)&vs);

    // Process Unicode parameters
    typedef int (CDECL *__wgetmainargs_t)(int*, wchar_t***, wchar_t***, int, int*);
    __wgetmainargs_t __wgetmainargs = (__wgetmainargs_t)GetProcAddress(GetModuleHandleA("msvcrt"), "__wgetmainargs");
    int argc = 0, si = 0;
    wchar_t **wenv, **wargv;
    __wgetmainargs(&argc, &wargv, &wenv, 1, &si);
    char** argv = (char**)calloc(argc, sizeof(char*));
    wstring_convert<codecvt_utf8<wchar_t>> utf8_conv;
    for (int i = 0; i < argc; i++)
        argv[i] = const_cast<char*>(utf8_conv.to_bytes(wargv[i]).c_str());
    int r = main(argc, argv);

    cout.rdbuf(cout_buf);
    cerr.rdbuf(cerr_buf);

    return r;
}
#endif
