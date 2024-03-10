/**
* @file  SlashCollision.h
* @brief �v���C���[�̋ߐڍU���̓����蔻��
*/
#include "..\IGameObject\IGameObject.h"

class Player;
struct ID2D1SolidColorBrush;

class SlashCollision : public IGameObject
{
public:
	/**
	* @brief SlashCollision�̃R���X�g���N�^
	* @param[in] attackCount	���i�ڍU�����̃J�E���^(4:�X���C�f�B���O�U��)
	* @param[in] faceToRight	��������t���O�@	true�F�E����  / false�F������
	* @param[in] shadowFlag		�V���h�E����t���O�@��0�F�V���h�E /	0�F�m�[�}��	�@bit���Z�ŕ����̃t���O��ۑ����邽��int�^
	*/
	SlashCollision(int attackCount, bool faceToRight, int shadowFlag);

	/**
	* @brief SlashCollision�̃f�X�g���N�^
	*/
	~SlashCollision();

	/**
	* @brief SlashCollision�̐���
	* @note	 �������Ԃ��߂���Ə���
	* return true:���� / false:���S
	*/
	virtual bool move();

	/**
	* @brief �`�惁�\�b�h
	*/
	virtual void draw(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief ��`�Ƃ̓����蔻�胁�\�b�h
	* @param[in] x �����x���W
	* @param[in] y �����y���W
	* @param[in] w ��`��Width
	* @param[in] h ��`��Height
	* @return true:������ / false:�O��
	*/
	virtual bool collide(float x, float y, float w, float h);

	/**
	* @brief ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h
	* @param[in] *pObj ���Q�[���I�u�W�F�N�g
	* @return true:������ / false:�O��
	*/
	virtual bool collide(IGameObject *pObj) override;

	/**
	* @fn
	* @brief ���L���f�B�A�t�@�C���̓ǂݍ���
	* @note	 �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Restore(Player *m_pPlayer);

	/**
	* @fn
	* @brief	���L���f�B�A�t�@�C���̏���
	* @note		�V�[���폜���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Finalize();

	/**
	* @brief �}�b�v�Ƃ̓����蔻�胁�\�b�h
	* @note	�����œ����蔻��ɏ]����(x, y)���W�̍X�V���s��
	*		X������Y�����ŕ������Ĕ���B
	*		�|�C���g�@: ���ꂼ������x�̐����ō��E�A�㉺�ɔ���𕪂���
	*		�|�C���g�A: ���肷������̂ݍ��W���ړ������A���肵�Ȃ������͈ړ��O�̍��W���g���Ĕ�����s��
	*		�|�C���g�B: X�����̔���͉����x0�̎��͍s��Ȃ��B�����x0�̎����܂߂Ă��܂��ƃW�����v���◎�����ɕǂɈ���������B
	*/
	bool collideWithMap();

	/**
	* @brief �_���[�W���\�b�h
	*/
	int damage(float amount) override;
#ifdef _DEBUG
	/**
	* @fn
	* @brief ���L���f�B�A�t�@�C���̓ǂݍ���
	* @note	 �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Restore(Player *pPlayer, ID2D1RenderTarget *pRT);
#endif
protected:
	//! �����X,Y���W
	float	m_fX, m_fY;
	float	m_fW, m_fH, m_fXOffSet, m_fYOffSet;
	int		m_iLifeTime;
	int		m_iAttackCount;
	//! ��������
	bool	m_bFaceToRight;
	static Player *m_pPlayer;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pRed;
#endif
};

/**
* @def SLASH_WIDTH
* @brief 1, 2�i�ڂ̃R���W����
*/
#define SLASH_WIDTH		40.f
/**
* @def SLASH_HEIGHT
* @brief 1, 2�i�ڂ̃R���W����
*/
#define SLASH_HEIGHT	60.f

/**
* @def SLASH_WIDTH3
* @brief 3�i�ڂ̃R���W����
*/
#define SLASH_WIDTH3	50.f
/**
* @def SLASH_HEIGHT3
* @brief 3�i�ڂ̃R���W����
*/
#define	SLASH_HEIGHT3	50.f

/**
* @def SLASH_WIDTH4
* @brief �_�b�V���U���̃R���W����
*/
#define SLASH_WIDTH4	55.f
/**
* @def SLASH_HEIGHT4
* @brief �_�b�V���U���̃R���W����
*/
#define SLASH_HEIGHT4	50.f