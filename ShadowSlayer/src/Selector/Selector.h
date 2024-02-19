/**
* @file  Selector.h
* @brief クラスSelectorの宣言ファイル
*/
#pragma once

//--- ヘッダガードで、同じヘッダファイルが複数回インクルードされることによる問題を防いでいる
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

class IGameScene;	// シーン管理クラス
struct ID2D1RenderTarget;
struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

class Selector
{
public:
	Selector(ID2D1RenderTarget *pRenderTarget);
	virtual ~Selector(void);
	void doAnim();		//	ゲーム全体のアニメート
	void doDraw(ID2D1RenderTarget *pRenderTarget);	//	ゲーム全体の描画
	ID2D1RenderTarget	*GetRenderTarget();		// ゲーム画面用のRenderTarget を返す
	IDWriteTextFormat	*GetTextFormat();		// デバッグ用のTextFormat を返す
protected:
	ID2D1RenderTarget		*m_pRenderTarget;
	IDWriteTextFormat		*m_pTextFormat;
	ID2D1SolidColorBrush	*m_pWhiteBrush;

	IGameScene		*m_pScene;		//	シーン
	GamePhase	m_eGamePhase;	//	状態変数

	INT m_iWait;
	INT	m_iCount;	//	カウンター(デバッグ用)
};

//	マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if(o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)	if(o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY if(o){ delete [] (o); o = NULL; }

#define FPS 30
