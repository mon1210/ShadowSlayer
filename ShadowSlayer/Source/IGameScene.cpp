/**
* @file IGameScene.cpp
* @brief シーンの基底クラス IGameScene の実装
*/

// Windowsアプリケーション開発用の共通ヘッダファイル
#include "stdafx.h"
// ゲームシーンの共通インターフェースと関連するプリプロセッサテクニック
#include "IGameScene.h"

/**
* @brief 純粋仮想デストラクタの実体
* @details デストラクタだけは用意しなければ、サブクラスがデストラクタを呼べない
*/
IGameScene::~IGameScene()
{
}
