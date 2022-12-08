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
	int coX, coY;
	Cell** grid = NULL;

	const int screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + menuHeight;
	const int screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

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

		int startX = 0;
		int startY = 0;

		while (status==2)
		{
			BeginDrawing();
			ClearBackground(BLACK);

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

		CellReveal(grid, startX, startY, sizeX, sizeY, &status);

		//if (status != 0) status = 3;

		//-GAME-----------------------------------------------------------------------------

		while (status==3)
		{
			BeginDrawing();
			ClearBackground(BLACK);

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

				if (CheckIndex(coX, coY, sizeX, sizeY)) CellReveal(grid, coX, coY, sizeX, sizeY, &status);
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
				coX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
				coY = (GetMouseY() - menuHeight - SPACE + 1) / (POLE + SPACE);
				if (CheckIndex(coX, coY, sizeX, sizeY) && grid[coX][coY].isFlag == false && grid[coX][coY].isRevealed == false) grid[coX][coY].isFlag = true;
				else if (CheckIndex(coX, coY, sizeX, sizeY) && grid[coX][coY].isFlag == true && grid[coX][coY].isRevealed == false) grid[coX][coY].isFlag = false;
			}



			if (WindowShouldClose())
			{
				status = 0;
				break;
			}

		}

		//-POST-GAME-LOST-------------------------------------------------------------------

		while (status==4)
		{
			BeginDrawing();

			ClearBackground(BLACK);
			DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6, LIGHTGRAY);

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
			DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6, LIGHTGRAY);

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
		if (status == 0) windowIsOpen = false;
		else status = 1;
	}

	CloseWindow();

	return 0;
}

