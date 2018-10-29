#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <unordered_map>

#include "Component.h"
#include "../_Vendor/MathGeoLib/Math/float4x4.h"
#include "../_Vendor/MathGeoLib/Math/float3.h"

#define DEG2RAD 0.0174532925
#define RAD2DEG 57.2957795

class GameObject
{
public:
	GameObject(const char* name) : name(name) {};

	virtual ~GameObject();

	virtual void Init();
	virtual void Start();
	virtual void Update();
	virtual void CleanUp();
	virtual void End();

	virtual math::float3 Up();
	virtual math::float3 Right();
	virtual math::float3 Front();

	virtual void SetPosition(float x, float y, float z);
	virtual void SetRotation(float x, float y, float z);
	virtual void SetScale(float x, float y, float z);

	virtual void Translate(float x, float y, float z);
	virtual void Rotate(float x, float y, float z);
	
	virtual math::float4x4* ModelMatrix() { return &transform; }

	void AddComponent(Component* component);

	const char* GetName() { return name; }

	math::float3 position = math::float3::zero;
	math::float3 rotation = math::float3::zero;
	math::float3 scale = math::float3::one;
	bool transformChanged = false;

	std::unordered_map<const char*, Component*> components;

private:
	math::float4x4 transform = math::float4x4::identity;
	const char* name;
};

#endif // !_GAME_OBJECT_H

