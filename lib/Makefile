NAME = libpowerwin
SUFFIX = .dll
BUILD_PATH = ..\build

WARNINGS= -Wall -Wuninitialized -Wno-unused-local-typedefs
FEATURES= -shared -O2 -s -std=gnu++0x -flto -fvisibility=hidden -fvisibility-inlines-hidden
# FEATURES= -shared -g -std=gnu++0x  -fvisibility=hidden -fvisibility-inlines-hidden
MACROS  = -DUNICODE -DBUILD_DLL -DPTW32_BUILD_INLINED -DPTW32_STATIC_LIB
INCLUDES= -I.. -IC:/Users/Richard/include/boost_1_47_0
LIBS    = -static-libgcc -static-libstdc++ -finline-functions -lwsock32 -static -lpthread

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
	mkdir $(BUILD_PATH)\plugins \windows \c++

$(BUILD_PATH)/%.o: %.cpp
	$(CXX) $(FEATURES) $(WARNINGS) $(MACROS) $(INCLUDES) -MMD -MP -MT $@ -MF $@.d -o $@ -c $<

$(BINARY): $(BUILD_PATH) $(BUILD_PATH)/plugins $(OBJECTS)
	$(CXX) $(FEATURES) $(WARNINGS) -o $(BINARY) $(OBJECTS) $(LIBS)

clean:
	rmdir /S /Q $(BUILD_PATH)
	
# -include $(wildcard $(BUILD_PATH)/*.o.d)
