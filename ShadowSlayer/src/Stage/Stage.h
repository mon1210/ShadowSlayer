/**
* @file  Stage.h
* @brief �N���X Stage �̐錾
*/
#include "..\Selector\Selector.h"
#include <list>
#include "..\IGameObject\IGameObject.h"

class Player;
class BG;
class EnemySet;
class UI;
class GateKeeper;
struct ID2D1SolidColorBrush;

enum StagePhase {
	STAGE_INIT = 0,
	STAGE_RUN = 1,
	STAGE_PAUSE = 2,
	STAGE_FADE = 3,
	STAGE_DONE = 4,
	STAGE_MAPCHANGE_X = 5,
	STAGE_MAPCHANGE_Y= 6,
	STAGE_MAPCHANGE_DONE = 7,
};

enum MapNum
{
	DEFAULT_STAGE = 0,
	RIGHT_STAGE = 1,
	UNDER_STAGE = 2,
	UPPER_STAGE = 3,

};

class Stage : public IGameScene
{
public:
	Stage(Selector *pSystem);
	virtual ~Stage(void);
	virtual GameSceneResultCode move() override; // �A�j���[�V����(1�t���[��)�̎��s�@
												 // �����̏��Ԃ� �I�u�W�F�N�g�� move ���Ă���e�X collide ������āA�K�v�Ȃ�e���W�𒲐�����
												 // return GAMESCENE_DEFAULT: �p�� / GAMESCENE_END_FAILURE: �Q�[���I�[�o�[
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;	// �`�惁�\�b�h
	virtual void addShots(IGameObject *pObj);
	virtual void addSlashes(IGameObject *pObj);
	virtual void addEnemyAttacks(IGameObject *pObj);
	virtual void clearGameObjects();	// �e��I�u�W�F�N�g�̃N���A�@�}�b�v�J�ڎ��Ɏg�p
	virtual void setGameObjects();		// ���}�b�v�Ɏg���f�[�^�E�I�u�W�F�N�g��ݒ�@m_pBG���X�V���Ă��炱�̊֐����Ă�
	ID2D1RenderTarget *GetRenderTarget();	// ID2D1RenderTarget ���擾���ĕԂ�
	float	m_fPlayerDrawX;	//	�v���C���[�̕`�掞�̉�ʏ���W�B�J�����̏œ_���v���C���[�ɍ��킹�Ă���̂ŁA���̃I�u�W�F�N�g�̓v���C���[���N�_�ɕ`�悷��B�f�[�^�ƕ`��𕪂��čl����̂��d�v�B
	float	m_fPlayerDrawY;
	float	m_fPlayerX;		//	�v���C���[�̃f�[�^��̍��W�B
	float	m_fPlayerY;
	float	m_fMapWidth;	//	�}�b�v�̒��� = �}�b�v�̉E�[�̍��W
	float	m_fMapHight;
protected:
	Selector	*m_pSystem;
	ID2D1SolidColorBrush	*m_pBlackBrush;
	bool		m_bGameOver;
	Player		*m_pPlayer;
	BG			*m_pBG;
	BG			*m_pBG_tmp;	// �X�e�[�W�J�ڂɎg�p
	GateKeeper	*m_pGateKeeperX;
	GateKeeper	*m_pGateKeeperY;
	GateKeeper	*m_pGateKeeperX_prev;
	GateKeeper	*m_pGateKeeperY_prev;
	EnemySet	*m_pEnemySet;
	UI			*m_pUI;
	StagePhase	m_ePhase;
	MapNum		m_eMapNum;
	bool		m_bPause;	//	�|�[�Y�{�^���A�Ŗh�~�t���O
	int		m_iTimer;
	int		m_iMapNum;		//	�}�b�v�ԍ�
	std::list<IGameObject*>	*m_pShots;
	std::list<IGameObject*>	*m_pEnemies;
	std::list<IGameObject*> *m_pSlashCollisions;
	std::list<IGameObject*>	*m_pEnemyAttackCollisions;
};

