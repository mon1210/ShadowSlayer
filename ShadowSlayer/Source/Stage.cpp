// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "stdafx.h"
// Direct2D�̃w�b�_�t�@�C�����C���N���[�h
#include <d2d1.h>
//�o�������X�g (Doubly Linked List) �f�[�^�\�����
#include <list>
// �Q�[�����̃X�e�[�W�֘A�N���X�Ɨ񋓌^�̒�`
#include "Stage.h"
// �e�N�X�`���ƃr�b�g�}�b�v�̓ǂݍ��݂ƊǗ��Ɋւ���N���X�ƃC���^�[�t�F�[�X���`
#include "TextureLoader.h"
// �v���C���[�L�����N�^�[�Ƃ��̊֘A�����`�����N���X�ƒ萔/�񋓌^�̒�`
#include "Player.h"
// �Q�[���̔w�i���Ǘ�����N���X�Ɗ֘A�f�[�^���`
#include "BG.h"
// �e�N���X�̐錾
#include "Shot.h"
// �G�L�����N�^�[�Z�b�g�A�b�v�ƊǗ��Ɋւ���N���X�ƃf�[�^�̒�`
#include "EnemySet.h"
// �X���b�V���R���W�����i�v���C���[�̍U���̓����蔻��j�N���X�̐錾
#include "SlashCollision.h"
// �G�̍U���ƃv���C���[�̓����蔻�菈�����s���N���X�Ɗ֘A����萔�E�񋓌^�̒�`
#include "EnemyAttackCollision.h"
// �Q�[�g�L�[�p�[�I�u�W�F�N�g�Ɋւ���w�b�_�t�@�C��
#include "GateKeeper.h"
// �v���C���[��UI�v�f���Ǘ�����
#include "UI.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "Constants.h"


/**
* @brief Shot�̃R���X�g���N�^
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

		// X�����}�b�v�ړ��p�I�u�W�F�N�g
		GateKeeper::Restore(pTarget);
		m_pGateKeeperX = new GateKeeper(this, m_fMapWidth - GATE_KEEPER1_POS_X, GATE_KEEPER1_POS_Y);
		m_pGateKeeperX_prev = NULL;

		// Y�����}�b�v�ړ��p�I�u�W�F�N�g
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


// �f�X�g���N�^
Stage::~Stage()
{
	// �Sshot�̋����폜
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

	// �S�G�l�~�[�̋����폜
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

	// �SSlashCollision�̋����폜
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


	// �SSlashCollision�̋����폜
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

	//	GateKeeper�폜
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
* @brief �A�j���[�V����(1�t���[��)�̎��s
* @note �����̏��Ԃ� �I�u�W�F�N�g�� move ���Ă���e�X collide ������āA�K�v�Ȃ�e���W�𒲐�����
* @return GAMESCENE_DEFAULT: �p�� / GAMESCENE_END_FAILURE: �Q�[���I�[�o�[
*/
GameSceneResultCode Stage::move() {

	switch (m_ePhase) {
	case STAGE_INIT:
		m_ePhase = STAGE_RUN;
		break;

	case STAGE_RUN:
		// if( (m_iMapNum = ��ԃI�u�W�F�N�g->collide()) > 0)	//	��ԃI�u�W�F�N�g��collide()�͓�����Ȃ玟�̃}�b�v�̔ԍ����A�����肶��Ȃ��Ȃ�-1��Ԃ��B
		// �E����1�}�X�̏ꏊ�ɍs������
		if (m_fPlayerX > m_fMapWidth - GATE_KEEPER1_POS_X) {	// X�������}�b�v�J��
			if (m_iMapNum++ > MAP_NUMBER_MAX)	// �ړ��̂��т� ++
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
		// ������1�}�X�̏ꏊ�ɍs������
		else if (m_fPlayerY > m_fMapHight - GATE_KEEPER2_POS_Y) {	// Y�������}�b�v�J��
			if (m_iMapNum++ > MAP_NUMBER_MAX)	// �ړ��̂��т� ++
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

		//	�|�[�Y��ʌĂяo��
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

		// �G�l�~�[�̃��X�|�[��
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
		// �G�l�~�[�̏���
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


		// shot�̏���
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

		// slash�̏���
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

		// EnemyAttack�̏���
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

		// Slash�ƓG�̔���
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

		//	EnemyAttack�ƃv���C���[�̔���
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

	// X�������Ƀ}�b�v�ړ�
	case STAGE_MAPCHANGE_X:
		if (m_iTimer++ < MAP_CHANGE_TIMER) {	// 60�t���[�������ăX�e�[�W�ړ�
			// �v���C���[���W�ړ�
			m_pPlayer->setDrawX(
				((m_fPlayerDrawX * (MAP_CHANGE_TIMER - m_iTimer))
					+ MAP_CHANGE_PLAYER_MOVE * m_iTimer) / PER_FLOAT
			);
			break;
		}
		//	�O�}�b�v�̊e��Q�[���I�u�W�F�N�g�폜
		clearGameObjects();
		SAFE_DELETE(m_pBG);
		m_pBG = m_pBG_tmp;
		m_pBG_tmp = NULL;

		// ��ԃI�u�W�F�N�g�̍폜�ƍX�V
		// ���ꂪ�Ȃ��ƈړ�������������
		SAFE_DELETE(m_pGateKeeperX);
		SAFE_DELETE(m_pGateKeeperY);
		SAFE_DELETE(m_pGateKeeperX_prev);
		SAFE_DELETE(m_pGateKeeperY_prev);
		GateKeeper::Finalize();

		//	���}�b�v�̃f�[�^�Ŋe�탁���o���X�V
		setGameObjects();

		m_iTimer = 0;
		m_ePhase = STAGE_MAPCHANGE_DONE;
		
		break;
	
	// Y�������Ƀ}�b�v�ړ�
	case STAGE_MAPCHANGE_Y:
		if (m_iTimer++ < MAP_CHANGE_TIMER) {	// 60�t���[�������ăX�e�[�W�ړ�
			// �v���C���[���W�ړ�
			m_pPlayer->setDrawY(
				((m_fPlayerDrawY * (MAP_CHANGE_TIMER - m_iTimer))
					+ MAP_CHANGE_PLAYER_MOVE * m_iTimer) / PER_FLOAT
			);
			break;
		}

		//	�O�}�b�v�̊e��Q�[���I�u�W�F�N�g�폜
		clearGameObjects();
		SAFE_DELETE(m_pBG);
		m_pBG = m_pBG_tmp;
		m_pBG_tmp = NULL;

		// ��ԃI�u�W�F�N�g�̍폜�ƍX�V
		// ���ꂪ�Ȃ��ƈړ�������������
		SAFE_DELETE(m_pGateKeeperX);
		SAFE_DELETE(m_pGateKeeperY);
		SAFE_DELETE(m_pGateKeeperX_prev);
		SAFE_DELETE(m_pGateKeeperY_prev);
		GateKeeper::Finalize();

		//	���}�b�v�̃f�[�^�Ŋe�탁���o���X�V
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
		// MAP_NUMBER_MAX�𒴂����
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
* @brief	�e��I�u�W�F�N�g�̃N���A
* @note		�}�b�v�J�ڎ��Ɏg�p
*/
void Stage::clearGameObjects() {
	// �Sshot�̋����폜
	if (m_pShots) {
		std::list<IGameObject*>::iterator it = m_pShots->begin();
		while (it != m_pShots->end()) {
			SAFE_DELETE(*it);
			it = m_pShots->erase(it);
		}
	}
	// �S�G�l�~�[�̋����폜
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
	// �SSlashCollision�̋����폜
	if (m_pSlashCollisions) {
		std::list<IGameObject*>::iterator it = m_pSlashCollisions->begin();
		while (it != m_pSlashCollisions->end()) {
			SAFE_DELETE(*it);
			it = m_pSlashCollisions->erase(it);
		}
	}
	// �SEnemyAttackCollision�̋����폜
	if (m_pEnemyAttackCollisions) {
		std::list<IGameObject*>::iterator it = m_pEnemyAttackCollisions->begin();
		while (it != m_pEnemyAttackCollisions->end()) {
			SAFE_DELETE(*it);
			it = m_pEnemyAttackCollisions->erase(it);
		}
	}
}


/**
* @brief	���}�b�v�Ɏg���f�[�^�E�I�u�W�F�N�g��ݒ�
* @note		m_pBG���X�V���Ă��炱�̊֐����ĂԂ悤��
*/
void Stage::setGameObjects() {
	// enumMapNum�ϐ��Ń}�b�v�ړ���̃v���C���[�`�`����ꍇ����
	switch (m_eMapNum) {
	case RIGHT_STAGE:// �E�ړ�
		m_pPlayer->setPos(MAP_CHANGE_PLAYER_POS, m_pPlayer->getY());	//	�����œ����蔻����C�����Ă����Ȃ���collideWithMap()�Ń}�b�v�E�[��x���W���C������ă}�b�v�J�ڂ��A�����Ă��܂�
		break;
	case UNDER_STAGE:// ���ړ�
		m_pPlayer->setPos(m_pPlayer->getX(), MAP_CHANGE_PLAYER_POS);	//	�����œ����蔻����C�����Ă����Ȃ���collideWithMap()�Ń}�b�v�E�[��x���W���C������ă}�b�v�J�ڂ��A�����Ă��܂�
		break;
	case UPPER_STAGE:
		break;
	default:
		break;
	}
	// setPos�Ń}�b�v�ړ���A�v���C���[�`��ʒu�ύX
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
* @brief �`�揈��
* @note  ���݂̃}�b�v�Ǝ��̃}�b�v�𓮂����Ȃ���`�悷�邱�Ƃňړ����Ă���悤�Ɍ�����
*/
void Stage::draw(ID2D1RenderTarget *pRenderTarget) {

	switch (m_ePhase) {
	case STAGE_MAPCHANGE_X:		// �E�Ɉړ�
		{
			float ratio = m_iTimer / PER_FLOAT;
			//	���̃}�b�v(���Ɉړ�)
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(MAP_CHANGE_X_MOVE_X * (MAP_CHANGE_SPEED - ratio), MAP_CHANGE_X_MOVE_Y));
			m_pBG_tmp->draw(pRenderTarget);
			//	���݂̃}�b�v(���Ɉړ�)
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
			//	�v���C���[��UI
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
			if (m_pPlayer)
				m_pPlayer->draw(pRenderTarget);
			if (m_pUI)
				m_pUI->draw(pRenderTarget);
		}
		break;

	case STAGE_MAPCHANGE_Y:		// ���Ɉړ�
	{
		float ratio = m_iTimer / PER_FLOAT;
		//	���̃}�b�v(��Ɉړ�)
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(MAP_CHANGE_Y_MOVE_X, MAP_CHANGE_Y_MOVE_Y * (MAP_CHANGE_SPEED - ratio)));
		m_pBG_tmp->draw(pRenderTarget);
		//	���݂̃}�b�v(��Ɉړ�)
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
		//	�v���C���[��UI
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

			D2D1_RECT_F		rc;		// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
			rc.left = 0;	// �`��̈�̍�����w��
			rc.top = 0;		//			�V
			rc.right = size.width;		// �`��̈�̉E�����w��
			rc.bottom = size.height;	//			�V
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
* @brief ID2D1RenderTarget ���擾���ĕԂ�
* @note ���̃��\�b�h�Ŏ󂯎���� RenderTarget �͎g�p��K��Release ���邱��
* @return ID2D1RenderTarget �I�u�W�F�N�g
*/
ID2D1RenderTarget *Stage::GetRenderTarget() {
	ID2D1RenderTarget *result = NULL;
	if (m_pSystem) {
		result = m_pSystem->GetRenderTarget();
	}
	return result;
}