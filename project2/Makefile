# Source and target
# Automatically include all .cpp files under src/ and one-level subdirectories
SRCS = $(wildcard src/*.cpp src/*/*.cpp)
TARGET = raylib_app
OBJS = $(SRCS:src/%.cpp=build/%.o)
DEPS = $(OBJS:.o=.d)

# OS detection (macOS = Darwin, Windows via MinGW = MINGW*)
UNAME_S := $(shell uname -s)

# Default values
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -MMD -MP

# Raylib configuration using pkg-config
RAYLIB_CFLAGS = $(shell pkg-config --cflags raylib)
RAYLIB_LIBS = $(shell pkg-config --libs raylib)

ifeq ($(UNAME_S), Darwin)
	# macOS configuration
	CXXFLAGS += -arch arm64 $(RAYLIB_CFLAGS)
	LIBS = $(RAYLIB_LIBS) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	EXEC = ./$(TARGET)
else ifneq (,$(findstring MINGW,$(UNAME_S)))
	# Windows configuration (assumes raylib in C:/raylib)
	CXXFLAGS += -IC:/raylib/include
	LIBS = -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
	TARGET := $(TARGET).exe
	EXEC = ./$(TARGET)
else
	# Linux/WSL fallback
	CXXFLAGS += $(RAYLIB_CFLAGS)
	LIBS = $(RAYLIB_LIBS) -lGL -lm -lpthread -ldl -lrt -lX11
	EXEC = ./$(TARGET)
endif

# Default target: link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Include dependency files so header changes trigger recompile
-include $(DEPS)

# Ensure build dirs exist (order-only for .o rule)
build:
	@mkdir -p build build/util

# Compile .cpp to .o; generate .d with header deps
build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean rule
clean:
	@rm -rf build
	@if [ -f "$(TARGET)" ]; then rm -f $(TARGET); fi
	@if [ -f "$(TARGET).exe" ]; then rm -f $(TARGET).exe; fi

# Run rule
run: $(TARGET)
	$(EXEC)
