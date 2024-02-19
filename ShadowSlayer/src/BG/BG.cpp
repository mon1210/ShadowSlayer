// Windowsアプリケーション開発用の共通ヘッダファイル
#include "..\stdafx\stdafx.h"
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
// ゲームの背景を管理するクラスと関連データを定義
#include "BG.h"
// テクスチャとビットマップの読み込みと管理に関するクラスとインターフェースを定義
#include "..\TextureLoader\TextureLoader.h"
// ゲーム内のマップデータを定義
#include "..\MapData\MapData.h"
// 定数値を定めたヘッダファイル
#include "..\Constants.h"


//  マクロ定義
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }


// コンストラクタ
BG::BG(ID2D1RenderTarget *pRenderTarget)
{
	
	m_pMapChip = NULL;
	map.data = NULL;
	TextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\test_mapchip2.png"), &m_pMapChip);
	TextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\title_background.png"), &m_pBGImage);
	map.data = MapData::data;
	map.length = MapData::length;

	// Mapデータヘッダの読み取りとデコード
	// @note: 元の値の下位8ビット以外のビットがクリア
	mChipW = map.data[4] & MASK_BIT_8;
	mChipH = map.data[5] & MASK_BIT_8;
	mMapW = map.data[2] & MASK_BIT_8;
	mMapH = map.data[3] & MASK_BIT_8;
	mPitch = mMapW * MAP_BYTES_PER_ROW;	// 横一行あたりのバイト数(配列の列数)
	mMapW *= mChipW;
	mMapH *= mChipH;

	m_iX = PLAYER_START_X;	// プレイヤーx座標
}


// デストラクタ
BG::~BG()
{
	SAFE_RELEASE(m_pMapChip);
	SAFE_RELEASE(m_pBGImage);
	map.data = NULL;
}


// プレイヤーのx座標をセットして返す
int BG::setX(float x) {
	m_iX = (int)x;	
	return m_iX;
}


// プレイヤーのx座標を取得して返す
int BG::getX() {
	return m_iX;
}

// マップの横幅を取得して返す
float BG::getMapWidth() {
	return (float)mMapW;
}


// マップの縦幅を取得して返す
float BG::getMapHight() {
	return (float)mMapH;
}


// 描画メソッド
void BG::draw(ID2D1RenderTarget *pRenderTarget) {
	int texCols;	// マップチップ画像の列数
	D2D1_SIZE_F size;

	if (m_pMapChip == NULL)
		return;
	size = m_pMapChip->GetSize();
	texCols = ((int)size.width + mChipW - ROUNDING) >> MAP_CHIP_SIZE_BITS;	// マップチップテクスチャの列数を計算
	
	
	int dx = 0, dy = 0;	// 描画開始座標
	int ix = 0;			// 描画開始の列番号
	float	bgX;

	// プレイヤーのX座標がマップ1の4分の1より左にいるとき
	if (m_iX < QUARTER_MAP_WIDTH_PIXELS)	// 20要素 = 20byte = 10chip = 320pixel
	{
		ix = 0;
		bgX = 0.f;
	}
	// ステージの一番右端が画面に映っていたら
	else if (m_iX > mMapW - QUARTER_MAP_WIDTH_PIXELS)	
	{
		ix = mPitch - MAP1_WIDHT_CHIP_COUNT;

		bgX = mMapW - HALF_MAP_WIDTH_PIXELS;
	}
	else {
		ix = (m_iX >> MAP_CHIP_SIZE_BITS) * MAP_BYTES_PER_ROW - MAP1_HALF_WIDHT_CHIP_COUNT;

		dx = -(m_iX % mChipW);	//	これがないと滑らかにスクロールしない
		bgX = m_iX - QUARTER_MAP_WIDTH_PIXELS;
	}


	// 以下背景描画処理 ----------------------------------------------------------------------------------------------------------------------------------------------------------- //
	D2D1_RECT_F rc,		// 描画領域(画面上での位置やサイズ)を指定する変数
				src;	// テクスチャの中での座標を指定し、テクスチャを決定する変数
	size = pRenderTarget->GetSize();
	// 描画領域の設定
	rc.left = 0.f; rc.top = 0.f;	// 描画領域の左上隅指定
	rc.right = size.width; rc.bottom = size.height;		// 描画領域の右下隅指定

	// テクスチャ座標の計算
	src.left = bgX; src.top = 0.f;		// テクスチャ座標の左上隅指定
	src.right = src.left + size.width; src.bottom = size.height;	// テクスチャ座標の右下隅指定

	// 背景の描画
	pRenderTarget->DrawBitmap(m_pBGImage, rc, BG_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, src);
	// 以上背景描画処理 ----------------------------------------------------------------------------------------------------------------------------------------------------------- //


	// 以下ステージ(マップチップ)描画 --------------------------------------------------------------------------------------------------------------------------------------------- //
	// *マップチップ情報は2つあることに注意！*　(MapDate.cpp参考)
	int code;		// マップチップの情報
	int tu, tv;
	int count = 0;
	int currentIndex = 0;
	while (dy < MAP_HEIGHT_PIXELS) {	// 画面下まで
		currentIndex = ix + mPitch * count;
		for (int x = dx; x < HALF_MAP_WIDTH_PIXELS; x += mChipW) {	// 画面端まで
			code = map.data[currentIndex + FILE_INFO_BYTES] & MASK_BIT_8;	// 先頭6byteはファイル情報なのでスキップ & 1つ目のマップチップ情報取得
			code += (map.data[currentIndex + MAP_CHIP_DATA_OFFSET] & MASK_BIT_8) & MASK_BIT_8;	// 2つ目のマップチップ情報取得
			currentIndex += MAP_CHIP_DATA_SIZE;		// 次のマップチップデータへ

			// 描画領域の設定	rc.left,re.top => 左上隅指定 rc.right,re.bottom => 右下隅指定
			rc.left = x;
			rc.top = dy;
			rc.right = rc.left + mChipW;
			rc.bottom = rc.top + mChipH;

			// マップチップのテクスチャ座標の計算	src.left,sre.top => 左上隅指定 src.right,sre.bottom => 右下隅指定
			tu = code % texCols;
			tv = code / texCols;
			src.left = tu << MAP_CHIP_SIZE_BITS;
			src.top = tv * mChipH;
			src.right = src.left + mChipW;
			src.bottom = src.top + mChipH;

			// マップチップの描画
			pRenderTarget->DrawBitmap(m_pMapChip, rc, MAP_CHIP_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, src);
		}

		dy += mChipH;	// 現在の高さを保存
		count++;		// 次の列へ
	}
	// 以上ステージ(マップチップ)描画 --------------------------------------------------------------------------------------------------------------------------------------------- //

}


// 入力された(x, y)座標のブロックの値を返す
int BG::tile_code(float x, float y) {
	int row, col, index, code;
	row = (int)y >> MAP_CHIP_SIZE_BITS;
	col = ((int)x >> MAP_CHIP_SIZE_BITS) << MAP_CHIP_DATA_SIZE_BITS;
	index = col + row * mPitch;
	code = mapdata_decode(index);
	return code;
}


// mapdataの配列から数値を復元する
int BG::mapdata_decode(int index) {
	int code;
	code = map.data[index + FILE_INFO_BYTES] & MASK_BIT_8;
	code += (map.data[index + MAP_CHIP_DATA_OFFSET] & MASK_BIT_8) & MASK_BIT_8;
	return code;
}


// 入力された(x, y)座標を起点に坂道のベクトルを計算して返す
void BG::getStepVector(float x, float y, STEP_VECTOR *step) {
	int start_row, start_col, index;
	int r_row, r_col, l_row, l_col;

	start_row = (int)y >> MAP_CHIP_SIZE_BITS;
	start_col = ((int)x >> MAP_CHIP_SIZE_BITS) << MAP_CHIP_DATA_SIZE_BITS;
	index = start_col + start_row * mPitch;

	if (mapdata_decode(index) > SLOPE_TEXTURE_NUMBER) {	//	右下がりの坂---------------------------------------

		// マップチップの番号情報
		int mapNum;

		//	右側へ探索 --------------------------------------
		r_row = start_row, r_col = start_col;
		mapNum = mapdata_decode(r_col + r_row * mPitch);
		do {
			switch (mapNum) {
			// 以下case番号は対応するTexture番号
			case 9:
			case 10:
				r_col ++;
				r_col ++;
				break;
			case 11:
				r_row ++;
				break;
			}
			mapNum = mapdata_decode(r_col + r_row * mPitch);
		} while (mapNum > SLOPE_TEXTURE_NUMBER);
		

		//	左側へ探索 --------------------------------------
		l_row = start_row, l_col = start_col;
		mapNum = mapdata_decode(l_col + l_row * mPitch);
		do {
			switch (mapNum) {
			// 以下case番号は対応するTexture番号
			case 9:
			case 11:
				l_col --;
				l_col --;
				break;
			case 10:
				l_row --;
				break;
			}
			mapNum = mapdata_decode(l_col + l_row * mPitch);
		} while (mapNum > SLOPE_BETWEEN_TEXTURE_NUMBER);
		l_col ++;
		l_col ++;

		//	坂道ベクトルを返す
		step->x = (float)((l_col >> MAP_CHIP_DATA_SIZE_BITS) << MAP_CHIP_SIZE_BITS);
		step->y = (float)(l_row << MAP_CHIP_SIZE_BITS);
		step->vx = (float)((r_col >> MAP_CHIP_DATA_SIZE_BITS) << MAP_CHIP_SIZE_BITS) - step->x;
		step->vy = (float)(r_row << MAP_CHIP_SIZE_BITS) - step->y;

	}
	else if (mapdata_decode(index) > SLOPE_BETWEEN_TEXTURE_NUMBER) {	//	右上がりの坂----------------------------------------

		// マップチップの番号情報
		int mapNum;

		//	右側へ探索 --------------------------------------
		r_row = start_row, r_col = start_col;
		mapNum = mapdata_decode(r_col + r_row * mPitch);
		do {
			switch (mapNum) {
			// 以下case番号は対応するTexture番号
			case 6:
			case 8:
				r_col ++;
				r_col ++;
				break;
			case 7:
				r_row --;
				break;
			}
			mapNum = mapdata_decode(r_col + r_row * mPitch);
		} while (mapNum > SLOPE_BETWEEN_TEXTURE_NUMBER);


		//	左側へ探索 --------------------------------------
		l_row = start_row, l_col = start_col;
		mapNum = mapdata_decode(l_col + l_row * mPitch);
		do {
			switch (mapNum) {			
			// 以下case番号は対応するTexture番号
			case 6:
			case 7:
				l_col --;
				l_col --;
				break;
			case 8:
				l_row ++;
				break;
			}
			mapNum = mapdata_decode(l_col + l_row * mPitch);
		} while (mapNum > SLOPE_BETWEEN_TEXTURE_NUMBER);
		l_col ++;
		l_col ++;
		

		//	坂道ベクトルを返す
		step->x = (float)((l_col >> MAP_CHIP_DATA_SIZE_BITS) << MAP_CHIP_SIZE_BITS);
		step->y = (float)(l_row << MAP_CHIP_SIZE_BITS);
		step->vx = (float)((r_col >> MAP_CHIP_DATA_SIZE_BITS) << MAP_CHIP_SIZE_BITS) - step->x;
		step->vy = (float)(r_row << MAP_CHIP_SIZE_BITS) - step->y;

	}

}


// BGのMAPDATA構造体を更新
void BG::resetMapData(int num) {

	switch (num) {
	case 1:
		map.data = MapData::data;
		map.length = MapData::length;
		break;

	case 2:
		if (MapData::length2 < 3)		// 3 = 必要最低限のマップ要素数 => なければreturn
			break;
		map.data = MapData::data2;
		map.length = MapData::length2;
		break;

	case 3:
		if (MapData::length3 < 3)		// 3 = 必要最低限のマップ要素数 => なければreturn
			break;
		map.data = MapData::data3;
		map.length = MapData::length3;
		break;

	}

	//	Mapデータヘッダの読み取りとデコード
	mChipW = map.data[4] & MASK_BIT_8;
	mChipH = map.data[5] & MASK_BIT_8;
	mMapW = map.data[2] & MASK_BIT_8;
	mMapH = map.data[3] & MASK_BIT_8;
	mPitch = mMapW * MAP_BYTES_PER_ROW; // 横一行あたりのバイト数(配列の列数)
	mMapW *= mChipW;
	mMapH *= mChipH;

	m_iX = PLAYER_START_X;	// プレイヤーx座標
}