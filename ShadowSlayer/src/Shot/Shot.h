/**
* @file  Shot.h
* @brief クラス Shot の宣言ファイル
*/
#include "..\IGameObject\IGameObject.h"

struct ID2D1Bitmap;
class Stage;
class BG;

class Shot : public IGameObject
{
public:
	/**
	* @brief Shotのコンストラクタ
	* @param[in] x	プレイヤーのx座標
	* @param[in] y	プレイヤーのy座標
	* @param[in] direction	1 or -1
	*/
	Shot(Stage *pParent, float x, float y, float direction);

	/**
	* @brief Shotのデストラクタ
	*/
	~Shot();

	/**
	* @brief shotのアニメーション
	* @note	敵やマップにぶつかるか、画面外に出ると消滅
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
	* @note	 ここで当たり判定に従って(x, y)座標の更新を行う
	*		 X方向とY方向で分離して判定。
	*		 ポイント①: それぞれ加速度の正負で左右、上下に判定を分ける
	*		 ポイント②: 判定する方向のみ座標を移動させ、判定しない方向は移動前の座標を使って判定を行う
	*		 ポイント③: X方向の判定は加速度0の時は行わない。加速度0の時も含めてしまうとジャンプ時や落下時に壁に引っかかる。
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
	 * @brief BGを設定してShotの環境を準備する
	 * @note  ShotのBG環境を設定する => Shotが描画等影響を与えるために必要
	 */
	static void resetBG(BG *pBG);

	/**
	* @fn
	* @brief 共有メディアファイルの消去
	* @note	 シーン削除時などに呼び出すようにする
	*/
	static void Finalize();			// 共有メディアファイルの消去　シーン削除時などに呼び出すようにする

protected:
	Stage		*m_pParent;
	bool		m_bDamage;
	float		m_fX;
	float		m_fY;
	float		m_fVX;
	float		m_fVY;
	float		m_fDirection;
	//! 生成時からのアニメーションタイマー
	int			m_iLifeTimer;
	//! 消滅までのアニメーションタイマー
	int			m_iDeathTimer;
	//! 静的メンバ。クラス全体で(=全インスタンスで)共有。
	static ID2D1Bitmap *m_pTexture;
	static ID2D1Bitmap *m_pTexture_Left;
	static BG	*m_pBG;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pWhite;
#endif
};

#define BULLETSPEED 8.0f
/**
* @def SHOTOFFSETX
* @brief 表示開始座標の修正
*/
#define SHOTOFFSETX 30.f
/**
* @def SHOTOFFSETY
* @brief 表示開始座標の修正
*/
#define SHOTOFFSETY 40.f