#include "../Engine/Engine.h"
#include "Space_Game.h"

#include <fstream>
#include <iostream>
#include <memory>

using namespace nu;

class Animal {
public:
	virtual void speak() { std::cout << "???"; }
};

class Cat : public Animal {
	void speak() override { std::cout << "meow"; }
};

class Dog : public Animal {
	void speak() override { std::cout << "BARK"; }
};

class Bird : public Animal {
	void speak() override { std::cout << "cheep"; }
};

//enum class Type {
//	Cat,
//	Dog,
//	Bird
//};
//
//Animal* AnimalFactory(Type id) {
//	Animal* animal = nullptr;
//
//	switch (id) {
//	case Type::Cat:
//		animal = new Cat;
//		break;
//	case Type::Dog:
//		animal = new Dog;
//		break;
//	case Type::Bird:
//		animal = new Bird;
//		break;
//	}
//
//	return animal;
//}

Animal* AnimalFactory(const std::string& id) {
	Animal* animal = nullptr;

	if (id == "Cat") animal = new Cat;
	else if (id == "Dog") animal = new Dog;
	else if (id == "Bird") animal = new Bird;

	return animal;
}

int main() {
	std::string selection;

	std::cout << "Select Animal: ";
	std::cin >> selection;

	auto animal = AnimalFactory(selection);
	if (animal) animal->speak();

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