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
#define LEFT 75 // ��
#define RIGHT 77  // ��
#define UP 72 // ��
#define DOWN 80 // ��

#define PER_POINT 100 // ���� ���� ������ �⺻ ���� 100���� �߰�. ���� ������ �׿� �°� ���� �� �߰�

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
	string head[4]; // ��, ��, �� , ��
	// ��, ��, ��, ��  77 80 75 72
	int headIndex;
public:
	Snake() : headIndex(0), head{ "��", "��", "��" , "��" }
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
		cout << "�Ӹ� ��ġ" << endl;
		cout << temp[0] << ", " << temp[1] + 1 << endl;
		position.insert(position.begin(), { temp[0], temp[1] + 1 });
		cout << "------------" << endl;
		cout << "���� ��ġ" << endl;
		cout << position[position.size() - 1][0] << ", " << position[position.size() - 1][1] << endl;
	}
	void goDown() {
		headIndex = 1;
		int temp[2] = { position[0][0], position[0][1] };
		cout << "�Ӹ� ��ġ" << endl;
		cout << temp[0] + 1 << ", " << temp[1] << endl;
		position.insert(position.begin(), { temp[0] + 1, temp[1] });
		cout << "------------" << endl;
		cout << "���� ��ġ" << endl;
		cout << position[position.size() - 1][0] << ", " << position[position.size() - 1][1] << endl;
	}
	void goLeft() {
		headIndex = 2;
		int temp[2] = { position[0][0], position[0][1] };
		cout << "�Ӹ� ��ġ" << endl;
		cout << temp[0] << ", " << temp[1] - 1 << endl;
		position.insert(position.begin(), { temp[0], temp[1] - 1 });
		cout << "------------" << endl;
		cout << "���� ��ġ" << endl;
		cout << position[position.size() - 1][0] << ", " << position[position.size() - 1][1] << endl;
	}
	void goUp() {
		headIndex = 3;
		int temp[2] = { position[0][0], position[0][1] };
		cout << "�Ӹ� ��ġ" << endl;
		cout << temp[0] - 1 << ", " << temp[1] << endl;
		position.insert(position.begin(), { temp[0] - 1, temp[1] });
		cout << "------------" << endl;
		cout << "���� ��ġ" << endl;
		cout << position[position.size() - 1][0] << ", " << position[position.size() - 1][1] << endl;
	}
};

class Score { // �ʱ� ������ �����ϰ� �� ����.
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
	vector<vector<int>> table; // 0 = "  ", 1 = "��", 2 = "��", 3 = "��", 4 = "��"
public:
	GameTable() : x(MAIN_X), y(MAIN_Y), delayTime(500) { // �⺻ table ������ִ� ������
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

	Snake* GetSnakeObject() { // snakeObject get �Լ�
		return snakeObject;
	}

	vector<vector<int>> GetTable() { // table get �Լ�
		return table;
	}

	void DrawGameTable() { // ����� table �������� GameTable �׷��ִ� �Լ�
		for (int i = 0; i < MAIN_X; ++i) {
			for (int j = 0; j < MAIN_Y; ++j) {
				if (table[i][j] == 1) cout << "��";
				else if (table[i][j] == 2) cout << snakeObject->getHead(snakeObject->getHeadIndex());
				else if (table[i][j] == 3) cout << "��";
				else if (table[i][j] == 4) cout << "��";
				else if (table[i][j] == 0) cout << "  ";
			}
			cout << "\n";
		}
	}

	void CreateSnake() { // ó���� Snake ������ִ� �Լ�
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		table[temp[0][0]][temp[0][1]] = 2;
		for (int i = 1; i < temp.size(); ++i)
			table[temp[i][0]][temp[i][1]] = 3;
	}

	void MoveSnake(int key) { // position�� end ���� begin�� insert����. ��, direction�� ���� ���� ����Ǿ� ��.
		if (key == RIGHT) snakeObject->goRight();
		else if (key == DOWN) snakeObject->goDown();
		else if (key == LEFT) snakeObject->goLeft();
		else if (key == UP) snakeObject->goUp();
	}

	void UpdateSnake_Normal() { // ����, ����� Snake�� position�� �ݿ�
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		table[temp[0][0]][temp[0][1]] = 2; // new �Ӹ�		
		int len = temp.size(); // len = last index + 1
		table[temp[1][0]][temp[1][1]] = 3; // old �Ӹ�
		table[temp[len - 1][0]][temp[len - 1][1]] = 0;
		snakeObject->deleteTail(); // position���� �����κе� ����
	}

	void UpdateSnake_GetStar() { // GetStar �� ���, ����� Snake�� position�� �ݿ�
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		table[temp[0][0]][temp[0][1]] = 2; // new �Ӹ�		
		int len = temp.size(); // len = last index + 1
		table[temp[1][0]][temp[1][1]] = 3; // old �Ӹ�
		table[temp[len - 1][0]][temp[len - 1][1]] = 3;
	}

	void GetStar() { // �� ȹ���� ��
		for (int i = 1; i < MAIN_X - 1; ++i) {
			for (int j = 1; j < MAIN_Y - 1; ++j) {
				if (table[i][j] == 4) table[i][j] = 0;
			}
		}
		gameScore->AddGotStar();
		gameScore->AddScore();
		CreateStar();
	}

	int TouchStar() { // ���� snake�� ��´�?
		vector<vector<int>> temp = snakeObject->getSnakePosition();
		if (table[temp[0][0]][temp[0][1]] == 4)
			return 1;
		else if (table[temp[0][0]][temp[0][1]] != 4)
			return 0;
	}

	void CreateStar() { // ���� ��ġ�� �� ����
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
			if (_kbhit()) { // keyboard �ԷµǾ�� ����
				while (true) {
					int nSelect1;
					int nSelect2;
					for (;;) { // Sleep�� �������� �ݺ�
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

						else { // ���� nSelect�� ������
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
						gotoxy(0, 0); //system("cls") ���
						gt->DrawGameTable(); // �ٽ� �׸���
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

// touch body / touch wall / show score / time auto move ����