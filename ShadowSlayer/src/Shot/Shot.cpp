// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "..\stdafx\stdafx.h"
// Direct2D�̃w�b�_�t�@�C�����C���N���[�h
#include <d2d1.h>
// �e�N���X�̐錾
#include "Shot.h"
// �Q�[�����̃X�e�[�W�֘A�N���X�Ɨ񋓌^�̒�`
#include "..\Stage\Stage.h"
// �e�N�X�`���ƃr�b�g�}�b�v�̓ǂݍ��݂ƊǗ��Ɋւ���N���X�ƃC���^�[�t�F�[�X���`
#include "..\TextureLoader\TextureLoader.h"
// �Q�[���̔w�i���Ǘ�����N���X�Ɗ֘A�f�[�^���`
#include "..\BG\BG.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "..\Constants.h"
// enum�ϐ��錾�p�t�H���_
#include "..\Enums.h"


ID2D1Bitmap *Shot::m_pTexture = NULL;
ID2D1Bitmap *Shot::m_pTexture_Left = NULL;
BG	*Shot::m_pBG = NULL;
#ifdef _DEBUG
	ID2D1SolidColorBrush *Shot::m_pWhite = NULL;
#endif


// �R���X�g���N�^
Shot::Shot(Stage *pParent, float x, float y, float direction)
{
	m_pParent = pParent;
	m_fX = x + SHOTOFFSETX * direction;
	m_fY = y - SHOTOFFSETY;
	m_fVX = BULLETSPEED * direction;
	m_fVY = 0;
	m_fDirection = direction;
	m_iLifeTimer = -1;
	m_iDeathTimer = -1;
	m_bDamage = false;

}


// �f�X�g���N�^
Shot::~Shot()
{
}


// �A�j���[�V����
bool Shot::move() {
	if (!m_bDamage) {	// �������Ă��Ȃ��Ƃ�
		m_iLifeTimer++;

		m_fX += m_fVX;

		if (m_iLifeTimer > SHOT_LIFE_TIME) {
			m_bDamage = true;
		}
	}
	else {	// ����������
		m_iDeathTimer++;
		if (m_iDeathTimer > SHOT_DESTROY_TIME) {
			return false;
		}
	}
	

	return true;
}


// �`�惁�\�b�h
void Shot::draw(ID2D1RenderTarget *pRenderTarget) {
	
	float tx, ty;
	D2D1_RECT_F rc,		// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
				src;	// �e�N�X�`���̒��ł̍��W���w�肵�A�e�N�X�`�������肷��ϐ�

	// �`��̈�̐ݒ�@�@�v���C���[���N�_�ɍs��		
	rc.left = (m_pParent->m_fPlayerDrawX) + (m_fX - m_pParent->m_fPlayerX) - SHOT_RECTANGLE_HALF_SIZE;	// �`��̈�̍�����w��
	rc.top = (m_pParent->m_fPlayerDrawY) + (m_fY - m_pParent->m_fPlayerY) - SHOT_RECTANGLE_HALF_SIZE;	//�@		�V
	rc.right = rc.left + SHOT_RECTANGLE_SIZE;		// �`��̈�̉E�����w��
	rc.bottom = rc.top + SHOT_RECTANGLE_SIZE;		//			�V	


	if (!m_bDamage) {	// �������Ă��Ȃ�

		if (m_iLifeTimer < HitStart) {	// �A�j���[�V�����ω������ł���
			tx = m_iLifeTimer >> 1;
		}
		else {	// ���ł�����
			tx = ProjectileFlying;
		}


		tx *= SHOT_TEXTURE_WIDTH;
		ty = 0.f;


		if (m_fVX > 0) {	// �E����
			src.left = tx;		// �e�N�X�`�����W�̍�����w��
			src.top = ty;		//				�V
			src.right = src.left + SHOT_TEXTURE_WIDTH;		// �e�N�X�`�����W�̉E�����w��
			src.bottom = src.top + SHOT_TEXTURE_HEIGHT;		//				�V
			pRenderTarget->DrawBitmap(m_pTexture, rc, SHOT_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
		}
		else {		// ������
			src.left = SHOT_LASTTEXTURE_POS - tx;		// �e�N�X�`�����W�̍�����w��
			src.top = ty;								//				�V
			src.right = src.left + SHOT_TEXTURE_WIDTH;		// �e�N�X�`�����W�̉E�����w��
			src.bottom = src.top + SHOT_TEXTURE_HEIGHT;		//				�V
			pRenderTarget->DrawBitmap(m_pTexture_Left, rc, SHOT_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
		}

#ifdef _DEBUG
		// �f�o�b�O�������蔻��\��
		rc.left += SHOT_COLLISION_OFFSET_W;			// �`��̈�̍�����w��
		rc.top = m_fY - SHOT_COLLISION_OFFSET_T;	//			�V
		rc.right -= SHOT_COLLISION_OFFSET_W;			// �`��̈�̉E�����w��
		rc.bottom = m_fY + SHOT_COLLISION_OFFSET_B;		//			�V 
		pRenderTarget->DrawRectangle(rc, m_pWhite);
#endif
	}
	else {		// ��������
		tx = StartDestroy + (m_iDeathTimer >> 1);
		ty = SHOT_TEXTURE_HEIGHT * ((int)tx / SRowPerElement);
		tx = SHOT_TEXTURE_WIDTH * ((int)tx % SRowPerElement);
		
		if (m_fVX > 0) {	// �E����
			src.left = tx;		// �e�N�X�`�����W�̍�����w��
			src.top = ty;		//				�V
			src.right = src.left + SHOT_TEXTURE_WIDTH;		// �e�N�X�`�����W�̉E�����w��
			src.bottom = src.top + SHOT_TEXTURE_HEIGHT;		//				�V
			pRenderTarget->DrawBitmap(m_pTexture, rc, SHOT_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
		}
		else {		// ������
			src.left = SHOT_LASTTEXTURE_POS - tx;	// �e�N�X�`�����W�̍�����w��
			src.top = ty;							//				�V
			src.right = src.left + SHOT_TEXTURE_WIDTH;		// �e�N�X�`�����W�̉E�����w��
			src.bottom = src.top + SHOT_TEXTURE_HEIGHT;		//				�V
			pRenderTarget->DrawBitmap(m_pTexture_Left, rc, SHOT_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
		}
	}

}


// ��`�Ƃ̓����蔻�胁�\�b�h
bool Shot::collide(float x, float y, float w, float h) {
	return true;
}


// ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h
bool Shot::collide(IGameObject* pObj) {
	return true;
}


// �}�b�v�Ƃ̓����蔻�胁�\�b�h
bool Shot::collideWithMap() {
	if (m_fVX > 0) {		// �E����	
		if (0 != m_pBG->tile_code(m_fX + SHOT_COLLISION_HEIGHT, m_fY)) {
			damage(SHOT_DAMAGE);
		}
	}
	else {		// ������
		if (0 != m_pBG->tile_code(m_fX - SHOT_COLLISION_HEIGHT, m_fY)) {
			damage(SHOT_DAMAGE);
		}
	}

	return true;
}


// �_���[�W���\�b�h
int Shot::damage(float amount){
	m_bDamage = true;
	return 1;
}


// ���L���f�B�A�t�@�C���̓ǂݍ���
void Shot::Restore(ID2D1RenderTarget *pRT, BG *pBG) {
	m_pBG = NULL;
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexture_Left);
	m_pBG = pBG;
	TextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\shot_32x32.png"), &m_pTexture);
	TextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\shot_left_32x32.png"), &m_pTexture_Left);

#ifdef _DEBUG
	SAFE_RELEASE(m_pWhite);
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhite);
#endif
}


// ���L���f�B�A�t�@�C���̏���
void Shot::Finalize() {
	m_pBG = NULL;
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexture_Left);

#ifdef _DEBUG
	SAFE_RELEASE(m_pWhite);
#endif
}


// BG��ݒ肵��Shot�̊�����������
void Shot::resetBG(BG *pBG) {
	m_pBG = pBG;
}
