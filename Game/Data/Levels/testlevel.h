#pragma once

#include "../../../Engine/State/levelstate.h"

#include <string>
#include <memory>

class Audio;
class Input;
class IGameState;
class EngineCore;

class IGameState;

class Message;

class TestLevel : public LevelState
{

	enum class GameState
	{
		RoundBegin,
		Ready,
		Playing,
		RoundEnd,
		Dead,
		GameOver,
	};

	GameState currentState;
	GameState nextState;

	int score;

	int lives;

	bool waiting;
	float timeToWait;
	float timeWaited;

	int frames;
	int fps;
	float timeSince;

	//std::string nextLevel;



public:
	TestLevel(std::string name, IGameState* parentState, EngineCore* engineCore);
	~TestLevel();

	virtual void update(float deltaTime, Input* input, Audio* audio) override;

	virtual void render(Graphics* graphics) override;

	static std::unique_ptr<IGameState> create(std::string name, IGameState* parentState, EngineCore* engineCore);

private:

	void handleCollision(int entityID_A, int entityID_B);

	void checkMail(Audio* audio);

	// Set the amount of time to wait in milliseconds
	void wait(float time);

	// Returns true if waiting
	bool updateWait(float deltaTime);

	void changeState();

	void handleGameEvent(std::shared_ptr<Message> message);
};