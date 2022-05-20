#include "../CLASSES - HEADER/Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>


// Constructor
/*Player::Player() {
	percent = 0; 
	score = 0.00; 
	clock.restart(); 
	Timer = 0; 
}*/
//Player::Player(sf::Texture texture) { this->setTexture(texture);  Timer = 0;}
Player::Player(sf::Texture texture, float scaleX, float scaleY) {
	this->setTexture(texture); 
	this->setScale(scaleX, scaleY); 
	percent = 0; 
	score = 0.00; 
	Timer = 0; 
	this->setHitbox({ 8.f,8.f,16.f,16.f });
	health = 3;
}

// Gets
float Player::getClockElapsed() { return Timer + clock.getElapsedTime().asSeconds(); }
double Player::getPercent(){ return percent; }

// Sets
void Player::setPercent(double value) { percent = value; }

// Pause
float Player::pauseClock() { return Timer += clock.restart().asSeconds();}

// Reset
void Player::resetTimer() { Timer = 0; }
void Player::resetClock() { clock.restart(); }

// Hitbox
void Player::setHitbox(const sf::FloatRect& hitbox) { m_hitbox = hitbox; }

// Health
void Player::getHit() {	health--; }
int Player::getHealth(){ return health; }
void Player::setHealth(int health) { this->health = health; }


// Score
float Player::saveScore() { score = pauseClock(); return score; }
float Player::getScore() {	return score; }


