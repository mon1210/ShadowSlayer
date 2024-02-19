/**
* @file  GateKeeper.h
* @brief クラス GateKeeper の宣言
*/
#pragma once

class Stage;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;

class GateKeeper
{
public:
	/**
	* @brief GateKeeperのコンストラクタ
	* @param[in] x	ゲートキーパーのx座標
	* @param[in] y	ゲートキーパーのy座標
	*/
	GateKeeper(Stage* pStage, float x, float y);

	/**
	* @brief GateKeeperのデストラクタ
	*/
	~GateKeeper();

	/**
	* @brief 描画メソッド
	*/
	void draw(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief	共有メディアファイルの読み込み
	* @note		シーン開始時などに呼び出すようにする
	*/
	static void Restore(ID2D1RenderTarget *pTarget);

	/**
	* @brief	共有メディアファイルの消去
	* @note		シーン削除時などに呼び出すようにする
	*/
	static void Finalize();
protected:
	Stage *m_pParent;
	static ID2D1Bitmap	*m_pImage;
	float m_fX, m_fY;
};

