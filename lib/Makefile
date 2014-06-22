NAME = libpowerwin
SUFFIX = .dll
BUILD_PATH = ..\build

WARNINGS= -Wall -Wuninitialized -Wno-unused-local-typedefs
FEATURES= -shared -O2 -s -std=gnu++0x
# FEATURES= -shared -g -std=gnu++0x  -fvisibility=hidden -fvisibility-inlines-hidden -finline-functions
MACROS  = -DUNICODE -DBUILD_DLL -DPTW32_BUILD_INLINED -DPTW32_STATIC_LIB
INCLUDES= -I.. -IC:/include/boost_1_55_0
LIBS    = -static-libgcc -static-libstdc++ -lwsock32 -static -lpthread

# compile for Windows XP
MACROS  += -DNTDDI_VERSION=0x05010300 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 \
          -D_WIN32_IE=0x0700
MACROS  += -DWIN32_LEAN_AND_MEAN


RM      = del
CXX     = g++

BINARY = $(BUILD_PATH)/$(NAME)$(SUFFIX)
OBJECTS = $(patsubst %.cpp, $(BUILD_PATH)/%.o, $(wildcard *.cpp))
OBJECTS += $(patsubst plugins/%.cpp, $(BUILD_PATH)/plugins/%.o, $(wildcard plugins/*.cpp))
OBJECTS += $(patsubst c++/%.cpp, $(BUILD_PATH)/c++/%.o, $(wildcard c++/*.cpp))
OBJECTS += $(patsubst windows/%.cpp, $(BUILD_PATH)/windows/%.o, $(wildcard windows/*.cpp))

all: $(BINARY)

$(BUILD_PATH):
	mkdir $(BUILD_PATH)
	
$(BUILD_PATH)/plugins:
	mkdir $(BUILD_PATH)\plugins

$(BUILD_PATH)/windows:
	mkdir $(BUILD_PATH)\windows

$(BUILD_PATH)/c++:
	mkdir $(BUILD_PATH)\c++

$(BUILD_PATH)/%.o: %.cpp
	$(CXX) $(FEATURES) $(WARNINGS) $(MACROS) $(INCLUDES) -MMD -MP -MT $@ -MF $@.d -o $@ -c $<

$(BINARY): $(BUILD_PATH) $(BUILD_PATH)/plugins $(BUILD_PATH)/windows $(BUILD_PATH)/c++ $(OBJECTS)
	$(CXX) $(FEATURES) $(WARNINGS) -o $(BINARY) $(OBJECTS) $(LIBS)

clean:
	rmdir /S /Q $(BUILD_PATH)
	
-include $(wildcard $(BUILD_PATH)/*.o.d)
-include $(wildcard $(BUILD_PATH)/windows/*.o.d)
-include $(wildcard $(BUILD_PATH)/plugins/*.o.d)
-include $(wildcard $(BUILD_PATH)/c++/*.o.d)
