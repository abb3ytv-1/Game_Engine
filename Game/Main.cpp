#include "../Engine/Engine.h"
#include "Space_Game.h"

#include <fstream>
#include <iostream>
#include <memory>

using namespace nu;

int main() {

	// read file (input file)
	{
		std::ifstream inFile("Assets/Data/data.txt");
		if (inFile.is_open()) {
			std::string str;
			while (std::getline(inFile, str)) {
				std::cout << str << std::endl;
			}
		}
		else {
			std::cout << "could not load: Assets/Data/data.txt\n";
		}
	}

	// write file (output file)
	{
		std::ofstream outFile("Assets/Data/data.txt", std::ios::app);
		if (outFile.is_open()) {
			outFile << "\nI want to buy a new coffee machine.\n";
		}
	}

	// read/write (input/output)
	{
		std::fstream ioFile("Assets/Data/data.txt", std::ios::in | std::ios::out | std::ios::app);
		if (ioFile.is_open()) {
			// input
			ioFile << "Add a line.\n";
			ioFile.seekg(0);
			// output
			std::string str;
			while(std::getline(ioFile, str)) {
				std::cout << str << std::endl;
			}
		}
	}

	{ 
		std::string name;
		int score;
		bool isAlive;
		
		//save game data
		bool save = false;
		if (save) {
			name = "Abbey";
			score = 143;
			isAlive = true;

			// save game data
			std::ofstream file("Assets/Data/game.txt", std::ios::app);
			if (file.is_open()) {
				file << name << "\n";
				file << score << "\n";
				file << std::boolalpha << isAlive << "\n";
			}
		}

		//load game data
		bool load = true;
		if (load) {
			//load game
			std::ifstream file("Assets/Data/data.txt");
			if (file.is_open()) {
				std::getline(file, name);

				std::string str;
				std::getline(file, str);

				file >> score;
				file >> std::boolalpha >> isAlive;

			}
		}

		//display game data
		std::cout << name << "\n";
		std::cout << score << "\n";
		std::cout << std::boolalpha << isAlive << "\n";
	}


	return 0;

	if (!engine.Initialize()) {
		return 1;
	}

	int result = 0;

	{
		SpaceGame game;
		result = game.Run();
	}

	engine.Shutdown();

	return result;
}