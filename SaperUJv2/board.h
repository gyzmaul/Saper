#pragma 

#define POLE 20
#define SPACE 2
#define MARGINES 2

void CellDraw(Cell** grid, int i, int j);
void CellSetUJ(int sizeX, int sizeY, int nUJ, Cell** aTab, int startX, int startY);
void CellFillNumbers(int sizeX, int sizeY, Cell** grid);
void CellReveal(Cell** grid, int i, int j, int sizeX, int sizeY, int*status, int* revealed);
void DrawTaskbar(int bombsLeft);