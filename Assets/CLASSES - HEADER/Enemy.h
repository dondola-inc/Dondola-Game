#pragma once
#include <SFML/Graphics.hpp>

class Enemy : public sf::Sprite {
    float speed;
    sf::Clock clock;
    float timer;
    sf::Texture* test;
    sf::FloatRect m_hitbox;
public:
    // Constructors
    Enemy();
    /*Enemy(float speedOfEnemy);
    Enemy(sf::Texture texture, float speed, float scaleX, float scaleY, float x, float y);*/
    Enemy(sf::Texture* texture, float speed, float scaleX, float scaleY, float x, float y);

    float getSpeed();
    void spawnEnemy(float spwnLocationX);

    // Clock
    void clockReset();
    float getClockElapsed();
    float pauseClock();

    // Hitbox
    void setHitbox(const sf::FloatRect& hitbox) { m_hitbox = hitbox; }
    sf::FloatRect getGlobalHitbox() const { return getTransform().transformRect(m_hitbox); }

    bool operator==(Enemy* obj2) {
        if (this != NULL && obj2 != NULL)
            if (this == obj2)
                return 1;
        return 0;
    }

};
