NAME = libpowerwin
SUFFIX = .dll
BUILD_PATH = ..\build

WARNINGS= -Wall -Wuninitialized -Wno-unused-local-typedefs
FEATURES= -shared -O2 -s -std=gnu++0x -fvisibility=hidden -fvisibility-inlines-hidden -finline-functions
# FEATURES= -shared -g -std=gnu++0x
MACROS  = -DUNICODE -DBUILD_DLL
INCLUDES= -I.. -IC:/include/boost_1_55_0
LIBS    = -static-libgcc -static-libstdc++

# compile for Windows XP
MACROS  += -DNTDDI_VERSION=0x05010300 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 \
          -D_WIN32_IE=0x0700
MACROS  += -DWIN32_LEAN_AND_MEAN

CXX     = g++

BINARY32 = $(BUILD_PATH)/$(NAME)32$(SUFFIX)
OBJECTS32 = $(patsubst %.cpp, $(BUILD_PATH)/%.o32, $(wildcard *.cpp))
OBJECTS32 += $(patsubst plugins/%.cpp, $(BUILD_PATH)/plugins/%.o32, $(wildcard plugins/*.cpp))
OBJECTS32 += $(patsubst c++/%.cpp, $(BUILD_PATH)/c++/%.o32, $(wildcard c++/*.cpp))
OBJECTS32 += $(patsubst windows/%.cpp, $(BUILD_PATH)/windows/%.o32, $(wildcard windows/*.cpp))

BINARY64 = $(BUILD_PATH)/$(NAME)64$(SUFFIX)
OBJECTS64 = $(patsubst %.cpp, $(BUILD_PATH)/%.o64, $(wildcard *.cpp))
OBJECTS64 += $(patsubst plugins/%.cpp, $(BUILD_PATH)/plugins/%.o64, $(wildcard plugins/*.cpp))
OBJECTS64 += $(patsubst c++/%.cpp, $(BUILD_PATH)/c++/%.o64, $(wildcard c++/*.cpp))
OBJECTS64 += $(patsubst windows/%.cpp, $(BUILD_PATH)/windows/%.o64, $(wildcard windows/*.cpp))

all: $(BINARY32) $(BINARY64)

$(BUILD_PATH):
	mkdir $(BUILD_PATH)
	
$(BUILD_PATH)/plugins:
	mkdir $(BUILD_PATH)\plugins

$(BUILD_PATH)/windows:
	mkdir $(BUILD_PATH)\windows

$(BUILD_PATH)/c++:
	mkdir $(BUILD_PATH)\c++

$(BUILD_PATH)/%.o32: %.cpp
	$(CXX) -m32 -DMAIN_MODULE $(FEATURES) $(WARNINGS) $(MACROS) $(INCLUDES) -MMD -MP -MT $@ -MF $@.d -o $@ -c $<

$(BUILD_PATH)/%.o64: %.cpp
	$(CXX) $(FEATURES) $(WARNINGS) $(MACROS) $(INCLUDES) -MMD -MP -MT $@ -MF $@.d -o $@ -c $<

$(BINARY32): $(BUILD_PATH) $(BUILD_PATH)/plugins $(BUILD_PATH)/windows $(BUILD_PATH)/c++ $(OBJECTS32)
	$(CXX) -m32 $(FEATURES) $(WARNINGS) -o $(BINARY32) $(OBJECTS32) $(LIBS)

$(BINARY64): $(BUILD_PATH) $(BUILD_PATH)/plugins $(BUILD_PATH)/windows $(BUILD_PATH)/c++ $(OBJECTS64)
	$(CXX) $(FEATURES) $(WARNINGS) -o $(BINARY64) $(OBJECTS64) $(LIBS)

clean:
	rmdir /S /Q $(BUILD_PATH)
	
-include $(wildcard $(BUILD_PATH)/*.o.d)
-include $(wildcard $(BUILD_PATH)/windows/*.o.d)
-include $(wildcard $(BUILD_PATH)/plugins/*.o.d)
-include $(wildcard $(BUILD_PATH)/c++/*.o.d)
