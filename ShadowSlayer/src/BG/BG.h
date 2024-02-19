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
	BG(ID2D1RenderTarget *pRenderTarget);
	virtual ~BG();
	virtual void draw(ID2D1RenderTarget *pRenderTarget);	// 描画メソッド	 画面上に見えている部分のみ描画
	virtual int setX(float x);		// プレイヤーのx座標をセットして返すメソッド　
	virtual int getX();				// プレイヤーのx座標を取得して返す　
	virtual float getMapWidth();	// マップの横幅を取得して返す 
	virtual float getMapHight();	// マップの縦幅を取得して返す 
	virtual int tile_code(float x, float y);	// 入力された(x, y)座標のブロックの値を返すメソッド
	virtual int mapdata_decode(int index);		// mapdataの配列から数値を復元するメソッド
	virtual void getStepVector(float x, float y, STEP_VECTOR *step);	// 入力された(x, y)座標を起点に坂道のベクトルを計算して返すメソッド
	virtual void resetMapData(int num);			// BGのMAPDATA構造体を更新するメソッド
protected:
	int mMapW, mMapH;	//	mapのwidthとheight
	int mChipW, mChipH;	//	map-chipのwidthとheight
	int mPitch;			//	map-chip データの行ピッチ
	int m_iX;			//	現在プレイヤーがいるx座標
	ID2D1Bitmap *m_pMapChip;
	ID2D1Bitmap *m_pBGImage;
	MAPDATA	map;
};

