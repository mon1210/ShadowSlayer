/**
* @file IGameScene.h
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
	virtual ~IGameScene() = 0;
	virtual GameSceneResultCode move() = 0;		// �A�j���[�V�������\�b�h�@Title, Stage, GameOver�Œ�`
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;	// �`�惁�\�b�h
};

#endif __IGAMESCENE_H__

