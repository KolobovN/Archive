#pragma once
#include <header.h>

class Animation{
	SDL_Rect CurrentFramePosition;	//Координаты текущего кадра в карте 
	unsigned int LastTime;		//Последнее время смены кадра
	unsigned int FrameRate;		//Скорость смены кадров в секунду
	unsigned int FrameCount;	//Количество кадров в изображении (для цикла)
public:
	Animation(SDL_Rect FrameSize, unsigned int nFrameRate, unsigned int nFrameCount);
	~Animation();
	void SetFrameRate(unsigned int nFrameRate) {FrameRate = nFrameRate;};
	void SetFrameCount(unsigned int nFrameCount) {FrameCount = nFrameCount;};
	void SetFrame(int nx, int ny) {
		CurrentFramePosition.x = nx;
		CurrentFramePosition.y = ny;
	}
	void SetFrame(SDL_Rect* FrameRect){
		CurrentFramePosition.h = FrameRect->h;
		CurrentFramePosition.w = FrameRect->w;
		CurrentFramePosition.x = FrameRect->x;
		CurrentFramePosition.y = FrameRect->y;
	}
	void SetFrameLine(int ny) {CurrentFramePosition.y = ny;};
	SDL_Rect NextFrame();	//Подготовка следующего кадра
};