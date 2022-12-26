#include "raylib.h"
#include "utility.h"
#include "board.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>

int status = 1;

//status = 0  exit
//status = 1  menu
//status = 2  loading
//status = 3  game
//status = 4  lost
//status = 5  won
//status = 6  ranking

int main()
{
	srand(time(NULL));

	bool gameIsRunning = 1;

	int mode = 0;
	//mode = 1 easy
	//mode = 2 med
	//mode = 3 hard

	int sizeX = MEDX;
	int sizeY = MEDY;
	int bombs = MEDB;

	int menuHeight = 2 * POLE + SPACE;
	int screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + 2 * menuHeight;
	int screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

	int flagsSet = 0;
	int* revealed;
	int rev = sizeX * sizeY;
	revealed = &rev;

	double* timeStart;
	double* timeStop;
	double* timeTemp;

	int coX, coY;
	Cell** grid = NULL;

	Rectangle easyRec = { screenWidth / 6    , 6.2 * screenHeight / 12, 2 * screenWidth / 3, screenHeight / 18 };
	Rectangle medRec  = { screenWidth / 6    , 7.2 * screenHeight / 12, 2 * screenWidth / 3, screenHeight / 18 };
	Rectangle hardRec = { screenWidth / 6    , 8.2 * screenHeight / 12, 2 * screenWidth / 3, screenHeight / 18 };
	Rectangle rankRec = { screenWidth / 6 - 4, 9.6 * screenHeight / 12, 76, 54 };

	int menu = 0;

	std::ifstream ranking;
	ranking.open("ranking.txt");
	int** nRanking=NULL;
	nRanking = fnMemoryAllocInt(3, 3, nRanking);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ranking >> nRanking[i][j];
		}
	}
	ranking.close();

	//-GAME-----------------------------------------------------------------------------

	while (gameIsRunning)
	{
		//-MENU-----------------------------------------------------------------------------

		OpenWindow("Menu");

		LoadTexturesMenu();
		LoadFonts();

		while (status==1 || status == 6)
		{
			while (status == 1)
			{
				if (CheckCollisionPointRec(GetMousePosition(), easyRec)) menu = 1;
				if (CheckCollisionPointRec(GetMousePosition(), medRec )) menu = 2;
				if (CheckCollisionPointRec(GetMousePosition(), hardRec)) menu = 3;
				if (CheckCollisionPointRec(GetMousePosition(), rankRec)) menu = 4;

				DrawMenu(menu);
				menu = 0;

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					if (CheckCollisionPointRec(GetMousePosition(), easyRec)) { status = 2; mode = 1; }
					if (CheckCollisionPointRec(GetMousePosition(), medRec )) { status = 2; mode = 2; }
					if (CheckCollisionPointRec(GetMousePosition(), hardRec)) { status = 2; mode = 3; }
					if (CheckCollisionPointRec(GetMousePosition(), rankRec)) { status = 6; }
				}

				if (WindowShouldClose())
				{
					status = 0;
					break;
				}
			}

			while (status == 6)
			{
				DrawRanking(nRanking);

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					if (CheckCollisionPointRec(GetMousePosition(), rankRec)) { status = 1; }
				}

				if (WindowShouldClose())
				{
					status = 0;
					break;
				}
			}
		}

		UnloadTexturesMenu();
		UnloadFonts();

		if(status != 0) CloseWindow();

		//-LOADING--------------------------------------------------------------------------

		switch (mode)
		{
		case 1: sizeEasy(&sizeX, &sizeY, &bombs); break;

		case 2: sizeMed(&sizeX, &sizeY, &bombs); break;

		case 3: sizeHard(&sizeX, &sizeY, &bombs); break;

		default: break;
		}

		menuHeight = 2 * POLE + SPACE;
		screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + 2 * menuHeight;
		screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

		*revealed = sizeX * sizeY;

		if (status != 0)
		{
			OpenWindow("Game");
		}

		grid = fnMemoryAlloc(sizeX, sizeY, grid);
		CellIndex(grid, sizeX, sizeY);

		timeStart = (double*)calloc(1, sizeof(double));
		if (timeStart == NULL) exit(-1);
		timeStop = (double*)calloc(1, sizeof(double));
		if (timeStop == NULL) exit(-1);
		timeTemp = (double*)calloc(1, sizeof(double));
		if (timeTemp == NULL) exit(-1);
		*timeTemp = 0;
		*timeStart = 0;

		int startX = 0;
		int startY = 0;

		//-GAME-FIRST-MOVE-----------------------------------------------------------------

		LoadTexturesGame();

		while (status==2)
		{
			BeginDrawing();
			ClearBackground(BLACK);

			DrawTaskbar((bombs - flagsSet), (*timeTemp - *timeStart) / CLOCKS_PER_SEC);

			for (int i = 0; i < sizeX; i++)
			{
				for (int j = 0; j < sizeY; j++)
				{
					CellDraw(grid, i, j, status);
				}
			}

			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				startX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
				startY = (GetMouseY() - menuHeight - SPACE) / (POLE + SPACE);
				status = 3;
			}

			if (WindowShouldClose())
			{
				status = 0;
				break;
			}
		}

		do //Setting up the board
		{
			CellSetUJ(sizeX, sizeY, bombs, grid, startX, startY);
			CellFillNumbers(sizeX, sizeY, grid);
		} while (grid[startX][startY].bombsAround > 0 || grid[startX][startY].isBomb == true); //first square must have 0 bombs around

		CellReveal(grid, startX, startY, sizeX, sizeY, &status, revealed);

		*timeStart = clock();

		//-GAME-----------------------------------------------------------------------------

		while (status==3)
		{
			BeginDrawing();
			ClearBackground(BLACK);

			DrawTaskbar((bombs-flagsSet), (*timeTemp-*timeStart)/CLOCKS_PER_SEC);

			for (int i = 0; i < sizeX; i++)
			{
				for (int j = 0; j < sizeY; j++)
				{
					CellDraw(grid, i, j, status);
				}
			}

			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				coX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
				coY = (GetMouseY() - menuHeight - 2*SPACE) / (POLE + SPACE);

				if (CheckIndex(coX, coY, sizeX, sizeY)) CellReveal(grid, coX, coY, sizeX, sizeY, &status, revealed);
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
				coX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
				coY = (GetMouseY() - menuHeight - 2*SPACE) / (POLE + SPACE);

				if (CheckIndex(coX, coY, sizeX, sizeY) && grid[coX][coY].isRevealed == false)
				{
					if (grid[coX][coY].isFlag == false && bombs - flagsSet > 0) { grid[coX][coY].isFlag = true ; flagsSet++; }
					else if (grid[coX][coY].isFlag == true ) { grid[coX][coY].isFlag = false; flagsSet--; }
				}
			}

			if(status != 4) CheckWin(bombs, *revealed, &status);

			*timeTemp = clock();

			if (WindowShouldClose())
			{
				status = 0;
				break;
			}

		}

		*timeStop = clock();

		//-LOST-SCREEN----------------------------------------------------------------------

		while (status == 4)
		{
			BeginDrawing();
			ClearBackground(BLACK);

			DrawTaskbar((bombs - flagsSet), (*timeStop - *timeStart) / CLOCKS_PER_SEC);

			DrawText(TextFormat("Przegrales"), screenWidth / 2 - 86, screenHeight - menuHeight + 12, 1.5 * POLE, RED);

			for (int i = 0; i < sizeX; i++)
			{
				for (int j = 0; j < sizeY; j++)
				{
					CellDraw(grid, i, j, status);
				}
			}
			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) status = 1;

			if (WindowShouldClose())
			{
				status = 0;
				break;
			}
		}

		//-WIN-SCREEN-----------------------------------------------------------------------

		if (status == 5)
		{
			updateRanks((*timeStop - *timeStart) / CLOCKS_PER_SEC, nRanking, mode);
		}

		while (status == 5)
		{
			BeginDrawing();
			ClearBackground(BLACK);

			DrawTaskbar(0, (*timeStop - *timeStart) / CLOCKS_PER_SEC);

			DrawText(TextFormat("Wygrales"), screenWidth / 2 - 72, screenHeight - menuHeight + 12, 1.5 * POLE, GREEN);

			for (int i = 0; i < sizeX; i++)
			{
				for (int j = 0; j < sizeY; j++)
				{
					CellDraw(grid, i, j, status);
				}
			}
			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) status = 1;

			if (WindowShouldClose())
			{
				status = 0;
				break;
			}
		}

		//-CLEAR-BOARD----------------------------------------------------------------------

		UnloadTexturesGame();

		CloseWindow();

		fnMemoryFree(sizeX, grid);
		free(timeStart);
		free(timeStop);
		free(timeTemp);

		sizeMed(&sizeX, &sizeY, &bombs);
		menuHeight = 2 * POLE + SPACE;
		screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + 2 * menuHeight;
		screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

		flagsSet = 0;
		*revealed = sizeX*sizeY;

		if (status == 0) gameIsRunning = false;
		else status = 1;
	}

	std::ofstream newRanking;
	newRanking.open("ranking.txt");

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newRanking << nRanking[i][j] << std::endl;
		}
	}
	newRanking.close();
	fnMemoryFreeInt(3, nRanking);

	return 0;
}

