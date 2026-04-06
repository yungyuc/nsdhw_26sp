all:
	mkdir -p build
	cd build && cmake .. && make
	cp build/_matrix*.so .

test: all
	pytest test_matrix.py -v

clean:
	rm -rf build
	rm -f _matrix*.so
	rm -rf __pycache__
	rm -f performance.txt