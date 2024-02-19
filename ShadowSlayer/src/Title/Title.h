/**
* @file Title.h
* @brief �N���X CTitle �̐錾
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
	virtual GameSceneResultCode move() override;	// �A�j���[�V�������\�b�h�@GAMESCENE_DEFAULT:�p�� / GAMESCENE_END_OK:�Q�[���J�n
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;	// �`�揈���@�w�i�͓񖇂̉摜�Ń��[�v���Ă���悤�Ɍ�����

protected:
	Selector	*m_pSystem;
	TitlePhase	m_ePhase;
	ID2D1Bitmap	*m_pImage, *m_pImage2, *m_pImage3;
	ID2D1SolidColorBrush	*m_pBlack;
	bool	m_bFlag;
	INT		m_iTimer;
	INT		m_iFadeTimer;
};

