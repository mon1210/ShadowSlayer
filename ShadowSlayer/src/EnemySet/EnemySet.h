/**
* @file  EnemySet.h
* @brief �N���X EnemySet �̐錾�t�@�C��
*/
#include "..\IGameObject\IGameObject.h"

class Stage;
class BG;

class EnemySet
{
public:
	/**
	* @brief EnemySet�̃R���X�g���N�^
	*/
	EnemySet(Stage *pParent);

	/**
	* @brief EnemySet�̃f�X�g���N�^
	*/
	~EnemySet();

	/**
	* @brief	���L���f�B�A�t�@�C���̓ǂݍ���
	* @note		�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	*/
	void Restore(ID2D1RenderTarget *pRT, BG *pBG);

	/**
	* @brief	���L���f�B�A�t�@�C���̏���
	* @note		�V�[���폜���ȂǂɌĂяo���悤�ɂ���
	*/
	void Finalize();

	/*
	* @brief �G�l�~�[�z�u���\�b�h
	* @note  �����̐��l�ȉ��ŃZ�b�g�f�[�^���Ăяo���ăG�l�~�[��z�u
	*/
	IGameObject *GetEnemyToSet(int x);
protected:
	Stage *m_pParent;
	int	m_iIndex;
	static SHORT m_pSetData[];
	static size_t m_szSetDaraSize;
};

