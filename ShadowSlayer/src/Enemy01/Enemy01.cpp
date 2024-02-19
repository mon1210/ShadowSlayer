// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "..\stdafx\stdafx.h"
// Direct2D�̃w�b�_�t�@�C�����C���N���[�h
#include <d2d1.h>
// �Q�[�����̃X�e�[�W�֘A�N���X�Ɨ񋓌^�̒�`
#include "..\Stage\Stage.h"
// �Q�[���̔w�i���Ǘ�����N���X�Ɗ֘A�f�[�^���`
#include "..\BG\BG.h"
// �Q�[�����̓G�L�����N�^�[�N���X�Ɗ֘A����萔�E�񋓌^�̒�`
#include "Enemy01.h"
// �e�N�X�`���ƃr�b�g�}�b�v�̓ǂݍ��݂ƊǗ��Ɋւ���N���X�ƃC���^�[�t�F�[�X���`
#include "..\TextureLoader\TextureLoader.h"
// �G�̍U���ƃv���C���[�̓����蔻�菈�����s���N���X�Ɗ֘A����萔�E�񋓌^�̒�`
#include "..\EnemyAttackCollision\EnemyAttackCollision.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "..\Constants.h"
// enum�ϐ��錾�p�t�H���_
#include "..\Enums.h"


ID2D1Bitmap *Enemy01::m_pTexture = NULL;
ID2D1Bitmap *Enemy01::m_pTexture_Left = NULL;
BG	*Enemy01::m_pBG = NULL;


// Texture��Index��t�^�E�w��
// @note: X:0�`7,Y:8�`10
float Enemy01::m_fTextureCoord[] = {
	//	X
	0, 32.f, 64.f, 96.f, 128.f, 160.f, 192.f, 224.f,
	//	Y
	0, 25.f, 50.f
};


#ifdef _DEBUG
ID2D1SolidColorBrush *Enemy01::m_pWhite = NULL;
#endif


/**
* @brief Enemy01�̃R���X�g���N�^
* @param[in] x	�G�l�~�[��x���W
* @param[in] y	�G�l�~�[��y���W
*/
Enemy01::Enemy01(Stage *pParent, float x, float y)
{
	m_pParent = pParent;
	m_fX = x;
	m_fY = y;
	m_fY_prev = m_fY;
	m_fVX = 0;
	m_fVY = 0;


	m_bAttacked = false;
	m_bFaceToRight = false;
	m_bIs_OnSlope = false;

	m_iCount = 0;
	m_iEnemyStateFlag |= EFG_IDLE;
}


// �f�X�g���N�^
Enemy01::~Enemy01()
{
}


/**
* @brief Enemy01�̃A�j���[�V����
* @note	 �_���[�W���󂯂�Ǝ��S
* return true:���� / false:���S
*/
bool Enemy01::move() {
	// �����J�E���^(�e�N�X�`�������Ǘ�)
	m_iCount++;
	// �v���C���[�Ƃ̍��W�̍�
	m_fDiffX = m_fX - m_pParent->m_fPlayerX;
	m_fDiffY = m_fY - m_pParent->m_fPlayerY;
	// �v���C���[�Ƃ̋����x�N�g��
	float l = m_fDiffX * m_fDiffX + m_fDiffY * m_fDiffY;

	// ��Ԃɍ��킹�ď���
	switch (m_iEnemyStateFlag) {
	case EFG_IDLE:
		if (m_iCount > ENEMY_MOVE_ABLE_TIMER && l < ENEMY01_SEARCH_DISTANCE * ENEMY01_SEARCH_DISTANCE) {
			m_iEnemyStateFlag |= EFG_MOVE;	// ������^�C�}�[���v���C���[�Ƃ̋����x�N�g�����G�l�~�[�̎���x�N�g���ȉ��Ȃ�MOVE
			m_iCount = 0;
		}
		break;

	case EFG_MOVE:
		if (m_fDiffX > 0) {		// ������
			m_fVX = -MOVE_SPEED;
			m_bFaceToRight = false;
		}
		else {					// �E����
			m_fVX = MOVE_SPEED;
			m_bFaceToRight = true;
		}
		// �v���C���[�Ƃ̋����x�N�g�����G�l�~�[�̍U���͈̓x�N�g���ȉ���ATTACK
		if (l <= ENEMY01_ATTACK_DISTANCE * ENEMY01_ATTACK_DISTANCE) {
			m_iEnemyStateFlag = 0;
			m_iEnemyStateFlag |= EFG_ATTACK;
			m_fVX = 0;		// ��~
			m_iCount = 0;	// �J�E���^������
		}
		break;

	case EFG_ATTACK:
		if (m_iCount == AT_COLLISION_GENERATE_COUNT) {	// AttackTexture�œ����蔻�萶��
			IGameObject *pObj = new EnemyAttackCollision(m_fX, m_fY, AT_COLLISION_POS_X, AT_COLLISION_WIDTH, AT_COLLISION_HEIGHT, COLLISION_LIFE_TIME, m_bFaceToRight);
			m_pParent->addEnemyAttacks(pObj);
		}
		if (m_iCount > AT_FINISH_COUNT) {		// AttackEndTexture�ŏ���������
			m_iEnemyStateFlag = 0;
			m_iCount = 0;
		}
		break;

	case EFG_JUMP:	// ����������
		m_fVY = m_fY - m_fY_prev + GRAVITY;
		if (m_fVY > FALL_SPEED_MAX) {		// �������x����
			m_fVY = FALL_SPEED_MAX;
		}

		m_fY_prev = m_fY;
		break;

	case EFG_DEATH:
		m_fVX = 0;	// ��~
		if (m_iCount >= EnemyTexture::Death_Y)	// ���S�����I��
			return false;
		break;

	default:
		break;
	}
	
	return true;
}


/**
* @brief �`�惁�\�b�h
*/
void Enemy01::draw(ID2D1RenderTarget *pRenderTarget) {

	D2D1_RECT_F rc, 	// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
				src;	// �e�N�X�`���̒��ł̍��W���w�肵�A�e�N�X�`�������肷��ϐ�
	int Xindex = 0, Yindex = 0;

	// �`��̈�̐ݒ�
	rc.left = m_pParent->m_fPlayerDrawX + 
		(m_fX - m_pParent->m_fPlayerX) - DRAW_POS_X;	// �`��̈�̍�����w��
	rc.top = m_fY - DRAW_POS_Y;							//			�V
	rc.right = rc.left + DRAW_WIDE;		// �`��̈�̉E�����w��
	rc.bottom = m_fY;					//			�V

	// �G�l�~�[�̏�Ԃɍ��킹�ĕ`�揈��
	switch (m_iEnemyStateFlag) {
	case EFG_IDLE:
	case EFG_JUMP:
		Xindex = (m_iCount % AllMoveTexture) >> 2;		// ( >> 2 == * 4 )	�A�j���[�V�������[�v
		Yindex = EnemyTexture::IdleAndMove_Y;
		break;

	case EFG_MOVE:
		Xindex = EnemyTexture::Idle_X + ((m_iCount % AllMoveTexture) >> 2);		// ( >> 2 == * 4 )	�A�j���[�V�������[�v
		Yindex = EnemyTexture::IdleAndMove_Y;
		break;

	case EFG_ATTACK:
		if (m_iCount < 2) {	// �U���O
			Xindex = EnemyTexture::Default_X;		// @�C���ӏ�
		}
		else if (m_iCount < AT_COLLISION_GENERATE_COUNT) {	// �U���\��
			Xindex = EnemyTexture::AttackStance_X;
		}
		else {	// �U������
			Xindex = (m_iCount >> 2); // ( >> 2 == * 4 )	�A�j���[�V�������[�v
		}
		Yindex = EnemyTexture::Attack_Y;
		break;

	case EFG_DEATH:
		Xindex = m_iCount >> 1; // ( >> 2 == * 4 )	�A�j���[�V�������[�v
		Yindex = EnemyTexture::Death_Y;
		break;

	default:
		break;
	}

	float tx, ty;
	// Texture���
	tx = m_fTextureCoord[Xindex];
	ty = m_fTextureCoord[Yindex];

	src.top = ty;	// �e�N�X�`�����W�̍�����w��p�̏����
	src.bottom = src.top + ENEMY_TEXTURE_HEIGHT;	// �e�N�X�`�����W�̉E�����w��p�̏����

	if (!(m_bFaceToRight)) {	// ������
		src.left = tx;		// �e�N�X�`�����W�̍�����w��
		src.right = tx + ENEMY_TEXTURE_WIDTH;		// �e�N�X�`�����W�̉E����w��
		pRenderTarget->DrawBitmap(m_pTexture_Left, rc, ENEMY_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
	}
	else {		// �E����
		src.left = ENEMY_LA_TEXTURE_COORD - tx;		// �e�N�X�`�����W�̍�����w��
		src.right = src.left + ENEMY_TEXTURE_WIDTH;			// �e�N�X�`�����W�̉E�����w��
		pRenderTarget->DrawBitmap(m_pTexture, rc, ENEMY_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
	}

// �f�o�b�O�p�����蔻��`��
#ifdef _DEBUG
	rc.left += ENEMY_COLLISION_OFFSET_L;	// �`��̈�̍�����w��
	rc.top += ENEMY_COLLISION_OFFSET_T;		//			�V
	rc.right = rc.left + ENEMY_TEXTURE_WIDTH;	// �`��̈�̉E�����w��
	pRenderTarget->DrawRectangle(rc, m_pWhite);
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
bool Enemy01::collide(float x, float y, float w, float h) {

	if (m_fX - ENEMY_TEXTURE_W_HALF > x + w)
		return false;
	if (m_fX + ENEMY_TEXTURE_W_HALF < x)
		return false;
	if (m_fY < y)
		return false;
	if (m_fY - ENEMY_TEXTURE_H_HALF > y + h)
		return false;

	return true;
}


/**
* @brief ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h
* @param[in] *pObj ���Q�[���I�u�W�F�N�g
* @return true:������ / false:�O��
*/
bool Enemy01::collide(IGameObject *pObj) {

	return pObj->collide(m_fX - ENEMY_TEXTURE_W_HALF, m_fY - ENEMY_TEXTURE_HEIGHT, ENEMY_TEXTURE_WIDTH, ENEMY_TEXTURE_H_HALF);
}


/**
* @brief �}�b�v�Ƃ̓����蔻�胁�\�b�h
* @note	�����œ����蔻��ɏ]����(x, y)���W�̍X�V���s��
*		X������Y�����ŕ������Ĕ���B
*		�|�C���g�@: ���ꂼ������x�̐����ō��E�A�㉺�ɔ���𕪂���
*		�|�C���g�A: ���肷������̂ݍ��W���ړ������A���肵�Ȃ������͈ړ��O�̍��W���g���Ĕ�����s��
*		�|�C���g�B: X�����̔���͉����x0�̎��͍s��Ȃ��B�����x0�̎����܂߂Ă��܂��ƃW�����v���◎�����ɕǂɈ���������B
*/
bool Enemy01::collideWithMap() {
	float l = m_fX - ENEMY_TEXTURE_W_HALF, t = m_fY - ENEMY_TEXTURE_H_HALF, w = ENEMY_TEXTURE_WIDTH, h = ENEMY_TEXTURE_H_HALF;
	float r = l + ENEMY_TEXTURE_WIDTH, b = m_fY;

	//	�⓹�ɓ��������A�o�����𔻒�
	//	��̃x�N�g�����v�Z���郁�\�b�h�́A��ɓ������Ƃ��ɂ����Ă΂��悤�ɂ��Ă���(�{�^���A�Ŗh�~�Ǝ�������)
	if (!m_bIs_OnSlope) {
		if (m_pBG->tile_code(m_fX, t + h) > SLOPE_BETWEEN_TEXTURE_NUMBER) {
			m_pBG->getStepVector(m_fX, t + h, &step);
			m_bIs_OnSlope = true;
		}
	}
	else if (m_fX > step.x + step.vx || m_fX < step.x) {
		m_bIs_OnSlope = false;
		step.x = 0, step.y = 0;
		step.vx = 0, step.vy = 0;
	}


	//	���n�ł̔���-----------------------------------------------------------------------------------------
	//	�⓹���\������}�b�v�`�b�v�Ƃ̔���͎��Ȃ��悤�ɂ��Ă���
	if (!m_bIs_OnSlope) {
		if (m_fVY >= 0) {	// �W�����v�A�K�i��蒆
			int code1 = m_pBG->tile_code(l + 2, t + h + m_fVY);		// +2 / -2 ���邱�ƂŁA�u���b�N�אڎ��ɃW�����v����Ə�ɏ���Ă��܂����ۂ�h����
			int code2 = m_pBG->tile_code(l + w - 2, t + h + m_fVY);
			// ���Ɣ���
			if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER)) { 
				m_iEnemyStateFlag &= ~EFG_JUMP;
				int row = (int)(b + m_fVY) >> MAP_CHIP_SIZE_BITS;
				m_fY = (float)(row << MAP_CHIP_SIZE_BITS);
				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {	//	�󒆎�
				m_iEnemyStateFlag = 0;
				m_iEnemyStateFlag |= EFG_JUMP;
				m_fY += m_fVY;
			}
		}

		if (m_fVX > 0) {	// �E�ړ�
			int code1 = m_pBG->tile_code(l + w + m_fVX, t);
			int code2 = m_pBG->tile_code(l + w + m_fVX, t + h - 4);	// -4 ���邱�ƂŒn�ʂƔ��肪�o�Ă��܂����Ƃ�h����
			// �E���̔���
			if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER)) {
				int col = (int)(l + w + m_fVX) >> MAP_CHIP_SIZE_BITS;
				m_fX = (float)((col << MAP_CHIP_SIZE_BITS) - ((int)w >> 1));
			}
			else {
				m_fX += m_fVX;
			}
		}
		else if (m_fVX < 0) {	// ���ړ�
			int code1 = m_pBG->tile_code(l + m_fVX, t);
			int code2 = m_pBG->tile_code(l + m_fVX, t + h - 4); // -4 ���邱�ƂŒn�ʂƔ��肪�o�Ă��܂����Ƃ�h����
			// �����̔���
			if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER)) {
				int col = (int)(l + m_fVX) >> MAP_CHIP_SIZE_BITS;
				m_fX = (float)((col << MAP_CHIP_SIZE_BITS) + ((int)w >> 1) + iMAP_CHIP_WIDTH);
			}
			else {
				m_fX += m_fVX;
			}
		}
	}
	//	�⓹�ł̔���-----------------------------------------------------------------------------------------
	else {

		if (m_fVY >= 0) {	// �W�����v�A�K�i��蒆
			float stepY, ratio;
			ratio = (m_fX - step.x) / step.vx;
			stepY = step.y + step.vy * ratio;
			if (m_fY + m_fVY + 5.f >= stepY) {	//	+5.f ���Ă����Ɖ���̎��ɃK�^�K�^���˂Ȃ��Ȃ�
				m_iEnemyStateFlag &= ~EFG_JUMP;
				m_fY = stepY;
				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {		// �󒆎�
				m_iEnemyStateFlag |= EFG_JUMP;
				m_fY += m_fVY;
			}
		}

		m_fX += m_fVX;

	}

	return true;
}


/**
* @brief �_���[�W���\�b�h
*/
int Enemy01::damage(float amount) {
	m_iEnemyStateFlag = 0;
	m_iEnemyStateFlag |= EFG_DEATH;		// ���S��Ԃ�
	m_iCount = DELETE_TEXTURE_COUNT;	// �`��J�E���g�폜
	return 1;
}


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̓ǂݍ���
* @note		�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*/
void Enemy01::Restore(ID2D1RenderTarget *pRT, BG *pBG) {
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexture_Left);
	m_pBG = NULL;

	// �摜���������ɓǂݍ���
	TextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\slime_32x25.png"), &m_pTexture);
	TextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\slime_left_32x25.png"), &m_pTexture_Left);
	m_pBG = pBG;

#ifdef _DEBUG
	SAFE_RELEASE(m_pWhite);
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhite);
#endif
}


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̏���
* @note		�V�[���폜���ȂǂɌĂяo���悤�ɂ���
*/
void Enemy01::Finalize() {
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexture_Left);
	m_pBG = NULL;

#ifdef _DEBUG
	SAFE_RELEASE(m_pWhite);
#endif
}