CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wall

TARGET = main
OBJS = main.o Line.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

check: $(TARGET)
	./$(TARGET) > result.txt

clean:
	rm -f $(OBJS) $(TARGET) result.txt

.PHONY: all run check clean
