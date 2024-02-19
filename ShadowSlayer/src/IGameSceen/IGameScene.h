/**
* @file  IGameScene.h
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
	/**
	* @brief 純粋仮想デストラクタの実体
	* @details デストラクタだけは用意しなければ、サブクラスがデストラクタを呼べない
	*/
	virtual ~IGameScene() = 0;

	/**
	* @brief アニメーションメソッド
	* @details Title, Stage, GameOverで定義
	*/
	virtual GameSceneResultCode move() = 0;

	/**
	* @brief 描画メソッド
	*/
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;
};

#endif __IGAMESCENE_H__

