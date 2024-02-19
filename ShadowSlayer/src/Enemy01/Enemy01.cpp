// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
// ゲーム内のステージ関連クラスと列挙型の定義
#include "..\Stage\Stage.h"
// ゲームの背景を管理するクラスと関連データを定義
#include "..\BG\BG.h"
// ゲーム内の敵キャラクタークラスと関連する定数・列挙型の定義
#include "Enemy01.h"
// テクスチャとビットマップの読み込みと管理に関するクラスとインターフェースを定義
#include "..\TextureLoader\TextureLoader.h"
// 敵の攻撃とプレイヤーの当たり判定処理を行うクラスと関連する定数・列挙型の定義
#include "..\EnemyAttackCollision\EnemyAttackCollision.h"
// 定数値を定めたヘッダファイル
#include "..\Constants.h"
// enum変数宣言用フォルダ
#include "..\Enums.h"


ID2D1Bitmap *Enemy01::m_pTexture = NULL;
ID2D1Bitmap *Enemy01::m_pTexture_Left = NULL;
BG	*Enemy01::m_pBG = NULL;


// TextureにIndexを付与・指定
// @note: X:0～7,Y:8～10
float Enemy01::m_fTextureCoord[] = {
	//	X
	0, 32.f, 64.f, 96.f, 128.f, 160.f, 192.f, 224.f,
	//	Y
	0, 25.f, 50.f
};


#ifdef _DEBUG
ID2D1SolidColorBrush *Enemy01::m_pWhite = NULL;
#endif


/**
* @brief Enemy01のコンストラクタ
* @param[in] x	エネミーのx座標
* @param[in] y	エネミーのy座標
*/
Enemy01::Enemy01(Stage *pParent, float x, float y)
{
	m_pParent = pParent;
	m_fX = x;
	m_fY = y;
	m_fY_prev = m_fY;
	m_fVX = 0;
	m_fVY = 0;


	m_bAttacked = false;
	m_bFaceToRight = false;
	m_bIs_OnSlope = false;

	m_iCount = 0;
	m_iEnemyStateFlag |= EFG_IDLE;
}


// デストラクタ
Enemy01::~Enemy01()
{
}


/**
* @brief Enemy01のアニメーション
* @note	 ダメージを受けると死亡
* return true:生存 / false:死亡
*/
bool Enemy01::move() {
	// 挙動カウンタ(テクスチャ等も管理)
	m_iCount++;
	// プレイヤーとの座標の差
	m_fDiffX = m_fX - m_pParent->m_fPlayerX;
	m_fDiffY = m_fY - m_pParent->m_fPlayerY;
	// プレイヤーとの距離ベクトル
	float l = m_fDiffX * m_fDiffX + m_fDiffY * m_fDiffY;

	// 状態に合わせて処理
	switch (m_iEnemyStateFlag) {
	case EFG_IDLE:
		if (m_iCount > ENEMY_MOVE_ABLE_TIMER && l < ENEMY01_SEARCH_DISTANCE * ENEMY01_SEARCH_DISTANCE) {
			m_iEnemyStateFlag |= EFG_MOVE;	// 動けるタイマー且つプレイヤーとの距離ベクトルがエネミーの視野ベクトル以下ならMOVE
			m_iCount = 0;
		}
		break;

	case EFG_MOVE:
		if (m_fDiffX > 0) {		// 左向き
			m_fVX = -MOVE_SPEED;
			m_bFaceToRight = false;
		}
		else {					// 右向き
			m_fVX = MOVE_SPEED;
			m_bFaceToRight = true;
		}
		// プレイヤーとの距離ベクトルがエネミーの攻撃範囲ベクトル以下でATTACK
		if (l <= ENEMY01_ATTACK_DISTANCE * ENEMY01_ATTACK_DISTANCE) {
			m_iEnemyStateFlag = 0;
			m_iEnemyStateFlag |= EFG_ATTACK;
			m_fVX = 0;		// 停止
			m_iCount = 0;	// カウンタ初期化
		}
		break;

	case EFG_ATTACK:
		if (m_iCount == AT_COLLISION_GENERATE_COUNT) {	// AttackTextureで当たり判定生成
			IGameObject *pObj = new EnemyAttackCollision(m_fX, m_fY, AT_COLLISION_POS_X, AT_COLLISION_WIDTH, AT_COLLISION_HEIGHT, COLLISION_LIFE_TIME, m_bFaceToRight);
			m_pParent->addEnemyAttacks(pObj);
		}
		if (m_iCount > AT_FINISH_COUNT) {		// AttackEndTextureで初期化処理
			m_iEnemyStateFlag = 0;
			m_iCount = 0;
		}
		break;

	case EFG_JUMP:	// 落下時処理
		m_fVY = m_fY - m_fY_prev + GRAVITY;
		if (m_fVY > FALL_SPEED_MAX) {		// 落下速度制限
			m_fVY = FALL_SPEED_MAX;
		}

		m_fY_prev = m_fY;
		break;

	case EFG_DEATH:
		m_fVX = 0;	// 停止
		if (m_iCount >= EnemyTexture::Death_Y)	// 死亡処理終了
			return false;
		break;

	default:
		break;
	}
	
	return true;
}


/**
* @brief 描画メソッド
*/
void Enemy01::draw(ID2D1RenderTarget *pRenderTarget) {

	D2D1_RECT_F rc, 	// 描画領域(画面上での位置やサイズ)を指定する変数
				src;	// テクスチャの中での座標を指定し、テクスチャを決定する変数
	int Xindex = 0, Yindex = 0;

	// 描画領域の設定
	rc.left = m_pParent->m_fPlayerDrawX + 
		(m_fX - m_pParent->m_fPlayerX) - DRAW_POS_X;	// 描画領域の左上隅指定
	rc.top = m_fY - DRAW_POS_Y;							//			〃
	rc.right = rc.left + DRAW_WIDE;		// 描画領域の右下隅指定
	rc.bottom = m_fY;					//			〃

	// エネミーの状態に合わせて描画処理
	switch (m_iEnemyStateFlag) {
	case EFG_IDLE:
	case EFG_JUMP:
		Xindex = (m_iCount % AllMoveTexture) >> 2;		// ( >> 2 == * 4 )	アニメーションループ
		Yindex = EnemyTexture::IdleAndMove_Y;
		break;

	case EFG_MOVE:
		Xindex = EnemyTexture::Idle_X + ((m_iCount % AllMoveTexture) >> 2);		// ( >> 2 == * 4 )	アニメーションループ
		Yindex = EnemyTexture::IdleAndMove_Y;
		break;

	case EFG_ATTACK:
		if (m_iCount < 2) {	// 攻撃前
			Xindex = EnemyTexture::Default_X;		// @修正箇所
		}
		else if (m_iCount < AT_COLLISION_GENERATE_COUNT) {	// 攻撃構え
			Xindex = EnemyTexture::AttackStance_X;
		}
		else {	// 攻撃動作
			Xindex = (m_iCount >> 2); // ( >> 2 == * 4 )	アニメーションループ
		}
		Yindex = EnemyTexture::Attack_Y;
		break;

	case EFG_DEATH:
		Xindex = m_iCount >> 1; // ( >> 2 == * 4 )	アニメーションループ
		Yindex = EnemyTexture::Death_Y;
		break;

	default:
		break;
	}

	float tx, ty;
	// Texture代入
	tx = m_fTextureCoord[Xindex];
	ty = m_fTextureCoord[Yindex];

	src.top = ty;	// テクスチャ座標の左上隅指定用の情報代入
	src.bottom = src.top + ENEMY_TEXTURE_HEIGHT;	// テクスチャ座標の右下隅指定用の情報代入

	if (!(m_bFaceToRight)) {	// 左向き
		src.left = tx;		// テクスチャ座標の左上隅指定
		src.right = tx + ENEMY_TEXTURE_WIDTH;		// テクスチャ座標の右上隅指定
		pRenderTarget->DrawBitmap(m_pTexture_Left, rc, ENEMY_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
	}
	else {		// 右向き
		src.left = ENEMY_LA_TEXTURE_COORD - tx;		// テクスチャ座標の左上隅指定
		src.right = src.left + ENEMY_TEXTURE_WIDTH;			// テクスチャ座標の右下隅指定
		pRenderTarget->DrawBitmap(m_pTexture, rc, ENEMY_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);
	}

// デバッグ用当たり判定描画
#ifdef _DEBUG
	rc.left += ENEMY_COLLISION_OFFSET_L;	// 描画領域の左上隅指定
	rc.top += ENEMY_COLLISION_OFFSET_T;		//			〃
	rc.right = rc.left + ENEMY_TEXTURE_WIDTH;	// 描画領域の右下隅指定
	pRenderTarget->DrawRectangle(rc, m_pWhite);
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
bool Enemy01::collide(float x, float y, float w, float h) {

	if (m_fX - ENEMY_TEXTURE_W_HALF > x + w)
		return false;
	if (m_fX + ENEMY_TEXTURE_W_HALF < x)
		return false;
	if (m_fY < y)
		return false;
	if (m_fY - ENEMY_TEXTURE_H_HALF > y + h)
		return false;

	return true;
}


/**
* @brief 他ゲームオブジェクトとの当たり判定メソッド
* @param[in] *pObj 他ゲームオブジェクト
* @return true:当たり / false:外れ
*/
bool Enemy01::collide(IGameObject *pObj) {

	return pObj->collide(m_fX - ENEMY_TEXTURE_W_HALF, m_fY - ENEMY_TEXTURE_HEIGHT, ENEMY_TEXTURE_WIDTH, ENEMY_TEXTURE_H_HALF);
}


/**
* @brief マップとの当たり判定メソッド
* @note	ここで当たり判定に従って(x, y)座標の更新を行う
*		X方向とY方向で分離して判定。
*		ポイント①: それぞれ加速度の正負で左右、上下に判定を分ける
*		ポイント②: 判定する方向のみ座標を移動させ、判定しない方向は移動前の座標を使って判定を行う
*		ポイント③: X方向の判定は加速度0の時は行わない。加速度0の時も含めてしまうとジャンプ時や落下時に壁に引っかかる。
*/
bool Enemy01::collideWithMap() {
	float l = m_fX - ENEMY_TEXTURE_W_HALF, t = m_fY - ENEMY_TEXTURE_H_HALF, w = ENEMY_TEXTURE_WIDTH, h = ENEMY_TEXTURE_H_HALF;
	float r = l + ENEMY_TEXTURE_WIDTH, b = m_fY;

	//	坂道に入ったか、出たかを判定
	//	坂のベクトルを計算するメソッドは、坂に入ったときにだけ呼ばれるようにしてある(ボタン連打防止と似たやり方)
	if (!m_bIs_OnSlope) {
		if (m_pBG->tile_code(m_fX, t + h) > SLOPE_BETWEEN_TEXTURE_NUMBER) {
			m_pBG->getStepVector(m_fX, t + h, &step);
			m_bIs_OnSlope = true;
		}
	}
	else if (m_fX > step.x + step.vx || m_fX < step.x) {
		m_bIs_OnSlope = false;
		step.x = 0, step.y = 0;
		step.vx = 0, step.vy = 0;
	}


	//	平地での判定-----------------------------------------------------------------------------------------
	//	坂道を構成するマップチップとの判定は取らないようにしている
	if (!m_bIs_OnSlope) {
		if (m_fVY >= 0) {	// ジャンプ、階段上り中
			int code1 = m_pBG->tile_code(l + 2, t + h + m_fVY);		// +2 / -2 することで、ブロック隣接時にジャンプすると上に乗ってしまう現象を防げる
			int code2 = m_pBG->tile_code(l + w - 2, t + h + m_fVY);
			// 床と判定
			if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER)) { 
				m_iEnemyStateFlag &= ~EFG_JUMP;
				int row = (int)(b + m_fVY) >> MAP_CHIP_SIZE_BITS;
				m_fY = (float)(row << MAP_CHIP_SIZE_BITS);
				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {	//	空中時
				m_iEnemyStateFlag = 0;
				m_iEnemyStateFlag |= EFG_JUMP;
				m_fY += m_fVY;
			}
		}

		if (m_fVX > 0) {	// 右移動
			int code1 = m_pBG->tile_code(l + w + m_fVX, t);
			int code2 = m_pBG->tile_code(l + w + m_fVX, t + h - 4);	// -4 することで地面と判定が出てしまうことを防げる
			// 右側の判定
			if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER)) {
				int col = (int)(l + w + m_fVX) >> MAP_CHIP_SIZE_BITS;
				m_fX = (float)((col << MAP_CHIP_SIZE_BITS) - ((int)w >> 1));
			}
			else {
				m_fX += m_fVX;
			}
		}
		else if (m_fVX < 0) {	// 左移動
			int code1 = m_pBG->tile_code(l + m_fVX, t);
			int code2 = m_pBG->tile_code(l + m_fVX, t + h - 4); // -4 することで地面と判定が出てしまうことを防げる
			// 左側の判定
			if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER)) {
				int col = (int)(l + m_fVX) >> MAP_CHIP_SIZE_BITS;
				m_fX = (float)((col << MAP_CHIP_SIZE_BITS) + ((int)w >> 1) + iMAP_CHIP_WIDTH);
			}
			else {
				m_fX += m_fVX;
			}
		}
	}
	//	坂道での判定-----------------------------------------------------------------------------------------
	else {

		if (m_fVY >= 0) {	// ジャンプ、階段上り中
			float stepY, ratio;
			ratio = (m_fX - step.x) / step.vx;
			stepY = step.y + step.vy * ratio;
			if (m_fY + m_fVY + 5.f >= stepY) {	//	+5.f しておくと下りの時にガタガタ跳ねなくなる
				m_iEnemyStateFlag &= ~EFG_JUMP;
				m_fY = stepY;
				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {		// 空中時
				m_iEnemyStateFlag |= EFG_JUMP;
				m_fY += m_fVY;
			}
		}

		m_fX += m_fVX;

	}

	return true;
}


/**
* @brief ダメージメソッド
*/
int Enemy01::damage(float amount) {
	m_iEnemyStateFlag = 0;
	m_iEnemyStateFlag |= EFG_DEATH;		// 死亡状態へ
	m_iCount = DELETE_TEXTURE_COUNT;	// 描画カウント削除
	return 1;
}


/**
* @fn
* @brief	共有メディアファイルの読み込み
* @note		シーン開始時などに呼び出すようにする
*/
void Enemy01::Restore(ID2D1RenderTarget *pRT, BG *pBG) {
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexture_Left);
	m_pBG = NULL;

	// 画像をメモリに読み込む
	TextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\slime_32x25.png"), &m_pTexture);
	TextureLoader::CreateD2D1BitmapFromFile(pRT, _T("res\\slime_left_32x25.png"), &m_pTexture_Left);
	m_pBG = pBG;

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
void Enemy01::Finalize() {
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexture_Left);
	m_pBG = NULL;

#ifdef _DEBUG
	SAFE_RELEASE(m_pWhite);
#endif
}