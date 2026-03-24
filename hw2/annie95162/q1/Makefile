CXX := g++
CXXFLAGS := -std=c++11 -O2 -Wall -Wextra

PROGRAM := main
SOURCE := main.cpp
OBJECT := main.o

all: $(PROGRAM)

# build executable
$(PROGRAM): $(OBJECT)
	$(CXX) $(CXXFLAGS) $(OBJECT) -o $(PROGRAM)

# compile cpp to object file
$(OBJECT): $(SOURCE)
	$(CXX) $(CXXFLAGS) -c $(SOURCE) -o $(OBJECT)

run: $(PROGRAM)
	./$(PROGRAM)

check: $(PROGRAM)
	./$(PROGRAM) > result.txt

clean:
	rm -f $(PROGRAM) $(OBJECT) result.txt

.PHONY: all run check clean
