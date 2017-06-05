PowerWin
========

*Extent Windows with some cool features.*

Features
--------

* scroll in inactive windows.
* move window to topmost (by the system menu of the window).
* kill windows (xkill clone).

* configure with ini file.
* portable application.
* support for 64-Bit and 32-Bit Windows.

Future features
---------------

* start moving of window by hotkey.
* rollup window (by system menu or hotkey).

Known Issues
------------

  * no scrolling in inactive windows:
  	- in Visual Studio 2010
  	- when the active window is topmost
  
  * OpenCV `cv::namedWindow` ends in dead-lock for system-menu module
  
Compile
-------

For compilation a *MSYS2* enviroment is needed:

https://sourceforge.net/projects/msys2/

In the **MSYS2 MinGW 64-Bit enviroment** do:

Install requirements:

    pacman -S \
      mingw32/mingw-w64-i686-gcc \
      mingw32/mingw-w64-i686-boost \
      mingw64/mingw-w64-i686-cmake \
      mingw32/mingw-w64-i686-qt5-static \
      mingw64/mingw-w64-x86_64-gcc \
      mingw64/mingw-w64-x86_64-boost \
      mingw64/mingw-w64-x86_64-cmake \
      msys/cmake \
      msys/git

Checkout the sources:

    git clone https://github.com/R1tschY/PowerWin.git
    
Create a build directory:

    cd PowerWin
    mkdir build
    
Configure the project with CMake:
    
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
    
Build project:

    make
    
Create zip bundle:

    cpack

