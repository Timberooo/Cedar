CC     = g++
TARGET = cedar
FILES  = 

STD_VERSION = -std=c++17
WARNINGS    = -Wall
DEBUG_MACRO = -D CEDAR_DEBUG

FLAGS       = $(WARNINGS) $(STD_VERSION)
DEBUG_FLAGS = $(WARNINGS) $(STD_VERSION) $(DEBUG_MACRO)

DEBUG_TARGET = $(TARGET)-debug

all: debug release

clean:
	rm -f $(TARGET) $(DEBUG_TARGET)

debug:
	$(CC) -o $(DEBUG_TARGET) $(DEBUG_FLAGS) $(FILES)

release:
	$(CC) -o $(TARGET) $(FLAGS) $(FILES)