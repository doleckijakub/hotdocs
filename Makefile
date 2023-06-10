CPP := clang++
CPPFLAGS := -Wall -Wextra -pedantic -std=c++17
LDFLAGS :=

EXEC := hotdocs

ifeq ($(OS),Windows_NT)
	WINDOWS := 1
else ifeq ($(shell uname),Linux)
	LINUX := 1
else ifeq ($(shell uname),Darwin)
	MACOS := 1
else
	$(error Unsupported platform: $(shell uname))
endif


ifeq ($(WINDOWS),1)
	EXEC := $(EXEC).exe
else ifeq ($(LINUX),1)
	
else ifeq ($(MACOS),1)

else
	$(error unreachable)
endif

all: $(EXEC)

$(EXEC): *.cpp
	$(CPP) $(CPPFLAGS) -o $(EXEC) *.cpp $(LDFLAGS)