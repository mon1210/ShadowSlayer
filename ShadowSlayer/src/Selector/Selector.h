/**
* @file  Selector.h
* @brief �N���XSelector�̐錾�t�@�C��
*/
#pragma once

//--- �w�b�_�K�[�h�ŁA�����w�b�_�t�@�C����������C���N���[�h����邱�Ƃɂ�����h���ł���
#ifndef __IGAMESCENE_H__
#include "..\IGameSceen\IGameScene.h"
#endif

enum GamePhase {
	GAMEPHASE_INIT		= 0,
	GAMEPHASE_RESET		= 1,
	GAMEPHASE_TITLE		= 0x010,
	GAMEPHASE_GAME		= 0x100,
	GAMEPHASE_GAMEOVER	= 0x200
};

enum GameSceneResultCode {
	GAMESCENE_DEFAULT		= 0,
	GAMESCENE_END_OK		= 1,
	GAMESCENE_END_TIMEOUT	= 2,
	GAMESCENE_END_FAILURE	= 3
};

class IGameScene;	// �V�[���Ǘ��N���X
struct ID2D1RenderTarget;
struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

class Selector
{
public:
	Selector(ID2D1RenderTarget *pRenderTarget);
	virtual ~Selector(void);
	void doAnim();		//	�Q�[���S�̂̃A�j���[�g
	void doDraw(ID2D1RenderTarget *pRenderTarget);	//	�Q�[���S�̂̕`��
	ID2D1RenderTarget	*GetRenderTarget();		// �Q�[����ʗp��RenderTarget ��Ԃ�
	IDWriteTextFormat	*GetTextFormat();		// �f�o�b�O�p��TextFormat ��Ԃ�
protected:
	ID2D1RenderTarget		*m_pRenderTarget;
	IDWriteTextFormat		*m_pTextFormat;
	ID2D1SolidColorBrush	*m_pWhiteBrush;

	IGameScene		*m_pScene;		//	�V�[��
	GamePhase	m_eGamePhase;	//	��ԕϐ�

	INT m_iWait;
	INT	m_iCount;	//	�J�E���^�[(�f�o�b�O�p)
};

//	�}�N����`
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if(o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)	if(o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY if(o){ delete [] (o); o = NULL; }

#define FPS 30
