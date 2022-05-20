#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

const std::string location = "Assets/Highscores.bin";
struct Highscore {
	float time = 0.0f;
	std::string username = "";
};

class OrderedList {	
public:
	Highscore scores[5];
	OrderedList() {
		for (int i = 0; i < 5 && !this->getScore(); i++) {
			scores[i].username = "nope";
			scores[i].time = -10.f;	
		}

		if(!this->getScore()) {
			std::cout << "Successfully loaded scores!\n";
		}
	}
	// Reading files
	int getScore() {
		std::fstream folder;
		folder.open(location, std::ios::in | std::ios::binary);

		//!? Checking for existance of folder
		if (!folder.is_open()) {
			std::cout << "Folder Missing!\n";
			return 0;
		}
		//!? Folder exists
		int i = 0;
		while (i < 5 && !folder.eof()) {
			int velikost;
			char tmp[255];
			std::string username;
			float time;
			folder.read((char*)&velikost, sizeof(int));
			folder.read((char*)&time, velikost);
			folder.read((char*)&velikost, sizeof(int));
			folder.read((char*)tmp, velikost);
			tmp[velikost] = '\0';
			username = tmp;

			if (!folder.eof()) {
				scores[i].username = username;
				scores[i].time = time;
			}
			i++;
		}
		folder.close();
		return i;
	}
	// Writing files
	void write_to_file() {
		std::fstream folder;
		folder.open(location, std::ios::out | std::ios::binary);
		for (int i = 0; i < 5; i++) {
			long long int velikost = sizeof(float);
			folder.write((char*)&velikost, sizeof(int));
			folder.write((char*)&scores[i].time, sizeof(float));
			velikost = scores[i].username.length();
			folder.write((char*)&velikost, sizeof(int));
			folder.write((char*)scores[i].username.c_str(), velikost);
			// std::cout << "<===========>\nPlayer: " << scores[i].username << "\nTime: " << scores[i].time << "\nWritten\n<============>\n";
		}
		folder.close();
	}
	~OrderedList() {
		std::cout << "Local Highscore list deleted\n";
	}

}* scoreList = new OrderedList();

void renderScene(sf::RenderWindow* window, double score) {
	sf::Text scores, title;
	sf::Font font;

	if (!font.loadFromFile("Assets/VCR_OSD_MONO_1.001.ttf")) {
		std::cout << "Failed Loading " << '"' << "VCR_OSD_MONO_1.001.ttf" << '"' << "\tCheck files\n";
		system("pause");
	}

	title.setString("Leaderboard");
	title.setCharacterSize(36);
	title.setFont(font);


	scores.setFont(font);
	scores.setCharacterSize(32);
	scores.setPosition(120, 100);
	scores.setString("");
	for (int i = 0; i < 5; i++){
		// Check if it's an empty thing
		if (scoreList->scores[i].username != "nope") { 
			scores.setString(scores.getString() + std::to_string(i + 1) + ". " + scoreList->scores[i].username + " " + std::to_string(scoreList->scores[i].time) + " seconds\n"); 
		}
		else { 
			scores.setString(scores.getString() + std::to_string(i + 1) + ". No score saved yet!\n"); 
		}
	}
	
	window->draw(title);
	window->draw(scores);
}

void setHigh(float score_n, std::string username_n) {
	scoreList->getScore();
	Highscore tmp = scoreList->scores[0];
	int i;
	for (i = 3; (i >= 0) && (scoreList->scores[i].time < score_n); i--) {
		scoreList->scores[i + 1] = scoreList->scores[i];
	}
	scoreList->scores[i + 1].time = score_n;
	scoreList->scores[i + 1].username = username_n;

	std::cout << "setHigh() Finished!\n";
}
