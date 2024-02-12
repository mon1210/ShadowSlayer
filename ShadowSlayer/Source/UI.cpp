// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "stdafx.h"
// Direct2D�̃w�b�_�t�@�C�����C���N���[�h
#include <d2d1.h>
// �v���C���[�L�����N�^�[�Ƃ��̊֘A�����`�����N���X�ƒ萔/�񋓌^�̒�`
#include "Player.h"
// �v���C���[��UI�v�f���Ǘ�����
#include "UI.h"
// �e�N�X�`���ƃr�b�g�}�b�v�̓ǂݍ��݂ƊǗ��Ɋւ���N���X�ƃC���^�[�t�F�[�X���`
#include "TextureLoader.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "Constants.h"


//  �}�N����`
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }


/**
* @brief UI�̃R���X�g���N�^
*/
UI::UI(Player *pPlayer, ID2D1RenderTarget *pRenderTarget)
{
	m_pPlayer = pPlayer;
	m_pBarFrameTexture = NULL;
	m_pHPBarTexture = NULL;
	m_pManaBarTexture = NULL;

	// �摜���������ɓǂݍ���
	TextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\BAR_Frame.png"), &m_pBarFrameTexture);
	TextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\HP_BAR.png"), &m_pHPBarTexture);
	TextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\MANA_BAR.png"), &m_pManaBarTexture);

	m_fHPRatio = 0.f;
	m_fManaRatio = 0.f;
}


// �f�X�g���N�^
UI::~UI()
{
	SAFE_RELEASE(m_pBarFrameTexture);
	SAFE_RELEASE(m_pHPBarTexture);
	SAFE_RELEASE(m_pManaBarTexture);
}


/**
* @brief UI�̓���
* @note  ���̃I�u�W�F�N�g�ƈႢ�A���łȂ�
*/
void UI::move() {
	//	HP�o�[
	if (m_fHPRatio > m_pPlayer->getHP()) {
		m_fHPRatio--;
	}
	else if (m_fHPRatio < m_pPlayer->getHP()) {
		m_fHPRatio++;
	}

	//	MP�o�[
	if (m_fManaRatio > m_pPlayer->getMana()) {
		m_fManaRatio--;
	}
	else if (m_fManaRatio < m_pPlayer->getMana()) {
		m_fManaRatio++;
	}
}


/**
* @brief �`�惁�\�b�h
*/
void UI::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc,		// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
				src;	// �e�N�X�`���̒��ł̍��W���w�肵�A�e�N�X�`�������肷��ϐ�
	float ratio;

	//	HP�o�[�̕`�� ==============================================================================================================================
	ratio = m_fHPRatio / INITIAL_HP;

	// �`��̈�̐ݒ�
	rc.left = HP_BAR_RC_LEFT;		// �`��̈�̍�����w��
	rc.top = HP_BAR_RC_TOP;		//			�V
	rc.right = rc.left + HP_TEXTURE_WIDTH_TWICE * ratio;		// �`��̈�̉E�����w��
	rc.bottom = rc.top + HP_TEXTURE_HEIGHT_TWICE;				//			�V

	// �e�N�X�`�����W�̌v�Z
	src.left = 0;		// �e�N�X�`�����W�̍�����w��
	src.top = 0;		//			�V
	src.right = HP_TEXTURE_WIDTH * ratio;		// �e�N�X�`�����W�̉E�����w��
	src.bottom = HP_TEXTURE_HEIGHT;				//			�V

	pRenderTarget->DrawBitmap(m_pHPBarTexture, rc, BAR_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);


	//	MANA�o�[�̕`�� ==============================================================================================================================
	ratio = m_fManaRatio / INITIAL_MANA;
	
	// �`��̈�̐ݒ�
	rc.left = MANA_BAR_RC_LEFT;		// �`��̈�̍�����w��
	rc.top = MANA_BAR_RC_TOP;		//			�V
	rc.right = rc.left + MANA_TEXTURE_WIDTH_TWICE * ratio;		// �`��̈�̉E�����w��
	rc.bottom = rc.top + MANA_TEXTURE_HEIGHT_TWICE;				//			�V

	// �e�N�X�`�����W�̌v�Z
	src.left = 0;		// �e�N�X�`�����W�̍�����w��
	src.top = 0;		//			�V
	src.right = MANA_TEXTURE_WIDTH * ratio;		// �e�N�X�`�����W�̉E�����w��
	src.bottom = MANA_TEXTURE_HEIGHT;				//			�V

	pRenderTarget->DrawBitmap(m_pManaBarTexture, rc, BAR_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);


	//	�t���[���̕`��	=============================================================================================================================
	// �`��̈�̐ݒ�
	rc.left = BAR_FLAME_RC_LEFT;	// �`��̈�̍�����w��
	rc.top = BAR_FLAME_RC_TOP;		//			�V
	rc.right = rc.left + FLAME_TEXTURE_WIDTH_TWICE;		// �`��̈�̉E�����w��
	rc.bottom = rc.top + FLAME_TEXTURE_HEIGHT_TWICE;		//			�V

	// �e�N�X�`�����W�̌v�Z
	src.left = 0;		// �e�N�X�`�����W�̍�����w��
	src.top = 0;		//			�V
	src.right = src.left + FLAME_TEXTURE_WIDTH;		// �e�N�X�`�����W�̉E�����w��
	src.bottom = src.top + FLAME_TEXTURE_HEIGHT;		//			�V

	pRenderTarget->DrawBitmap(m_pBarFrameTexture, rc, FLAME_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);


}