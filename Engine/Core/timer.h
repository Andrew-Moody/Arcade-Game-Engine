#pragma once

// Small class used to get sub millisecond time between frames
class Timer
{
	long long unsigned int countStart;
	long long unsigned int countEnd;
	float deltaTime; //ms
	
public:

	Timer();

	// Calculates the time since the last update and saves in deltaTime
	float update();

	// returns deltaTime
	float getDeltaTime() const;

};