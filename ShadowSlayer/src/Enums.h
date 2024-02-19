/**
* @file	 Enums.h
* @brief eunm�ϐ��錾�p�w�b�_�t�@�C��
* @note	 ���Texture�֌W
*/
#pragma once

enum EnemyTexture
{
	AllMoveTexture = 16,		// �s���Ɋւ���S�Ẵe�N�X�`��
	// X
	Default_X = 0,				// Default�e�N�X�`����X��Index
	AttackStance_X = 1,			// �U���̍\���e�N�X�`����X��Index
	Idle_X = 4,					// Idle�e�N�X�`����X��Index
	// Y
	IdleAndMove_Y = 8,			// Idle�ƈړ��e�N�X�`����Y��Index
	Attack_Y = 9,				// �U������e�N�X�`����Y��Index
	Death_Y = 10,				// ���S�e�N�X�`����Y��Index


};

enum PlayerTexture
{
	AllTexture = 16,				// �S�Ẵe�N�X�`��
	WalkTextureTimesFour = 24,		// 6(�ړ��e�N�X�`��) * 4
	RowPerElement = 7,				// 1�s���Ƃ̗v�f��
	// X
	UnusedTexture = 1,				// �g�p���Ȃ��e�N�X�`����X��Index
	FirstAttackCollisionStart = 2,	// ��i�ڂ̍U���̓����蔻�萶���J�n�e�N�X�`����X��Index
	FirstAttackCollisionFinish = 4,	// ��i�ڂ̍U���̓����蔻��I���e�N�X�`����X��Index
	Slide_X = 3,					// �X���C�f�B���O�e�N�X�`����X��Index
	Damaged_X = 3,					// �_���[�W�e�N�X�`����X��Index
	GetUp_X = 4,					// �N���オ��e�N�X�`����X��Index
	SlideAttackCollisionBefore = 5, // �X���C�f�B���O�U���̓����蔻�萶���O�̃e�N�X�`����X��Index
	SlideAttackCollisionStart = 6,	// �X���C�f�B���O�U���̓����蔻�萶���J�n�̃e�N�X�`����X��Index
	// Y
	Idle_Y = 7,						// Idle�e�N�X�`����Y��Index
	Walk_Y = 8,						// �����e�N�X�`����Y��Index
	Slide_Y = 9,					// �X���C�f�B���O�e�N�X�`����Y��Index
	GetUp_Y = 9,					// �N���オ��e�N�X�`����Y��Index
	FirstAttack_Y = 11,				// ��i�ڂ̍U���e�N�X�`����Y��Index
	SecondAndThirdAttack_Y = 12,	// ��E�O�i�ڂ̍U���e�N�X�`����Y��Index
	SlideAttack_Y = 14,				// �X���C�f�B���O�U���e�N�X�`����Y��Index
	SlideAttack_Y2 = 15,			// �X���C�f�B���O���e�N�X�`����Y��Index
	Damaged_Y = 16,					// �_���[�W�e�N�X�`����Y��Index

};

enum AttackCount
{	// �ǉ𐫌���̂��ߎg�p���́uAttacCount::�v������
	First = 1,		// ��i�ڂ̍U��
	Second = 2,		// ��i�ڂ̍U��
	Third = 3,		// �O�i�ڂ̍U��
	Slide = 4		// �X���C�f�B���O�U��

};

enum ShotTexture
{
	SRowPerElement = 7,		// 1�s���Ƃ̗v�f��(S => Shot)
	HitStart = 8,			// �Փˎ��̃A�j���[�V�����J�n�ԍ�
	StartDestroy = 5,		// ��������Փˎ��A�j���[�V������
	ProjectileFlying = 4,	// �Փ˂��Ă��Ȃ��Ƃ��̕`��ԍ�

};