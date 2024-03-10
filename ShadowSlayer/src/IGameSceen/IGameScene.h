/**
* @file  IGameScene.h
* @brief �V�[���̊��N���X IGameScene �̐錾
*/
/*
*�����w�b�_�t�@�C����������C���N���[�h����邱�Ƃɂ�����h�����߂̃v���v���Z�b�T�e�N�j�b�N
*/
#ifndef __IGAMESCENE_H__
#define __IGAMESCENE_H__

enum GameSceneResultCode;
struct ID2D1RenderTarget;

class IGameScene
{
public:
	/**
	* @brief �������z�f�X�g���N�^�̎���
	* @details �f�X�g���N�^�����͗p�ӂ��Ȃ���΁A�T�u�N���X���f�X�g���N�^���ĂׂȂ�
	*/
	virtual ~IGameScene() = 0;

	/**
	* @brief �A�j���[�V�������\�b�h
	* @details Title, Stage, GameOver�Œ�`
	*/
	virtual GameSceneResultCode move() = 0;

	/**
	* @brief �`�惁�\�b�h
	*/
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;
};

#endif __IGAMESCENE_H__

