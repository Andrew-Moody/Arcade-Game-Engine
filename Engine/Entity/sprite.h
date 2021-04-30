#pragma once

#include <SDL.h>

struct ImageProperties
{
	SDL_Rect clip; // source
	SDL_Rect dest;  // destination
	SDL_Point center; // rotation axis point
	SDL_RendererFlip flip; // enum to specify flip direction
	double angle;   // rotation angle
};



class Sprite
{
	//Data used to render the image
	ImageProperties imageProperties;

	bool visible;

	//Animation info
	float currentFrameTime;
	float frameDelay;
	int currentFrame;
	int startFrame;
	int endFrame;
	int clipXOffset; // added to remember the starting position of a sprite sequence
	bool paused;
	bool loopAnimation;

public:

	Sprite();
	~Sprite() {}


	void update(float frameTime);

	void setVisible(bool visible);

	

	
	
	//Animation

	//Amount of time between frame changes in milliseconds
	void setFrameDelay(float delay);

	//Selects which portion of the sheet is used for animation, affectively choosing which animation to play
	void setStartEndFrames(int start, int end);  

	//Force the animation to jump to the specified frame
	void setCurrentFrame(int currentFrame); 

	//Begins the current animation starting from the first frame
	void startAnimation(bool loop);

	//Stops the animation and sets currentFrame back to the start
	void stopAnimation();

	//Pause or resume the current animation
	void setPause(bool pause);

	//Select wether the animation is allowed to loop
	void setLoop(bool loop);


	//Clip
	
	//Animation frames are defined relative to these parameters
	void setClip(int x, int y, int w, int h);


	//Transform

	
	void setXY(int x, int y);

	// Usefull for tiles
	void setDest(int x, int y, int w, int h);

	void setScale(float scale);

	void setWScale(float scale);

	void setHScale(float scale);

	//looks like sdl uses degrees but will have to check
	void setDegrees(double deg); 

	void setFlip(SDL_RendererFlip flip) { imageProperties.flip = flip; }

	

	const ImageProperties& getImageProperties() const;

	bool isPaused() const;

private:
	void setRect(); //set properties based on current frame. Used by update()
};
