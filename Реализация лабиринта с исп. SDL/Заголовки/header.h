#pragma once
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <conio.h>
#include <exception>
#include <list>
#include <stack>
#include <unordered_map>
////
#include "Animation.h"
#include "GameObjects.h"
#include "App.h"

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")
#undef main
#define MUSH_RES "res\\mush.png"
#define SEGMENTS_RES "res\\segments.png"
#define BUTTON_RES "res\\reset.png"
#define BACKGROUND_RES "res\\background.png"
#define ENDGAME_SIGN "res\\end.png"
#define DARKMODE_SIGN "res\\mode.png"
#define SWITCH_MODE_RES "res\\switch_mode.png"
#define DARKNESS_SMOG_RES "res\\darkness.png"

