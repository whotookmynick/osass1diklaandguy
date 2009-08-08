CPP = $(wildcard *.cpp)
HPP = $(wildcard *.h)
OBJ = $(CPP:.cpp=.o)
EXE = main
.PHONEY: all clean
all: $(EXE)
$(EXE): $(OBJ)
	g++ -g -o $(EXE) $(OBJ) -lpthread
%.o: %.cpp $(HPP)
	g++ -g -I. -c $<
clean:
	rm -f $(OBJ)
