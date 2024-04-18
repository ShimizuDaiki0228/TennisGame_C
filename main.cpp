#include "DxLib.h"


void ballAction(int* posX,
				int* posY,
				int* velX,
				int* velY,
				int radius,
				int screenWidth,
				int screenHeight,
				int white)
{
	*posX += *velX;
	if ((*posX < radius && *velX < 0)
		|| (*posX > screenWidth - radius && *velX > 0)) *velX *= -1;

	*posY += *velY;
	if (*posY < radius && *velY < 0
		|| *posY > screenHeight - radius && *velY > 0) *velY *= -1;

	DrawCircle(*posX, *posY, radius, white, TRUE);
}

void racketAction(int* posX, int posY, int racketWidth, int racketHeight, int screenWidth, int white)
{
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		*posX -= 10;
		if (*posX < racketWidth / 2) *posX = racketWidth / 2;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		*posX += 10;
		if (*posX > screenWidth - racketWidth / 2) *posX = screenWidth - racketWidth / 2;
	}

	DrawBox(*posX - racketWidth / 2,
		posY - racketHeight / 2,
		*posX + racketWidth / 2,
		posY + racketHeight / 2,
		white,
		TRUE
	);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const int SCREEN_WIDTH = 960, SCREEN_HEIGHT = 640;
	const int WHITE = GetColor(255, 255, 255);

	SetWindowText("テニスゲーム");
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) return -1;
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);

	//ボール
	int _ballPosX = 40, _ballPosY = 80, _ballVX = 5, _ballVY = 5, _ballR = 10;

	//ラケット
	int _racketX = SCREEN_WIDTH / 2, _racketY = SCREEN_HEIGHT - 50, _racketW = 120, _racketH = 12;

	while (1)
	{
		ClearDrawScreen();

		//ボールを動かす
		ballAction(&_ballPosX, &_ballPosY, &_ballVX, &_ballVY, _ballR, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);

		//ラケットを動かす
		racketAction(&_racketX, _racketY, _racketW, _racketH, SCREEN_WIDTH, WHITE);

		ScreenFlip();
		WaitTimer(16);
		if (ProcessMessage() == -1) break;
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	DxLib_End();
	return 0;
}