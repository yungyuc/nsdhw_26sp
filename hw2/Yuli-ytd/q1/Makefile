CXX = g++

.PHONY: default
default: Line

Line: Line.o Linemain.o
	$(CXX) $^ -o $@

Line.o: Line.cpp Line.hpp
	$(CXX) -c $< -o $@

Linemain.o: Linemain.cpp Line.hpp
	$(CXX) -c $< -o $@

.PHONY: run check clean
run:
	./Line

check:
	./Line  > result.txt

clean:
	rm -rf Line *.o *.txt