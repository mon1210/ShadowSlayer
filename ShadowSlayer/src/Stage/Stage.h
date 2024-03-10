/**
* @file  Stage.h
* @brief クラス Stage の宣言
*/
#include "..\Selector\Selector.h"
#include <list>
#include "..\IGameObject\IGameObject.h"
#include "..\Enums.h"

class Player;
class BG;
class EnemySet;
class UI;
class GateKeeper;
struct ID2D1SolidColorBrush;

class Stage : public IGameScene
{
public:

	/**
	* @brief Shotのコンストラクタ
	*/
	Stage(Selector *pSystem);

	/**
	* @brief Shotのコンストラクタ
	*/
	 ~Stage();

	/**
	* @brief アニメーション(1フレーム)の実行
	* @note 処理の順番は オブジェクトを move してから各々 collide を取って、必要なら各座標を調整する
	* @return GAMESCENE_DEFAULT: 継続 / GAMESCENE_END_FAILURE: ゲームオーバー
	*/
	GameSceneResultCode move() override;

	/**
	* @brief 描画処理
	* @note  現在のマップと次のマップを動かしながら描画することで移動しているように見せる
	*/
	void draw(ID2D1RenderTarget *pRenderTarget) override;

	/**
	* @brief	各種オブジェクトのクリア
	* @note		マップ遷移時に使用
	*/
	void clearGameObjects();

	/**
	* @brief	次マップに使うデータ・オブジェクトを設定
	* @note		m_pBGを更新してからこの関数を呼ぶように
	*/
	void setGameObjects();

	/**
	* @brief ID2D1RenderTarget を取得して返す
	* @note このメソッドで受け取った RenderTarget は使用後必ずRelease すること
	* @return ID2D1RenderTarget オブジェクト
	*/
	ID2D1RenderTarget *GetRenderTarget();

	void addShots(IGameObject* pObj);
	void addSlashes(IGameObject* pObj);
	void addEnemyAttacks(IGameObject* pObj);

	//! プレイヤーの描画時の画面上座標X。カメラの焦点をプレイヤーに合わせているので、他のオブジェクトはプレイヤーを起点に描画する。データと描画を分けて考えるのが重要。
	float	m_fPlayerDrawX;
	//! プレイヤーの描画時の画面上座標Y。カメラの焦点をプレイヤーに合わせているので、他のオブジェクトはプレイヤーを起点に描画する。データと描画を分けて考えるのが重要。
	float	m_fPlayerDrawY;
	//! プレイヤーのデータ上の座標X。
	float	m_fPlayerX;
	//! プレイヤーのデータ上の座標Y。
	float	m_fPlayerY;
	//! マップの長さ = マップの右端の座標
	float	m_fMapWidth;
	float	m_fMapHight;
protected:
	Selector	*m_pSystem;
	ID2D1SolidColorBrush	*m_pBlackBrush;
	bool		m_bGameOver;
	Player		*m_pPlayer;
	BG			*m_pBG;
	//! ステージ遷移に使用
	BG			*m_pBG_tmp;
	GateKeeper	*m_pGateKeeperX;
	GateKeeper	*m_pGateKeeperY;
	GateKeeper	*m_pGateKeeperX_prev;
	GateKeeper	*m_pGateKeeperY_prev;
	EnemySet	*m_pEnemySet;
	UI			*m_pUI;
	StagePhase	m_ePhase;
	MapNum		m_eMapNum;
	//! ポーズボタン連打防止フラグ
	bool		m_bPause;
	int		m_iTimer;
	//! マップ番号
	int		m_iMapNum;
	std::list<IGameObject*>	*m_pShots;
	std::list<IGameObject*>	*m_pEnemies;
	std::list<IGameObject*> *m_pSlashCollisions;
	std::list<IGameObject*>	*m_pEnemyAttackCollisions;
};

