#pragma once
#include <header.h>
#define MUSH_TEXTURE_SIZE 50
#define CELL_SIZE 50
typedef class Animation Animation;
typedef class DarknessSmog DarknessSmog;

class GameObject
{
public:
	GameObject();
	//GameObject(GameObject &newObj);
	virtual ~GameObject();
	virtual void Draw(SDL_Renderer* TargetRender) = 0;
};

class Mushroom:public GameObject
{
	SDL_Texture* Texture;
	Animation* MushAnimation;
	SDL_Rect Position;
	SDL_Rect TexSource;
	int rowIndex;
	int columnIndex;
public:
	Mushroom(SDL_Texture* Tex);
	~Mushroom();
	void Draw(SDL_Renderer* TargetRender);
	void SetCoordinate(int x, int y);
	void SetIndex(int r, int c);
	void Move(int nx, int ny);
	int GetX() 	{return Position.x;}
	int GetY()	{return Position.y;}
	int GetRow() {return rowIndex;}
	int GetColumn() {return columnIndex;}
};

class GameField:public GameObject
{
	Mushroom* Player1;
	DarknessSmog* Darkness;
	SDL_Texture *SegmentsTexture;
	int MazeW; 
	int MazeH;
	int ScreenW;
	int ScreenH;
	struct ScreenRect{
		int X1;	//Левый верхний угол
		int X2; //Правый верхний угол
		int Y1;	//Нижний левый угол
		int Y2;	//Нижний правый угол
	} VisibleCells, DarkModeCells;
	bool DarkMode;
	class Cell{
		public:
		SDL_Rect pos;
		SDL_Rect src;
		int rowIndex;
		int columnIndex;
		int segment;	//Тип ячейки
		Cell()
		{
			pos.x = 0;
			pos.y = 0;
			pos.w = CELL_SIZE;
			pos.h = CELL_SIZE;
			src.x = 0;
			src.y = 0;
			src.w = CELL_SIZE;
			src.h = CELL_SIZE;
			segment = SEG_GROUND;
			rowIndex = 0;
			columnIndex = 0;
		};
		int GetX(){return pos.x;};
		int GetY(){return pos.y;};
		int GetRow(){return rowIndex;};
		int GetColumn(){return columnIndex;};
		void SetCoordinate(int x, int y);
		void SetSegmentType(int type);
		int GetSegmentType(){return segment;};
		void Draw(SDL_Renderer* TargetRender, SDL_Texture* SegTex);
	} **CellsMap;

	Cell* GetNeighbour(Cell* CurrentCell);
	void DestroyWall(Cell* const Curr, Cell* constDest);
	void SetPlayerLocation(int IndexX, int IndexY);
	void SetExitLocation(int IndexX, int IndexY);
	void CalcDarkmodeCells();
	void CalcVisibleCells();
	void SetScreenCoordinate();
public:
	enum segments{SEG_GROUND, SEG_WALL, SEG_SPAWN, SEG_EXIT, SEG_UNVISITED};
	GameField(int ScreenWidth, int ScreenHeight, Mushroom* player1, SDL_Texture* WallTex, SDL_Texture* DarkTex);
	~GameField();
	void Draw(SDL_Renderer* TargetRender);
	bool IsFinish(int nx, int ny);
	bool IsWall(int nx, int ny);
	int GetSegType(int nx, int ny);
	bool MazeGenerator(int MazeWidth, int MazeHeight);
	void DestroyMaze();
	bool IsEnd();
	void SwitchGameMode();	
	void CalcVisible();
};

class DarknessSmog:public GameObject{
	SDL_Texture* Texture;
	Animation* DarkAnimation;
	SDL_Rect Position;
	SDL_Rect TexSource;
public:
	DarknessSmog(SDL_Texture* Tex);
	~DarknessSmog();
	void SetCoordinate(int x, int y);
	void Draw(SDL_Renderer* TargetRender);
};


class MenuWindow:public GameObject{
	SDL_Texture* Background;
	SDL_Rect Position;
	SDL_Rect TexSource;
	class Button{
		SDL_Texture* ButTex;
		SDL_Rect Position;
		SDL_Rect TexSource;
		std::string Name;
	public:
		Button(SDL_Texture* ButtonTexture, SDL_Rect ButtonPosition, SDL_Rect ButtonSize, char* ButtonName);
		~Button();
		bool IsPressed(int nx, int ny, char* name);
		bool IsMouseOver(int nx, int ny);
		void Draw(SDL_Renderer* TargetRender);
	};
	class Sign{
		SDL_Rect Position;
		SDL_Rect SRC;
		SDL_Texture* Texture;
		std::string Name;
		bool visible;
	public:
		Sign(SDL_Texture* nText, SDL_Rect nPos, char* name);
		~Sign();
		void SetVisible(){visible = true;};
		void SetInvisible(){visible = false;};
		void SwitchVisible(){
			if (visible) visible = false;
			else visible = true;
		}
		const char* GetName(){return Name.c_str();};
		void Draw(SDL_Renderer* TargetRender);
	};
	template <typename Type> class ControlList{
		struct Node{
			Node* nextNode;
			Type* Control;
		};
		Node* HeadNode;
		unsigned int size;
	public:
		class Iterator
		{
			Node* Data;
		public:
			Iterator(Node* node) : Data (node) {};
			bool operator==(const Iterator &other) const
			{
				if (this == &other) return true;
				return Data == other.Data;
			}
			bool operator!=(const Iterator &other) const
			{
				return !operator==(other);
			}
			Type* GetObj() const 
			{
				if (Data->Control) return Data->Control;
				else return 0;
			}
			void operator++() {
				if (Data != 0)
				{
					Data = Data->nextNode;
				}
			}
		};
	ControlList(){
		size = 0;
		HeadNode = 0;
	};
	~ControlList(){
		Node* temp = new Node;
		while (HeadNode->nextNode != 0)
		{
			temp = HeadNode->nextNode;
			delete HeadNode;
			HeadNode = temp;
		}
	delete temp;
	};
	Node* GetHead()	{return HeadNode;}
	Iterator begin() const 	{return Iterator(HeadNode);}
	Iterator end() const {return Iterator(0);}
	int GetSize() {return size;}
	void Emplace(Type* newElement){
		Node* temp = new Node;
		temp->nextNode = HeadNode;
		temp->Control = newElement;
		HeadNode = temp;
		size++;
	};
	Type* GetObject();
};
	ControlList<Button>* ButtList;
	ControlList<Sign>* SignList;
public:
	MenuWindow(SDL_Texture* nBackground, SDL_Rect nTexSource, SDL_Rect nPosition);
	~MenuWindow();
	void AddButton(SDL_Texture* ButtonTexture, SDL_Rect ButtonPosition, SDL_Rect ButtonSize, char* ButtonName);
	void AddSign(SDL_Texture* SignTexture, SDL_Rect SignPosition, char* name, bool Visbl);
	bool MouseOver(int nx, int ny);
	//bool IsPressed(int nx, int ny);
	bool IsPressed(int nx, int ny, char* name);
	void ShowSign(const char* name);
	void HideSign(const char* name);
	void SwitchSignVisible(const char* name);
	void Draw(SDL_Renderer* TargetRender);
};


//////////////////////////////////////////////////////
class ObjectContainer
{
	struct Node {
		Node* nextNode;
		GameObject* Object;
	};
	Node* HeadNode;
	unsigned int size;
public:
	class Iterator
	{
		Node* Data;
	public:
		Iterator(Node* node) : Data (node) {};
		bool operator==(const Iterator &other) const
		{
			if (this == &other) return true;
			return Data == other.Data;
		}
		bool operator!=(const Iterator &other) const
		{
			return !operator==(other);
		}
		GameObject* GetObj() const 
		{
			if (Data->Object) return Data->Object;
			else return 0;
		}
		void operator++() {
			if (Data != 0)
			{
				Data = Data->nextNode;
			}
		}
	};
	//Методы 
	ObjectContainer(){
		size = 0;
		HeadNode = 0;
	};
	~ObjectContainer(){
		Node* temp = new Node;
		while (HeadNode->nextNode != 0)
		{
			temp = HeadNode->nextNode;
			delete HeadNode;
			HeadNode = temp;
		}
	delete temp;
	};
	Node* GetHead()	{return HeadNode;}
	Iterator begin() const 	{return Iterator(HeadNode);}
	Iterator end() const {return Iterator(0);}
	int GetSize() {return size;}
	void Emplace(GameObject* newElement){
		Node* temp = new Node;
		temp->nextNode = HeadNode;
		temp->Object = newElement;
		HeadNode = temp;
		size++;
	};
	GameObject* GetObject();
	//void Remove(GameObject* Element);

};


