CXX = g++
CXXFLAGS = -O3 -Wall -Wextra -std=c++17 -Isrc
TARGET = spof_audit
SRC = src/Graph.cpp src/Parser.cpp src/TarjanSPOF.cpp src/main.cpp
OBJ = $(SRC:.cpp=.o)

GRAFO ?= data/amostra-grafo.txt

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) $(GRAFO)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run clean
