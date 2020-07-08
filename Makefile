build:
	gcc -std=c99 -Wall ./src/*.c -o game -lSDL2
run:
	./game	
clean:
	rm game