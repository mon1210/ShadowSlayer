/**
* @file	 UI.h
* @brief �N���X UI �̐錾�t�@�C��
*/
#pragma once

class Player;
struct ID2D1Bitmap;
struct ID2D1RenderTarget;

class UI
{
public:
	/**
	* @brief UI�̃R���X�g���N�^
	*/
	UI(Player *pPlayer, ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief UI�̃R���X�g���N�^
	*/
	~UI();

	/**
	* @brief UI�̓���
	* @note  ���̃I�u�W�F�N�g�ƈႢ�A���łȂ�
	*/
	void move();

	/**
	* @brief �`�惁�\�b�h
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


