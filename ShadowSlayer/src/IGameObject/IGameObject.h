/**
* @file  IGameObject.h
* @brief �Q�[����̃I�u�W�F�N�g��\������N���X��ĉ������C���^�[�t�F�[�X�̐錾
* @note  ���{�̃Q�[���ƊE�ł̓^�X�N�ƌĂԏꍇ������
*/
#pragma once
#define __IGAMEOBJECT_H__

typedef struct {
	float x;	//	�N�_
	float y;	//	�N�_
	float vx;	//	x����
	float vy;	//	y����
}	STEP_VECTOR;

struct ID2D1RenderTarget;

class IGameObject
{
public:
	/**
	* @brief �������z�f�X�g���N�^�̎���
	* @details �f�X�g���N�^�����͗p�ӂ��Ȃ���΁A�T�u�N���X���f�X�g���N�^���ĂׂȂ�
	*/
	virtual ~IGameObject() = 0;

	/**
	* @brief �s��,�A�j���[�V�����Ǘ����\�b�h
	*/
	virtual bool move() = 0;

	/**
	* @brief �`�惁�\�b�h
	*/
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;

	/**
	* @brief  ��`�Ƃ̓����蔻�胁�\�b�h�@
	* @return true:������ / false:�O��
	*/
	virtual bool collide(float x, float y, float w, float h) {
		return false;
	}

	/**
	* @brief  ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h
	* @return true:������ / false:�O��
	*/
	virtual bool collide(IGameObject *pObj) {
		return false;
	}

	/**
	* @brief  �}�b�v�Ƃ̓����蔻�胁�\�b�h
	*/
	virtual bool collideWithMap() = 0;

	/**
	* @brief  �_���[�W���\�b�h
	*/
	virtual int damage(float amount) = 0;

	STEP_VECTOR step;
	float	m_fDamage;
};


//  �}�N����`
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
/**
* @def SAFE_RELEASE
* @brief �|�C���^�̉��
*/
#define SAFE_RELEASE(o) if(o){ (o)->Release(); o = NULL; }
/**
* @def SAFE_DELETE
* @brief �|�C���^�̍폜
*/
#define SAFE_DELETE(o)	if(o){ delete (o); o = NULL; }
/**
* @def SAFE_DELETE_ARRAY
* @brief �|�C���^�z��̍폜
*/
#define SAFE_DELETE_ARRAY(o) if(o){ delete [] (o); o = NULL; }

#define GRAVITY 2.0f
