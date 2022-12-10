all: chess

chess: src/*
	g++ src/*.cc -lglut -lGL -lGLU -o chess

.PHONY: all clean

clean:
	rm -f chess
