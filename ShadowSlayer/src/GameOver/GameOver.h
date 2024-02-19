/**
* @file  GameOver.h
* @brief クラス GameOVer の宣言
*/
#include "..\IGameSceen\IGameScene.h"
#include "..\Enums.h"

class Selector;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class GameOver : public IGameScene
{
public:
	/**
	* @brief GameOverのコンストラクタ
	*/
	GameOver(Selector *pSystem);

	/**
	* @brief GameOverのデストラクタ
	*/
	virtual ~GameOver();

	/**
	* @brief アニメーションメソッド
	* @return GAMESCENE_DEFAULT:継続 / GAMESCENE_END_OK:ゲーム開始
	*/
	GameSceneResultCode move() override;

	/**
	* @brief 描画メソッド
	*/
	void draw(ID2D1RenderTarget *pRendetTarget);
protected:
	Selector		*m_pSystem;
	GameOverPhase	m_ePhase;
	ID2D1Bitmap		*m_pImage;
	ID2D1SolidColorBrush	*m_pBlack;
	INT m_iTimer;
	INT m_iFadeTimer;
	bool m_bFlag;
};

