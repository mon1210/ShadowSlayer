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
	EnemyAttackCollision(float x, float y, float w, float colW, float colH, int LifeTime,bool faceToRight);
						// x���W, y���W, �����蔻��`����W, �����蔻�艡��(W), �����蔻��c��(H), ���ݎ���, ��������t���O(true:�E���� / false:������)
	~EnemyAttackCollision();
	virtual bool move();	// EnemyAttackCollision�̍s���Ǘ��@�����^�C�}�[���߂���Ǝ��S�@true:���� / false:���S
	virtual void draw(ID2D1RenderTarget *pRenderTarget);		// �`�惁�\�b�h
	virtual bool collide(float x, float y, float w, float h);	// ��`�Ƃ̓����蔻�胁�\�b�h�@true:������ / false:�O��
	virtual bool collide(IGameObject *pObj) override;			// ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h�@true:������ / false:�O��
	static void Restore(Player *m_pPlayer);		// ���L���f�B�A�t�@�C���̓ǂݍ��݁@�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	static void Finalize();				// ���L���f�B�A�t�@�C���̏����@�V�[���폜���ȂǂɌĂяo���悤�ɂ���
	virtual bool collideWithMap();		// �}�b�v�Ƃ̓����蔻�胁�\�b�h
	virtual int damage(float amount);	// �_���[�W���\�b�h(return �_���[�W)

#ifdef _DEBUG
	static void Restore(Player *pPlayer, ID2D1RenderTarget *pRT);
#endif
protected:
	float	m_fX, m_fY;	//	�����X,Y���W
	float	m_fW, m_fH;
	int		m_iCount, m_iLifeTime;
	bool	m_bFaceToRight;		// ��������
	static Player *m_pPlayer;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pRed;
#endif
};

