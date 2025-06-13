CC     = g++
TARGET = cedar
FILES  = src/main/common_main.cpp src/main/linux_main.cpp src/io/log.cpp src/io/terminal.cpp src/window.cpp

STD_VERSION = -std=c++20
WARNINGS    = -Wall
DEBUG_MACRO = -D CEDAR_DEBUG

FLAGS       = $(WARNINGS) $(STD_VERSION)
DEBUG_FLAGS = $(WARNINGS) $(STD_VERSION) $(DEBUG_MACRO)

DEBUG_TARGET = $(TARGET)-debug

all: debug release

clean:
	rm -f $(TARGET) $(DEBUG_TARGET)

debug:
	$(CC) -o $(DEBUG_TARGET) $(DEBUG_FLAGS) $(FILES) -lX11

release:
	$(CC) -o $(TARGET) $(FLAGS) $(FILES) -lX11