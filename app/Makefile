# config
NAME = powerwin
SUFFIX = .exe

# flags
WARNINGS = -Wall -Wuninitialized -Wno-unused-local-typedefs
FEATURES = -Wl,--subsystem,windows
MACROS   = -DUNICODE -D_UNICODE
INCLUDES = -I../lib
LIBS     = -L$(BUILD_PATH) -static-libgcc -lpowerwin32 -mwindows -m32
RCFLAGS  = --output-format=coff --target=pe-i386

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

# Release:
#FEATURES += -O2 -s -DNDEBUG -flto -fvisibility=hidden -fvisibility-inlines-hidden -finline-functions

# Debug
FEATURES += -g -Og

# compile for Windows XP
MACROS  += -DNTDDI_VERSION=0x05010300 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 \
          -D_WIN32_IE=0x0700
MACROS  += -DWIN32_LEAN_AND_MEAN

BINARY = $(BUILD_PATH)/$(NAME)$(SUFFIX)

all: $(BINARY)

$(BUILD_PATH):
	$(MKDIR) $(BUILD_PATH)

$(BUILD_PATH)/powerwinrc.o32: powerwin.rc
	$(RES) $(RCFLAGS) -i powerwin.rc -o $(BUILD_PATH)/powerwinrc.o32
	
$(BINARY): $(BUILD_PATH) main.c $(BUILD_PATH)/powerwinrc.o32
	$(CC) $(FEATURES) $(WARNINGS) $(INCLUDES) -o $(BINARY) main.c $(BUILD_PATH)/powerwinrc.o32 $(LIBS)

clean:
	$(RMDIR) $(BUILD_PATH)

