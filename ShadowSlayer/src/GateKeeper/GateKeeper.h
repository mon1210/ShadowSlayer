#pragma once

class Stage;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;

class GateKeeper
{
public:
	GateKeeper(Stage* pStage, float x, float y);
	virtual ~GateKeeper();
	virtual void draw(ID2D1RenderTarget *pRenderTarget);	// �`�惁�\�b�h
	static void Restore(ID2D1RenderTarget *pTarget);	// ���L���f�B�A�t�@�C���̓ǂݍ��݁@�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	static void Finalize();		// ���L���f�B�A�t�@�C���̏����@�V�[���폜���ȂǂɌĂяo���悤�ɂ���
protected:
	Stage *m_pParent;
	static ID2D1Bitmap	*m_pImage;
	float m_fX, m_fY;
};

