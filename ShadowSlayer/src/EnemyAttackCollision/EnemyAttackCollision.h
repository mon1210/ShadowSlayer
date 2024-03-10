/**
* @file  EnemyAttackCollision.h
* @brief �v���C���[�̋ߐڍU���̓����蔻��
*/
#include "..\IGameObject\IGameObject.h"

class Player;
struct ID2D1SolidColorBrush;

class EnemyAttackCollision : public IGameObject
{
public:
	/**
	* @brief EnemyAttackCollision�̃R���X�g���N�^
	* @param[in] x				�G�l�~�[��x���W
	* @param[in] y				�G�l�~�[��y���W
	* @param[in] w				�����蔻��`����W
	* @param[in] colW			�����蔻�艡��(W)
	* @param[in] colH			�����蔻��c��(H)
	* @param[in] LifeTime		���ݎ���
	* @param[in] faceToRight	��������t���O�@true:�E���� / false:������
	*/
	EnemyAttackCollision(float x, float y, float w, float colW, float colH, int LifeTime,bool faceToRight);

	/**
	* @brief EnemyAttackCollision�̃f�X�g���N�^
	*/
	~EnemyAttackCollision();

	/**
	* @brief �s���Ǘ�
	* @note	 �����^�C�}�[���߂���Ǝ��S
	* return true:���� / false:���S
	*/
	bool move();

	/**
	* @brief �`�惁�\�b�h
	*/
	void draw(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief ��`�Ƃ̓����蔻�胁�\�b�h
	* @param[in] x �����x���W
	* @param[in] y �����y���W
	* @param[in] w ��`��Width
	* @param[in] h ��`��Height
	* @return true:������ / false:�O��
	*/
	bool collide(float x, float y, float w, float h);

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
	bool collideWithMap();		// �}�b�v�Ƃ̓����蔻�胁�\�b�h

	/**
	* @brief �_���[�W���\�b�h
	*/
	int damage(float amount);

	/**
	* @brief ���L���f�B�A�t�@�C���̓ǂݍ���
	* @note	 �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Restore(Player *m_pPlayer);

	/**
	* @brief ���L���f�B�A�t�@�C���̏���
	* @note	 �V�[���폜���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Finalize();
#ifdef _DEBUG
	/**
	* @brief ���L���f�B�A�t�@�C���̓ǂݍ���
	* @note	 �V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Restore(Player *pPlayer, ID2D1RenderTarget *pRT);
#endif
protected:
	//! �����X,Y���W
	float	m_fX, m_fY;
	float	m_fW, m_fH;
	int		m_iCount, m_iLifeTime;
	//! ��������
	bool	m_bFaceToRight;
	static Player *m_pPlayer;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pRed;
#endif
};

