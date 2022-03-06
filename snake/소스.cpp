#include <iostream>
#include <vector>
#include <cstdlib> // rand
#include <ctime> // time
#include <windows.h>
#include <conio.h>
using namespace std;

#define MAIN_X 20
#define MAIN_Y 20
#define LEFT 75 // ←
#define RIGHT 77  // →
#define UP 72 // ↑
#define DOWN 80 // ↓

void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

class Snake {
private:
	int snakeShape[4];
	vector<vector<int>> position;
	int headIndex;
	char head[4]; // ▶, ▼, ◀ , ▲
	// →, ↓, ←, ↑  77 80 75 72
public:
	Snake() : headIndex(0) // head{ '▶', '▼', '◀' , '▲' }, 
	{
		position.push_back({1, 3}); // 1
		position.push_back({1, 2}); // 2 
		position.push_back({1, 1}); // 3
	}
	
	int getSnakeShape(int headIndex) {
		return snakeShape[headIndex];
	}
	int getHeadIndex() {
		return headIndex;
	}
	char getHead(int headIndex) {
		return head[headIndex];
	}
	vector<vector<int>> getSnakePosition() {
		return position;
	}
	void setHead(int headIndex) {
		this->headIndex = headIndex;
	}
	void addTail() {
		switch (headIndex){
		case 0: //RIGHT
		{
			int len = position.size();
			vector<int> temp = {position[len][0], position[len][1] - 1};
			position.push_back(temp);
			break;
		}
		case 1: //DOWN
		{
			int len = position.size();
			vector<int> temp = {position[len][0] - 1, position[len][1]};
			position.push_back(temp);
			break;
		}
		case 2: //LEFT
		{
			int len = position.size();
			vector<int> temp = { position[len][0], position[len][1] + 1};
			position.push_back(temp);
			break;
		}
		case 3: //UP
		{
			int len = position.size();
			vector<int> temp = { position[len][0] + 1, position[len][1] };
			position.push_back(temp);
		}
		}
	}
	void goRight() {
		headIndex = 0;
		int temp[2] = {position[0][0], position[0][1]};
		position.pop_back();
		position.insert(position.begin(), {temp[0], temp[1] + 1});
	}
	void goDown() {
		headIndex = 1;
		int temp[2] = {position[0][0], position[0][1]};
		position.pop_back();
		position.insert(position.begin(), {temp[0] + 1, temp[1]});
	}
	void goLeft() {
		headIndex = 2;
		int temp[2] = {position[0][0], position[0][1]};
		position.pop_back();
		position.insert(position.begin(), {temp[0], temp[1] - 1});
	}
	void goUp() {
		headIndex = 3;
		int temp[2] = {position[0][0], position[0][1]};
		position.pop_back();
		position.insert(position.begin(), {temp[0] - 1, temp[1]});
	}
};

class GameTable {
private:
	int x;
	int y;
	Snake* snakeObject;
	vector<vector<int>> table; // 0 = "  ", 1 = "▦", 2 = "▲", 3 = "■", 4 = "★"
public:
	GameTable(): x(MAIN_X), y(MAIN_Y) {
		snakeObject = nullptr;
		for (int i = 0; i < MAIN_Y; ++i) {
			vector<int> temp;
			for (int j = 0; j < MAIN_X; ++j) {
				temp.push_back(0);
			}
			table.push_back(temp);
		}
		for (int i = 0; i < MAIN_X; ++i) {
			table[0][i] = 1; // top
			table[MAIN_X - 1][i] = 1; // down
		}
		for (int j = 1; j < MAIN_Y - 1; ++j) {
			table[j][0] = 1;
			table[j][MAIN_Y - 1] = 1;
		}
	};
	
	void DrawGameTable() {
		for (int i = 0; i < MAIN_X; ++i) {
			for (int j = 0; j < MAIN_Y; ++j) {
				if (table[j][i] == 1) cout << "▦";
				else if (table[j][i] == 2) cout << snakeObject->getHead(snakeObject->getHeadIndex());
				else if (table[j][i] == 3) cout << "■";
				else if (table[j][i] == 4) cout << "★";
				else cout << "  ";
			}
			cout << "\n";
		}
	}
	void createStar() {
		srand((unsigned int)time(NULL));
		int starX = NULL; int starY = NULL;
		while (table[starX][starY] != 0) {
			starX = rand() % 18 + 1;
			starY = rand() % 18 + 1;
		}
		table[starX][starY] = 3;
	}

	void createSnake() {
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		table[temp[0][0]][temp[0][1]] = 2;
		for (int i = 1; i < temp.size(); ++i)
			table[temp[i][0]][temp[i][1]] = 3;
		
		for (int i = 0; i < MAIN_X; ++i) {
			for (int j = 0; j < MAIN_Y; ++j) {
				if (table[j][i] == 1) cout << "▦";
				else if (table[j][i] == 2) cout << snakeObject->getHead(snakeObject->getHeadIndex());
				else if (table[j][i] == 3) cout << "■";
				else if (table[j][i] == 4) cout << "★";
				else cout << "  ";
			}
		}
	}

	void moveSnake(int key) { // position의 end 값을 begin에 insert해줌. 단, direction에 따라서 값이 변경되어 들어감.
		if (key == RIGHT) snakeObject->goRight();
		else if (key == DOWN) snakeObject->goDown();
		else if (key == LEFT) snakeObject->goLeft();
		else if (key == UP) snakeObject->goUp();

		for (int i = 0; i < MAIN_X; ++i) {
			for (int j = 0; j < MAIN_Y; ++j) {
				if (table[j][i] == 1) cout << "▦";
				else if (table[j][i] == 2) cout << snakeObject->getHead(snakeObject->getHeadIndex());
				else if (table[j][i] == 3) cout << "■";
				else if (table[j][i] == 4) cout << "★";
				else cout << "  ";
			}
		}
	}

	void GetStar() {

	}
};


int main() {
	//GameTable gtable;
	//gtable.DrawGameTable();
	Snake checkSnake;
	vector<vector<int>> v = checkSnake.getSnakePosition();

	cout << v[0][0] << "\t" << v[0][1] << endl;
	cout << v[1][0] << "\t" << v[1][1] << endl;
	cout << v[2][0] << "\t" << v[2][1] << endl;
}