// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
// ゲーム内のステージ関連クラスと列挙型の定義
#include "..\Stage\Stage.h"
// ゲートキーパーオブジェクトに関するヘッダファイル
#include "GateKeeper.h"
// テクスチャとビットマップの読み込みと管理に関するクラスとインターフェースを定義
#include "..\TextureLoader\TextureLoader.h"
// 定数値を定めたヘッダファイル
#include "..\Constants.h"


//	macros
#undef  SAFE_RELEASE
#undef	SAFE_DELETE
#undef	SAFE_DELETE_ARRAY
#define	SAFE_RELEASE(o)		if(o){	(o)->Release(); (o) = NULL;	}
#define	SAFE_DELETE(o)		if(o){	delete (o); (o) = NULL;	}
#define SAFE_DELETE_ARRAY(o)	if(o){	delete [] (o); (o) = NULL;	}

ID2D1Bitmap *GateKeeper::m_pImage = NULL;


// コンストラクタ
GateKeeper::GateKeeper(Stage *pStage,float x,float y)
{
	m_pParent = pStage;
	m_fX = x;
	m_fY = y;
}


// デストラクタ
GateKeeper::~GateKeeper()
{
}


// 共有メディアファイルの読み込み
void GateKeeper::Restore(ID2D1RenderTarget *pTarget) {
	SAFE_RELEASE(m_pImage);
	// 画像をメモリに読み込む
	TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\gatekeeper_short.png"), &m_pImage);
}


// 共有メディアファイルの消去
void GateKeeper::Finalize() {
	SAFE_RELEASE(m_pImage);
}


// 描画メソッド
void GateKeeper::draw(ID2D1RenderTarget *pRenderTarget) {
	float distX = m_fX - m_pParent->m_fPlayerX;
	float distY = m_fY - m_pParent->m_fPlayerY;
	//プレイヤーからオブジェクトが10マス以内(画面内に映っていたら)
	if (distX < HALF_MAP_WIDTH_PIXELS) {
		D2D1_RECT_F rc;		// 描画領域(画面上での位置やサイズ)を指定する変数
		rc.left = m_pParent->m_fPlayerDrawX + distX;		// 描画領域の左上隅指定
		rc.top = m_fY;										//			〃
		rc.right = rc.left + GATE_KEEPER_WIDTH_SIZE;	//  描画領域の右下隅指定
		rc.bottom = rc.top + GATE_KEEPER_HEIGHT_SIZE;	//			〃
		pRenderTarget->DrawBitmap(m_pImage, rc, GATE_KEEPER_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
	}
	
}