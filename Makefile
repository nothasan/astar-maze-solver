compile: ./main.cpp
	g++ -c -g ./main.cpp ./files/astar.cpp -I. -Iheaders
	g++ main.o astar.o -o app -lsfml-graphics -lsfml-window -lsfml-system

run:
	./app

clean:
	rm -f main.o astar.o app
	rm -f *.o