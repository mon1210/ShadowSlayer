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
	/**
	* @brief Enemy01のコンストラクタ
	* @param[in] x	エネミーのx座標
	* @param[in] y	エネミーのy座標
	*/
	Enemy01(Stage *pParent, float x, float y);

	/**
	* @brief Enemy01のデストラクタ
	*/
	~Enemy01();

	/**
	* @brief Enemy01のアニメーション
	* @note	 ダメージを受けると死亡
	* return true:生存 / false:死亡
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
	* @brief ダメージメソッド
	*/
	int damage(float amount) override;

	/**
	* @fn
	* @brief 共有メディアファイルの読み込み
	* @note	 シーン開始時などに呼び出すようにする
	*/
	static void Restore(ID2D1RenderTarget *pRT, BG *pBG);

	/**
	* @brief	共有メディアファイルの消去
	* @note		シーン削除時などに呼び出すようにする
	*/
	static void Finalize();
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
