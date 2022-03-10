#include <iostream>
#include <vector>
#include <string>
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

#define PER_POINT 100 // 별을 먹을 때마다 기본 점수 100점씩 추가. 별이 많으면 그에 맞게 점수 더 추가

void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

class Snake {
private:
	vector<vector<int>> position;
	string head[4]; // ▶, ▼, ◀ , ▲
	// →, ↓, ←, ↑  77 80 75 72
	int headIndex;
public:
	Snake() : headIndex(0), head{ "▶", "▼", "◀" , "▲" }
	{
		position.push_back({ 1, 3 }); // 1 
		position.push_back({ 1, 2 }); // 2 
		position.push_back({ 1, 1 }); // 3
	}
	int getHeadIndex() {
		return headIndex;
	}
	string getHead(int headIndex) {
		return head[headIndex];
	}
	vector<vector<int>> getSnakePosition() {
		return position;
	}
	void setHead(int headIndex) {
		this->headIndex = headIndex;
	}
	void deleteTail() {
		position.pop_back();
	}
	void goRight() {
		headIndex = 0;
		int temp[2] = { position[0][0], position[0][1] };
		cout << "머리 위치" << endl;
		cout << temp[0] << ", " << temp[1] + 1 << endl;
		position.insert(position.begin(), { temp[0], temp[1] + 1 });
		cout << "------------" << endl;
		cout << "꼬리 위치" << endl;
		cout << position[position.size() - 1][0] << ", " << position[position.size() - 1][1] << endl;
	}
	void goDown() {
		headIndex = 1;
		int temp[2] = { position[0][0], position[0][1] };
		cout << "머리 위치" << endl;
		cout << temp[0] + 1 << ", " << temp[1] << endl;
		position.insert(position.begin(), { temp[0] + 1, temp[1] });
		cout << "------------" << endl;
		cout << "꼬리 위치" << endl;
		cout << position[position.size() - 1][0] << ", " << position[position.size() - 1][1] << endl;
	}
	void goLeft() {
		headIndex = 2;
		int temp[2] = { position[0][0], position[0][1] };
		cout << "머리 위치" << endl;
		cout << temp[0] << ", " << temp[1] - 1 << endl;
		position.insert(position.begin(), { temp[0], temp[1] - 1 });
		cout << "------------" << endl;
		cout << "꼬리 위치" << endl;
		cout << position[position.size() - 1][0] << ", " << position[position.size() - 1][1] << endl;
	}
	void goUp() {
		headIndex = 3;
		int temp[2] = { position[0][0], position[0][1] };
		cout << "머리 위치" << endl;
		cout << temp[0] - 1 << ", " << temp[1] << endl;
		position.insert(position.begin(), { temp[0] - 1, temp[1] });
		cout << "------------" << endl;
		cout << "꼬리 위치" << endl;
		cout << position[position.size() - 1][0] << ", " << position[position.size() - 1][1] << endl;
	}
};

class Score { // 초기 버전은 간단하게 값 산정.
private:
	int score;
	int gotStar;
public:
	Score() : score(0), gotStar(0) {}
	int GetScore() {
		return score;
	}
	int GetGotStar() {
		return gotStar;
	}
	void AddScore() {
		score += PER_POINT * gotStar;
	}
	void AddGotStar() {
		gotStar += 1;
	}
};


class GameTable {
private:
	int x;
	int y;
	int delayTime;
	Snake* snakeObject;
	Score* gameScore;
	vector<vector<int>> table; // 0 = "  ", 1 = "▦", 2 = "▲", 3 = "■", 4 = "★"
public:
	GameTable() : x(MAIN_X), y(MAIN_Y), delayTime(500) { // 기본 table 만들어주는 생성자
		snakeObject = new Snake();
		gameScore = new Score();
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

	int GetDelayTime() {
		return delayTime;
	}

	Snake* GetSnakeObject() { // snakeObject get 함수
		return snakeObject;
	}

	vector<vector<int>> GetTable() { // table get 함수
		return table;
	}

	void DrawGameTable() { // 변경된 table 바탕으로 GameTable 그려주는 함수
		for (int i = 0; i < MAIN_X; ++i) {
			for (int j = 0; j < MAIN_Y; ++j) {
				if (table[i][j] == 1) cout << "▦";
				else if (table[i][j] == 2) cout << snakeObject->getHead(snakeObject->getHeadIndex());
				else if (table[i][j] == 3) cout << "■";
				else if (table[i][j] == 4) cout << "★";
				else if (table[i][j] == 0) cout << "  ";
			}
			cout << "\n";
		}
	}

	void CreateSnake() { // 처음에 Snake 만들어주는 함수
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		table[temp[0][0]][temp[0][1]] = 2;
		for (int i = 1; i < temp.size(); ++i)
			table[temp[i][0]][temp[i][1]] = 3;
	}

	void MoveSnake(int key) { // position의 end 값을 begin에 insert해줌. 단, direction에 따라서 값이 변경되어 들어감.
		if (key == RIGHT) snakeObject->goRight();
		else if (key == DOWN) snakeObject->goDown();
		else if (key == LEFT) snakeObject->goLeft();
		else if (key == UP) snakeObject->goUp();
	}

	void UpdateSnake_Normal() { // 평상시, 변경된 Snake를 position에 반영
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		table[temp[0][0]][temp[0][1]] = 2; // new 머리		
		int len = temp.size(); // len = last index + 1
		table[temp[1][0]][temp[1][1]] = 3; // old 머리
		table[temp[len - 1][0]][temp[len - 1][1]] = 0;
		snakeObject->deleteTail(); // position에서 꼬리부분도 삭제
	}

	void UpdateSnake_GetStar() { // GetStar 한 경우, 변경된 Snake를 position에 반영
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		table[temp[0][0]][temp[0][1]] = 2; // new 머리		
		int len = temp.size(); // len = last index + 1
		table[temp[1][0]][temp[1][1]] = 3; // old 머리
		table[temp[len - 1][0]][temp[len - 1][1]] = 3;
	}

	void GetStar() { // 별 획득한 후
		for (int i = 1; i < MAIN_X - 1; ++i) {
			for (int j = 1; j < MAIN_Y - 1; ++j) {
				if (table[i][j] == 4) table[i][j] = 0;
			}
		}
		gameScore->AddGotStar();
		gameScore->AddScore();
		CreateStar();
	}

	int TouchStar() { // 별에 snake가 닿는다?
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		if (table[temp[0][0]][temp[0][1]] == 4)
			return 1;
		else if (table[temp[0][0]][temp[0][1]] != 4)
			return 0;
	}

	void CreateStar() { // 랜덤 위치에 별 생성
		srand((unsigned int)time(NULL));
		int starX = NULL; int starY = NULL;
		while (table[starX][starY] != 0) {
			starX = rand() % 18 + 1;
			starY = rand() % 18 + 1;
		}
		table[starX][starY] = 4;
	}

	void ShowScore()
	{
		gotoxy(50, 3);
		cout << "Score: " << gameScore->GetScore() << " ";
		cout << "Gained Star: " << gameScore->GetGotStar();
	}

	int TouchMyBody(int direction) {
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		switch (direction) {
		case UP:
			if (table[temp[0][0] - 1][temp[0][1]] == 3)
				return 1;
			else
				return 0;
			break;
		case RIGHT:
			if (table[temp[0][0]][temp[0][1] + 1] == 3)
				return 1;
			else
				return 0;
			break;
		case DOWN:
			if (table[temp[0][0] + 1][temp[0][1]] == 3)
				return 1;
			else
				return 0;
			break;
		case LEFT:
			if (table[temp[0][0]][temp[0][1] - 1] == 3)
				return 1;
			else
				return 0;
			break;
		default:
			break;
		}
	}

	int TouchWall() {
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		if (table[temp[0][0]][temp[0][1]] == 1)
			return 1;
		else
			return 0;
	}

	void AutoDelayTime() {
		delayTime -= 5;
	}
};

class GamePlay {
private:
	GameTable* gt;
public:
	GamePlay() {
		gt = new GameTable();
		gt->CreateSnake();
		gt->CreateStar();
		gt->DrawGameTable();
		while (true) {
			if (_kbhit()) { // keyboard 입력되어야 시작
				while (true) {
					int nSelect1;
					int nSelect2;
					for (;;) { // Sleep을 기준으로 반복
						Sleep(gt->GetDelayTime());
						if (_kbhit()) {
							nSelect1 = _getch();
							if (nSelect1 == 224) {
								nSelect2 = _getch();
								if (gt->TouchMyBody(nSelect2)) {
									cout << "Touched Body!" << endl;
									break;
								}
								switch (nSelect2) {
								case UP:
									gt->MoveSnake(UP);
									break;
								case DOWN:
									gt->MoveSnake(DOWN);
									break;
								case LEFT:
									gt->MoveSnake(LEFT);
									break;
								case RIGHT:
									gt->MoveSnake(RIGHT);
									break;
								default:
									break;
								}
							}
							if (gt->TouchWall()) {
								cout << "Touched Wall!" << endl;
								break;
							}
							if (gt->TouchStar()) {
								cout << "Touched!" << endl;
								gt->AutoDelayTime();
								gt->GetStar();
								gt->UpdateSnake_GetStar();
							}
							else {
								cout << "Not Touched" << endl;
								gt->UpdateSnake_Normal();
							}
						}

						else { // 기존 nSelect로 움직임
							if (nSelect1 == 224) {
								if (gt->TouchMyBody(nSelect2)) {
									cout << "Touched Body!" << endl;
									break;
								}
								switch (nSelect2) {
								case UP:
									gt->MoveSnake(UP);
									break;
								case DOWN:
									gt->MoveSnake(DOWN);
									break;
								case LEFT:
									gt->MoveSnake(LEFT);
									break;
								case RIGHT:
									gt->MoveSnake(RIGHT);
									break;
								default:
									break;
								}
							}
							if (gt->TouchWall()) {
								cout << "Touched Wall!" << endl;
								break;
							}
							if (gt->TouchStar()) {
								cout << "Touched!" << endl;
								gt->AutoDelayTime();
								gt->GetStar();
								gt->UpdateSnake_GetStar();
							}
							else {
								cout << "Not Touched" << endl;
								gt->UpdateSnake_Normal();
							}
						}
						gt->ShowScore();
						gotoxy(0, 0); //system("cls") 대신
						gt->DrawGameTable(); // 다시 그리기
					}
					system("cls");
					gotoxy(MAIN_X / 2, MAIN_Y / 4);
					cout << "GAME OVER!" << endl;
					cout << "\n\n\n\n" << endl;
					break;
				}
			}
		}
	}
	~GamePlay() {
		delete gt;
	}
};

int main() {
	GamePlay gt;

	return 0;
}

// touch body / touch wall / show score / time auto move 구현