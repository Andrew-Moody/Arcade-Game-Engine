#pragma once

#include <SDL.h>
#include <map>
#include <string>
#include <memory>

class Graphics;
class Input;
class Timer;
class Entity;
class IEntityFactory;
enum class EntityType : int;

class IGame
{
protected:

	// Common Properties
	std::shared_ptr<Graphics> graphics;
	std::shared_ptr<Input> input;  
	std::shared_ptr<Timer> timer;
	std::shared_ptr<IEntityFactory> entFactory;

	SDL_Event sdl_event; //holds event information from the most recent poll

	bool paused;
	bool initialized;
	float deltaTime;

	// Map of game objects keyed by id
	std::map<int, std::shared_ptr<Entity>> entities;

public:

	IGame();
	virtual ~IGame();

	// Returns true if an SDL_QUIT event occurs
	bool handleSDLEvents();

	// Initialize core systems
	virtual void initialize(int screenWidth, int screenHeight, bool captureMouse, std::string spriteSheetPath, std::string windowName);

	// Called once per frame by the entry point
	// Returns true if an SDL_QUIT event occurs
	virtual bool run();

	// Prepares for rendering before calling IGame::render()
	virtual void renderGame();

	virtual void createEntity(EntityType type);

	// Needed by the player class
	std::shared_ptr<Input> getPtoInput() { return input; }


	// These will be overriden when creating a new game
	virtual void setupGame() = 0;

	virtual void updatePhysics() = 0;

	virtual void handleCollisions() = 0;

	virtual void updateAI() = 0;

	virtual void render() = 0;


	

};