// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// 敵の攻撃とプレイヤーの当たり判定処理を行うクラスと関連する定数・列挙型の定義
#include "EnemyAttackCollision.h"
// プレイヤーキャラクターとその関連情報を定義したクラスと定数/列挙型の定義
#include "..\Player\Player.h"
// 定数値を定めたヘッダファイル
#include "..\Constants.h"


// ヘッダガードを使用して二度のインクルードを防ぐ
#ifdef _DEBUG
#include <d2d1.h>
#endif


Player *EnemyAttackCollision::m_pPlayer = NULL;


#ifdef _DEBUG
ID2D1SolidColorBrush *EnemyAttackCollision::m_pRed = NULL;
#endif


// コンストラクタ
EnemyAttackCollision::EnemyAttackCollision(float x, float y, float w, float colW, float colH, int LifeTime, bool faceToRight)
{
	m_bFaceToRight = faceToRight;
	m_iLifeTime = LifeTime;
	
	m_fW = colW;
	m_fH = colH;

	if (!m_bFaceToRight) {	// 左向き
		m_fX = x - m_fW - w;	// 攻撃主X座標 - 当たり判定横幅(W) - 当たり判定描画座標
	}
	else {		// 右向き
		m_fX = x + w;	// 攻撃主X座標 + 当たり判定描画座標
	}

	m_fY = y - m_fH;	// 攻撃主Y座標 - 当たり判定縦幅(H)


	// 描画カウント削除
	m_iCount = DELETE_TEXTURE_COUNT;
}


// デストラクタ
EnemyAttackCollision::~EnemyAttackCollision()
{
}


// 行動管理
bool EnemyAttackCollision::move() {

	// LifeTimeを超えるとfalseを返す
	if (++m_iCount > m_iLifeTime)
		return false;

	return true;
}


// 描画メソッド
void EnemyAttackCollision::draw(ID2D1RenderTarget *pRenderTarget) {
#ifdef _DEBUG
	D2D1_RECT_F rc;		// 描画領域(画面上での位置やサイズ)を指定する変数
	rc.left = m_pPlayer->m_fDrawX + (m_fX - m_pPlayer->getX());		// 描画領域の左上隅指定
	rc.top = m_fY;													//			〃
	rc.right = rc.left + m_fW;		//  描画領域の右下隅指定
	rc.bottom = rc.top + m_fH;		// 			〃
	pRenderTarget->DrawRectangle(rc, m_pRed);
#endif
}


// 矩形との当たり判定メソッド
bool EnemyAttackCollision::collide(float x, float y, float w, float h) {

	if (m_fX > x + w)
		return false;
	if (m_fX + m_fW < x)
		return false;
	if (m_fY > y + h)
		return false;
	if (m_fY + m_fH < y)
		return false;

	return true;
}


// 他ゲームオブジェクトとの当たり判定メソッド
bool EnemyAttackCollision::collide(IGameObject *pObj) {
	return true;
}


// マップとの当たり判定メソッド
bool EnemyAttackCollision::collideWithMap() { return true; }


// ダメージメソッド
int EnemyAttackCollision::damage(float amount) {
	m_iCount = m_iLifeTime << 1;
	return 1;
}


// 共有メディアファイルの読み込み
void EnemyAttackCollision::Restore(Player *pPlayer) {
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
}


// 共有メディアファイルの読み込み
#ifdef _DEBUG
void EnemyAttackCollision::Restore(Player *pPlayer, ID2D1RenderTarget *pRT) {
	SAFE_RELEASE(m_pRed);
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRed);
}
#endif


// 共有メディアファイルの消去
void EnemyAttackCollision::Finalize() {
	m_pPlayer = NULL;

#ifdef _DEBUG
	SAFE_RELEASE(m_pRed);
#endif
}