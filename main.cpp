#include "DxLib.h"
#include <stdlib.h>

const int SCREEN_WIDTH = 960, SCREEN_HEIGHT = 640;
const int WHITE = GetColor(255, 255, 255);
const int RED = GetColor(255, 0, 0);
const int GREEN = GetColor(0, 255, 0);
const int YELLOW = GetColor(255, 255, 0);
const int CYAN = GetColor(0, 255, 255);


//�{�[���̏������
const int BALL_INIT_POS_X = 40, BALL_INIT_POS_Y = 80, BALL_INIT_VELOCITY_X = 5, BALL_INIT_VELOCITY_Y = 5, BALL_RADIUS = 10;
//���P�b�g�̏������
const int RACKET_INIT_POS_X = SCREEN_WIDTH / 2, RACKET_INIT_POS_Y = SCREEN_HEIGHT - 50, RACKET_WIDTH = 120, RACKET_HEIGHT = 12;
//�Q�[���i�s�Ɋւ���ϐ�
enum { TITLE, PLAY, GAMEOVER };


//�^�C�g����ʂ̒萔
const int TITLE_TEXT_POS_X = SCREEN_WIDTH / 2 - 50 / 2 * 12 / 2;
const int TITLE_TEXT_POS_Y = SCREEN_HEIGHT / 3;
const int INSTRUCTION_TEXT_POS_X = SCREEN_WIDTH / 2 - 30 / 2 * 21 / 2;
const int INSTRUCTION_TEXT_POS_Y = SCREEN_HEIGHT * 2 / 3;


//�Q�[���I�[�o�[��ʂ̒萔
const int GAMEOVER_TEXT_POS_X = SCREEN_WIDTH / 2 - 40 / 2 * 9 / 2;
const int GAMEOVER_TEXT_POS_Y = SCREEN_HEIGHT / 3;
const int TRANSITION_TO_TITLE_TIME = 60 * 5;


/// <summary>
/// �{�[���̓���
/// </summary>
/// <param name="posX">x���W</param>
/// <param name="posY">y���W</param>
/// <param name="velX">x�����̑���</param>
/// <param name="velY">y�����̑���</param>
/// <param name="radius">���a</param>
/// <param name="scene">��ʂ̏��</param>
/// <param name="timer">����</param>
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
/// ���P�b�g�̓���
/// </summary>
/// <param name="posX">x���W</param>
/// <param name="posY">y���W</param>
/// <param name="racketWidth">���̃T�C�Y</param>
/// <param name="racketHeight">�c�̃T�C�Y</param>
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
/// ������
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
/// ���C���֐�
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	

	SetWindowText("�e�j�X�Q�[��");
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1) return -1;
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);

	//�{�[��
	int _ballPosX, _ballPosY, _ballVX, _ballVY, _ballR = BALL_RADIUS;
	//���P�b�g
	int _racketPosX, _racketPosY, _racketW = RACKET_WIDTH, _racketH = RACKET_HEIGHT;
	//�X�R�A
	int _score = 0, _highScore = 200;
	//�Q�[���i�s�Ɋւ���ϐ�
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
			
			//������_�ŕ\��
			SetFontSize(30);
			// �`�惂�[�h���A���t�@�u�����h�ɂ��ē����x�����Ԃɍ��킹�ĕύX������
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * abs(_timer % 80 - 40) / 40.0);
			DrawString(INSTRUCTION_TEXT_POS_X, INSTRUCTION_TEXT_POS_Y, "Press SPACE to start.", CYAN);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
			
			if (CheckHitKey(KEY_INPUT_SPACE) == 1)
			{
				//�Q�[���J�n�̂��߂ɏ�����
				SetUp(&_ballPosX, &_ballPosY, &_ballVX, &_ballVY, &_racketPosX, &_racketPosY, &_score, &_scene);
			}
			break;

		case PLAY:
			//�{�[���𓮂���
			ballAction(&_ballPosX, &_ballPosY, &_ballVX, &_ballVY, _ballR, &_scene, &_timer);
			//���P�b�g�𓮂���
			racketAction(&_racketPosX, _racketPosY, _racketW, _racketH);
			//�q�b�g�`�F�b�N
			checkHit(_ballPosX, _ballPosY, _racketPosX, _racketPosY, _racketW, &_ballVY, &_score, &_highScore);
			//�e�L�X�g�\��
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