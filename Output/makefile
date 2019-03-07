Game: IOFile.o maze.o
	g++ -o Game IOFile.o game.o -w -no-pie -fPIC -lSDL2

IOFile.o: IOFile.cpp
	g++ -c IOFile.cpp -I.

game.o: game.ll
	llc -filetype=obj game.ll
