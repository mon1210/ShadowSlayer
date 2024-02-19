/**
* @file		Constants.h
* @brief	�萔���߂��w�b�_�t�@�C��
* @note		�}�W�b�N�i���o�[�̍폜
*/
#pragma once
#include "Selector\Selector.h"	// FPS �擾�p

// �ȉ�BG�p�萔====================================================================================================== //
const int MASK_BIT_8 = 255;					// ����8�r�b�g�ȊO�̃r�b�g���N���A(256 = 2^8)
const int MAP_BYTES_PER_ROW = 2;			// ����s������̃o�C�g�����w��
const int PLAYER_START_X = 64;				// �v���C���[����X���W
const int MAP_WIDTH_PIXELS = 1280;			// 40�v�f * 32pixel
const int MAP_HEIGHT_PIXELS = 480;			// 15�v�f * 32pixel
const int HALF_MAP_WIDTH_PIXELS = 640;		// 20�v�f * 32pixel
const int QUARTER_MAP_WIDTH_PIXELS = 320;	// 1280 / 2 / 2
const int ROUNDING = 1;						// �؂�グ���߂̕␳�l
const int MAP1_WIDHT_CHIP_COUNT = 40;		// �}�b�v1�̃}�b�v�`�b�v�̑���(����)
const int MAP1_HALF_WIDHT_CHIP_COUNT = 20;	// �}�b�v1�̃}�b�v�`�b�v�̔���(����)
const int SLOPE_TEXTURE_NUMBER = 8;			// ��̉摜�ԍ�
const int SLOPE_START_TEXTURE_NUBER = 6;	// ��̎n�܂�̉摜�ԍ�
const int SLOPE_BETWEEN_TEXTURE_NUMBER = 5;	// ��̒��Ԃ̉摜�ԍ�
const int MAP_CHIP_SIZE_BITS = 5;			// �}�b�v�`�b�v�̕��������̃r�b�g(32 = 2^5)
const int FILE_INFO_BYTES = 6;				// �t�@�C�������X�L�b�v
const int MAP_CHIP_DATA_OFFSET = 7;			// �}�b�v�`�b�v�f�[�^
const int MAP_CHIP_DATA_SIZE = 2;			// �}�b�v�`�b�v�f�[�^�T�C�Y(��̃f�[�^�ň�̃}�b�v�`�b�v)
const int MAP_CHIP_DATA_SIZE_BITS = 1;		// �}�b�v�`�b�v�f�[�^��BIT�T�C�Y
const float MAP_CHIP_SCALE_FACTOR = 1.0f;	// �}�b�v�`�b�v�`��̍ۂ̊g�嗦
const float BG_SCALE_FACTOR = 1.0f;			// �w�i�`��̍ۂ̊g�嗦
// �ȏ�BG�p�萔====================================================================================================== //



// �ȉ�CEnemy01�p�萔================================================================================================ //
const int ENEMY_MOVE_ABLE_TIMER = 59;		// ������悤�ɂȂ�܂ł̃^�C�}�[
const int COLLISION_LIFE_TIME = 4;			// �U���̓����蔻�莞��
const int iMAP_CHIP_WIDTH = 32;				// �}�b�v�`�b�v�T�C�Y(int)
const int DELETE_TEXTURE_COUNT = -1;		// �`��J�E���g�폜����(0������������)
const int AT_COLLISION_GENERATE_COUNT = 8;	// �����蔻�萶���J�n�J�E���g
const int AT_FINISH_COUNT = 15;				// �U���I���J�E���g
const float MOVE_SPEED = 2.0f;				// �ړ��X�s�[�h
const float AT_COLLISION_POS_X = 16.f;		// �U�������蔻��X���W
const float AT_COLLISION_WIDTH = 16.f;		// �U�������蔻�艡��
const float AT_COLLISION_HEIGHT = 25.5f;	// �U�������蔻��c��
const float FALL_SPEED_MAX = 31.f;			// �������x����
const float DRAW_POS_X = 24.f;				// �`��X���W
const float DRAW_POS_Y = 37.5f;				// �`��Y���W
const float DRAW_WIDE = 48.f;				// �`�扡��
const float ENEMY_COLLISION_OFFSET_L = 8.f;	// �����蔻��g����
const float ENEMY_COLLISION_OFFSET_T = 12.f;// �����蔻��g���
const float ENEMY_LA_TEXTURE_COORD = 224.f;	// �e�N�X�`���̍ŏI�摜�R�[�h
const float ENEMY_TEXTURE_WIDTH = 32.f;		// �e�N�X�`�������T�C�Y
const float ENEMY_TEXTURE_W_HALF = 16.f;	// �e�N�X�`�������T�C�Y�̔���	
const float ENEMY_TEXTURE_HEIGHT = 25.f;	// �e�N�X�`���c���T�C�Y
const float ENEMY_TEXTURE_H_HALF = 12.f;	// �e�N�X�`���c���T�C�Y�̔���
const float ENEMY_SCALE_FACTOR = 1.0f;		// �`�掞�̊g�嗦
// �ȏ�CEnemy01�p�萔================================================================================================ //



// �ȉ�GameOver�p�萔================================================================================================ //
const int GAMEOVER_DONE_TIMER = 600;		// �I���܂ł̎���
const int GAMEOVER_FADE_TIMER = 30;			// �t�F�[�h�A�E�g�J�n����I���܂ł̎���
const float GAMEOVER_FADE_OUT_TIMER = 30.f;	// �t�F�[�h�A�E�g�ɂ����鎞��
// �ȏ�GameOver�p�萔================================================================================================ //



// �ȉ�GateKeeper�p�萔============================================================================================== //
const float GATE_KEEPER_WIDTH_SIZE = 64.f;	// ����
const float GATE_KEEPER_HEIGHT_SIZE = 96.f;	// �c��
const float GATE_KEEPER_SCALE_FACTOR = 1.0f;// �`�掞�̊g�嗦
// �ȏ�GateKeeper�p�萔============================================================================================== //



// �ȉ�My2DAction(main)�p�萔======================================================================================== //
const int WINDOW_BG_COLOR_OFFSET = 1;		// �w�i�F���J�X�^�}�C�Y���邽�߂̃I�t�Z�b�g
const int WINDOW_POS_Y = 0;					// ������Y���W
const int WINDOW_WIDTH_SIZE = 640;			// �E�B���h�E�̉���
const int WINDOW_HEIGHT_SIZE = 480;			// �E�B���h�E�̏c��
const double DOUBLE_ONE = 1.0;				// �{���x���������_�����e����
// �ȏ�My2DAction(main)�p�萔======================================================================================== //



// �ȉ�Player�p�萔================================================================================================== //
const int SHADOW_MODE_COUNT = 5;			// ShadowMode�ւ̐؂�ւ����̎���
const int PLAYER_DAMAGE_ANIM_TIME = 15;		// �v���C���[���_���[�W���󂯂����̃A�j���[�V��������
const int PLAYER_ATTTACK_TIME = 24;			// �v���C���[���A�^�b�N������̂ɕK�v�ȃA�j���[�V��������
const int PLAYER_VY_LIMIT = -50;			// �v���C���[��Y���ɑ΂��鑬���̐���(�}�b�v�ړ����Ɏg�p)
const int ATTACK_THRESHOLD = 4;				// �U�����A�\�����[�V�������瓖���蔻�萶���܂ł̋��E
const int ATTACK_AFTER_THRESHOLD = 12;		// �U�����A�����蔻�萶������폜�܂ł̋��E
const int ATTACK_END_THRESHOLD = 16;		// �U�����A�����蔻��폜����U���I���܂ł̋��E
const int SL_ATTACK_THRESHOLD = 2;			// �X���C�f�B���O�U�����A�\�����[�V�������瓖���蔻�萶���܂ł̋��E
const int SL_ATTACK_AFTER_THRESHOLD = 6;	// �X���C�f�B���O�U�����A�����蔻�萶������폜�܂ł̋��E
const int SL_ATTACK_END_THRESHOLD = 10;		// �X���C�f�B���O�U�����A�����蔻��폜����U���I���܂ł̋��E
const int ATTACK_COLLISION_GENERATE = 4;	// �U�����A�����蔻��𐶐�����J�E���g
const int ATTACK_COLLISION_END = 15;		// �U�����A�����蔻����폜����J�E���g
const int ATTACK_END = 23;					// �U�����[�V�����̂��ׂĂ��I��
const int SL_ATTACK_END = 10;				// �X���C�f�B���O�U���̑S�Ă��I��
const int SL_ATTACK_COLLISION_GENERATE = 2;	// �X���C�f�B���O�U�����A�����蔻��𐶐�����J�E���g
const int SL_DURATION_IN_FRAMES = FPS / 15; // �X���C�f�B���O��1�b�Ԃ̃A�j���[�V�����t���[��
const int SL_DUALATION_SECONDS = 11;		// �X���C�f�B���O�ɂ�����b��
const int PLAYER_INVINCIBLE_TIME = 60;		// ��_���[�W���̖��G����
const int SECOND_ATTACK_THRESHOLD = 10;		// ��i�ڍU���̋���
const int THIRD_ATTACK_THRESHOLD = 13;		// �O�i�ڍU���̋���
const int C_OK_SECOND_ATTACK_THRESHOLD = 20;// �U���L�����Z���\���̓�i�ڍU���̋���
const int C_OK_THIRD_ATTACK_THRESHOLD = 18;	// �U���L�����Z���\���̎O�i�ڍU���̋���
const float SLIDE_DECELERATION_SPEED = 1.0f;// �X���C�f�B���O�U�����̈ړ��X�s�[�h������
const float fMAP_CHIP_WIDTH = 32.f;			// �}�b�v�`�b�v�T�C�Y(float)
const float SHADOW_DRAW_OPACITY = 0.1f;		// Shadow�؂�ւ����A�ω������x
const float SHADOW_MANA_DEPLETION = 0.5f;	// ShadowMode��MANA�̌�����
const float SHADOW_POS_X_TO_P = -10.f;		// �v���C���[�ɑ΂���Shadow��X���W
const float SHADOW_POS_Y_TO_P = 3.f;		// �v���C���[�ɑ΂���Shadow��Y���W
const float SHADOW_SKEW_X_TO_P = 5.f;		// �v���C���[�ɑ΂���Shadow�̎ΓxX
const float SHADOW_SKEW_Y_TO_P = 0.f;		// �v���C���[�ɑ΂���Shadow�̎ΓxY
const float SHADOW_SCALE_X_TO_P = 1.25f;	// �v���C���[�ɑ΂���Shadow�̑傫��X(w)
const float SHADOW_SCALE_Y_TO_P = 1.5f;		// �v���C���[�ɑ΂���Shadow�̑傫��Y(h)
const float SHADOW_CHANGE_SPEED = 2.f;		// ShadowMode�؂�ւ����x
const float SHADOW_CHANGE_DIRECTION = 8.f;	// ShadowMode�؂�ւ����ړ�����
const float SHADOW_AVAILABLE_MANA = 50.f;	// ShadowMode�g�p�\MANA
const float SHOT_DIRECTION = 1.f;			// Shot�̕������߂�
const float PLAYER_MOVE_LIMIT = 32.1f;		// Player�̈ړ�����(1�}�b�v�`�b�v + 0.1f)
const float PLAYER_TEXTURE_WIDTH = 50.f;	// �v���C���[�e�N�X�`�������T�C�Y
const float PLAYER_TEXTURE_W_TWICE = 100.f;	// �v���C���[�e�N�X�`�������T�C�Y�̓�{	
const float PLAYER_TEXTURE_HEIGHT = 37.f;	// �v���C���[�e�N�X�`���c���T�C�Y
const float PLAYER_TEXTURE_H_TWICE = 74.f;	// �v���C���[�e�N�X�`���c���T�C�Y�̓�{
const float PLAYER_LASTTEXTURE_POS = 300.f;	// �ł��E�̃e�N�X�`���̍��W(50 * (7 - 1))
const float MANA_LIMIT = 100.f;				// MANA�̍ő�l
const float PLAYER_KNOCK_BACK = 0.5f;		// ��_���[�W���m�b�N�o�b�N����
const float PLAYER_DAMAGE_OPACITY = 0.5f;	// ��_���[�W�������x(������)
const float PLAYER_MAX_OPACITY = 1.0f;		// �ő哧���x(���S����)
// �ȏ�Player�p�萔================================================================================================== //



// �ȉ�Shot�p�萔==================================================================================================== //
const int SHOT_LIFE_TIME = 20;				// �����蔻��Ȃ��̎�������܂ł̎���
const int SHOT_DESTROY_TIME = 15;			// �ڐG�������܂ł̎���
const float SHOT_RECTANGLE_HALF_SIZE = 64.f;// �`��p��`�̕�(����)�̔���
const float SHOT_RECTANGLE_SIZE = 128.f;	// �`��p��`�̕�(����)
const float SHOT_TEXTURE_WIDTH = 32.f;		// �e�N�X�`���̉���
const float SHOT_TEXTURE_HEIGHT = 32.f;		// �e�N�X�`���̏c��
const float SHOT_LASTTEXTURE_POS = 192.f;	// �ł��E�̃e�N�X�`���̍��W(32 * (7 - 1))
const float SHOT_COLLISION_OFFSET_W = 49.f;	// �����蔻��g����
const float SHOT_COLLISION_OFFSET_T = 5.f;	// �����蔻��g����W
const float SHOT_COLLISION_OFFSET_B = 10.f;	// �����蔻��g�����W
const float SHOT_COLLISION_HEIGHT = 15.f;	// 
const float SHOT_DAMAGE = 10.f;				// Shot�̍U����
const float SHOT_SCALE_FACTOR = 1.0f;		// Shot�`��̍ۂ̊g�嗦
// �ȏ�Shot�p�萔==================================================================================================== //



// �ȉ�SlashCollision�p�萔========================================================================================== //
const float SHADOW_START_LENGTH = 1.f;		// shadowlenght�����l
const float SHADOW_ON_LENGTH = 1.25f;		// Shadow���[�h���̍U���͈�
const float SLASH_DAMAGE = 10.f;			// �U���̃_���[�W
const float SHADOW_SLASH_DAMAGE = 15.f;		// Shadow���[�h���̍U���_���[�W
const float R_SLASH_OFFSET_X = 10.f;		// FaceToRight(�E����)���̍U���͈�X���W�I�t�Z�b�g(1,2�i��)
const float R_THIRD_SLASH_OFFSET_X = 10.f;	//			�V		(3�i��)
const float R_DASH_SLASH_OFFSET_X = 10.f;	// 			�V		(�_�b�V��)
const float L_SLASH_OFFSET_X = -50.f;		// FaceToLeft(������)���̍U���͈�X���W�I�t�Z�b�g(1,2�i��)
const float L_THIRD_SLASH_OFFSET_X = -60.f;	// 			�V		(3�i��)
const float L_DASH_SLASH_OFFSET_X = -65.f;	// 			�V		(�_�b�V��)
const float SLASH_OFFSET_Y = -70.f;			// �U���͈͂�Y���W�I�t�Z�b�g(1,2�i��)
const float THIRD_SLASH_OFFSET_Y = -50.f;	// 			�V		(3�i��)
const float DASH_SLASH_OFFSET_Y = -50.f;	// 			�V		(�_�b�V��)
const int SLASH_LIFE_TIME = 5;				// �U���̎�������
const int SLASH_DAMAGE_LIFE_TIME = 8;		// �U���_���[�W�̎�������
const int SLASH_RETURN_DAMAGE = 1;			// damage���\�b�h�̖߂�l
const int INVINCIBLE_TIME = -1;			// �����蔻��폜
// �ȏ�SlashCollision�p�萔========================================================================================== //



// �ȉ�Stage�p�萔=================================================================================================== //
const int MAP_NUMBER_MAX = 0;				// �}�b�v�̍ő�l
const int MAP_CHANGE_TIMER = 60;			// �}�b�v�ړ����̃v���[���[�`��
const int MAP_CHANGE_DONE_TIMER = 6;		// �}�b�v�ړ��I��
const int MAP_CHANGE_PLAYER_MOVE = 64;		// �}�b�v�ړ����̃v���C���[�ړ�����
const int STAGE_FADE_TIMER = 120;			// �}�b�v�t�F�[�h�A�E�g��Phase�ҋ@�p
const float MAP_CHANGE_PLAYER_POS = 64.f;	// �}�b�v�ړ���A�v���C���[�̍��W
const float GATE_KEEPER1_POS_X = 32.f;		// �Q�[�g�L�[�p�[�I�u�W�F�N�g1��X���W�����p
const float GATE_KEEPER1_POS_Y = 224.f;		// �Q�[�g�L�[�p�[�I�u�W�F�N�g1��Y���W
const float GATE_KEEPER2_POS_X = 32.f;		// �Q�[�g�L�[�p�[�I�u�W�F�N�g2��X���W
const float GATE_KEEPER2_POS_Y = 32.f;		// �Q�[�g�L�[�p�[�I�u�W�F�N�g1��Y���W�����p
const float ENEMY_SPOWN_POS_X = 640.f;		// �v���C���[�ƃG�l�~�[�����̋����ȉ��ŃX�|�[��
const float KILL_ENEMY_GET_MANA = 10.f;		// �G�l�~�[���j���l��MANA
const float ENEMY_ATTACK_POWER = 10.f;		// �G�l�~�[�̍U����(�v���C���[�̎󂯂�_���[�W)
const float PLAYER_ATTACK_POWER = 1.f;		// �v���C���[�̍U����(�G�l�~�[�̎󂯂�_���[�W)
const float MAP_CHANGE_X_MOVE_X = 640.f;	// �}�b�v�ړ����A���݂̃}�b�v�Ǝ��̃}�b�v��X���ɑ΂���ړ�����
const float MAP_CHANGE_X_MOVE_Y = 0.f;		// �}�b�v�ړ����A���݂̃}�b�v�Ǝ��̃}�b�v��Y���ɑ΂���ړ�����
const float MAP_CHANGE_Y_MOVE_Y = 480.f;	// �}�b�v�ړ����A���݂̃}�b�v�Ǝ��̃}�b�v��Y���ɑ΂���ړ�����
const float MAP_CHANGE_Y_MOVE_X = 0.f;		// �}�b�v�ړ����A���݂̃}�b�v�Ǝ��̃}�b�v��X���ɑ΂���ړ�����
const float MAP_CHANGE_SPEED = 1.f;			// �}�b�v�ړ������x����
const float STAGE_DONE_TIMER = 120.f;		// �X�e�[�W�ړ���t�F�[�h�A�E�g�p
// �ȏ�Stage�p�萔=================================================================================================== //



// �ȉ�Title�p�萔=================================================================================================== //
const int TITLE_TIMER_INIT = 0;				// �^�C�g���J�n���̃^�C�}�[�����l
const int TITLE_DONE_TIME = 0;				// �^�C�g���t�F�[�h�A�E�g�J�n���̏������l
const int TITLE_FADEOUT_TIME = 30;			// �^�C�g���t�F�[�h�A�E�g�ɗv���鎞��
const int TITLE_LOOP_FRAME = 1516;			// �^�C�g�����[�v����
const int TITLE_LOOP_HALF_FRAME = 758;		// �����̔���(�Ȃ߂炩�ȕ`�惋�[�v)
const int CENTER = 2;						// ��ʂ̃Z���^�[�w��p
const int VERTICAL_SHIFT_AMOUNT = 40;		// PressSpace����ɃV�t�g����
const int TEXT_OPACITY_TIMER = 150;			// PressSpace�̓����x�𑪂鐔�l
const int DARK_THRESHOLD = 60;				// ���̐��l�ȉ��͂��񂾂񓧖�
const int LIGHT_THRESHOLD = 90;				// ���̐��l�ȏ�͂��񂾂�N��
const int DARK_START_OPACITY = 2;			// ���񂾂񓧖��ɂȂ鏉���l
const float MAX_OPACITY = 1.0f;				// �����x�̍ő�l
const float PER_FLOAT = 60.f;				// �t���[���l��
const float DARK_THRESHOLD_FLAME = 30;		// �����ƑN���l�̍�
const float TITLE_MARGIN_ABOVE = 120.f;		// �^�C�g������ɗ]�����J����
const float TITLE_SCALE_FACTOR = 1.0f;		// �^�C�g���`��̊g�嗦
const float TITLE_BLACKOUT_TIME = 30.0f;	// �^�C�g���̃t�F�[�h�A�E�g����ʂ��Â�����
// �ȏ�Title�p�萔=================================================================================================== //



// �ȉ�UI�p�萔====================================================================================================== //
const float INITIAL_HP = 100.f;				// HP�o�[�̏����l
const float INITIAL_MANA = 100.f;			// MANA�o�[�̏����l
const float HP_BAR_RC_LEFT = 26.f;			// HP�o�[�`��̈捶����w����
const float HP_BAR_RC_TOP = 27.f;			// HP�o�[�`��̈捶����w����
const float MANA_BAR_RC_LEFT = 26.f;		// MANA�o�[�`��̈捶����w����
const float MANA_BAR_RC_TOP = 51.f;			// MANA�o�[�`��̈捶����w����
const float BAR_FLAME_RC_LEFT = 10.f;		// �t���[���`��̈捶����w����
const float BAR_FLAME_RC_TOP = 25.f;		// �t���[���`��̈捶����w����
const float HP_TEXTURE_WIDTH = 66.f;		// HP�o�[�̃e�N�X�`������
const float HP_TEXTURE_HEIGHT = 7.f;		// HP�o�[�̃e�N�X�`���c��
const float MANA_TEXTURE_WIDTH = 66.f;		// MANA�o�[�̃e�N�X�`������
const float MANA_TEXTURE_HEIGHT = 7.f;		// MANA�o�[�̃e�N�X�`���c��
const float FLAME_TEXTURE_WIDTH = 80.f;		// �t���[���̃e�N�X�`������
const float FLAME_TEXTURE_HEIGHT = 21.f;	// �t���[���̃e�N�X�`���c��
const float BAR_SCALE_FACTOR = 1.0f;		// �o�[�`��̊g�嗦
const float FLAME_SCALE_FACTOR = 1.0f;		// �t���[���`��g�嗦

const float HP_TEXTURE_WIDTH_TWICE = HP_TEXTURE_WIDTH * 2;
const float HP_TEXTURE_HEIGHT_TWICE = HP_TEXTURE_HEIGHT * 2;
const float MANA_TEXTURE_WIDTH_TWICE = MANA_TEXTURE_WIDTH * 2;
const float MANA_TEXTURE_HEIGHT_TWICE = MANA_TEXTURE_HEIGHT * 2;
const float FLAME_TEXTURE_WIDTH_TWICE = FLAME_TEXTURE_WIDTH * 2;
const float FLAME_TEXTURE_HEIGHT_TWICE = FLAME_TEXTURE_HEIGHT * 2;
// �ȏ�UI�p�萔====================================================================================================== //
