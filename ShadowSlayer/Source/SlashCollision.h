/**
* @file SlashCollision.h
* @brief �v���C���[�̋ߐڍU���̓����蔻��
*/
#include "IGameObject.h"

class Player;
struct ID2D1SolidColorBrush;

class SlashCollision : public IGameObject
{
public:
	SlashCollision(int attackCount, bool faceToRight, int shadowFlag);
	~SlashCollision();
	virtual bool move();	// �s�����,�A�j���[�V�������Ǘ����郁�\�b�h	true:���� / false:���S
	virtual void draw(ID2D1RenderTarget *pRenderTarget);		// �`�惁�\�b�h
	virtual bool collide(float x, float y, float w, float h);	// ��`�Ƃ̓����蔻�胁�\�b�h	true:������ / false:�O��
	virtual bool collide(IGameObject *pObj) override;			// ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h	true:������ / false:�O��
	static void Restore(Player *m_pPlayer);		// ���L���f�B�A�t�@�C���̓ǂݍ��݁@�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	static void Finalize();				// ���L���f�B�A�t�@�C���̏����@�V�[���폜���ȂǂɌĂяo���悤�ɂ���
	virtual bool collideWithMap();		// �}�b�v�Ƃ̓����蔻�胁�\�b�h�@�����蔻��ɏ]����(x, y)���W�̍X�V���s��
	virtual int damage(float amount) override;	// �_���[�W���\�b�h (return �_���[�W)

#ifdef _DEBUG
	static void Restore(Player *pPlayer, ID2D1RenderTarget *pRT);
#endif
protected:
	float	m_fX, m_fY;	//	�����X,Y���W
	float	m_fW, m_fH, m_fXOffSet, m_fYOffSet;
	int		m_iLifeTime;
	int		m_iAttackCount;
	bool	m_bFaceToRight;		// ��������
	static Player *m_pPlayer;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pRed;
#endif
};

//	1, 2�i�ڂ̃R���W����
#define SLASH_WIDTH		40.f
#define SLASH_HEIGHT	60.f
//	3�i�ڂ̃R���W����
#define SLASH_WIDTH3	50.f
#define	SLASH_HEIGHT3	50.f
//	�_�b�V���U���̃R���W����
#define SLASH_WIDTH4	55.f
#define SLASH_HEIGHT4	50.f