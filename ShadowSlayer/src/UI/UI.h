/**
* @file	 UI.h
* @brief クラス UI の宣言ファイル
*/
#pragma once

class Player;
struct ID2D1Bitmap;
struct ID2D1RenderTarget;

class UI
{
public:
	/**
	* @brief UIのコンストラクタ
	*/
	UI(Player *pPlayer, ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief UIのコンストラクタ
	*/
	~UI();

	/**
	* @brief UIの動作
	* @note  他のオブジェクトと違い、消滅なし
	*/
	void move();

	/**
	* @brief 描画メソッド
	*/
	void draw(ID2D1RenderTarget *pRenderTarget);
protected:
	Player		*m_pPlayer;
	ID2D1Bitmap *m_pBarFrameTexture;
	ID2D1Bitmap	*m_pHPBarTexture;
	ID2D1Bitmap *m_pManaBarTexture;
	float		m_fHPRatio;
	float		m_fManaRatio;
};


