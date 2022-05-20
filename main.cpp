#include <SFML/Audio.hpp>
#include "Assets/CLASSES - HEADER/Player.h"
#include "Assets/CLASSES - HEADER/Highscore.h"
#include "Assets/CLASSES - HEADER/EnemyList.h"

// Public variable for checking the pause, game ending and leaderboard status
bool isPaused = false, gameEnd = false, leaderboard = false;
// Pubilc path so i don't keep writing the same string
const std::string path = "Assets/";
// To input your username
std::string vnos;
// Functions 
void finishGame(Player* player, sf::Sprite* background);
void reset(Player* dondola, sf::Sprite* background);


//# === MAIN FUNCTION ===
int main() {
    srand(time(NULL));
    // std::cout << "Hello World!\n(program started)\n";

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(1280,720),"Igra zicnice ali Dondola", sf::Style::Titlebar | sf::Style::Close);
    sf::Image icon;
        icon.loadFromFile(path + "icon.png");
        window->setIcon(32,32,icon.getPixelsPtr());
        window->setFramerateLimit(60);
        window->setMouseCursorVisible(0);

    //! WILL READ EVENTS FROM KEYBOARD OR MOUSE
    sf::Event ev; 
    
    //!? LOADING TEXTURES
    sf::Texture t_map, t_bg;
    if (!t_map.loadFromFile(path + "Texture-Map-3.png")) { 
        std::cout << "Failed Loading " << '"' << "Texture-map-3.png" << '"' << "\tCheck files\n"; 
        system("pause"); 
        return EXIT_FAILURE; 
    }
    if (!t_bg.loadFromFile(path + "Background.png")) { 
        std::cout << "Failed Loading "<< '"' << "Background.png" << '"' << "\tCheck files\n"; 
        system("pause");
        return EXIT_FAILURE; 
    }
    // So I don't have to worry about player seing outside the game
    t_bg.setRepeated(true); 

    //# added Game Paused + timer text
    std::string s_UI;
    sf::Font font;
    if (!font.loadFromFile(path + "VCR_OSD_MONO_1.001.ttf")) {
        std::cout << "Failed Loading " << '"' << "VCR_OSD_MONO_1.001.ttf" << '"' << "\tCheck files\n"; 
        system("pause");  
        return EXIT_FAILURE;
    }
    sf::Text t_UI;
        t_UI.setFont(font);
        t_UI.setOutlineColor(sf::Color::Black);
        t_UI.setOutlineThickness(2.5);
        t_UI.setCharacterSize(36);
    
    //!? DEFINING PLAYER AND NON MOVING/CHANGING STUFF
    Player* dondola = new Player(t_map,1,1);  //# The Player
        dondola->setTextureRect(sf::IntRect(0, 0, 128, 128));
        dondola->setTexture(t_map);
        dondola->setPosition(0, 120);
       
    sf::Sprite* wire = new sf::Sprite(t_map); //# Wire goes from one end to the other
        wire->setTextureRect(sf::IntRect(264, 52, 128, 5));
        wire->setScale(20, 2);
        wire->setRotation(16); //x rad * M_PI / 180 //rotation is in degress anyway lol
        wire->setPosition(0, 120);
    
    sf::Sprite* BG = new sf::Sprite(t_bg); //# Background
        BG->setTextureRect(sf::IntRect(0, 0, 10000, 1500));
        BG->setScale(1.4f, 1.4f);
    
    //# Starts the function to spawn enemies/rockets
        if (rockets::begin() != 0) { std::cout << "Couldn't run list begin() function\n"; system("pause"); return EXIT_FAILURE; }
        //sf::Clock counter_fps;
    //? === Game loop while the game is open ===
    while (window->isOpen()) {
        if (!isPaused && !gameEnd) {
            s_UI = " Elapsed time: " + 
            std::to_string((int)dondola->getClockElapsed()) + 
            "\n Health: " + 
            std::to_string(dondola->getHealth());
            // Before entering the username
            leaderboard = false;
        }
        t_UI.setString(s_UI);
        sf::Vector2f e_movemet;
        while (window->pollEvent(ev)) {
            // Checking for focus before switching the type of event
            if (window->hasFocus()) {
                // Checking if i am writing my username or playing
                if (!gameEnd) {
                    switch (ev.type) {
                    case sf::Event::Closed:
                        window->close();
                        break;                 
                    //#  === Keyboard key check ===
                    case sf::Event::KeyPressed:
                        switch (ev.key.code) {
                        case sf::Keyboard::Escape:
                            window->close();
                            break;
                        /* //# Debug to see collision in effect -> spawn a rocket above player's head
                        case sf::Keyboard::S:
                            rockets::setAbove(dondola);
                            break;
                        */
                            //? Moving Right (Forward)
                        case sf::Keyboard::D:
                            if (!isPaused) {  
                                double percent_d = dondola->getPercent();
                                dondola->setPercent(percent_d + 1);
                               /* //? Debug
                               std::cout << "Dondola position at:" << percent_d << " or " <<
                                    dondola->getPosition().x << ", " << dondola->getPosition().y << "\n";*/
                                // Limit
                                if (percent_d > 100) dondola->setPercent(100);

                                //# Start of map
                                if (percent_d <= 6) {
                                    dondola->move(64, 18);
                                }
                                else {
                                    //# Somewhere in the middle
                                    if (percent_d > 6 && percent_d < 80) {
                                        BG->move(-64, -18);
                                        e_movemet.x = -64;
                                    }
                                    //# End of the map
                                    else {
                                        if (percent_d >= 80 && dondola->getPosition().x < 1152) {
                                            dondola->move(64, 18);
                                        }
                                    }
                                }
                            }
                            break;

                            //? Moving left (Back)
                        case sf::Keyboard::A:
                            if (!isPaused) {
                                double percent_d = dondola->getPercent();
                                dondola->setPercent(percent_d - 1);
                                /* //? Debug
                                std::cout << "Dondola position at:" << percent_d << " or " <<
                                     dondola->getPosition().x << ", " << dondola->getPosition().y << "\n";*/
                                // Limit
                                if (percent_d < 0) dondola->setPercent(0);

                                //# Start of map
                                if (percent_d <= 7 && dondola->getPosition().x >= 64) {
                                    dondola->move(-64, -18);
                                }
                                else {
                                    //# Somewhere in the middle
                                    if (percent_d > 7 && percent_d < 90) {
                                        BG->move(64, 18);
                                        e_movemet.x = 64;
                                    }
                                    //# End of the map
                                    else {
                                        if (percent_d >= 90) {
                                            dondola->move(-64, -18);
                                        }
                                    }
                                }
                                // Debug
                                //std::cout << "Dondola percent is " << dondola->getPercent() << "\n";
                                if (BG->getPosition().y > 0) BG->setPosition(0, 0);
                            }
                            break;

                        case sf::Keyboard::P:
                            if (!gameEnd) {
                                isPaused = !isPaused;
                                if (isPaused) {
                                    std::cout << "Player paused the game!\nAt location " << dondola->getPercent() << "\n";
                                    s_UI = "Game paused!";
                                    dondola->pauseClock();
                                    rockets::clocks_pause();
                                }
                                else {
                                    std::cout << "Player unpaused the game!\n";
                                    rockets::clocks_reset();
                                    dondola->resetClock();
                                }
                                t_UI.setString(s_UI);
                            }
                            break;

                        // Restart/reset the game
                        case sf::Keyboard::R:
                            if (gameEnd) {
                                s_UI = "Game reset!";
                                reset(dondola, BG);
                            }
                            else std::cout << "cannot reset if the game hasn't finished oops!\n";
                             break;

                        case sf::Keyboard::K:
                            s_UI = "you died";
                            dondola->setHealth(-1);
                            break;
                        } 
                    }
                }
                else {
                    switch (ev.type) {
                    case sf::Event::Closed:
                        window->close();
                        break;
                    case sf::Event::KeyPressed: {
                        switch (ev.key.code) {
                        case sf::Keyboard::Escape:
                            if (vnos.length() > 0) {
                                scoreList->write_to_file();
                            }
                            reset(dondola, BG);
                            break;
                            if (!isPaused) {
                            case sf::Keyboard::A:
                                vnos += "A";
                                break;
                            case sf::Keyboard::B:
                                vnos += "B";
                                break;
                            case sf::Keyboard::C:
                                vnos += "C";
                                break;
                            case sf::Keyboard::D:
                                vnos += "D";
                                break;
                            case sf::Keyboard::E:
                                vnos += "E";
                                break;
                            case sf::Keyboard::F:
                                vnos += "F";
                                break;
                            case sf::Keyboard::G:
                                vnos += "G";
                                break;
                            case sf::Keyboard::H:
                                vnos += "H";
                                break;
                            case sf::Keyboard::I:
                                vnos += "I";
                                break;
                            case sf::Keyboard::J:
                                vnos += "J";
                                break;
                            case sf::Keyboard::K:
                                vnos += "K";
                                break;
                            case sf::Keyboard::L:
                                vnos += "L";
                                break;
                            case sf::Keyboard::M:
                                vnos += "M";
                                break;
                            case sf::Keyboard::N:
                                vnos += "N";
                                break;
                            case sf::Keyboard::O:
                                vnos += "O";
                                break;
                            case sf::Keyboard::P:
                                vnos += "P";
                                break;
                            case sf::Keyboard::Q:
                                vnos += "Q";
                                break;
                            case sf::Keyboard::R:
                                vnos += "R";
                                break;
                            case sf::Keyboard::S:
                                vnos += "S";
                                break;
                            case sf::Keyboard::T:
                                vnos += "T";
                                break;
                            case sf::Keyboard::U:
                                vnos += "U";
                                break;
                            case sf::Keyboard::V:
                                vnos += "V";
                                break;
                            case sf::Keyboard::W:
                                vnos += "W";
                                break;
                            case sf::Keyboard::X:
                                vnos += "X";
                                break;
                            case sf::Keyboard::Y:
                                vnos += "Y";
                                break;
                            case sf::Keyboard::Z:
                                vnos += "Z";
                                break;
                            case sf::Keyboard::Backspace:
                                if (vnos.length()) vnos.erase(vnos.length() - 1);
                                else std::cout << "empty input\n";
                                break;
                            case sf::Keyboard::Enter:
                                isPaused = true;
                                //std::cout << "Input ended :)\n";
                                break;
                            }

                        }

                    }
                        
                    }// end of switch ev type
                }// End of Else
            }
        }
        //! Update ######################################################################################################################################
        rockets::update(gameEnd, isPaused, e_movemet,dondola->getPosition().x, 20.f + 0.01f * dondola->getClockElapsed());

        if (!isPaused && !gameEnd) {
            if (rockets::getEnemyCollision(dondola)) {
                dondola->getHit();
                std::cout << "Dondola got hit by a rocket!\n" <<
                    "New health : " << dondola->getHealth() << "\n";
            }
            if (dondola->getHealth() < 0) { 
                std::cout << "You died :(\n";
                finishGame(dondola, BG);
            }

        }
        
        window->clear(); 
        //# Not game over
        if (!gameEnd) {
            window->draw(*BG);
            window->draw(*wire);
            window->draw(*dondola);
            window->draw(t_UI); // UI text
            rockets::renderAll(window);
        }

        //# Game over
        else {

            window->clear(sf::Color::Black);
           
            //# Username entered
            if (isPaused){
                if (!leaderboard) setHigh(dondola->getScore(), vnos);
                renderScene(window, dondola->getScore());
                leaderboard = true;
            }

            //# Need to enter username
            else {
                sf::Text besedilo_k;
                besedilo_k.setFont(font);
                besedilo_k.setCharacterSize(30);
                besedilo_k.setPosition(320, 42);
                besedilo_k.setString("Game Over!\nEnter username: " + vnos + "\n\nYour Time was: " + std::to_string(dondola->getScore()) + " seconds!\n");
                window->draw(besedilo_k);
            }

        }
        //# Show the drawn sprites in window
        window->display();
        // shows current fps
        // std::cout << "Current Frame Rate: " << 1.f / counter_fps.restart().asSeconds() << "\n";
    }

    //? DELETING ALL ASSETS DON'T REMOVE THIS (memory leak)
    // system("cls");
    std::cout << "Deleting Assets...\n";
    delete window;
    // sprites
    delete BG;
    delete wire;
    // player
    delete dondola;
    // lists
    delete scoreList;
    delete rockets::e_list;
    std::cout << "Deleted all assets!\tThanks For Playing!\n";
    // pause for debug reasons
    // system("pause");
    return EXIT_SUCCESS;
}

void reset(Player* dondola, sf::Sprite* background) {
    dondola->setPercent(0);
    dondola->setPosition(0, 120);
    dondola->resetClock();
    dondola->resetTimer();
    dondola->setHealth(3);

    rockets::popAll();
    rockets::begin();

    isPaused = false;
    gameEnd = false;

    background->setPosition(0, 0);
}

void finishGame(Player* player, sf::Sprite* background) {
    gameEnd = true;
    player->pauseClock();
    rockets::popAll();
    player->saveScore();
}