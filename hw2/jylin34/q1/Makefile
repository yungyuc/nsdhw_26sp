CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
TARGET = line_program

all: $(TARGET)
$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)
run: $(TARGET)
	./$(TARGET)
check: $(TARGET)
	./$(TARGET) > result.txt
clean:
	rm -f $(TARGET) *.o result.txt *.out

