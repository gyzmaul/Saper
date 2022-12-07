#include "raylib.h"
#include "utility.h"
#include "board.h"
#include <iostream>
#include <time.h>

int status = 0;

int main()
{
	srand(time(NULL));

	int sizeX = 26;
	int sizeY = 38;
	int bombs = 310;
	int coX, coY;
	Cell** grid = NULL;

	const int screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + menuHeight;
	const int screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

	InitWindow(screenWidth, screenHeight, "Samper - yebac UJ");

	Rectangle easyRec = { screenWidth / 4,       screenHeight / 6, screenWidth / 2, screenHeight / 6 };
	Rectangle medRec = { screenWidth / 4, 2.5 * screenHeight / 6, screenWidth / 2, screenHeight / 6 };
	Rectangle hardRec = { screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6 };

	SetTargetFPS(120);

	while (!WindowShouldClose())
	{
		//-MENU-----------------------------------------------------------------------------

		while (!status)
		{
			BeginDrawing();

			ClearBackground(BLACK);
			DrawRectangle(screenWidth / 4, screenHeight / 6, screenWidth / 2, screenHeight / 6, GREEN);
			DrawRectangle(screenWidth / 4, 2.5 * screenHeight / 6, screenWidth / 2, screenHeight / 6, YELLOW);
			DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6, RED);

			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if (CheckCollisionPointRec(GetMousePosition(), hardRec)) status = 1;
			}
		}

		//-LOADING--------------------------------------------------------------------------

		grid = fnMemoryAlloc(sizeX, sizeY, grid);
		CellIndex(grid, sizeX, sizeY);

		int startX;
		int startY;

		while (status)
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
				status = 0;
			}
		}

		do
		{
			CellSetUJ(sizeX, sizeY, bombs, grid, startX, startY);
			CellFillNumbers(sizeX, sizeY, grid);
		} while (grid[startX][startY].bombsAround > 0 || grid[startX][startY].isBomb == true);

		status=CellReveal(grid, startX, startY, sizeX, sizeY);

		status = 1;

		//-GAME-----------------------------------------------------------------------------

		while (status)
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

				if (CheckIndex(coX, coY, sizeX, sizeY)) status=CellReveal(grid, coX, coY, sizeX, sizeY);
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
				coX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
				coY = (GetMouseY() - menuHeight - SPACE + 1) / (POLE + SPACE);
				if (CheckIndex(coX, coY, sizeX, sizeY) && grid[coX][coY].isFlag == false && grid[coX][coY].isRevealed == false) grid[coX][coY].isFlag = true;
				else if (CheckIndex(coX, coY, sizeX, sizeY) && grid[coX][coY].isFlag == true && grid[coX][coY].isRevealed == false) grid[coX][coY].isFlag = false;
			}

			if (WindowShouldClose()) status = 0;
		}

		//-POST-GAME------------------------------------------------------------------------

		while (!status)
		{
			BeginDrawing();

			ClearBackground(BLACK);
			DrawRectangle(screenWidth / 4, 4 * screenHeight / 6, screenWidth / 2, screenHeight / 6, LIGHTGRAY);

			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if (CheckCollisionPointRec(GetMousePosition(), hardRec)) status = 1;
			}
		}

		fnMemoryFree(sizeX, grid);
		status = 0;
	}

	CloseWindow();

	return 0;
}

