/**
* @file  Selector.h
* @brief �N���XSelector�̐錾�t�@�C��
*/
#pragma once

//--- �w�b�_�K�[�h�ŁA�����w�b�_�t�@�C����������C���N���[�h����邱�Ƃɂ�����h���ł���
#ifndef __IGAMESCENE_H__
#include "..\IGameSceen\IGameScene.h"
#include "..\Enums.h"
#endif

class IGameScene;	// �V�[���Ǘ��N���X
struct ID2D1RenderTarget;
struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

class Selector
{
public:
	/**
	* @brief Selector�̃R���X�g���N�^
	*/
	Selector(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief Selector�̃f�X�g���N�^
	*/
	~Selector(void);

	/**
	* @brief �S�̂̃A�j���[�g���s���֐�
	*/
	void doAnim();

	/**
	* @brief �S�̂̕`����s���֐�
	*/
	void doDraw(ID2D1RenderTarget *pRenderTarget);

	/**
	* @method
	* @brief �Q�[����ʗp��RenderTarget ��Ԃ�
	* @note	���̃��\�b�h���Ԃ��� ID2D1RenderTarget �͕K��Release ���邱��
	*/
	ID2D1RenderTarget	*GetRenderTarget();

	/**
	* @method
	* @brief	�f�o�b�O�p��TextFormat ��Ԃ�
	* @note		���̃��\�b�h���Ԃ��� IDWriteTextFormat �͕K��Release ���邱��
	*/
	IDWriteTextFormat	*GetTextFormat();
protected:
	ID2D1RenderTarget		*m_pRenderTarget;
	IDWriteTextFormat		*m_pTextFormat;
	ID2D1SolidColorBrush	*m_pWhiteBrush;

	//! �V�[��
	IGameScene	*m_pScene;
	//! ��ԕϐ�
	GamePhase	m_eGamePhase;

	INT m_iWait;
	//! �J�E���^�[(�f�o�b�O�p)
	INT	m_iCount;
};

//	�}�N����`
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
/**
* @def SAFE_RELEASE
* @brief �|�C���^�̉��
*/
#define SAFE_RELEASE(o) if(o){ (o)->Release(); o = NULL; }
/**
* @def SAFE_DELETE
* @brief �|�C���^�̍폜
*/
#define SAFE_DELETE(o)	if(o){ delete (o); o = NULL; }
/**
* @def SAFE_DELETE_ARRAY
* @brief �|�C���^�z��̍폜
*/
#define SAFE_DELETE_ARRAY(o) if(o){ delete [] (o); o = NULL; }

#define FPS 30
