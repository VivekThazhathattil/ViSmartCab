build:	./*.cpp
	g++ -g -std=c++17 -c ./testEnv.cpp ../src/*.cpp
	mkdir -p obj && mv ./*.o ./obj
	rm ./obj/main.o
	g++ -g ./obj/*.o -o main -lsfml-graphics -lsfml-window -lsfml-system
	chmod 755 ./main
	./main
