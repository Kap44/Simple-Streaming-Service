output: main.o streaming_service.o
	gcc main.o streaming_service.o -o streaming_service

main.o: main.c
	gcc -c main.c

streaming_service.o: streaming_service.c streaming_service.h
	gcc -c streaming_service.c

clean:
	rm *.o streaming_service