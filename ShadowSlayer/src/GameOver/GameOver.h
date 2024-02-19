/**
* @file GameOver.h
* @brief �N���X CGameOVer �̐錾
*/
#include "..\IGameSceen\IGameScene.h"

enum GameOverPhase {
	GAMEOVER_INIT = 0,
	GAMEOVER_RUN = 1,
	GAMEOVER_FADE = 2,
	GAMEOVER_DONE = 3
};

class Selector;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class GameOver : public IGameScene
{
public:
	GameOver(Selector *pSystem);
	virtual ~GameOver();
	virtual GameSceneResultCode move() override;	// �A�j���[�V�������\�b�h�@GAMESCENE_DEFAULT:�p�� / GAMESCENE_END_OK:�Q�[���J�n
	virtual void draw(ID2D1RenderTarget *pRendetTarget);	// �`�惁�\�b�h
protected:
	Selector		*m_pSystem;
	GameOverPhase	m_ePhase;
	ID2D1Bitmap		*m_pImage;
	ID2D1SolidColorBrush	*m_pBlack;
	INT m_iTimer;
	INT m_iFadeTimer;
	bool m_bFlag;
};

