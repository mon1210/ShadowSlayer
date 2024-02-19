/**
* @file	UI.h
* @brief	クラス CUI の宣言ファイル
*/
#pragma once

class Player;
struct ID2D1Bitmap;
struct ID2D1RenderTarget;

class UI
{
public:
	UI(Player *pPlayer, ID2D1RenderTarget *pRenderTarget);
	virtual ~UI();
	virtual void move();	// UIの動作　他のオブジェクトと違い、消滅なし
	virtual void draw(ID2D1RenderTarget *pRenderTarget);	// 描画メソッド
protected:
	Player		*m_pPlayer;
	ID2D1Bitmap *m_pBarFrameTexture;
	ID2D1Bitmap	*m_pHPBarTexture;
	ID2D1Bitmap *m_pManaBarTexture;
	float		m_fHPRatio;
	float		m_fManaRatio;
};


