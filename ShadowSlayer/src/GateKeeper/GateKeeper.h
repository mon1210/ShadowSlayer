/**
* @file  GateKeeper.h
* @brief �N���X GateKeeper �̐錾
*/
#pragma once

class Stage;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;

class GateKeeper
{
public:
	/**
	* @brief GateKeeper�̃R���X�g���N�^
	* @param[in] x	�Q�[�g�L�[�p�[��x���W
	* @param[in] y	�Q�[�g�L�[�p�[��y���W
	*/
	GateKeeper(Stage* pStage, float x, float y);

	/**
	* @brief GateKeeper�̃f�X�g���N�^
	*/
	~GateKeeper();

	/**
	* @brief �`�惁�\�b�h
	*/
	void draw(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief	���L���f�B�A�t�@�C���̓ǂݍ���
	* @note		�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Restore(ID2D1RenderTarget *pTarget);

	/**
	* @brief	���L���f�B�A�t�@�C���̏���
	* @note		�V�[���폜���ȂǂɌĂяo���悤�ɂ���
	*/
	static void Finalize();
protected:
	Stage *m_pParent;
	static ID2D1Bitmap	*m_pImage;
	float m_fX, m_fY;
};

