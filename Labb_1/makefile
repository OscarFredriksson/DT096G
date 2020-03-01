SRC = $(wildcard *.cpp)
TARGET = main

FLAGS = -g -std=c++17 -o

clean: 
	rm -f $(TARGET)

$(TARGET): $(SRC)
	g++ $^ $(FLAGS) $@

run: $(TARGET)
	./$(TARGET)   

rebuild: 
	$(MAKE) clean 
	$(MAKE) run


