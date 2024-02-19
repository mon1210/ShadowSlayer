/**
* @file  GameOver.h
* @brief �N���X GameOVer �̐錾
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
	* @brief GameOver�̃R���X�g���N�^
	*/
	GameOver(Selector *pSystem);

	/**
	* @brief GameOver�̃f�X�g���N�^
	*/
	virtual ~GameOver();

	/**
	* @brief �A�j���[�V�������\�b�h
	* @return GAMESCENE_DEFAULT:�p�� / GAMESCENE_END_OK:�Q�[���J�n
	*/
	GameSceneResultCode move() override;

	/**
	* @brief �`�惁�\�b�h
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

