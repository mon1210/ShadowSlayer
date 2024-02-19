/**
* @file  IGameObject.h
* @brief ゲーム上のオブジェクトを表現するクラスを汎化したインターフェースの宣言
* @note  日本のゲーム業界ではタスクと呼ぶ場合がある
*/
#pragma once
#define __IGAMEOBJECT_H__

typedef struct {
	float x;	//	起点
	float y;	//	起点
	float vx;	//	x成分
	float vy;	//	y成分
}	STEP_VECTOR;

struct ID2D1RenderTarget;

class IGameObject
{
public:
	/**
	* @brief 純粋仮想デストラクタの実体
	* @details デストラクタだけは用意しなければ、サブクラスがデストラクタを呼べない
	*/
	virtual ~IGameObject() = 0;

	/**
	* @brief 行動,アニメーション管理メソッド
	*/
	virtual bool move() = 0;

	/**
	* @brief 描画メソッド
	*/
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;

	/**
	* @brief  矩形との当たり判定メソッド　
	* @return true:当たり / false:外れ
	*/
	virtual bool collide(float x, float y, float w, float h) {
		return false;
	}

	/**
	* @brief  他ゲームオブジェクトとの当たり判定メソッド
	* @return true:当たり / false:外れ
	*/
	virtual bool collide(IGameObject *pObj) {
		return false;
	}

	/**
	* @brief  マップとの当たり判定メソッド
	*/
	virtual bool collideWithMap() = 0;

	/**
	* @brief  ダメージメソッド
	*/
	virtual int damage(float amount) = 0;

	STEP_VECTOR step;
	float	m_fDamage;
};


//  マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
/**
* @def SAFE_RELEASE
* @brief ポインタの解放
*/
#define SAFE_RELEASE(o) if(o){ (o)->Release(); o = NULL; }
/**
* @def SAFE_DELETE
* @brief ポインタの削除
*/
#define SAFE_DELETE(o)	if(o){ delete (o); o = NULL; }
/**
* @def SAFE_DELETE_ARRAY
* @brief ポインタ配列の削除
*/
#define SAFE_DELETE_ARRAY(o) if(o){ delete [] (o); o = NULL; }

#define GRAVITY 2.0f
