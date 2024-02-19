/**
* @file		My2DAction.cpp
* @author	R.Monda
* @date		26.October.2023
* @brief	シーン管理クラス		: CSelector	(Selector.h)
*			ゲームシーンクラス		: CStage	(Stage.h)
*			プレイヤークラス		: CPlayer	(Player.h)
*			背景・マップクラス		: CBG		(BG.h)
*/

// Windowsアプリケーション開発用の共通ヘッダファイル
#include "stdafx.h"
// CRT デバッグヘッダファイルをインクルード
#include <crtdbg.h>
// TCHAR タイプやマクロを定義したヘッダファイルをインクルード
#include <tchar.h>
// 標準入出力関数を提供する C ライブラリヘッダファイルをインクルード
#include <stdio.h>
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
// DirectWrite ヘッダーファイルをインクルード
#include <dwrite.h>
// ゲームシーンとセレクターの管理に関連するヘッダファイル
#include "Selector.h"
// 定数値を定めたヘッダファイル
#include "Constants.h"

#pragma comment(lib,"d2d1.lib")		//	direct2d に必要
#pragma comment(lib,"dwrite.lib")	//	文字表示に必要

#define MAX_LOADSTRING 100

#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if(o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)	if(o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if(o){ delete [] (o); o = NULL; }


//!	関数WndProcのプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


//!	グローバル変数
ID2D1Factory			*g_pD2DFactory = NULL;		//	D2D1Factoryオブジェクト
ID2D1HwndRenderTarget	*g_pRenderTarget = NULL;	//	描画ターゲット

double		g_dblDenominator;
double		g_dblFrame;
__int64		g_i64OldTimer;
#define FPS 30.0
#define INTERVAL (1.0/FPS)

Selector	*g_pSelector = NULL;	//	ゲームシステムオブジェクト

/**
* @fn
* @brief	アプリケーションのエントリーポイント
*/
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR	   lpCmdLine,
					 int	   nCmdShow)
{
	//! デバッグ用のフラグセット
	//--- メモリデバッグを行うために使用される _Crt ライブラリの関数
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//--- COM（Component Object Model）を初期化するための関数
	CoInitialize(NULL);		// ここからCOM使用可

	//! 変数宣言
	WNDCLASSEX wcex;	//	ウィンドウクラス構造体
	HWND	hWnd;		//	ウィンドウハンドル
	RECT	bounds, client;	//	RECT構造体


	/*********************
		⑴初期化部分	
	*********************/

	//	⑴-a	ウィンドウクラスの登録
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;	//	ウィンドウプロシージャの登録
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;	//	アプリケーションインスタンス
	wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + WINDOW_BG_COLOR_OFFSET);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("My2DAction");	//	ウィンドウクラス名
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wcex);


	//	⑴-b ウィンドウの生成
	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wcex.lpszClassName, _T("My2DAction"),
					WS_VISIBLE|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX,
					CW_USEDEFAULT, WINDOW_POS_Y, WINDOW_WIDTH_SIZE, WINDOW_HEIGHT_SIZE, NULL, NULL, hInstance, NULL);
	if (!hWnd)
		return FALSE;

	// ウィンドウサイズの調整
	GetWindowRect(hWnd, &bounds); // ウィンドウサイズ	:枠を含んだサイズ
	GetClientRect(hWnd, &client); // クライアントサイズ	:枠を含まない表示領域
	MoveWindow(hWnd, bounds.left, bounds.top,
		WINDOW_WIDTH_SIZE * 2 - client.right,		// ウィンドウサイズに枠を足す->クライアントサイズを狙ったサイズにできる
		WINDOW_HEIGHT_SIZE * 2 - client.bottom,
		false);

	// ウィンドウの再表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//	再表示の際、WndProcが呼び出されることに注意！


	//	⑴-c Direct2Dの初期化
	{
		HRESULT hr;
		//	Direct2D Factory の生成
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
		GetClientRect(hWnd, &client);
		D2D1_SIZE_U size = D2D1::SizeU(
			client.right - client.left,
			client.bottom - client.top
		);

		//	Direct2D 描画ターゲット の生成
		hr = g_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, size),
			&g_pRenderTarget
		);
	}


	//	⑴-d タイマーの初期化
	::QueryPerformanceCounter((LARGE_INTEGER*)&g_i64OldTimer);
	__int64		i64Tmp;
	::QueryPerformanceFrequency((LARGE_INTEGER*)&i64Tmp);
	g_dblDenominator = DOUBLE_ONE / (double)i64Tmp;
	g_dblFrame = 0.0f;

	//	ゲームシステム初期化
	g_pSelector = new Selector(g_pRenderTarget);


	/**************************
		⑵メッセージループ
	**************************/
	MSG		msg;
	while (true) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {	// PeekMessage() イベントの取り出し
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);		//	ショートカット等の変換
			DispatchMessage(&msg);		//	イベントの配信。WndProc() 呼び出し
		}
		else {

			double	t;
			::QueryPerformanceCounter((LARGE_INTEGER*)&i64Tmp);
			t = (i64Tmp - g_i64OldTimer) * g_dblDenominator;
			g_i64OldTimer = i64Tmp;
			g_dblFrame += t;

			if (g_dblFrame >= INTERVAL) {
				int c = (int)(g_dblFrame / INTERVAL);
				g_dblFrame -= INTERVAL * c;

				if (g_pSelector)
					g_pSelector->doAnim();

				InvalidateRect(hWnd, NULL, false);	//	書き換えの実行
			}
		}
	}



	/**************************
			⑶終了処理
	**************************/

	SAFE_DELETE(g_pSelector);
	SAFE_RELEASE(g_pRenderTarget);
	SAFE_RELEASE(g_pD2DFactory);

	return (int)msg.wParam;
}


/**
* @fn
* @brief	ウィンドウに渡されたイベントのハンドラ
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message) {

	case WM_PAINT:
		if (g_pRenderTarget) {
			g_pRenderTarget->BeginDraw();	//	描画開始

			g_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());	// Transform の設定

			g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkBlue));	//	画面のクリア

			//	この中に描画処理を書く
			
			if (g_pSelector)
				g_pSelector->doDraw(g_pRenderTarget);

			//	この中に描画処理を書く

			g_pRenderTarget->EndDraw();		//	描画終了
		}
		ValidateRect(hWnd, NULL);
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}