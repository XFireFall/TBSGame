//
// Created by airat on 09.03.2020.
//

#include "game.h"

#include <SFML/Graphics.hpp>
#include <iostream>

// check the chat
static std::vector<std::string> m = {};

//const float Game::PlayerSpeed = 500.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() : mWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TBS", sf::Style::Close),
               mFont(),
               mStatisticsText(),
               mStatisticsUpdateTime(),
               mStatisticsNumFrames(0),
               menu_(true),
               playing_(false),

               menu_window_("Media/background_menu_sunset.png",
                            {{
                                     "Media/connect_button.png",
                                     "Media/create_button.png",
                                     "Media/exit_button.png",
                                     "Media/nickname_textbox.png",
                                     "Media/ip_textbox.png"
                             }},
                            {{
                                     {sf::IntRect{{440, 450},
                                                  {200, 50}}},
                                     {sf::IntRect{{440, 520},
                                                  {200, 50}}},
                                     {sf::IntRect{{440, 590},
                                                  {200, 50}}},
                                     {sf::IntRect{{800, 100},
                                                  {250, 30}}},
                                     {sf::IntRect{{30,  100},
                                                  {250, 30}}}
                             }},
                            {{
                                     "Media/Chilanka-Regular.ttf",
                                     "Media/Chilanka-Regular.ttf"
                             }},
                            {
                                    18,
                                    18
                            },
                            {
                                    sf::Color::Black,
                                    sf::Color::Black
                            },
                            {
                                    false,
                                    false
                            },
                            {
                                    "UnnamedPlayer",
                                    "127.0.0.1"
                            }
               ), field_(10, 10),

               field_window_(field_,
                             "Media/disconnect_button.png",
                             {{10,  660},
                              {200, 50}},
                             sf::Vector2f{230, 5},
                             hexagonal_tile(
                                     {},
                                     sf::Vector2f{0, 0},
                                     "Media/Sansation.ttf",
                                     16,
                                     sf::Color::Black,//sf::Color(255, 110, 0),
                                     45,
                                     sf::Color(87, 65, 37),
                                     sf::Color(255, 110, 0),
                                     1
                             )),

               players_list_("Media/players_list.png",
                             {{10,  37},
                              {200, 120}},
                             "Media/Chilanka-Regular.ttf",
                             16,
                             sf::Color::Black,
                             {15, 80},
                             4,
                             15,
                             "Media/button_up.png",
                             {{210, 78},
                              {20,  20}},
                             "Media/button_down.png",
                             {{210, 98},
                              {20,  20}}),

               chat_("Media/chat.png",
                     {{10,  170},
                      {200, 500}},
                       //List
                     "Media/chat_list.png",
                     {{10,  170},
                      {200, 470}},
                     "Media/Chilanka-Regular.ttf",
                     16,
                     sf::Color::Black,
                     {15, 175},
                     25,
                     15,
                     "Media/button_up.png",
                     {{210, 170},
                      {20,  20}},
                     "Media/button_down.png",
                     {{210, 190},
                      {20,  20}},
                       //textbox
                     "Media/chat_textbox.png",
                     {{10,  610},
                      {200, 30}},
                     "Media/Chilanka-Regular.ttf",
                     16,
                     sf::Color::Black,
                     false,
                     "",
                     15,  //non-standard char length
                     true //moving window
               ) {

    // font for fps

    if (!mFont.loadFromFile("Media/Sansation.ttf")) {
        throw std::runtime_error("Cannot load font");
    }

    mStatisticsText.setFont(mFont);
    mStatisticsText.setPosition(0.f, 0.f);
    mStatisticsText.setCharacterSize(10);

    //examples
    //load list of players
    std::vector<gm::player_data> v = {};
    players_list_.setPlayers(v);

    //example
    //load messages

    chat_.loadMessages(m, true);
}


void Game::run() {
    sf::Clock clock{};
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;

            processEvents();
            update();
        }
        updateStatistics(elapsedTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event{};

    while (mWindow.pollEvent(event)) {

        switch (event.type) {
            case sf::Event::MouseButtonPressed:
                handleMouse(event.mouseButton.button);
                break;
            case sf::Event::TextEntered: // (!) goes before keyPressed
                handleEnteredText(event);
                break;

            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;

            case sf::Event::KeyReleased:
                //handlePlayerInput(event.key.code, false);
                break;

            case sf::Event::Closed:
                mWindow.close();
                break;
        }
    }
}

void Game::update() {
    if (playing_) {
        //Important: update the state of the field
        field_window_.updateField_draw(_client.field());
		chat_.loadMessages(_client.msg_queue());
		players_list_.setPlayers(_client.players());
    } else if (menu_) {
        //menu
    }
}

void Game::render() {
    mWindow.RenderWindow::setVerticalSyncEnabled(true);
    mWindow.clear();

    if (playing_) {
        mWindow.draw(field_window_);
        mWindow.draw(players_list_);
        mWindow.draw(chat_);
        mWindow.draw(mStatisticsText);
    } else if (menu_) {
        mWindow.draw(menu_window_);
    }

    mWindow.RenderWindow::display();
}

void Game::updateStatistics(sf::Time elapsedTime) {
    if (playing_) {
        mStatisticsUpdateTime += elapsedTime;
        ++mStatisticsNumFrames;

        if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
            mStatisticsText.setString(
                    "FPS: " + std::to_string(mStatisticsNumFrames) + "\n" +
                    "ping: " + std::to_string(mStatisticsUpdateTime.asMilliseconds() / mStatisticsNumFrames) +
                    "ms"
            );
            mStatisticsUpdateTime -= sf::seconds(1.0f);
            mStatisticsNumFrames = 0;
        }
    } else if (menu_) {
        //menu
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (playing_) {
        //check
        std::string tmp;
        switch (key) {
            case sf::Keyboard::Return:
                if (chat_.textbox.getSelected()) {
                    chat_.textbox.setSelected(false);
                }

                tmp = chat_.getText();
                std::cout << tmp;
                std::cout << std::endl;
                m.push_back(tmp);
//                 chat_.loadMessages(m, true);
				_client.msg_send(tmp);

                break;
            default:;
        }
    } else if (menu_) {
        switch (key) {
            case sf::Keyboard::Return:
                if (menu_window_.textbox_nickname.getSelected()) {
                    menu_window_.textbox_nickname.setSelected(false);
                }

                if (menu_window_.textbox_ip.getSelected()) {
                    menu_window_.textbox_ip.setSelected(false);
                }
                break;

            default:;
        }
    }
}

void Game::handleMouse(sf::Mouse::Button button) {
	static sf::Vector2u prev_selected{static_cast<unsigned int>(field_.height() + 1), static_cast<unsigned int>(field_.width() + 1)};
	static bool prev_valid = false;
	
    if (menu_) {
        switch (button) {
            case sf::Mouse::Left:
                if (menu_window_.button_connect.getDimensions().contains(sf::Mouse::getPosition(mWindow))) {
					std::cout << "Connecting to " << menu_window_.textbox_ip.getText() << " as " << menu_window_.textbox_nickname.getText() << std::endl;
					_client.setNickname(menu_window_.textbox_nickname.getText());
					_client.connect(menu_window_.textbox_ip.getText());
					menu_ = false;
                    playing_ = true;
                }

                if (menu_window_.button_create.getDimensions().contains(sf::Mouse::getPosition(mWindow))) {
                    std::cout << "Creating host as " << menu_window_.textbox_nickname.getText() << std::endl;
					_client.setNickname(menu_window_.textbox_nickname.getText());
					_client.host(10, 10);
					menu_ = false;
                    playing_ = true;
                }

                if (menu_window_.button_exit.getDimensions().contains(sf::Mouse::getPosition(mWindow))) {
                    mWindow.close();
                }

                if (menu_window_.textbox_nickname.getDimensions().contains(sf::Mouse::getPosition(mWindow)) &&
                    !menu_window_.textbox_ip.getSelected()) {
                    menu_window_.textbox_nickname.setSelected(true);
                }

                if (menu_window_.textbox_ip.getDimensions().contains(sf::Mouse::getPosition(mWindow)) &&
                    !menu_window_.textbox_nickname.getSelected()) {
                    menu_window_.textbox_ip.setSelected(true);
                }

                break;

            default:;
        }
    } else if (playing_) {
		try {
			switch (button) {
				case sf::Mouse::Left:
					sf::Vector2u hex_selected = field_window_.hexSelected(sf::Mouse::getPosition(mWindow));

					//acknowledgement
					std::cout << hex_selected << std::endl; // print selected hex
					// inc size of selected cell
					if (hex_selected != sf::Vector2u{static_cast<unsigned int>(field_.width() + 1),
													static_cast<unsigned int>(field_.height() + 1)}) {
	//                  ++field_[hex_selected]._size;
						
						switch (_client.getPhase()) {
							case gm::ATTACK:
								if (prev_valid) {
									std::cout << "Attack" << std::endl;
									_client.attack(prev_selected, hex_selected);
									prev_valid = false;
									field_window_.deselect(prev_selected);
								} else {
									prev_selected = hex_selected;
									prev_valid = true;
									field_window_.select(prev_selected);
								}
								break;
								
							case gm::FEED:
								std::cout << "Feed" << std::endl;
								_client.feed(hex_selected);
								break;
								
							default:
								std::cout << "Wait for your turn!" << std::endl;
								break;
						}
					} else
					//

					if (field_window_.button_phase.getDimensions().contains(sf::Mouse::getPosition(mWindow))) {
						_client.nextPhase();
						if (prev_valid)
							field_window_.deselect(prev_selected);
					}
					
					if (field_window_.button_disconnect.getDimensions().contains(sf::Mouse::getPosition(mWindow))) {
						_client.disconnect();
						menu_ = true;
						playing_ = false;
						if (prev_valid)
							field_window_.deselect(prev_selected);
						prev_valid = false;
					}

					if (players_list_.button_up.getDimensions().contains(sf::Mouse::getPosition(mWindow))) {
						players_list_.shiftDisplayedStrUp();
					}

					if (players_list_.button_down.getDimensions().contains(sf::Mouse::getPosition(mWindow))) {
						players_list_.shiftDisplayedStrDown();
					}

					if (chat_.textbox.getDimensions().contains(sf::Mouse::getPosition(mWindow))) {
						chat_.textbox.setSelected(true);
					}

					if (chat_.msg_list.button_up.getDimensions().contains(sf::Mouse::getPosition(mWindow))) {
						chat_.msg_list.shiftDisplayedStrUp();
					}

					if (chat_.msg_list.button_down.getDimensions().contains(sf::Mouse::getPosition(mWindow))) {
						chat_.msg_list.shiftDisplayedStrDown();
					}
					
					break;
			}
		} catch (Client::ex x) {
			std::cerr << "Game over!" << std::endl;
			_client.disconnect();
			playing_ = false;
			menu_ = true;
		}
    }
}

void Game::handleEnteredText(sf::Event event) {
    if (menu_) {
        if (menu_window_.textbox_nickname.getSelected()) {
            menu_window_.textbox_nickname.typedOn(event);
        }

        if (menu_window_.textbox_ip.getSelected()) {
            menu_window_.textbox_ip.typedOn(event);
        }
    } else if (playing_) {
		chat_.textbox.setSelected(true);
        if (chat_.textbox.getSelected()) {
            chat_.textbox.typedOn(event);
        }
    }
}
