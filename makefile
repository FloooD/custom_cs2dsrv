all:
	gcc -O3 -lm -lrt -I"include" -I"lua/src" src/*.c -o bin/custom_cs2dsrv
