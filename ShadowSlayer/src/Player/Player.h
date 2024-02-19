/**
* @file Player.h
* @brief クラス CPlayer の宣言
*/
#include "IGameObject.h"

class Stage;
class BG;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class Player : public IGameObject
{
public:
	Player(Stage *pParent, BG *pBG);
	virtual ~Player();
	virtual bool move() override;	// 行動状態,アニメーションを管理するメソッド	true:生存 / false:死亡
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;		// 描画メソッド
	virtual bool collide(float x, float y, float w, float h) override;	// 矩形との当たり判定メソッド	true:当たり / false:外れ
	virtual bool collide(IGameObject *pObj) override;	// 他ゲームオブジェクトとの当たり判定メソッド	true:当たり / false:外れ
	virtual bool collideWithMap() override;		// マップとの当たり判定メソッド　当たり判定に従って(x, y)座標の更新を行う
	virtual float calcPlayerDrawX();	// プレイヤーの描画X座標を計算するメソッド(return m_fDrawX)　collideWithMap()の後にこのメソッドを呼ぶようにする
	virtual float calcPlayerDrawY();	// プレイヤーの描画Y座標を計算するメソッド(return m_fDrawY)　collideWithMap()の後にこのメソッドを呼ぶようにする
	virtual int damage(float amount) override;	// ダメージメソッド　プレイヤーは被ダメージ時 半透明 & ノックバック
	virtual float getX();		// プレイヤーのx座標を返す
	virtual float getY();		// プレイヤーのY座標を取得して返す
	virtual float getHP();		// プレイヤーのHPを取得して返す
	virtual float getMana();	// プレイヤーのマナを取得して返す
	virtual void  setPos(float x, float y);		// プレイヤーのx座標をセットする
	virtual void  setDrawX(float x);			// プレイヤーの画面上のX座標をセット
	virtual void  setDrawY(float y);			// プレイヤーの画面上のY座標をセット
	virtual void  setMana(float amount);		// プレイヤーのマナをセットするメソッド
	virtual void setMapWidth(float mw);			// マップの横幅をセットする
	virtual void setMapHight(float mh);			// マップの縦幅をセットする
	virtual void resetBG(BG *pBG);	// BGを設定してPlayerの環境を準備する　PlayerのBG環境を設定する => Playerが描画等に影響を与えるために必要
	virtual void clearVX();			// X方向のプレイヤー加速度をリセット
	virtual void clearVY();			// Y方向のプレイヤー加速度をリセット
	virtual void SetVY(float vy);	// Y方向のプレイヤー加速度を制限するメソッド　これがないとY方向のマップ移動時にプレイヤーが消える
	// プレイヤーの描画時の画面上座標。カメラの焦点をプレイヤーに合わせているので、他のオブジェクトはプレイヤーを起点に描画する。データ(m_fX)と描画(m_fDrawX)を分けて考えるのが重要。
	float	m_fDrawX;	
	float	m_fDrawY;

protected:
	Stage		*m_pParent;
	BG			*m_pBG;
	ID2D1Bitmap	*m_pTexture;
	ID2D1Bitmap *m_pTexture_Left;
	ID2D1SolidColorBrush	*m_pBlackBrush;
	float		m_fX, m_fY;		//	プレイヤー位置
	float		m_fY_prev;		//	ジャンプ、落下処理に使用
	float		m_fVX, m_fVY;	//	プレイヤー加速度
	float		m_fPl, m_fPt, m_fPw, m_fPh;		//	プレイヤーのヒットボックス
	float		m_fOpacity;
	float		m_fShadowOpacity;
	float		m_fMapWidth;
	float		m_fMapHight;
	float		m_fRBoader;

	bool		m_bDeath;	//	死亡判定
	bool		m_bDamage;	//	ダメージ判定
	bool		m_bSlidingTurn;	//	スライディング中に向きを変えたかどうか(向きを変えられるのは1回まで)
	bool		m_bJump;		//	ジャンプ連打防止
	bool		m_bAttack;		//	攻撃連打防止
	bool		m_bShot;		//	射撃連打防止
	bool		m_bShadow;		//	シャドウ化連打防止
	bool		m_bFaceToRight;	//	プレイヤーが右を向いているか左を向いているか
	bool		m_bIs_AttackCancel_OK;	//	別行動可能かどうか
	bool		m_bIs_OnSlope;	//	坂道にいるかどうか

	float		m_fHP;
	float		m_fMana;
	//--- UINT 非負の整数型
	UINT		m_iFrame;
	UINT		m_iIdleFrame;			//	アイドル状態になってからの経過フレーム数
	UINT		m_iAnimFrame;			//	アニメートを始めてからの経過フレーム
	UINT		m_iSlidingFrame;		//	スライディングを始めてからの経過フレーム
	UINT		m_iAttackFrame;			//	アタックを始めてからの経過フレーム
	UINT		m_iAttackCount;			//	今何段目の攻撃かを数える
	UINT		m_iSlidingAttackFrame;	//	スライディングアタックを始めてからの経過フレーム
	UINT		m_iDamagedFrame;		//	ダメージを受けてからの経過フレーム
	UINT		m_iShadowSwitchFrame;
	UINT		m_iPlayerStateFlag;		//	プレイヤーの行動状態フラグ	bit演算で複数のフラグを保存するためint型
	UINT		m_iShadowStateFlag;		//	シャドウ状態のフラグ   bit演算で複数のフラグを保存するためint型

	static float	m_fTextureCoord[];

#ifdef _DEBUG
	ID2D1SolidColorBrush	*m_pWhiteBrush;	//	デバッグ用
#endif
};

//	プレイヤーのヒットボックスサイズ
//	基本サイズ
#define PHB_W 28.f
#define PHB_H 60.f
#define PHB_L (m_fX - 14.f)
#define PHB_T (m_fY - 60.f)
//	スライディング時サイズ
#define PHBS_W 30.f
#define PHBS_H 30.f
#define PHBS_L (m_fX - 15.f)
#define PHBS_T (m_fY - 30.f)

// プレイヤーの移動速度
#define WALKSPEED 6.5f
#define SLIDESPEED 9.f
#define SLIDESPEED_DECREASE 0.25f

// プレイヤーの行動状態フラグ
#define PFG_IDLE	0x0000	//	待機中
#define PFG_WALK	0x0001	//	通常移動中
#define PFG_SLIDE	0x0002	//	スライディング中
#define PFG_SLIDE_CONTINUE	0x0100	//	スライディング継続フラグ
#define PFG_JUMP	0x0004	//	空中
#define PFG_ATTACK	0x0008	//	攻撃中
#define PFG_SLIDEATTACK	0x0080	//	スライディング攻撃中
#define PFG_DAMAGED	0x0010	//	被弾

//	シャドウのフラグ
#define	SFG_OFF		0x0000	//	オフ
#define	SFG_SWITCH  0x0001	//	移行中
#define	SFG_ON		0x0002	//	オン

// 重力速度 ジャンプ速度
#define JUMP -16.f

// キー設定
#define KEY_ATTACK	0x0046	//	Fキー