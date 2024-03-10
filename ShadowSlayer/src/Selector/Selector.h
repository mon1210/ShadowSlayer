/**
* @file  Selector.h
* @brief クラスSelectorの宣言ファイル
*/
#pragma once

//--- ヘッダガードで、同じヘッダファイルが複数回インクルードされることによる問題を防いでいる
#ifndef __IGAMESCENE_H__
#include "..\IGameSceen\IGameScene.h"
#include "..\Enums.h"
#endif

class IGameScene;	// シーン管理クラス
struct ID2D1RenderTarget;
struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

class Selector
{
public:
	/**
	* @brief Selectorのコンストラクタ
	*/
	Selector(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief Selectorのデストラクタ
	*/
	~Selector(void);

	/**
	* @brief 全体のアニメートを行う関数
	*/
	void doAnim();

	/**
	* @brief 全体の描画を行う関数
	*/
	void doDraw(ID2D1RenderTarget *pRenderTarget);

	/**
	* @method
	* @brief ゲーム画面用のRenderTarget を返す
	* @note	このメソッドが返した ID2D1RenderTarget は必ずRelease すること
	*/
	ID2D1RenderTarget	*GetRenderTarget();

	/**
	* @method
	* @brief	デバッグ用のTextFormat を返す
	* @note		このメソッドが返した IDWriteTextFormat は必ずRelease すること
	*/
	IDWriteTextFormat	*GetTextFormat();
protected:
	ID2D1RenderTarget		*m_pRenderTarget;
	IDWriteTextFormat		*m_pTextFormat;
	ID2D1SolidColorBrush	*m_pWhiteBrush;

	//! シーン
	IGameScene	*m_pScene;
	//! 状態変数
	GamePhase	m_eGamePhase;

	INT m_iWait;
	//! カウンター(デバッグ用)
	INT	m_iCount;
};

//	マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
/**
* @def SAFE_RELEASE
* @brief ポインタの解放
*/
#define SAFE_RELEASE(o) if(o){ (o)->Release(); o = NULL; }
/**
* @def SAFE_DELETE
* @brief ポインタの削除
*/
#define SAFE_DELETE(o)	if(o){ delete (o); o = NULL; }
/**
* @def SAFE_DELETE_ARRAY
* @brief ポインタ配列の削除
*/
#define SAFE_DELETE_ARRAY(o) if(o){ delete [] (o); o = NULL; }

#define FPS 30
