build:	./src/*.cpp
	g++ -g -std=c++17 -c ./src/*.cpp
	mkdir -p obj && mv ./*.o ./obj
	g++ -g ./obj/*.o -o main -lsfml-graphics -lsfml-window -lsfml-system
	chmod 755 ./main
	./main
