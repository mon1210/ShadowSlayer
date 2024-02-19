/**
* @file Title.cpp
* @brief �N���X CTitle �̎���
*/

// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "stdafx.h"
// Direct2D�̃w�b�_�t�@�C�����C���N���[�h
#include <d2d1.h>
// DirectWrite �w�b�_�[�t�@�C�����C���N���[�h
#include <dwrite.h>
// �Q�[���V�[���ƃZ���N�^�[�̊Ǘ��Ɋ֘A����w�b�_�t�@�C��
#include "Selector.h"
// �^�C�g����ʂ̃Q�[���V�[�����Ǘ�����w�b�_�t�@�C��
#include "Title.h"
// �e�N�X�`���ƃr�b�g�}�b�v�̓ǂݍ��݂ƊǗ��Ɋւ���N���X�ƃC���^�[�t�F�[�X���`
#include "TextureLoader.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "Constants.h"


/**
* @brief Title�̃R���X�g���N�^
*/
Title::Title(Selector *pSystem)
{
	ID2D1RenderTarget *pTarget;

	m_pSystem = pSystem;	//---CSelect
	m_ePhase = TITLE_INIT;	//---TitlePhase
	m_iTimer = 0;			//---INT
	m_iFadeTimer = 0;		//---INT
	m_pImage = NULL;		//---ID2D1Bitmap
	m_pImage2 = NULL;		//---ID2D1Bitmap
	m_pBlack = NULL;		//---ID2D1SolidColorBrush

	pTarget = m_pSystem->GetRenderTarget();
	if (pTarget) {
		// �摜���������ɓǂݍ���
		TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\title_background.png"), &m_pImage);
		TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\title_string.png"), &m_pImage2);
		TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\title_press_space.png"), &m_pImage3);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}
	
}


// �f�X�g���N�^
Title::~Title()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pImage2);
	SAFE_RELEASE(m_pImage3);
	SAFE_RELEASE(m_pBlack);
}


/**
* @brief �A�j���[�V�������\�b�h
* @return GAMESCENE_DEFAULT:�p�� / GAMESCENE_END_OK:�Q�[���J�n
*/
GameSceneResultCode Title::move() {
	switch (m_ePhase) {
	// �^�C�g���J�n
	case TITLE_INIT:
		m_iTimer = TITLE_TIMER_INIT;
		m_bFlag = true;
		m_ePhase = TITLE_RUN;

	// �^�C�g�����
	case TITLE_RUN:
	{	//	���ꂪ�Ȃ��� bDone �̏������ŃG���[���ł�
		bool bDone = false;
		m_iTimer++;

		if (GetAsyncKeyState(VK_SPACE))
		{
			if (!m_bFlag) {
				bDone = true;
				m_bFlag = true;
			}
		}
		else	// �������Ă��Ȃ���
		{
			m_bFlag = false;
		}
		// �^�C�g���I���t���OON
		if (bDone) {
			m_iFadeTimer = TITLE_DONE_TIME;
			m_ePhase = TITLE_FADE;
		}
		break;
	}

	// �^�C�g���t�F�[�h�A�E�g
	case TITLE_FADE:
		m_iFadeTimer++;
		// 30�t���[�������ăt�F�[�h�A�E�g
		if (m_iFadeTimer < TITLE_FADEOUT_TIME)
			break;
		m_ePhase = TITLE_DONE;

	// �^�C�g���I��
	case TITLE_DONE:
		return GAMESCENE_END_OK;
	}

	return GAMESCENE_DEFAULT;
}


/**
* @brief �`�揈��
* @note	 �w�i�͓񖇂̉摜�Ń��[�v���Ă���悤�Ɍ�����
*/
void Title::draw(ID2D1RenderTarget *pRenderTarget) {
	int frame;
	float opacity;
	D2D1_RECT_F rc, rc2, 	// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
				src;		// �e�N�X�`���̒��ł̍��W���w�肵�A�e�N�X�`�������肷��ϐ�
	D2D1_SIZE_F screenSize, textureSize;
	frame = m_iTimer % TITLE_LOOP_FRAME;
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImage2->GetSize();

	//	�w�i�`��
	// �`��̈�̐ݒ�
	if (frame < TITLE_LOOP_HALF_FRAME) {
		rc.left = -frame;
		rc2.left = TITLE_LOOP_HALF_FRAME - frame;
	}
	else {	// �񖇖ڂ̉摜
		rc.left = TITLE_LOOP_FRAME - frame;
		rc2.left = TITLE_LOOP_HALF_FRAME - frame;
	}
	rc.right = rc.left + TITLE_LOOP_HALF_FRAME;
	rc.top = 0;
	rc.bottom = rc.top + screenSize.height;

	// �e�N�X�`�����W�̌v�Z
	src.left = 0;
	src.right = src.left + TITLE_LOOP_HALF_FRAME;
	src.top = 0;
	src.bottom = src.top + screenSize.height;
	pRenderTarget->DrawBitmap(m_pImage, rc, BG_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

	// �`��̈�̐ݒ�
	rc.left = TITLE_LOOP_HALF_FRAME - frame;
	rc.right = rc.left + TITLE_LOOP_HALF_FRAME;

	// �e�N�X�`�����W�̌v�Z
	src.left = TITLE_LOOP_HALF_FRAME;
	src.right = src.left + TITLE_LOOP_HALF_FRAME;
	pRenderTarget->DrawBitmap(m_pImage, rc, BG_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);


	//	�^�C�g���`��
	// �`��̈�̐ݒ�
	rc.left  = (screenSize.width - textureSize.width) / CENTER;	//	�Z���^�����O
	rc.right = rc.left + textureSize.width;
	rc.top = (screenSize.height - textureSize.height) / CENTER - TITLE_MARGIN_ABOVE;	//	�Z���^�����O
	rc.bottom = rc.top + textureSize.height;
	pRenderTarget->DrawBitmap(m_pImage2, rc, TITLE_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	//	press space
	// �`��̈�̐ݒ�
	int resizeWidth = ((int)textureSize.width >> 2) * 3;	// �摜�T�C�Y��3/4�ɂ��Ďg�p
	int resizeHeight = ((int)textureSize.height >> 2) * 3;
	rc.left = (screenSize.width - resizeWidth) / CENTER;	//	�Z���^�����O
	rc.right = rc.left + resizeWidth;
	rc.top = (screenSize.height - resizeHeight) / CENTER + VERTICAL_SHIFT_AMOUNT;	//	�Z���^�����O
	rc.bottom = rc.top + resizeHeight;

	// press space �̓_�ŏ���
	frame = m_iTimer % TEXT_OPACITY_TIMER;
	if (frame < DARK_THRESHOLD) {	// ���X�ɓ���
		opacity = frame / PER_FLOAT;
	}
	else if (frame < LIGHT_THRESHOLD) {		// ����
		opacity = MAX_OPACITY;
	}
	else		// ���X�ɑN��
	{
		opacity = DARK_START_OPACITY - ((frame - DARK_THRESHOLD_FLAME) / PER_FLOAT);
	}
	pRenderTarget->DrawBitmap(m_pImage3, rc, opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	
	switch (m_ePhase) {
	case TITLE_FADE:
	case TITLE_DONE:
		rc.left = 0;	// �`��̈�̍�����w��
		rc.top = 0;		//			�V
		rc.right = screenSize.width;	// �`��̈�̉E�����w��
		rc.bottom = screenSize.height;	//			�V
		m_pBlack->SetOpacity(m_iFadeTimer / TITLE_BLACKOUT_TIME);	// 30�t���[�������ău���b�N�A�E�g
		pRenderTarget->FillRectangle(rc, m_pBlack);
		break;
	}
}