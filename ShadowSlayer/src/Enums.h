/**
* @file	 Enums.h
* @brief eunm変数宣言用ヘッダファイル
* @note	 主にTexture関係
*/
#pragma once

enum GameOverPhase {
	GAMEOVER_INIT = 0,
	GAMEOVER_RUN = 1,
	GAMEOVER_FADE = 2,
	GAMEOVER_DONE = 3
};

enum GamePhase {
	GAMEPHASE_INIT = 0,
	GAMEPHASE_RESET = 1,
	GAMEPHASE_TITLE = 0x010,
	GAMEPHASE_GAME = 0x100,
	GAMEPHASE_GAMEOVER = 0x200
};

enum GameSceneResultCode {
	GAMESCENE_DEFAULT = 0,
	GAMESCENE_END_OK = 1,
	GAMESCENE_END_TIMEOUT = 2,
	GAMESCENE_END_FAILURE = 3
};

enum StagePhase {
	STAGE_INIT = 0,
	STAGE_RUN = 1,
	STAGE_PAUSE = 2,
	STAGE_FADE = 3,
	STAGE_DONE = 4,
	STAGE_MAPCHANGE_X = 5,
	STAGE_MAPCHANGE_Y = 6,
	STAGE_MAPCHANGE_DONE = 7,
};

enum MapNum
{
	DEFAULT_STAGE = 0,
	RIGHT_STAGE = 1,
	UNDER_STAGE = 2,
	UPPER_STAGE = 3,

};

enum TitlePhase {
	TITLE_INIT = 0,
	TITLE_RUN = 1,
	TITLE_FADE = 2,
	TITLE_DONE = 3
};

enum EnemyTexture
{
	AllMoveTexture = 16,		// 行動に関する全てのテクスチャ
	// X
	Default_X = 0,				// DefaultテクスチャのXのIndex
	AttackStance_X = 1,			// 攻撃の構えテクスチャのXのIndex
	Idle_X = 4,					// IdleテクスチャのXのIndex
	// Y
	IdleAndMove_Y = 8,			// Idleと移動テクスチャのYのIndex
	Attack_Y = 9,				// 攻撃動作テクスチャのYのIndex
	Death_Y = 10,				// 死亡テクスチャのYのIndex


};

enum PlayerTexture
{
	AllTexture = 16,				// 全てのテクスチャ
	WalkTextureTimesFour = 24,		// 6(移動テクスチャ) * 4
	RowPerElement = 7,				// 1行ごとの要素数
	// X
	UnusedTexture = 1,				// 使用しないテクスチャのXのIndex
	FirstAttackCollisionStart = 2,	// 一段目の攻撃の当たり判定生成開始テクスチャのXのIndex
	FirstAttackCollisionFinish = 4,	// 一段目の攻撃の当たり判定終了テクスチャのXのIndex
	Slide_X = 3,					// スライディングテクスチャのXのIndex
	Damaged_X = 3,					// ダメージテクスチャのXのIndex
	GetUp_X = 4,					// 起き上がりテクスチャのXのIndex
	SlideAttackCollisionBefore = 5, // スライディング攻撃の当たり判定生成前のテクスチャのXのIndex
	SlideAttackCollisionStart = 6,	// スライディング攻撃の当たり判定生成開始のテクスチャのXのIndex
	// Y
	Idle_Y = 7,						// IdleテクスチャのYのIndex
	Walk_Y = 8,						// 歩きテクスチャのYのIndex
	Slide_Y = 9,					// スライディングテクスチャのYのIndex
	GetUp_Y = 9,					// 起き上がりテクスチャのYのIndex
	FirstAttack_Y = 11,				// 一段目の攻撃テクスチャのYのIndex
	SecondAndThirdAttack_Y = 12,	// 二・三段目の攻撃テクスチャのYのIndex
	SlideAttack_Y = 14,				// スライディング攻撃テクスチャのYのIndex
	SlideAttack_Y2 = 15,			// スライディング撃テクスチャのYのIndex
	Damaged_Y = 16,					// ダメージテクスチャのYのIndex

};

enum AttackCount
{	// 読解性向上のため使用時は「AttacCount::」をつける
	First = 1,		// 一段目の攻撃
	Second = 2,		// 二段目の攻撃
	Third = 3,		// 三段目の攻撃
	Slide = 4		// スライディング攻撃

};

enum ShotTexture
{
	SRowPerElement = 7,		// 1行ごとの要素数(S => Shot)
	HitStart = 8,			// 衝突時のアニメーション開始番号
	StartDestroy = 5,		// ここから衝突時アニメーションへ
	ProjectileFlying = 4,	// 衝突していないときの描画番号

};