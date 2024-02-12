// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "stdafx.h"
// �G�̍U���ƃv���C���[�̓����蔻�菈�����s���N���X�Ɗ֘A����萔�E�񋓌^�̒�`
#include "EnemyAttackCollision.h"
// �v���C���[�L�����N�^�[�Ƃ��̊֘A�����`�����N���X�ƒ萔/�񋓌^�̒�`
#include "Player.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "Constants.h"


// �w�b�_�K�[�h���g�p���ē�x�̃C���N���[�h��h��
#ifdef _DEBUG
#include <d2d1.h>
#endif


Player *EnemyAttackCollision::m_pPlayer = NULL;


#ifdef _DEBUG
ID2D1SolidColorBrush *EnemyAttackCollision::m_pRed = NULL;
#endif


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


/**
* @brief EnemyAttackCollision�̍s���Ǘ�
* @note	 �����^�C�}�[���߂���Ǝ��S
* return true:���� / false:���S
*/
bool EnemyAttackCollision::move() {

	// LifeTime�𒴂����false��Ԃ�
	if (++m_iCount > m_iLifeTime)
		return false;

	return true;
}


/**
* @brief �`�惁�\�b�h
*/
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


/**
* @brief ��`�Ƃ̓����蔻�胁�\�b�h
* @param[in] x �����x���W
* @param[in] y �����y���W
* @param[in] w ��`��Width
* @param[in] h ��`��Height
* @return true:������ / false:�O��
*/
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


/**
* @brief ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h
* @param[in] *pObj ���Q�[���I�u�W�F�N�g
* @return true:������ / false:�O��
*/
bool EnemyAttackCollision::collide(IGameObject *pObj) {
	return true;
}


/**
* @brief �}�b�v�Ƃ̓����蔻�胁�\�b�h
* @note	�����œ����蔻��ɏ]����(x, y)���W�̍X�V���s��
*		X������Y�����ŕ������Ĕ���B
*		�|�C���g�@: ���ꂼ������x�̐����ō��E�A�㉺�ɔ���𕪂���
*		�|�C���g�A: ���肷������̂ݍ��W���ړ������A���肵�Ȃ������͈ړ��O�̍��W���g���Ĕ�����s��
*		�|�C���g�B: X�����̔���͉����x0�̎��͍s��Ȃ��B�����x0�̎����܂߂Ă��܂��ƃW�����v���◎�����ɕǂɈ���������B
*/
bool EnemyAttackCollision::collideWithMap() { return true; }


/**
* @brief �_���[�W���\�b�h
*/
int EnemyAttackCollision::damage(float amount) {
	m_iCount = m_iLifeTime << 1;
	return 1;
}


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̓ǂݍ���
* @note		�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*/
void EnemyAttackCollision::Restore(Player *pPlayer) {
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
}


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̓ǂݍ���
* @note		�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*/
#ifdef _DEBUG
void EnemyAttackCollision::Restore(Player *pPlayer, ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pRed);
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRed);
}
#endif


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̏���
* @note		�V�[���폜���ȂǂɌĂяo���悤�ɂ���
*/
void EnemyAttackCollision::Finalize() {
	m_pPlayer = NULL;

#ifdef _DEBUG
	SAFE_RELEASE(m_pRed);
#endif
}