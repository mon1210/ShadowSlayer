/**
* @file	UI.h
* @brief	�N���X CUI �̐錾�t�@�C��
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
	virtual void move();	// UI�̓���@���̃I�u�W�F�N�g�ƈႢ�A���łȂ�
	virtual void draw(ID2D1RenderTarget *pRenderTarget);	// �`�惁�\�b�h
protected:
	Player		*m_pPlayer;
	ID2D1Bitmap *m_pBarFrameTexture;
	ID2D1Bitmap	*m_pHPBarTexture;
	ID2D1Bitmap *m_pManaBarTexture;
	float		m_fHPRatio;
	float		m_fManaRatio;
};


