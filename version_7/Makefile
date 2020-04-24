all:
	g++ game-no-graphics.cpp lib/prim/Prim_common.cpp lib/prim/Prim_game.cpp lib/prim/Prim_network.cpp lib/prim/Prim_system.cpp lib/Client.cpp -lsfml-graphics -lsfml-network -lsfml-system -o game-no-graphics


	g++ host.cpp lib/prim/Prim_common.cpp lib/prim/Prim_game.cpp lib/prim/Prim_network.cpp lib/prim/Prim_system.cpp lib/Server.cpp -lsfml-graphics -lsfml-network -lsfml-system -o host

	g++ game.cpp lib/prim/Prim_common.cpp lib/prim/Prim_game.cpp lib/prim/Prim_network.cpp lib/prim/Prim_system.cpp lib/Client.cpp -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window -o game

clean:
	rm game-no-graphics host game
