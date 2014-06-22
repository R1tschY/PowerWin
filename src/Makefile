NAME = powerwin
SUFFIX = .exe
BUILD_PATH = ..\build

WARNINGS= -Wall -Wuninitialized -Wno-unused-local-typedefs
FEATURES= -O2 -s -std=gnu++0x -mwindows
MACROS  = -DUNICODE
INCLUDES= -I../lib
LIBS    = -L$(BUILD_PATH) -static-libgcc -static-libstdc++ -lpowerwin -mwindows


# compile for Windows XP
MACROS  += -DNTDDI_VERSION=0x05010300 -D_WIN32_WINNT=0x0501 -DWINVER=0x0501 \
          -D_WIN32_IE=0x0700
MACROS  += -DWIN32_LEAN_AND_MEAN


RM      = del
CXX     = g++

BINARY = $(BUILD_PATH)/$(NAME)$(SUFFIX)
OBJECTS = $(patsubst %.cpp, $(BUILD_PATH)/%.o, $(wildcard *.cpp))
# OBJECTS += $(patsubst %.rc, $(BUILD_PATH)/%.o, $(wildcard *.rc))

all: $(BINARY)

$(BUILD_PATH):
	mkdir $(BUILD_PATH)
	
$(BUILD_PATH)\libwinpthread-1.dll:
	copy ..\3th-party\libwinpthread-1.dll $(BUILD_PATH)\libwinpthread-1.dll

####
# ///////(BUILD_PATH).
# 	windres $< $@

$(BUILD_PATH)/%.o: %.cpp
	$(CXX) $(FEATURES) $(WARNINGS) $(MACROS) $(INCLUDES) -MMD -MP -MT $@ -MF $@.d -o $@ -c $<

$(BINARY): $(BUILD_PATH) $(BUILD_PATH)\libwinpthread-1.dll $(OBJECTS)
	$(CXX) $(FEATURES) $(WARNINGS) -o $(BINARY) $(OBJECTS) $(LIBS)

clean:
	rmdir /S /Q $(BUILD_PATH)
	
-include $(wildcard $(BUILD_PATH)/*.o.d)
