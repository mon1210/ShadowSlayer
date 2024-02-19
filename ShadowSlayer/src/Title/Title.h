/**
* @file  Title.h
* @brief �N���X Title �̐錾
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
	* @brief Title�̃R���X�g���N�^
	*/
	Title(Selector *pSystem);

	/**
	* @brief Title�̃f�X�g���N�^
	*/
	~Title();

	/**
	* @brief �A�j���[�V�������\�b�h
	* @return GAMESCENE_DEFAULT:�p�� / GAMESCENE_END_OK:�Q�[���J�n
	*/
	GameSceneResultCode move() override;

	/**
	* @brief �`�揈��
	* @note	 �w�i�͓񖇂̉摜�Ń��[�v���Ă���悤�Ɍ�����
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

