#pragma once
#include <header.h>

 typedef class Mushroom Mushroom;
 typedef class GameField GameField;
 typedef class ObjectContainer ObjectContainer;
 typedef class MenuWindow MenuWindow;

#define STEP 50

class App
{
private:
	SDL_Window* win; //Главное окно
	SDL_Renderer* render; //Объект
	SDL_DisplayMode displayMode;
	MenuWindow* Menu;
	bool run;
	bool end;
	void OnEvent(SDL_Event* Event, Mushroom* player, GameField* field); //Обработка событий
	void Loop();	//Главный цикл
	void Render(ObjectContainer *Objects);
public:
	App();
	~App(void);
	int Initialize();
	SDL_Renderer* GetRend()	{return render;}
	int Execute();
};

