// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "stdafx.h"
// �X���b�V���R���W�����i�v���C���[�̍U���̓����蔻��j�N���X�̐錾
#include "SlashCollision.h"
// �v���C���[�L�����N�^�[�Ƃ��̊֘A�����`�����N���X�ƒ萔/�񋓌^�̒�`
#include "Player.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "Constants.h"
// enum�ϐ��錾�p�t�H���_
#include "Enums.h"


//--- �w�b�_�K�[�h���g���ē�x�̃C���N���[�h��h��
#ifdef _DEBUG
#include <d2d1.h>
#endif


Player *SlashCollision::m_pPlayer = NULL;

#ifdef _DEBUG
ID2D1SolidColorBrush *SlashCollision::m_pRed = NULL;
#endif

/**
* @brief SlashCollision�̃R���X�g���N�^
* @param[in] attackCount	���i�ڍU�����̃J�E���^(4:�X���C�f�B���O�U��)
* @param[in] faceToRight	��������t���O�@	true�F�E����  / false�F������
* @param[in] shadowFlag		�V���h�E����t���O�@��0�F�V���h�E /	0�F�m�[�}��	�@bit���Z�ŕ����̃t���O��ۑ����邽��int�^
*/
SlashCollision::SlashCollision(int attackCount, bool faceToRight, int shadowFlag)
{
	float shadowLength = SHADOW_START_LENGTH;
	m_fDamage = SLASH_DAMAGE;
	//	@�C���ӏ�
	if (shadowFlag != 0) {	// �V���h�E���(true)
		shadowLength = SHADOW_ON_LENGTH;
		m_fDamage = SHADOW_SLASH_DAMAGE;
	}

	m_bFaceToRight = faceToRight;
	m_iAttackCount = attackCount;

	switch (m_iAttackCount) {
	// 1.2�i�� ----------------------------------------
	case First:
	case Second:
		if (m_bFaceToRight) {	//�E����
			m_fXOffSet = R_SLASH_OFFSET_X;
		}
		else {		//������
			m_fXOffSet = L_SLASH_OFFSET_X;
		}
		m_fYOffSet = SLASH_OFFSET_Y;
		m_fW = SLASH_WIDTH * shadowLength;
		m_fH = SLASH_HEIGHT * shadowLength;
		break;

	// 3�i�� ------------------------------------------
	case Third:
		if (m_bFaceToRight) {	//�E����
			m_fXOffSet = R_THIRD_SLASH_OFFSET_X;
		}
		else {		//������
			m_fXOffSet = L_THIRD_SLASH_OFFSET_X;
		}
		m_fYOffSet = THIRD_SLASH_OFFSET_Y;
		m_fW = SLASH_WIDTH3 * shadowLength;
		m_fH = SLASH_HEIGHT3 * shadowLength;
		break;

	// �X���C�f�B���O ---------------------------------
	case Slide:
		if (m_bFaceToRight) {	//�E����
			m_fXOffSet = R_DASH_SLASH_OFFSET_X;
		}
		else {		//������
			m_fXOffSet = L_DASH_SLASH_OFFSET_X;
		}
		m_fYOffSet = DASH_SLASH_OFFSET_Y;
		m_fW = SLASH_WIDTH4 * shadowLength;
		m_fH = SLASH_HEIGHT4 * shadowLength;
		break;
	default:
		if (m_bFaceToRight) {	//�E����
			m_fXOffSet = R_SLASH_OFFSET_X;
		}
		else {		//������
			m_fXOffSet = L_SLASH_OFFSET_X;
		}
		break;
	}
	
	
	// �����蔻��폜
	m_iLifeTime = INVINCIBLE_TIME;	// @�C���ӏ�
}


// �f�X�g���N�^
SlashCollision::~SlashCollision()
{
}


/**
* @brief SlashCollision�̐���
* @note	 �������Ԃ��߂���Ə���
* return true:���� / false:���S
*/
bool SlashCollision::move() {
	m_iLifeTime++;
	m_fX = m_pPlayer->getX() + m_fXOffSet;
	m_fY = m_pPlayer->getY() + m_fYOffSet;

	if (m_iLifeTime > SLASH_LIFE_TIME)
		return false;

	return true;
}


/**
* @brief �`�惁�\�b�h
*/
void SlashCollision::draw(ID2D1RenderTarget *pRenderTarget) {
#ifdef _DEBUG
	D2D1_RECT_F rc;		// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
	rc.left = m_pPlayer->m_fDrawX + (m_fX - m_pPlayer->getX());		// �`��̈�̍�����w��
	rc.top = m_pPlayer->m_fDrawY + (m_fY - m_pPlayer->getY());		//			�V
	rc.right = rc.left + m_fW;		// �`��̈�̉E�����w��
	rc.bottom = rc.top + m_fH;		//			�V
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
bool SlashCollision::collide(float x, float y, float w, float h) { 

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
bool SlashCollision::collide(IGameObject *pObj) { 
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
bool SlashCollision::collideWithMap() { return true; }


/**
* @brief �_���[�W���\�b�h
*/
int SlashCollision::damage(float amount) {
	m_iLifeTime = SLASH_DAMAGE_LIFE_TIME;
	return SLASH_RETURN_DAMAGE;
}


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̓ǂݍ���
* @note		�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*/
void SlashCollision::Restore(Player *pPlayer) {
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
}


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̓ǂݍ���
* @note		�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*/
#ifdef _DEBUG
void SlashCollision::Restore(Player *pPlayer, ID2D1RenderTarget *pRT) {
	//--- SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
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
void SlashCollision::Finalize() {
	m_pPlayer = NULL;

#ifdef _DEBUG
	SAFE_RELEASE(m_pRed);
#endif
}