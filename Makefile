all:
	g++ -o main snake.cpp -lcurses
clean:
	rm -f main
run: all
	./main