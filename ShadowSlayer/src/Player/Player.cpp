// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "..\stdafx\stdafx.h"
// Direct2D�̃w�b�_�t�@�C�����C���N���[�h
#include <d2d1.h>
// �v���C���[�L�����N�^�[�Ƃ��̊֘A�����`�����N���X�ƒ萔/�񋓌^�̒�`
#include "Player.h"
// �Q�[�����̃X�e�[�W�֘A�N���X�Ɨ񋓌^�̒�`
#include "..\Stage\Stage.h"
// �e�N�X�`���ƃr�b�g�}�b�v�̓ǂݍ��݂ƊǗ��Ɋւ���N���X�ƃC���^�[�t�F�[�X���`
#include "..\TextureLoader\TextureLoader.h"
// �Q�[���̔w�i���Ǘ�����N���X�Ɗ֘A�f�[�^���`
#include "..\BG\BG.h"
// �e�N���X�̐錾
#include "..\Shot\Shot.h"
// �X���b�V���R���W�����i�v���C���[�̍U���̓����蔻��j�N���X�̐錾
#include "..\SlashCollision\SlashCollision.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "..\Constants.h"
// enum�ϐ��錾�p�t�H���_
#include "..\Enums.h"


/**
* @brief Player�̃R���X�g���N�^
*/
Player::Player(Stage *pParent, BG *pBG)
{
	ID2D1RenderTarget *pTarget = NULL;
	m_pBlackBrush = NULL;
	m_pParent = pParent;
	m_pBG = pBG;
	m_pTexture = NULL;
	m_pTexture_Left = NULL;

	m_bDeath = false;
	m_bDamage = false;
	m_bSlidingTurn = false;
	m_bJump = true;
	m_bAttack = true;
	m_bShot = true;
	m_bShadow = true;
	m_bFaceToRight = true;
	m_bIs_AttackCancel_OK = false;
	m_bIs_OnSlope = false;
	m_iPlayerStateFlag = 0;
	m_iShadowStateFlag = 0;

	m_iFrame = 0;
	m_iIdleFrame = 0;
	m_iAnimFrame = 0;
	m_iSlidingFrame = 0;
	m_iAttackFrame = 24;
	m_iAttackCount = 0;
	m_iSlidingAttackFrame = 0;
	m_iShadowSwitchFrame = 0;

	m_fX = 128.f;//64.f
	m_fY = 320.f;
	m_fY_prev = m_fY;
	m_fVX = 0.f;
	m_fVY = 0.f;
	m_fDrawX = 128.f;//64.f
	m_fDrawY = 320.f;
	m_fOpacity = 1.0f;
	m_fShadowOpacity = 0.5f;

	m_fMapWidth = 0;
	m_fMapHight = 0;
	m_fRBoader = 0;

	m_fPl = PHB_L; m_fPt = PHB_T; m_fPw = PHB_W; m_fPh = PHB_H;
	m_fHP = 100.f;
	m_fMana = 100.f;

	pTarget = m_pParent->GetRenderTarget();
	if (pTarget) {
		TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\player.png"), &m_pTexture);
		TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\player_L.png"), &m_pTexture_Left);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
		m_pBlackBrush->SetOpacity(m_fShadowOpacity);

#ifdef _DEBUG
		m_pWhiteBrush = NULL;
		pTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhiteBrush);
#endif // _DEBUG

		pTarget->Release();
		pTarget = NULL;
	}
}


// �f�X�g���N�^
Player::~Player()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexture_Left);
	SAFE_RELEASE(m_pBlackBrush);

#ifdef _DEBUG
	SAFE_RELEASE(m_pWhiteBrush);
#endif // _DEBUG

}


/**
* @brief �v���C���[�̃A�j���[�V�������\�b�h
* @return true:���� / false:���S
* @note �����ł͉����x�̐ݒ肾���s���A(x, y)���W�̍X�V��collide() �ōs��
*/
bool Player::move() {
	if (m_fHP <= 0)
	{
		return false;
	}


	//	�t���O���� �t���[���J�E���g �t���O�N���A �����x�N���A----------------------
	//	���̒��Ńt���[���J�E���g���C���N�������g���Ă���̂ŁA�擪�ɒu���Ă����Ȃ��� m_iSlidingFrame = FPS �ł��`�揈����ʂ��Ă��܂��B(m_iSlidingFrame < FPS)

	//	shadow���t���[���J�E���g
	if (m_iShadowStateFlag == SFG_SWITCH) {	// Shadow���[�h�؂�ւ���
		if (m_iShadowSwitchFrame < SHADOW_MODE_COUNT) {
			m_iShadowSwitchFrame++;
			m_fShadowOpacity += SHADOW_DRAW_OPACITY;
			m_fOpacity -= SHADOW_DRAW_OPACITY;
			m_pBlackBrush->SetOpacity(m_fShadowOpacity);
			return true;
		}
		else {
			m_iShadowSwitchFrame = 0;
			m_iShadowStateFlag = 0;
			m_iShadowStateFlag |= SFG_ON;
		}
	}
	else if (m_iShadowStateFlag == SFG_ON) {	// ������MANA����
		m_fMana -= SHADOW_MANA_DEPLETION;
		if (m_fMana < 0) {	// Shadow��������
			m_fMana = 0;
			m_iShadowStateFlag |= SFG_SWITCH;
		}
	}
	else if (m_iShadowStateFlag == (SFG_ON | SFG_SWITCH)) {	// Shadow������
		if (m_iShadowSwitchFrame < SHADOW_MODE_COUNT) {
			m_iShadowSwitchFrame++;
			m_fShadowOpacity -= SHADOW_DRAW_OPACITY;
			m_fOpacity += SHADOW_DRAW_OPACITY;
			m_pBlackBrush->SetOpacity(m_fShadowOpacity);
			return true;
		}
		else {
			m_iShadowSwitchFrame = 0;
			m_iShadowStateFlag = 0;
		}
	}

	

	{
		UINT check_flag = 0;
		float check_VX = 0;

		switch (m_iPlayerStateFlag) {
		case PFG_IDLE:
			// �A�j���[�V��������芊�炩�ɂ��邽�߂� FPS * 2
			m_iIdleFrame = (++m_iIdleFrame) % (FPS * 2);	// Idle�A�j���[�V�������[�v
			m_iAnimFrame = 0;
			break;

		case PFG_DAMAGED:
		case PFG_DAMAGED | PFG_JUMP:
			if (m_iDamagedFrame < PLAYER_DAMAGE_ANIM_TIME) {		// �_���[�W���󂯂��A�j���[�V�����̍Đ�����
				check_flag |= PFG_DAMAGED;
				check_VX = m_fVX;
			}
			break;

		case PFG_JUMP:
			// �A�j���[�V��������芊�炩�ɂ��邽�߂� FPS * 2
			m_iIdleFrame = (++m_iIdleFrame) % (FPS * 2);
			m_iAnimFrame = 0;
			check_VX = m_fVX;
			break;

		case PFG_WALK:
		case PFG_WALK | PFG_JUMP:
			m_iIdleFrame = 0;
			m_iAnimFrame++;
			m_iAttackFrame = PLAYER_ATTTACK_TIME;
			break;
                                                                                                                                                                                                                      
		case PFG_SLIDE:
		case PFG_SLIDE | PFG_JUMP:
			// �X���C�h������Ƀu���b�N�������ė�����ԂɂȂ�Ȃ��Ƃ��A�X���C�f�B���O���p���B�t���[���J�E���g���i�߂Ȃ�
			if (0 != (m_pBG->tile_code(PHB_L, PHB_T) || m_pBG->tile_code(PHB_L + PHB_W, PHB_T))) {
				check_flag |= (PFG_SLIDE | PFG_SLIDE_CONTINUE);
				check_VX = m_fVX;
			}
			else {
				m_iSlidingFrame++;
				check_VX = m_fVX;
			}
			m_iIdleFrame = 0;
			m_iAnimFrame = 0;
			m_iAttackFrame = PLAYER_ATTTACK_TIME;
			break;

		case PFG_ATTACK:
		case PFG_ATTACK | PFG_JUMP:
			if (m_iAttackFrame < ATTACK_COLLISION_END) {
				m_iAttackFrame++;
				check_flag |= (PFG_ATTACK & m_iPlayerStateFlag);
				if (m_iAttackFrame == ATTACK_COLLISION_GENERATE) {	// �����蔻�萶��
					IGameObject *pObj = new SlashCollision(m_iAttackCount, m_bFaceToRight, m_iShadowStateFlag);
					m_pParent->addSlashes(pObj);
				}
			}
			else if (m_iAttackFrame < ATTACK_END) {
				m_iAttackFrame++;
				check_flag |= (PFG_ATTACK & m_iPlayerStateFlag);
				m_bIs_AttackCancel_OK = true;
			}
			else {
				m_bIs_AttackCancel_OK = false;
				m_iAttackCount = 0;
				m_iAttackFrame++;
			}
			m_iIdleFrame = 0;
			m_iAnimFrame = 0;
			break;

		case PFG_SLIDEATTACK:
		case PFG_SLIDEATTACK | PFG_JUMP:
			if (m_iSlidingAttackFrame++ < SL_ATTACK_END) {
				check_flag |= PFG_SLIDEATTACK;
				if (m_fVX > 0) {	// �E����
					check_VX = m_fVX - SLIDE_DECELERATION_SPEED;
				}
				else {	// ������
					check_VX = m_fVX + SLIDE_DECELERATION_SPEED;
				}
				if (m_iSlidingAttackFrame == ATTACK_COLLISION_GENERATE) {
					IGameObject *pObj = new SlashCollision(AttackCount::Slide, m_bFaceToRight, m_iShadowStateFlag);
					m_pParent->addSlashes(pObj);
				}
			}
			m_iIdleFrame = 0;
			m_iAnimFrame = 0;
			m_iAttackFrame = PLAYER_ATTTACK_TIME;
			break;

		default:
			break;
		}


		if (m_bDamage) {	// �A���_���[�W�̖�����
			m_iDamagedFrame++;
			if (m_iDamagedFrame == PLAYER_INVINCIBLE_TIME) {
				m_iDamagedFrame = 0;
				m_bDamage = false;
				m_fOpacity = PLAYER_MAX_OPACITY;	// Player�̓����x��߂�
			}
		}
		
		if (m_iPlayerStateFlag & PFG_JUMP) {
			check_flag |= PFG_JUMP;
		}

		m_iPlayerStateFlag &= 0;
		m_iPlayerStateFlag |= check_flag;
		m_fVX = check_VX;
	}
	// -------------------------------------

	// shadow����--------------------------
	if (GetAsyncKeyState(0x47)) {
		//	�V���h�E��
		if ( !m_bShadow && !(m_iPlayerStateFlag & (PFG_JUMP | PFG_SLIDE | PFG_DAMAGED)) && (m_fMana >= SHADOW_AVAILABLE_MANA) ) {
			if ((m_iShadowStateFlag == SFG_OFF)) {
				m_iShadowStateFlag |= SFG_SWITCH;
				m_iPlayerStateFlag &= 0;
				m_iIdleFrame = 0;
				m_iAnimFrame = 0;
				m_iAttackFrame = PLAYER_ATTTACK_TIME;
				m_fVX = 0;
				m_bShadow = true;
				return true;
			}
		}

		//	�V���h�E������
		if ( !m_bShadow && (m_iShadowStateFlag == SFG_ON) ) {
			m_iShadowStateFlag |= SFG_SWITCH;
		}
	}
	else {
		m_bShadow = false;
	}

	//-------------------------------------

	if (m_iPlayerStateFlag & PFG_DAMAGED) {
		;
	}
	else if (!m_bIs_AttackCancel_OK) {
		// JUMP---------------------------------
		if (GetAsyncKeyState(VK_SPACE)) {
			if (!(m_iPlayerStateFlag & (PFG_JUMP | PFG_ATTACK | PFG_SLIDEATTACK)) && !m_bJump) { // �W�����v���܂��͍U�����łȂ���΃W�����v�ł���
				m_fVY = JUMP;
				m_fY_prev = m_fY;
				m_bJump = true;
			}
		}
		else {
			m_bJump = false;
		}

		// SLIDING ------------------------------
		if (GetAsyncKeyState(VK_SHIFT)) {
			if (!(m_iPlayerStateFlag & (PFG_ATTACK | PFG_SLIDEATTACK)) && m_iSlidingFrame < FPS) {
				if (m_iSlidingFrame == 0) {
					if (m_bFaceToRight) {	// �E����(��)�Ɉړ�
						m_fVX = 1.f * SLIDESPEED;
					}
					else {		// ������(��)�Ɉړ�
						m_fVX = -1.f * SLIDESPEED;
					}
				}
				else {
					if (m_bFaceToRight) {
						if (!(m_bSlidingTurn) && GetAsyncKeyState(VK_LEFT)) {
							m_fVX *= -1.f;		// ����̒l�𕉂ɕϊ� 
							m_bSlidingTurn = true;
							m_bFaceToRight = false;
						}
					}
					else {
						if (!(m_bSlidingTurn) && GetAsyncKeyState(VK_RIGHT)) {
							m_fVX *= -1.f;		// ����̒l�𕉂ɕϊ� 
							m_bSlidingTurn = true;
							m_bFaceToRight = true;
						}
					}
				}
				m_iPlayerStateFlag |= PFG_SLIDE;
			}
		}
		else {	//	�����̓L�[�𗣂��Ȃ��ƍX�V����Ȃ��̂ŁA�X���C�f�B���O�����̏������Z�b�g��������ɒu���Ă����Ɖ������ςȂ��ŘA�ł����̂�h����
			m_iSlidingFrame = 0;
			m_bSlidingTurn = false;
		}

		// ATTACK----------------------------
		// @note: �U���͂R�i�܂ŉ\�Bm_iAttackCount�Œi�����Ǘ�
		if (GetAsyncKeyState(KEY_ATTACK)) {	
			if (!(m_iPlayerStateFlag & (PFG_SLIDE | PFG_SLIDEATTACK))) {
				if (m_iAttackFrame == PLAYER_ATTTACK_TIME && !m_bAttack && m_iAttackCount == 0) {
					m_iPlayerStateFlag |= PFG_ATTACK;
					m_iAttackFrame = 0;
					m_iAttackCount++;
					m_bAttack = true;
				}
			}
		}
		else {
			m_bAttack = false;
		}
		// �A�^�b�N��i��
		if ( (m_iAttackCount == AttackCount::First) && (m_iPlayerStateFlag & PFG_ATTACK) && (m_iAttackFrame > SECOND_ATTACK_THRESHOLD) ) {
			if (GetAsyncKeyState(KEY_ATTACK) && !m_bAttack) {
				m_iAttackFrame = 0;
				m_iAttackCount++;
				m_bAttack = true;
			}
		}
		// �A�^�b�N�O�i��
		if ( (m_iAttackCount == AttackCount::Second) && (m_iPlayerStateFlag & PFG_ATTACK) && (m_iAttackFrame > THIRD_ATTACK_THRESHOLD)) {
			if (GetAsyncKeyState(KEY_ATTACK) && !m_bAttack) {
				m_iAttackFrame = 0;
				m_iAttackCount++;
				m_bAttack = true;
			}
		}

		// WALK -----------------------------
		if (!(m_iPlayerStateFlag & (PFG_SLIDE | PFG_ATTACK | PFG_SLIDEATTACK))) {
			if (GetAsyncKeyState(VK_RIGHT)) {		// �E����(��)�Ɉړ�
				m_fVX = 1.0f * WALKSPEED;		
				m_iPlayerStateFlag |= PFG_WALK;
				m_bFaceToRight = true;
			}
			else if (GetAsyncKeyState(VK_LEFT)) {	// ������(��)�Ɉړ�
				m_fVX = -1.0f * WALKSPEED;
				m_iPlayerStateFlag |= PFG_WALK;
				m_bFaceToRight = false;
			}
		}

	}
	else {	// m_bIs_AttackCancel_OK = true �̎�---------------------------------

		// WALK -------------------------------
		if (GetAsyncKeyState(VK_RIGHT)) {		// �E����(��)�Ɉړ�
			m_fVX = 1.0f * WALKSPEED;
			m_iPlayerStateFlag |= PFG_WALK;
			m_bFaceToRight = true;
			m_iPlayerStateFlag &= ~PFG_ATTACK;
		}
		else if (GetAsyncKeyState(VK_LEFT)) {	// ������(��)�Ɉړ�
			m_fVX = -1.0f * WALKSPEED;
			m_iPlayerStateFlag |= PFG_WALK;
			m_bFaceToRight = false;
			m_iPlayerStateFlag &= ~PFG_ATTACK;
		}	// JUMP---------------------------------
		else if (GetAsyncKeyState(VK_SPACE)) {
			if (!(m_iPlayerStateFlag & PFG_JUMP)) { // �W�����v���łȂ���΃W�����v�ł���
				m_fVY = JUMP;
				m_fY_prev = m_fY;
				m_bJump = true;
				m_iPlayerStateFlag &= ~PFG_ATTACK;
			}
		}	// SLIDING ------------------------------
		else if (GetAsyncKeyState(VK_SHIFT)) {
			if (m_bFaceToRight) {		// �E����(��)�Ɉړ�
				m_fVX = 1.f * SLIDESPEED;
			}
			else {			// ������(��)�Ɉړ�
				m_fVX = -1.f * SLIDESPEED;
			}
			m_iPlayerStateFlag |= PFG_SLIDE;
			m_iPlayerStateFlag &= ~PFG_ATTACK;
		}

		//	ATTACK -----------------------------------
		if (m_iPlayerStateFlag & PFG_ATTACK) {	// PFG_ATTACK�t���O���c���Ă����� = �ʍs�����Ă��Ȃ�������
			if (GetAsyncKeyState(KEY_ATTACK)) {
				// �A�^�b�N��i��
				if (!m_bAttack && m_iAttackCount == AttackCount::First && m_iAttackFrame < C_OK_SECOND_ATTACK_THRESHOLD) {
					m_iAttackFrame = 0;
					m_bAttack = true;
					m_iAttackCount++;
					m_bIs_AttackCancel_OK = false;
				}	// �A�^�b�N�O�i��
				else if (!m_bAttack && m_iAttackCount == AttackCount::Second && m_iAttackFrame < C_OK_THIRD_ATTACK_THRESHOLD) {
					m_iAttackFrame = 0;
					m_bAttack = true;
					m_iAttackCount++;
					m_bIs_AttackCancel_OK = false;
				}
			}
			else {
				m_bAttack = false;
			}
		}
		else {	// �ʍs�����Ă�����L�����Z���\��ԏI��
			m_bIs_AttackCancel_OK = false;
			m_iAttackCount = 0;
		}
		
	}


	//	�e�폈��---------------------------------



	// �󒆎��̏���
	if(m_iPlayerStateFlag & PFG_JUMP){
		// �ЂƂO�̃t���[���ɏd�͂𑫂��Ď��̃t���[���̍��W���v�Z
		SetVY(m_fY - m_fY_prev + GRAVITY);
		if (m_fVY > FALL_SPEED_MAX)
			m_fVY = FALL_SPEED_MAX;	// �������x����
		m_fY_prev = m_fY;
	}
	

	// �W�����v�U�����ɍ��E�ړ���
	if ( (m_iPlayerStateFlag & (PFG_JUMP | PFG_ATTACK)) == (PFG_JUMP | PFG_ATTACK) ) {
		if (m_bFaceToRight && GetAsyncKeyState(VK_RIGHT)) {		// �E����(��)�Ɉړ�
			m_fVX = 1.f * WALKSPEED;
		}
		else if (!(m_bFaceToRight) && GetAsyncKeyState(VK_LEFT)) {		// ������(��)�Ɉړ�
			m_fVX = -1.f * WALKSPEED;
		}
	}

	// �X���C�f�B���O�U��
	if ((m_iPlayerStateFlag & PFG_SLIDE) && !(m_iPlayerStateFlag & PFG_SLIDE_CONTINUE)) {
		if (GetAsyncKeyState(KEY_ATTACK)) {
			m_iPlayerStateFlag |= PFG_SLIDEATTACK;
			m_iPlayerStateFlag &= ~PFG_SLIDE;
			m_iSlidingFrame = FPS + 1;	// �X���C�f�B���O�U���ҋ@�p1�t���[��
			m_iSlidingAttackFrame = 0;
		}
	}
	m_iPlayerStateFlag &= ~PFG_SLIDE_CONTINUE;
	//-----------------------------------


	// SHOT------------------------------
	if (GetAsyncKeyState(0x58)) {	// X
		if (!m_bShot) {
			IGameObject *pObj;
			if (m_bFaceToRight)
				pObj = new Shot(m_pParent, m_fX, m_fY, SHOT_DIRECTION);
			else
				pObj = new Shot(m_pParent, m_fX, m_fY, -SHOT_DIRECTION);
			m_pParent->addShots(pObj);
			m_bShot = true;
		}
	}
	else {
		m_bShot = false;
	}

	//	X�����̃X�e�[�W�[����
	if (m_fX <= fMAP_CHIP_WIDTH)
	{
		m_fX = PLAYER_MOVE_LIMIT;
		m_fVX = 0;
	}
	//	Y�����̃X�e�[�W�[����
	if (m_fY <= fMAP_CHIP_WIDTH)
	{
		m_fY = PLAYER_MOVE_LIMIT;
		m_fVY = 0;
	}

	return true;
}


/**
* @brief �`�惁�\�b�h
*/
void Player::draw(ID2D1RenderTarget *pRenderTarget) {

	D2D1_RECT_F rc,		// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
				src;	// �e�N�X�`���̒��ł̍��W���w�肵�A�e�N�X�`�������肷��ϐ�
	float tx, ty;
	int Xindex = 0, Yindex = 0;

	// �`��̈�̐ݒ�
	rc.left = m_fDrawX - PLAYER_TEXTURE_WIDTH;	// �`��̈�̍�����w��
	rc.top = m_fDrawY - PLAYER_TEXTURE_H_TWICE;	//			�V
	rc.right = rc.left + PLAYER_TEXTURE_W_TWICE;	// �`��̈�̉E�����w��
	rc.bottom = m_fDrawY;							//			�V

	tx = 0; ty = 0;

	// Shadow�`��
	D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(SHADOW_POS_X_TO_P, SHADOW_POS_Y_TO_P);
	D2D1::Matrix3x2F skew = D2D1::Matrix3x2F::Skew(SHADOW_SKEW_X_TO_P, SHADOW_SKEW_Y_TO_P, D2D1::Point2F(rc.right, rc.bottom));
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(SHADOW_SCALE_X_TO_P, SHADOW_SCALE_Y_TO_P, D2D1::Point2F(rc.right, rc.bottom));
	D2D1::Matrix3x2F calc = trans * scale;

	//	shadow���̕`��-----------------
	if (m_iShadowStateFlag == SFG_SWITCH) {		// �ʏ� => �V���h�E�؂�ւ���
		src.top = ty;	// �e�N�X�`�����W�̍�����w��p�̏����
		src.bottom = ty + PLAYER_TEXTURE_HEIGHT;	// �e�N�X�`�����W�̉E�����w��p�̏����
		// �e�ړ�����
		trans = D2D1::Matrix3x2F::Translation((SHADOW_POS_X_TO_P + m_iShadowSwitchFrame * SHADOW_CHANGE_SPEED), SHADOW_POS_Y_TO_P);
		calc = trans * scale;

		if (m_bFaceToRight) {
			src.left = tx;		// �e�N�X�`�����W�̍�����w��
			src.right = tx + PLAYER_TEXTURE_WIDTH;	// �e�N�X�`�����W�̉E�����w��

			pRenderTarget->DrawBitmap(m_pTexture, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			return;
		}
		else {		// ������
			src.left = PLAYER_LASTTEXTURE_POS - tx;		// �e�N�X�`�����W�̍�����w��
			src.right = src.left + PLAYER_TEXTURE_WIDTH;	// �e�N�X�`�����W�̉E�����w��

			pRenderTarget->DrawBitmap(m_pTexture_Left, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture_Left, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			return;
		}
	}
	else if (m_iShadowStateFlag == (SFG_ON | SFG_SWITCH)) {		// �V���h�E => �ʏ�؂�ւ���
		src.top = ty;	// �e�N�X�`�����W�̍�����w��p�̏����
		src.bottom = ty + PLAYER_TEXTURE_HEIGHT;	// �e�N�X�`�����W�̉E�����w��p�̏����
		// �e�ړ�����
		trans = D2D1::Matrix3x2F::Translation( -(m_iShadowSwitchFrame * SHADOW_CHANGE_SPEED), SHADOW_POS_Y_TO_P);
		calc = trans * scale;

		if (m_bFaceToRight) {
			src.left = tx;	// �e�N�X�`�����W�̍�����w��
			src.right = tx + PLAYER_TEXTURE_WIDTH;	// �e�N�X�`�����W�̉E�����w��

			pRenderTarget->DrawBitmap(m_pTexture, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			return;
		}
		else {		// ������
			src.left = PLAYER_LASTTEXTURE_POS - tx;		// �e�N�X�`�����W�̍�����w��
			src.right = src.left + PLAYER_TEXTURE_WIDTH;	// �e�N�X�`�����W�̉E�����w��

			pRenderTarget->DrawBitmap(m_pTexture_Left, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture_Left, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			return;
		}
	}
	// ---------------------------------------------------- �ȉ��@@�C���ӏ� ------------------------------------------------------- //
	if (!m_bIs_AttackCancel_OK) {

		switch (m_iPlayerStateFlag) {
		case PFG_IDLE:
		case PFG_JUMP:		// ��ł� FPS * 2 ���Ă��邽��
			if (m_iIdleFrame > FPS * 2 - AllTexture) {
				Xindex = (m_iIdleFrame - (FPS * 2 - AllTexture)) >> 2;	// ( >> 2 == * 4 )	�A�j���[�V�������[�v
				Yindex = PlayerTexture::Idle_Y;
			}
			break;

		case PFG_DAMAGED:
		case PFG_DAMAGED | PFG_JUMP:
			Xindex = PlayerTexture::Damaged_X;
			Yindex = PlayerTexture::Damaged_Y;
			break;

		case PFG_WALK:
		case PFG_WALK | PFG_JUMP:
			Xindex = UnusedTexture + ((m_iAnimFrame % WalkTextureTimesFour) >> 2);	// ( >> 2 == * 4 )	�A�j���[�V�������[�v
			Yindex = PlayerTexture::Walk_Y;
			break;

		case PFG_SLIDE:
		case PFG_SLIDE | PFG_JUMP:
			if (m_iSlidingFrame < SL_DURATION_IN_FRAMES * SL_DUALATION_SECONDS) {	// 22�t���[��(�X���C�f�B���O�ɂ�����t���[��)��艺
				Xindex = PlayerTexture::Slide_X;
				Yindex = PlayerTexture::Slide_Y;
			}
			else {		// �N���オ�蓮��
				Xindex = PlayerTexture::GetUp_X + ((m_iSlidingFrame - (SL_DURATION_IN_FRAMES * SL_DUALATION_SECONDS)) >> 1);	// 22�t���[�� * 2
				Yindex = PlayerTexture::GetUp_Y + Xindex / RowPerElement;	// �����̓�s�Ŋ��炩�ȃA�j���[�V������
				Xindex = Xindex % RowPerElement;							//	
			}
			break;

		case PFG_ATTACK:
		case PFG_ATTACK | PFG_JUMP:
			if (m_iAttackCount == AttackCount::First) {	//	��i��	-----------------------------------------

				if (m_iAttackFrame < ATTACK_THRESHOLD) {	// �\��
					Xindex = m_iAttackFrame >> 1;
				}
				else if (m_iAttackFrame < ATTACK_AFTER_THRESHOLD) {		// �a��
					Xindex = FirstAttackCollisionStart + ((m_iAttackFrame - ATTACK_THRESHOLD) >> 2);	// ( >> 2 == * 4 )	�A�j���[�V�������[�v
				}
				else if (m_iAttackFrame < ATTACK_END_THRESHOLD) {		// �a����ҋ@
					Xindex = FirstAttackCollisionFinish + ((m_iAttackFrame - ATTACK_AFTER_THRESHOLD) >> 1);
				}
				Yindex = PlayerTexture::FirstAttack_Y;
			}
			else if (m_iAttackCount == AttackCount::Second) {	//	��i��	-------------------------------------------

				if (m_iAttackFrame < ATTACK_THRESHOLD) {		// �\��
					Xindex = (m_iAttackFrame >> 1) + 5;		// 5 Texture7�s�ڂ̈�i�ڂ̍U�����[�V����
				}
				else if (m_iAttackFrame < ATTACK_AFTER_THRESHOLD) {		// �a��
					Xindex = ((m_iAttackFrame - ATTACK_THRESHOLD) >> 2) + 7;	// 7 == 5(����) + 2(Texture7�s�ڂ̓�i�ڂ̍\�����[�V����)	 ( >> 2 == * 4 )	�A�j���[�V�������[�v
				}
				else if (m_iAttackFrame < ATTACK_END_THRESHOLD) {			// �a����ҋ@
					Xindex = ((m_iAttackFrame - ATTACK_AFTER_THRESHOLD) >> 1) + 9;	 // 9 == 5(����) + 2(����) + 2(Texture8�s�ڂ̓�i�ڂ̎a�����[�V����)
				}
				Yindex = PlayerTexture::FirstAttack_Y + Xindex / RowPerElement;	// �����̓�s�Ŋ��炩�ȃA�j���[�V������
				Xindex = Xindex % RowPerElement;								//
			}
			else {		//	�O�i��	-----------------------------------------------------------------------------------------------
				if (m_iAttackFrame < ATTACK_THRESHOLD) {		// �\��
					Xindex = (m_iAttackFrame >> 1) + 4;		// 4 Texture��8�s�ڂɂ����i�ڂ̍U�����[�V����
				}
				else if (m_iAttackFrame < ATTACK_AFTER_THRESHOLD) {		// �a��
					Xindex = ((m_iAttackFrame - ATTACK_THRESHOLD) >> 2) + 6;	// 6 == 4(����) + 2(Texture��8�s�ڂɂ���O�i�ڂ̍\�����[�V����)		( >> 2 == * 4 )	�A�j���[�V�������[�v
				}
				else if (m_iAttackFrame < ATTACK_END_THRESHOLD) {		// �a����ҋ@
					Xindex = ((m_iAttackFrame - ATTACK_AFTER_THRESHOLD) >> 1) + 8;	// 8 == 4(����) + 2(����) + 2(Texture��8,9�s�ڂɂ���O�i�ڂ̎a�����[�V����)
				}
				Yindex = PlayerTexture::SecondAndThirdAttack_Y + Xindex / RowPerElement;	// �����̓�s�Ŋ��炩�ȃA�j���[�V������
				Xindex = Xindex % RowPerElement;											//
			}
			break;

		case PFG_SLIDEATTACK:
		case PFG_SLIDEATTACK | PFG_JUMP:
			if (m_iSlidingAttackFrame < SL_ATTACK_THRESHOLD) {
				Xindex = PlayerTexture::SlideAttackCollisionBefore;
				Yindex = PlayerTexture::SlideAttack_Y;
			}
			else if (m_iSlidingAttackFrame < SL_ATTACK_AFTER_THRESHOLD) {
				Xindex = PlayerTexture::SlideAttackCollisionStart;
				Yindex = PlayerTexture::SlideAttack_Y;
			}
			else if (m_iSlidingAttackFrame < SL_ATTACK_END_THRESHOLD) {
				Xindex = ((m_iSlidingAttackFrame - PlayerTexture::SlideAttackCollisionStart) >> 1);		// ���s���1,2�̃e�N�X�`���w��
				Yindex = PlayerTexture::SlideAttack_Y2;
			}
			break;

		default:
			break;
		}
	}
	else {
		Xindex = 0;
		Yindex = 0;
	}

	// ---------------------------------------------------- �ȏ�@@�C���ӏ� ------------------------------------------------------- //

	// Texture�ɑ��
	tx = m_fTextureCoord[Xindex];
	ty = m_fTextureCoord[Yindex];

	src.top = ty;	// �e�N�X�`�����W�̍�����w��p�̏����
	src.bottom = ty + PLAYER_TEXTURE_HEIGHT;	// �e�N�X�`�����W�̉E�����w��p�̏����

	
	//	�v���C���[�Ɖe�̕`��--------------------------
	if (!m_iShadowStateFlag) {		// �ʏ펞

		if (m_bFaceToRight) {
			src.left = tx;		// �e�N�X�`�����W�̍�����w��
			src.right = tx + PLAYER_TEXTURE_WIDTH;	// �e�N�X�`�����W�̉E�����w��
			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			pRenderTarget->DrawBitmap(m_pTexture, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

		}
		else {	// ������
			src.left = PLAYER_LASTTEXTURE_POS - tx;		// �e�N�X�`�����W�̍�����w��
			src.right = src.left + PLAYER_TEXTURE_WIDTH;	// �e�N�X�`�����W�̉E�����w��
			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture_Left, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			pRenderTarget->DrawBitmap(m_pTexture_Left, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

		}

	}
	else {		// �V���h�E���[�h��
		trans = D2D1::Matrix3x2F::Translation(SHADOW_CHANGE_DIRECTION, 0.f);
		calc = trans * scale;

		if (m_bFaceToRight) {
			src.left = tx;	// �e�N�X�`�����W�̍�����w��
			src.right = tx + PLAYER_TEXTURE_WIDTH;	// �e�N�X�`�����W�̉E�����w��
			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		}
		else {		// ������
			src.left = PLAYER_LASTTEXTURE_POS - tx;	// �e�N�X�`�����W�̍�����w��
			src.right = src.left + PLAYER_TEXTURE_WIDTH;	// �e�N�X�`�����W�̉E�����w��
			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture_Left, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		}

	}



#ifdef _DEBUG
	//	�f�o�b�O�������蔻��\��
	float l, r, t, b;
	if (m_fX > QUARTER_MAP_WIDTH_PIXELS && m_fX < m_fRBoader) {
		l = m_fPl - (m_fX - QUARTER_MAP_WIDTH_PIXELS);
		r = l + m_fPw;
		t = m_fPt;
		b = t + m_fPh;
	}
	else if(m_fX >= m_fRBoader){
		l = m_fPl - m_fRBoader + QUARTER_MAP_WIDTH_PIXELS;
		r = l + m_fPw;
		t = m_fPt;
		b = t + m_fPh;
	}
	else {
		l = m_fPl;
		r = l + m_fPw;
		t = m_fPt;
		b = t + m_fPh;
	}
	
	// �`��̈�w��
	rc.left = l;  rc.top = t;		// �`��̈�̍�����w��
	rc.right = r; rc.bottom = b;		// �`��̈�̉E�����w��
	pRenderTarget->DrawRectangle(rc, m_pWhiteBrush);
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
bool Player::collide(float x, float y, float w, float h) {

	if (m_fPl > x + w)
		return false;
	if (m_fPl + m_fPw < x)
		return false;
	if (m_fPt < y + h)
		return false;
	if (m_fPt + m_fPh > y)
		return false;

	return true;
}


/**
* @brief ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h
* @param[in] *pObj ���Q�[���I�u�W�F�N�g
* @return true:������ / false:�O��
*/
bool Player::collide(IGameObject *pObj) {
	if (m_bDamage || m_iShadowStateFlag & SFG_SWITCH) {
		return false;
	}

	return pObj->collide(m_fPl, m_fPt, m_fPw, m_fPh);
}


/**
* @brief �}�b�v�Ƃ̓����蔻�胁�\�b�h
* @note	�����œ����蔻��ɏ]����(x, y)���W�̍X�V���s��
*		X������Y�����ŕ������Ĕ���B
*		�|�C���g�@: ���ꂼ������x�̐����ō��E�A�㉺�ɔ���𕪂���
*		�|�C���g�A: ���肷������̂ݍ��W���ړ������A���肵�Ȃ������͈ړ��O�̍��W���g���Ĕ�����s��
*		�|�C���g�B: X�����̔���͉����x0�̎��͍s��Ȃ��B�����x0�̎����܂߂Ă��܂��ƃW�����v���◎�����ɕǂɈ���������B
*/
bool Player::collideWithMap() {
	float l = m_fPl, t = m_fPt, w = m_fPw, h = m_fPh, h_half = h / 2.f;


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
	




	//	���n�ł̔���------------------------
	//	�⓹���\������}�b�v�`�b�v�Ƃ̔���͎��Ȃ��悤�ɂ��Ă���
	if (!m_bIs_OnSlope) {
		if (m_fVY >= 0) {
			int code1 = m_pBG->tile_code(l + 4, t + h + m_fVY);	// +4 / -4 ���邱�ƂŁA�u���b�N�אڎ��ɃW�����v����Ə�ɏ���Ă��܂����ۂ�h����
			int code2 = m_pBG->tile_code(l + w - 4, t + h + m_fVY);
			// ���Ɣ���
			if ( (code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER)) { 
				if (m_iPlayerStateFlag & PFG_JUMP)
					m_iPlayerStateFlag ^= PFG_JUMP;
				int row = (int)(t + h + m_fVY) >> MAP_CHIP_SIZE_BITS;

				//---�G���[����
				m_fY = (float)(row << MAP_CHIP_SIZE_BITS);

				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {	//	�󒆎�
				m_iPlayerStateFlag |= PFG_JUMP;
				m_fY += m_fVY;
			}
		}
		else {
			// �V��Ɣ���
			if (0 != (m_pBG->tile_code(l + 4, t + m_fVY) || m_pBG->tile_code(l + w - 4, t + m_fVY))) {	// +4 / -4
				int row = (int)(t + m_fVY) >> MAP_CHIP_SIZE_BITS;

				//---�G���[����
				m_fY = (float)((row << MAP_CHIP_SIZE_BITS) + fMAP_CHIP_WIDTH + h);

				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {
				m_iPlayerStateFlag |= PFG_JUMP;
				m_fY += m_fVY;
			}
		}

		if (m_fVX > 0) {
			int code1 = m_pBG->tile_code(l + w + m_fVX, t);
			int code2 = m_pBG->tile_code(l + w + m_fVX, t + h - 4);	// -4 ���邱�ƂŒn�ʂƔ��肪�o�Ă��܂����Ƃ�h����
			int code3 = m_pBG->tile_code(l + w + m_fVX, t + h_half);
			// �E���̔���
			if ( (code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER) || (code3 > 0 && code3 < SLOPE_START_TEXTURE_NUBER) ) {
				int col = (int)(l + w + m_fVX) >> MAP_CHIP_SIZE_BITS;
				m_fX = (float)((col << MAP_CHIP_SIZE_BITS) - ((int)w >> 1));
			}
			else {
				m_fX += m_fVX;
			}
		}
		else if (m_fVX < 0) {
			int code1 = m_pBG->tile_code(l + m_fVX, t);
			int code2 = m_pBG->tile_code(l + m_fVX, t + h - 4); // -4 ���邱�ƂŒn�ʂƔ��肪�o�Ă��܂����Ƃ�h����
			int code3 = m_pBG->tile_code(l + m_fVX, t + h_half);
			// �����̔���
			if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER) || (code3 > 0 && code3 < SLOPE_START_TEXTURE_NUBER)) {
				int col = (int)(l + m_fVX) >> MAP_CHIP_SIZE_BITS;
				m_fX = (float)((col << MAP_CHIP_SIZE_BITS) + ((int)w >> 1) + iMAP_CHIP_WIDTH);
			}
			else {
				m_fX += m_fVX;
			}
		}
	}
	//	�⓹�ł̔���--------------------------------------------
	else {
		
		if (m_fVY >= 0) {
			float stepY, ratio;
			ratio = (m_fX - step.x) / step.vx;
			stepY = step.y + step.vy * ratio;
			if (m_fY + m_fVY + 5.f >= stepY) {	//	+5.f ���Ă����Ɖ���̎��ɃK�^�K�^���˂Ȃ��Ȃ�
				m_iPlayerStateFlag &= ~PFG_JUMP;
				m_fY = stepY;
				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {
				m_iPlayerStateFlag |= PFG_JUMP;
				m_fY += m_fVY;
			}
		}
		else {
			// �V��Ɣ���
			if (0 != (m_pBG->tile_code(l + 4, t + m_fVY) || m_pBG->tile_code(l + w - 4, t + m_fVY))) {	// +4 / -4
				int row = (int)(t + m_fVY) >> MAP_CHIP_SIZE_BITS;
				m_fY = (float)(row << MAP_CHIP_SIZE_BITS) + fMAP_CHIP_WIDTH + h;
				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {
				m_iPlayerStateFlag |= PFG_JUMP;
				m_fY += m_fVY;
			}
		}

		if (!(m_iPlayerStateFlag & PFG_JUMP)) {
			m_fX += m_fVX;
		}
		else {
			if (m_fVX > 0) {
				int code1 = m_pBG->tile_code(l + w + m_fVX, t);
				int code2 = m_pBG->tile_code(l + w + m_fVX, t + h - 4);	// -4 ���邱�ƂŒn�ʂƔ��肪�o�Ă��܂����Ƃ�h����
				int code3 = m_pBG->tile_code(l + w + m_fVX, t + h_half);
				// �E���̔���
				if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER) || (code3 > 0 && code3 < SLOPE_START_TEXTURE_NUBER)) {
					int col = (int)(l + w + m_fVX) >> MAP_CHIP_SIZE_BITS;
					m_fX = (float)((col << MAP_CHIP_SIZE_BITS) - ((int)w >> 1));
				}
				else {
					m_fX += m_fVX;
				}
			}
			else if (m_fVX < 0) {
				int code1 = m_pBG->tile_code(l + m_fVX, t);
				int code2 = m_pBG->tile_code(l + m_fVX, t + h - 4); // -4 ���邱�ƂŒn�ʂƔ��肪�o�Ă��܂����Ƃ�h����
				int code3 = m_pBG->tile_code(l + m_fVX, t + h_half);
				// �����̔���
				if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER) || (code3 > 0 && code3 < SLOPE_START_TEXTURE_NUBER)) {
					int col = (int)(l + m_fVX) >> MAP_CHIP_SIZE_BITS;
					m_fX = (float)((col << MAP_CHIP_SIZE_BITS) + ((int)w >> 1) + iMAP_CHIP_WIDTH);
				}
				else {
					m_fX += m_fVX;
				}
			}
		}

	}

	//	�����蔻��X�V-------------------
	if (m_iPlayerStateFlag & PFG_SLIDE) {
		m_fPl = PHBS_L; m_fPt = PHBS_T; m_fPw = PHBS_W; m_fPh = PHBS_H;
	}
	else {
		m_fPl = PHB_L; m_fPt = PHB_T; m_fPw = PHB_W; m_fPh = PHB_H;
	}
	//--------------------------------------

	return true;
}


/**
* @brief	�v���C���[��x���W��Ԃ�
* @return	�v���C���[��x���W
*/
float Player::getX() {
	return m_fX;
}


// �v���C���[��x���W���Z�b�g����
void Player::setPos(float x,float y) {
	m_fX = x;
	m_fY = y;
	if (m_iPlayerStateFlag & PFG_SLIDE) {
		m_fPl = PHBS_L; m_fPt = PHBS_T; m_fPw = PHBS_W; m_fPh = PHBS_H;
	}
	else {
		m_fPl = PHB_L; m_fPt = PHB_T; m_fPw = PHB_W; m_fPh = PHB_H;
	}
}


// �v���C���[��Y���W���擾���ĕԂ�
float Player::getY() {
	return m_fY;
}


// �v���C���[��HP���擾���ĕԂ�
float Player::getHP() {
	return m_fHP;
}


// �v���C���[�̃}�i���擾���ĕԂ�
float Player::getMana() {
	return m_fMana;
}


// �v���C���[�̃}�i���Z�b�g���郁�\�b�h
void Player::setMana(float amount) {
	m_fMana += amount;
	if (m_fMana > MANA_LIMIT) {
		m_fMana = MANA_LIMIT;
	}
}


/**
* @brief �_���[�W���\�b�h
* @note	 �v���C���[�͔�_���[�W�� ������ & �m�b�N�o�b�N
*/
int Player::damage(float amount) {
	m_fHP -= amount;
	m_iPlayerStateFlag &= PFG_JUMP;
	m_iPlayerStateFlag |= PFG_DAMAGED;
	m_fOpacity = PLAYER_DAMAGE_OPACITY;		// ������
	m_iAttackCount = 0;
	m_iAttackFrame = PLAYER_ATTTACK_TIME;
	m_bIs_AttackCancel_OK = false;

	if (m_bFaceToRight) {	// �E����
		m_fVX = -PLAYER_KNOCK_BACK;		// �m�b�N�o�b�N
	}
	else {	//	������
		m_fVX = PLAYER_KNOCK_BACK;		// �m�b�N�o�b�N
	}

	m_bDamage = true;

	return 0;
}


// �}�b�v�̉������Z�b�g����
void Player::setMapWidth(float mw) {
	m_fMapWidth = mw;
	m_fRBoader = mw - QUARTER_MAP_WIDTH_PIXELS;
}


// �}�b�v�̏c�����Z�b�g����
void Player::setMapHight(float mh) {
	m_fMapHight = mh;
}


// �v���C���[�̉�ʏ��X���W���Z�b�g
void Player::setDrawX(float drawX) {
	m_fDrawX = drawX;
}


// �v���C���[�̉�ʏ��Y���W���Z�b�g
void Player::setDrawY(float drawY) {
	m_fDrawY = drawY;
}


/**
 * @brief	BG��ݒ肵��Player�̊�����������
 * @note	Player��BG����ݒ肷�� => Player���`�擙�ɉe����^���邽�߂ɕK�v
 */
void Player::resetBG(BG *pBG) {
	m_pBG = pBG;
}


// X�����̃v���C���[�����x�����Z�b�g
void Player::clearVX() {
	m_fVX = 0;
}


// Y�����̃v���C���[�����x�����Z�b�g
void Player::clearVY() {
	m_fVY = 0;
}


/**
* @brief �v���C���[�̕`��X���W���v�Z���郁�\�b�h
* @return m_fDrawX
* @note collideWithMap()�̌�ɂ��̃��\�b�h���ĂԂ悤�ɂ��邱��
*/
float Player::calcPlayerDrawX() {

	// �v���C���[����ʒ��S�ɕ`��
	if (m_fX > QUARTER_MAP_WIDTH_PIXELS && m_fX < m_fRBoader) {
		m_fDrawX = QUARTER_MAP_WIDTH_PIXELS;
	}
	else if (m_fX >= m_fRBoader) {
		m_fDrawX = m_fX - (m_fRBoader - QUARTER_MAP_WIDTH_PIXELS);
	}
	else {
		m_fDrawX = m_fX;
	}

	return m_fDrawX;
}


/**
* @brief �v���C���[�̕`��Y���W���v�Z���郁�\�b�h
* @return m_fDrawY
* @note collideWithMap()�̌�ɂ��̃��\�b�h���ĂԂ悤�ɂ��邱��
*/
float Player::calcPlayerDrawY() 
{

	m_fDrawY = m_fY;

	return m_fDrawY;
}


// Texture��Index��t�^�E�w��
// @note: X:0�`6,Y:7�`16
float  Player::m_fTextureCoord[] = {
	// X���W
	0.f, 50.f, 100.f, 150.f, 200.f, 250.f, 300.f,

	// Y���W
	0.f, 37.f, 111.f, 148.f, 222.f, 259.f, 296.f, 481.f, 518.f, 444.f,
};


/*
* @brief	Y�����̃v���C���[�����x�𐧌����郁�\�b�h
* @note		���ꂪ�Ȃ���Y�����̃}�b�v�ړ����Ƀv���C���[��������
*/
void Player::SetVY(float vy)
{
	if (vy <= PLAYER_VY_LIMIT)	// -15���ő�l
	{
		return;	
	}
	else
	{
		m_fVY = vy;
	}
}
