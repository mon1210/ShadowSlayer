/**
* @file EnemySet.h
* @brief	�N���X CEnemySet �̐錾�t�@�C��
*/
#include "..\IGameObject\IGameObject.h"

class Stage;
class BG;

class EnemySet
{
public:
	EnemySet(Stage *pParent);
	~EnemySet();
	virtual void Restore(ID2D1RenderTarget *pRT, BG *pBG);	// ���L���f�B�A�t�@�C���̓ǂݍ��݁@�V�[���J�n���ȂǂɌĂяo���悤�ɂ���
	virtual void Finalize();	// ���L���f�B�A�t�@�C���̏����@�V�[���폜���ȂǂɌĂяo���悤�ɂ���
	virtual IGameObject *GetEnemyToSet(int x);	// �G�l�~�[�z�u���\�b�h�@�����̐��l�ȉ��ŃZ�b�g�f�[�^���Ăяo���ăG�l�~�[��z�u
protected:
	Stage *m_pParent;
	int	m_iIndex;
	static SHORT m_pSetData[];
	static size_t m_szSetDaraSize;
};

