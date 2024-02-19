/**
* @file GameOver.cpp
* @brief クラス CGameOVer の実装
*/

// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
// DirectWrite ヘッダーファイルをインクルード
#include <dwrite.h>
// ゲームシーンとセレクターの管理に関連するヘッダファイル
#include "..\Selector\Selector.h"
// ゲームオーバーシーンの管理に関連するヘッダファイル
#include "GameOver.h"
// テクスチャとビットマップの読み込みと管理に関するクラスとインターフェースを定義
#include "..\TextureLoader\TextureLoader.h"
// 定数値を定めたヘッダファイル
#include "..\Constants.h"


/**
* @brief GameOverのコンストラクタ
*/
GameOver::GameOver(Selector *pSystem)
{
	m_pSystem = pSystem;
	m_pImage = NULL;
	m_pBlack = NULL;
	m_iTimer = 0;
	m_ePhase = GAMEOVER_INIT;
	ID2D1RenderTarget *pTarget = NULL;
	pTarget = m_pSystem->GetRenderTarget();
	if (pTarget) {
		//CTextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\"), &m_pImage);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlack);
		pTarget->Release();
		pTarget = NULL;
	}

}


// デストラクタ
GameOver::~GameOver()
{
	SAFE_RELEASE(m_pBlack);
	SAFE_RELEASE(m_pImage);
}


/**
* @brief アニメーションメソッド
* @return GAMESCENE_DEFAULT:継続 / GAMESCENE_END_OK:ゲーム開始
*/
GameSceneResultCode GameOver::move() {
	switch (m_ePhase) {

	case GAMEOVER_INIT:
		m_iTimer = 0;
		m_bFlag = true;
		m_ePhase = GAMEOVER_RUN;

	case GAMEOVER_RUN:
	{	//	これがないと bDone の初期化でエラーがでる
		bool bDone = false;
		m_iTimer++;
		// 終了時間をすぎると自動終了
		if (m_iTimer > GAMEOVER_DONE_TIMER)
		{
			bDone = true;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{
			if (!m_bFlag) {
				bDone = true;
				m_bFlag = true;
			}
		}
		else	// 何もしなければ
		{
			m_bFlag = false;
		}

		// 終了フラグON時
		if (bDone) {
			m_iFadeTimer = 0;
			m_ePhase = GAMEOVER_FADE;
		}
		break;
	}
	
	case GAMEOVER_FADE:
		m_iFadeTimer++;
		// 30フレームかけてフェードアウト
		if (m_iFadeTimer < GAMEOVER_FADE_TIMER)
			break;
		m_ePhase = GAMEOVER_DONE;

	case GAMEOVER_DONE:
		return GAMESCENE_END_OK;
	}

	return GAMESCENE_DEFAULT;
}


/**
* @brief 描画メソッド
*/
void GameOver::draw(ID2D1RenderTarget *pRenderTarget) {
	D2D1_RECT_F rc;		// 描画領域(画面上での位置やサイズ)を指定する変数
	D2D1_SIZE_F screenSize, textureSize;
	screenSize = pRenderTarget->GetSize();
	//textureSize = m_pImage->GetSize();
	//rc.left  = (screenSize.width - textureSize.width) / 2;	//	センタリング
	//rc.right = rc.left + textureSize.width;
	//rc.top = (screenSize.height - textureSize.height) / 2;	//	センタリング
	//rc.bottom = rc.top + textureSize.height;
	//pRenderTarget->DrawBitmap(m_pImage, rc, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);

	switch (m_ePhase) {
	case GAMEOVER_FADE:
	case GAMEOVER_DONE:
		rc.left = 0;	// 描画領域の左上隅指定
		rc.top = 0;		//			〃
		rc.right = screenSize.width;		// 描画領域の右下隅指定
		rc.bottom = screenSize.height;		// 			〃
		m_pBlack->SetOpacity(m_iFadeTimer / GAMEOVER_FADE_OUT_TIMER);	// 30フレームかけてブラックアウト
		pRenderTarget->FillRectangle(rc, m_pBlack);
		break;
	}
}
