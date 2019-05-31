Ojda - A cross platform OpenGL/GLFW model viewer
================================================

Ojda is a cross platform __dependency free__ 3D model viewer.

We decided to create this project because we got tired of seeing people
putting either closed-source or restrictive (i.e. non-GPL) versions of
such software, as well as requiring the installation of cumbersome or
ill-advised preliminaries like QT or CMake to be installed (You really
shouldn't be forced to use CMake or QT to compile a project like this).

So here we provide a GPLv3 version of basic OpenGL model renderer, that
does not require you to hunt for 3rd party libraries, and that should
compile on most platforms.

Compilation
-----------

On Windows, either use the Visual Studio 2017 solution file or run
make with MinGW.

On Linux or Mac, just run make.

Usage
-----

Just launch the executable. You should be able to rotate the model
with the left mouse button as well as zoom with the scroll wheel.
