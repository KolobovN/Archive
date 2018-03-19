#include "App.h"


App::App()
{
	win = 0;
	render = 0;
	int req = SDL_GetDesktopDisplayMode(0, &displayMode);
	run = true;
	end = false;
}


App::~App(void)
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

int App::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout<<"SDL_Init Error: "<<SDL_GetError()<<std::endl;
		return 1;
	}
	win = SDL_CreateWindow("App", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1250, 740, 0);
	render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Rect Pos;
	Pos.x = 1051;
	Pos.y = 0;
	SDL_Rect Src;
	Src.x = 0;
	Src.y = 0;
	Src.w = 200;
	Src.h = 500;
	Menu = new MenuWindow(IMG_LoadTexture(render, BACKGROUND_RES), Src, Pos);
	Pos.x = 1075;
	Pos.y = 25;
	Src.w = 120;
	Src.h = 60;
	Menu->AddButton(IMG_LoadTexture(render, BUTTON_RES), Pos, Src, "RESET");
	Pos.y = 100;
	Menu->AddButton(IMG_LoadTexture(render, SWITCH_MODE_RES), Pos, Src, "SWITCH_MODE");
	Pos.y = 400;
	Pos.w = 115;
	Pos.h = 35;
	Menu->AddSign(IMG_LoadTexture(render, ENDGAME_SIGN), Pos, "ENDGAME", false);
	Pos.y = 450;
	Menu->AddSign(IMG_LoadTexture(render, DARKMODE_SIGN), Pos, "DARKMODE", false);
	return 0;
}

void App::Render(ObjectContainer *Objects)
{
	SDL_RenderClear(render);
	for (ObjectContainer::Iterator it = Objects->begin(); it != Objects->end(); it++)
		it.GetObj()->Draw(render);
	SDL_Delay(60);
	SDL_RenderPresent(render);
}


void App::OnEvent(SDL_Event *Event, Mushroom* player, GameField* field)
{
	const Uint8* keyboardstate = SDL_GetKeyboardState(NULL);
	switch(Event->type)
	{
		case SDL_QUIT: run = false;	break;
		case SDL_MOUSEMOTION:
			Menu->MouseOver(Event->motion.x, Event->motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (Menu->IsPressed(Event->button.x, Event->button.y, "RESET")) {
				field->DestroyMaze();
				field->MazeGenerator(15, 15); 
				end = false;
				Menu->HideSign("ENDGAME");
			}
			if (Menu->IsPressed(Event->button.x, Event->button.y, "SWITCH_MODE")) {
				field->SwitchGameMode();
				Menu->SwitchSignVisible("DARKMODE");
			}
			break;
		case SDL_MOUSEBUTTONUP:
			Menu->MouseOver(Event->button.x, Event->button.y);
			break;
		case SDL_KEYDOWN:
			if (!end){
			if (keyboardstate[SDL_SCANCODE_UP])
				if (! field->IsWall(player->GetRow(), player->GetColumn() - 1)) player->Move(0, -1);
			if (keyboardstate[SDL_SCANCODE_DOWN])
				if (! field->IsWall(player->GetRow()+0, player->GetColumn() + 1)) player->Move(0, 1);
			if (keyboardstate[SDL_SCANCODE_LEFT])
				if (! field->IsWall(player->GetRow()-1, player->GetColumn())) player->Move(-1, 0);
			if (keyboardstate[SDL_SCANCODE_RIGHT])
				if (! field->IsWall(player->GetRow()+1, player->GetColumn())) player->Move(1, 0);
			if (field->IsEnd()) {
				end = true;
				Menu->ShowSign("ENDGAME");
			}
			field->CalcVisible();
			break;
		}
	}
}

int App::Execute()
{
	ObjectContainer Obj;
	Mushroom* Player = new Mushroom(IMG_LoadTexture(render, MUSH_RES));
	Player->SetCoordinate(100, 100);
	GameField* Field = new GameField(1050, 750, Player, IMG_LoadTexture(render, SEGMENTS_RES),
		IMG_LoadTexture(render, DARKNESS_SMOG_RES));
	Field->MazeGenerator(13, 11);
	Obj.Emplace(Player);
	Obj.Emplace(Field);
	Obj.Emplace(Menu);
	SDL_Event Event;
	while (run)
	{
		while (SDL_PollEvent(&Event))
		{
			OnEvent(&Event, Player, Field);
		}
		Render(&Obj);
	}
	delete Player;
	delete Field;
	return 0;
}

