/**
* @file	 Enemy01.h
* @brief クラス Enemy01(スライム) の宣言
* @note	 当たり判定 32 x 25.5
*/
#include "..\IGameObject\IGameObject.h"

class Stage;
class BG;
struct ID2D1Bitmap;
struct ID2D1SolidColorBrush;

class Enemy01 : public IGameObject
{
public:
	Enemy01(Stage *pParent, float x, float y);
	~Enemy01();
	virtual bool move() override;	// 行動状態,アニメーションを管理するメソッド	true:生存 / false:死亡
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;		// 描画メソッド
	virtual bool collide(float x, float y, float w, float h) override;	// 矩形との当たり判定メソッド	true:当たり / false:外れ
	virtual bool collide(IGameObject *pObj) override;	// 他ゲームオブジェクトとの当たり判定メソッド	true:当たり / false:外れ
	virtual bool collideWithMap() override;				// マップとの当たり判定メソッド　当たり判定に従って(x, y)座標の更新を行う
	virtual int damage(float amount) override;			// ダメージメソッド (return ダメージ)
	static void Restore(ID2D1RenderTarget *pRT, BG *pBG);	// 共有メディアファイルの読み込み　シーン開始時などに呼び出すようにする
	static void Finalize();		// 共有メディアファイルの消去　シーン削除時などに呼び出すようにする
protected:
	Stage	*m_pParent;
	float	m_fX, m_fY, m_fY_prev;
	float	m_fVX, m_fVY;
	float	m_fDiffX, m_fDiffY;	//	プレイヤーとの距離
	bool	m_bAttacked;
	bool	m_bFaceToRight;		// 向き判定
	bool	m_bIs_OnSlope;		// 坂にいるかどうか
	UINT	m_iCount;
	UINT	m_iEnemyStateFlag;	// bit演算で複数のフラグを保存するためint型
	static ID2D1Bitmap *m_pTexture, *m_pTexture_Left;
	static BG	*m_pBG;
	static float	m_fTextureCoord[];

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pWhite;
#endif
};

//	行動フラグ
#define EFG_IDLE	0x0000
#define	EFG_MOVE	0x0001
#define EFG_ATTACK	0x0002
#define EFG_JUMP	0x0004
#define EFG_DEATH	0x0008
#define EFG_FALL	0x0008

//	各種ステータス
#define	ENEMY01_SEARCH_DISTANCE		200.f
#define	ENEMY01_ATTACK_DISTANCE		35.f
