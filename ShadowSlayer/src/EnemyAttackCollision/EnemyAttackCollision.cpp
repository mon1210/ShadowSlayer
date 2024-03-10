// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "..\stdafx\stdafx.h"
// �G�̍U���ƃv���C���[�̓����蔻�菈�����s���N���X�Ɗ֘A����萔�E�񋓌^�̒�`
#include "EnemyAttackCollision.h"
// �v���C���[�L�����N�^�[�Ƃ��̊֘A�����`�����N���X�ƒ萔/�񋓌^�̒�`
#include "..\Player\Player.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "..\Constants.h"


// �w�b�_�K�[�h���g�p���ē�x�̃C���N���[�h��h��
#ifdef _DEBUG
#include <d2d1.h>
#endif


Player *EnemyAttackCollision::m_pPlayer = NULL;


#ifdef _DEBUG
ID2D1SolidColorBrush *EnemyAttackCollision::m_pRed = NULL;
#endif


// �R���X�g���N�^
EnemyAttackCollision::EnemyAttackCollision(float x, float y, float w, float colW, float colH, int LifeTime, bool faceToRight)
{
	m_bFaceToRight = faceToRight;
	m_iLifeTime = LifeTime;
	
	m_fW = colW;
	m_fH = colH;

	if (!m_bFaceToRight) {	// ������
		m_fX = x - m_fW - w;	// �U����X���W - �����蔻�艡��(W) - �����蔻��`����W
	}
	else {		// �E����
		m_fX = x + w;	// �U����X���W + �����蔻��`����W
	}

	m_fY = y - m_fH;	// �U����Y���W - �����蔻��c��(H)


	// �`��J�E���g�폜
	m_iCount = DELETE_TEXTURE_COUNT;
}


// �f�X�g���N�^
EnemyAttackCollision::~EnemyAttackCollision()
{
}


// �s���Ǘ�
bool EnemyAttackCollision::move() {

	// LifeTime�𒴂����false��Ԃ�
	if (++m_iCount > m_iLifeTime)
		return false;

	return true;
}


// �`�惁�\�b�h
void EnemyAttackCollision::draw(ID2D1RenderTarget *pRenderTarget) {
#ifdef _DEBUG
	D2D1_RECT_F rc;		// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
	rc.left = m_pPlayer->m_fDrawX + (m_fX - m_pPlayer->getX());		// �`��̈�̍�����w��
	rc.top = m_fY;													//			�V
	rc.right = rc.left + m_fW;		//  �`��̈�̉E�����w��
	rc.bottom = rc.top + m_fH;		// 			�V
	pRenderTarget->DrawRectangle(rc, m_pRed);
#endif
}


// ��`�Ƃ̓����蔻�胁�\�b�h
bool EnemyAttackCollision::collide(float x, float y, float w, float h) {

	if (m_fX > x + w)
		return false;
	if (m_fX + m_fW < x)
		return false;
	if (m_fY > y + h)
		return false;
	if (m_fY + m_fH < y)
		return false;

	return true;
}


// ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h
bool EnemyAttackCollision::collide(IGameObject *pObj) {
	return true;
}


// �}�b�v�Ƃ̓����蔻�胁�\�b�h
bool EnemyAttackCollision::collideWithMap() { return true; }


// �_���[�W���\�b�h
int EnemyAttackCollision::damage(float amount) {
	m_iCount = m_iLifeTime << 1;
	return 1;
}


// ���L���f�B�A�t�@�C���̓ǂݍ���
void EnemyAttackCollision::Restore(Player *pPlayer) {
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
}


// ���L���f�B�A�t�@�C���̓ǂݍ���
#ifdef _DEBUG
void EnemyAttackCollision::Restore(Player *pPlayer, ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pRed);
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRed);
}
#endif


// ���L���f�B�A�t�@�C���̏���
void EnemyAttackCollision::Finalize() {
	m_pPlayer = NULL;

#ifdef _DEBUG
	SAFE_RELEASE(m_pRed);
#endif
}