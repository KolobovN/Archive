#include "Animation.h"

Animation::Animation(SDL_Rect FrameSize, unsigned int nFrameRate, unsigned int nFrameCount)
{
	CurrentFramePosition = FrameSize;
	FrameCount = nFrameCount;
	FrameRate = 1000 / nFrameRate;	// секунды в миллисекунды
	LastTime = 0;
}

Animation::~Animation()
{}

SDL_Rect Animation::NextFrame()
{
	unsigned int PresTime = SDL_GetTicks();
	if ((PresTime - LastTime) > FrameRate) 
	{
		if (CurrentFramePosition.x >= (CurrentFramePosition.w * (FrameCount-1))) CurrentFramePosition.x = 0;
		else CurrentFramePosition.x += CurrentFramePosition.w;
		LastTime = PresTime;
	}
	return CurrentFramePosition;
}
