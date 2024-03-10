/**
* @file  SlashCollision.h
* @brief プレイヤーの近接攻撃の当たり判定
*/
#include "..\IGameObject\IGameObject.h"

class Player;
struct ID2D1SolidColorBrush;

class SlashCollision : public IGameObject
{
public:
	/**
	* @brief SlashCollisionのコンストラクタ
	* @param[in] attackCount	何段目攻撃かのカウンタ(4:スライディング攻撃)
	* @param[in] faceToRight	向き判定フラグ　	true：右向き  / false：左向き
	* @param[in] shadowFlag		シャドウ判定フラグ　非0：シャドウ /	0：ノーマル	　bit演算で複数のフラグを保存するためint型
	*/
	SlashCollision(int attackCount, bool faceToRight, int shadowFlag);

	/**
	* @brief SlashCollisionのデストラクタ
	*/
	~SlashCollision();

	/**
	* @brief SlashCollisionの生死
	* @note	 生存時間を過ぎると消滅
	* return true:生存 / false:死亡
	*/
	virtual bool move();

	/**
	* @brief 描画メソッド
	*/
	virtual void draw(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief 矩形との当たり判定メソッド
	* @param[in] x 左上のx座標
	* @param[in] y 左上のy座標
	* @param[in] w 矩形のWidth
	* @param[in] h 矩形のHeight
	* @return true:当たり / false:外れ
	*/
	virtual bool collide(float x, float y, float w, float h);

	/**
	* @brief 他ゲームオブジェクトとの当たり判定メソッド
	* @param[in] *pObj 他ゲームオブジェクト
	* @return true:当たり / false:外れ
	*/
	virtual bool collide(IGameObject *pObj) override;

	/**
	* @fn
	* @brief 共有メディアファイルの読み込み
	* @note	 シーン開始時などに呼び出すようにする
	*/
	static void Restore(Player *m_pPlayer);

	/**
	* @fn
	* @brief	共有メディアファイルの消去
	* @note		シーン削除時などに呼び出すようにする
	*/
	static void Finalize();

	/**
	* @brief マップとの当たり判定メソッド
	* @note	ここで当たり判定に従って(x, y)座標の更新を行う
	*		X方向とY方向で分離して判定。
	*		ポイント①: それぞれ加速度の正負で左右、上下に判定を分ける
	*		ポイント②: 判定する方向のみ座標を移動させ、判定しない方向は移動前の座標を使って判定を行う
	*		ポイント③: X方向の判定は加速度0の時は行わない。加速度0の時も含めてしまうとジャンプ時や落下時に壁に引っかかる。
	*/
	bool collideWithMap();

	/**
	* @brief ダメージメソッド
	*/
	int damage(float amount) override;
#ifdef _DEBUG
	/**
	* @fn
	* @brief 共有メディアファイルの読み込み
	* @note	 シーン開始時などに呼び出すようにする
	*/
	static void Restore(Player *pPlayer, ID2D1RenderTarget *pRT);
#endif
protected:
	//! 左上のX,Y座標
	float	m_fX, m_fY;
	float	m_fW, m_fH, m_fXOffSet, m_fYOffSet;
	int		m_iLifeTime;
	int		m_iAttackCount;
	//! 向き判定
	bool	m_bFaceToRight;
	static Player *m_pPlayer;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pRed;
#endif
};

/**
* @def SLASH_WIDTH
* @brief 1, 2段目のコリジョン
*/
#define SLASH_WIDTH		40.f
/**
* @def SLASH_HEIGHT
* @brief 1, 2段目のコリジョン
*/
#define SLASH_HEIGHT	60.f

/**
* @def SLASH_WIDTH3
* @brief 3段目のコリジョン
*/
#define SLASH_WIDTH3	50.f
/**
* @def SLASH_HEIGHT3
* @brief 3段目のコリジョン
*/
#define	SLASH_HEIGHT3	50.f

/**
* @def SLASH_WIDTH4
* @brief ダッシュ攻撃のコリジョン
*/
#define SLASH_WIDTH4	55.f
/**
* @def SLASH_HEIGHT4
* @brief ダッシュ攻撃のコリジョン
*/
#define SLASH_HEIGHT4	50.f