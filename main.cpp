#include "DxLib.h"
#include <stdlib.h>

const int SCREEN_WIDTH = 960, SCREEN_HEIGHT = 640;
const int WHITE = GetColor(255, 255, 255);
const int RED = GetColor(255, 0, 0);
const int GREEN = GetColor(0, 255, 0);
const int YELLOW = GetColor(255, 255, 0);
const int CYAN = GetColor(0, 255, 255);


//ボールの初期情報
const int BALL_INIT_POS_X = 40, BALL_INIT_POS_Y = 80, BALL_INIT_VELOCITY_X = 5, BALL_INIT_VELOCITY_Y = 5, BALL_RADIUS = 10;
//ラケットの初期情報
const int RACKET_INIT_POS_X = SCREEN_WIDTH / 2, RACKET_INIT_POS_Y = SCREEN_HEIGHT - 50, RACKET_WIDTH = 120, RACKET_HEIGHT = 12;
//ゲーム進行に関する変数
enum { TITLE, PLAY, GAMEOVER };


//タイトル画面の定数
const int TITLE_TEXT_POS_X = SCREEN_WIDTH / 2 - 50 / 2 * 12 / 2;
const int TITLE_TEXT_POS_Y = SCREEN_HEIGHT / 3;
const int INSTRUCTION_TEXT_POS_X = SCREEN_WIDTH / 2 - 30 / 2 * 21 / 2;
const int INSTRUCTION_TEXT_POS_Y = SCREEN_HEIGHT * 2 / 3;


//ゲームオーバー画面の定数
const int GAMEOVER_TEXT_POS_X = SCREEN_WIDTH / 2 - 40 / 2 * 9 / 2;
const int GAMEOVER_TEXT_POS_Y = SCREEN_HEIGHT / 3;
const int TRANSITION_TO_TITLE_TIME = 60 * 5;


/// <summary>
/// ボールの動き
/// </summary>
/// <param name="posX">x座標</param>
/// <param name="posY">y座標</param>
/// <param name="velX">x方向の速さ</param>
/// <param name="velY">y方向の速さ</param>
/// <param name="radius">半径</param>
/// <param name="scene">画面の状態</param>
/// <param name="timer">時間</param>
void ballAction(int* posX,
				int* posY,
				int* velX,
				int* velY,
				int radius,
				int* scene,
				int* timer)
{
	*posX += *velX;
	if ((*posX < radius && *velX < 0)
		|| (*posX > SCREEN_WIDTH - radius && *velX > 0)) *velX *= -1;

	*posY += *velY;
	if (*posY < radius && *velY < 0) *velY *= -1;

	if (*posY > SCREEN_HEIGHT)
	{
		*scene = GAMEOVER;
		*timer = 0;
	}

	DrawCircle(*posX, *posY, radius, WHITE, TRUE);
}

/// <summary>
/// ラケットの動き
/// </summary>
/// <param name="posX">x座標</param>
/// <param name="posY">y座標</param>
/// <param name="racketWidth">横のサイズ</param>
/// <param name="racketHeight">縦のサイズ</param>
void racketAction(int* posX, int posY, int racketWidth, int racketHeight)
{
	if (CheckHitKey(KEY_INPUT_LEFT) == 1)
	{
		*posX -= 10;
		if (*posX < racketWidth / 2) *posX = racketWidth / 2;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
	{
		*posX += 10;
		if (*posX > SCREEN_WIDTH - racketWidth / 2) *posX = SCREEN_WIDTH - racketWidth / 2;
	}

	DrawBox(*posX - racketWidth / 2,
		posY - racketHeight / 2,
		*posX + racketWidth / 2,
		posY + racketHeight / 2,
		WHITE,
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

void textDisplay(int score, int highScore)
{
	SetFontSize(30);
	DrawFormatString(10, 10, WHITE, "SCORE %d", score);
	DrawFormatString(SCREEN_WIDTH - 260, 10, YELLOW, "HIGHSCORE %d", highScore);
}

/// <summary>
/// 初期化
/// </summary>
void SetUp(int* ballPosX, int* ballPosY, int* ballVX, int* ballVY, int* racketPosX, int* racketPosY, int* score, int* scene)
{
	*ballPosX = BALL_INIT_POS_X;
	*ballPosY = BALL_INIT_POS_Y;
	*ballVX = BALL_INIT_VELOCITY_X;
	*ballVY = BALL_INIT_VELOCITY_Y;
	*racketPosX = RACKET_INIT_POS_X;
	*racketPosY = RACKET_INIT_POS_Y;
	*score = 0;
	*scene = PLAY;
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
	

	SetWindowText("テニスゲーム");
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) return -1;
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);

	//ボール
	int _ballPosX, _ballPosY, _ballVX, _ballVY, _ballR = BALL_RADIUS;
	//ラケット
	int _racketPosX, _racketPosY, _racketW = RACKET_WIDTH, _racketH = RACKET_HEIGHT;
	//スコア
	int _score = 0, _highScore = 200;
	//ゲーム進行に関する変数
	int _scene = TITLE;
	int _timer = 0;

	while (1)
	{
		ClearDrawScreen();
		_timer++;

		switch (_scene)
		{
		case TITLE:
			SetFontSize(50);
			DrawString(TITLE_TEXT_POS_X, TITLE_TEXT_POS_Y, "Tennis Game", GREEN);
			
			//文字を点滅表示
			SetFontSize(30);
			// 描画モードをアルファブレンドにして透明度を時間に合わせて変更させる
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * abs(_timer % 80 - 40) / 40.0);
			DrawString(INSTRUCTION_TEXT_POS_X, INSTRUCTION_TEXT_POS_Y, "Press SPACE to start.", CYAN);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			
			if (CheckHitKey(KEY_INPUT_SPACE) == 1)
			{
				//ゲーム開始のために初期化
				SetUp(&_ballPosX, &_ballPosY, &_ballVX, &_ballVY, &_racketPosX, &_racketPosY, &_score, &_scene);
			}
			break;

		case PLAY:
			//ボールを動かす
			ballAction(&_ballPosX, &_ballPosY, &_ballVX, &_ballVY, _ballR, &_scene, &_timer);
			//ラケットを動かす
			racketAction(&_racketPosX, _racketPosY, _racketW, _racketH);
			//ヒットチェック
			checkHit(_ballPosX, _ballPosY, _racketPosX, _racketPosY, _racketW, &_ballVY, &_score, &_highScore);
			//テキスト表示
			textDisplay(_score, _highScore);
			break;

		case GAMEOVER:
			SetFontSize(40);
			DrawString(GAMEOVER_TEXT_POS_X, GAMEOVER_TEXT_POS_Y, "GAME OVER", RED);
			if (_timer > TRANSITION_TO_TITLE_TIME) _scene = TITLE;
			break;
		}

		

		ScreenFlip();
		WaitTimer(16);
		if (ProcessMessage() == -1) break;
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;
	}

	DxLib_End();
	return 0;
}