#pragma once

#include "../CLASSES - HEADER/Enemy.h"
#include "SFML/System/Clock.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <cmath>

namespace rockets{
	float fRand(float fMin, float fMax) {
		float f = (float) rand() / RAND_MAX;
		return fMin + f * (fMax - fMin);
	}

	float difficulty_m = 2.f;

	struct Item {
		Enemy enemy;
		Item* next = NULL;
	};

	class List {
		sf::Clock delay;
		
	public:
		Item* start;
		sf::Texture texture;
		List() { start = NULL; delay.restart(); texture.loadFromFile("Assets/Texture-Map-3.png", sf::IntRect(126, 0, 126, 256)); }

		void AddToList(Enemy newEnemy) {
			Item* novi = new Item;
			novi->enemy = newEnemy;
			novi->next = NULL;
			if (start == NULL) {
				start = novi;
			}
			else {
				Item* tmp;
				for (tmp = start; tmp->next != NULL; tmp = tmp->next);
				// tmp is now the last item
				tmp->next = novi;
			}
		}

		sf::Texture getTexture() { return texture; }

		//? BUG - you need FIX
		bool removeFromList(Item* item) {
			if (start == NULL) return 0;
			Item* tmp = start;
			Item* b_tmp = NULL;
			while (tmp != NULL && tmp != item) {
				b_tmp = tmp;
				tmp = tmp->next;
			}			
			
			if (tmp == NULL) return 0;
			
			if (tmp == start) {
				start = start->next;
				delete tmp;
				return 1;
			}

			// if (b_tmp->next == tmp) std::cout << "no way,,, it works\n";
			b_tmp->next = tmp->next;
			delete tmp;
			tmp = b_tmp;
			return 1;
		}

		//! delete first item
		bool pop() {
			if (start == NULL) return false;
			else {
				std::cout << "Deleted a rocket!\n";
				// n_ as "new" but not new
				Item* n_start = start->next;
				delete start;
				start = n_start;
			}
			return true;
		}

		float getElapsedClock() { return delay.getElapsedTime().asSeconds(); }
		void restartClock() { delay.restart(); }

		~List() {
			while (this->pop());
			std::cout << "Deleted all rockets\n";
		}

	}*e_list = new List();

	//? Function to call in int main to start the spawning basically (ig)
	int begin() {
		difficulty_m = 2;
		std::cout << "begin() function started!\n";
		rockets::e_list->AddToList(Enemy(&e_list->texture, rand() % 20 + 5.f, 1.f, 1.f, rand() % 1250 + 30.f, -132.f));
		std::cout << "begin() function finished with success!\n";
		return 0;
	}

	//TODO FINISH UPDATE 
	void update(bool gameEnd, bool isPaused, sf::Vector2f r_movement, int p_x, float time_e) {	
		if (!isPaused && !gameEnd) {

			//# as time passes the spawnrate increases
			if ((int)time_e % 10 == 0) {
				difficulty_m-= 0.002f;
				if (difficulty_m <= 0.2f) difficulty_m = 0.2f;
			}
			// Debug
			// std::cout << difficulty_m << " is the difficulty\n";
			if (e_list->getElapsedClock() >= difficulty_m) {
				e_list->restartClock();
				// random formula to get the speed (from super slow to super fast)
				float r_speed = fRand(time_e * difficulty_m, time_e / difficulty_m);
				// Limit so it's not too fast
				if (r_speed > 40) r_speed = 40;

				// Debug 
				// std::cout << "minimum: " << time_e * difficulty_m<< ", max: " << time_e / difficulty_m << "\n";
				// Debug 
				// std::cout << "Another e_list added to list!\nWith the speed of " << r_speed << "\n";
				
				// Spawning the rocket
				rockets::e_list->AddToList(Enemy(&e_list->texture, r_speed, 1.f, 1.f, rand() % (p_x + 120)  + 120, -132.f));
			}
			if (!isPaused) {

			Item* tmp = e_list->start;
				if (tmp != NULL) r_movement.y = tmp->enemy.getSpeed();
				//! Moving all rockets			
				while (tmp != NULL) {
					tmp->enemy.move(r_movement * tmp->enemy.getClockElapsed()); 
					if (tmp->next != NULL) r_movement.y = tmp->next->enemy.getSpeed();

					if (tmp->enemy.getPosition().y > 720) { 
						Item* t_tmp = tmp->next;
						e_list->removeFromList(tmp); 
						// std::cout << "Rocket is off-screen!\n";
						tmp = t_tmp;
					}else tmp = tmp->next;
				}
			}
		}

	}
	void popAll() {	while (e_list->pop());}

	void renderAll(sf::RenderWindow* window) {
		Item* tmp = e_list->start;
		//! Rendering all rockets
		while (tmp != NULL) {
			window->draw(tmp->enemy);
			tmp = tmp->next;
		}
	}


	//! check for collisions with dondola
	bool getEnemyCollision(Player* obj) {
		Item* tmp = e_list->start;
		while (tmp != NULL) {
			bool isCollided = obj->getGlobalHitbox().intersects(tmp->enemy.getGlobalHitbox());
			if (isCollided) { 
				Item* t_tmp = tmp->next;
				e_list->removeFromList(tmp); 
				tmp = t_tmp;
				return 1; 
			}
			tmp = tmp->next;
		}
		return 0;
	}

	//! Sets the position of the rocket above the player -> DEBUG
	void setAbove(float x) {
		e_list->start->enemy.setPosition(x, 0.f);
	}

	void clocks_reset() {
		Item* tmp = e_list->start;
		while (tmp != NULL) {
			tmp->enemy.clockReset();
			tmp = tmp->next;
		}
	}

	void clocks_pause() {
		Item* tmp = e_list->start;
		while (tmp != NULL) {
			tmp->enemy.pauseClock();
			tmp = tmp->next;
		}
	}
}
