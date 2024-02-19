/**
* @file  Title.h
* @brief クラス Title の宣言
*/
#include "..\IGameSceen\IGameScene.h"
#include "..\Enums.h"

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
	/**
	* @brief Titleのコンストラクタ
	*/
	Title(Selector *pSystem);

	/**
	* @brief Titleのデストラクタ
	*/
	~Title();

	/**
	* @brief アニメーションメソッド
	* @return GAMESCENE_DEFAULT:継続 / GAMESCENE_END_OK:ゲーム開始
	*/
	GameSceneResultCode move() override;

	/**
	* @brief 描画処理
	* @note	 背景は二枚の画像でループしているように見せる
	*/
	void draw(ID2D1RenderTarget *pRenderTarget) override;

protected:
	Selector	*m_pSystem;
	TitlePhase	m_ePhase;
	ID2D1Bitmap	*m_pImage, *m_pImage2, *m_pImage3;
	ID2D1SolidColorBrush	*m_pBlack;
	bool	m_bFlag;
	INT		m_iTimer;
	INT		m_iFadeTimer;
};

