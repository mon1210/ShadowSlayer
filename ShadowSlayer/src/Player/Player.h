/**
* @file Player.h
* @brief �N���X CPlayer �̐錾
*/
#include "IGameObject.h"

class Stage;
class BG;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class Player : public IGameObject
{
public:
	Player(Stage *pParent, BG *pBG);
	virtual ~Player();
	virtual bool move() override;	// �s�����,�A�j���[�V�������Ǘ����郁�\�b�h	true:���� / false:���S
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;		// �`�惁�\�b�h
	virtual bool collide(float x, float y, float w, float h) override;	// ��`�Ƃ̓����蔻�胁�\�b�h	true:������ / false:�O��
	virtual bool collide(IGameObject *pObj) override;	// ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h	true:������ / false:�O��
	virtual bool collideWithMap() override;		// �}�b�v�Ƃ̓����蔻�胁�\�b�h�@�����蔻��ɏ]����(x, y)���W�̍X�V���s��
	virtual float calcPlayerDrawX();	// �v���C���[�̕`��X���W���v�Z���郁�\�b�h(return m_fDrawX)�@collideWithMap()�̌�ɂ��̃��\�b�h���ĂԂ悤�ɂ���
	virtual float calcPlayerDrawY();	// �v���C���[�̕`��Y���W���v�Z���郁�\�b�h(return m_fDrawY)�@collideWithMap()�̌�ɂ��̃��\�b�h���ĂԂ悤�ɂ���
	virtual int damage(float amount) override;	// �_���[�W���\�b�h�@�v���C���[�͔�_���[�W�� ������ & �m�b�N�o�b�N
	virtual float getX();		// �v���C���[��x���W��Ԃ�
	virtual float getY();		// �v���C���[��Y���W���擾���ĕԂ�
	virtual float getHP();		// �v���C���[��HP���擾���ĕԂ�
	virtual float getMana();	// �v���C���[�̃}�i���擾���ĕԂ�
	virtual void  setPos(float x, float y);		// �v���C���[��x���W���Z�b�g����
	virtual void  setDrawX(float x);			// �v���C���[�̉�ʏ��X���W���Z�b�g
	virtual void  setDrawY(float y);			// �v���C���[�̉�ʏ��Y���W���Z�b�g
	virtual void  setMana(float amount);		// �v���C���[�̃}�i���Z�b�g���郁�\�b�h
	virtual void setMapWidth(float mw);			// �}�b�v�̉������Z�b�g����
	virtual void setMapHight(float mh);			// �}�b�v�̏c�����Z�b�g����
	virtual void resetBG(BG *pBG);	// BG��ݒ肵��Player�̊�����������@Player��BG����ݒ肷�� => Player���`�擙�ɉe����^���邽�߂ɕK�v
	virtual void clearVX();			// X�����̃v���C���[�����x�����Z�b�g
	virtual void clearVY();			// Y�����̃v���C���[�����x�����Z�b�g
	virtual void SetVY(float vy);	// Y�����̃v���C���[�����x�𐧌����郁�\�b�h�@���ꂪ�Ȃ���Y�����̃}�b�v�ړ����Ƀv���C���[��������
	// �v���C���[�̕`�掞�̉�ʏ���W�B�J�����̏œ_���v���C���[�ɍ��킹�Ă���̂ŁA���̃I�u�W�F�N�g�̓v���C���[���N�_�ɕ`�悷��B�f�[�^(m_fX)�ƕ`��(m_fDrawX)�𕪂��čl����̂��d�v�B
	float	m_fDrawX;	
	float	m_fDrawY;

protected:
	Stage		*m_pParent;
	BG			*m_pBG;
	ID2D1Bitmap	*m_pTexture;
	ID2D1Bitmap *m_pTexture_Left;
	ID2D1SolidColorBrush	*m_pBlackBrush;
	float		m_fX, m_fY;		//	�v���C���[�ʒu
	float		m_fY_prev;		//	�W�����v�A���������Ɏg�p
	float		m_fVX, m_fVY;	//	�v���C���[�����x
	float		m_fPl, m_fPt, m_fPw, m_fPh;		//	�v���C���[�̃q�b�g�{�b�N�X
	float		m_fOpacity;
	float		m_fShadowOpacity;
	float		m_fMapWidth;
	float		m_fMapHight;
	float		m_fRBoader;

	bool		m_bDeath;	//	���S����
	bool		m_bDamage;	//	�_���[�W����
	bool		m_bSlidingTurn;	//	�X���C�f�B���O���Ɍ�����ς������ǂ���(������ς�����̂�1��܂�)
	bool		m_bJump;		//	�W�����v�A�Ŗh�~
	bool		m_bAttack;		//	�U���A�Ŗh�~
	bool		m_bShot;		//	�ˌ��A�Ŗh�~
	bool		m_bShadow;		//	�V���h�E���A�Ŗh�~
	bool		m_bFaceToRight;	//	�v���C���[���E�������Ă��邩���������Ă��邩
	bool		m_bIs_AttackCancel_OK;	//	�ʍs���\���ǂ���
	bool		m_bIs_OnSlope;	//	�⓹�ɂ��邩�ǂ���

	float		m_fHP;
	float		m_fMana;
	//--- UINT �񕉂̐����^
	UINT		m_iFrame;
	UINT		m_iIdleFrame;			//	�A�C�h����ԂɂȂ��Ă���̌o�߃t���[����
	UINT		m_iAnimFrame;			//	�A�j���[�g���n�߂Ă���̌o�߃t���[��
	UINT		m_iSlidingFrame;		//	�X���C�f�B���O���n�߂Ă���̌o�߃t���[��
	UINT		m_iAttackFrame;			//	�A�^�b�N���n�߂Ă���̌o�߃t���[��
	UINT		m_iAttackCount;			//	�����i�ڂ̍U�����𐔂���
	UINT		m_iSlidingAttackFrame;	//	�X���C�f�B���O�A�^�b�N���n�߂Ă���̌o�߃t���[��
	UINT		m_iDamagedFrame;		//	�_���[�W���󂯂Ă���̌o�߃t���[��
	UINT		m_iShadowSwitchFrame;
	UINT		m_iPlayerStateFlag;		//	�v���C���[�̍s����ԃt���O	bit���Z�ŕ����̃t���O��ۑ����邽��int�^
	UINT		m_iShadowStateFlag;		//	�V���h�E��Ԃ̃t���O   bit���Z�ŕ����̃t���O��ۑ����邽��int�^

	static float	m_fTextureCoord[];

#ifdef _DEBUG
	ID2D1SolidColorBrush	*m_pWhiteBrush;	//	�f�o�b�O�p
#endif
};

//	�v���C���[�̃q�b�g�{�b�N�X�T�C�Y
//	��{�T�C�Y
#define PHB_W 28.f
#define PHB_H 60.f
#define PHB_L (m_fX - 14.f)
#define PHB_T (m_fY - 60.f)
//	�X���C�f�B���O���T�C�Y
#define PHBS_W 30.f
#define PHBS_H 30.f
#define PHBS_L (m_fX - 15.f)
#define PHBS_T (m_fY - 30.f)

// �v���C���[�̈ړ����x
#define WALKSPEED 6.5f
#define SLIDESPEED 9.f
#define SLIDESPEED_DECREASE 0.25f

// �v���C���[�̍s����ԃt���O
#define PFG_IDLE	0x0000	//	�ҋ@��
#define PFG_WALK	0x0001	//	�ʏ�ړ���
#define PFG_SLIDE	0x0002	//	�X���C�f�B���O��
#define PFG_SLIDE_CONTINUE	0x0100	//	�X���C�f�B���O�p���t���O
#define PFG_JUMP	0x0004	//	��
#define PFG_ATTACK	0x0008	//	�U����
#define PFG_SLIDEATTACK	0x0080	//	�X���C�f�B���O�U����
#define PFG_DAMAGED	0x0010	//	��e

//	�V���h�E�̃t���O
#define	SFG_OFF		0x0000	//	�I�t
#define	SFG_SWITCH  0x0001	//	�ڍs��
#define	SFG_ON		0x0002	//	�I��

// �d�͑��x �W�����v���x
#define JUMP -16.f

// �L�[�ݒ�
#define KEY_ATTACK	0x0046	//	F�L�[