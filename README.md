PowerWin
========

*Extent Windows with some cool features.*

Features
--------

* scroll in inactive windows.
* move window to topmost (by the system memu of the window).
* don't start screensaver or go to sleep when a fullscreen window exist.

* configure with ini file.
* use as portable application.
* support for 64-Bit (for WoW64 and native applications) and 32-Bit Windows.

Future features
---------------

* start moving of window by hotkey.
* hotkey to close other processes.
* rollup window (by system menu or hotkey).
* configure with gui.
* kill windows like xkill for X11 (by hotkey)

Known Issues
------------

  * no scrolling in inactive windows:
  	- in Visual Studio 2010
  	- when the active window is topmost
  
Compile (with biicode)
----------------------

~~~
bii init -L
bii configure -DCMAKE_BUILD_TYPE=Release ..
bii build -j4
~~~

Cross compile from Linux (with biicode)
---------------------------------------

~~~
bii init -L
bii configure -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=`pwd`/Toolchain-cross-mingw.cmake -DCOMPILER_PREFIX=<COMPILER_PREFIX> ..
bii build -j4
~~~

Replace `<COMPILER_PREFIX>` with the prefix of your mingw cross compiler. In
Ubuntu use `x86_64-w64-mingw32`.

  
Compile (without biicode)
-------------------------

~~~
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=<BOOST_DIR> ..
make -j4
~~~
Replace `<BOOST_DIR>` with the directory you installed boost, e.x. `C:\boost_1_55_0`.

Cross compile from Linux (without biicode)
------------------------------------------

~~~
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=<BOOST_DIR> -DCMAKE_TOOLCHAIN_FILE=../Toolchain-cross-mingw.cmake -DCOMPILER_PREFIX=<COMPILER_PREFIX> ..
make -j4
~~~

Replace `<COMPILER_PREFIX>` with the prefix of your mingw cross compiler. In
Ubuntu use `x86_64-w64-mingw32`. Also replace `<BOOST_DIR>` with the directory
you installed boost, e.x. `~/boost_1_55_0`.
