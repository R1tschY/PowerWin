PowerWin
========

*Extent Windows with some cool features.*

Features
--------

* scroll in inactive windows.
* move window to topmost (by the system menu of the window).
* kill windows (xkill clone).
* resizing and moving of window by mouse.


* configure with ini file.
* portable application.
* support for 64-Bit and 32-Bit Windows.

Future features
---------------

* rollup window (by system menu or hotkey).
* change transparency of window.

Known Issues
------------

  * no scrolling in inactive windows:
  	- in Visual Studio 2010
  	- when the active window is topmost
  	
  * ConEmu: no scrolling in splitted consoles 
  
  * OpenCV `cv::namedWindow` ends in dead-lock for system-menu module
  
Configure
---------

Place a file named `config.ini` next to the `PowerWin.exe`. This file
has to be in the ini format. The default configuration is:

```
[scroll]
active=true
inverse=false

[system-menu]
active=true

[wkill]
active=true
hotkey=Ctrl+Alt+X

[wnotify]
active=true
move=alt+leftbutton
resize=alt+middlebutton
```

  
Compile
-------

For compilation a *MSYS2* enviroment is needed:

https://sourceforge.net/projects/msys2/

In the **MSYS2 MSYS console** do:

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
    git submodule init
    
Create a build directory:

    cd PowerWin
    mkdir build
    
Configure the project with CMake:
    
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ..
    
Build project:

    make install
    
Create zip bundle:

    cpack

