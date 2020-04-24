CC = g++

LIBS = -lsfml-network -lsfml-system -lsfml-graphics

PRIM = Primitives_common.cpp Primitives_game.cpp Primitives_network.cpp Primitives_system.cpp
PRIM_OBJS = $(PRIM:%.cpp=obj/%.o)

all:
	$(CC) lib/prim/Primitives_common.cpp -c -o obj/Primitives_common.o $(LIBS)
	$(CC) lib/prim/Primitives_game.cpp -c -o obj/Primitives_game.o $(LIBS)
	$(CC) lib/prim/Primitives_network.cpp -c -o obj/Primitives_network.o $(LIBS)
	$(CC) lib/prim/Primitives_system.cpp -c -o obj/Primitives_system.o $(LIBS)
	
	$(CC) lib/Server.cpp -c -o obj/Server.o $(LIBS)
	$(CC) lib/Client.cpp -c -o obj/Client.o $(LIBS)
	
	$(CC) host.cpp -c -o obj/host.o $(LIBS)
	$(CC) game-no-graphics.cpp -c -o obj/game-no-graphics.o $(LIBS)
	
	$(CC) obj/host.o obj/Server.o $(PRIM_OBJS) -o host $(LIBS)
	$(CC) obj/game-no-graphics.o obj/Client.o $(PRIM_OBJS) -o game-no-graphics $(LIBS)
	
clean:
	rm obj/*.o
