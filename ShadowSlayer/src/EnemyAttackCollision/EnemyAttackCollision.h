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
	/**
	* @brief EnemyAttackCollisionのコンストラクタ
	* @param[in] x				エネミーのx座標
	* @param[in] y				エネミーのy座標
	* @param[in] w				当たり判定描画座標
	* @param[in] colW			当たり判定横幅(W)
	* @param[in] colH			当たり判定縦幅(H)
	* @param[in] LifeTime		存在時間
	* @param[in] faceToRight	向き判定フラグ　true:右向き / false:左向き
	*/
	EnemyAttackCollision(float x, float y, float w, float colW, float colH, int LifeTime,bool faceToRight);

	/**
	* @brief EnemyAttackCollisionのデストラクタ
	*/
	~EnemyAttackCollision();

	/**
	* @brief 行動管理
	* @note	 生存タイマーを過ぎると死亡
	* return true:生存 / false:死亡
	*/
	bool move();

	/**
	* @brief 描画メソッド
	*/
	void draw(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief 矩形との当たり判定メソッド
	* @param[in] x 左上のx座標
	* @param[in] y 左上のy座標
	* @param[in] w 矩形のWidth
	* @param[in] h 矩形のHeight
	* @return true:当たり / false:外れ
	*/
	bool collide(float x, float y, float w, float h);

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
	bool collideWithMap();		// マップとの当たり判定メソッド

	/**
	* @brief ダメージメソッド
	*/
	int damage(float amount);

	/**
	* @brief 共有メディアファイルの読み込み
	* @note	 シーン開始時などに呼び出すようにする
	*/
	static void Restore(Player *m_pPlayer);

	/**
	* @brief 共有メディアファイルの消去
	* @note	 シーン削除時などに呼び出すようにする
	*/
	static void Finalize();
#ifdef _DEBUG
	/**
	* @brief 共有メディアファイルの読み込み
	* @note	 シーン開始時などに呼び出すようにする
	*/
	static void Restore(Player *pPlayer, ID2D1RenderTarget *pRT);
#endif
protected:
	//! 左上のX,Y座標
	float	m_fX, m_fY;
	float	m_fW, m_fH;
	int		m_iCount, m_iLifeTime;
	//! 向き判定
	bool	m_bFaceToRight;
	static Player *m_pPlayer;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pRed;
#endif
};

