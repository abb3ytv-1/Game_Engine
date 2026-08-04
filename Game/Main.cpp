#include "../Engine/Engine.h"
#include <iostream>
#include <memory>
#include "Space_Game.h"

using namespace nu;

class Object {
public:
	Object() { std::cout << "constructor\n"; }
	~Object() { std::cout << "destructor\n"; }

	Object(const Object& object) { std::cout << "copy\n"; }
	Object& operator = (const Object& object) { std::cout << "assignment\n"; return *this; }

};

int main() {
	//std::cout << "====================== Object ==================\n"; {
	//	Object objectA;
	//	Object objectB(objectA);
	//	Object objectC;
	//	objectC = objectA;
	//}

	//std::cout << "===================== Raw Pointers =====================\n"; {
	//	Object* objectA = new Object();
	//	std::cout << objectA << std::endl;
	//	Object* objectB = new Object(*objectA);
	//	std::cout << objectB << std::endl;
	//	Object* objectC = nullptr;
	//	objectC = objectA;
	//	std::cout << objectC << std::endl;

	//	delete objectA;
	//	delete objectB;
	//}

	//std::cout << "===================== Smart Pointers =====================\n"; {
	//	std::unique_ptr<Object> objectA = std::make_unique<Object>();
	//	std::cout << objectA.get() << std::endl;
	//	std::unique_ptr<Object> objectB;
	//	objectB = std::move(objectA);
	//	std::cout << objectB.get() << std::endl;
	//}

	//std::cout << " ===================== Unique Pointers ====================\n"; {
	//	std::unique_ptr<Object> objectA = std::make_unique<Object>();
	//	std::cout << objectA.get() << std::endl;
	//	
	//}

	//std::cout << " ===================== Shared Pointers ====================\n"; {
	//	auto objectA = std::make_shared<Object>();
	//	std::cout << objectA.get() << std::endl;
	//	std::cout << objectA.use_count() << std::endl;
	//	auto objectB = objectA;



	//}


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