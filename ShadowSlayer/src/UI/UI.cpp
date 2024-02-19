// Windowsアプリケーション開発用の共通ヘッダファイル
#include "stdafx.h"
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
// プレイヤーキャラクターとその関連情報を定義したクラスと定数/列挙型の定義
#include "Player.h"
// プレイヤーのUI要素を管理する
#include "UI.h"
// テクスチャとビットマップの読み込みと管理に関するクラスとインターフェースを定義
#include "TextureLoader.h"
// 定数値を定めたヘッダファイル
#include "Constants.h"


//  マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }


/**
* @brief UIのコンストラクタ
*/
UI::UI(Player *pPlayer, ID2D1RenderTarget *pRenderTarget)
{
	m_pPlayer = pPlayer;
	m_pBarFrameTexture = NULL;
	m_pHPBarTexture = NULL;
	m_pManaBarTexture = NULL;

	// 画像をメモリに読み込む
	TextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\BAR_Frame.png"), &m_pBarFrameTexture);
	TextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\HP_BAR.png"), &m_pHPBarTexture);
	TextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\MANA_BAR.png"), &m_pManaBarTexture);

	m_fHPRatio = 0.f;
	m_fManaRatio = 0.f;
}


// デストラクタ
UI::~UI()
{
	SAFE_RELEASE(m_pBarFrameTexture);
	SAFE_RELEASE(m_pHPBarTexture);
	SAFE_RELEASE(m_pManaBarTexture);
}


/**
* @brief UIの動作
* @note  他のオブジェクトと違い、消滅なし
*/
void UI::move() {
	//	HPバー
	if (m_fHPRatio > m_pPlayer->getHP()) {
		m_fHPRatio--;
	}
	else if (m_fHPRatio < m_pPlayer->getHP()) {
		m_fHPRatio++;
	}

	//	MPバー
	if (m_fManaRatio > m_pPlayer->getMana()) {
		m_fManaRatio--;
	}
	else if (m_fManaRatio < m_pPlayer->getMana()) {
		m_fManaRatio++;
	}
}


/**
* @brief 描画メソッド
*/
void UI::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc,		// 描画領域(画面上での位置やサイズ)を指定する変数
				src;	// テクスチャの中での座標を指定し、テクスチャを決定する変数
	float ratio;

	//	HPバーの描写 ==============================================================================================================================
	ratio = m_fHPRatio / INITIAL_HP;

	// 描画領域の設定
	rc.left = HP_BAR_RC_LEFT;		// 描画領域の左上隅指定
	rc.top = HP_BAR_RC_TOP;		//			〃
	rc.right = rc.left + HP_TEXTURE_WIDTH_TWICE * ratio;		// 描画領域の右下隅指定
	rc.bottom = rc.top + HP_TEXTURE_HEIGHT_TWICE;				//			〃

	// テクスチャ座標の計算
	src.left = 0;		// テクスチャ座標の左上隅指定
	src.top = 0;		//			〃
	src.right = HP_TEXTURE_WIDTH * ratio;		// テクスチャ座標の右下隅指定
	src.bottom = HP_TEXTURE_HEIGHT;				//			〃

	pRenderTarget->DrawBitmap(m_pHPBarTexture, rc, BAR_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);


	//	MANAバーの描写 ==============================================================================================================================
	ratio = m_fManaRatio / INITIAL_MANA;
	
	// 描画領域の設定
	rc.left = MANA_BAR_RC_LEFT;		// 描画領域の左上隅指定
	rc.top = MANA_BAR_RC_TOP;		//			〃
	rc.right = rc.left + MANA_TEXTURE_WIDTH_TWICE * ratio;		// 描画領域の右下隅指定
	rc.bottom = rc.top + MANA_TEXTURE_HEIGHT_TWICE;				//			〃

	// テクスチャ座標の計算
	src.left = 0;		// テクスチャ座標の左上隅指定
	src.top = 0;		//			〃
	src.right = MANA_TEXTURE_WIDTH * ratio;		// テクスチャ座標の右下隅指定
	src.bottom = MANA_TEXTURE_HEIGHT;				//			〃

	pRenderTarget->DrawBitmap(m_pManaBarTexture, rc, BAR_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);


	//	フレームの描写	=============================================================================================================================
	// 描画領域の設定
	rc.left = BAR_FLAME_RC_LEFT;	// 描画領域の左上隅指定
	rc.top = BAR_FLAME_RC_TOP;		//			〃
	rc.right = rc.left + FLAME_TEXTURE_WIDTH_TWICE;		// 描画領域の右下隅指定
	rc.bottom = rc.top + FLAME_TEXTURE_HEIGHT_TWICE;		//			〃

	// テクスチャ座標の計算
	src.left = 0;		// テクスチャ座標の左上隅指定
	src.top = 0;		//			〃
	src.right = src.left + FLAME_TEXTURE_WIDTH;		// テクスチャ座標の右下隅指定
	src.bottom = src.top + FLAME_TEXTURE_HEIGHT;		//			〃

	pRenderTarget->DrawBitmap(m_pBarFrameTexture, rc, FLAME_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);


}