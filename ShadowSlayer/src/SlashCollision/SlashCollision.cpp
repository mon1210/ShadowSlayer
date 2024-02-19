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

/**
* @brief SlashCollisionのコンストラクタ
* @param[in] attackCount	何段目攻撃かのカウンタ(4:スライディング攻撃)
* @param[in] faceToRight	向き判定フラグ　	true：右向き  / false：左向き
* @param[in] shadowFlag		シャドウ判定フラグ　非0：シャドウ /	0：ノーマル	　bit演算で複数のフラグを保存するためint型
*/
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


/**
* @brief SlashCollisionの生死
* @note	 生存時間を過ぎると消滅
* return true:生存 / false:死亡
*/
bool SlashCollision::move() {
	m_iLifeTime++;
	m_fX = m_pPlayer->getX() + m_fXOffSet;
	m_fY = m_pPlayer->getY() + m_fYOffSet;

	if (m_iLifeTime > SLASH_LIFE_TIME)
		return false;

	return true;
}


/**
* @brief 描画メソッド
*/
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


/**
* @brief 矩形との当たり判定メソッド
* @param[in] x 左上のx座標
* @param[in] y 左上のy座標
* @param[in] w 矩形のWidth
* @param[in] h 矩形のHeight
* @return true:当たり / false:外れ
*/
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


/**
* @brief 他ゲームオブジェクトとの当たり判定メソッド
* @param[in] *pObj 他ゲームオブジェクト
* @return true:当たり / false:外れ
*/
bool SlashCollision::collide(IGameObject *pObj) { 
	return true;
}


/**
* @brief マップとの当たり判定メソッド
* @note	ここで当たり判定に従って(x, y)座標の更新を行う
*		X方向とY方向で分離して判定。
*		ポイント①: それぞれ加速度の正負で左右、上下に判定を分ける
*		ポイント②: 判定する方向のみ座標を移動させ、判定しない方向は移動前の座標を使って判定を行う
*		ポイント③: X方向の判定は加速度0の時は行わない。加速度0の時も含めてしまうとジャンプ時や落下時に壁に引っかかる。
*/
bool SlashCollision::collideWithMap() { return true; }


/**
* @brief ダメージメソッド
*/
int SlashCollision::damage(float amount) {
	m_iLifeTime = SLASH_DAMAGE_LIFE_TIME;
	return SLASH_RETURN_DAMAGE;
}


/**
* @fn
* @brief	共有メディアファイルの読み込み
* @note		シーン開始時などに呼び出すようにする
*/
void SlashCollision::Restore(Player *pPlayer) {
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
}


/**
* @fn
* @brief	共有メディアファイルの読み込み
* @note		シーン開始時などに呼び出すようにする
*/
#ifdef _DEBUG
void SlashCollision::Restore(Player *pPlayer, ID2D1RenderTarget *pRT) {
	//--- SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
	SAFE_RELEASE(m_pRed);
	m_pPlayer = NULL;
	m_pPlayer = pPlayer;
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRed);
}
#endif


/**
* @fn
* @brief	共有メディアファイルの消去
* @note		シーン削除時などに呼び出すようにする
*/
void SlashCollision::Finalize() {
	m_pPlayer = NULL;

#ifdef _DEBUG
	SAFE_RELEASE(m_pRed);
#endif
}