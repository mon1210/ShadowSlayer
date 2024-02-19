/**
* @file		Constants.h
* @brief	定数を定めたヘッダファイル
* @note		マジックナンバーの削除
*/
#pragma once
#include "Selector\Selector.h"	// FPS 取得用

// 以下BG用定数====================================================================================================== //
const int MASK_BIT_8 = 255;					// 下位8ビット以外のビットをクリア(256 = 2^8)
const int MAP_BYTES_PER_ROW = 2;			// 横一行あたりのバイト数を指定
const int PLAYER_START_X = 64;				// プレイヤー初期X座標
const int MAP_WIDTH_PIXELS = 1280;			// 40要素 * 32pixel
const int MAP_HEIGHT_PIXELS = 480;			// 15要素 * 32pixel
const int HALF_MAP_WIDTH_PIXELS = 640;		// 20要素 * 32pixel
const int QUARTER_MAP_WIDTH_PIXELS = 320;	// 1280 / 2 / 2
const int ROUNDING = 1;						// 切り上げための補正値
const int MAP1_WIDHT_CHIP_COUNT = 40;		// マップ1のマップチップの総数(横幅)
const int MAP1_HALF_WIDHT_CHIP_COUNT = 20;	// マップ1のマップチップの半分(横幅)
const int SLOPE_TEXTURE_NUMBER = 8;			// 坂の画像番号
const int SLOPE_START_TEXTURE_NUBER = 6;	// 坂の始まりの画像番号
const int SLOPE_BETWEEN_TEXTURE_NUMBER = 5;	// 坂の中間の画像番号
const int MAP_CHIP_SIZE_BITS = 5;			// マップチップの幅＆高さのビット(32 = 2^5)
const int FILE_INFO_BYTES = 6;				// ファイル情報をスキップ
const int MAP_CHIP_DATA_OFFSET = 7;			// マップチップデータ
const int MAP_CHIP_DATA_SIZE = 2;			// マップチップデータサイズ(二つのデータで一つのマップチップ)
const int MAP_CHIP_DATA_SIZE_BITS = 1;		// マップチップデータのBITサイズ
const float MAP_CHIP_SCALE_FACTOR = 1.0f;	// マップチップ描画の際の拡大率
const float BG_SCALE_FACTOR = 1.0f;			// 背景描画の際の拡大率
// 以上BG用定数====================================================================================================== //



// 以下CEnemy01用定数================================================================================================ //
const int ENEMY_MOVE_ABLE_TIMER = 59;		// 動けるようになるまでのタイマー
const int COLLISION_LIFE_TIME = 4;			// 攻撃の当たり判定時間
const int iMAP_CHIP_WIDTH = 32;				// マップチップサイズ(int)
const int DELETE_TEXTURE_COUNT = -1;		// 描画カウント削除処理(0も役割がある)
const int AT_COLLISION_GENERATE_COUNT = 8;	// 当たり判定生成開始カウント
const int AT_FINISH_COUNT = 15;				// 攻撃終了カウント
const float MOVE_SPEED = 2.0f;				// 移動スピード
const float AT_COLLISION_POS_X = 16.f;		// 攻撃当たり判定X座標
const float AT_COLLISION_WIDTH = 16.f;		// 攻撃当たり判定横幅
const float AT_COLLISION_HEIGHT = 25.5f;	// 攻撃当たり判定縦幅
const float FALL_SPEED_MAX = 31.f;			// 落下速度制限
const float DRAW_POS_X = 24.f;				// 描画X座標
const float DRAW_POS_Y = 37.5f;				// 描画Y座標
const float DRAW_WIDE = 48.f;				// 描画横幅
const float ENEMY_COLLISION_OFFSET_L = 8.f;	// 当たり判定枠左辺
const float ENEMY_COLLISION_OFFSET_T = 12.f;// 当たり判定枠上辺
const float ENEMY_LA_TEXTURE_COORD = 224.f;	// テクスチャの最終画像コード
const float ENEMY_TEXTURE_WIDTH = 32.f;		// テクスチャ横幅サイズ
const float ENEMY_TEXTURE_W_HALF = 16.f;	// テクスチャ横幅サイズの半分	
const float ENEMY_TEXTURE_HEIGHT = 25.f;	// テクスチャ縦幅サイズ
const float ENEMY_TEXTURE_H_HALF = 12.f;	// テクスチャ縦幅サイズの半分
const float ENEMY_SCALE_FACTOR = 1.0f;		// 描画時の拡大率
// 以上CEnemy01用定数================================================================================================ //



// 以下GameOver用定数================================================================================================ //
const int GAMEOVER_DONE_TIMER = 600;		// 終了までの時間
const int GAMEOVER_FADE_TIMER = 30;			// フェードアウト開始から終了までの時間
const float GAMEOVER_FADE_OUT_TIMER = 30.f;	// フェードアウトにかかる時間
// 以上GameOver用定数================================================================================================ //



// 以下GateKeeper用定数============================================================================================== //
const float GATE_KEEPER_WIDTH_SIZE = 64.f;	// 横幅
const float GATE_KEEPER_HEIGHT_SIZE = 96.f;	// 縦幅
const float GATE_KEEPER_SCALE_FACTOR = 1.0f;// 描画時の拡大率
// 以上GateKeeper用定数============================================================================================== //



// 以下My2DAction(main)用定数======================================================================================== //
const int WINDOW_BG_COLOR_OFFSET = 1;		// 背景色をカスタマイズするためのオフセット
const int WINDOW_POS_Y = 0;					// 生成時Y座標
const int WINDOW_WIDTH_SIZE = 640;			// ウィンドウの横幅
const int WINDOW_HEIGHT_SIZE = 480;			// ウィンドウの縦幅
const double DOUBLE_ONE = 1.0;				// 倍精度浮動小数点数リテラル
// 以上My2DAction(main)用定数======================================================================================== //



// 以下Player用定数================================================================================================== //
const int SHADOW_MODE_COUNT = 5;			// ShadowModeへの切り替え時の時間
const int PLAYER_DAMAGE_ANIM_TIME = 15;		// プレイヤーがダメージを受けた時のアニメーション時間
const int PLAYER_ATTTACK_TIME = 24;			// プレイヤーがアタックをするのに必要なアニメーション時間
const int PLAYER_VY_LIMIT = -50;			// プレイヤーのY軸に対する速さの制限(マップ移動時に使用)
const int ATTACK_THRESHOLD = 4;				// 攻撃時、構えモーションから当たり判定生成までの境界
const int ATTACK_AFTER_THRESHOLD = 12;		// 攻撃時、当たり判定生成から削除までの境界
const int ATTACK_END_THRESHOLD = 16;		// 攻撃時、当たり判定削除から攻撃終了までの境界
const int SL_ATTACK_THRESHOLD = 2;			// スライディング攻撃時、構えモーションから当たり判定生成までの境界
const int SL_ATTACK_AFTER_THRESHOLD = 6;	// スライディング攻撃時、当たり判定生成から削除までの境界
const int SL_ATTACK_END_THRESHOLD = 10;		// スライディング攻撃時、当たり判定削除から攻撃終了までの境界
const int ATTACK_COLLISION_GENERATE = 4;	// 攻撃時、当たり判定を生成するカウント
const int ATTACK_COLLISION_END = 15;		// 攻撃時、当たり判定を削除するカウント
const int ATTACK_END = 23;					// 攻撃モーションのすべてが終了
const int SL_ATTACK_END = 10;				// スライディング攻撃の全てが終了
const int SL_ATTACK_COLLISION_GENERATE = 2;	// スライディング攻撃時、当たり判定を生成するカウント
const int SL_DURATION_IN_FRAMES = FPS / 15; // スライディングの1秒間のアニメーションフレーム
const int SL_DUALATION_SECONDS = 11;		// スライディングにかかる秒数
const int PLAYER_INVINCIBLE_TIME = 60;		// 被ダメージ時の無敵時間
const int SECOND_ATTACK_THRESHOLD = 10;		// 二段目攻撃の境目
const int THIRD_ATTACK_THRESHOLD = 13;		// 三段目攻撃の境目
const int C_OK_SECOND_ATTACK_THRESHOLD = 20;// 攻撃キャンセル可能時の二段目攻撃の境目
const int C_OK_THIRD_ATTACK_THRESHOLD = 18;	// 攻撃キャンセル可能時の三段目攻撃の境目
const float SLIDE_DECELERATION_SPEED = 1.0f;// スライディング攻撃時の移動スピード減衰量
const float fMAP_CHIP_WIDTH = 32.f;			// マップチップサイズ(float)
const float SHADOW_DRAW_OPACITY = 0.1f;		// Shadow切り替え時、変化透明度
const float SHADOW_MANA_DEPLETION = 0.5f;	// ShadowMode時MANAの減少量
const float SHADOW_POS_X_TO_P = -10.f;		// プレイヤーに対するShadowのX座標
const float SHADOW_POS_Y_TO_P = 3.f;		// プレイヤーに対するShadowのY座標
const float SHADOW_SKEW_X_TO_P = 5.f;		// プレイヤーに対するShadowの斜度X
const float SHADOW_SKEW_Y_TO_P = 0.f;		// プレイヤーに対するShadowの斜度Y
const float SHADOW_SCALE_X_TO_P = 1.25f;	// プレイヤーに対するShadowの大きさX(w)
const float SHADOW_SCALE_Y_TO_P = 1.5f;		// プレイヤーに対するShadowの大きさY(h)
const float SHADOW_CHANGE_SPEED = 2.f;		// ShadowMode切り替え速度
const float SHADOW_CHANGE_DIRECTION = 8.f;	// ShadowMode切り替え時移動距離
const float SHADOW_AVAILABLE_MANA = 50.f;	// ShadowMode使用可能MANA
const float SHOT_DIRECTION = 1.f;			// Shotの方向を定める
const float PLAYER_MOVE_LIMIT = 32.1f;		// Playerの移動制限(1マップチップ + 0.1f)
const float PLAYER_TEXTURE_WIDTH = 50.f;	// プレイヤーテクスチャ横幅サイズ
const float PLAYER_TEXTURE_W_TWICE = 100.f;	// プレイヤーテクスチャ横幅サイズの二倍	
const float PLAYER_TEXTURE_HEIGHT = 37.f;	// プレイヤーテクスチャ縦幅サイズ
const float PLAYER_TEXTURE_H_TWICE = 74.f;	// プレイヤーテクスチャ縦幅サイズの二倍
const float PLAYER_LASTTEXTURE_POS = 300.f;	// 最も右のテクスチャの座標(50 * (7 - 1))
const float MANA_LIMIT = 100.f;				// MANAの最大値
const float PLAYER_KNOCK_BACK = 0.5f;		// 被ダメージ時ノックバック距離
const float PLAYER_DAMAGE_OPACITY = 0.5f;	// 被ダメージ時透明度(半透明)
const float PLAYER_MAX_OPACITY = 1.0f;		// 最大透明度(完全透明)
// 以上Player用定数================================================================================================== //



// 以下Shot用定数==================================================================================================== //
const int SHOT_LIFE_TIME = 20;				// 当たり判定なしの時消えるまでの時間
const int SHOT_DESTROY_TIME = 15;			// 接触後消えるまでの時間
const float SHOT_RECTANGLE_HALF_SIZE = 64.f;// 描画用矩形の幅(高さ)の半分
const float SHOT_RECTANGLE_SIZE = 128.f;	// 描画用矩形の幅(高さ)
const float SHOT_TEXTURE_WIDTH = 32.f;		// テクスチャの横幅
const float SHOT_TEXTURE_HEIGHT = 32.f;		// テクスチャの縦幅
const float SHOT_LASTTEXTURE_POS = 192.f;	// 最も右のテクスチャの座標(32 * (7 - 1))
const float SHOT_COLLISION_OFFSET_W = 49.f;	// 当たり判定枠横幅
const float SHOT_COLLISION_OFFSET_T = 5.f;	// 当たり判定枠上座標
const float SHOT_COLLISION_OFFSET_B = 10.f;	// 当たり判定枠下座標
const float SHOT_COLLISION_HEIGHT = 15.f;	// 
const float SHOT_DAMAGE = 10.f;				// Shotの攻撃力
const float SHOT_SCALE_FACTOR = 1.0f;		// Shot描画の際の拡大率
// 以上Shot用定数==================================================================================================== //



// 以下SlashCollision用定数========================================================================================== //
const float SHADOW_START_LENGTH = 1.f;		// shadowlenght初期値
const float SHADOW_ON_LENGTH = 1.25f;		// Shadowモード時の攻撃範囲
const float SLASH_DAMAGE = 10.f;			// 攻撃のダメージ
const float SHADOW_SLASH_DAMAGE = 15.f;		// Shadowモード時の攻撃ダメージ
const float R_SLASH_OFFSET_X = 10.f;		// FaceToRight(右向き)時の攻撃範囲X座標オフセット(1,2段目)
const float R_THIRD_SLASH_OFFSET_X = 10.f;	//			〃		(3段目)
const float R_DASH_SLASH_OFFSET_X = 10.f;	// 			〃		(ダッシュ)
const float L_SLASH_OFFSET_X = -50.f;		// FaceToLeft(左向き)時の攻撃範囲X座標オフセット(1,2段目)
const float L_THIRD_SLASH_OFFSET_X = -60.f;	// 			〃		(3段目)
const float L_DASH_SLASH_OFFSET_X = -65.f;	// 			〃		(ダッシュ)
const float SLASH_OFFSET_Y = -70.f;			// 攻撃範囲のY座標オフセット(1,2段目)
const float THIRD_SLASH_OFFSET_Y = -50.f;	// 			〃		(3段目)
const float DASH_SLASH_OFFSET_Y = -50.f;	// 			〃		(ダッシュ)
const int SLASH_LIFE_TIME = 5;				// 攻撃の持続時間
const int SLASH_DAMAGE_LIFE_TIME = 8;		// 攻撃ダメージの持続時間
const int SLASH_RETURN_DAMAGE = 1;			// damageメソッドの戻り値
const int INVINCIBLE_TIME = -1;			// 当たり判定削除
// 以上SlashCollision用定数========================================================================================== //



// 以下Stage用定数=================================================================================================== //
const int MAP_NUMBER_MAX = 0;				// マップの最大値
const int MAP_CHANGE_TIMER = 60;			// マップ移動時のプレーヤー描画
const int MAP_CHANGE_DONE_TIMER = 6;		// マップ移動終了
const int MAP_CHANGE_PLAYER_MOVE = 64;		// マップ移動時のプレイヤー移動距離
const int STAGE_FADE_TIMER = 120;			// マップフェードアウト時Phase待機用
const float MAP_CHANGE_PLAYER_POS = 64.f;	// マップ移動後、プレイヤーの座標
const float GATE_KEEPER1_POS_X = 32.f;		// ゲートキーパーオブジェクト1のX座標調整用
const float GATE_KEEPER1_POS_Y = 224.f;		// ゲートキーパーオブジェクト1のY座標
const float GATE_KEEPER2_POS_X = 32.f;		// ゲートキーパーオブジェクト2のX座標
const float GATE_KEEPER2_POS_Y = 32.f;		// ゲートキーパーオブジェクト1のY座標調整用
const float ENEMY_SPOWN_POS_X = 640.f;		// プレイヤーとエネミーがこの距離以下でスポーン
const float KILL_ENEMY_GET_MANA = 10.f;		// エネミー撃破時獲得MANA
const float ENEMY_ATTACK_POWER = 10.f;		// エネミーの攻撃力(プレイヤーの受けるダメージ)
const float PLAYER_ATTACK_POWER = 1.f;		// プレイヤーの攻撃力(エネミーの受けるダメージ)
const float MAP_CHANGE_X_MOVE_X = 640.f;	// マップ移動時、現在のマップと次のマップのX軸に対する移動距離
const float MAP_CHANGE_X_MOVE_Y = 0.f;		// マップ移動時、現在のマップと次のマップのY軸に対する移動距離
const float MAP_CHANGE_Y_MOVE_Y = 480.f;	// マップ移動時、現在のマップと次のマップのY軸に対する移動距離
const float MAP_CHANGE_Y_MOVE_X = 0.f;		// マップ移動時、現在のマップと次のマップのX軸に対する移動距離
const float MAP_CHANGE_SPEED = 1.f;			// マップ移動時速度調整
const float STAGE_DONE_TIMER = 120.f;		// ステージ移動後フェードアウト用
// 以上Stage用定数=================================================================================================== //



// 以下Title用定数=================================================================================================== //
const int TITLE_TIMER_INIT = 0;				// タイトル開始時のタイマー初期値
const int TITLE_DONE_TIME = 0;				// タイトルフェードアウト開始時の初期化値
const int TITLE_FADEOUT_TIME = 30;			// タイトルフェードアウトに要する時間
const int TITLE_LOOP_FRAME = 1516;			// タイトルループ周期
const int TITLE_LOOP_HALF_FRAME = 758;		// 周期の半分(なめらかな描画ループ)
const int CENTER = 2;						// 画面のセンター指定用
const int VERTICAL_SHIFT_AMOUNT = 40;		// PressSpaceを上にシフトする
const int TEXT_OPACITY_TIMER = 150;			// PressSpaceの透明度を測る数値
const int DARK_THRESHOLD = 60;				// この数値以下はだんだん透明
const int LIGHT_THRESHOLD = 90;				// この数値以上はだんだん鮮明
const int DARK_START_OPACITY = 2;			// だんだん透明になる初期値
const float MAX_OPACITY = 1.0f;				// 透明度の最大値
const float PER_FLOAT = 60.f;				// フレーム値に
const float DARK_THRESHOLD_FLAME = 30;		// 透明と鮮明値の差
const float TITLE_MARGIN_ABOVE = 120.f;		// タイトルを上に余白を開ける
const float TITLE_SCALE_FACTOR = 1.0f;		// タイトル描画の拡大率
const float TITLE_BLACKOUT_TIME = 30.0f;	// タイトルのフェードアウト時画面が暗い時間
// 以上Title用定数=================================================================================================== //



// 以下UI用定数====================================================================================================== //
const float INITIAL_HP = 100.f;				// HPバーの初期値
const float INITIAL_MANA = 100.f;			// MANAバーの初期値
const float HP_BAR_RC_LEFT = 26.f;			// HPバー描画領域左上隅指定情報
const float HP_BAR_RC_TOP = 27.f;			// HPバー描画領域左上隅指定情報
const float MANA_BAR_RC_LEFT = 26.f;		// MANAバー描画領域左上隅指定情報
const float MANA_BAR_RC_TOP = 51.f;			// MANAバー描画領域左上隅指定情報
const float BAR_FLAME_RC_LEFT = 10.f;		// フレーム描画領域左上隅指定情報
const float BAR_FLAME_RC_TOP = 25.f;		// フレーム描画領域左上隅指定情報
const float HP_TEXTURE_WIDTH = 66.f;		// HPバーのテクスチャ横幅
const float HP_TEXTURE_HEIGHT = 7.f;		// HPバーのテクスチャ縦幅
const float MANA_TEXTURE_WIDTH = 66.f;		// MANAバーのテクスチャ横幅
const float MANA_TEXTURE_HEIGHT = 7.f;		// MANAバーのテクスチャ縦幅
const float FLAME_TEXTURE_WIDTH = 80.f;		// フレームのテクスチャ横幅
const float FLAME_TEXTURE_HEIGHT = 21.f;	// フレームのテクスチャ縦幅
const float BAR_SCALE_FACTOR = 1.0f;		// バー描画の拡大率
const float FLAME_SCALE_FACTOR = 1.0f;		// フレーム描画拡大率

const float HP_TEXTURE_WIDTH_TWICE = HP_TEXTURE_WIDTH * 2;
const float HP_TEXTURE_HEIGHT_TWICE = HP_TEXTURE_HEIGHT * 2;
const float MANA_TEXTURE_WIDTH_TWICE = MANA_TEXTURE_WIDTH * 2;
const float MANA_TEXTURE_HEIGHT_TWICE = MANA_TEXTURE_HEIGHT * 2;
const float FLAME_TEXTURE_WIDTH_TWICE = FLAME_TEXTURE_WIDTH * 2;
const float FLAME_TEXTURE_HEIGHT_TWICE = FLAME_TEXTURE_HEIGHT * 2;
// 以上UI用定数====================================================================================================== //
