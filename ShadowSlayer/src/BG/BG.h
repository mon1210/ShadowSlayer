/**
* @file	 BG.h
* @brief �N���X BG �̐錾
* @note	 �w�i�E�}�b�v�N���X
*/
//�Q�[����̃I�u�W�F�N�g��\������N���X��ĉ������C���^�[�t�F�[�X�̐錾
#include "..\IGameObject\IGameObject.h"

struct ID2D1Bitmap;
struct ID2D1RenderTarget;

typedef struct mapdata {
	BYTE	*data;
	size_t	length;
}	MAPDATA;


class BG
{
public:
	/**
	* @brief BG�̃R���X�g���N�^
	*/
	BG(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief BG�̃R���X�g���N�^
	*/
	~BG();

	/**
	* @brief �`�惁�\�b�h
	* @note  ��ʏ�Ɍ����Ă��镔���̂ݕ`��
	*/
	void draw(ID2D1RenderTarget *pRenderTarget);

	/**
	* @brief �v���C���[��x���W���Z�b�g���ĕԂ�
	*/
	int setX(float x);

	/**
	* @brief �v���C���[��x���W���擾���ĕԂ�
	*/
	int getX();

	/**
	* @brief �}�b�v�̉������擾���ĕԂ� 
	*/
	float getMapWidth();

	/**
	* @brief �}�b�v�̏c�����擾���ĕԂ�
	*/
	float getMapHight();

	/**
	* @brief ���͂��ꂽ(x, y)���W�̃u���b�N�̒l��Ԃ�
	* @param[in] x x���W
	* @param[in] y y���W
	* @return int code = 0 : �����蔻��Ȃ� / code != 0 : �����蔻�肠��
	*/
	int tile_code(float x, float y);

	/**
	* @brief mapdata�̔z�񂩂琔�l�𕜌����郁�\�b�h
	* @param[in] index	�z��̓Y����
	*/
	int mapdata_decode(int index);

	/**
	* @brief	���͂��ꂽ(x, y)���W���N�_�ɍ⓹�̃x�N�g�����v�Z���ĕԂ�
	* @note		�N�_����E�ƍ��֏��ɍ�̏I����{������Bcol���狁�܂�x���W�̓^�C���̍��[�̍��W�Ȃ̂ŁAr_col�͂��̂܂܁Al_col�ɂ�+2���邱�ƁB
	*/
	void getStepVector(float x, float y, STEP_VECTOR *step);

	/**
	* @brief	BG��MAPDATA�\���̂��X�V���܂��B
	* @note		���̊֐��Ń��Z�b�g������Am_pPlayer->setMapWidth(m_pBG->getMapWidth()); ��Y�ꂸ�Ɉ�x���s���邱��
	*/
	void resetMapData(int num);
protected:
	//! map��width��height
	int mMapW, mMapH;
	//! map-chip��width��height
	int mChipW, mChipH;
	//! map-chip �f�[�^�̍s�s�b�`
	int mPitch;
	//! ���݃v���C���[������x���W
	int m_iX;
	ID2D1Bitmap *m_pMapChip;
	ID2D1Bitmap *m_pBGImage;
	MAPDATA	map;
};

