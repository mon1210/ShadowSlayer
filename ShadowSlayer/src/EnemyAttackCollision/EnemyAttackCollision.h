/**
* @file  EnemyAttackCollision.h
* @brief プレイヤーの近接攻撃の当たり判定
*/
#include "..\IGameObject\IGameObject.h"

class Player;
struct ID2D1SolidColorBrush;

class EnemyAttackCollision : public IGameObject
{
public:
	EnemyAttackCollision(float x, float y, float w, float colW, float colH, int LifeTime,bool faceToRight);
						// x座標, y座標, 当たり判定描画座標, 当たり判定横幅(W), 当たり判定縦幅(H), 存在時間, 向き判定フラグ(true:右向き / false:左向き)
	~EnemyAttackCollision();
	virtual bool move();	// EnemyAttackCollisionの行動管理　生存タイマーを過ぎると死亡　true:生存 / false:死亡
	virtual void draw(ID2D1RenderTarget *pRenderTarget);		// 描画メソッド
	virtual bool collide(float x, float y, float w, float h);	// 矩形との当たり判定メソッド　true:当たり / false:外れ
	virtual bool collide(IGameObject *pObj) override;			// 他ゲームオブジェクトとの当たり判定メソッド　true:当たり / false:外れ
	static void Restore(Player *m_pPlayer);		// 共有メディアファイルの読み込み　シーン開始時などに呼び出すようにする
	static void Finalize();				// 共有メディアファイルの消去　シーン削除時などに呼び出すようにする
	virtual bool collideWithMap();		// マップとの当たり判定メソッド
	virtual int damage(float amount);	// ダメージメソッド(return ダメージ)

#ifdef _DEBUG
	static void Restore(Player *pPlayer, ID2D1RenderTarget *pRT);
#endif
protected:
	float	m_fX, m_fY;	//	左上のX,Y座標
	float	m_fW, m_fH;
	int		m_iCount, m_iLifeTime;
	bool	m_bFaceToRight;		// 向き判定
	static Player *m_pPlayer;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pRed;
#endif
};

