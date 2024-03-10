// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "..\stdafx\stdafx.h"
// Direct2D�̃w�b�_�t�@�C�����C���N���[�h
#include <d2d1.h>
// DirectWrite �w�b�_�[�t�@�C�����C���N���[�h
#include <dwrite.h>
// �Q�[���V�[���ƃZ���N�^�[�̊Ǘ��Ɋ֘A����w�b�_�t�@�C��
#include "Selector.h"
// �^�C�g����ʂ̃Q�[���V�[�����Ǘ�����w�b�_�t�@�C��
#include "..\Title\Title.h"
// �Q�[�����̃X�e�[�W�֘A�N���X�Ɨ񋓌^�̒�`
#include "..\Stage\Stage.h"
// �Q�[���I�[�o�[�V�[���̊Ǘ��Ɋ֘A����w�b�_�t�@�C��
#include "..\GameOver\GameOver.h"
// �e�N�X�`���ƃr�b�g�}�b�v�̓ǂݍ��݂ƊǗ��Ɋւ���N���X�ƃC���^�[�t�F�[�X���`
#include "..\TextureLoader\TextureLoader.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "..\Constants.h"


// �R���X�g���N�^
Selector::Selector(ID2D1RenderTarget *pRenderTarget)
{
	m_pRenderTarget = pRenderTarget;
	m_pRenderTarget->AddRef();

	// �����o������
	m_iCount = 0;
	m_iWait = 0;
	m_eGamePhase = GAMEPHASE_INIT;
	m_pScene = NULL;
	m_pWhiteBrush = NULL;
	
	// Direct Write ������
	{
		HRESULT hr;
		IDWriteFactory *pFactory;
		// DirectWrite�t�@�N�g���[����
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(pFactory),
			reinterpret_cast<IUnknown **>(&pFactory)
		);
		// �e�L�X�g�t�H�[�}�b�g����
		hr = pFactory->CreateTextFormat(
			_T("consolas"),
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20,
			L"",
			&m_pTextFormat
		);

		SAFE_RELEASE(pFactory);
	}

	// �e�L�X�g�p�u���V�̐���
	if (FAILED(m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&m_pWhiteBrush
	))) {
		SAFE_RELEASE(m_pWhiteBrush);
	}
}


// �f�X�g���N�^
Selector::~Selector()
{
	SAFE_DELETE(m_pScene);
	SAFE_RELEASE(m_pWhiteBrush);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pTextFormat);
	TextureLoader::Destroy();
}


// �S�̂̃A�j���[�g���s��
void Selector::doAnim() {
	GameSceneResultCode rc = GAMESCENE_DEFAULT;

	switch (m_eGamePhase) {

	case GAMEPHASE_INIT:
		m_eGamePhase = GAMEPHASE_RESET;

	case GAMEPHASE_RESET:
		SAFE_DELETE(m_pScene);
		m_pScene = new Title(this);
		m_eGamePhase = GAMEPHASE_TITLE;

	// �^�C�g���V�[��
	case GAMEPHASE_TITLE:
		if (m_pScene != NULL)
			rc = m_pScene->move();
		if (rc == GAMESCENE_DEFAULT)
			break;
		SAFE_DELETE(m_pScene);
		m_pScene = new Stage(this);
		m_eGamePhase = GAMEPHASE_GAME;

	// �Q�[���V�[��
	case GAMEPHASE_GAME:
		if (m_pScene != NULL)
			rc = m_pScene->move();
		if (rc == GAMESCENE_DEFAULT)
			break;

		m_eGamePhase = GAMEPHASE_RESET;

	}
	// �f�o�b�O�p�J�E���^
	m_iCount++;
}


// �S�̂̕`����s��
void Selector::doDraw(ID2D1RenderTarget *pRenderTarget) {

	// 1�o�C�g�̃r�b�g��(2^8)
	TCHAR	str[256];

	//	�V�[����`��
	if (m_pScene != NULL)
		m_pScene->draw(pRenderTarget);
#ifdef _DEBUG
	//	�f�o�b�O�p�\��
	D2D1_SIZE_F size;
	size = pRenderTarget->GetSize();
	_stprintf_s(str, _countof(str), _T("%08d"), m_iCount);
	//����Ƀ^�C�}�[�\��
	D2D1_RECT_F	 rc;	// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
	rc.left = 0;	// �`��̈�̍�����w��
	rc.top = 0;		//			�V
	rc.right = size.width;		// �`��̈�̉E�����w��
	rc.bottom = size.height;	//			�V

	if (m_pWhiteBrush) {
		pRenderTarget->DrawText(str, _tcslen(str), m_pTextFormat, &rc, m_pWhiteBrush);
	}
#endif
}


// �Q�[����ʗp��RenderTarget ��Ԃ�
ID2D1RenderTarget *Selector::GetRenderTarget() {
	m_pRenderTarget->AddRef();
	return m_pRenderTarget;
}


// �f�o�b�O�p��TextFormat ��Ԃ�
IDWriteTextFormat *Selector::GetTextFormat() {
	m_pTextFormat->AddRef();
	return m_pTextFormat;
}