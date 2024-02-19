/**
* @file  Stage.h
* @brief クラス Stage の宣言
*/
#include "..\Selector\Selector.h"
#include <list>
#include "..\IGameObject\IGameObject.h"

class Player;
class BG;
class EnemySet;
class UI;
class GateKeeper;
struct ID2D1SolidColorBrush;

enum StagePhase {
	STAGE_INIT = 0,
	STAGE_RUN = 1,
	STAGE_PAUSE = 2,
	STAGE_FADE = 3,
	STAGE_DONE = 4,
	STAGE_MAPCHANGE_X = 5,
	STAGE_MAPCHANGE_Y= 6,
	STAGE_MAPCHANGE_DONE = 7,
};

enum MapNum
{
	DEFAULT_STAGE = 0,
	RIGHT_STAGE = 1,
	UNDER_STAGE = 2,
	UPPER_STAGE = 3,

};

class Stage : public IGameScene
{
public:
	Stage(Selector *pSystem);
	virtual ~Stage(void);
	virtual GameSceneResultCode move() override; // アニメーション(1フレーム)の実行　
												 // 処理の順番は オブジェクトを move してから各々 collide を取って、必要なら各座標を調整する
												 // return GAMESCENE_DEFAULT: 継続 / GAMESCENE_END_FAILURE: ゲームオーバー
	virtual void draw(ID2D1RenderTarget *pRenderTarget) override;	// 描画メソッド
	virtual void addShots(IGameObject *pObj);
	virtual void addSlashes(IGameObject *pObj);
	virtual void addEnemyAttacks(IGameObject *pObj);
	virtual void clearGameObjects();	// 各種オブジェクトのクリア　マップ遷移時に使用
	virtual void setGameObjects();		// 次マップに使うデータ・オブジェクトを設定　m_pBGを更新してからこの関数を呼ぶ
	ID2D1RenderTarget *GetRenderTarget();	// ID2D1RenderTarget を取得して返す
	float	m_fPlayerDrawX;	//	プレイヤーの描画時の画面上座標。カメラの焦点をプレイヤーに合わせているので、他のオブジェクトはプレイヤーを起点に描画する。データと描画を分けて考えるのが重要。
	float	m_fPlayerDrawY;
	float	m_fPlayerX;		//	プレイヤーのデータ上の座標。
	float	m_fPlayerY;
	float	m_fMapWidth;	//	マップの長さ = マップの右端の座標
	float	m_fMapHight;
protected:
	Selector	*m_pSystem;
	ID2D1SolidColorBrush	*m_pBlackBrush;
	bool		m_bGameOver;
	Player		*m_pPlayer;
	BG			*m_pBG;
	BG			*m_pBG_tmp;	// ステージ遷移に使用
	GateKeeper	*m_pGateKeeperX;
	GateKeeper	*m_pGateKeeperY;
	GateKeeper	*m_pGateKeeperX_prev;
	GateKeeper	*m_pGateKeeperY_prev;
	EnemySet	*m_pEnemySet;
	UI			*m_pUI;
	StagePhase	m_ePhase;
	MapNum		m_eMapNum;
	bool		m_bPause;	//	ポーズボタン連打防止フラグ
	int		m_iTimer;
	int		m_iMapNum;		//	マップ番号
	std::list<IGameObject*>	*m_pShots;
	std::list<IGameObject*>	*m_pEnemies;
	std::list<IGameObject*> *m_pSlashCollisions;
	std::list<IGameObject*>	*m_pEnemyAttackCollisions;
};

