/**
* @file IGameObject.cpp
* @brief クラス IGameObject の実装
*/

// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// ゲームオブジェクトの共通インターフェースと関連するマクロ定義
#include "IGameObject.h"

/**
* @brief 純粋仮想デストラクタの実体
* @details デストラクタだけは用意しなければ、サブクラスがデストラクタを呼べない
*/
IGameObject::~IGameObject()
{
}
