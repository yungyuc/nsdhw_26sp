CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = line
SRC = line.cpp

.PHONY: all run check clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

check: $(TARGET)
	./$(TARGET) > result.txt

clean:
	rm -f $(TARGET) result.txt
	find . -executable -type f ! -name "*.sh" | xargs -r chmod -x