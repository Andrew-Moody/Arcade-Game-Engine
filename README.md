# Simple-2D-Game-Engine
### A basic 2D game engine written in C++ with SDL2

As part of Harvard's CS50x online course, we are asked to create a final project that can be anything we want.
The project I have chosen to create is a simple 2D game engine. The engine is written in C++ and uses SDL2 to handle texture rendering. 


The project is broken into engine and game components where the engine is able to stand alone separate from any game specific files.
Along with the engine a sample game will be created to show off features of the engine. 

##### Dependencies:
To build this project yourself you will require the following dependencies:
* SDL2 32bit version 2.0.14
* SDL_image 32bit version 2.0.5

I am using SDL_image to load png images. To enable png loading you will need to put the libpng16-16.dll file that comes with SDL_image in your source directory.
There is an excellent tutorial on setting up these libraries for several different operating systems and IDE's at https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php

##### Version History:

4/15/21
* Added a game event system based on a publish-subscribe pattern with the added ability for subscribers to post messages they want published to other subscribers.
* Game objects remain decoupled by posting a message to the publisher that then distributes the message to all interested parties.
* Game objects can recieve events buy owning a mailbox and subscribing to event types.
* Publisher recieves message responses from subscribers at a seperate time from distribution effectively forming a double buffer that prevents feedback loops.
* The player now posts an event to let the game know it has moved positions. Enemy AI now listens for these events in order to start chasing the player 
  if the player comes into aggresion range and pursue them until they catch up to the player.
* Player and Enemies now flip their sprite orientation based on direction of motion.
* Tile grids can now be loaded from a file.

4/12/21
* Added a class to manage game tiles with animated sprites. A tile grid can be populated with tile types to render background scenery.
* Redesigned entity class to allow post construction attachment of components which allow entityfactory to create custom entities with derived components.
* Added a sprite factory interface to allow initializing sprites by type.
* Implemented AABB collision in collideWith().
* Changed some class names from I to base prefix because they contained default implementation.
* The sample "game" now includes an enemy sprite and shows some background tiles.

4/10/21 
* The initial commit includes enough game code to provide a movable character with animation.
