#include "DxLib.h"
#include <stdlib.h>

/// <summary>
/// ボールの動き
/// </summary>
/// <param name="posX">x座標</param>
/// <param name="posY">y座標</param>
/// <param name="velX">x方向の速さ</param>
/// <param name="velY">y方向の速さ</param>
/// <param name="radius">半径</param>
/// <param name="screenWidth">画面の横のサイズ</param>
/// <param name="screenHeight">画面の縦のサイズ</param>
/// <param name="white">白のカラー</param>
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

/// <summary>
/// ラケットの動き
/// </summary>
/// <param name="posX">x座標</param>
/// <param name="posY">y座標</param>
/// <param name="racketWidth">横のサイズ</param>
/// <param name="racketHeight">縦のサイズ</param>
/// <param name="screenWidth">画面の横のサイズ</param>
/// <param name="white">白のカラー</param>
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

void checkHit(int ballPosX,
			  int ballPosY,
		      int racketPosX,
			  int racketPosY,
			  int racketWidth,
			  int* ballVY,
			  int* score,
			  int* highScore)
{
	int dx = ballPosX - racketPosX;
	int dy = ballPosY - racketPosY;

	if ((-racketWidth / 2 - 10 < dx && dx < racketWidth / 2 + 10)
		&& (-20 < dy && dy < 0))
	{
		*ballVY = -5 - rand() % 5;
		*score += 100;
		if (*score > *highScore) *highScore = *score;
	}
}

void textDisplay(int score, int highScore, int screenWidth, int white)
{
	SetFontSize(30);
	DrawFormatString(10, 10, white, "SCORE %d", score);
	DrawFormatString(screenWidth - 200, 10, white, "HIGHSCORE %d", highScore);
}

/// <summary>
/// メイン関数
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
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
	int _racketPosX = SCREEN_WIDTH / 2, _racketPosY = SCREEN_HEIGHT - 50, _racketW = 120, _racketH = 12;
	//スコア
	int _score = 0, _highScore = 200;

	while (1)
	{
		ClearDrawScreen();

		//ボールを動かす
		ballAction(&_ballPosX, &_ballPosY, &_ballVX, &_ballVY, _ballR, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);

		//ラケットを動かす
		racketAction(&_racketPosX, _racketPosY, _racketW, _racketH, SCREEN_WIDTH, WHITE);

		//ヒットチェック
		checkHit(_ballPosX, _ballPosY, _racketPosX, _racketPosY, _racketW, &_ballVY, &_score, &_highScore);

		//テキスト表示
		textDisplay(_score, _highScore, SCREEN_WIDTH, WHITE);

		ScreenFlip();
		WaitTimer(16);
		if (ProcessMessage() == -1) break;
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	DxLib_End();
	return 0;
}