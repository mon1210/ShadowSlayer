/**
* @file  Shot.h
* @brief �N���X Shot �̐錾�t�@�C��
*/
#include "..\IGameObject\IGameObject.h"

struct ID2D1Bitmap;
class Stage;
class BG;

class Shot : public IGameObject
{
public:
	/**
	* @brief Shot�̃R���X�g���N�^
	* @param[in] x	�v���C���[��x���W
	* @param[in] y	�v���C���[��y���W
	* @param[in] direction	1 or -1
	*/
	Shot(Stage *pParent, float x, float y, float direction);

	/**
	* @brief Shot�̃f�X�g���N�^
	*/
	~Shot();

	/**
	* @brief shot�̃A�j���[�V����
	* @note	�G��}�b�v�ɂԂ��邩�A��ʊO�ɏo��Ə���
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
	* @note	 �����œ����蔻��ɏ]����(x, y)���W�̍X�V���s��
	*		 X������Y�����ŕ������Ĕ���B
	*		 �|�C���g�@: ���ꂼ������x�̐����ō��E�A�㉺�ɔ���𕪂���
	*		 �|�C���g�A: ���肷������̂ݍ��W���ړ������A���肵�Ȃ������͈ړ��O�̍��W���g���Ĕ�����s��
	*		 �|�C���g�B: X�����̔���͉����x0�̎��͍s��Ȃ��B�����x0�̎����܂߂Ă��܂��ƃW�����v���◎�����ɕǂɈ���������B
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
	 * @brief BG��ݒ肵��Shot�̊�����������
	 * @note  Shot��BG����ݒ肷�� => Shot���`�擙�e����^���邽�߂ɕK�v
	 */
	static void resetBG(BG *pBG);

	/**
	* @fn
	* @brief ���L���f�B�A�t�@�C���̏���
	* @note	 �V�[���폜���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Finalize();			// ���L���f�B�A�t�@�C���̏����@�V�[���폜���ȂǂɌĂяo���悤�ɂ���

protected:
	Stage		*m_pParent;
	bool		m_bDamage;
	float		m_fX;
	float		m_fY;
	float		m_fVX;
	float		m_fVY;
	float		m_fDirection;
	//! ����������̃A�j���[�V�����^�C�}�[
	int			m_iLifeTimer;
	//! ���ł܂ł̃A�j���[�V�����^�C�}�[
	int			m_iDeathTimer;
	//! �ÓI�����o�B�N���X�S�̂�(=�S�C���X�^���X��)���L�B
	static ID2D1Bitmap *m_pTexture;
	static ID2D1Bitmap *m_pTexture_Left;
	static BG	*m_pBG;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pWhite;
#endif
};

#define BULLETSPEED 8.0f
/**
* @def SHOTOFFSETX
* @brief �\���J�n���W�̏C��
*/
#define SHOTOFFSETX 30.f
/**
* @def SHOTOFFSETY
* @brief �\���J�n���W�̏C��
*/
#define SHOTOFFSETY 40.f