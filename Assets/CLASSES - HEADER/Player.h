#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
class Player : public sf::Sprite {
private:
    double percent;
    float score;
    sf::Clock clock;
    float Timer;
    sf::FloatRect m_hitbox;
    int health;
public:
    // Constructors
    //Player();
    //Player(sf::Texture texture);
    Player(sf::Texture texture, float scaleX, float scaleY);
    
    // Percent
    double getPercent();
    void setPercent(double value);

    //Clock + timer
    float getClockElapsed();
    void resetClock();
    float pauseClock();

    void resetTimer();

    // Hitbox
    void setHitbox(const sf::FloatRect& hitbox);
    sf::FloatRect getGlobalHitbox() const { return getTransform().transformRect(m_hitbox); }

    void getHit();
    int getHealth();
    void setHealth(int health);

    //Scoring
    float saveScore();
    float getScore();

    ~Player() {
        std::cout << "Player deleted\n";
    }


};