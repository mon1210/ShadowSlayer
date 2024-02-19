/**
* @file  Player.h
* @brief クラス Player の宣言
*/
#include "..\IGameObject\IGameObject.h"

class Stage;
class BG;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class Player : public IGameObject
{
public:
	/**
	* @brief Playerのコンストラクタ
	*/
	Player(Stage *pParent, BG *pBG);

	/**
	* @brief Playerのデストラクタ
	*/
	 ~Player();

	/**
	* @brief プレイヤーのアニメーションメソッド
	* @return true:生存 / false:死亡
	* @note ここでは加速度の設定だけ行い、(x, y)座標の更新はcollide() で行う
	*/
	bool move() override;

	/**
	* @brief 描画メソッド
	*/
	void draw(ID2D1RenderTarget *pRenderTarget) override;

	/**
	* @brief 矩形との当たり判定メソッド
	* @param[in] x 左上のx座標
	* @param[in] y 左上のy座標
	* @param[in] w 矩形のWidth
	* @param[in] h 矩形のHeight
	* @return true:当たり / false:外れ
	*/
	bool collide(float x, float y, float w, float h) override;

	/**
	* @brief 他ゲームオブジェクトとの当たり判定メソッド
	* @param[in] *pObj 他ゲームオブジェクト
	* @return true:当たり / false:外れ
	*/
	bool collide(IGameObject *pObj) override;

	/**
	* @brief マップとの当たり判定メソッド
	* @note	ここで当たり判定に従って(x, y)座標の更新を行う
	*		X方向とY方向で分離して判定。
	*		ポイント①: それぞれ加速度の正負で左右、上下に判定を分ける
	*		ポイント②: 判定する方向のみ座標を移動させ、判定しない方向は移動前の座標を使って判定を行う
	*		ポイント③: X方向の判定は加速度0の時は行わない。加速度0の時も含めてしまうとジャンプ時や落下時に壁に引っかかる。
	*/
	bool collideWithMap() override;

	/**
	* @brief プレイヤーの描画X座標を計算するメソッド
	* @return m_fDrawX
	* @note collideWithMap()の後にこのメソッドを呼ぶようにすること
	*/
	float calcPlayerDrawX();

	/**
	* @brief プレイヤーの描画Y座標を計算するメソッド
	* @return m_fDrawY
	* @note collideWithMap()の後にこのメソッドを呼ぶようにすること
	*/
	float calcPlayerDrawY();

	/**
	* @brief ダメージメソッド
	* @note	 プレイヤーは被ダメージ時 半透明 & ノックバック
	*/
	int damage(float amount) override;
	
	/**
	* @brief  プレイヤーのx座標を返す
	* @return プレイヤーのx座標
	*/
	float getX();

	/**
	* @brief  プレイヤーのY座標を取得して返す
	* @return プレイヤーのY座標
	*/
	float getY();

	/**
	* @brief  プレイヤーのHPを取得して返す
	* @return プレイヤーのHP
	*/
	float getHP();

	/**
	* @brief  プレイヤーのマナを取得して返す
	* @return プレイヤーのマナ
	*/
	float getMana();

	/**
	* @brief プレイヤーのx座標をセットする
	*/
	void  setPos(float x, float y);

	/**
	* @brief プレイヤーの画面上のX座標をセット
	*/
	void  setDrawX(float x);

	/**
	* @brief プレイヤーの画面上のY座標をセット
	*/
	void  setDrawY(float y);

	/**
	* @brief プレイヤーのマナをセットするメソッド
	*/
	void  setMana(float amount);

	/**
	* @brief マップの横幅をセットする
	*/
	void setMapWidth(float mw);

	/**
	* @brief マップの縦幅をセットする
	*/
	void setMapHight(float mh);

	/**
	 * @brief	BGを設定してPlayerの環境を準備する
	 * @note	PlayerのBG環境を設定する => Playerが描画等に影響を与えるために必要
	 */
	void resetBG(BG *pBG);

	/**
	 * @brief X方向のプレイヤー加速度をリセット
	 */
	void clearVX();

	/**
	 * @brief Y方向のプレイヤー加速度をリセット
	 */
	void clearVY();

	/*
	* @brief	Y方向のプレイヤー加速度を制限するメソッド
	* @note		これがないとY方向のマップ移動時にプレイヤーが消える
	*/
	void setVY(float vy);

	//! プレイヤーの描画時の画面上座標X。カメラの焦点をプレイヤーに合わせているので、他のオブジェクトはプレイヤーを起点に描画する。データ(m_fX)と描画(m_fDrawX)を分けて考えるのが重要。
	float	m_fDrawX;	
	//! プレイヤーの描画時の画面上座標Y。カメラの焦点をプレイヤーに合わせているので、他のオブジェクトはプレイヤーを起点に描画する。データ(m_fX)と描画(m_fDrawX)を分けて考えるのが重要。
	float	m_fDrawY;

protected:
	Stage		*m_pParent;
	BG			*m_pBG;
	ID2D1Bitmap	*m_pTexture;
	ID2D1Bitmap *m_pTexture_Left;
	ID2D1SolidColorBrush	*m_pBlackBrush;
	//! プレイヤー位置
	float		m_fX, m_fY;
	//! ジャンプ、落下処理に使用
	float		m_fY_prev;
	//! プレイヤー加速度
	float		m_fVX, m_fVY;
	//! プレイヤーのヒットボックス
	float		m_fPl, m_fPt, m_fPw, m_fPh;
	float		m_fOpacity;
	float		m_fShadowOpacity;
	float		m_fMapWidth;
	float		m_fMapHight;
	float		m_fRBoader;

	//! 死亡判定
	bool		m_bDeath;
	//! ダメージ判定
	bool		m_bDamage;
	//! スライディング中に向きを変えたかどうか(向きを変えられるのは1回まで)
	bool		m_bSlidingTurn;
	//! ジャンプ連打防止
	bool		m_bJump;
	//! 攻撃連打防止
	bool		m_bAttack;
	//! 射撃連打防止
	bool		m_bShot;
	//! シャドウ化連打防止
	bool		m_bShadow;
	//! プレイヤーが右を向いているか左を向いているか
	bool		m_bFaceToRight;
	//! 別行動可能かどうか
	bool		m_bIs_AttackCancel_OK;
	//! 坂道にいるかどうか
	bool		m_bIs_OnSlope;

	float		m_fHP;
	float		m_fMana;
	UINT		m_iFrame;
	//! アイドル状態になってからの経過フレーム数
	UINT		m_iIdleFrame;
	//! アニメートを始めてからの経過フレーム
	UINT		m_iAnimFrame;
	//! スライディングを始めてからの経過フレーム
	UINT		m_iSlidingFrame;
	//! アタックを始めてからの経過フレーム
	UINT		m_iAttackFrame;
	//! 今何段目の攻撃かを数える
	UINT		m_iAttackCount;
	//! スライディングアタックを始めてからの経過フレーム
	UINT		m_iSlidingAttackFrame;
	//! ダメージを受けてからの経過フレーム
	UINT		m_iDamagedFrame;
	UINT		m_iShadowSwitchFrame;
	//! プレイヤーの行動状態フラグ	bit演算で複数のフラグを保存するためint型
	UINT		m_iPlayerStateFlag;
	//! シャドウ状態のフラグ   bit演算で複数のフラグを保存するためint型
	UINT		m_iShadowStateFlag;

	static float	m_fTextureCoord[];

#ifdef _DEBUG
	//! デバッグ用
	ID2D1SolidColorBrush* m_pWhiteBrush;
#endif
};

//	プレイヤーのヒットボックスサイズ
//	基本サイズ ========================
/**
* @def PHB_W
* @brief 横幅
*/
#define PHB_W 28.f
/**
* @def PHB_H
* @brief 縦幅
*/
#define PHB_H 60.f
/**
* @def PHB_L
* @brief 左上
*/
#define PHB_L (m_fX - 14.f)
/**
* @def PHB_T
* @brief 頂点
*/
#define PHB_T (m_fY - 60.f)
// =====================================

//	スライディング時サイズ ============
/**
* @def PHBS_W
* @brief 横幅
*/
#define PHBS_W 30.f
/**
* @def PHBS_H
* @brief 縦幅
*/
#define PHBS_H 30.f
/**
* @def PHBS_L
* @brief 左上
*/
#define PHBS_L (m_fX - 15.f)
/**
* @def PHBS_T
* @brief 頂点
*/
#define PHBS_T (m_fY - 30.f)
// ====================================

// プレイヤーの移動速度 ===============
/**
* @def WALKSPEED
* @brief 歩く速度
*/
#define WALKSPEED 6.5f
/**
* @def SLIDESPEED
* @brief スライディング速度
*/
#define SLIDESPEED 9.f
// ====================================

// プレイヤーの行動状態フラグ =========
/**
* @def PFG_IDLE
* @brief 待機中
*/
#define PFG_IDLE	0x0000
/**
* @def PFG_WALK
* @brief 通常移動中
*/
#define PFG_WALK	0x0001
/**
* @def PFG_SLIDE
* @brief スライディング中
*/
#define PFG_SLIDE	0x0002
/**
* @def PFG_SLIDE_CONTINUE
* @brief スライディング継続フラグ
*/
#define PFG_SLIDE_CONTINUE	0x0100
/**
* @def PFG_JUMP
* @brief 空中
*/
#define PFG_JUMP	0x0004
/**
* @def PFG_ATTACK
* @brief 攻撃中
*/
#define PFG_ATTACK	0x0008
/**
* @def PFG_SLIDEATTACK
* @brief スライディング攻撃中
*/
#define PFG_SLIDEATTACK	0x0080
/**
* @def PFG_DAMAGED
* @brief 被弾
*/
#define PFG_DAMAGED	0x0010
// ====================================

//	シャドウのフラグ ==================
/**
* @def SFG_OFF
* @brief オフ
*/
#define	SFG_OFF		0x0000
/**
* @def SFG_SWITCH
* @brief 移行中
*/
#define	SFG_SWITCH  0x0001
/**
* @def SFG_ON
* @brief オン
*/
#define	SFG_ON		0x0002
// ====================================

// 重力速度 ジャンプ速度 ==============
/**
* @def JUMP
* @brief ジャンプ速度
*/
#define JUMP -16.f
// ====================================

// キー設定 ===========================
/**
* @def KEY_ATTACK
* @brief 攻撃キー(F)
*/
#define KEY_ATTACK	0x0046
// ====================================