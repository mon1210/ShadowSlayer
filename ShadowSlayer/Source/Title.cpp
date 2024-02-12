/**
* @file Title.cpp
* @brief クラス CTitle の実装
*/

// Windowsアプリケーション開発用の共通ヘッダファイル
#include "stdafx.h"
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
// DirectWrite ヘッダーファイルをインクルード
#include <dwrite.h>
// ゲームシーンとセレクターの管理に関連するヘッダファイル
#include "Selector.h"
// タイトル画面のゲームシーンを管理するヘッダファイル
#include "Title.h"
// テクスチャとビットマップの読み込みと管理に関するクラスとインターフェースを定義
#include "TextureLoader.h"
// 定数値を定めたヘッダファイル
#include "Constants.h"


/**
* @brief Titleのコンストラクタ
*/
Title::Title(Selector *pSystem)
{
	ID2D1RenderTarget *pTarget;

	m_pSystem = pSystem;	//---CSelect
	m_ePhase = TITLE_INIT;	//---TitlePhase
	m_iTimer = 0;			//---INT
	m_iFadeTimer = 0;		//---INT
	m_pImage = NULL;		//---ID2D1Bitmap
	m_pImage2 = NULL;		//---ID2D1Bitmap
	m_pBlack = NULL;		//---ID2D1SolidColorBrush

	pTarget = m_pSystem->GetRenderTarget();
	if (pTarget) {
		// 画像をメモリに読み込む
		TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\title_background.png"), &m_pImage);
		TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\title_string.png"), &m_pImage2);
		TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\title_press_space.png"), &m_pImage3);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}
	
}


// デストラクタ
Title::~Title()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pImage2);
	SAFE_RELEASE(m_pImage3);
	SAFE_RELEASE(m_pBlack);
}


/**
* @brief アニメーションメソッド
* @return GAMESCENE_DEFAULT:継続 / GAMESCENE_END_OK:ゲーム開始
*/
GameSceneResultCode Title::move() {
	switch (m_ePhase) {
	// タイトル開始
	case TITLE_INIT:
		m_iTimer = TITLE_TIMER_INIT;
		m_bFlag = true;
		m_ePhase = TITLE_RUN;

	// タイトル画面
	case TITLE_RUN:
	{	//	これがないと bDone の初期化でエラーがでる
		bool bDone = false;
		m_iTimer++;

		if (GetAsyncKeyState(VK_SPACE))
		{
			if (!m_bFlag) {
				bDone = true;
				m_bFlag = true;
			}
		}
		else	// 何もしていない時
		{
			m_bFlag = false;
		}
		// タイトル終了フラグON
		if (bDone) {
			m_iFadeTimer = TITLE_DONE_TIME;
			m_ePhase = TITLE_FADE;
		}
		break;
	}

	// タイトルフェードアウト
	case TITLE_FADE:
		m_iFadeTimer++;
		// 30フレームかけてフェードアウト
		if (m_iFadeTimer < TITLE_FADEOUT_TIME)
			break;
		m_ePhase = TITLE_DONE;

	// タイトル終了
	case TITLE_DONE:
		return GAMESCENE_END_OK;
	}

	return GAMESCENE_DEFAULT;
}


/**
* @brief 描画処理
* @note	 背景は二枚の画像でループしているように見せる
*/
void Title::draw(ID2D1RenderTarget *pRenderTarget) {
	int frame;
	float opacity;
	D2D1_RECT_F rc, rc2, 	// 描画領域(画面上での位置やサイズ)を指定する変数
				src;		// テクスチャの中での座標を指定し、テクスチャを決定する変数
	D2D1_SIZE_F screenSize, textureSize;
	frame = m_iTimer % TITLE_LOOP_FRAME;
	screenSize = pRenderTarget->GetSize();
	textureSize = m_pImage2->GetSize();

	//	背景描画
	// 描画領域の設定
	if (frame < TITLE_LOOP_HALF_FRAME) {
		rc.left = -frame;
		rc2.left = TITLE_LOOP_HALF_FRAME - frame;
	}
	else {	// 二枚目の画像
		rc.left = TITLE_LOOP_FRAME - frame;
		rc2.left = TITLE_LOOP_HALF_FRAME - frame;
	}
	rc.right = rc.left + TITLE_LOOP_HALF_FRAME;
	rc.top = 0;
	rc.bottom = rc.top + screenSize.height;

	// テクスチャ座標の計算
	src.left = 0;
	src.right = src.left + TITLE_LOOP_HALF_FRAME;
	src.top = 0;
	src.bottom = src.top + screenSize.height;
	pRenderTarget->DrawBitmap(m_pImage, rc, BG_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

	// 描画領域の設定
	rc.left = TITLE_LOOP_HALF_FRAME - frame;
	rc.right = rc.left + TITLE_LOOP_HALF_FRAME;

	// テクスチャ座標の計算
	src.left = TITLE_LOOP_HALF_FRAME;
	src.right = src.left + TITLE_LOOP_HALF_FRAME;
	pRenderTarget->DrawBitmap(m_pImage, rc, BG_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);


	//	タイトル描画
	// 描画領域の設定
	rc.left  = (screenSize.width - textureSize.width) / CENTER;	//	センタリング
	rc.right = rc.left + textureSize.width;
	rc.top = (screenSize.height - textureSize.height) / CENTER - TITLE_MARGIN_ABOVE;	//	センタリング
	rc.bottom = rc.top + textureSize.height;
	pRenderTarget->DrawBitmap(m_pImage2, rc, TITLE_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	//	press space
	// 描画領域の設定
	int resizeWidth = ((int)textureSize.width >> 2) * 3;	// 画像サイズを3/4にして使用
	int resizeHeight = ((int)textureSize.height >> 2) * 3;
	rc.left = (screenSize.width - resizeWidth) / CENTER;	//	センタリング
	rc.right = rc.left + resizeWidth;
	rc.top = (screenSize.height - resizeHeight) / CENTER + VERTICAL_SHIFT_AMOUNT;	//	センタリング
	rc.bottom = rc.top + resizeHeight;

	// press space の点滅処理
	frame = m_iTimer % TEXT_OPACITY_TIMER;
	if (frame < DARK_THRESHOLD) {	// 徐々に透明
		opacity = frame / PER_FLOAT;
	}
	else if (frame < LIGHT_THRESHOLD) {		// 透明
		opacity = MAX_OPACITY;
	}
	else		// 徐々に鮮明
	{
		opacity = DARK_START_OPACITY - ((frame - DARK_THRESHOLD_FLAME) / PER_FLOAT);
	}
	pRenderTarget->DrawBitmap(m_pImage3, rc, opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	
	switch (m_ePhase) {
	case TITLE_FADE:
	case TITLE_DONE:
		rc.left = 0;	// 描画領域の左上隅指定
		rc.top = 0;		//			〃
		rc.right = screenSize.width;	// 描画領域の右下隅指定
		rc.bottom = screenSize.height;	//			〃
		m_pBlack->SetOpacity(m_iFadeTimer / TITLE_BLACKOUT_TIME);	// 30フレームかけてブラックアウト
		pRenderTarget->FillRectangle(rc, m_pBlack);
		break;
	}
}