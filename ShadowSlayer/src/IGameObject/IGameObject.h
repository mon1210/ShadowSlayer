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
	virtual ~IGameObject() = 0;
	virtual bool move() = 0;	// 行動,アニメーション管理メソッド
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;	// 描画メソッド
	virtual bool collide(float x, float y, float w, float h) {	// 矩形との当たり判定メソッド　true:当たり / false:外れ
		return false;
	}
	virtual bool collide(IGameObject *pObj) {	// 他ゲームオブジェクトとの当たり判定メソッド　true:当たり / false:外れ
		return false;
	}
	virtual bool collideWithMap() = 0;		// マップとの当たり判定メソッド
	virtual int damage(float amount) = 0;	// ダメージメソッド(return ダメージ)

	STEP_VECTOR step;
	float	m_fDamage;
};


//  マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }

#define GRAVITY 2.0f
