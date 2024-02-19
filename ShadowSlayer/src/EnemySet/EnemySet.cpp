// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// 敵キャラクターセットアップと管理に関するクラスとデータの定義
#include "EnemySet.h"
// ゲーム内の敵キャラクタークラスと関連する定数・列挙型の定義
#include "..\Enemy01\Enemy01.h"


/**
* @brief EnemySetのコンストラクタ
*/
EnemySet::EnemySet(Stage *pParent)
{
	m_iIndex = 0;
	m_pParent = pParent;
}


// デストラクタ
EnemySet::~EnemySet()
{
}


/**
* @fn
* @brief	共有メディアファイルの読み込み
* @note		シーン開始時などに呼び出すようにする
*/
void EnemySet::Restore(ID2D1RenderTarget *pRT, BG *pBG) {
	Enemy01::Restore(pRT, pBG);
}


/**
* @fn
* @brief	共有メディアファイルの消去
* @note		シーン削除時などに呼び出すようにする
*/
void EnemySet::Finalize() {
	Enemy01::Finalize();
}


/*
* @brief エネミー配置メソッド
* @note  引数の数値以下でセットデータを呼び出してエネミーを配置
*/ 
IGameObject* EnemySet::GetEnemyToSet(int x) {
	IGameObject *pObj = NULL;
	int sx, sy;
	if (m_iIndex < m_szSetDaraSize) {
		if (m_pSetData[m_iIndex] <= x) {
			sx = m_pSetData[m_iIndex];		// 一つ目のデータ(X座標)
			sy = m_pSetData[m_iIndex + 1];		// 二つ目のデータ(Y座標)
			switch (m_pSetData[m_iIndex + 2]) {		// 三つ目のデータ(種類)
			case 1:
				pObj = new Enemy01(m_pParent, sx, sy);
				break;
			}
			m_iIndex ++;
			m_iIndex ++;
			m_iIndex ++;	// 改行処理(次のエネミーの情報へ)
		}
	}
	return pObj;
}


/*
*	敵セットデータ
*/

//	出現 X座標, Y座標, 敵種類
SHORT EnemySet::m_pSetData[] = {
	150, 160, 1,
	320, 160, 1,
	610, 160, 1,
	840, 160, 1
};

size_t EnemySet::m_szSetDaraSize = _countof(EnemySet::m_pSetData);