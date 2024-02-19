// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
// 弾クラスの宣言
#include "Shot.h"
// ゲーム内のステージ関連クラスと列挙型の定義
#include "..\Stage\Stage.h"
// テクスチャとビットマップの読み込みと管理に関するクラスとインターフェースを定義
#include "..\TextureLoader\TextureLoader.h"
// ゲームの背景を管理するクラスと関連データを定義
#include "..\BG\BG.h"
// 定数値を定めたヘッダファイル
#include "..\Constants.h"
// enum変数宣言用フォルダ
#include "..\Enums.h"


ID2D1Bitmap *Shot::m_pTexture = NULL;
ID2D1Bitmap *Shot::m_pTexture_Left = NULL;
BG	*Shot::m_pBG = NULL;
#ifdef _DEBUG
	ID2D1SolidColorBrush *Shot::m_pWhite = NULL;
#endif


/**
* @brief Shotのコンストラクタ
* @param[in] x	プレイヤーのx座標
* @param[in] y	プレイヤーのy座標
* @param[in] direction	1 or -1
*/
Shot::Shot(Stage *pParent, float x, float y, float direction)
{
	m_pParent = pParent;
	m_fX = x + SHOTOFFSETX * direction;
	m_fY = y - SHOTOFFSETY;
	m_fVX = BULLETSPEED * direction;
	m_fVY = 0;
	m_fDirection = direction;
	m_iLifeTimer = -1;
	m_iDeathTimer = -1;
	m_bDamage = false;

}


// デストラクタ
Shot::~Shot()
{
}


/**
* @brief shotのアニメーション
* @note	敵やマップにぶつかるか、画面外に出ると消滅
* return true:生存 / false:死亡
*/
bool Shot::move() {
	if (!m_bDamage) {	// 当たっていないとき
		m_iLifeTimer++;

		m_fX += m_fVX;

		if (m_iLifeTimer > SHOT_LIFE_TIME) {
			m_bDamage = true;
		}
	}
	else {	// 当たったら
		m_iDeathTimer++;
		if (m_iDeathTimer > SHOT_DESTROY_TIME) {
			return false;
		}
	}
	

	return true;
}


/**
* @brief 描画メソッド
*/
void Shot::draw(ID2D1RenderTarget *pRenderTarget) {
	
	float tx, ty;
	D2D1_RECT_F rc,		// 描画領域(画面上での位置やサイズ)を指定する変数
				src;	// テクスチャの中での座標を指定し、テクスチャを決定する変数

	// 描画領域の設定　　プレイヤーを起点に行う		
	rc.left = (m_pParent->m_fPlayerDrawX) + (m_fX - m_pParent->m_fPlayerX) - SHOT_RECTANGLE_HALF_SIZE;	// 描画領域の左上隅指定
	rc.top = (m_pParent->m_fPlayerDrawY) + (m_fY - m_pParent->m_fPlayerY) - SHOT_RECTANGLE_HALF_SIZE;	//　		〃
	rc.right = rc.left + SHOT_RECTANGLE_SIZE;		// 描画領域の右下隅指定
	rc.bottom = rc.top + SHOT_RECTANGLE_SIZE;		//			〃	


	if (!m_bDamage) {	// 当たっていない

		if (m_iLifeTimer < HitStart) {	// アニメーション変化しつつ飛んでいる
			tx = m_iLifeTimer >> 1;
		}
		else {	// 飛んでいる状態
			tx = ProjectileFlying;
		}


		tx *= SHOT_TEXTURE_WIDTH;
		ty = 0.f;


		if (m_fVX > 0) {	// 右向き
			src.left = tx;		// テクスチャ座標の左上隅指定
			src.top = ty;		//				〃
			src.right = src.left + SHOT_TEXTURE_WIDTH;		// テクスチャ座標の右下隅指定
			src.bottom = src.top + SHOT_TEXTURE_HEIGHT;		//				〃
			pRenderTarget->DrawBitmap(m_pTexture, rc, SHOT_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
		}
		else {		// 左向き
			src.left = SHOT_LASTTEXTURE_POS - tx;		// テクスチャ座標の左上隅指定
			src.top = ty;								//				〃
			src.right = src.left + SHOT_TEXTURE_WIDTH;		// テクスチャ座標の右下隅指定
			src.bottom = src.top + SHOT_TEXTURE_HEIGHT;		//				〃
			pRenderTarget->DrawBitmap(m_pTexture_Left, rc, SHOT_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
		}

#ifdef _DEBUG
		// デバッグ時当たり判定表示
		rc.left += SHOT_COLLISION_OFFSET_W;			// 描画領域の左上隅指定
		rc.top = m_fY - SHOT_COLLISION_OFFSET_T;	//			〃
		rc.right -= SHOT_COLLISION_OFFSET_W;			// 描画領域の右下隅指定
		rc.bottom = m_fY + SHOT_COLLISION_OFFSET_B;		//			〃 
		pRenderTarget->DrawRectangle(rc, m_pWhite);
#endif
	}
	else {		// 当たった
		tx = StartDestroy + (m_iDeathTimer >> 1);
		ty = SHOT_TEXTURE_HEIGHT * ((int)tx / SRowPerElement);
		tx = SHOT_TEXTURE_WIDTH * ((int)tx % SRowPerElement);
		
		if (m_fVX > 0) {	// 右向き
			src.left = tx;		// テクスチャ座標の左上隅指定
			src.top = ty;		//				〃
			src.right = src.left + SHOT_TEXTURE_WIDTH;		// テクスチャ座標の右下隅指定
			src.bottom = src.top + SHOT_TEXTURE_HEIGHT;		//				〃
			pRenderTarget->DrawBitmap(m_pTexture, rc, SHOT_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
		}
		else {		// 左向き
			src.left = SHOT_LASTTEXTURE_POS - tx;	// テクスチャ座標の左上隅指定
			src.top = ty;							//				〃
			src.right = src.left + SHOT_TEXTURE_WIDTH;		// テクスチャ座標の右下隅指定
			src.bottom = src.top + SHOT_TEXTURE_HEIGHT;		//				〃
			pRenderTarget->DrawBitmap(m_pTexture_Left, rc, SHOT_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
		}
	}

}


/**
* @brief 矩形との当たり判定メソッド
* @param[in] x 左上のx座標
* @param[in] y 左上のy座標
* @param[in] w 矩形のWidth
* @param[in] h 矩形のHeight
* @return true:当たり / false:外れ
*/
bool Shot::collide(float x, float y, float w, float h) {
	return true;
}


/**
* @brief 他ゲームオブジェクトとの当たり判定メソッド
* @param[in] *pObj 他ゲームオブジェクト
* @return true:当たり / false:外れ
*/
bool Shot::collide(IGameObject* pObj) {
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
bool Shot::collideWithMap() {
	if (m_fVX > 0) {		// 右向き	
		if (0 != m_pBG->tile_code(m_fX + SHOT_COLLISION_HEIGHT, m_fY)) {
			damage(SHOT_DAMAGE);
		}
	}
	else {		// 左向き
		if (0 != m_pBG->tile_code(m_fX - SHOT_COLLISION_HEIGHT, m_fY)) {
			damage(SHOT_DAMAGE);
		}
	}

	return true;
}


/**
* @brief ダメージメソッド
*/
int Shot::damage(float amount) {
	m_bDamage = true;
	return 1;
}


/**
* @fn
* @brief	共有メディアファイルの読み込み
* @note		シーン開始時などに呼び出すようにする
*/
void Shot::Restore(ID2D1RenderTarget *pRT, BG *pBG) {
	m_pBG = NULL;
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexture_Left);
	m_pBG = pBG;
	TextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\shot_32x32.png"), &m_pTexture);
	TextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\shot_left_32x32.png"), &m_pTexture_Left);

#ifdef _DEBUG
	SAFE_RELEASE(m_pWhite);
	pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhite);
#endif
}


/**
* @fn
* @brief	共有メディアファイルの消去
* @note		シーン削除時などに呼び出すようにする
*/
void Shot::Finalize() {
	m_pBG = NULL;
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexture_Left);

#ifdef _DEBUG
	SAFE_RELEASE(m_pWhite);
#endif
}


/**
 * @brief	BGを設定してShotの環境を準備する
 * @note	ShotのBG環境を設定する => Shotが描画等影響を与えるために必要
 */
void Shot::resetBG(BG *pBG) {
	m_pBG = pBG;
}
