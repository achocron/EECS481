#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H


class Accelerometer {

public:
	//constructor
	Accelerometer();

	void init1();
	void init2();

	void operate();

	float getYaw();
	float getPitch();
	float getRoll();

	double getShakeMagnitude();

	bool getDmpReady();

	bool isFree();

	bool constructed_successfully = false;

	// ================================================================
	// ===               INTERRUPT DETECTION ROUTINE                ===
	// ================================================================

	volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

private:


};

#endif


