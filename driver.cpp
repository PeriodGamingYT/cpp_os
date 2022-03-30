#include "driver.h"

Driver::Driver() {}
void Driver::Activate() {}
int Driver::Reset() { return 0; }
void Driver::Deactivate() {}
DriverManager::DriverManager() {
	numDrivers = 0;
}

DriverManager::~DriverManager() {}
void printf(const char *);

void DriverManager::AddDriver(Driver *driver) {
	if(numDrivers == 255) {
		printf("Too many drivers added!");
		return;
	}
	
	drivers[numDrivers] = driver;
	numDrivers++;
}

void DriverManager::ActivateAll() {
	for(int i = 0; i < numDrivers; i++) {
		drivers[i]->Activate();
	}
}
