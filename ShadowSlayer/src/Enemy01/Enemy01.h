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
	Enemy01(Stage *pParent, float x, float y);
	~Enemy01();
	virtual bool move() override;	// �s�����,�A�j���[�V�������Ǘ����郁�\�b�h	true:���� / false:���S
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;		// �`�惁�\�b�h
	virtual bool collide(float x, float y, float w, float h) override;	// ��`�Ƃ̓����蔻�胁�\�b�h	true:������ / false:�O��
	virtual bool collide(IGameObject *pObj) override;	// ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h	true:������ / false:�O��
	virtual bool collideWithMap() override;				// �}�b�v�Ƃ̓����蔻�胁�\�b�h�@�����蔻��ɏ]����(x, y)���W�̍X�V���s��
	virtual int damage(float amount) override;			// �_���[�W���\�b�h (return �_���[�W)
	static void Restore(ID2D1RenderTarget *pRT, BG *pBG);	// ���L���f�B�A�t�@�C���̓ǂݍ��݁@�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	static void Finalize();		// ���L���f�B�A�t�@�C���̏����@�V�[���폜���ȂǂɌĂяo���悤�ɂ���
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
