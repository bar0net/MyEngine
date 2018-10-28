#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <list>

#include "Component.h"
#include "../_Vendor/MathGeoLib/Math/float4x4.h"
#include "../_Vendor/MathGeoLib/Math/float3.h"

class GameObject
{
public:
	GameObject() {};

	virtual ~GameObject();

	virtual void Init();
	virtual void Start();
	virtual void Update();
	virtual void CleanUp();
	virtual void End();

	virtual void SetPosition(float x, float y, float z);
	virtual void SetRotation(float x, float y, float z);
	virtual void SetScale(float x, float y, float z);
	virtual math::float4x4* ModelMatrix() { return &transform; }

	void AddComponent(Component* component);

	math::float3 position = math::float3::zero;
	math::float3 rotation = math::float3::zero;
	math::float3 scale = math::float3::one;
	bool transformChanged = false;

	std::list<Component*> components;

private:
	math::float4x4 transform = math::float4x4::identity;
};

#endif // !_GAME_OBJECT_H

