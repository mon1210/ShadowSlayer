/**
* @file	 Enemy01.h
* @brief �N���X Enemy01(�X���C��) �̐錾
* @note	 �����蔻�� 32 x 25.5
*/
#include "..\IGameObject\IGameObject.h"

class Stage;
class BG;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class Enemy01 : public IGameObject
{
public:
	/**
	* @brief Enemy01�̃R���X�g���N�^
	* @param[in] x	�G�l�~�[��x���W
	* @param[in] y	�G�l�~�[��y���W
	*/
	Enemy01(Stage *pParent, float x, float y);

	/**
	* @brief Enemy01�̃f�X�g���N�^
	*/
	~Enemy01();

	/**
	* @brief Enemy01�̃A�j���[�V����
	* @note	 �_���[�W���󂯂�Ǝ��S
	* return true:���� / false:���S
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
	* @brief �_���[�W���\�b�h
	*/
	int damage(float amount) override;

	/**
	* @fn
	* @brief ���L���f�B�A�t�@�C���̓ǂݍ���
	* @note	 �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Restore(ID2D1RenderTarget *pRT, BG *pBG);

	/**
	* @brief	���L���f�B�A�t�@�C���̏���
	* @note		�V�[���폜���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Finalize();
protected:
	Stage	*m_pParent;
	float	m_fX, m_fY, m_fY_prev;
	float	m_fVX, m_fVY;
	float	m_fDiffX, m_fDiffY;	//	�v���C���[�Ƃ̋���
	bool	m_bAttacked;
	bool	m_bFaceToRight;		// ��������
	bool	m_bIs_OnSlope;		// ��ɂ��邩�ǂ���
	UINT	m_iCount;
	UINT	m_iEnemyStateFlag;	// bit���Z�ŕ����̃t���O��ۑ����邽��int�^
	static ID2D1Bitmap *m_pTexture, *m_pTexture_Left;
	static BG	*m_pBG;
	static float	m_fTextureCoord[];

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pWhite;
#endif
};

//	�s���t���O
#define EFG_IDLE	0x0000
#define	EFG_MOVE	0x0001
#define EFG_ATTACK	0x0002
#define EFG_JUMP	0x0004
#define EFG_DEATH	0x0008
#define EFG_FALL	0x0008

//	�e��X�e�[�^�X
#define	ENEMY01_SEARCH_DISTANCE		200.f
#define	ENEMY01_ATTACK_DISTANCE		35.f
