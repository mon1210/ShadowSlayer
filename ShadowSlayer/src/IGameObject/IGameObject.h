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
	virtual ~IGameObject() = 0;
	virtual bool move() = 0;	// �s��,�A�j���[�V�����Ǘ����\�b�h
	virtual void draw(ID2D1RenderTarget *pRenderTarget) = 0;	// �`�惁�\�b�h
	virtual bool collide(float x, float y, float w, float h) {	// ��`�Ƃ̓����蔻�胁�\�b�h�@true:������ / false:�O��
		return false;
	}
	virtual bool collide(IGameObject *pObj) {	// ���Q�[���I�u�W�F�N�g�Ƃ̓����蔻�胁�\�b�h�@true:������ / false:�O��
		return false;
	}
	virtual bool collideWithMap() = 0;		// �}�b�v�Ƃ̓����蔻�胁�\�b�h
	virtual int damage(float amount) = 0;	// �_���[�W���\�b�h(return �_���[�W)

	STEP_VECTOR step;
	float	m_fDamage;
};


//  �}�N����`
#undef SAFE_RELEASE
#undef SAFE_DELETE
#undef SAFE_DELETE_ARRAY
#define SAFE_RELEASE(o) if (o){ (o)->Release(); o = NULL; }
#define SAFE_DELETE(o)  if (o){ delete (o); o = NULL; }
#define SAFE_DELETE_ARRAY(o) if (o){ delete [] (o); o = NULL; }

#define GRAVITY 2.0f
