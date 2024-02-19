/**
* @file Title.h
* @brief クラス CTitle の宣言
*/
#include "..\IGameSceen\IGameScene.h"


enum TitlePhase {
	TITLE_INIT = 0,
	TITLE_RUN = 1,
	TITLE_FADE = 2,
	TITLE_DONE = 3
};

class Selector;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class Title : public IGameScene
{
public:
	Title(Selector *pSystem);
	virtual ~Title();
	virtual GameSceneResultCode move() override;	// アニメーションメソッド　GAMESCENE_DEFAULT:継続 / GAMESCENE_END_OK:ゲーム開始
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;	// 描画処理　背景は二枚の画像でループしているように見せる

protected:
	Selector	*m_pSystem;
	TitlePhase	m_ePhase;
	ID2D1Bitmap	*m_pImage, *m_pImage2, *m_pImage3;
	ID2D1SolidColorBrush	*m_pBlack;
	bool	m_bFlag;
	INT		m_iTimer;
	INT		m_iFadeTimer;
};

