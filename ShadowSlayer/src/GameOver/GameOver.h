/**
* @file GameOver.h
* @brief クラス CGameOVer の宣言
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
	virtual GameSceneResultCode move() override;	// アニメーションメソッド　GAMESCENE_DEFAULT:継続 / GAMESCENE_END_OK:ゲーム開始
	virtual void draw(ID2D1RenderTarget *pRendetTarget);	// 描画メソッド
protected:
	Selector		*m_pSystem;
	GameOverPhase	m_ePhase;
	ID2D1Bitmap		*m_pImage;
	ID2D1SolidColorBrush	*m_pBlack;
	INT m_iTimer;
	INT m_iFadeTimer;
	bool m_bFlag;
};

