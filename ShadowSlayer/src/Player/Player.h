/**
* @file  Player.h
* @brief �N���X Player �̐錾
*/
#include "..\IGameObject\IGameObject.h"

class Stage;
class BG;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class Player : public IGameObject
{
public:
	/**
	* @brief Player�̃R���X�g���N�^
	*/
	Player(Stage *pParent, BG *pBG);

	/**
	* @brief Player�̃f�X�g���N�^
	*/
	 ~Player();

	/**
	* @brief �v���C���[�̃A�j���[�V�������\�b�h
	* @return true:���� / false:���S
	* @note �����ł͉����x�̐ݒ肾���s���A(x, y)���W�̍X�V��collide() �ōs��
	*/
	bool move() override;

	/**
	* @brief �`�惁�\�b�h
	*/
	void draw(ID2D1RenderTarget *pRenderTarget) override;

	/**
	* @brief ��`�Ƃ̓����蔻�胁�\�b�h
	* @param[in] x �����x���W
	* @param[in] y �����y���W
	* @param[in] w ��`��Width
	* @param[in] h ��`��Height
	* @return true:������ / false:�O��
	*/
	bool collide(float x, float y, float w, float h) override;

	/**
	* @brief ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h
	* @param[in] *pObj ���Q�[���I�u�W�F�N�g
	* @return true:������ / false:�O��
	*/
	bool collide(IGameObject *pObj) override;

	/**
	* @brief �}�b�v�Ƃ̓����蔻�胁�\�b�h
	* @note	�����œ����蔻��ɏ]����(x, y)���W�̍X�V���s��
	*		X������Y�����ŕ������Ĕ���B
	*		�|�C���g�@: ���ꂼ������x�̐����ō��E�A�㉺�ɔ���𕪂���
	*		�|�C���g�A: ���肷������̂ݍ��W���ړ������A���肵�Ȃ������͈ړ��O�̍��W���g���Ĕ�����s��
	*		�|�C���g�B: X�����̔���͉����x0�̎��͍s��Ȃ��B�����x0�̎����܂߂Ă��܂��ƃW�����v���◎�����ɕǂɈ���������B
	*/
	bool collideWithMap() override;

	/**
	* @brief �v���C���[�̕`��X���W���v�Z���郁�\�b�h
	* @return m_fDrawX
	* @note collideWithMap()�̌�ɂ��̃��\�b�h���ĂԂ悤�ɂ��邱��
	*/
	float calcPlayerDrawX();

	/**
	* @brief �v���C���[�̕`��Y���W���v�Z���郁�\�b�h
	* @return m_fDrawY
	* @note collideWithMap()�̌�ɂ��̃��\�b�h���ĂԂ悤�ɂ��邱��
	*/
	float calcPlayerDrawY();

	/**
	* @brief �_���[�W���\�b�h
	* @note	 �v���C���[�͔�_���[�W�� ������ & �m�b�N�o�b�N
	*/
	int damage(float amount) override;
	
	/**
	* @brief  �v���C���[��x���W��Ԃ�
	* @return �v���C���[��x���W
	*/
	float getX();

	/**
	* @brief  �v���C���[��Y���W���擾���ĕԂ�
	* @return �v���C���[��Y���W
	*/
	float getY();

	/**
	* @brief  �v���C���[��HP���擾���ĕԂ�
	* @return �v���C���[��HP
	*/
	float getHP();

	/**
	* @brief  �v���C���[�̃}�i���擾���ĕԂ�
	* @return �v���C���[�̃}�i
	*/
	float getMana();

	/**
	* @brief �v���C���[��x���W���Z�b�g����
	*/
	void  setPos(float x, float y);

	/**
	* @brief �v���C���[�̉�ʏ��X���W���Z�b�g
	*/
	void  setDrawX(float x);

	/**
	* @brief �v���C���[�̉�ʏ��Y���W���Z�b�g
	*/
	void  setDrawY(float y);

	/**
	* @brief �v���C���[�̃}�i���Z�b�g���郁�\�b�h
	*/
	void  setMana(float amount);

	/**
	* @brief �}�b�v�̉������Z�b�g����
	*/
	void setMapWidth(float mw);

	/**
	* @brief �}�b�v�̏c�����Z�b�g����
	*/
	void setMapHight(float mh);

	/**
	 * @brief	BG��ݒ肵��Player�̊�����������
	 * @note	Player��BG����ݒ肷�� => Player���`�擙�ɉe����^���邽�߂ɕK�v
	 */
	void resetBG(BG *pBG);

	/**
	 * @brief X�����̃v���C���[�����x�����Z�b�g
	 */
	void clearVX();

	/**
	 * @brief Y�����̃v���C���[�����x�����Z�b�g
	 */
	void clearVY();

	/*
	* @brief	Y�����̃v���C���[�����x�𐧌����郁�\�b�h
	* @note		���ꂪ�Ȃ���Y�����̃}�b�v�ړ����Ƀv���C���[��������
	*/
	void setVY(float vy);

	//! �v���C���[�̕`�掞�̉�ʏ���WX�B�J�����̏œ_���v���C���[�ɍ��킹�Ă���̂ŁA���̃I�u�W�F�N�g�̓v���C���[���N�_�ɕ`�悷��B�f�[�^(m_fX)�ƕ`��(m_fDrawX)�𕪂��čl����̂��d�v�B
	float	m_fDrawX;	
	//! �v���C���[�̕`�掞�̉�ʏ���WY�B�J�����̏œ_���v���C���[�ɍ��킹�Ă���̂ŁA���̃I�u�W�F�N�g�̓v���C���[���N�_�ɕ`�悷��B�f�[�^(m_fX)�ƕ`��(m_fDrawX)�𕪂��čl����̂��d�v�B
	float	m_fDrawY;

protected:
	Stage		*m_pParent;
	BG			*m_pBG;
	ID2D1Bitmap	*m_pTexture;
	ID2D1Bitmap *m_pTexture_Left;
	ID2D1SolidColorBrush	*m_pBlackBrush;
	//! �v���C���[�ʒu
	float		m_fX, m_fY;
	//! �W�����v�A���������Ɏg�p
	float		m_fY_prev;
	//! �v���C���[�����x
	float		m_fVX, m_fVY;
	//! �v���C���[�̃q�b�g�{�b�N�X
	float		m_fPl, m_fPt, m_fPw, m_fPh;
	float		m_fOpacity;
	float		m_fShadowOpacity;
	float		m_fMapWidth;
	float		m_fMapHight;
	float		m_fRBoader;

	//! ���S����
	bool		m_bDeath;
	//! �_���[�W����
	bool		m_bDamage;
	//! �X���C�f�B���O���Ɍ�����ς������ǂ���(������ς�����̂�1��܂�)
	bool		m_bSlidingTurn;
	//! �W�����v�A�Ŗh�~
	bool		m_bJump;
	//! �U���A�Ŗh�~
	bool		m_bAttack;
	//! �ˌ��A�Ŗh�~
	bool		m_bShot;
	//! �V���h�E���A�Ŗh�~
	bool		m_bShadow;
	//! �v���C���[���E�������Ă��邩���������Ă��邩
	bool		m_bFaceToRight;
	//! �ʍs���\���ǂ���
	bool		m_bIs_AttackCancel_OK;
	//! �⓹�ɂ��邩�ǂ���
	bool		m_bIs_OnSlope;

	float		m_fHP;
	float		m_fMana;
	UINT		m_iFrame;
	//! �A�C�h����ԂɂȂ��Ă���̌o�߃t���[����
	UINT		m_iIdleFrame;
	//! �A�j���[�g���n�߂Ă���̌o�߃t���[��
	UINT		m_iAnimFrame;
	//! �X���C�f�B���O���n�߂Ă���̌o�߃t���[��
	UINT		m_iSlidingFrame;
	//! �A�^�b�N���n�߂Ă���̌o�߃t���[��
	UINT		m_iAttackFrame;
	//! �����i�ڂ̍U�����𐔂���
	UINT		m_iAttackCount;
	//! �X���C�f�B���O�A�^�b�N���n�߂Ă���̌o�߃t���[��
	UINT		m_iSlidingAttackFrame;
	//! �_���[�W���󂯂Ă���̌o�߃t���[��
	UINT		m_iDamagedFrame;
	UINT		m_iShadowSwitchFrame;
	//! �v���C���[�̍s����ԃt���O	bit���Z�ŕ����̃t���O��ۑ����邽��int�^
	UINT		m_iPlayerStateFlag;
	//! �V���h�E��Ԃ̃t���O   bit���Z�ŕ����̃t���O��ۑ����邽��int�^
	UINT		m_iShadowStateFlag;

	static float	m_fTextureCoord[];

#ifdef _DEBUG
	//! �f�o�b�O�p
	ID2D1SolidColorBrush* m_pWhiteBrush;
#endif
};

//	�v���C���[�̃q�b�g�{�b�N�X�T�C�Y
//	��{�T�C�Y ========================
/**
* @def PHB_W
* @brief ����
*/
#define PHB_W 28.f
/**
* @def PHB_H
* @brief �c��
*/
#define PHB_H 60.f
/**
* @def PHB_L
* @brief ����
*/
#define PHB_L (m_fX - 14.f)
/**
* @def PHB_T
* @brief ���_
*/
#define PHB_T (m_fY - 60.f)
// =====================================

//	�X���C�f�B���O���T�C�Y ============
/**
* @def PHBS_W
* @brief ����
*/
#define PHBS_W 30.f
/**
* @def PHBS_H
* @brief �c��
*/
#define PHBS_H 30.f
/**
* @def PHBS_L
* @brief ����
*/
#define PHBS_L (m_fX - 15.f)
/**
* @def PHBS_T
* @brief ���_
*/
#define PHBS_T (m_fY - 30.f)
// ====================================

// �v���C���[�̈ړ����x ===============
/**
* @def WALKSPEED
* @brief �������x
*/
#define WALKSPEED 6.5f
/**
* @def SLIDESPEED
* @brief �X���C�f�B���O���x
*/
#define SLIDESPEED 9.f
// ====================================

// �v���C���[�̍s����ԃt���O =========
/**
* @def PFG_IDLE
* @brief �ҋ@��
*/
#define PFG_IDLE	0x0000
/**
* @def PFG_WALK
* @brief �ʏ�ړ���
*/
#define PFG_WALK	0x0001
/**
* @def PFG_SLIDE
* @brief �X���C�f�B���O��
*/
#define PFG_SLIDE	0x0002
/**
* @def PFG_SLIDE_CONTINUE
* @brief �X���C�f�B���O�p���t���O
*/
#define PFG_SLIDE_CONTINUE	0x0100
/**
* @def PFG_JUMP
* @brief ��
*/
#define PFG_JUMP	0x0004
/**
* @def PFG_ATTACK
* @brief �U����
*/
#define PFG_ATTACK	0x0008
/**
* @def PFG_SLIDEATTACK
* @brief �X���C�f�B���O�U����
*/
#define PFG_SLIDEATTACK	0x0080
/**
* @def PFG_DAMAGED
* @brief ��e
*/
#define PFG_DAMAGED	0x0010
// ====================================

//	�V���h�E�̃t���O ==================
/**
* @def SFG_OFF
* @brief �I�t
*/
#define	SFG_OFF		0x0000
/**
* @def SFG_SWITCH
* @brief �ڍs��
*/
#define	SFG_SWITCH  0x0001
/**
* @def SFG_ON
* @brief �I��
*/
#define	SFG_ON		0x0002
// ====================================

// �d�͑��x �W�����v���x ==============
/**
* @def JUMP
* @brief �W�����v���x
*/
#define JUMP -16.f
// ====================================

// �L�[�ݒ� ===========================
/**
* @def KEY_ATTACK
* @brief �U���L�[(F)
*/
#define KEY_ATTACK	0x0046
// ====================================