default:
	gcc -o main source/main.c -std=c99 -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

run:
	gcc -o main source/main.c -std=c99 -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
	main