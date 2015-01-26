# config
NAME = powerwin
SUFFIX = .exe

# flags
WARNINGS= -Wall -Wuninitialized -Wno-unused-local-typedefs
FEATURES= -O2 -s -Wl,--subsystem,windows
MACROS  = -DUNICODE -D_UNICODE
INCLUDES= -I../lib
LIBS    = -L$(BUILD_PATH) -static-libgcc -lpowerwin32 -mwindows -m32

# plattforms
ifeq ($(OS),Windows_NT)
    CPREFIX=  
    RMDIR   = rmdir /S /Q 
    BUILD_PATH = ..\build
else
	CPREFIX=i686-w64-mingw32-
	RMDIR   = rm -rf 
	BUILD_PATH = ../build
endif

# programs
CXX     = $(CPREFIX)g++
CC      = $(CPREFIX)gcc
RES     = $(CPREFIX)windres
MKDIR   = mkdir

# compile for Windows XP
MACROS  += -DNTDDI_VERSION=0x05010300 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 \
          -D_WIN32_IE=0x0700
MACROS  += -DWIN32_LEAN_AND_MEAN

BINARY = $(BUILD_PATH)/$(NAME)$(SUFFIX)

all: $(BINARY)

$(BUILD_PATH):
	$(MKDIR) $(BUILD_PATH)

powerwin.o: powerwin.rc
	$(RES) -i powerwin.rc -o powerwin.o
	
$(BINARY): $(BUILD_PATH) main.c powerwin.o
	$(CC) $(FEATURES) $(WARNINGS) $(INCLUDES) -o $(BINARY) main.c powerwin.o $(LIBS)

clean:
	$(RMDIR) $(BUILD_PATH)

