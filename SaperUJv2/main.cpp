#include "raylib.h"
#include "utility.h"
#include "board.h"
#include <iostream>
#include <time.h>

int status = 1;

//status = 0  exit
//status = 1  menu
//status = 2  loading
//status = 3  game
//status = 4  lost
//status = 5  won

int main()
{
	srand(time(NULL));

	bool windowIsOpen = 1;

	int sizeX = 26;
	int sizeY = 38;
	int bombs = 30;

	const int menuHeight = 2 * POLE + SPACE;
	const int screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + menuHeight;
	const int screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

	int flagsSet = 0;

	int* revealed;
	int rev = sizeX * sizeY;
	revealed = &rev;

	double* timeStart;
	double* timeStop;

	int coX, coY;
	Cell** grid = NULL;

	InitWindow(screenWidth, screenHeight, "Samper");

	Rectangle easyRec = { screenWidth / 4,       screenHeight / 6, screenWidth / 2, screenHeight / 6 };
	Rectangle medRec = { screenWidth / 4, 2.5 * screenHeight / 6, screenWidth / 2, screenHeight / 6 };
	Rectangle hardRec = { screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6 };

	SetTargetFPS(120);

	while (windowIsOpen)
	{
		//-MENU-----------------------------------------------------------------------------

		while (status==1)
		{
			BeginDrawing();

			ClearBackground(BLACK);
			DrawRectangle(screenWidth / 4, screenHeight / 6, screenWidth / 2, screenHeight / 6, GREEN);
			DrawRectangle(screenWidth / 4, 2.5 * screenHeight / 6, screenWidth / 2, screenHeight / 6, YELLOW);
			DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6, RED);

			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if (CheckCollisionPointRec(GetMousePosition(), hardRec)) status = 2;
			}

			if (WindowShouldClose())
			{
				status = 0;
				break;
			}
		}

		//-LOADING--------------------------------------------------------------------------

		grid = fnMemoryAlloc(sizeX, sizeY, grid);
		CellIndex(grid, sizeX, sizeY);

		timeStart = (double*)calloc(1, sizeof(double));
		if (timeStart == NULL) exit(-1);
		timeStop = (double*)calloc(1, sizeof(double));
		if (timeStop == NULL) exit(-1);

		int startX = 0;
		int startY = 0;

		while (status==2)
		{
			BeginDrawing();
			ClearBackground(BLACK);

			DrawTaskbar((bombs - flagsSet));

			for (int i = 0; i < sizeX; i++)
			{
				for (int j = 0; j < sizeY; j++)
				{
					CellDraw(grid, i, j);
				}
			}

			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				startX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
				startY = (GetMouseY() - menuHeight - SPACE + 1) / (POLE + SPACE);
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

		//eee

		//-GAME-----------------------------------------------------------------------------

		while (status==3)
		{
			BeginDrawing();
			ClearBackground(BLACK);

			DrawTaskbar((bombs-flagsSet));

			for (int i = 0; i < sizeX; i++)
			{
				for (int j = 0; j < sizeY; j++)
				{
					CellDraw(grid, i, j);
				}
			}

			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				coX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
				coY = (GetMouseY() - menuHeight - SPACE + 1) / (POLE + SPACE);

				if (CheckIndex(coX, coY, sizeX, sizeY)) CellReveal(grid, coX, coY, sizeX, sizeY, &status, revealed);
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
				coX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
				coY = (GetMouseY() - menuHeight - SPACE + 1) / (POLE + SPACE);
				if (CheckIndex(coX, coY, sizeX, sizeY) && grid[coX][coY].isFlag == false && grid[coX][coY].isRevealed == false) { grid[coX][coY].isFlag = true; flagsSet++; }
				else if (CheckIndex(coX, coY, sizeX, sizeY) && grid[coX][coY].isFlag == true && grid[coX][coY].isRevealed == false) { grid[coX][coY].isFlag = false; flagsSet++; }
			}

			CheckWin(bombs, *revealed, &status);

			if (WindowShouldClose())
			{
				status = 0;
				break;
			}

		}

		*timeStop = clock();

		//-POST-GAME-LOST-------------------------------------------------------------------

		while (status==4)
		{
			BeginDrawing();

			ClearBackground(BLACK);
			DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6, DARKBROWN);

			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if (CheckCollisionPointRec(GetMousePosition(), hardRec)) status = 1;
			}

			if (WindowShouldClose())
			{
				status = 0;
				break;
			}
		}
		
		//-POST-GAME-WON--------------------------------------------------------------------

		while (status==5)
		{
			BeginDrawing();

			ClearBackground(BLACK);
			DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6, DARKGREEN);

			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if (CheckCollisionPointRec(GetMousePosition(), hardRec)) status = 1;
			}

			if (WindowShouldClose())
			{
				status = 0;
				break;
			}
		}

		fnMemoryFree(sizeX, grid);
		free(timeStart);
		free(timeStop);
		if (status == 0) windowIsOpen = false;
		else status = 1;
		flagsSet = 0;
		*revealed = sizeX*sizeY;
	}

	CloseWindow();

	return 0;
}

