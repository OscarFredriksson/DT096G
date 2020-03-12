SRC = $(wildcard *.cpp)
TARGET = main

FLAGS = -std=c++17 -o

$(TARGET): $(SRC)
	g++ $^ $(FLAGS) $@

run: $(TARGET)
	./$(TARGET)   