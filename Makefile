CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC = ./src/*.cpp
OUT = ./build/enigma

all:
	mkdir -p ./build
	$(CXX) -std=c++17 $(SRC) -o $(OUT)

debug:
	mkdir -p ./build
	$(CXX) $(CXXFLAGS) -DDEBUG -g $(SRC) -o ./build/enigma_debug

run:
	$(OUT)

clean:
	rm -f ./build/*