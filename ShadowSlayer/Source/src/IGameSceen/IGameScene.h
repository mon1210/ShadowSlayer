/**
* @file IGameScene.h
* @brief シーンの基底クラス IGameScene の宣言
*/
/*
*同じヘッダファイルが複数回インクルードされることによる問題を防ぐためのプリプロセッサテクニック
*/
#ifndef __IGAMESCENE_H__
#define __IGAMESCENE_H__

enum GameSceneResultCode;
struct ID2D1RenderTarget;

class IGameScene
{
public:
	virtual ~IGameScene() = 0;
	virtual GameSceneResultCode move() = 0;		// アニメーションメソッド　Title, Stage, GameOverで定義
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;	// 描画メソッド
};

#endif __IGAMESCENE_H__

