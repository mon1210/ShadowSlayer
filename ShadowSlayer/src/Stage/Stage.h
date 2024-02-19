/**
* @file  Stage.h
* @brief �N���X Stage �̐錾
*/
#include "..\Selector\Selector.h"
#include <list>
#include "..\IGameObject\IGameObject.h"
#include "..\Enums.h"

class Player;
class BG;
class EnemySet;
class UI;
class GateKeeper;
struct ID2D1SolidColorBrush;

class Stage : public IGameScene
{
public:

	/**
	* @brief Shot�̃R���X�g���N�^
	*/
	Stage(Selector *pSystem);

	/**
	* @brief Shot�̃R���X�g���N�^
	*/
	 ~Stage();

	/**
	* @brief �A�j���[�V����(1�t���[��)�̎��s
	* @note �����̏��Ԃ� �I�u�W�F�N�g�� move ���Ă���e�X collide ������āA�K�v�Ȃ�e���W�𒲐�����
	* @return GAMESCENE_DEFAULT: �p�� / GAMESCENE_END_FAILURE: �Q�[���I�[�o�[
	*/
	GameSceneResultCode move() override;

	/**
	* @brief �`�揈��
	* @note  ���݂̃}�b�v�Ǝ��̃}�b�v�𓮂����Ȃ���`�悷�邱�Ƃňړ����Ă���悤�Ɍ�����
	*/
	void draw(ID2D1RenderTarget *pRenderTarget) override;

	/**
	* @brief	�e��I�u�W�F�N�g�̃N���A
	* @note		�}�b�v�J�ڎ��Ɏg�p
	*/
	void clearGameObjects();

	/**
	* @brief	���}�b�v�Ɏg���f�[�^�E�I�u�W�F�N�g��ݒ�
	* @note		m_pBG���X�V���Ă��炱�̊֐����ĂԂ悤��
	*/
	void setGameObjects();

	/**
	* @brief ID2D1RenderTarget ���擾���ĕԂ�
	* @note ���̃��\�b�h�Ŏ󂯎���� RenderTarget �͎g�p��K��Release ���邱��
	* @return ID2D1RenderTarget �I�u�W�F�N�g
	*/
	ID2D1RenderTarget *GetRenderTarget();

	void addShots(IGameObject* pObj);
	void addSlashes(IGameObject* pObj);
	void addEnemyAttacks(IGameObject* pObj);

	//! �v���C���[�̕`�掞�̉�ʏ���WX�B�J�����̏œ_���v���C���[�ɍ��킹�Ă���̂ŁA���̃I�u�W�F�N�g�̓v���C���[���N�_�ɕ`�悷��B�f�[�^�ƕ`��𕪂��čl����̂��d�v�B
	float	m_fPlayerDrawX;
	//! �v���C���[�̕`�掞�̉�ʏ���WY�B�J�����̏œ_���v���C���[�ɍ��킹�Ă���̂ŁA���̃I�u�W�F�N�g�̓v���C���[���N�_�ɕ`�悷��B�f�[�^�ƕ`��𕪂��čl����̂��d�v�B
	float	m_fPlayerDrawY;
	//! �v���C���[�̃f�[�^��̍��WX�B
	float	m_fPlayerX;
	//! �v���C���[�̃f�[�^��̍��WY�B
	float	m_fPlayerY;
	//! �}�b�v�̒��� = �}�b�v�̉E�[�̍��W
	float	m_fMapWidth;
	float	m_fMapHight;
protected:
	Selector	*m_pSystem;
	ID2D1SolidColorBrush	*m_pBlackBrush;
	bool		m_bGameOver;
	Player		*m_pPlayer;
	BG			*m_pBG;
	//! �X�e�[�W�J�ڂɎg�p
	BG			*m_pBG_tmp;
	GateKeeper	*m_pGateKeeperX;
	GateKeeper	*m_pGateKeeperY;
	GateKeeper	*m_pGateKeeperX_prev;
	GateKeeper	*m_pGateKeeperY_prev;
	EnemySet	*m_pEnemySet;
	UI			*m_pUI;
	StagePhase	m_ePhase;
	MapNum		m_eMapNum;
	//! �|�[�Y�{�^���A�Ŗh�~�t���O
	bool		m_bPause;
	int		m_iTimer;
	//! �}�b�v�ԍ�
	int		m_iMapNum;
	std::list<IGameObject*>	*m_pShots;
	std::list<IGameObject*>	*m_pEnemies;
	std::list<IGameObject*> *m_pSlashCollisions;
	std::list<IGameObject*>	*m_pEnemyAttackCollisions;
};

