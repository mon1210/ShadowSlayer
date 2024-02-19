/**
* @file GameOver.cpp
* @brief �N���X CGameOVer �̎���
*/

// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "..\stdafx\stdafx.h"
// Direct2D�̃w�b�_�t�@�C�����C���N���[�h
#include <d2d1.h>
// DirectWrite �w�b�_�[�t�@�C�����C���N���[�h
#include <dwrite.h>
// �Q�[���V�[���ƃZ���N�^�[�̊Ǘ��Ɋ֘A����w�b�_�t�@�C��
#include "..\Selector\Selector.h"
// �Q�[���I�[�o�[�V�[���̊Ǘ��Ɋ֘A����w�b�_�t�@�C��
#include "GameOver.h"
// �e�N�X�`���ƃr�b�g�}�b�v�̓ǂݍ��݂ƊǗ��Ɋւ���N���X�ƃC���^�[�t�F�[�X���`
#include "..\TextureLoader\TextureLoader.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "..\Constants.h"


/**
* @brief GameOver�̃R���X�g���N�^
*/
GameOver::GameOver(Selector *pSystem)
{
	m_pSystem = pSystem;
	m_pImage = NULL;
	m_pBlack = NULL;
	m_iTimer = 0;
	m_ePhase = GAMEOVER_INIT;
	ID2D1RenderTarget *pTarget = NULL;
	pTarget = m_pSystem->GetRenderTarget();
	if (pTarget) {
		//CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\"), &m_pImage);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}

}


// �f�X�g���N�^
GameOver::~GameOver()
{
	SAFE_RELEASE(m_pBlack);
	SAFE_RELEASE(m_pImage);
}


/**
* @brief �A�j���[�V�������\�b�h
* @return GAMESCENE_DEFAULT:�p�� / GAMESCENE_END_OK:�Q�[���J�n
*/
GameSceneResultCode GameOver::move() {
	switch (m_ePhase) {

	case GAMEOVER_INIT:
		m_iTimer = 0;
		m_bFlag = true;
		m_ePhase = GAMEOVER_RUN;

	case GAMEOVER_RUN:
	{	//	���ꂪ�Ȃ��� bDone �̏������ŃG���[���ł�
		bool bDone = false;
		m_iTimer++;
		// �I�����Ԃ�������Ǝ����I��
		if (m_iTimer > GAMEOVER_DONE_TIMER)
		{
			bDone = true;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{
			if (!m_bFlag) {
				bDone = true;
				m_bFlag = true;
			}
		}
		else	// �������Ȃ����
		{
			m_bFlag = false;
		}

		// �I���t���OON��
		if (bDone) {
			m_iFadeTimer = 0;
			m_ePhase = GAMEOVER_FADE;
		}
		break;
	}
	
	case GAMEOVER_FADE:
		m_iFadeTimer++;
		// 30�t���[�������ăt�F�[�h�A�E�g
		if (m_iFadeTimer < GAMEOVER_FADE_TIMER)
			break;
		m_ePhase = GAMEOVER_DONE;

	case GAMEOVER_DONE:
		return GAMESCENE_END_OK;
	}

	return GAMESCENE_DEFAULT;
}


/**
* @brief �`�惁�\�b�h
*/
void GameOver::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc;		// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	//textureSize = m_pImage->GetSize();
	//rc.left  = (screenSize.width - textureSize.width) / 2;	//	�Z���^�����O
	//rc.right = rc.left + textureSize.width;
	//rc.top = (screenSize.height - textureSize.height) / 2;	//	�Z���^�����O
	//rc.bottom = rc.top + textureSize.height;
	//pRenderTarget->DrawBitmap(m_pImage, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	switch (m_ePhase) {
	case GAMEOVER_FADE:
	case GAMEOVER_DONE:
		rc.left = 0;	// �`��̈�̍�����w��
		rc.top = 0;		//			�V
		rc.right = screenSize.width;		// �`��̈�̉E�����w��
		rc.bottom = screenSize.height;		// 			�V
		m_pBlack->SetOpacity(m_iFadeTimer / GAMEOVER_FADE_OUT_TIMER);	// 30�t���[�������ău���b�N�A�E�g
		pRenderTarget->FillRectangle(rc, m_pBlack);
		break;
	}
}
