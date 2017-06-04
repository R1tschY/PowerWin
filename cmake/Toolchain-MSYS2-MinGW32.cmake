# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# Choose an appropriate compiler prefix
SET(COMPILER_PREFIX i686-w64-mingw32)

# which compilers to use for C and C++
set(CMAKE_RC_COMPILER C:/msys64/mingw32/bin/windres.exe)
set(CMAKE_C_COMPILER C:/msys64/mingw32/bin/gcc.exe)
set(CMAKE_CXX_COMPILER C:/msys64/mingw32/bin/g++.exe)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH C:/msys64/mingw32)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
