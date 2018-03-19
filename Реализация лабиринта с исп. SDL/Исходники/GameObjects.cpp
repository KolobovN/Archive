#include "GameObjects.h"

GameObject::GameObject()
{}

GameObject::~GameObject()
{}

Mushroom::Mushroom(SDL_Texture* Tex)
{
	Texture = Tex;
	Position.x = 0;
	Position.y = 0;
	Position.w = CELL_SIZE;
	Position.h = CELL_SIZE;
	TexSource.x = 0;
	TexSource.y = 0;
	TexSource.w = CELL_SIZE;
	TexSource.h = CELL_SIZE;
	MushAnimation = new Animation(TexSource, 6, 11);
}

Mushroom::~Mushroom()
{
	SDL_DestroyTexture(Texture);
}

void Mushroom::Draw(SDL_Renderer* TargetRender)
{
	TexSource = MushAnimation->NextFrame();
	//SDL_QueryTexture(Texture, NULL, NULL, 0, 0);
	SDL_RenderCopy(TargetRender, Texture, &TexSource, &Position);
}

void Mushroom::SetIndex(int r, int c)
{
	rowIndex = r;
	columnIndex = c;
}

void Mushroom::SetCoordinate(int nx, int ny)
{
	Position.x = nx; 
	Position.y = ny;	
}

void Mushroom::Move(int nx, int ny)
{
	if (nx < 0) MushAnimation->SetFrameLine(0);
	else if (nx > 0) MushAnimation->SetFrameLine(0 + CELL_SIZE);
	rowIndex += nx;
	columnIndex += ny;
}

///////////////////////////////////

GameField::GameField(int ScreenWidth, int ScreenHeight, Mushroom* player1, SDL_Texture* WallTex, SDL_Texture* DarkTex)
{
	Player1 = player1;
	Darkness = new DarknessSmog(DarkTex);
	SegmentsTexture = WallTex;
	DarkMode = false;
	ScreenW = 20;
	ScreenH = 15;
	VisibleCells.X1 = 0;
	VisibleCells.X2 = 0;
	VisibleCells.Y1 = 0;
	VisibleCells.Y2 = 0;
}

bool GameField::MazeGenerator(int MazeWidth, int MazeHeight)
{
	MazeW = MazeWidth;
	MazeH = MazeHeight;
	CellsMap = new Cell* [MazeW];
	for (int i = 0; i < MazeW; i++)
	{
		CellsMap[i] = new Cell[MazeH]; //Ебучий двумерный динамический массив
	}
	for (int i = 0; i < MazeW; i++)//Распределение координат для ячеек на игровом поле
	{
		for (int j = 0; j < MazeH; j++)
		{
			CellsMap[i][j].rowIndex = i;
			CellsMap[i][j].columnIndex = j;
			CellsMap[i][j].SetSegmentType(SEG_WALL);
		};
	};
	for (int i = 1; i < MazeW; i += 2){	//Помечаем нечётные ячейки как непосещённые генератором 
		for (int j = 1; j < MazeH; j += 2){
			CellsMap[i][j].SetSegmentType(SEG_UNVISITED);
		};
	};
	srand(SDL_GetTicks());
	int X;
	int Y;
	// Генерируем случайные нечётные числа в диапазоне нечётных клеток поля
	do {
		X = rand() % MazeW;
	} while ((X % 2 == 0));
	do {
		Y = rand() % MazeH;
	} while ((Y % 2 == 0));
	std::stack<Cell*> PreviousLocation;
	Cell* CurrentCell = &CellsMap[X][Y];
	Cell* UnvisitedCell = nullptr;
	while(1)
	{
		UnvisitedCell = GetNeighbour(CurrentCell);	//Функция возвращает случайного непосещённого соседа
		if (UnvisitedCell == nullptr) 
		{
			if (PreviousLocation.empty()) //Непосещённых клеток нет. Генерация завершена
				{
					CellsMap[X][Y].SetSegmentType(SEG_SPAWN);
					SetPlayerLocation(X, Y);
					SetExitLocation(X, Y);
					return true;	
				}
			CurrentCell = PreviousLocation.top();	//Вынимаем ячейку из стека и отмечаем её как текущую
			PreviousLocation.pop();
		}
		else
		{
			PreviousLocation.push(CurrentCell);		//Заносим текущую позицию в стек
			DestroyWall(CurrentCell, UnvisitedCell);
			CurrentCell = UnvisitedCell;
			CurrentCell->SetSegmentType(SEG_GROUND);
			UnvisitedCell = nullptr;
		}
	}
}

GameField::Cell* GameField::GetNeighbour(Cell* CurrentCell)
{
	int Count = 0;
	Cell* Unvisited[4];
	for (int i = 0; i < 4; i++)	{
		Unvisited[i] = nullptr;
	};
	int indexX = CurrentCell->GetRow();
	int indexY = CurrentCell->GetColumn();
	if (((indexX + 2) < MazeW) && (CellsMap[indexX + 2][indexY].GetSegmentType() == SEG_UNVISITED)) 
	{
		Unvisited[Count] = &CellsMap[indexX + 2][indexY];
		Count++;
	}
	if (((indexX - 2) > 0) && (CellsMap[indexX - 2][indexY].GetSegmentType() == SEG_UNVISITED))
	{
		Unvisited[Count] = &CellsMap[indexX - 2][indexY];
		Count++;
	}
	if (((indexY + 2) < MazeH) && (CellsMap[indexX][indexY + 2].GetSegmentType() == SEG_UNVISITED))
	{
		Unvisited[Count] = &CellsMap[indexX][indexY + 2];
		Count++;
	}
	if (((indexY - 2) > 0) && (CellsMap[indexX][indexY - 2].GetSegmentType() == SEG_UNVISITED))
	{
		Unvisited[Count] = &CellsMap[indexX][indexY - 2];
		Count++;
	}
	if (Count == 0) return nullptr;
	else if (Count == 1) return Unvisited[0];
	else return Unvisited[rand() % Count]; //АПАСНА
}

void GameField::DestroyWall(Cell* const CurrentCell, Cell* const Dest)
{
	int cx = CurrentCell->GetRow();	//Рассчитываем позицию в массиве ячеек
	int dx = Dest->GetRow();
	int cy = CurrentCell->GetColumn();
	int dy = Dest->GetColumn();
	int x = 0;
	int y = 0;
	if (dx > cx) x = 1;
	else if (dx < cx) x = -1;
	if (dy > cy) y = 1;
	else if (dy < cy) y = -1; //Магия. Получаем индекс стены между ячейками
	CellsMap[cx + x][cy + y].SetSegmentType(SEG_GROUND);
}

void GameField::SetPlayerLocation(int IndexX, int IndexY)
{
	Player1->SetIndex(IndexX, IndexY);
	this->CalcVisibleCells();
	this->CalcDarkmodeCells();

}

void GameField::SetExitLocation(int IndexX, int IndexY)	//Задаёт точку выхода из лабиринта. Пока выбирается дальняя от начала
{
	int StartX;
	int IncX;
	int StartY;
	int IncY;
	if (IndexX <= (MazeW / 2)) {StartX = MazeW-1; IncX = -1;}
	else {StartX = 0; IncX = 1;}
	if (IndexY <= (MazeH / 2)) {StartY = MazeH-1; IncY = -1;}
	else {StartY = 0; IncY = 1;}
	for (int i = StartX; i != MazeW - StartX; i += IncX)
		for (int j = StartY; j != MazeH - StartY; j += IncY)
			if (CellsMap[i][j].GetSegmentType() != SEG_WALL)
			{
				CellsMap[i][j].SetSegmentType(SEG_EXIT);
				return;
			}
}

GameField::~GameField()
{
	for (int i = 0; i < MazeW; i++)
		delete []CellsMap[i];
	SDL_DestroyTexture(SegmentsTexture);
}

void GameField::Draw(SDL_Renderer* TargetRender)
{
	if (DarkMode)
	{
		for (int i = DarkModeCells.X1; i < DarkModeCells.X2; i++)
			for (int j = DarkModeCells.Y1; j < DarkModeCells.Y2; j++)
				CellsMap[i][j].Draw(TargetRender, SegmentsTexture);
		Darkness->SetCoordinate(Player1->GetX() - 100, Player1->GetY() - 100);
		Darkness->Draw(TargetRender);
	}
	else
	{
		for (int i = VisibleCells.X1; i < VisibleCells.X2; i++)
			for (int j = VisibleCells.Y1; j < VisibleCells.Y2; j++)
				CellsMap[i][j].Draw(TargetRender, SegmentsTexture);
	}
}

bool GameField::IsWall(int row, int column)
{
	if (CellsMap[row][column].segment == SEG_WALL)
		return true;
	return false;
}

bool GameField::IsEnd()
{
	int x = Player1->GetRow();
	int y = Player1->GetColumn();
	if (CellsMap[x][y].segment == SEG_EXIT)
		return true;
	return false;
}

int GameField::GetSegType(int row, int column)
{
	return CellsMap[row][column].segment;
}

void GameField::CalcDarkmodeCells()
{
	int x = Player1->GetRow();
	int y = Player1->GetColumn();
	//Проверяем выход за пределы поля
	if ((x - 2) >= 0) DarkModeCells.X1 = x - 2;
	else DarkModeCells.X1 = x - 1;
	if ((x + 3) <= MazeW) DarkModeCells.X2 = x + 3;
	else if ((x + 2) <= MazeW) DarkModeCells.X2 = x + 2;
	else DarkModeCells.X2 = x + 1;
	if ((y - 2) >= 0) DarkModeCells.Y1 = y - 2;
	else DarkModeCells.Y1 = y - 1;
	if ((y + 3) <= MazeH) DarkModeCells.Y2 = y + 3;
	else if ((y + 2) <= MazeH) DarkModeCells.Y2 = y + 2;
	else DarkModeCells.Y2 = y + 1;
}

void GameField::CalcVisibleCells()
{
	if ((MazeW > ScreenW) || (MazeH > ScreenH))	//Если поле не вмещается экран 
	{
		int x = Player1->GetRow();
		int y = Player1->GetColumn();
		int radW = ScreenW / 2;		//Половина ширины экрана
		int radH = ScreenH / 2;		//Половина высоты экрана
		//Если гриб далеко от границ то он всегда в центре экрана
		//Проверяем выход за пределы поля (Для правильного отображения поля и игрока рядом с границами)
		if ((x - radW) > 0) {
				VisibleCells.X1 = x - radW;		//Тут расчет индексов в случае нахождения далеко от границ поля
				if ((x + radW + 1) < MazeW) VisibleCells.X2 = x + radW + 1;
				else 
				{
					VisibleCells.X2 = MazeW;		//Если гриб рядом с правой частью экрана
					VisibleCells.X1 = MazeW - ScreenW-1;
				}
		}
		else{
				VisibleCells.X1 = 0;	//Вот здесь, если гриб рядом с левой частью экрана
				VisibleCells.X2 = ScreenW+1;
		}
		if ((y - radH) > 0) {
			VisibleCells.Y1 = y - radH;	//Аналогично - тут далеко от границ
			if ((y + radH + 1) < MazeH) VisibleCells.Y2 = y + radH + 1;
			else{
					VisibleCells.Y2 = MazeH;	//Близко к нижней границе
					VisibleCells.Y1 = MazeH-ScreenH;
			}
		}
		else {
			VisibleCells.Y1 = 0;	//Верхняя граница
			VisibleCells.Y2 = ScreenH;
		}

	}
	else	//В случае, если поле меньше экрана то опускаем всю еболу сверху
	{
		VisibleCells.X1 = 0;
		VisibleCells.X2 = MazeW;
		VisibleCells.Y1 = 0;
		VisibleCells.Y2 = MazeH;
	}
	SetScreenCoordinate();		//Распределяем координаты
}

void GameField::SetScreenCoordinate()
{
	// Распределяем координаты для отрисовки
	for (int i = VisibleCells.X1, m = 0; i < VisibleCells.X2; i++, m++)
		for (int j = VisibleCells.Y1, k = 0; j < VisibleCells.Y2; j++, k++){
			CellsMap[i][j].SetCoordinate(CELL_SIZE * m, CELL_SIZE * k);
			if ((i == Player1->GetRow()) && (j == Player1->GetColumn())) Player1->SetCoordinate(CELL_SIZE * m, CELL_SIZE * k);
		}
}

void GameField::CalcVisible()
{
	this->CalcVisibleCells();
	this->CalcDarkmodeCells();
}

void GameField::DestroyMaze()
{
	for (int i = 0; i < MazeW; i++)
	delete []CellsMap[i];
}

void GameField::SwitchGameMode()
{
	if (DarkMode) DarkMode = false;
	else DarkMode = true;
}
////////////////////////////////////////////////////////////////
void GameField::Cell::SetSegmentType(int type)
{
	switch(type)
	{
	case SEG_GROUND: src.x = 0; segment = SEG_GROUND; break;
	case SEG_WALL: src.x = 50; segment = SEG_WALL; break;
	case SEG_SPAWN: src.x = 100; segment = SEG_SPAWN; break;
	case SEG_EXIT: src.x = 150; segment = SEG_EXIT; break;
	case SEG_UNVISITED: src.x=0; segment = SEG_UNVISITED; break;
	};
}

void GameField::Cell::SetCoordinate(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

void GameField::Cell::Draw(SDL_Renderer* TargetRender, SDL_Texture* SegTex)
{
	//SDL_QueryTexture(SegTex, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(TargetRender, SegTex, &src, &pos);
}

///////////////////////////////////////////////////////////////////
DarknessSmog::DarknessSmog(SDL_Texture* Tex)
{
	Texture = Tex;
	Position.x = 0;
	Position.y = 0;
	Position.w = CELL_SIZE*5;
	Position.h = CELL_SIZE*5;
	TexSource.x = 0;
	TexSource.y = 0;
	TexSource.w = CELL_SIZE*5;
	TexSource.h = CELL_SIZE*5;
	DarkAnimation = new Animation(TexSource, 6, 10);
}

DarknessSmog::~DarknessSmog()
{
	SDL_DestroyTexture(Texture);
}

void DarknessSmog::SetCoordinate(int x, int y)
{
	Position.x = x;
	Position.y = y;
}

void DarknessSmog::Draw(SDL_Renderer* TargetRender)
{
	TexSource = DarkAnimation->NextFrame();
	SDL_RenderCopy(TargetRender, Texture, &TexSource, &Position);
}

///////////////////////////////////////////////////////////////////

MenuWindow::Button::Button(SDL_Texture* ButtonTexture, SDL_Rect ButtonPosition, SDL_Rect ButtonSize, char* ButtonName)
{
	ButTex = ButtonTexture;
	Position = ButtonPosition;
	Position.w = 120;
	Position.h = 60;
	TexSource = ButtonSize;
	Name = ButtonName;
}

MenuWindow::Button::~Button()
{
	SDL_DestroyTexture(ButTex);
}

bool MenuWindow::Button::IsMouseOver(int nx, int ny)
{
	if ((nx >= Position.x) && (nx <= (Position.x + TexSource.w)))
		if ((ny >= Position.y) && (ny <=(Position.y + TexSource.h)))
		{
			TexSource.y = TexSource.h;
			return true;
		}
	TexSource.y = 0;
	return false;
}

bool MenuWindow::Button::IsPressed(int nx, int ny, char* name)
{
	if (!strcmp(Name.c_str(), name))
		if ((nx >= Position.x) && (nx <= (Position.x + TexSource.w)))
			if ((ny >= Position.y) && (ny <=(Position.y + TexSource.h)))
			{
				TexSource.y = TexSource.h * 2;
				return true;
			}
	TexSource.y = 0;
	return false;
}

void MenuWindow::Button::Draw(SDL_Renderer* TargetRender)
{
	SDL_RenderCopy(TargetRender, ButTex, &TexSource, &Position);
};

/////////////////////////////////////////////////

MenuWindow::Sign::Sign(SDL_Texture* nText, SDL_Rect nPos, char* name)
{
	Texture = nText;
	Position = nPos;
	SRC.x = 0;
	SRC.y = 0;
	SRC.w = 115;
	SRC.h = 35;
	Name = name;
	visible = false;
}

MenuWindow::Sign::~Sign()
{
	SDL_DestroyTexture(Texture);
}

void MenuWindow::Sign::Draw(SDL_Renderer* TargetRender)
{
	if (visible) SDL_RenderCopy(TargetRender, Texture, &SRC, &Position);
}

///////////////////////////////////////////////////
MenuWindow::MenuWindow(SDL_Texture* nBackground, SDL_Rect nTexSource, SDL_Rect nPosition)
{
	Background = nBackground;
	Position = nPosition;
	Position.w = 200;
	Position.h = 750;
	TexSource = nTexSource;
	ButtList = new ControlList<Button>();
	SignList = new ControlList<Sign>();
}

MenuWindow::~MenuWindow()
{
	SDL_DestroyTexture(Background);
}

void MenuWindow::AddButton(SDL_Texture* ButtonTexture, SDL_Rect ButtonPosition, SDL_Rect ButtonSize, char* ButtonName)
{
	Button* NewBut = new Button(ButtonTexture, ButtonPosition, ButtonSize, ButtonName);
	ButtList->Emplace(NewBut);
}

void MenuWindow::AddSign(SDL_Texture* SignTexture, SDL_Rect SignPosition, char* name, bool Visbl)
{
	Sign* newSign = new Sign(SignTexture, SignPosition, name);
	if (Visbl) newSign->SetVisible();
	SignList->Emplace(newSign);
}

void MenuWindow::ShowSign(const char* name)
{
	for (MenuWindow::ControlList<Sign>::Iterator it = SignList->begin(); it != SignList->end(); it++)
		if (!strcmp(name, it.GetObj()->GetName()))	//Если совпало
			{
				it.GetObj()->SetVisible();
				return;
			}
}

void MenuWindow::HideSign(const char* name)
{
	for (MenuWindow::ControlList<Sign>::Iterator it = SignList->begin(); it != SignList->end(); it++)
		if (!strcmp(name, it.GetObj()->GetName()))	//Если совпало
			{
				it.GetObj()->SetInvisible();
				return;
			}
}

void MenuWindow::SwitchSignVisible(const char* name)
{
	for (MenuWindow::ControlList<Sign>::Iterator it = SignList->begin(); it != SignList->end(); it++)
		if (!strcmp(name, it.GetObj()->GetName()))	//Если совпало
			{
				it.GetObj()->SwitchVisible();
				return;
			}
}


bool MenuWindow::MouseOver(int nx, int ny)
{
	for (MenuWindow::ControlList<Button>::Iterator it = ButtList->begin(); it != ButtList->end(); it++)
		if (it.GetObj()->IsMouseOver(nx, ny)) return true;
	return false;
}

bool MenuWindow::IsPressed(int nx, int ny, char* name)
{
	for (MenuWindow::ControlList<Button>::Iterator it = ButtList->begin(); it != ButtList->end(); it++)
		if (it.GetObj()->IsPressed(nx, ny, name)) return true;
	return false;
}
/*bool MenuWindow::IsPressed(int nx, int ny, char* name)
{
	for (MenuWindow::ControlList<Button>::Iterator it = ButtList->begin(); it != ButtList->end(); it++)
		it.GetObj()->IsPressed(nx, ny);
}*/

void MenuWindow::Draw(SDL_Renderer* TargetRender)
{
	SDL_RenderCopy(TargetRender, Background, 0, &Position);
	for (MenuWindow::ControlList<Button>::Iterator it = ButtList->begin(); it != ButtList->end(); it++)
		it.GetObj()->Draw(TargetRender);
	for (MenuWindow::ControlList<Sign>::Iterator it = SignList->begin(); it != SignList->end(); it++)
		it.GetObj()->Draw(TargetRender);
}

