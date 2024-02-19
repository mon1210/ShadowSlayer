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
	Shot(Stage *pParent, float x, float y, float direction);
	~Shot();

	virtual bool move() override;	// �s�����,�A�j���[�V�������Ǘ����郁�\�b�h	true:���� / false:���S
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;	// �`�惁�\�b�h
	virtual bool collide(float x, float y, float w, float h) override;	// ��`�Ƃ̓����蔻�胁�\�b�h	true:������ / false:�O��
	virtual bool collide(IGameObject *pObj) override;		// ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h	true:������ / false:�O��
	virtual bool collideWithMap() override;			// �}�b�v�Ƃ̓����蔻�胁�\�b�h�@�����蔻��ɏ]����(x, y)���W�̍X�V���s��
	virtual int damage(float amount) override;		// �_���[�W���\�b�h (return �_���[�W)
	static void Restore(ID2D1RenderTarget *pRT, BG *pBG);	// ���L���f�B�A�t�@�C���̓ǂݍ��݁@�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	static void resetBG(BG *pBG);	// BG��ݒ肵��Shot�̊�����������@Shot��BG����ݒ肷�� => Shot���`�擙�e����^���邽�߂ɕK�v
	static void Finalize();			// ���L���f�B�A�t�@�C���̏����@�V�[���폜���ȂǂɌĂяo���悤�ɂ���

protected:
	Stage		*m_pParent;
	bool		m_bDamage;
	float		m_fX;
	float		m_fY;
	float		m_fVX;
	float		m_fVY;
	float		m_fDirection;
	int			m_iLifeTimer;	//	����������̃A�j���[�V�����^�C�}�[
	int			m_iDeathTimer;	//	���ł܂ł̃A�j���[�V�����^�C�}�[
	static ID2D1Bitmap *m_pTexture;	//	�ÓI�����o�B�N���X�S�̂�(=�S�C���X�^���X��)���L�B
	static ID2D1Bitmap *m_pTexture_Left;
	static BG	*m_pBG;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pWhite;
#endif
};

#define BULLETSPEED 8.0f
#define SHOTOFFSETX 30.f	// �\���J�n���W�̏C��
#define SHOTOFFSETY 40.f	// �\���J�n���W�̏C��