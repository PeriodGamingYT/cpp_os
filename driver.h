#ifndef __DRIVER_H
#define	__DRIVER_H

class Driver {
	public:
		Driver();
		virtual void Activate();
		virtual int Reset();
		virtual void Deactivate();
};

class DriverManager {
	private:
		Driver *drivers[256];
		int numDrivers;

	public:
		void AddDriver(Driver*);
		void ActivateAll();
		DriverManager();
		~DriverManager();
};

#endif
