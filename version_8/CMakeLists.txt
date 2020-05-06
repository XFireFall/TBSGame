cmake_minimum_required(VERSION 3.13)
project(TBS)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

add_executable(TBS main.cpp
					Lib/frontend/game.h             Lib/frontend/game.cpp
					Lib/frontend/Menu.h             Lib/frontend/Menu.cpp 
					Lib/frontend/Button.h           Lib/frontend/Button.cpp 
					Lib/frontend/textbox.h          Lib/frontend/textbox.cpp
					Lib/frontend/hexagonal_tile.h   Lib/frontend/hexagonal_tile.cpp
					Lib/frontend/List.h             Lib/frontend/List.cpp
					Lib/frontend/Chat.h             Lib/frontend/Chat.cpp
					Lib/frontend/Field_draw.h       Lib/frontend/Field_draw.cpp
					
					Lib/primitives/Prim_game.hpp    Lib/primitives/Prim_game.cpp
					Lib/primitives/Prim_common.hpp  Lib/primitives/Prim_common.cpp
					Lib/primitives/Prim_system.hpp  Lib/primitives/Prim_system.cpp
					Lib/primitives/Prim_network.hpp Lib/primitives/Prim_network.cpp
					
					Lib/backend/Client.hpp          Lib/backend/Client.cpp
					)

add_executable(TBS-console console.cpp
					Lib/primitives/Prim_game.hpp    Lib/primitives/Prim_game.cpp
					Lib/primitives/Prim_common.hpp  Lib/primitives/Prim_common.cpp
					Lib/primitives/Prim_system.hpp  Lib/primitives/Prim_system.cpp
					Lib/primitives/Prim_network.hpp Lib/primitives/Prim_network.cpp
					
					Lib/backend/Client.hpp          Lib/backend/Client.cpp
					)

add_executable(host host.cpp
					Lib/primitives/Prim_game.hpp    Lib/primitives/Prim_game.cpp
					Lib/primitives/Prim_common.hpp  Lib/primitives/Prim_common.cpp
					Lib/primitives/Prim_system.hpp  Lib/primitives/Prim_system.cpp
					Lib/primitives/Prim_network.hpp Lib/primitives/Prim_network.cpp
					
					Lib/backend/Server.hpp          Lib/backend/Server.cpp
					)

target_link_libraries(TBS sfml-graphics
		sfml-audio
		sfml-window
		sfml-system
		sfml-network)

target_link_libraries(TBS-console sfml-graphics
		sfml-system
		sfml-network)

target_link_libraries(host sfml-graphics
		sfml-system
		sfml-network)
