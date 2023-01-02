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
	bool musicIsPlaying = 1;

	int menuScreen = 1;
	int menuHighlight = 0;
	int taskbarHighlight = 0;
	int gameMode = 0;
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
	int correct = 0;
	int* stillHidden;
	int rev = sizeX * sizeY;
	stillHidden = &rev;

	int cellColor;

	double* timeStart;
	double* timeStop;
	double* timeTemp;

	int coX, coY;
	Cell** grid = NULL;

	Rectangle easyRec = { screenWidth / 6    , 6.2 * screenHeight / 12, 2 * screenWidth / 3, screenHeight / 18 };
	Rectangle medRec  = { screenWidth / 6    , 7.2 * screenHeight / 12, 2 * screenWidth / 3, screenHeight / 18 };
	Rectangle hardRec = { screenWidth / 6    , 8.2 * screenHeight / 12, 2 * screenWidth / 3, screenHeight / 18 };
	Rectangle rankRec = { screenWidth / 6 - 4, 9.6 * screenHeight / 12, 76, 54 };
	Rectangle beerRec = { 3 * screenWidth / 4 - 36, 9.6 * screenHeight / 12 - 4, 64, 60 };
	Rectangle  cogRec = { screenWidth / 2 - 30, 9.6 * screenHeight / 12 - 4, 64, 54 };
	Rectangle backRec = { 30, 10, 68, 40 };
	Rectangle musicRec = { 16, screenHeight - menuHeight + 5, 40, 40};
	Rectangle backMenu = { screenWidth - 16 - 40, screenHeight - menuHeight + 5, 40, 40};

	Music gameOst;

	Vector2 position;
	position.x = (MONITORWIDTH - screenWidth) / 2; 
	position.y = (MONITORHEIGHT - screenHeight) / 2;
	//GetMonitorWidth(1);                          
	//GetMonitorHeight(0);

	//-file-read------------------------------------------------------------------------

	std::ifstream ranking;
	ranking.open("files/data.txt");
	int** nRanking=NULL;
	nRanking = fnMemoryAllocInt(3, 3, nRanking);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ranking >> nRanking[i][j];
		}
	}
	ranking >> cellColor;

	ranking.close();

	setColors(cellColor);

	//-GAME-----------------------------------------------------------------------------

	PlayAudio(&gameOst, "files/password-infinity-123276.mp3");
	SetMusicVolume( gameOst, 0.05);

	while (gameIsRunning)
	{

		//-MENU-----------------------------------------------------------------------------

		OpenWindow("Menu", position.x, position.y);

		LoadTexturesMenu();
		LoadFonts();

		while (status==1) //menu
		{
			while (status == 1 && menuScreen == 1) //main menu
			{
				windowSnap(screenWidth, screenHeight);

				if (musicIsPlaying == 1) UpdateMusicStream(gameOst);

				if (CheckCollisionPointRec(GetMousePosition(), easyRec)) menuHighlight = 1;
				if (CheckCollisionPointRec(GetMousePosition(), medRec )) menuHighlight = 2;
				if (CheckCollisionPointRec(GetMousePosition(), hardRec)) menuHighlight = 3;
				if (CheckCollisionPointRec(GetMousePosition(), rankRec)) menuHighlight = 4;
				if (CheckCollisionPointRec(GetMousePosition(), cogRec )) menuHighlight = 5;
				if (CheckCollisionPointRec(GetMousePosition(), beerRec)) menuHighlight = 6;

				DrawMenu(menuHighlight);
				menuHighlight = 0;

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					if (CheckCollisionPointRec(GetMousePosition(), easyRec)) { status = 2; gameMode = 1; }
					if (CheckCollisionPointRec(GetMousePosition(), medRec )) { status = 2; gameMode = 2; }
					if (CheckCollisionPointRec(GetMousePosition(), hardRec)) { status = 2; gameMode = 3; }
					if (CheckCollisionPointRec(GetMousePosition(), beerRec)) { status = 2; gameMode = 4; }
					if (CheckCollisionPointRec(GetMousePosition(), rankRec)) { menuScreen = 2; }
					if (CheckCollisionPointRec(GetMousePosition(), cogRec )) { menuScreen = 3; }
				}

				if (WindowShouldClose())
				{
					status = 0;
					break;
				}
			}

			while (menuScreen == 2) //ranking
			{
				windowSnap(screenWidth, screenHeight);

				if (musicIsPlaying == 1) UpdateMusicStream(gameOst);

				DrawRanking(nRanking);

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					if (CheckCollisionPointRec(GetMousePosition(), backRec)) { menuScreen = 1; }
				}

				if (WindowShouldClose())
				{
					status = 0;
					break;
				}
			}

			while (menuScreen == 3) //settings
			{
				windowSnap(screenWidth, screenHeight);

				if (musicIsPlaying == 1) UpdateMusicStream(gameOst);

				DrawSettings(cellColor);

				changeColor(&cellColor);

				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					if (CheckCollisionPointRec(GetMousePosition(), backRec)) { menuScreen = 1; }
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

		position = GetWindowPosition();
		position.x += screenWidth/2;
		position.y += screenHeight/2;

		if(status != 0) CloseWin();

		//-LOADING--------------------------------------------------------------------------

		switch (gameMode)
		{
		case 1: sizeEasy(&sizeX, &sizeY, &bombs); break;

		case 2: sizeMed(&sizeX, &sizeY, &bombs); break;
		
		case 4: sizeMed(&sizeX, &sizeY, &bombs); break;

		case 3: sizeHard(&sizeX, &sizeY, &bombs); break;

		default: break;
		}

		menuHeight = 2 * POLE + SPACE;
		screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + 2 * menuHeight;
		screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;

		musicRec = { 16, (float) screenHeight - menuHeight + 5, 40, 40 };
		backMenu = { (float) screenWidth - 16 - 40, (float) screenHeight - menuHeight + 5, 40, 40 };

		*stillHidden = sizeX * sizeY;

		position.x -= screenWidth / 2;
		position.y -= screenHeight / 2;

		if (status != 0)
		{
			OpenWindow("Game", position.x, position.y);
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
		LoadFonts();

		while (status==2)
		{
			windowSnap(screenWidth, screenHeight);

			if (musicIsPlaying == 1) UpdateMusicStream(gameOst);

			BeginDrawing();
			ClearBackground(BLACK);

			DrawTaskbar((bombs - flagsSet), (*timeTemp - *timeStart) / CLOCKS_PER_SEC);

			DrawBottomBar(musicIsPlaying, taskbarHighlight);

			CellDraw(grid, status);

			EndDrawing();

			taskbarHighlight = 0;
			if (CheckCollisionPointRec(GetMousePosition(), musicRec)) taskbarHighlight = 1;
			if (CheckCollisionPointRec(GetMousePosition(), backMenu)) taskbarHighlight = 2;

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if (CheckCollisionPointRec(GetMousePosition(), musicRec)) musicIsPlaying = !musicIsPlaying;
				if (CheckCollisionPointRec(GetMousePosition(), backMenu)) status = 1;
				
				if (GetMouseY() - menuHeight - SPACE > 0 && GetMouseY() < screenHeight - menuHeight)
				{
					startX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
					startY = (GetMouseY() - menuHeight - SPACE) / (POLE + SPACE);
					status = 3;
				}
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

		CellReveal(grid, startX, startY, sizeX, sizeY, &status, stillHidden);

		*timeStart = clock();

		//-GAME-----------------------------------------------------------------------------

		while (status==3)
		{
			windowSnap(screenWidth, screenHeight);

			if (musicIsPlaying == 1) UpdateMusicStream(gameOst);

			BeginDrawing();
			ClearBackground(BLACK);

			DrawTaskbar((bombs-flagsSet), (*timeTemp-*timeStart)/CLOCKS_PER_SEC);

			DrawBottomBar(musicIsPlaying, taskbarHighlight);

			CellDraw(grid, status);

			EndDrawing();

			correct = countCorrect(grid, sizeX, sizeY);

			taskbarHighlight = 0;
			if (CheckCollisionPointRec(GetMousePosition(), musicRec)) taskbarHighlight = 1;
			if (CheckCollisionPointRec(GetMousePosition(), backMenu)) taskbarHighlight = 2;

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if (CheckCollisionPointRec(GetMousePosition(), musicRec)) musicIsPlaying = !musicIsPlaying;
				if (CheckCollisionPointRec(GetMousePosition(), backMenu)) status = 1;

				coX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
				coY = (GetMouseY() - menuHeight - 2*SPACE) / (POLE + SPACE);

				if (GetMouseY() - menuHeight - SPACE > 0 && GetMouseY() < screenHeight - menuHeight)
				{
					if (CheckIndex(coX, coY, sizeX, sizeY))
					{
						int revealOld = *stillHidden;
						CellReveal(grid, coX, coY, sizeX, sizeY, &status, stillHidden);

						if (gameMode == 4 && revealOld != *stillHidden && status == 3)
						{
							cellShuffle(sizeX, sizeY, bombs, grid, correct);
							CellFillNumbers(sizeX, sizeY, grid);
						}
					}
				}
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
				coX = (GetMouseX() - SPACE + 1) / (POLE + SPACE);
				coY = (GetMouseY() - menuHeight - 2*SPACE) / (POLE + SPACE);

				if (GetMouseY() - menuHeight - SPACE > 0)
				{
					if (CheckIndex(coX, coY, sizeX, sizeY) && grid[coX][coY].isRevealed == false)
					{
						if (grid[coX][coY].isFlag == false && bombs - flagsSet > 0) { grid[coX][coY].isFlag = true; flagsSet++; }
						else if (grid[coX][coY].isFlag == true) { grid[coX][coY].isFlag = false; flagsSet--; }
					}
				}
			}

			if(status != 4) CheckWin(bombs, *stillHidden, &status);

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
			windowSnap(screenWidth, screenHeight);

			if (musicIsPlaying == 1) UpdateMusicStream(gameOst);

			BeginDrawing();
			ClearBackground(BLACK);

			DrawTaskbar((bombs - flagsSet), (*timeStop - *timeStart) / CLOCKS_PER_SEC);

			DrawText(TextFormat("Przegrales"), screenWidth / 2 - 86, screenHeight - menuHeight + 12, 1.5 * POLE, RED);

			CellDraw(grid, status);

			EndDrawing();

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) status = 1;

			if (WindowShouldClose())
			{
				status = 0;
				break;
			}
		}

		//-WIN-SCREEN-----------------------------------------------------------------------

		if (status == 5 && gameMode != 4)
		{
			updateRanks((*timeStop - *timeStart) / CLOCKS_PER_SEC, nRanking, gameMode);
		}

		while (status == 5)
		{
			windowSnap(screenWidth, screenHeight);

			if (musicIsPlaying == 1) UpdateMusicStream(gameOst);

			BeginDrawing();
			ClearBackground(BLACK);

			DrawTaskbar(0, (*timeStop - *timeStart) / CLOCKS_PER_SEC);

			DrawText(TextFormat("Wygrales"), screenWidth / 2 - 72, screenHeight - menuHeight + 12, 1.5 * POLE, GREEN);

			CellDraw(grid, status);

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
		UnloadFonts(); 

		position = GetWindowPosition();
		position.x += screenWidth / 2;
		position.y += screenHeight / 2;

		CloseWin();

		fnMemoryFree(sizeX, grid);
		free(timeStart);
		free(timeStop);
		free(timeTemp);

		sizeMed(&sizeX, &sizeY, &bombs);
		menuHeight = 2 * POLE + SPACE;
		screenHeight = POLE * sizeY + SPACE * (sizeY - 1) + MARGINES * 2 + 2 * menuHeight;
		screenWidth = POLE * sizeX + SPACE * (sizeX - 1) + MARGINES * 2;
		position.x -= screenWidth / 2;
		position.y -= screenHeight / 2;

		correct = 0;
		flagsSet = 0;
		*stillHidden = sizeX*sizeY;

		if (status == 0) gameIsRunning = false;
		else status = 1;
	}

	StopMusicStream(gameOst);
	UnloadMusicStream(gameOst);
	CloseAudioDevice();

	std::ofstream newRanking;
	newRanking.open("files/data.txt");

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newRanking << nRanking[i][j] << std::endl;
		}
	}
	newRanking << cellColor << std::endl;

	newRanking.close();
	fnMemoryFreeInt(3, nRanking);

	return 0;
}

