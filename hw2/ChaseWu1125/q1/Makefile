CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = main
OBJS = main.o Line.o

.PHONY: all clean run check

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
%.o: %.cpp Line.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
run: $(TARGET)
	./$(TARGET)
check: $(TARGET)
	./$(TARGET) > result.txt
clean:
	rm -f $(TARGET) $(OBJS) result.txt