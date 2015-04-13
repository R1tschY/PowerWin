NAME = libpowerwin
SUFFIX = .dll

# compile flags
WARNINGS = -Wall -Wextra -Wuninitialized -Wno-unused-local-typedefs -Wno-missing-field-initializers
FEATURES = -std=gnu++0x
MACROS   = -DUNICODE -DBUILD_DLL
INCLUDES = -I. -I..
LIBS     = -shared -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lwinpthread -Wl,-Bdynamic -lgdiplus

# plattforms
ifeq ($(OS),Windows_NT)
    CPREFIX     =  
    RMDIR       = rmdir /S /Q 
    SEP         = \\
    BUILD_PATH  = ..\build
    INCLUDES   += -I C:/include/boost_1_55_0
else
    CPREFIX     = i686-w64-mingw32-
    SEP         = /
    RMDIR       = rm -rf 
    BUILD_PATH  = ../build
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
#MACROS  += -DWIN32_LEAN_AND_MEAN

# sources
SOURCES = \
	$(wildcard *.cpp) \
	$(wildcard plugins/*.cpp) \
	$(wildcard c++/*.cpp) \
	$(wildcard windows/*.cpp) \
	$(wildcard windows/controls/*.cpp)

BINARY32 = $(BUILD_PATH)/$(NAME)32$(SUFFIX)
OBJECTS32 = $(patsubst %.cpp, $(BUILD_PATH)/%.o32, $(SOURCES))

BINARY64 = $(BUILD_PATH)/$(NAME)64$(SUFFIX)
OBJECTS64 = $(patsubst %.cpp, $(BUILD_PATH)/%.o64, $(SOURCES))


all: $(BINARY32) $(BINARY64)

$(BUILD_PATH):
	$(MKDIR) $(BUILD_PATH)

$(BUILD_PATH)/%.o32: %.cpp
	@$(MKDIR) -p `dirname $@`
	$(CXX) -m32 -DMAIN_MODULE $(FEATURES) $(WARNINGS) $(MACROS) $(INCLUDES) -MMD -MP -MT $@ -MF $@.d -o $@ -c $<

$(BUILD_PATH)/%.o64: %.cpp
	@$(MKDIR) -p `dirname $@`
	$(CXX) $(FEATURES) $(WARNINGS) $(MACROS) $(INCLUDES) -MMD -MP -MT $@ -MF $@.d -o $@ -c $<

$(BINARY32): $(BUILD_PATH) $(OBJECTS32)
	$(CXX) -m32 $(FEATURES) $(WARNINGS) -o $(BINARY32) $(OBJECTS32) $(LIBS)

$(BINARY64): $(BUILD_PATH) $(OBJECTS64)
	$(CXX) $(FEATURES) $(WARNINGS) -o $(BINARY64) $(OBJECTS64) $(LIBS)

clean:
	$(RMDIR) $(BUILD_PATH)
	
-include $(wildcard $(BUILD_PATH)/*.o32.d)
-include $(wildcard $(BUILD_PATH)/*.o64.d)
