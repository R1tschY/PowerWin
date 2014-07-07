NAME = powerwin
SUFFIX = .exe
BUILD_PATH = ..\build

WARNINGS= -Wall -Wuninitialized -Wno-unused-local-typedefs
FEATURES= -O2 -s
MACROS  = -DUNICODE
INCLUDES= -I../lib
LIBS    = -L$(BUILD_PATH) -static-libgcc -lpowerwin32 -mwindows -m32


# compile for Windows XP
MACROS  += -DNTDDI_VERSION=0x05010300 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 \
          -D_WIN32_IE=0x0700
MACROS  += -DWIN32_LEAN_AND_MEAN

CXX     = gcc

BINARY = $(BUILD_PATH)/$(NAME)$(SUFFIX)

all: $(BINARY)

$(BUILD_PATH):
	mkdir $(BUILD_PATH)
	
$(BINARY): $(BUILD_PATH) main.c
	$(CXX) $(FEATURES) $(WARNINGS) $(INCLUDES) -o $(BINARY) main.c $(LIBS)

clean:
	rmdir /S /Q $(BUILD_PATH)

