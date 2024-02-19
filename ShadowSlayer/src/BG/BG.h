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
	BG(ID2D1RenderTarget *pRenderTarget);
	virtual ~BG();
	virtual void draw(ID2D1RenderTarget *pRenderTarget);	// �`�惁�\�b�h	 ��ʏ�Ɍ����Ă��镔���̂ݕ`��
	virtual int setX(float x);		// �v���C���[��x���W���Z�b�g���ĕԂ����\�b�h�@
	virtual int getX();				// �v���C���[��x���W���擾���ĕԂ��@
	virtual float getMapWidth();	// �}�b�v�̉������擾���ĕԂ� 
	virtual float getMapHight();	// �}�b�v�̏c�����擾���ĕԂ� 
	virtual int tile_code(float x, float y);	// ���͂��ꂽ(x, y)���W�̃u���b�N�̒l��Ԃ����\�b�h
	virtual int mapdata_decode(int index);		// mapdata�̔z�񂩂琔�l�𕜌����郁�\�b�h
	virtual void getStepVector(float x, float y, STEP_VECTOR *step);	// ���͂��ꂽ(x, y)���W���N�_�ɍ⓹�̃x�N�g�����v�Z���ĕԂ����\�b�h
	virtual void resetMapData(int num);			// BG��MAPDATA�\���̂��X�V���郁�\�b�h
protected:
	int mMapW, mMapH;	//	map��width��height
	int mChipW, mChipH;	//	map-chip��width��height
	int mPitch;			//	map-chip �f�[�^�̍s�s�b�`
	int m_iX;			//	���݃v���C���[������x���W
	ID2D1Bitmap *m_pMapChip;
	ID2D1Bitmap *m_pBGImage;
	MAPDATA	map;
};

