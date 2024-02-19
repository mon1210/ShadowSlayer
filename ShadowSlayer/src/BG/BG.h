/**
* @file	 BG.h
* @brief クラス BG の宣言
* @note	 背景・マップクラス
*/
//ゲーム上のオブジェクトを表現するクラスを汎化したインターフェースの宣言
#include "..\IGameObject\IGameObject.h"

struct ID2D1Bitmap;
struct ID2D1RenderTarget;

typedef struct mapdata {
	BYTE	*data;
	size_t	length;
}	MAPDATA;


class BG
{
public:
	/**
	* @brief BGのコンストラクタ
	*/
	BG(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief BGのコンストラクタ
	*/
	~BG();

	/**
	* @brief 描画メソッド
	* @note  画面上に見えている部分のみ描画
	*/
	void draw(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief プレイヤーのx座標をセットして返す
	*/
	int setX(float x);

	/**
	* @brief プレイヤーのx座標を取得して返す
	*/
	int getX();

	/**
	* @brief マップの横幅を取得して返す 
	*/
	float getMapWidth();

	/**
	* @brief マップの縦幅を取得して返す
	*/
	float getMapHight();

	/**
	* @brief 入力された(x, y)座標のブロックの値を返す
	* @param[in] x x座標
	* @param[in] y y座標
	* @return int code = 0 : 当たり判定なし / code != 0 : 当たり判定あり
	*/
	int tile_code(float x, float y);

	/**
	* @brief mapdataの配列から数値を復元するメソッド
	* @param[in] index	配列の添え字
	*/
	int mapdata_decode(int index);

	/**
	* @brief	入力された(x, y)座標を起点に坂道のベクトルを計算して返す
	* @note		起点から右と左へ順に坂の終わりを捜索する。colから求まるx座標はタイルの左端の座標なので、r_colはそのまま、l_colには+2すること。
	*/
	void getStepVector(float x, float y, STEP_VECTOR *step);

	/**
	* @brief	BGのMAPDATA構造体を更新します。
	* @note		この関数でリセットしたら、m_pPlayer->setMapWidth(m_pBG->getMapWidth()); を忘れずに一度実行すること
	*/
	void resetMapData(int num);
protected:
	//! mapのwidthとheight
	int mMapW, mMapH;
	//! map-chipのwidthとheight
	int mChipW, mChipH;
	//! map-chip データの行ピッチ
	int mPitch;
	//! 現在プレイヤーがいるx座標
	int m_iX;
	ID2D1Bitmap *m_pMapChip;
	ID2D1Bitmap *m_pBGImage;
	MAPDATA	map;
};

