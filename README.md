PowerWin
========

*Extent Windows with some cool features.*

Features
--------

* scroll in inactive windows.
* move window to topmost (by the system menu of the window).

* configure with ini file.
* use as portable application.
* support for 64-Bit and 32-Bit Windows.

Future features
---------------

* start moving of window by hotkey.
* rollup window (by system menu or hotkey).
* configure with gui.
* kill windows like xkill for X11 (by hotkey)

Known Issues
------------

  * no scrolling in inactive windows:
  	- in Visual Studio 2010
  	- when the active window is topmost
  
  * OpenCV `cv::namedWindow` ends in dead-lock for system-menu module
  
Compile
-------

For compilation a MSYS2 enviroment is needed:

https://sourceforge.net/projects/msys2/

Install requirements in MSYS:

    pacman -S mingw32/mingw-w64-i686-gcc mingw64/mingw-w64-x86_64-gcc \
      mingw32/mingw-w64-i686-boost mingw64/mingw-w64-x86_64-boost \
      mingw64/mingw-w64-x86_64-cmake msys/git


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


Cross compile from Linux
------------------------

Install *Mingw-w64* cross-compiler and requirements:

    # for Ubuntu:
    sudo apt-get install mingw-w64 ninja-build cmake git


Build:

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release \
      -DBOOST_ROOT=<BOOST_DIR> \
      -DCOMPILER_PREFIX=<COMPILER_PREFIX> \
      ..
    make -j4


Replace `<COMPILER_PREFIX>` with the prefix of your mingw cross compiler. In
Ubuntu use `x86_64-w64-mingw32`. Also replace `<BOOST_DIR>` with the directory
you installed boost, e.x. `~/boost_1_55_0`. `<BOOST_DIR>` should not be 
`/usr/include` or any other host system include directory.
