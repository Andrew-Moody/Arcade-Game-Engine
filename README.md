# Simple-2D-Game-Engine
### Arcade Game Engine written in C++ with SDL2

As part of Harvard's CS50x online course, we are asked to create a final project that can be anything we want.
The project I have chosen to create is a game engine that provides all the facilities necessary to create arcade style games. The engine is written in C++ and uses SDL2 to handle texture rendering, input, and sound. The engine will allow switching between multiple games at runtime by means of a game select menu.
Along with the engine, a few sample games will be created to show off features of the engine. 


##### Dependencies:
To build this project yourself you will require the following dependencies:
* SDL2 32bit version 2.0.14
* SDL_image 32bit version 2.0.5
* SDL_ttf 32bit version 2.0.15

I am using SDL_image to load png images. To enable png loading you will need to put the libpng16-16.dll file that comes with SDL_image in your source directory.
There is an excellent tutorial on setting up these libraries for several different operating systems and IDE's at https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php

##### Version History:

6/22/21
* Refactored all game specific factories to build their respective objects using text files rather than hard coded values. This allows the factories to be moved into the engine as they no longer contain game specific code.
* A component factory was added that allows user created classes to be instantiated by the built-in factories. After creating a new class, users must add an entry to the component registry function.
* The game folder now contains only user created components, levels, and text files.
* Sample Game now features a Pacman style maze with several ghosts.
* Getting caught by a ghost decreases lives left. Running out of lives causes the game to end
* Fireballs can be launched to kill ghosts, removing them from the level and increasing the score.
* After killing a ghost, the remaining ghosts retreat for a few seconds before resuming the chase.
* killing all the ghosts on a level will cause the next level to be entered.


5/11/21
* Changed stateManager class to act as a state but contain a managed stack of child states. 
* States can be pushed, popped, or changed by receiving a message.
* States now have a handlemessage method to handle messages immediately and pass the message to its parent state if unhandled.
* Added the ability to draw boxes of arbitrary size from a png of the border and fill.
* Added text rendering using the SDL_ttf TrueType font library.
* Gave states the ability to choose which sprite sheet and font they want to use when rendering. Keeps art assets separated by game.
* Implemented menu navigation. 
* Sample game now has a Main Menu, Game Menu, Pause Menu, and Game Over screen.
* Game can be paused with the escape key and resumed by pressing escape again or selecting the resume button. Paused game is rendered in the background.
* Getting caught by an enemy causes transition to Game Over screen.


4/29/21
* Overhauled engine structure to incorporate Event-driven Hierarchical State Management.
* Established a framework for transitioning between different games at runtime.
* StateManager class provides a means for managing various state types to include menu navigation, level transitions, and artificial intelligence.
* Player control is now fully event based such that a reference to input is no longer needed.
* Added a mailAddress class that gives limited access to its associated mailbox and provides the ability to post messages directly to a recipient by holding the recipients mailAddress.
* No new features in the sample game, but all existing features still work correctly.


4/15/21
* Added a game event system based on a publish-subscribe pattern.
* Game objects remain decoupled by posting a message to a message bus that then distributes the message to all interested parties on update.
* Game objects can post and receive events buy owning a mailbox.
* The player now posts an event to let the game know it has moved positions. Enemy AI now listens for these events to allow it to chase the player 
  if the player comes into aggression range and pursue them until they catch up to the player.
* Player and Enemies now flip their sprite orientation based on direction of motion.
* Tile grids can now be loaded from a file.



4/12/21
* Added a class to manage game tiles with animated sprites. A tile grid can be populated with tile types to render background scenery.
* Redesigned entity class to allow post construction attachment of components which allows entityfactory to create custom entities with derived components.
* Added a sprite factory interface to allow initializing sprites by type.
* Implemented AABB collision in collideWith().
* Changed some class names from I to base prefix because they contained default implementation.
* The sample "game" now includes an enemy sprite and shows some background tiles.


4/10/21 
* The initial commit includes enough game code to provide a movable character with animation.
