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
    SEP         = \
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

BINARY32 = $(BUILD_PATH)/$(NAME)32$(SUFFIX)
OBJECTS32 = $(patsubst %.cpp, $(BUILD_PATH)/%.o32, $(wildcard *.cpp))
OBJECTS32 += $(patsubst plugins/%.cpp, $(BUILD_PATH)/plugins/%.o32, $(wildcard plugins/*.cpp))
OBJECTS32 += $(patsubst c++/%.cpp, $(BUILD_PATH)/c++/%.o32, $(wildcard c++/*.cpp))
OBJECTS32 += $(patsubst windows/%.cpp, $(BUILD_PATH)/windows/%.o32, $(wildcard windows/*.cpp))
OBJECTS32 += $(patsubst windows/controls/%.cpp, $(BUILD_PATH)/windows/controls/%.o32, $(wildcard windows/controls/*.cpp))

BINARY64 = $(BUILD_PATH)/$(NAME)64$(SUFFIX)
OBJECTS64 = $(patsubst %.cpp, $(BUILD_PATH)/%.o64, $(wildcard *.cpp))
OBJECTS64 += $(patsubst plugins/%.cpp, $(BUILD_PATH)/plugins/%.o64, $(wildcard plugins/*.cpp))
OBJECTS64 += $(patsubst c++/%.cpp, $(BUILD_PATH)/c++/%.o64, $(wildcard c++/*.cpp))
OBJECTS64 += $(patsubst windows/%.cpp, $(BUILD_PATH)/windows/%.o64, $(wildcard windows/*.cpp))
OBJECTS64 += $(patsubst windows/controls/%.cpp, $(BUILD_PATH)/windows/controls/%.o64, $(wildcard windows/controls/*.cpp))


all: $(BINARY32) $(BINARY64)

$(BUILD_PATH):
	$(MKDIR) $(BUILD_PATH)
	
$(BUILD_PATH)/plugins:
	$(MKDIR) $(BUILD_PATH)$(SEP)plugins

$(BUILD_PATH)/windows:
	$(MKDIR) $(BUILD_PATH)$(SEP)windows
	
$(BUILD_PATH)/windows/controls:
	$(MKDIR) $(BUILD_PATH)$(SEP)windows$(SEP)controls

$(BUILD_PATH)/c++:
	$(MKDIR) $(BUILD_PATH)$(SEP)c++

$(BUILD_PATH)/%.o32: %.cpp
	$(CXX) -m32 -DMAIN_MODULE $(FEATURES) $(WARNINGS) $(MACROS) $(INCLUDES) -MMD -MP -MT $@ -MF $@.d -o $@ -c $<

$(BUILD_PATH)/%.o64: %.cpp
	$(CXX) $(FEATURES) $(WARNINGS) $(MACROS) $(INCLUDES) -MMD -MP -MT $@ -MF $@.d -o $@ -c $<

$(BINARY32): $(BUILD_PATH) $(BUILD_PATH)/plugins $(BUILD_PATH)/windows $(BUILD_PATH)/c++ $(BUILD_PATH)/windows/controls $(OBJECTS32)
	$(CXX) -m32 $(FEATURES) $(WARNINGS) -o $(BINARY32) $(OBJECTS32) $(LIBS)

$(BINARY64): $(BUILD_PATH) $(BUILD_PATH)/plugins $(BUILD_PATH)/windows $(BUILD_PATH)/c++ $(BUILD_PATH)/windows/controls $(OBJECTS64)
	$(CXX) $(FEATURES) $(WARNINGS) -o $(BINARY64) $(OBJECTS64) $(LIBS)

clean:
	$(RMDIR) $(BUILD_PATH)
	
-include $(wildcard $(BUILD_PATH)/*.o.d)
-include $(wildcard $(BUILD_PATH)/windows/*.o.d)
-include $(wildcard $(BUILD_PATH)/windows/controls/*.o.d)
-include $(wildcard $(BUILD_PATH)/plugins/*.o.d)
-include $(wildcard $(BUILD_PATH)/c++/*.o.d)
