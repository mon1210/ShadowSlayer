// Windows�A�v���P�[�V�����J���p�̋��ʃw�b�_�t�@�C��
#include "..\stdafx\stdafx.h"
// Direct2D�̃w�b�_�t�@�C�����C���N���[�h
#include <d2d1.h>
// �Q�[���̔w�i���Ǘ�����N���X�Ɗ֘A�f�[�^���`
#include "BG.h"
// �e�N�X�`���ƃr�b�g�}�b�v�̓ǂݍ��݂ƊǗ��Ɋւ���N���X�ƃC���^�[�t�F�[�X���`
#include "..\TextureLoader\TextureLoader.h"
// �Q�[�����̃}�b�v�f�[�^���`
#include "..\MapData\MapData.h"
// �萔�l���߂��w�b�_�t�@�C��
#include "..\Constants.h"


//  �}�N����`
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }


// �R���X�g���N�^
BG::BG(ID2D1RenderTarget *pRenderTarget)
{
	
	m_pMapChip = NULL;
	map.data = NULL;
	TextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\test_mapchip2.png"), &m_pMapChip);
	TextureLoader::CreateD2D1BitmapFromFile(pRenderTarget, _T("res\\title_background.png"), &m_pBGImage);
	map.data = MapData::data;
	map.length = MapData::length;

	// Map�f�[�^�w�b�_�̓ǂݎ��ƃf�R�[�h
	// @note: ���̒l�̉���8�r�b�g�ȊO�̃r�b�g���N���A
	mChipW = map.data[4] & MASK_BIT_8;
	mChipH = map.data[5] & MASK_BIT_8;
	mMapW = map.data[2] & MASK_BIT_8;
	mMapH = map.data[3] & MASK_BIT_8;
	mPitch = mMapW * MAP_BYTES_PER_ROW;	// ����s������̃o�C�g��(�z��̗�)
	mMapW *= mChipW;
	mMapH *= mChipH;

	m_iX = PLAYER_START_X;	// �v���C���[x���W
}


// �f�X�g���N�^
BG::~BG()
{
	SAFE_RELEASE(m_pMapChip);
	SAFE_RELEASE(m_pBGImage);
	map.data = NULL;
}


// �v���C���[��x���W���Z�b�g���ĕԂ�
int BG::setX(float x) {
	m_iX = (int)x;	
	return m_iX;
}


// �v���C���[��x���W���擾���ĕԂ�
int BG::getX() {
	return m_iX;
}

// �}�b�v�̉������擾���ĕԂ�
float BG::getMapWidth() {
	return (float)mMapW;
}


// �}�b�v�̏c�����擾���ĕԂ�
float BG::getMapHight() {
	return (float)mMapH;
}


// �`�惁�\�b�h
void BG::draw(ID2D1RenderTarget *pRenderTarget) {
	int texCols;	// �}�b�v�`�b�v�摜�̗�
	D2D1_SIZE_F size;

	if (m_pMapChip == NULL)
		return;
	size = m_pMapChip->GetSize();
	texCols = ((int)size.width + mChipW - ROUNDING) >> MAP_CHIP_SIZE_BITS;	// �}�b�v�`�b�v�e�N�X�`���̗񐔂��v�Z
	
	
	int dx = 0, dy = 0;	// �`��J�n���W
	int ix = 0;			// �`��J�n�̗�ԍ�
	float	bgX;

	// �v���C���[��X���W���}�b�v1��4����1��荶�ɂ���Ƃ�
	if (m_iX < QUARTER_MAP_WIDTH_PIXELS)	// 20�v�f = 20byte = 10chip = 320pixel
	{
		ix = 0;
		bgX = 0.f;
	}
	// �X�e�[�W�̈�ԉE�[����ʂɉf���Ă�����
	else if (m_iX > mMapW - QUARTER_MAP_WIDTH_PIXELS)	
	{
		ix = mPitch - MAP1_WIDHT_CHIP_COUNT;

		bgX = mMapW - HALF_MAP_WIDTH_PIXELS;
	}
	else {
		ix = (m_iX >> MAP_CHIP_SIZE_BITS) * MAP_BYTES_PER_ROW - MAP1_HALF_WIDHT_CHIP_COUNT;

		dx = -(m_iX % mChipW);	//	���ꂪ�Ȃ��Ɗ��炩�ɃX�N���[�����Ȃ�
		bgX = m_iX - QUARTER_MAP_WIDTH_PIXELS;
	}


	// �ȉ��w�i�`�揈�� ----------------------------------------------------------------------------------------------------------------------------------------------------------- //
	D2D1_RECT_F rc,		// �`��̈�(��ʏ�ł̈ʒu��T�C�Y)���w�肷��ϐ�
				src;	// �e�N�X�`���̒��ł̍��W���w�肵�A�e�N�X�`�������肷��ϐ�
	size = pRenderTarget->GetSize();
	// �`��̈�̐ݒ�
	rc.left = 0.f; rc.top = 0.f;	// �`��̈�̍�����w��
	rc.right = size.width; rc.bottom = size.height;		// �`��̈�̉E�����w��

	// �e�N�X�`�����W�̌v�Z
	src.left = bgX; src.top = 0.f;		// �e�N�X�`�����W�̍�����w��
	src.right = src.left + size.width; src.bottom = size.height;	// �e�N�X�`�����W�̉E�����w��

	// �w�i�̕`��
	pRenderTarget->DrawBitmap(m_pBGImage, rc, BG_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, src);
	// �ȏ�w�i�`�揈�� ----------------------------------------------------------------------------------------------------------------------------------------------------------- //


	// �ȉ��X�e�[�W(�}�b�v�`�b�v)�`�� --------------------------------------------------------------------------------------------------------------------------------------------- //
	// *�}�b�v�`�b�v����2���邱�Ƃɒ��ӁI*�@(MapDate.cpp�Q�l)
	int code;		// �}�b�v�`�b�v�̏��
	int tu, tv;
	int count = 0;
	int currentIndex = 0;
	while (dy < MAP_HEIGHT_PIXELS) {	// ��ʉ��܂�
		currentIndex = ix + mPitch * count;
		for (int x = dx; x < HALF_MAP_WIDTH_PIXELS; x += mChipW) {	// ��ʒ[�܂�
			code = map.data[currentIndex + FILE_INFO_BYTES] & MASK_BIT_8;	// �擪6byte�̓t�@�C�����Ȃ̂ŃX�L�b�v & 1�ڂ̃}�b�v�`�b�v���擾
			code += (map.data[currentIndex + MAP_CHIP_DATA_OFFSET] & MASK_BIT_8) & MASK_BIT_8;	// 2�ڂ̃}�b�v�`�b�v���擾
			currentIndex += MAP_CHIP_DATA_SIZE;		// ���̃}�b�v�`�b�v�f�[�^��

			// �`��̈�̐ݒ�	rc.left,re.top => ������w�� rc.right,re.bottom => �E�����w��
			rc.left = x;
			rc.top = dy;
			rc.right = rc.left + mChipW;
			rc.bottom = rc.top + mChipH;

			// �}�b�v�`�b�v�̃e�N�X�`�����W�̌v�Z	src.left,sre.top => ������w�� src.right,sre.bottom => �E�����w��
			tu = code % texCols;
			tv = code / texCols;
			src.left = tu << MAP_CHIP_SIZE_BITS;
			src.top = tv * mChipH;
			src.right = src.left + mChipW;
			src.bottom = src.top + mChipH;

			// �}�b�v�`�b�v�̕`��
			pRenderTarget->DrawBitmap(m_pMapChip, rc, MAP_CHIP_SCALE_FACTOR, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, src);
		}

		dy += mChipH;	// ���݂̍�����ۑ�
		count++;		// ���̗��
	}
	// �ȏ�X�e�[�W(�}�b�v�`�b�v)�`�� --------------------------------------------------------------------------------------------------------------------------------------------- //

}


// ���͂��ꂽ(x, y)���W�̃u���b�N�̒l��Ԃ�
int BG::tile_code(float x, float y) {
	int row, col, index, code;
	row = (int)y >> MAP_CHIP_SIZE_BITS;
	col = ((int)x >> MAP_CHIP_SIZE_BITS) << MAP_CHIP_DATA_SIZE_BITS;
	index = col + row * mPitch;
	code = mapdata_decode(index);
	return code;
}


// mapdata�̔z�񂩂琔�l�𕜌�����
int BG::mapdata_decode(int index) {
	int code;
	code = map.data[index + FILE_INFO_BYTES] & MASK_BIT_8;
	code += (map.data[index + MAP_CHIP_DATA_OFFSET] & MASK_BIT_8) & MASK_BIT_8;
	return code;
}


// ���͂��ꂽ(x, y)���W���N�_�ɍ⓹�̃x�N�g�����v�Z���ĕԂ�
void BG::getStepVector(float x, float y, STEP_VECTOR *step) {
	int start_row, start_col, index;
	int r_row, r_col, l_row, l_col;

	start_row = (int)y >> MAP_CHIP_SIZE_BITS;
	start_col = ((int)x >> MAP_CHIP_SIZE_BITS) << MAP_CHIP_DATA_SIZE_BITS;
	index = start_col + start_row * mPitch;

	if (mapdata_decode(index) > SLOPE_TEXTURE_NUMBER) {	//	�E������̍�---------------------------------------

		// �}�b�v�`�b�v�̔ԍ����
		int mapNum;

		//	�E���֒T�� --------------------------------------
		r_row = start_row, r_col = start_col;
		mapNum = mapdata_decode(r_col + r_row * mPitch);
		do {
			switch (mapNum) {
			// �ȉ�case�ԍ��͑Ή�����Texture�ԍ�
			case 9:
			case 10:
				r_col ++;
				r_col ++;
				break;
			case 11:
				r_row ++;
				break;
			}
			mapNum = mapdata_decode(r_col + r_row * mPitch);
		} while (mapNum > SLOPE_TEXTURE_NUMBER);
		

		//	�����֒T�� --------------------------------------
		l_row = start_row, l_col = start_col;
		mapNum = mapdata_decode(l_col + l_row * mPitch);
		do {
			switch (mapNum) {
			// �ȉ�case�ԍ��͑Ή�����Texture�ԍ�
			case 9:
			case 11:
				l_col --;
				l_col --;
				break;
			case 10:
				l_row --;
				break;
			}
			mapNum = mapdata_decode(l_col + l_row * mPitch);
		} while (mapNum > SLOPE_BETWEEN_TEXTURE_NUMBER);
		l_col ++;
		l_col ++;

		//	�⓹�x�N�g����Ԃ�
		step->x = (float)((l_col >> MAP_CHIP_DATA_SIZE_BITS) << MAP_CHIP_SIZE_BITS);
		step->y = (float)(l_row << MAP_CHIP_SIZE_BITS);
		step->vx = (float)((r_col >> MAP_CHIP_DATA_SIZE_BITS) << MAP_CHIP_SIZE_BITS) - step->x;
		step->vy = (float)(r_row << MAP_CHIP_SIZE_BITS) - step->y;

	}
	else if (mapdata_decode(index) > SLOPE_BETWEEN_TEXTURE_NUMBER) {	//	�E�オ��̍�----------------------------------------

		// �}�b�v�`�b�v�̔ԍ����
		int mapNum;

		//	�E���֒T�� --------------------------------------
		r_row = start_row, r_col = start_col;
		mapNum = mapdata_decode(r_col + r_row * mPitch);
		do {
			switch (mapNum) {
			// �ȉ�case�ԍ��͑Ή�����Texture�ԍ�
			case 6:
			case 8:
				r_col ++;
				r_col ++;
				break;
			case 7:
				r_row --;
				break;
			}
			mapNum = mapdata_decode(r_col + r_row * mPitch);
		} while (mapNum > SLOPE_BETWEEN_TEXTURE_NUMBER);


		//	�����֒T�� --------------------------------------
		l_row = start_row, l_col = start_col;
		mapNum = mapdata_decode(l_col + l_row * mPitch);
		do {
			switch (mapNum) {			
			// �ȉ�case�ԍ��͑Ή�����Texture�ԍ�
			case 6:
			case 7:
				l_col --;
				l_col --;
				break;
			case 8:
				l_row ++;
				break;
			}
			mapNum = mapdata_decode(l_col + l_row * mPitch);
		} while (mapNum > SLOPE_BETWEEN_TEXTURE_NUMBER);
		l_col ++;
		l_col ++;
		

		//	�⓹�x�N�g����Ԃ�
		step->x = (float)((l_col >> MAP_CHIP_DATA_SIZE_BITS) << MAP_CHIP_SIZE_BITS);
		step->y = (float)(l_row << MAP_CHIP_SIZE_BITS);
		step->vx = (float)((r_col >> MAP_CHIP_DATA_SIZE_BITS) << MAP_CHIP_SIZE_BITS) - step->x;
		step->vy = (float)(r_row << MAP_CHIP_SIZE_BITS) - step->y;

	}

}


// BG��MAPDATA�\���̂��X�V
void BG::resetMapData(int num) {

	switch (num) {
	case 1:
		map.data = MapData::data;
		map.length = MapData::length;
		break;

	case 2:
		if (MapData::length2 < 3)		// 3 = �K�v�Œ���̃}�b�v�v�f�� => �Ȃ����return
			break;
		map.data = MapData::data2;
		map.length = MapData::length2;
		break;

	case 3:
		if (MapData::length3 < 3)		// 3 = �K�v�Œ���̃}�b�v�v�f�� => �Ȃ����return
			break;
		map.data = MapData::data3;
		map.length = MapData::length3;
		break;

	}

	//	Map�f�[�^�w�b�_�̓ǂݎ��ƃf�R�[�h
	mChipW = map.data[4] & MASK_BIT_8;
	mChipH = map.data[5] & MASK_BIT_8;
	mMapW = map.data[2] & MASK_BIT_8;
	mMapH = map.data[3] & MASK_BIT_8;
	mPitch = mMapW * MAP_BYTES_PER_ROW; // ����s������̃o�C�g��(�z��̗�)
	mMapW *= mChipW;
	mMapH *= mChipH;

	m_iX = PLAYER_START_X;	// �v���C���[x���W
}