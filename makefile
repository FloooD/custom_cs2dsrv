all:
	gcc -O3 -lm -lrt -I"include" -I"lua/src" src/*.c -L. -llua -o bin/custom_cs2dsrv

no-shared:
	gcc -O3 -lm -lrt -I"include" -I"lua/src" src/*.c lua/src/*.c -o bin/custom_cs2dsrv

clang:
	clang -lm -lrt -lz -I"include" -I"lua/src" -D"USE_ZLIB" src/*.c lua/src/*.c -o bin/clang_srv
