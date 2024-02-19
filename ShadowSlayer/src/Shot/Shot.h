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
	Shot(Stage *pParent, float x, float y, float direction);
	~Shot();

	virtual bool move() override;	// 行動状態,アニメーションを管理するメソッド	true:生存 / false:死亡
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;	// 描画メソッド
	virtual bool collide(float x, float y, float w, float h) override;	// 矩形との当たり判定メソッド	true:当たり / false:外れ
	virtual bool collide(IGameObject *pObj) override;		// 他ゲームオブジェクトとの当たり判定メソッド	true:当たり / false:外れ
	virtual bool collideWithMap() override;			// マップとの当たり判定メソッド　当たり判定に従って(x, y)座標の更新を行う
	virtual int damage(float amount) override;		// ダメージメソッド (return ダメージ)
	static void Restore(ID2D1RenderTarget *pRT, BG *pBG);	// 共有メディアファイルの読み込み　シーン開始時などに呼び出すようにする
	static void resetBG(BG *pBG);	// BGを設定してShotの環境を準備する　ShotのBG環境を設定する => Shotが描画等影響を与えるために必要
	static void Finalize();			// 共有メディアファイルの消去　シーン削除時などに呼び出すようにする

protected:
	Stage		*m_pParent;
	bool		m_bDamage;
	float		m_fX;
	float		m_fY;
	float		m_fVX;
	float		m_fVY;
	float		m_fDirection;
	int			m_iLifeTimer;	//	生成時からのアニメーションタイマー
	int			m_iDeathTimer;	//	消滅までのアニメーションタイマー
	static ID2D1Bitmap *m_pTexture;	//	静的メンバ。クラス全体で(=全インスタンスで)共有。
	static ID2D1Bitmap *m_pTexture_Left;
	static BG	*m_pBG;

#ifdef _DEBUG
	static ID2D1SolidColorBrush *m_pWhite;
#endif
};

#define BULLETSPEED 8.0f
#define SHOTOFFSETX 30.f	// 表示開始座標の修正
#define SHOTOFFSETY 40.f	// 表示開始座標の修正