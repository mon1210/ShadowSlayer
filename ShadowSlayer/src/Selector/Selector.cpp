// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
// DirectWrite ヘッダーファイルをインクルード
#include <dwrite.h>
// ゲームシーンとセレクターの管理に関連するヘッダファイル
#include "Selector.h"
// タイトル画面のゲームシーンを管理するヘッダファイル
#include "..\Title\Title.h"
// ゲーム内のステージ関連クラスと列挙型の定義
#include "..\Stage\Stage.h"
// ゲームオーバーシーンの管理に関連するヘッダファイル
#include "..\GameOver\GameOver.h"
// テクスチャとビットマップの読み込みと管理に関するクラスとインターフェースを定義
#include "..\TextureLoader\TextureLoader.h"
// 定数値を定めたヘッダファイル
#include "..\Constants.h"


// コンストラクタ
Selector::Selector(ID2D1RenderTarget *pRenderTarget)
{
	m_pRenderTarget = pRenderTarget;
	m_pRenderTarget->AddRef();

	// メンバ初期化
	m_iCount = 0;
	m_iWait = 0;
	m_eGamePhase = GAMEPHASE_INIT;
	m_pScene = NULL;
	m_pWhiteBrush = NULL;
	
	// Direct Write 初期化
	{
		HRESULT hr;
		IDWriteFactory *pFactory;
		// DirectWriteファクトリー生成
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(pFactory),
			reinterpret_cast<IUnknown **>(&pFactory)
		);
		// テキストフォーマット生成
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

	// テキスト用ブラシの生成
	if (FAILED(m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&m_pWhiteBrush
	))) {
		SAFE_RELEASE(m_pWhiteBrush);
	}
}


// デストラクタ
Selector::~Selector()
{
	SAFE_DELETE(m_pScene);
	SAFE_RELEASE(m_pWhiteBrush);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pTextFormat);
	TextureLoader::Destroy();
}


// 全体のアニメートを行う
void Selector::doAnim() {
	GameSceneResultCode rc = GAMESCENE_DEFAULT;

	switch (m_eGamePhase) {

	case GAMEPHASE_INIT:
		m_eGamePhase = GAMEPHASE_RESET;

	case GAMEPHASE_RESET:
		SAFE_DELETE(m_pScene);
		m_pScene = new Title(this);
		m_eGamePhase = GAMEPHASE_TITLE;

	// タイトルシーン
	case GAMEPHASE_TITLE:
		if (m_pScene != NULL)
			rc = m_pScene->move();
		if (rc == GAMESCENE_DEFAULT)
			break;
		SAFE_DELETE(m_pScene);
		m_pScene = new Stage(this);
		m_eGamePhase = GAMEPHASE_GAME;

	// ゲームシーン
	case GAMEPHASE_GAME:
		if (m_pScene != NULL)
			rc = m_pScene->move();
		if (rc == GAMESCENE_DEFAULT)
			break;

		m_eGamePhase = GAMEPHASE_RESET;

	}
	// デバッグ用カウンタ
	m_iCount++;
}


// 全体の描画を行う
void Selector::doDraw(ID2D1RenderTarget *pRenderTarget) {

	// 1バイトのビット数(2^8)
	TCHAR	str[256];

	//	シーンを描画
	if (m_pScene != NULL)
		m_pScene->draw(pRenderTarget);
#ifdef _DEBUG
	//	デバッグ用表示
	D2D1_SIZE_F size;
	size = pRenderTarget->GetSize();
	_stprintf_s(str, _countof(str), _T("%08d"), m_iCount);
	//左上にタイマー表示
	D2D1_RECT_F	 rc;	// 描画領域(画面上での位置やサイズ)を指定する変数
	rc.left = 0;	// 描画領域の左上隅指定
	rc.top = 0;		//			〃
	rc.right = size.width;		// 描画領域の右下隅指定
	rc.bottom = size.height;	//			〃

	if (m_pWhiteBrush) {
		pRenderTarget->DrawText(str, _tcslen(str), m_pTextFormat, &rc, m_pWhiteBrush);
	}
#endif
}


// ゲーム画面用のRenderTarget を返す
ID2D1RenderTarget *Selector::GetRenderTarget() {
	m_pRenderTarget->AddRef();
	return m_pRenderTarget;
}


// デバッグ用のTextFormat を返す
IDWriteTextFormat *Selector::GetTextFormat() {
	m_pTextFormat->AddRef();
	return m_pTextFormat;
}