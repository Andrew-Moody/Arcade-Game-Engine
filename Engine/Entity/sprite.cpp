#include "sprite.h"
#include <SDL.h>


Sprite::Sprite()
{
	imageProperties.clip = { 0, 0, 16, 16 };
	imageProperties.dest = { 0, 0, 200, 200 };
	imageProperties.center = { 0,0 };
	imageProperties.flip = SDL_FLIP_NONE;
	imageProperties.angle = 0.0f;

	visible = false;

	currentFrameTime = 0.0f;
	frameDelay = 100.0f;
	currentFrame = 0;
	startFrame = 0;
	endFrame = 0;
	clipXOffset = 0;
	paused = true;
	loopAnimation = false;
}

Sprite::~Sprite() {}


void Sprite::update(float frameTime)
{
	currentFrameTime += frameTime;

	if (currentFrameTime >= frameDelay)
	{
		currentFrameTime -= frameDelay;
		
		if (!paused)
		{
			currentFrame++;
		}
		
	}

	setRect();
}


void Sprite::setRect()
{
	if (currentFrame > endFrame)
	{
		currentFrame = startFrame;
		if (!loopAnimation)
		{
			paused = true;
		}
	}

	imageProperties.clip.x = imageProperties.clip.w * (currentFrame) + clipXOffset;
}

void Sprite::setVisible(bool show) 
{
	visible = show;
}



void Sprite::setFrameDelay(float delay) 
{ 
	frameDelay = delay;
}

void Sprite::setStartEndFrames(int start, int end) 
{
	startFrame = start;
	endFrame = end; 
}

void Sprite::setCurrentFrame(int frame) 
{ 
	currentFrame = frame;
}

void Sprite::startAnimation(bool loop) 
{ 
	currentFrame = startFrame;
	paused = false;
	loopAnimation = loop; 
}

void Sprite::stopAnimation() 
{
	currentFrame = startFrame;
	paused = true;
	loopAnimation = false; 
}

void Sprite::setPause(bool pause)
{ 
	paused = pause;
}

void Sprite::setLoop(bool loop)
{ 
	loopAnimation = loop;
}


void Sprite::setClip(int x, int y, int w, int h) 
{ 
	imageProperties.clip = { x, y, w, h };
	clipXOffset = x;
}


void Sprite::setXY(int x, int y)
{ 
	imageProperties.dest.x = x; 
	imageProperties.dest.y = y; 
}

void Sprite::setDest(int x, int y, int w, int h)
{
	imageProperties.dest = { x, y, w, h };
}

void Sprite::setScale(float scale)
{ 
	setWScale(scale); 
	setHScale(scale); 
}

void Sprite::setWScale(float scale) 
{ 
	imageProperties.dest.w = (int)(scale * imageProperties.clip.w);
}

void Sprite::setHScale(float scale)
{
	imageProperties.dest.h = (int)(scale * imageProperties.clip.h); 
}

void Sprite::setDegrees(double deg)
{ 
	imageProperties.angle = deg;
}


const ImageProperties& Sprite::getImageProperties() const 
{ 
	return imageProperties; 
}

bool Sprite::isPaused() const 
{ 
	return paused; 
}