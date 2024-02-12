/**
* @file SlashCollision.h
* @brief プレイヤーの近接攻撃の当たり判定
*/
#include "IGameObject.h"

class Player;
struct ID2D1SolidColorBrush;

class SlashCollision : public IGameObject
{
public:
	SlashCollision(int attackCount, bool faceToRight, int shadowFlag);
	~SlashCollision();
	virtual bool move();	// 行動状態,アニメーションを管理するメソッド	true:生存 / false:死亡
	virtual void draw(ID2D1RenderTarget *pRenderTarget);		// 描画メソッド
	virtual bool collide(float x, float y, float w, float h);	// 矩形との当たり判定メソッド	true:当たり / false:外れ
	virtual bool collide(IGameObject *pObj) override;			// 他ゲームオブジェクトとの当たり判定メソッド	true:当たり / false:外れ
	static void Restore(Player *m_pPlayer);		// 共有メディアファイルの読み込み　シーン開始時などに呼び出すようにする
	static void Finalize();				// 共有メディアファイルの消去　シーン削除時などに呼び出すようにする
	virtual bool collideWithMap();		// マップとの当たり判定メソッド　当たり判定に従って(x, y)座標の更新を行う
	virtual int damage(float amount) override;	// ダメージメソッド (return ダメージ)

#ifdef _DEBUG
	static void Restore(Player *pPlayer, ID2D1RenderTarget *pRT);
#endif
protected:
	float	m_fX, m_fY;	//	左上のX,Y座標
	float	m_fW, m_fH, m_fXOffSet, m_fYOffSet;
	int		m_iLifeTime;
	int		m_iAttackCount;
	bool	m_bFaceToRight;		// 向き判定
	static Player *m_pPlayer;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pRed;
#endif
};

//	1, 2段目のコリジョン
#define SLASH_WIDTH		40.f
#define SLASH_HEIGHT	60.f
//	3段目のコリジョン
#define SLASH_WIDTH3	50.f
#define	SLASH_HEIGHT3	50.f
//	ダッシュ攻撃のコリジョン
#define SLASH_WIDTH4	55.f
#define SLASH_HEIGHT4	50.f