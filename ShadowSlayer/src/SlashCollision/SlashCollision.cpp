// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// スラッシュコリジョン（プレイヤーの攻撃の当たり判定）クラスの宣言
#include "SlashCollision.h"
// プレイヤーキャラクターとその関連情報を定義したクラスと定数/列挙型の定義
#include "..\Player\Player.h"
// 定数値を定めたヘッダファイル
#include "..\Constants.h"
// enum変数宣言用フォルダ
#include "..\Enums.h"


//--- ヘッダガードを使って二度のインクルードを防ぐ
#ifdef _DEBUG
#include <d2d1.h>
#endif


Player *SlashCollision::m_pPlayer = NULL;

#ifdef _DEBUG
ID2D1SolidColorBrush *SlashCollision::m_pRed = NULL;
#endif


// コンストラクタ
SlashCollision::SlashCollision(int attackCount, bool faceToRight, int shadowFlag)
{
	float shadowLength = SHADOW_START_LENGTH;
	m_fDamage = SLASH_DAMAGE;
	//	@修正箇所
	if (shadowFlag != 0) {	// シャドウ状態(true)
		shadowLength = SHADOW_ON_LENGTH;
		m_fDamage = SHADOW_SLASH_DAMAGE;
	}

	m_bFaceToRight = faceToRight;
	m_iAttackCount = attackCount;

	switch (m_iAttackCount) {
	// 1.2段目 ----------------------------------------
	case First:
	case Second:
		if (m_bFaceToRight) {	//右向き
			m_fXOffSet = R_SLASH_OFFSET_X;
		}
		else {		//左向き
			m_fXOffSet = L_SLASH_OFFSET_X;
		}
		m_fYOffSet = SLASH_OFFSET_Y;
		m_fW = SLASH_WIDTH * shadowLength;
		m_fH = SLASH_HEIGHT * shadowLength;
		break;

	// 3段目 ------------------------------------------
	case Third:
		if (m_bFaceToRight) {	//右向き
			m_fXOffSet = R_THIRD_SLASH_OFFSET_X;
		}
		else {		//左向き
			m_fXOffSet = L_THIRD_SLASH_OFFSET_X;
		}
		m_fYOffSet = THIRD_SLASH_OFFSET_Y;
		m_fW = SLASH_WIDTH3 * shadowLength;
		m_fH = SLASH_HEIGHT3 * shadowLength;
		break;

	// スライディング ---------------------------------
	case Slide:
		if (m_bFaceToRight) {	//右向き
			m_fXOffSet = R_DASH_SLASH_OFFSET_X;
		}
		else {		//左向き
			m_fXOffSet = L_DASH_SLASH_OFFSET_X;
		}
		m_fYOffSet = DASH_SLASH_OFFSET_Y;
		m_fW = SLASH_WIDTH4 * shadowLength;
		m_fH = SLASH_HEIGHT4 * shadowLength;
		break;
	default:
		if (m_bFaceToRight) {	//右向き
			m_fXOffSet = R_SLASH_OFFSET_X;
		}
		else {		//左向き
			m_fXOffSet = L_SLASH_OFFSET_X;
		}
		break;
	}
	
	
	// 当たり判定削除
	m_iLifeTime = INVINCIBLE_TIME;	// @修正箇所
}


// デストラクタ
SlashCollision::~SlashCollision()
{
}


// SlashCollisionの生死
bool SlashCollision::move() {
	m_iLifeTime++;
	m_fX = m_pPlayer->getX() + m_fXOffSet;
	m_fY = m_pPlayer->getY() + m_fYOffSet;

	if (m_iLifeTime > SLASH_LIFE_TIME)
		return false;

	return true;
}


// 描画メソッド
void SlashCollision::draw(ID2D1RenderTarget *pRenderTarget) {
#ifdef _DEBUG
	D2D1_RECT_F rc;		// 描画領域(画面上での位置やサイズ)を指定する変数
	rc.left = m_pPlayer->m_fDrawX + (m_fX - m_pPlayer->getX());		// 描画領域の左上隅指定
	rc.top = m_pPlayer->m_fDrawY + (m_fY - m_pPlayer->getY());		//			〃
	rc.right = rc.left + m_fW;		// 描画領域の右下隅指定
	rc.bottom = rc.top + m_fH;		//			〃
	pRenderTarget->DrawRectangle(rc, m_pRed);
#endif
}


// 矩形との当たり判定メソッド
bool SlashCollision::collide(float x, float y, float w, float h) { 

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
bool SlashCollision::collide(IGameObject *pObj) { 
	return true;
}


// マップとの当たり判定メソッド
bool SlashCollision::collideWithMap() { return true; }


// ダメージメソッド
int SlashCollision::damage(float amount) {
	m_iLifeTime = SLASH_DAMAGE_LIFE_TIME;
	return SLASH_RETURN_DAMAGE;
}


// 共有メディアファイルの読み込み
void SlashCollision::Restore(Player *pPlayer) {
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
}


// 共有メディアファイルの読み込み
#ifdef _DEBUG
void SlashCollision::Restore(Player *pPlayer, ID2D1RenderTarget *pRT) {
	//--- SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
	SAFE_RELEASE(m_pRed);
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRed);
}
#endif


// 共有メディアファイルの消去
void SlashCollision::Finalize() {
	m_pPlayer = NULL;

#ifdef _DEBUG
	SAFE_RELEASE(m_pRed);
#endif
}