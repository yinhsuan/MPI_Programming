CXX=mpicxx
SRC=main.cc
SRC+= matrix.cc
EXE=matmul

OBJ=${SRC:.cc=.o}

${EXE}: ${OBJ}
	${CXX} -o $@ $^

%.o: %.cc
	${CXX} -c $^ -o $@

.PHONY: clean

clean:
	rm ${OBJ} ${EXE}