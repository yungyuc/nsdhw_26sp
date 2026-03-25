CXX	=	g++

EXECUTABLE	=	run.exe

SRCS	=	container.cpp

$(EXECUTABLE):	$(SRCS)
	$(CXX)	$(SRCS)	-o	$(EXECUTABLE)

run:	$(EXECUTABLE)
	chmod	+x	$@.exe
	./$(EXECUTABLE)

check:	$(EXECUTABLE)
	./$(EXECUTABLE)>result.txt
clean:
	rm	-f	$(EXECUTABLE)	result.txt
