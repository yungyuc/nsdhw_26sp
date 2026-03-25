CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = line

all: $(TARGET)
$(TARGET): main.o Line.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp Line.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

check: $(TARGET)
	./$(TARGET) > result.txt

clean:
	rm -f $(TARGET) *.o result.txt
