#include "../CLASSES - HEADER/Enemy.h"
//constructors
Enemy::Enemy() { speed = 20; clock.restart(); }
/*Enemy::Enemy(float speedOfEnemy) { speed = speedOfEnemy; }
Enemy::Enemy(sf::Texture texture, float speed, float scaleX, float scaleY, float x, float y) {
	this->setTexture(texture);
	this->speed = speed;
	this->setScale(scaleX, scaleY);
	this->clock.restart();
	this->setPosition(x, y);
}
*/
Enemy::Enemy(sf::Texture* texture, float speed, float scaleX, float scaleY, float x, float y){
	this->test = texture;
	this->setTexture(*test);
	this->speed = speed;
	// this->setScale(scaleX, scaleY);
	this->clock.restart();
	this->setPosition(x, y);
	// (this one starts (8, 8) pixels from the top left and has a size of (16, 16)
	this->setHitbox({ 0.f,8.f,64.f,32.f });
	this->timer = 0.f;
}


//other stuff
float Enemy::getSpeed() { if (this != NULL) return speed; else return 1; }
void Enemy::spawnEnemy(float spwnLocationX) { this->setPosition(spwnLocationX, 50); }
void Enemy::clockReset() { clock.restart(); }
float Enemy::getClockElapsed() { if (this != NULL) return timer + clock.getElapsedTime().asSeconds(); else return 1; }
float Enemy::pauseClock() { return timer += clock.restart().asSeconds();  }
