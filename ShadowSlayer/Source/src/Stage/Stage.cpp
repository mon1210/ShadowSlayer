// Windowsアプリケーション開発用の共通ヘッダファイル
#include "stdafx.h"
// Direct2Dのヘッダファイルをインクルード
#include <d2d1.h>
//双方向リスト (Doubly Linked List) データ構造を提供
#include <list>
// ゲーム内のステージ関連クラスと列挙型の定義
#include "Stage.h"
// テクスチャとビットマップの読み込みと管理に関するクラスとインターフェースを定義
#include "TextureLoader.h"
// プレイヤーキャラクターとその関連情報を定義したクラスと定数/列挙型の定義
#include "Player.h"
// ゲームの背景を管理するクラスと関連データを定義
#include "BG.h"
// 弾クラスの宣言
#include "Shot.h"
// 敵キャラクターセットアップと管理に関するクラスとデータの定義
#include "EnemySet.h"
// スラッシュコリジョン（プレイヤーの攻撃の当たり判定）クラスの宣言
#include "SlashCollision.h"
// 敵の攻撃とプレイヤーの当たり判定処理を行うクラスと関連する定数・列挙型の定義
#include "EnemyAttackCollision.h"
// ゲートキーパーオブジェクトに関するヘッダファイル
#include "GateKeeper.h"
// プレイヤーのUI要素を管理する
#include "UI.h"
// 定数値を定めたヘッダファイル
#include "Constants.h"


/**
* @brief Shotのコンストラクタ
*/
//float Stage::m_fMapHight;
Stage::Stage(Selector *pSystem)
{
	ID2D1RenderTarget *pTarget = NULL;

	m_bGameOver = false;
	m_pSystem = pSystem;
	m_ePhase = STAGE_INIT;
	m_bPause = true;
	m_pBG_tmp = NULL;
	m_iTimer = 0;
	m_iMapNum = 0;

	pTarget = m_pSystem->GetRenderTarget();
	if (pTarget) {
		m_pBG = new BG(pTarget);
		m_pPlayer = new Player(this, m_pBG);
		m_pPlayer->setMapWidth(m_pBG->getMapWidth());
		m_pPlayer->setMapHight(m_pBG->getMapHight());
		m_fMapWidth = m_pBG->getMapWidth();
		m_fMapHight = m_pBG->getMapHight();

		Shot::Restore(pTarget, m_pBG);
		m_pShots = new std::list<IGameObject*>();

		m_pEnemySet = new EnemySet(this);
		m_pEnemySet->Restore(pTarget, m_pBG);
		m_pEnemies = new std::list<IGameObject*>();

		m_pUI = new UI(m_pPlayer, pTarget);

		// X方向マップ移動用オブジェクト
		GateKeeper::Restore(pTarget);
		m_pGateKeeperX = new GateKeeper(this, m_fMapWidth - GATE_KEEPER1_POS_X, GATE_KEEPER1_POS_Y);
		m_pGateKeeperX_prev = NULL;

		// Y方向マップ移動用オブジェクト
		GateKeeper::Restore(pTarget);
		m_pGateKeeperY = new GateKeeper(this, GATE_KEEPER2_POS_X, m_fMapHight - GATE_KEEPER2_POS_Y);
		m_pGateKeeperY_prev = NULL;

		if (FAILED(pTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			&m_pBlackBrush
		))) {
			SAFE_RELEASE(m_pBlackBrush);
		}

#ifdef _DEBUG
		SlashCollision::Restore(m_pPlayer, pTarget);
#else
		CSlashCollision::Restore(m_pPlayer);
#endif
		m_pSlashCollisions = new std::list<IGameObject*>();

#ifdef _DEBUG
		EnemyAttackCollision::Restore(m_pPlayer, pTarget);
#else
		CEnemyAttackCollision::Restore(m_pPlayer);
#endif
		m_pEnemyAttackCollisions = new std::list<IGameObject*>();

	}
	SAFE_RELEASE(pTarget);

	m_fPlayerDrawX = m_pPlayer->m_fDrawX;
	m_fPlayerDrawY = m_pPlayer->m_fDrawY;
	m_fPlayerX = m_pPlayer->getX();
	m_fPlayerY = m_pPlayer->getY();

}


// デストラクタ
Stage::~Stage()
{
	// 全shotの強制削除
	if (m_pShots) {
		std::list<IGameObject*>::iterator it = m_pShots->begin();
		while (it != m_pShots->end()) {
			SAFE_DELETE(*it);
			it = m_pShots->erase(it);
		}
		delete m_pShots;
		m_pShots = NULL;
	}
	Shot::Finalize();

	// 全エネミーの強制削除
	if (m_pEnemies) {
		std::list<IGameObject*>::iterator it = m_pEnemies->begin();
		while (it != m_pEnemies->end()) {
			SAFE_DELETE(*it);
			it = m_pEnemies->erase(it);
		}
		delete m_pEnemies;
		m_pEnemies = NULL;
	}
	if(m_pEnemySet)
		m_pEnemySet->Finalize();

	// 全SlashCollisionの強制削除
	if (m_pSlashCollisions) {
		std::list<IGameObject*>::iterator it = m_pSlashCollisions->begin();
		while (it != m_pSlashCollisions->end()) {
			SAFE_DELETE(*it);
			it = m_pSlashCollisions->erase(it);
		}
		delete m_pSlashCollisions;
		m_pSlashCollisions = NULL;
	}
	SlashCollision::Finalize();


	// 全SlashCollisionの強制削除
	if (m_pEnemyAttackCollisions) {
		std::list<IGameObject*>::iterator it = m_pEnemyAttackCollisions->begin();
		while (it != m_pEnemyAttackCollisions->end()) {
			SAFE_DELETE(*it);
			it = m_pEnemyAttackCollisions->erase(it);
		}
		delete m_pEnemyAttackCollisions;
		m_pEnemyAttackCollisions = NULL;
	}
	EnemyAttackCollision::Finalize();

	//	GateKeeper削除
	SAFE_DELETE(m_pGateKeeperX);
	SAFE_DELETE(m_pGateKeeperY);
	SAFE_DELETE(m_pGateKeeperX_prev);
	SAFE_DELETE(m_pGateKeeperY_prev);
	GateKeeper::Finalize();
	
	SAFE_RELEASE(m_pBlackBrush);
	SAFE_DELETE(m_pUI);
	SAFE_DELETE(m_pEnemySet);
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pBG);
}


/**
* @brief アニメーション(1フレーム)の実行
* @note 処理の順番は オブジェクトを move してから各々 collide を取って、必要なら各座標を調整する
* @return GAMESCENE_DEFAULT: 継続 / GAMESCENE_END_FAILURE: ゲームオーバー
*/
GameSceneResultCode Stage::move() {

	switch (m_ePhase) {
	case STAGE_INIT:
		m_ePhase = STAGE_RUN;
		break;

	case STAGE_RUN:
		// if( (m_iMapNum = 門番オブジェクト->collide()) > 0)	//	門番オブジェクトのcollide()は当たりなら次のマップの番号を、当たりじゃないなら-1を返す。
		// 右から1マスの場所に行ったら
		if (m_fPlayerX > m_fMapWidth - GATE_KEEPER1_POS_X) {	// X軸方向マップ遷移
			if (m_iMapNum++ > MAP_NUMBER_MAX)	// 移動のたびに ++
			{
				m_ePhase = STAGE_FADE;
				m_iTimer = 0;
				break;
			}
			m_pPlayer->clearVX();
			m_ePhase = STAGE_MAPCHANGE_X;
			m_eMapNum = RIGHT_STAGE;
			ID2D1RenderTarget* pTarget = m_pSystem->GetRenderTarget();
			m_pBG_tmp = new BG(pTarget);
			SAFE_RELEASE(pTarget);
			m_pBG_tmp->resetMapData(2);
			m_iTimer = 0;
			break;
		}
		// 下から1マスの場所に行ったら
		else if (m_fPlayerY > m_fMapHight - GATE_KEEPER2_POS_Y) {	// Y軸方向マップ遷移
			if (m_iMapNum++ > MAP_NUMBER_MAX)	// 移動のたびに ++
			{
				m_ePhase = STAGE_FADE;
				m_iTimer = 0;
				break;
			}
			m_pPlayer->clearVY();
			m_ePhase = STAGE_MAPCHANGE_Y;
			m_eMapNum = UNDER_STAGE;
			ID2D1RenderTarget* pTarget = m_pSystem->GetRenderTarget();
			m_pBG_tmp = new BG(pTarget);
			SAFE_RELEASE(pTarget);
			m_pBG_tmp->resetMapData(3);
			m_iTimer = 0;
			break;
		}

		//	ポーズ画面呼び出し
		if (GetAsyncKeyState(0x50)) { // P
			if (!m_bPause) {
				m_ePhase = STAGE_PAUSE;
				m_bPause = true;
				break;
			}
		}
		else {
			m_bPause = false;
		}

		if (m_pPlayer) {
			if (!m_pPlayer->move())
			{
				m_ePhase = STAGE_FADE;
				m_iTimer = 0;
				break;
			}
			m_pPlayer->collideWithMap();
			m_fPlayerDrawX = m_pPlayer->calcPlayerDrawX();
			m_fPlayerDrawY = m_pPlayer->calcPlayerDrawY();
			m_fPlayerX = m_pPlayer->getX();
			m_fPlayerY = m_pPlayer->getY();
		}


		m_pBG->setX(m_pPlayer->getX());
		m_pUI->move();

		// エネミーのリスポーン
		if (m_pEnemies && m_pEnemySet) {
			IGameObject* pObj;
			float x = m_fPlayerX + ENEMY_SPOWN_POS_X;
			do {
				pObj = m_pEnemySet->GetEnemyToSet(x);
				if (pObj != NULL) {
					m_pEnemies->push_back(pObj);
				}
			} while (pObj);
		}
		// エネミーの処理
		if (m_pEnemies) {
			std::list<IGameObject*>::iterator it = m_pEnemies->begin();
			while (it != m_pEnemies->end()) {
				if ((*it)->move()) {
					(*it)->collideWithMap();
					++it;
				}
				else {
					SAFE_DELETE(*it);
					it = m_pEnemies->erase(it);
				}
			}
		}


		// shotの処理
		if (m_pShots) {
			std::list<IGameObject*>::iterator it = m_pShots->begin();
			while (it != m_pShots->end()) {
				if ((*it)->move()) {
					(*it)->collideWithMap();
					++it;
				}
				else {
					SAFE_DELETE(*it);
					it = m_pShots->erase(it);
				}
			}
		}

		// slashの処理
		if (m_pSlashCollisions) {
			std::list<IGameObject*>::iterator it = m_pSlashCollisions->begin();
			while (it != m_pSlashCollisions->end()) {
				if ((*it)->move()) {
					++it;
				}
				else {
					SAFE_DELETE(*it);
					it = m_pSlashCollisions->erase(it);
				}
			}
		}

		// EnemyAttackの処理
		if (m_pEnemyAttackCollisions) {
			std::list<IGameObject*>::iterator it = m_pEnemyAttackCollisions->begin();
			while (it != m_pEnemyAttackCollisions->end()) {
				if ((*it)->move()) {
					++it;
				}
				else {
					SAFE_DELETE(*it);
					it = m_pEnemyAttackCollisions->erase(it);
				}
			}
		}

		// Slashと敵の判定
		if (m_pSlashCollisions && m_pEnemies) {
			std::list<IGameObject*>::iterator it = m_pSlashCollisions->begin();
			std::list<IGameObject*>::iterator it2;

			while (it != m_pSlashCollisions->end()) {
				it2 = m_pEnemies->begin();
				while (it2 != m_pEnemies->end()) {
					if ((*it2)->collide((*it))) {
						(*it)->damage(1.f);
						if ((*it2)->damage((*it)->m_fDamage)) {
							m_pPlayer->setMana(KILL_ENEMY_GET_MANA);
						}
					}
					++it2;
				}
				++it;
			}
		}

		//	EnemyAttackとプレイヤーの判定
		if (m_pEnemyAttackCollisions && m_pPlayer) {
			std::list<IGameObject*>::iterator it = m_pEnemyAttackCollisions->begin();
			while (it != m_pEnemyAttackCollisions->end()) {
				if (m_pPlayer->collide((*it))) {
					m_pPlayer->damage(ENEMY_ATTACK_POWER);
					(*it)->damage(PLAYER_ATTACK_POWER);
				}
				++it;
			}
		}

		break;
		// -------------STAGE_RUN END-----------------------

	case STAGE_PAUSE:
		if (GetAsyncKeyState(0x50)) {	// P
			if (!m_bPause) {
				m_ePhase = STAGE_RUN;
				m_bPause = true;
				GetAsyncKeyState(VK_LEFT);
				GetAsyncKeyState(VK_RIGHT);
				GetAsyncKeyState(VK_SPACE);
				GetAsyncKeyState(0x47);
				GetAsyncKeyState(0x46);
				break;
			}
		}
		else {
			m_bPause = false;
		}

		break;

	// X軸方向にマップ移動
	case STAGE_MAPCHANGE_X:
		if (m_iTimer++ < MAP_CHANGE_TIMER) {	// 60フレームかけてステージ移動
			// プレイヤー座標移動
			m_pPlayer->setDrawX(
				((m_fPlayerDrawX * (MAP_CHANGE_TIMER - m_iTimer))
					+ MAP_CHANGE_PLAYER_MOVE * m_iTimer) / PER_FLOAT
			);
			break;
		}
		//	前マップの各種ゲームオブジェクト削除
		clearGameObjects();
		SAFE_DELETE(m_pBG);
		m_pBG = m_pBG_tmp;
		m_pBG_tmp = NULL;

		// 門番オブジェクトの削除と更新
		// これがないと移動後も生成される
		SAFE_DELETE(m_pGateKeeperX);
		SAFE_DELETE(m_pGateKeeperY);
		SAFE_DELETE(m_pGateKeeperX_prev);
		SAFE_DELETE(m_pGateKeeperY_prev);
		GateKeeper::Finalize();

		//	次マップのデータで各種メンバを更新
		setGameObjects();

		m_iTimer = 0;
		m_ePhase = STAGE_MAPCHANGE_DONE;
		
		break;
	
	// Y軸方向にマップ移動
	case STAGE_MAPCHANGE_Y:
		if (m_iTimer++ < MAP_CHANGE_TIMER) {	// 60フレームかけてステージ移動
			// プレイヤー座標移動
			m_pPlayer->setDrawY(
				((m_fPlayerDrawY * (MAP_CHANGE_TIMER - m_iTimer))
					+ MAP_CHANGE_PLAYER_MOVE * m_iTimer) / PER_FLOAT
			);
			break;
		}

		//	前マップの各種ゲームオブジェクト削除
		clearGameObjects();
		SAFE_DELETE(m_pBG);
		m_pBG = m_pBG_tmp;
		m_pBG_tmp = NULL;

		// 門番オブジェクトの削除と更新
		// これがないと移動後も生成される
		SAFE_DELETE(m_pGateKeeperX);
		SAFE_DELETE(m_pGateKeeperY);
		SAFE_DELETE(m_pGateKeeperX_prev);
		SAFE_DELETE(m_pGateKeeperY_prev);
		GateKeeper::Finalize();

		//	次マップのデータで各種メンバを更新
		setGameObjects();

		m_iTimer = 0;
		m_ePhase = STAGE_MAPCHANGE_DONE;

		break;

	case STAGE_MAPCHANGE_DONE:
		m_iTimer++;
		if (m_iTimer > MAP_CHANGE_DONE_TIMER) {
			m_ePhase = STAGE_RUN;
		}
		break;

	case STAGE_FADE:
		// MAP_NUMBER_MAXを超えると
		if (m_iTimer++ < STAGE_FADE_TIMER)
		{
			break;
		}
		m_ePhase = STAGE_DONE;
		break;

	case STAGE_DONE:
		return GAMESCENE_END_OK;
		}

		return GAMESCENE_DEFAULT;
}


/**
* @brief	各種オブジェクトのクリア
* @note		マップ遷移時に使用
*/
void Stage::clearGameObjects() {
	// 全shotの強制削除
	if (m_pShots) {
		std::list<IGameObject*>::iterator it = m_pShots->begin();
		while (it != m_pShots->end()) {
			SAFE_DELETE(*it);
			it = m_pShots->erase(it);
		}
	}
	// 全エネミーの強制削除
	if (m_pEnemies) {
		std::list<IGameObject*>::iterator it = m_pEnemies->begin();
		while (it != m_pEnemies->end()) {
			SAFE_DELETE(*it);
			it = m_pEnemies->erase(it);
		}
	}
	if (m_pEnemySet) {
		m_pEnemySet->Finalize();
		SAFE_DELETE(m_pEnemySet);
	}
	// 全SlashCollisionの強制削除
	if (m_pSlashCollisions) {
		std::list<IGameObject*>::iterator it = m_pSlashCollisions->begin();
		while (it != m_pSlashCollisions->end()) {
			SAFE_DELETE(*it);
			it = m_pSlashCollisions->erase(it);
		}
	}
	// 全EnemyAttackCollisionの強制削除
	if (m_pEnemyAttackCollisions) {
		std::list<IGameObject*>::iterator it = m_pEnemyAttackCollisions->begin();
		while (it != m_pEnemyAttackCollisions->end()) {
			SAFE_DELETE(*it);
			it = m_pEnemyAttackCollisions->erase(it);
		}
	}
}


/**
* @brief	次マップに使うデータ・オブジェクトを設定
* @note		m_pBGを更新してからこの関数を呼ぶように
*/
void Stage::setGameObjects() {
	// enumMapNum変数でマップ移動後のプレイヤー描描画を場合分け
	switch (m_eMapNum) {
	case RIGHT_STAGE:// 右移動
		m_pPlayer->setPos(MAP_CHANGE_PLAYER_POS, m_pPlayer->getY());	//	ここで当たり判定も修正しておかないとcollideWithMap()でマップ右端へx座標が修正されてマップ遷移が連続してしまう
		break;
	case UNDER_STAGE:// 下移動
		m_pPlayer->setPos(m_pPlayer->getX(), MAP_CHANGE_PLAYER_POS);	//	ここで当たり判定も修正しておかないとcollideWithMap()でマップ右端へx座標が修正されてマップ遷移が連続してしまう
		break;
	case UPPER_STAGE:
		break;
	default:
		break;
	}
	// setPosでマップ移動後、プレイヤー描画位置変更
	m_fPlayerX = m_pPlayer->getX();
	m_fPlayerY = m_pPlayer->getY();
	m_fPlayerDrawX = m_pPlayer->calcPlayerDrawX();
	m_fPlayerDrawY = m_pPlayer->calcPlayerDrawY();
	m_pPlayer->resetBG(m_pBG);
	m_pPlayer->setMapWidth(m_pBG->getMapWidth());
	m_pPlayer->setMapHight(m_pBG->getMapHight());
	Shot::resetBG(m_pBG);
	m_fMapWidth = m_pBG->getMapWidth();
	m_fMapHight = m_pBG->getMapHight();

	
}


/**
* @brief 描画処理
* @note  現在のマップと次のマップを動かしながら描画することで移動しているように見せる
*/
void Stage::draw(ID2D1RenderTarget *pRenderTarget) {

	switch (m_ePhase) {
	case STAGE_MAPCHANGE_X:		// 右に移動
		{
			float ratio = m_iTimer / PER_FLOAT;
			//	次のマップ(左に移動)
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(MAP_CHANGE_X_MOVE_X * (MAP_CHANGE_SPEED - ratio), MAP_CHANGE_X_MOVE_Y));
			m_pBG_tmp->draw(pRenderTarget);
			//	現在のマップ(左に移動)
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(-MAP_CHANGE_X_MOVE_X * ratio, MAP_CHANGE_X_MOVE_Y));
			if (m_pBG)
				m_pBG->draw(pRenderTarget);
			if (m_pShots) {
				std::list<IGameObject*>::iterator it = m_pShots->begin();
				while (it != m_pShots->end()) {
					(*it++)->draw(pRenderTarget);
				}
			}
			if (m_pEnemies) {
				std::list<IGameObject*>::iterator it = m_pEnemies->begin();
				while (it != m_pEnemies->end()) {
					(*it++)->draw(pRenderTarget);
				}
			}
			//	プレイヤーとUI
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			if (m_pPlayer)
				m_pPlayer->draw(pRenderTarget);
			if (m_pUI)
				m_pUI->draw(pRenderTarget);
		}
		break;

	case STAGE_MAPCHANGE_Y:		// 下に移動
	{
		float ratio = m_iTimer / PER_FLOAT;
		//	次のマップ(上に移動)
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(MAP_CHANGE_Y_MOVE_X, MAP_CHANGE_Y_MOVE_Y * (MAP_CHANGE_SPEED - ratio)));
		m_pBG_tmp->draw(pRenderTarget);
		//	現在のマップ(上に移動)
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(MAP_CHANGE_Y_MOVE_X,-MAP_CHANGE_Y_MOVE_Y * ratio));
		if (m_pBG)
			m_pBG->draw(pRenderTarget);
		if (m_pShots) {
			std::list<IGameObject*>::iterator it = m_pShots->begin();
			while (it != m_pShots->end()) {
				(*it++)->draw(pRenderTarget);
			}
		}
		if (m_pEnemies) {
			std::list<IGameObject*>::iterator it = m_pEnemies->begin();
			while (it != m_pEnemies->end()) {
				(*it++)->draw(pRenderTarget);
			}
		}
		//	プレイヤーとUI
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		if (m_pPlayer)
			m_pPlayer->draw(pRenderTarget);
		if (m_pUI)
			m_pUI->draw(pRenderTarget);
	}
	break;

	default:
		if (m_pBG)
			m_pBG->draw(pRenderTarget);
		if (m_pGateKeeperX)
			m_pGateKeeperX->draw(pRenderTarget);
		if (m_pGateKeeperY)
			m_pGateKeeperY->draw(pRenderTarget);
		if (m_pGateKeeperX_prev)
			m_pGateKeeperX_prev->draw(pRenderTarget);
		if (m_pPlayer)
			m_pPlayer->draw(pRenderTarget);
		if (m_pUI)
			m_pUI->draw(pRenderTarget);
		if (m_pShots) {
			std::list<IGameObject*>::iterator it = m_pShots->begin();
			while (it != m_pShots->end()) {
				(*it++)->draw(pRenderTarget);
			}
		}
		if (m_pEnemies) {
			std::list<IGameObject*>::iterator it = m_pEnemies->begin();
			while (it != m_pEnemies->end()) {
				(*it++)->draw(pRenderTarget);
			}
		}
		break;
	}

	switch (m_ePhase)
	{
	case STAGE_FADE:
	case STAGE_DONE:
		{
			float opacity = (float)m_iTimer / STAGE_DONE_TIMER;
			
			D2D1_SIZE_F size;
			size = pRenderTarget->GetSize();

			D2D1_RECT_F		rc;		// 描画領域(画面上での位置やサイズ)を指定する変数
			rc.left = 0;	// 描画領域の左上隅指定
			rc.top = 0;		//			〃
			rc.right = size.width;		// 描画領域の右下隅指定
			rc.bottom = size.height;	//			〃
			m_pBlackBrush->SetOpacity(opacity);
			pRenderTarget->FillRectangle(rc, m_pBlackBrush);
		}
		break;
	default:
		break;
	}
	

#ifdef _DEBUG
	if (m_pSlashCollisions) {
		std::list<IGameObject*>::iterator it = m_pSlashCollisions->begin();
		while (it != m_pSlashCollisions->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}

	if (m_pEnemyAttackCollisions) {
		std::list<IGameObject*>::iterator it = m_pEnemyAttackCollisions->begin();
		while (it != m_pEnemyAttackCollisions->end()) {
			(*it++)->draw(pRenderTarget);
		}
	}
#endif

}


void Stage::addShots(IGameObject *pObj) {
	if (m_pShots) {
		m_pShots->push_back(pObj);
	}
}


void Stage::addSlashes(IGameObject *pObj) {
	if (m_pSlashCollisions) {
		m_pSlashCollisions->push_back(pObj);
	}
}


void Stage::addEnemyAttacks(IGameObject *pObj) {
	if (m_pEnemyAttackCollisions) {
		m_pEnemyAttackCollisions->push_back(pObj);
	}
}


/**
* @brief ID2D1RenderTarget を取得して返す
* @note このメソッドで受け取った RenderTarget は使用後必ずRelease すること
* @return ID2D1RenderTarget オブジェクト
*/
ID2D1RenderTarget *Stage::GetRenderTarget() {
	ID2D1RenderTarget *result = NULL;
	if (m_pSystem) {
		result = m_pSystem->GetRenderTarget();
	}
	return result;
}