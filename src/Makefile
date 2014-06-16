NAME = powerwin
SUFFIX = .exe
BUILD_PATH = ..\build

WARNINGS= -Wall -Wuninitialized -Wno-unused-local-typedefs
FEATURES= -O2 -s -std=gnu++0x -flto -mwindows
# FEATURES= -g -std=gnu++0x
MACROS  = -DUNICODE
INCLUDES= -I../lib -IC:/Users/Richard/include/boost_1_47_0
LIBS    = -L$(BUILD_PATH) -static-libgcc -static-libstdc++ -lpowerwin -mwindows

RM      = del
CXX     = g++

BINARY = $(BUILD_PATH)/$(NAME)$(SUFFIX)
OBJECTS = $(patsubst %.cpp, $(BUILD_PATH)/%.o, $(wildcard *.cpp))

all: $(BINARY)

$(BUILD_PATH):
	mkdir $(BUILD_PATH)

$(BUILD_PATH)/%.o: %.cpp
	$(CXX) $(FEATURES) $(WARNINGS) $(MACROS) $(INCLUDES) -MMD -MP -MT $@ -MF $@.d -o $@ -c $<

$(BINARY): $(BUILD_PATH) $(OBJECTS)
	$(CXX) $(FEATURES) $(WARNINGS) -o $(BINARY) $(OBJECTS) $(LIBS)

clean:
	rmdir /S /Q $(BUILD_PATH)
	
# -include $(wildcard $(BUILD_PATH)/*.o.d)
