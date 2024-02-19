// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "stdafx.h"
// Direct2D�̃w�b�_�t�@�C�����C���N���[�h
#include <d2d1.h>
// �Q�[�����̃X�e�[�W�֘A�N���X�Ɨ񋓌^�̒�`
#include "Stage.h"
// �Q�[�g�L�[�p�[�I�u�W�F�N�g�Ɋւ���w�b�_�t�@�C��
#include "GateKeeper.h"
// �e�N�X�`���ƃr�b�g�}�b�v�̓ǂݍ��݂ƊǗ��Ɋւ���N���X�ƃC���^�[�t�F�[�X���`
#include "TextureLoader.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "Constants.h"


//	macros
#undef  SAFE_RELEASE
#undef	SAFE_DELETE
#undef	SAFE_DELETE_ARRAY
#define	SAFE_RELEASE(o)		if(o){	(o)->Release(); (o) = NULL;	}
#define	SAFE_DELETE(o)		if(o){	delete (o); (o) = NULL;	}
#define SAFE_DELETE_ARRAY(o)	if(o){	delete [] (o); (o) = NULL;	}

ID2D1Bitmap *GateKeeper::m_pImage = NULL;


/**
* @brief GateKeeper�̃R���X�g���N�^
* @param[in] x	�Q�[�g�L�[�p�[��x���W
* @param[in] y	�Q�[�g�L�[�p�[��y���W
*/
GateKeeper::GateKeeper(Stage *pStage,float x,float y)
{
	m_pParent = pStage;
	m_fX = x;
	m_fY = y;
}


// �f�X�g���N�^
GateKeeper::~GateKeeper()
{
}


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̓ǂݍ���
* @note		�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*/
void GateKeeper::Restore(ID2D1RenderTarget *pTarget) {
	SAFE_RELEASE(m_pImage);
	// �摜���������ɓǂݍ���
	TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\gatekeeper_short.png"), &m_pImage);
}


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̏���
* @note		�V�[���폜���ȂǂɌĂяo���悤�ɂ���
*/
void GateKeeper::Finalize() {
	SAFE_RELEASE(m_pImage);
}


/**
* @brief �`�惁�\�b�h
*/
void GateKeeper::draw(ID2D1RenderTarget *pRenderTarget) {
	float distX = m_fX - m_pParent->m_fPlayerX;
	float distY = m_fY - m_pParent->m_fPlayerY;
	//�v���C���[����I�u�W�F�N�g��10�}�X�ȓ�(��ʓ��ɉf���Ă�����)
	if (distX < HALF_MAP_WIDTH_PIXELS) {
		D2D1_RECT_F rc;		// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
		rc.left = m_pParent->m_fPlayerDrawX + distX;		// �`��̈�̍�����w��
		rc.top = m_fY;										//			�V
		rc.right = rc.left + GATE_KEEPER_WIDTH_SIZE;	//  �`��̈�̉E�����w��
		rc.bottom = rc.top + GATE_KEEPER_HEIGHT_SIZE;	//			�V
		pRenderTarget->DrawBitmap(m_pImage, rc, GATE_KEEPER_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}
	
}