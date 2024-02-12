// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "stdafx.h"
// �G�L�����N�^�[�Z�b�g�A�b�v�ƊǗ��Ɋւ���N���X�ƃf�[�^�̒�`
#include "EnemySet.h"
// �Q�[�����̓G�L�����N�^�[�N���X�Ɗ֘A����萔�E�񋓌^�̒�`
#include "Enemy01.h"


/**
* @brief EnemySet�̃R���X�g���N�^
*/
EnemySet::EnemySet(Stage *pParent)
{
	m_iIndex = 0;
	m_pParent = pParent;
}


// �f�X�g���N�^
EnemySet::~EnemySet()
{
}


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̓ǂݍ���
* @note		�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
*/
void EnemySet::Restore(ID2D1RenderTarget *pRT, BG *pBG) {
	Enemy01::Restore(pRT, pBG);
}


/**
* @fn
* @brief	���L���f�B�A�t�@�C���̏���
* @note		�V�[���폜���ȂǂɌĂяo���悤�ɂ���
*/
void EnemySet::Finalize() {
	Enemy01::Finalize();
}


/*
* @brief �G�l�~�[�z�u���\�b�h
* @note  �����̐��l�ȉ��ŃZ�b�g�f�[�^���Ăяo���ăG�l�~�[��z�u
*/ 
IGameObject* EnemySet::GetEnemyToSet(int x) {
	IGameObject *pObj = NULL;
	int sx, sy;
	if (m_iIndex < m_szSetDaraSize) {
		if (m_pSetData[m_iIndex] <= x) {
			sx = m_pSetData[m_iIndex];		// ��ڂ̃f�[�^(X���W)
			sy = m_pSetData[m_iIndex + 1];		// ��ڂ̃f�[�^(Y���W)
			switch (m_pSetData[m_iIndex + 2]) {		// �O�ڂ̃f�[�^(���)
			case 1:
				pObj = new Enemy01(m_pParent, sx, sy);
				break;
			}
			m_iIndex ++;
			m_iIndex ++;
			m_iIndex ++;	// ���s����(���̃G�l�~�[�̏���)
		}
	}
	return pObj;
}


/*
*	�G�Z�b�g�f�[�^
*/

//	�o�� X���W, Y���W, �G���
SHORT EnemySet::m_pSetData[] = {
	150, 160, 1,
	320, 160, 1,
	610, 160, 1,
	840, 160, 1
};

size_t EnemySet::m_szSetDaraSize = _countof(EnemySet::m_pSetData);