// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
// プレイヤーキャラクターとその関連情報を定義したクラスと定数/列挙型の定義
#include "Player.h"
// ゲーム内のステージ関連クラスと列挙型の定義
#include "..\Stage\Stage.h"
// テクスチャとビットマップの読み込みと管理に関するクラスとインターフェースを定義
#include "..\TextureLoader\TextureLoader.h"
// ゲームの背景を管理するクラスと関連データを定義
#include "..\BG\BG.h"
// 弾クラスの宣言
#include "..\Shot\Shot.h"
// スラッシュコリジョン（プレイヤーの攻撃の当たり判定）クラスの宣言
#include "..\SlashCollision\SlashCollision.h"
// 定数値を定めたヘッダファイル
#include "..\Constants.h"
// enum変数宣言用フォルダ
#include "..\Enums.h"


/**
* @brief Playerのコンストラクタ
*/
Player::Player(Stage *pParent, BG *pBG)
{
	ID2D1RenderTarget *pTarget = NULL;
	m_pBlackBrush = NULL;
	m_pParent = pParent;
	m_pBG = pBG;
	m_pTexture = NULL;
	m_pTexture_Left = NULL;

	m_bDeath = false;
	m_bDamage = false;
	m_bSlidingTurn = false;
	m_bJump = true;
	m_bAttack = true;
	m_bShot = true;
	m_bShadow = true;
	m_bFaceToRight = true;
	m_bIs_AttackCancel_OK = false;
	m_bIs_OnSlope = false;
	m_iPlayerStateFlag = 0;
	m_iShadowStateFlag = 0;

	m_iFrame = 0;
	m_iIdleFrame = 0;
	m_iAnimFrame = 0;
	m_iSlidingFrame = 0;
	m_iAttackFrame = 24;
	m_iAttackCount = 0;
	m_iSlidingAttackFrame = 0;
	m_iShadowSwitchFrame = 0;

	m_fX = 128.f;//64.f
	m_fY = 320.f;
	m_fY_prev = m_fY;
	m_fVX = 0.f;
	m_fVY = 0.f;
	m_fDrawX = 128.f;//64.f
	m_fDrawY = 320.f;
	m_fOpacity = 1.0f;
	m_fShadowOpacity = 0.5f;

	m_fMapWidth = 0;
	m_fMapHight = 0;
	m_fRBoader = 0;

	m_fPl = PHB_L; m_fPt = PHB_T; m_fPw = PHB_W; m_fPh = PHB_H;
	m_fHP = 100.f;
	m_fMana = 100.f;

	pTarget = m_pParent->GetRenderTarget();
	if (pTarget) {
		TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\player.png"), &m_pTexture);
		TextureLoader::CreateD2D1BitmapFromFile(pTarget, _T("res\\player_L.png"), &m_pTexture_Left);
		pTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush);
		m_pBlackBrush->SetOpacity(m_fShadowOpacity);

#ifdef _DEBUG
		m_pWhiteBrush = NULL;
		pTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhiteBrush);
#endif // _DEBUG

		pTarget->Release();
		pTarget = NULL;
	}
}


// デストラクタ
Player::~Player()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTexture_Left);
	SAFE_RELEASE(m_pBlackBrush);

#ifdef _DEBUG
	SAFE_RELEASE(m_pWhiteBrush);
#endif // _DEBUG

}


/**
* @brief プレイヤーのアニメーションメソッド
* @return true:生存 / false:死亡
* @note ここでは加速度の設定だけ行い、(x, y)座標の更新はcollide() で行う
*/
bool Player::move() {
	if (m_fHP <= 0)
	{
		return false;
	}


	//	フラグ判定 フレームカウント フラグクリア 加速度クリア----------------------
	//	この中でフレームカウントをインクリメントしているので、先頭に置いておかないと m_iSlidingFrame = FPS でも描画処理を通ってしまう。(m_iSlidingFrame < FPS)

	//	shadow化フレームカウント
	if (m_iShadowStateFlag == SFG_SWITCH) {	// Shadowモード切り替え時
		if (m_iShadowSwitchFrame < SHADOW_MODE_COUNT) {
			m_iShadowSwitchFrame++;
			m_fShadowOpacity += SHADOW_DRAW_OPACITY;
			m_fOpacity -= SHADOW_DRAW_OPACITY;
			m_pBlackBrush->SetOpacity(m_fShadowOpacity);
			return true;
		}
		else {
			m_iShadowSwitchFrame = 0;
			m_iShadowStateFlag = 0;
			m_iShadowStateFlag |= SFG_ON;
		}
	}
	else if (m_iShadowStateFlag == SFG_ON) {	// 発動時MANA操作
		m_fMana -= SHADOW_MANA_DEPLETION;
		if (m_fMana < 0) {	// Shadow強制解除
			m_fMana = 0;
			m_iShadowStateFlag |= SFG_SWITCH;
		}
	}
	else if (m_iShadowStateFlag == (SFG_ON | SFG_SWITCH)) {	// Shadow解除時
		if (m_iShadowSwitchFrame < SHADOW_MODE_COUNT) {
			m_iShadowSwitchFrame++;
			m_fShadowOpacity -= SHADOW_DRAW_OPACITY;
			m_fOpacity += SHADOW_DRAW_OPACITY;
			m_pBlackBrush->SetOpacity(m_fShadowOpacity);
			return true;
		}
		else {
			m_iShadowSwitchFrame = 0;
			m_iShadowStateFlag = 0;
		}
	}

	

	{
		UINT check_flag = 0;
		float check_VX = 0;

		switch (m_iPlayerStateFlag) {
		case PFG_IDLE:
			// アニメーションをより滑らかにするための FPS * 2
			m_iIdleFrame = (++m_iIdleFrame) % (FPS * 2);	// Idleアニメーションループ
			m_iAnimFrame = 0;
			break;

		case PFG_DAMAGED:
		case PFG_DAMAGED | PFG_JUMP:
			if (m_iDamagedFrame < PLAYER_DAMAGE_ANIM_TIME) {		// ダメージを受けたアニメーションの再生時間
				check_flag |= PFG_DAMAGED;
				check_VX = m_fVX;
			}
			break;

		case PFG_JUMP:
			// アニメーションをより滑らかにするための FPS * 2
			m_iIdleFrame = (++m_iIdleFrame) % (FPS * 2);
			m_iAnimFrame = 0;
			check_VX = m_fVX;
			break;

		case PFG_WALK:
		case PFG_WALK | PFG_JUMP:
			m_iIdleFrame = 0;
			m_iAnimFrame++;
			m_iAttackFrame = PLAYER_ATTTACK_TIME;
			break;
                                                                                                                                                                                                                      
		case PFG_SLIDE:
		case PFG_SLIDE | PFG_JUMP:
			// スライド中頭上にブロックがあって立ち状態になれないとき、スライディングを継続。フレームカウントも進めない
			if (0 != (m_pBG->tile_code(PHB_L, PHB_T) || m_pBG->tile_code(PHB_L + PHB_W, PHB_T))) {
				check_flag |= (PFG_SLIDE | PFG_SLIDE_CONTINUE);
				check_VX = m_fVX;
			}
			else {
				m_iSlidingFrame++;
				check_VX = m_fVX;
			}
			m_iIdleFrame = 0;
			m_iAnimFrame = 0;
			m_iAttackFrame = PLAYER_ATTTACK_TIME;
			break;

		case PFG_ATTACK:
		case PFG_ATTACK | PFG_JUMP:
			if (m_iAttackFrame < ATTACK_COLLISION_END) {
				m_iAttackFrame++;
				check_flag |= (PFG_ATTACK & m_iPlayerStateFlag);
				if (m_iAttackFrame == ATTACK_COLLISION_GENERATE) {	// 当たり判定生成
					IGameObject *pObj = new SlashCollision(m_iAttackCount, m_bFaceToRight, m_iShadowStateFlag);
					m_pParent->addSlashes(pObj);
				}
			}
			else if (m_iAttackFrame < ATTACK_END) {
				m_iAttackFrame++;
				check_flag |= (PFG_ATTACK & m_iPlayerStateFlag);
				m_bIs_AttackCancel_OK = true;
			}
			else {
				m_bIs_AttackCancel_OK = false;
				m_iAttackCount = 0;
				m_iAttackFrame++;
			}
			m_iIdleFrame = 0;
			m_iAnimFrame = 0;
			break;

		case PFG_SLIDEATTACK:
		case PFG_SLIDEATTACK | PFG_JUMP:
			if (m_iSlidingAttackFrame++ < SL_ATTACK_END) {
				check_flag |= PFG_SLIDEATTACK;
				if (m_fVX > 0) {	// 右向き
					check_VX = m_fVX - SLIDE_DECELERATION_SPEED;
				}
				else {	// 左向き
					check_VX = m_fVX + SLIDE_DECELERATION_SPEED;
				}
				if (m_iSlidingAttackFrame == ATTACK_COLLISION_GENERATE) {
					IGameObject *pObj = new SlashCollision(AttackCount::Slide, m_bFaceToRight, m_iShadowStateFlag);
					m_pParent->addSlashes(pObj);
				}
			}
			m_iIdleFrame = 0;
			m_iAnimFrame = 0;
			m_iAttackFrame = PLAYER_ATTTACK_TIME;
			break;

		default:
			break;
		}


		if (m_bDamage) {	// 連続ダメージの無効化
			m_iDamagedFrame++;
			if (m_iDamagedFrame == PLAYER_INVINCIBLE_TIME) {
				m_iDamagedFrame = 0;
				m_bDamage = false;
				m_fOpacity = PLAYER_MAX_OPACITY;	// Playerの透明度を戻す
			}
		}
		
		if (m_iPlayerStateFlag & PFG_JUMP) {
			check_flag |= PFG_JUMP;
		}

		m_iPlayerStateFlag &= 0;
		m_iPlayerStateFlag |= check_flag;
		m_fVX = check_VX;
	}
	// -------------------------------------

	// shadow処理--------------------------
	if (GetAsyncKeyState(0x47)) {
		//	シャドウ化
		if ( !m_bShadow && !(m_iPlayerStateFlag & (PFG_JUMP | PFG_SLIDE | PFG_DAMAGED)) && (m_fMana >= SHADOW_AVAILABLE_MANA) ) {
			if ((m_iShadowStateFlag == SFG_OFF)) {
				m_iShadowStateFlag |= SFG_SWITCH;
				m_iPlayerStateFlag &= 0;
				m_iIdleFrame = 0;
				m_iAnimFrame = 0;
				m_iAttackFrame = PLAYER_ATTTACK_TIME;
				m_fVX = 0;
				m_bShadow = true;
				return true;
			}
		}

		//	シャドウ化解除
		if ( !m_bShadow && (m_iShadowStateFlag == SFG_ON) ) {
			m_iShadowStateFlag |= SFG_SWITCH;
		}
	}
	else {
		m_bShadow = false;
	}

	//-------------------------------------

	if (m_iPlayerStateFlag & PFG_DAMAGED) {
		;
	}
	else if (!m_bIs_AttackCancel_OK) {
		// JUMP---------------------------------
		if (GetAsyncKeyState(VK_SPACE)) {
			if (!(m_iPlayerStateFlag & (PFG_JUMP | PFG_ATTACK | PFG_SLIDEATTACK)) && !m_bJump) { // ジャンプ中または攻撃中でなければジャンプできる
				m_fVY = JUMP;
				m_fY_prev = m_fY;
				m_bJump = true;
			}
		}
		else {
			m_bJump = false;
		}

		// SLIDING ------------------------------
		if (GetAsyncKeyState(VK_SHIFT)) {
			if (!(m_iPlayerStateFlag & (PFG_ATTACK | PFG_SLIDEATTACK)) && m_iSlidingFrame < FPS) {
				if (m_iSlidingFrame == 0) {
					if (m_bFaceToRight) {	// 右向き(正)に移動
						m_fVX = 1.f * SLIDESPEED;
					}
					else {		// 左向き(負)に移動
						m_fVX = -1.f * SLIDESPEED;
					}
				}
				else {
					if (m_bFaceToRight) {
						if (!(m_bSlidingTurn) && GetAsyncKeyState(VK_LEFT)) {
							m_fVX *= -1.f;		// 現状の値を負に変換 
							m_bSlidingTurn = true;
							m_bFaceToRight = false;
						}
					}
					else {
						if (!(m_bSlidingTurn) && GetAsyncKeyState(VK_RIGHT)) {
							m_fVX *= -1.f;		// 現状の値を負に変換 
							m_bSlidingTurn = true;
							m_bFaceToRight = true;
						}
					}
				}
				m_iPlayerStateFlag |= PFG_SLIDE;
			}
		}
		else {	//	ここはキーを離さないと更新されないので、スライディング処理の条件リセットをこちらに置いておくと押しっぱなしで連打されるのを防げる
			m_iSlidingFrame = 0;
			m_bSlidingTurn = false;
		}

		// ATTACK----------------------------
		// @note: 攻撃は３段まで可能。m_iAttackCountで段数を管理
		if (GetAsyncKeyState(KEY_ATTACK)) {	
			if (!(m_iPlayerStateFlag & (PFG_SLIDE | PFG_SLIDEATTACK))) {
				if (m_iAttackFrame == PLAYER_ATTTACK_TIME && !m_bAttack && m_iAttackCount == 0) {
					m_iPlayerStateFlag |= PFG_ATTACK;
					m_iAttackFrame = 0;
					m_iAttackCount++;
					m_bAttack = true;
				}
			}
		}
		else {
			m_bAttack = false;
		}
		// アタック二段目
		if ( (m_iAttackCount == AttackCount::First) && (m_iPlayerStateFlag & PFG_ATTACK) && (m_iAttackFrame > SECOND_ATTACK_THRESHOLD) ) {
			if (GetAsyncKeyState(KEY_ATTACK) && !m_bAttack) {
				m_iAttackFrame = 0;
				m_iAttackCount++;
				m_bAttack = true;
			}
		}
		// アタック三段目
		if ( (m_iAttackCount == AttackCount::Second) && (m_iPlayerStateFlag & PFG_ATTACK) && (m_iAttackFrame > THIRD_ATTACK_THRESHOLD)) {
			if (GetAsyncKeyState(KEY_ATTACK) && !m_bAttack) {
				m_iAttackFrame = 0;
				m_iAttackCount++;
				m_bAttack = true;
			}
		}

		// WALK -----------------------------
		if (!(m_iPlayerStateFlag & (PFG_SLIDE | PFG_ATTACK | PFG_SLIDEATTACK))) {
			if (GetAsyncKeyState(VK_RIGHT)) {		// 右向き(正)に移動
				m_fVX = 1.0f * WALKSPEED;		
				m_iPlayerStateFlag |= PFG_WALK;
				m_bFaceToRight = true;
			}
			else if (GetAsyncKeyState(VK_LEFT)) {	// 左向き(負)に移動
				m_fVX = -1.0f * WALKSPEED;
				m_iPlayerStateFlag |= PFG_WALK;
				m_bFaceToRight = false;
			}
		}

	}
	else {	// m_bIs_AttackCancel_OK = true の時---------------------------------

		// WALK -------------------------------
		if (GetAsyncKeyState(VK_RIGHT)) {		// 右向き(正)に移動
			m_fVX = 1.0f * WALKSPEED;
			m_iPlayerStateFlag |= PFG_WALK;
			m_bFaceToRight = true;
			m_iPlayerStateFlag &= ~PFG_ATTACK;
		}
		else if (GetAsyncKeyState(VK_LEFT)) {	// 左向き(負)に移動
			m_fVX = -1.0f * WALKSPEED;
			m_iPlayerStateFlag |= PFG_WALK;
			m_bFaceToRight = false;
			m_iPlayerStateFlag &= ~PFG_ATTACK;
		}	// JUMP---------------------------------
		else if (GetAsyncKeyState(VK_SPACE)) {
			if (!(m_iPlayerStateFlag & PFG_JUMP)) { // ジャンプ中でなければジャンプできる
				m_fVY = JUMP;
				m_fY_prev = m_fY;
				m_bJump = true;
				m_iPlayerStateFlag &= ~PFG_ATTACK;
			}
		}	// SLIDING ------------------------------
		else if (GetAsyncKeyState(VK_SHIFT)) {
			if (m_bFaceToRight) {		// 右向き(正)に移動
				m_fVX = 1.f * SLIDESPEED;
			}
			else {			// 左向き(負)に移動
				m_fVX = -1.f * SLIDESPEED;
			}
			m_iPlayerStateFlag |= PFG_SLIDE;
			m_iPlayerStateFlag &= ~PFG_ATTACK;
		}

		//	ATTACK -----------------------------------
		if (m_iPlayerStateFlag & PFG_ATTACK) {	// PFG_ATTACKフラグが残っていたら = 別行動していなかったら
			if (GetAsyncKeyState(KEY_ATTACK)) {
				// アタック二段目
				if (!m_bAttack && m_iAttackCount == AttackCount::First && m_iAttackFrame < C_OK_SECOND_ATTACK_THRESHOLD) {
					m_iAttackFrame = 0;
					m_bAttack = true;
					m_iAttackCount++;
					m_bIs_AttackCancel_OK = false;
				}	// アタック三段目
				else if (!m_bAttack && m_iAttackCount == AttackCount::Second && m_iAttackFrame < C_OK_THIRD_ATTACK_THRESHOLD) {
					m_iAttackFrame = 0;
					m_bAttack = true;
					m_iAttackCount++;
					m_bIs_AttackCancel_OK = false;
				}
			}
			else {
				m_bAttack = false;
			}
		}
		else {	// 別行動していたらキャンセル可能状態終了
			m_bIs_AttackCancel_OK = false;
			m_iAttackCount = 0;
		}
		
	}


	//	各種処理---------------------------------



	// 空中時の処理
	if(m_iPlayerStateFlag & PFG_JUMP){
		// ひとつ前のフレームに重力を足して次のフレームの座標を計算
		SetVY(m_fY - m_fY_prev + GRAVITY);
		if (m_fVY > FALL_SPEED_MAX)
			m_fVY = FALL_SPEED_MAX;	// 落下速度制限
		m_fY_prev = m_fY;
	}
	

	// ジャンプ攻撃中に左右移動可
	if ( (m_iPlayerStateFlag & (PFG_JUMP | PFG_ATTACK)) == (PFG_JUMP | PFG_ATTACK) ) {
		if (m_bFaceToRight && GetAsyncKeyState(VK_RIGHT)) {		// 右向き(正)に移動
			m_fVX = 1.f * WALKSPEED;
		}
		else if (!(m_bFaceToRight) && GetAsyncKeyState(VK_LEFT)) {		// 左向き(負)に移動
			m_fVX = -1.f * WALKSPEED;
		}
	}

	// スライディング攻撃
	if ((m_iPlayerStateFlag & PFG_SLIDE) && !(m_iPlayerStateFlag & PFG_SLIDE_CONTINUE)) {
		if (GetAsyncKeyState(KEY_ATTACK)) {
			m_iPlayerStateFlag |= PFG_SLIDEATTACK;
			m_iPlayerStateFlag &= ~PFG_SLIDE;
			m_iSlidingFrame = FPS + 1;	// スライディング攻撃待機用1フレーム
			m_iSlidingAttackFrame = 0;
		}
	}
	m_iPlayerStateFlag &= ~PFG_SLIDE_CONTINUE;
	//-----------------------------------


	// SHOT------------------------------
	if (GetAsyncKeyState(0x58)) {	// X
		if (!m_bShot) {
			IGameObject *pObj;
			if (m_bFaceToRight)
				pObj = new Shot(m_pParent, m_fX, m_fY, SHOT_DIRECTION);
			else
				pObj = new Shot(m_pParent, m_fX, m_fY, -SHOT_DIRECTION);
			m_pParent->addShots(pObj);
			m_bShot = true;
		}
	}
	else {
		m_bShot = false;
	}

	//	X方向のステージ端制限
	if (m_fX <= fMAP_CHIP_WIDTH)
	{
		m_fX = PLAYER_MOVE_LIMIT;
		m_fVX = 0;
	}
	//	Y方向のステージ端制限
	if (m_fY <= fMAP_CHIP_WIDTH)
	{
		m_fY = PLAYER_MOVE_LIMIT;
		m_fVY = 0;
	}

	return true;
}


/**
* @brief 描画メソッド
*/
void Player::draw(ID2D1RenderTarget *pRenderTarget) {

	D2D1_RECT_F rc,		// 描画領域(画面上での位置やサイズ)を指定する変数
				src;	// テクスチャの中での座標を指定し、テクスチャを決定する変数
	float tx, ty;
	int Xindex = 0, Yindex = 0;

	// 描画領域の設定
	rc.left = m_fDrawX - PLAYER_TEXTURE_WIDTH;	// 描画領域の左上隅指定
	rc.top = m_fDrawY - PLAYER_TEXTURE_H_TWICE;	//			〃
	rc.right = rc.left + PLAYER_TEXTURE_W_TWICE;	// 描画領域の右下隅指定
	rc.bottom = m_fDrawY;							//			〃

	tx = 0; ty = 0;

	// Shadow描画
	D2D1::Matrix3x2F trans = D2D1::Matrix3x2F::Translation(SHADOW_POS_X_TO_P, SHADOW_POS_Y_TO_P);
	D2D1::Matrix3x2F skew = D2D1::Matrix3x2F::Skew(SHADOW_SKEW_X_TO_P, SHADOW_SKEW_Y_TO_P, D2D1::Point2F(rc.right, rc.bottom));
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(SHADOW_SCALE_X_TO_P, SHADOW_SCALE_Y_TO_P, D2D1::Point2F(rc.right, rc.bottom));
	D2D1::Matrix3x2F calc = trans * scale;

	//	shadow化の描写-----------------
	if (m_iShadowStateFlag == SFG_SWITCH) {		// 通常 => シャドウ切り替え時
		src.top = ty;	// テクスチャ座標の左上隅指定用の情報代入
		src.bottom = ty + PLAYER_TEXTURE_HEIGHT;	// テクスチャ座標の右下隅指定用の情報代入
		// 影移動処理
		trans = D2D1::Matrix3x2F::Translation((SHADOW_POS_X_TO_P + m_iShadowSwitchFrame * SHADOW_CHANGE_SPEED), SHADOW_POS_Y_TO_P);
		calc = trans * scale;

		if (m_bFaceToRight) {
			src.left = tx;		// テクスチャ座標の左上隅指定
			src.right = tx + PLAYER_TEXTURE_WIDTH;	// テクスチャ座標の右下隅指定

			pRenderTarget->DrawBitmap(m_pTexture, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			return;
		}
		else {		// 左向き
			src.left = PLAYER_LASTTEXTURE_POS - tx;		// テクスチャ座標の左上隅指定
			src.right = src.left + PLAYER_TEXTURE_WIDTH;	// テクスチャ座標の右下隅指定

			pRenderTarget->DrawBitmap(m_pTexture_Left, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture_Left, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			return;
		}
	}
	else if (m_iShadowStateFlag == (SFG_ON | SFG_SWITCH)) {		// シャドウ => 通常切り替え時
		src.top = ty;	// テクスチャ座標の左上隅指定用の情報代入
		src.bottom = ty + PLAYER_TEXTURE_HEIGHT;	// テクスチャ座標の右下隅指定用の情報代入
		// 影移動処理
		trans = D2D1::Matrix3x2F::Translation( -(m_iShadowSwitchFrame * SHADOW_CHANGE_SPEED), SHADOW_POS_Y_TO_P);
		calc = trans * scale;

		if (m_bFaceToRight) {
			src.left = tx;	// テクスチャ座標の左上隅指定
			src.right = tx + PLAYER_TEXTURE_WIDTH;	// テクスチャ座標の右下隅指定

			pRenderTarget->DrawBitmap(m_pTexture, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			return;
		}
		else {		// 左向き
			src.left = PLAYER_LASTTEXTURE_POS - tx;		// テクスチャ座標の左上隅指定
			src.right = src.left + PLAYER_TEXTURE_WIDTH;	// テクスチャ座標の右下隅指定

			pRenderTarget->DrawBitmap(m_pTexture_Left, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture_Left, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			return;
		}
	}
	// ---------------------------------------------------- 以下　@修正箇所 ------------------------------------------------------- //
	if (!m_bIs_AttackCancel_OK) {

		switch (m_iPlayerStateFlag) {
		case PFG_IDLE:
		case PFG_JUMP:		// 上でも FPS * 2 しているため
			if (m_iIdleFrame > FPS * 2 - AllTexture) {
				Xindex = (m_iIdleFrame - (FPS * 2 - AllTexture)) >> 2;	// ( >> 2 == * 4 )	アニメーションループ
				Yindex = PlayerTexture::Idle_Y;
			}
			break;

		case PFG_DAMAGED:
		case PFG_DAMAGED | PFG_JUMP:
			Xindex = PlayerTexture::Damaged_X;
			Yindex = PlayerTexture::Damaged_Y;
			break;

		case PFG_WALK:
		case PFG_WALK | PFG_JUMP:
			Xindex = UnusedTexture + ((m_iAnimFrame % WalkTextureTimesFour) >> 2);	// ( >> 2 == * 4 )	アニメーションループ
			Yindex = PlayerTexture::Walk_Y;
			break;

		case PFG_SLIDE:
		case PFG_SLIDE | PFG_JUMP:
			if (m_iSlidingFrame < SL_DURATION_IN_FRAMES * SL_DUALATION_SECONDS) {	// 22フレーム(スライディングにかかるフレーム)より下
				Xindex = PlayerTexture::Slide_X;
				Yindex = PlayerTexture::Slide_Y;
			}
			else {		// 起き上がり動作
				Xindex = PlayerTexture::GetUp_X + ((m_iSlidingFrame - (SL_DURATION_IN_FRAMES * SL_DUALATION_SECONDS)) >> 1);	// 22フレーム * 2
				Yindex = PlayerTexture::GetUp_Y + Xindex / RowPerElement;	// ここの二行で滑らかなアニメーションに
				Xindex = Xindex % RowPerElement;							//	
			}
			break;

		case PFG_ATTACK:
		case PFG_ATTACK | PFG_JUMP:
			if (m_iAttackCount == AttackCount::First) {	//	一段目	-----------------------------------------

				if (m_iAttackFrame < ATTACK_THRESHOLD) {	// 構え
					Xindex = m_iAttackFrame >> 1;
				}
				else if (m_iAttackFrame < ATTACK_AFTER_THRESHOLD) {		// 斬撃
					Xindex = FirstAttackCollisionStart + ((m_iAttackFrame - ATTACK_THRESHOLD) >> 2);	// ( >> 2 == * 4 )	アニメーションループ
				}
				else if (m_iAttackFrame < ATTACK_END_THRESHOLD) {		// 斬撃後待機
					Xindex = FirstAttackCollisionFinish + ((m_iAttackFrame - ATTACK_AFTER_THRESHOLD) >> 1);
				}
				Yindex = PlayerTexture::FirstAttack_Y;
			}
			else if (m_iAttackCount == AttackCount::Second) {	//	二段目	-------------------------------------------

				if (m_iAttackFrame < ATTACK_THRESHOLD) {		// 構え
					Xindex = (m_iAttackFrame >> 1) + 5;		// 5 Texture7行目の一段目の攻撃モーション
				}
				else if (m_iAttackFrame < ATTACK_AFTER_THRESHOLD) {		// 斬撃
					Xindex = ((m_iAttackFrame - ATTACK_THRESHOLD) >> 2) + 7;	// 7 == 5(同上) + 2(Texture7行目の二段目の構えモーション)	 ( >> 2 == * 4 )	アニメーションループ
				}
				else if (m_iAttackFrame < ATTACK_END_THRESHOLD) {			// 斬撃後待機
					Xindex = ((m_iAttackFrame - ATTACK_AFTER_THRESHOLD) >> 1) + 9;	 // 9 == 5(同上) + 2(同上) + 2(Texture8行目の二段目の斬撃モーション)
				}
				Yindex = PlayerTexture::FirstAttack_Y + Xindex / RowPerElement;	// ここの二行で滑らかなアニメーションに
				Xindex = Xindex % RowPerElement;								//
			}
			else {		//	三段目	-----------------------------------------------------------------------------------------------
				if (m_iAttackFrame < ATTACK_THRESHOLD) {		// 構え
					Xindex = (m_iAttackFrame >> 1) + 4;		// 4 Textureの8行目にある二段目の攻撃モーション
				}
				else if (m_iAttackFrame < ATTACK_AFTER_THRESHOLD) {		// 斬撃
					Xindex = ((m_iAttackFrame - ATTACK_THRESHOLD) >> 2) + 6;	// 6 == 4(同上) + 2(Textureの8行目にある三段目の構えモーション)		( >> 2 == * 4 )	アニメーションループ
				}
				else if (m_iAttackFrame < ATTACK_END_THRESHOLD) {		// 斬撃後待機
					Xindex = ((m_iAttackFrame - ATTACK_AFTER_THRESHOLD) >> 1) + 8;	// 8 == 4(同上) + 2(同上) + 2(Textureの8,9行目にある三段目の斬撃モーション)
				}
				Yindex = PlayerTexture::SecondAndThirdAttack_Y + Xindex / RowPerElement;	// ここの二行で滑らかなアニメーションに
				Xindex = Xindex % RowPerElement;											//
			}
			break;

		case PFG_SLIDEATTACK:
		case PFG_SLIDEATTACK | PFG_JUMP:
			if (m_iSlidingAttackFrame < SL_ATTACK_THRESHOLD) {
				Xindex = PlayerTexture::SlideAttackCollisionBefore;
				Yindex = PlayerTexture::SlideAttack_Y;
			}
			else if (m_iSlidingAttackFrame < SL_ATTACK_AFTER_THRESHOLD) {
				Xindex = PlayerTexture::SlideAttackCollisionStart;
				Yindex = PlayerTexture::SlideAttack_Y;
			}
			else if (m_iSlidingAttackFrame < SL_ATTACK_END_THRESHOLD) {
				Xindex = ((m_iSlidingAttackFrame - PlayerTexture::SlideAttackCollisionStart) >> 1);		// 改行後の1,2のテクスチャ指定
				Yindex = PlayerTexture::SlideAttack_Y2;
			}
			break;

		default:
			break;
		}
	}
	else {
		Xindex = 0;
		Yindex = 0;
	}

	// ---------------------------------------------------- 以上　@修正箇所 ------------------------------------------------------- //

	// Textureに代入
	tx = m_fTextureCoord[Xindex];
	ty = m_fTextureCoord[Yindex];

	src.top = ty;	// テクスチャ座標の左上隅指定用の情報代入
	src.bottom = ty + PLAYER_TEXTURE_HEIGHT;	// テクスチャ座標の右下隅指定用の情報代入

	
	//	プレイヤーと影の描写--------------------------
	if (!m_iShadowStateFlag) {		// 通常時

		if (m_bFaceToRight) {
			src.left = tx;		// テクスチャ座標の左上隅指定
			src.right = tx + PLAYER_TEXTURE_WIDTH;	// テクスチャ座標の右下隅指定
			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			pRenderTarget->DrawBitmap(m_pTexture, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

		}
		else {	// 左向き
			src.left = PLAYER_LASTTEXTURE_POS - tx;		// テクスチャ座標の左上隅指定
			src.right = src.left + PLAYER_TEXTURE_WIDTH;	// テクスチャ座標の右下隅指定
			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture_Left, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			pRenderTarget->DrawBitmap(m_pTexture_Left, rc, m_fOpacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, src);

		}

	}
	else {		// シャドウモード時
		trans = D2D1::Matrix3x2F::Translation(SHADOW_CHANGE_DIRECTION, 0.f);
		calc = trans * scale;

		if (m_bFaceToRight) {
			src.left = tx;	// テクスチャ座標の左上隅指定
			src.right = tx + PLAYER_TEXTURE_WIDTH;	// テクスチャ座標の右下隅指定
			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		}
		else {		// 左向き
			src.left = PLAYER_LASTTEXTURE_POS - tx;	// テクスチャ座標の左上隅指定
			src.right = src.left + PLAYER_TEXTURE_WIDTH;	// テクスチャ座標の右下隅指定
			pRenderTarget->SetTransform(calc);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
			pRenderTarget->FillOpacityMask(m_pTexture_Left, m_pBlackBrush, D2D1_OPACITY_MASK_CONTENT::D2D1_OPACITY_MASK_CONTENT_GRAPHICS, rc, src);
			pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		}

	}



#ifdef _DEBUG
	//	デバッグ時当たり判定表示
	float l, r, t, b;
	if (m_fX > QUARTER_MAP_WIDTH_PIXELS && m_fX < m_fRBoader) {
		l = m_fPl - (m_fX - QUARTER_MAP_WIDTH_PIXELS);
		r = l + m_fPw;
		t = m_fPt;
		b = t + m_fPh;
	}
	else if(m_fX >= m_fRBoader){
		l = m_fPl - m_fRBoader + QUARTER_MAP_WIDTH_PIXELS;
		r = l + m_fPw;
		t = m_fPt;
		b = t + m_fPh;
	}
	else {
		l = m_fPl;
		r = l + m_fPw;
		t = m_fPt;
		b = t + m_fPh;
	}
	
	// 描画領域指定
	rc.left = l;  rc.top = t;		// 描画領域の左上隅指定
	rc.right = r; rc.bottom = b;		// 描画領域の右下隅指定
	pRenderTarget->DrawRectangle(rc, m_pWhiteBrush);
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
bool Player::collide(float x, float y, float w, float h) {

	if (m_fPl > x + w)
		return false;
	if (m_fPl + m_fPw < x)
		return false;
	if (m_fPt < y + h)
		return false;
	if (m_fPt + m_fPh > y)
		return false;

	return true;
}


/**
* @brief 他ゲームオブジェクトとの当たり判定メソッド
* @param[in] *pObj 他ゲームオブジェクト
* @return true:当たり / false:外れ
*/
bool Player::collide(IGameObject *pObj) {
	if (m_bDamage || m_iShadowStateFlag & SFG_SWITCH) {
		return false;
	}

	return pObj->collide(m_fPl, m_fPt, m_fPw, m_fPh);
}


/**
* @brief マップとの当たり判定メソッド
* @note	ここで当たり判定に従って(x, y)座標の更新を行う
*		X方向とY方向で分離して判定。
*		ポイント①: それぞれ加速度の正負で左右、上下に判定を分ける
*		ポイント②: 判定する方向のみ座標を移動させ、判定しない方向は移動前の座標を使って判定を行う
*		ポイント③: X方向の判定は加速度0の時は行わない。加速度0の時も含めてしまうとジャンプ時や落下時に壁に引っかかる。
*/
bool Player::collideWithMap() {
	float l = m_fPl, t = m_fPt, w = m_fPw, h = m_fPh, h_half = h / 2.f;


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
	




	//	平地での判定------------------------
	//	坂道を構成するマップチップとの判定は取らないようにしている
	if (!m_bIs_OnSlope) {
		if (m_fVY >= 0) {
			int code1 = m_pBG->tile_code(l + 4, t + h + m_fVY);	// +4 / -4 することで、ブロック隣接時にジャンプすると上に乗ってしまう現象を防げる
			int code2 = m_pBG->tile_code(l + w - 4, t + h + m_fVY);
			// 床と判定
			if ( (code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER)) { 
				if (m_iPlayerStateFlag & PFG_JUMP)
					m_iPlayerStateFlag ^= PFG_JUMP;
				int row = (int)(t + h + m_fVY) >> MAP_CHIP_SIZE_BITS;

				//---エラー原因
				m_fY = (float)(row << MAP_CHIP_SIZE_BITS);

				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {	//	空中時
				m_iPlayerStateFlag |= PFG_JUMP;
				m_fY += m_fVY;
			}
		}
		else {
			// 天井と判定
			if (0 != (m_pBG->tile_code(l + 4, t + m_fVY) || m_pBG->tile_code(l + w - 4, t + m_fVY))) {	// +4 / -4
				int row = (int)(t + m_fVY) >> MAP_CHIP_SIZE_BITS;

				//---エラー原因
				m_fY = (float)((row << MAP_CHIP_SIZE_BITS) + fMAP_CHIP_WIDTH + h);

				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {
				m_iPlayerStateFlag |= PFG_JUMP;
				m_fY += m_fVY;
			}
		}

		if (m_fVX > 0) {
			int code1 = m_pBG->tile_code(l + w + m_fVX, t);
			int code2 = m_pBG->tile_code(l + w + m_fVX, t + h - 4);	// -4 することで地面と判定が出てしまうことを防げる
			int code3 = m_pBG->tile_code(l + w + m_fVX, t + h_half);
			// 右側の判定
			if ( (code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER) || (code3 > 0 && code3 < SLOPE_START_TEXTURE_NUBER) ) {
				int col = (int)(l + w + m_fVX) >> MAP_CHIP_SIZE_BITS;
				m_fX = (float)((col << MAP_CHIP_SIZE_BITS) - ((int)w >> 1));
			}
			else {
				m_fX += m_fVX;
			}
		}
		else if (m_fVX < 0) {
			int code1 = m_pBG->tile_code(l + m_fVX, t);
			int code2 = m_pBG->tile_code(l + m_fVX, t + h - 4); // -4 することで地面と判定が出てしまうことを防げる
			int code3 = m_pBG->tile_code(l + m_fVX, t + h_half);
			// 左側の判定
			if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER) || (code3 > 0 && code3 < SLOPE_START_TEXTURE_NUBER)) {
				int col = (int)(l + m_fVX) >> MAP_CHIP_SIZE_BITS;
				m_fX = (float)((col << MAP_CHIP_SIZE_BITS) + ((int)w >> 1) + iMAP_CHIP_WIDTH);
			}
			else {
				m_fX += m_fVX;
			}
		}
	}
	//	坂道での判定--------------------------------------------
	else {
		
		if (m_fVY >= 0) {
			float stepY, ratio;
			ratio = (m_fX - step.x) / step.vx;
			stepY = step.y + step.vy * ratio;
			if (m_fY + m_fVY + 5.f >= stepY) {	//	+5.f しておくと下りの時にガタガタ跳ねなくなる
				m_iPlayerStateFlag &= ~PFG_JUMP;
				m_fY = stepY;
				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {
				m_iPlayerStateFlag |= PFG_JUMP;
				m_fY += m_fVY;
			}
		}
		else {
			// 天井と判定
			if (0 != (m_pBG->tile_code(l + 4, t + m_fVY) || m_pBG->tile_code(l + w - 4, t + m_fVY))) {	// +4 / -4
				int row = (int)(t + m_fVY) >> MAP_CHIP_SIZE_BITS;
				m_fY = (float)(row << MAP_CHIP_SIZE_BITS) + fMAP_CHIP_WIDTH + h;
				m_fY_prev = m_fY;
				m_fVY = 0;
			}
			else {
				m_iPlayerStateFlag |= PFG_JUMP;
				m_fY += m_fVY;
			}
		}

		if (!(m_iPlayerStateFlag & PFG_JUMP)) {
			m_fX += m_fVX;
		}
		else {
			if (m_fVX > 0) {
				int code1 = m_pBG->tile_code(l + w + m_fVX, t);
				int code2 = m_pBG->tile_code(l + w + m_fVX, t + h - 4);	// -4 することで地面と判定が出てしまうことを防げる
				int code3 = m_pBG->tile_code(l + w + m_fVX, t + h_half);
				// 右側の判定
				if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER) || (code3 > 0 && code3 < SLOPE_START_TEXTURE_NUBER)) {
					int col = (int)(l + w + m_fVX) >> MAP_CHIP_SIZE_BITS;
					m_fX = (float)((col << MAP_CHIP_SIZE_BITS) - ((int)w >> 1));
				}
				else {
					m_fX += m_fVX;
				}
			}
			else if (m_fVX < 0) {
				int code1 = m_pBG->tile_code(l + m_fVX, t);
				int code2 = m_pBG->tile_code(l + m_fVX, t + h - 4); // -4 することで地面と判定が出てしまうことを防げる
				int code3 = m_pBG->tile_code(l + m_fVX, t + h_half);
				// 左側の判定
				if ((code1 > 0 && code1 < SLOPE_START_TEXTURE_NUBER) || (code2 > 0 && code2 < SLOPE_START_TEXTURE_NUBER) || (code3 > 0 && code3 < SLOPE_START_TEXTURE_NUBER)) {
					int col = (int)(l + m_fVX) >> MAP_CHIP_SIZE_BITS;
					m_fX = (float)((col << MAP_CHIP_SIZE_BITS) + ((int)w >> 1) + iMAP_CHIP_WIDTH);
				}
				else {
					m_fX += m_fVX;
				}
			}
		}

	}

	//	当たり判定更新-------------------
	if (m_iPlayerStateFlag & PFG_SLIDE) {
		m_fPl = PHBS_L; m_fPt = PHBS_T; m_fPw = PHBS_W; m_fPh = PHBS_H;
	}
	else {
		m_fPl = PHB_L; m_fPt = PHB_T; m_fPw = PHB_W; m_fPh = PHB_H;
	}
	//--------------------------------------

	return true;
}


/**
* @brief	プレイヤーのx座標を返す
* @return	プレイヤーのx座標
*/
float Player::getX() {
	return m_fX;
}


// プレイヤーのx座標をセットする
void Player::setPos(float x,float y) {
	m_fX = x;
	m_fY = y;
	if (m_iPlayerStateFlag & PFG_SLIDE) {
		m_fPl = PHBS_L; m_fPt = PHBS_T; m_fPw = PHBS_W; m_fPh = PHBS_H;
	}
	else {
		m_fPl = PHB_L; m_fPt = PHB_T; m_fPw = PHB_W; m_fPh = PHB_H;
	}
}


// プレイヤーのY座標を取得して返す
float Player::getY() {
	return m_fY;
}


// プレイヤーのHPを取得して返す
float Player::getHP() {
	return m_fHP;
}


// プレイヤーのマナを取得して返す
float Player::getMana() {
	return m_fMana;
}


// プレイヤーのマナをセットするメソッド
void Player::setMana(float amount) {
	m_fMana += amount;
	if (m_fMana > MANA_LIMIT) {
		m_fMana = MANA_LIMIT;
	}
}


/**
* @brief ダメージメソッド
* @note	 プレイヤーは被ダメージ時 半透明 & ノックバック
*/
int Player::damage(float amount) {
	m_fHP -= amount;
	m_iPlayerStateFlag &= PFG_JUMP;
	m_iPlayerStateFlag |= PFG_DAMAGED;
	m_fOpacity = PLAYER_DAMAGE_OPACITY;		// 半透明
	m_iAttackCount = 0;
	m_iAttackFrame = PLAYER_ATTTACK_TIME;
	m_bIs_AttackCancel_OK = false;

	if (m_bFaceToRight) {	// 右向き
		m_fVX = -PLAYER_KNOCK_BACK;		// ノックバック
	}
	else {	//	左向き
		m_fVX = PLAYER_KNOCK_BACK;		// ノックバック
	}

	m_bDamage = true;

	return 0;
}


// マップの横幅をセットする
void Player::setMapWidth(float mw) {
	m_fMapWidth = mw;
	m_fRBoader = mw - QUARTER_MAP_WIDTH_PIXELS;
}


// マップの縦幅をセットする
void Player::setMapHight(float mh) {
	m_fMapHight = mh;
}


// プレイヤーの画面上のX座標をセット
void Player::setDrawX(float drawX) {
	m_fDrawX = drawX;
}


// プレイヤーの画面上のY座標をセット
void Player::setDrawY(float drawY) {
	m_fDrawY = drawY;
}


/**
 * @brief	BGを設定してPlayerの環境を準備する
 * @note	PlayerのBG環境を設定する => Playerが描画等に影響を与えるために必要
 */
void Player::resetBG(BG *pBG) {
	m_pBG = pBG;
}


// X方向のプレイヤー加速度をリセット
void Player::clearVX() {
	m_fVX = 0;
}


// Y方向のプレイヤー加速度をリセット
void Player::clearVY() {
	m_fVY = 0;
}


/**
* @brief プレイヤーの描画X座標を計算するメソッド
* @return m_fDrawX
* @note collideWithMap()の後にこのメソッドを呼ぶようにすること
*/
float Player::calcPlayerDrawX() {

	// プレイヤーを画面中心に描画
	if (m_fX > QUARTER_MAP_WIDTH_PIXELS && m_fX < m_fRBoader) {
		m_fDrawX = QUARTER_MAP_WIDTH_PIXELS;
	}
	else if (m_fX >= m_fRBoader) {
		m_fDrawX = m_fX - (m_fRBoader - QUARTER_MAP_WIDTH_PIXELS);
	}
	else {
		m_fDrawX = m_fX;
	}

	return m_fDrawX;
}


/**
* @brief プレイヤーの描画Y座標を計算するメソッド
* @return m_fDrawY
* @note collideWithMap()の後にこのメソッドを呼ぶようにすること
*/
float Player::calcPlayerDrawY() 
{

	m_fDrawY = m_fY;

	return m_fDrawY;
}


// TextureにIndexを付与・指定
// @note: X:0～6,Y:7～16
float  Player::m_fTextureCoord[] = {
	// X座標
	0.f, 50.f, 100.f, 150.f, 200.f, 250.f, 300.f,

	// Y座標
	0.f, 37.f, 111.f, 148.f, 222.f, 259.f, 296.f, 481.f, 518.f, 444.f,
};


/*
* @brief	Y方向のプレイヤー加速度を制限するメソッド
* @note		これがないとY方向のマップ移動時にプレイヤーが消える
*/
void Player::SetVY(float vy)
{
	if (vy <= PLAYER_VY_LIMIT)	// -15が最大値
	{
		return;	
	}
	else
	{
		m_fVY = vy;
	}
}
