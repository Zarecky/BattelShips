#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

const int N = 10;
const int COUNT_SHIPS = 10;

struct Ship {
	int startX;
	int startY;
	int size;
	bool isHorizontal;
};

struct Data {
	char area[N][N];
	Ship ships[COUNT_SHIPS];
	int countDestroyed = 0;
	int currentLength = 0;
};

void init();
bool update();
void draw();
bool inputCoords(int *x, int *y);
void input(int *x, int *y);
bool isHit(int x, int y, Data *data);
void setShips(Data *data);
void generateShip(const int sizeShip, Data *data);
void cleanStdin();
bool isDestroyShip(int x, int y, Data *data);
void applyBorderShip(int x, int y, Data *data);
bool playerTurn();
bool enemyTurn();
void end();

Data player;
Data enemy;

int main() {
	init();

	while (update());

	end();

	getchar();
	return 0;
}

void init() {
	for (int i = 0; i < N; i++) 
		for (int j = 0; j < N; j++) {
			player.area[i][j] = ' ';
			enemy.area[i][j] = ' ';
		}

	player.countDestroyed = 0;
	enemy.countDestroyed = 0;

	setShips(&enemy);
	setShips(&player);

	int countShips = 0;
	char x, y, orient;
}

bool update() {
	draw();

	while (playerTurn()) {
		if (enemy.countDestroyed >= COUNT_SHIPS) 
			return false;
	}

	while (enemyTurn()) {
		if (player.countDestroyed >= COUNT_SHIPS)
			return false;
	}

	draw();
	getchar();

	return true;
}

void draw() {
	system("cls");

	printf("\t\t\t\t\t***Sea Battle***\n\n");

	printf("\t\tYour Ships\t\t\t\t\t\tShips of Enemy\n\n   ");

	for (int i = 0; i < N; i++) printf("  %c ", 'A'+i);
	printf("\t\t  ");
	for (int i = 0; i < N; i++) printf("  %c ", 'A'+i);
	printf("\n   ");
	for (int i = 0; i < N; i++) printf("----");
	printf("-\t\t  ");
	for (int i = 0; i < N; i++) printf("----");
	printf("-\n");
	
	for (int k = 0; k < N-1; k++) {
		printf(" %d ", k + 1);
		for (int i = 0; i < N; i++) printf("| %c ", player.area[k][i]);
		printf("|\t\t%d ", k + 1);
		//for (int i = 0; i < N; i++) printf("| %c ", enemy.area[k][i]);
		for (int i = 0; i < N; i++) {
			if (enemy.area[k][i] == 'O') printf("|   ");
			else printf("| %c ", enemy.area[k][i]);
		}
		printf("|\n   ");
		for (int i = 0; i < N; i++) printf("----");
		printf("-\t\t  ");
		for (int i = 0; i < N; i++) printf("----");
		printf("-\n");
	}
	
	printf(" 10");
	for (int i = 0; i < N; i++) printf("| %c ", player.area[9][i]);
	printf("|\t\t10");
	//for (int i = 0; i < N; i++) printf("| %c ", enemy.area[9][i]);
	for (int i = 0; i < N; i++) {
		if (enemy.area[9][i] == 'O') printf("|   ");
		else printf("| %c ", enemy.area[9][i]);
	}
	printf("|\n   ");
	for (int i = 0; i < N; i++) printf("----");
	printf("-\t\t  ");
	for (int i = 0; i < N; i++) printf("----");
	printf("-\n\n");
}

bool inputCoords(int *x, int *y) {
	char c1, c2, c3;
	printf_s("Input fire coords: ");
	c1 = getchar();
	c2 = getchar();
	c3 = getchar();

	*x = c1 - 'A';
	if (*x < 0 || *x > 9) return false;

	if (c3 != '\n') {
		if (c2 == '1' && c3 == '0') *y = 9;
		else return false;
	}
	else *y = c2 - '1';
	if (*y < 0 || *y > 9) return false;

	return true;
}

void input(int *x, int *y) {
	bool turn;
	while (true) {
		draw();

		turn = inputCoords(x, y);

		if (!turn) {
			draw();
			printf_s("Error!\n");
			cleanStdin();
			continue;
		}

		if (enemy.area[*y][*x] == '.' || enemy.area[*y][*x] == '*') {
			draw();
			printf_s("You alredy inputed this coords!\n");
			cleanStdin();
			continue;
		}
		break;
	}
}

bool isHit(int x, int y, Data *data) {
	if ((*data).area[y][x] == 'O') return true;
	else return false;
}

void setShips(Data *data) {
	srand(time(NULL));

	generateShip(4, data);

	generateShip(3, data);
	generateShip(3, data);

	generateShip(2, data);
	generateShip(2, data);
	generateShip(2, data);

	generateShip(1, data);
	generateShip(1, data);
	generateShip(1, data);
	generateShip(1, data);
}

void generateShip(const int sizeShip, Data *data) {
	int x, y, checkStartX, checkEndX, checkStartY, checkEndY;
	bool isHor;

	char *debug = new char[(sizeShip + 2) * 3];
	int k;
	bool repeat;

	while (true) {
		repeat = false;

		x = rand() % N;
		y = rand() % N;
		isHor = (rand() % 2) == 0 ? true : false;

		if (x > N - sizeShip && isHor) x -= sizeShip;
		if (y > N - sizeShip && !isHor) y -= sizeShip;

		k = 0;

		if (isHor) {
			checkStartX = (x - 1) >= 0 ? x - 1 : x;
			checkEndX = (x + sizeShip) >= N ? x + sizeShip - 1 : x + sizeShip;
			checkStartY = (y - 1) >= 0 ? y - 1 : y;
			checkEndY = (y + 1) >= N ? y : y + 1;

			for (int i = checkStartY; i <= checkEndY; i++) {
				for (int j = checkStartX; j <= checkEndX; j++) {
					char c = (*data).area[i][j];
					debug[k] = c;
					k++;
					if ((*data).area[i][j] != ' ') {
						repeat = true;
						break;
					}
				}
				if (repeat) break;
			}

			if (repeat) continue;

			for (int i = x; i < x + sizeShip; i++)
				(*data).area[y][i] = 'O';
		} else {
			checkStartY = (y - 1) >= 0 ? y - 1 : y;
			checkEndY = (y + sizeShip) >= N ? y + sizeShip - 1: y + sizeShip;
			checkStartX = (x - 1) >= 0 ? x - 1 : x;
			checkEndX = (x + 1) >= N ? x : x + 1;

			for (int i = checkStartY; i <= checkEndY; i++) {
				for (int j = checkStartX; j <= checkEndX; j++) {
					char c = (*data).area[i][j];
					debug[k] = c;
					k++;
					if ((*data).area[i][j] != ' ') {
						repeat = true;
						break;
					}
				}
				if (repeat) break;
			}

			if (repeat) continue;

			for (int i = y; i < y + sizeShip; i++)
				(*data).area[i][x] = 'O';
		}

		(*data).ships[(*data).currentLength].isHorizontal = isHor;
		(*data).ships[(*data).currentLength].startX = x;
		(*data).ships[(*data).currentLength].startY = y;
		(*data).ships[(*data).currentLength].size = sizeShip;

		(*data).currentLength++;

		break;
	}

	free(debug);
}

bool isDestroyShip(int x, int y, Data *data) {
	for (int i = 0; i < COUNT_SHIPS; i++) {
		if ((*data).ships[i].isHorizontal) {
			if ((*data).ships[i].startX <= x && (*data).ships[i].startX + (*data).ships[i].size > x && (*data).ships[i].startY == y) {
				for (int j = (*data).ships[i].startX; j < (*data).ships[i].startX + (*data).ships[i].size; j++) {
					if ((*data).area[y][j] == 'O') 
						return false;
				}
				(*data).countDestroyed++;
				return true;
			}
		} else {
			if ((*data).ships[i].startY <= y && (*data).ships[i].startY + (*data).ships[i].size > y && (*data).ships[i].startX == x) {
				for (int j = (*data).ships[i].startY; j < (*data).ships[i].startY + (*data).ships[i].size; j++) {
					if ((*data).area[j][x] == 'O') 
						return false;
				}
				(*data).countDestroyed++;
				return true;
			}
		}
	}

	return false;
}

void applyBorderShip(int x, int y, Data *data) {
	int j;
	for (int i = 0; i < COUNT_SHIPS; i++) {
		if ((*data).ships[i].isHorizontal) {
			if ((*data).ships[i].startX <= x && (*data).ships[i].startX + (*data).ships[i].size > x && (*data).ships[i].startY == y) {
				x = (*data).ships[i].startX;
				y = (*data).ships[i].startY;

				if (x > 0) {
					(*data).area[y][x-1] = '.';
					if (y > 0) (*data).area[y-1][x-1] = '.';
					if (y < N-1) (*data).area[y+1][x-1] = '.';
				}

				for (j = (*data).ships[i].startX; j < (*data).ships[i].startX + (*data).ships[i].size; j++) {
					if (y > 0) (*data).area[y-1][j] = '.';
					if (y < N-1) (*data).area[y+1][j] = '.';
				}
			
				x = j - 1;

				if (x < N-1) {
					(*data).area[y][x + 1] = '.';
					if (y > 0) (*data).area[y - 1][x + 1] = '.';
					if (y < N - 1) (*data).area[y + 1][x + 1] = '.';
				}
			}
		}
		else {
			if ((*data).ships[i].startY <= y && (*data).ships[i].startY + (*data).ships[i].size > y && (*data).ships[i].startX == x) {
				x = (*data).ships[i].startX;
				y = (*data).ships[i].startY;

				if (y > 0) {
					(*data).area[y-1][x] = '.';
					if (x > 0) (*data).area[y - 1][x - 1] = '.';
					if (x < N - 1) (*data).area[y - 1][x + 1] = '.';
				}

				for (j = (*data).ships[i].startY; j < (*data).ships[i].startY + (*data).ships[i].size; j++) {
					if (x > 0) (*data).area[j][x-1] = '.';
					if (x < N-1) (*data).area[j][x+1] = '.';
				}

				y = j - 1;

				if (y < N-1) {
					(*data).area[y + 1][x] = '.';
					if (x > 0) (*data).area[y + 1][x - 1] = '.';
					if (x < N - 1) (*data).area[y + 1][x + 1] = '.';
				}
			}
		}
	}
}

void cleanStdin() {
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}

bool playerTurn() {
	int x, y;

	draw();

	input(&x, &y);
	if (isHit(x, y, &enemy)) {
		enemy.area[y][x] = '*';

		if (isDestroyShip(x, y, &enemy)) {
			applyBorderShip(x, y, &enemy);
			draw();
			printf_s("Destroyed!");
		}
		else {
			draw();
			printf_s("You hitted!\n");
		}

		getchar();
		return true;
	}
	else {
		enemy.area[y][x] = '.';
		draw();
		printf_s("You missed!\n");
		
		getchar();
		return false;
	}
}

bool enemyTurn() {
	srand(time(NULL));

	static int enemyX, enemyY;
	static int hitEnemyX, hitEnemyY;
	static int isHitEnemy = 0;
	static int choice;

	if (isHitEnemy == 0) {
		do {
			enemyX = rand() % N;
			enemyY = rand() % N;
		} while (player.area[enemyY][enemyX] == '.' || player.area[enemyY][enemyX] == '*');

		if (isHit(enemyX, enemyY, &player)) {
			player.area[enemyY][enemyX] = '*';
			if (isDestroyShip(enemyX, enemyY, &player)) {
				applyBorderShip(enemyX, enemyY, &player);
				isHitEnemy = 0;
				draw();
				printf_s("Enemy destroyed your ships...");
			}
			else {
				isHitEnemy = 1;
				hitEnemyX = enemyX;
				hitEnemyY = enemyY;
				draw();
				printf_s("Enemy hitted your ships...");
			}

			getchar();
			return true;
		}
		else {
			player.area[enemyY][enemyX] = '.';
			draw();
			printf_s("Enemy missed!");
		}
	}
	else if (isHitEnemy == 1) {
		do {
			choice = rand() % 4;
			switch (choice) {
			case 0:
				enemyX = hitEnemyX - 1;
				enemyY = hitEnemyY;
				break;
			case 1:
				enemyX = hitEnemyX + 1;
				enemyY = hitEnemyY;
				break;
			case 2:
				enemyX = hitEnemyX;
				enemyY = hitEnemyY - 1;
				break;
			case 3:
				enemyX = hitEnemyX;
				enemyY = hitEnemyY + 1;
				break;
			}
		} while ((enemyX < 0 || enemyX >= N) && (enemyY < 0 || enemyY >= N) || player.area[enemyY][enemyX] == '.' || player.area[enemyY][enemyX] == '*');

		if (isHit(enemyX, enemyY, &player)) {
			player.area[enemyY][enemyX] = '*';
			if (isDestroyShip(enemyX, enemyY, &player)) {
				applyBorderShip(enemyX, enemyY, &player);
				isHitEnemy = 0;
				draw();
				printf_s("Enemy destroyed your ships...");
			}
			else {
				isHitEnemy = 2;
				draw();
				printf_s("Enemy hitted your ships...");
			}

			getchar();
			return true;
		}
		else {
			player.area[enemyY][enemyX] = '.';
			draw();
			printf_s("Enemy missed!");
		}
	}
	else if (isHitEnemy == 2) {
		switch (choice) {
		case 0: enemyX--; break;
		case 1: enemyX++; break;
		case 2: enemyY--; break;
		case 3: enemyY++; break;
		}

		if (isHit(enemyX, enemyY, &player)) {
			player.area[enemyY][enemyX] = '*';
			if (isDestroyShip(enemyX, enemyY, &player)) {
				applyBorderShip(enemyX, enemyY, &player);
				isHitEnemy = 0;
				draw();
				printf_s("Enemy destroyed your ships...");
			}
			else {
				switch (choice) {
				case 0: if (enemyX - 1 < 0) isHitEnemy = 3; break;
				case 1: if (enemyX + 1 >= N) isHitEnemy = 3; break;
				case 2: if (enemyY - 1 < 0) isHitEnemy = 3; break;
				case 3: if (enemyY + 1 >= N) isHitEnemy = 3; break;
				}

				draw();
				printf_s("Enemy hitted your ships...");
			}

			getchar();
			return true;
		}
		else {
			player.area[enemyY][enemyX] = '.';
			isHitEnemy = 3;
			enemyX = hitEnemyX;
			enemyY = hitEnemyY;
			draw();
			printf_s("Enemy missed!");
		}
	}
	else if (isHitEnemy == 3) {
		switch (choice) {
		case 0: enemyX++; break;
		case 1: enemyX--; break;
		case 2: enemyY++; break;
		case 3: enemyY--; break;
		}

		if (isHit(enemyX, enemyY, &player)) {
			player.area[enemyY][enemyX] = '*';
			if (isDestroyShip(enemyX, enemyY, &player)) {
				applyBorderShip(enemyX, enemyY, &player);
				isHitEnemy = 0;
				draw();
				printf_s("Enemy destroyed your ships...");
			}
			else {
				draw();
				printf_s("Enemy hitted your ships...");
			}

			getchar();
			return true;
		}
	}

	getchar();
	return false;
}

void end() {
	if (enemy.countDestroyed >= COUNT_SHIPS)
		printf_s("\t\tYOU WIN!!!");
	else printf_s("\t\tYOU LOSE...");
}