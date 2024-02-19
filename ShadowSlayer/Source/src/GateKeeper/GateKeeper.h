#pragma once

class Stage;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;

class GateKeeper
{
public:
	GateKeeper(Stage* pStage, float x, float y);
	virtual ~GateKeeper();
	virtual void draw(ID2D1RenderTarget *pRenderTarget);	// 描画メソッド
	static void Restore(ID2D1RenderTarget *pTarget);	// 共有メディアファイルの読み込み　シーン開始時などに呼び出すようにする
	static void Finalize();		// 共有メディアファイルの消去　シーン削除時などに呼び出すようにする
protected:
	Stage *m_pParent;
	static ID2D1Bitmap	*m_pImage;
	float m_fX, m_fY;
};

