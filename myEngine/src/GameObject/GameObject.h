#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <unordered_map>
#include <vector>

#include "_Vendor/MathGeoLib/Math/float4x4.h"
#include "_Vendor/MathGeoLib/Math/float3.h"
#include "_Vendor/MathGeoLib/Math/Quat.h"

#include "Components/Component.h"

#define DEG2RAD 0.0174532925F
#define RAD2DEG 57.2957795F

class GameObject
{
public:
	GameObject(const char* name) { this->name = std::string(name); }

	virtual ~GameObject();

	virtual void Init();
	virtual void Start();
	virtual void Update();
	virtual void CleanUp();
	virtual void End();

	virtual math::float3 Up() const;
	virtual math::float3 Right() const;
	virtual math::float3 Front() const;

	virtual float3 Position() const { return position; }
	virtual float3 Rotation() const { return rotation.ToEulerZYX().zyx() * RAD2DEG; };
	virtual float3 Scale() const { return scale; }

	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(float3 position);
	virtual void SetRotation(float x, float y, float z);
	virtual void SetScale(float x, float y, float z);

	virtual void Translate(math::float3 direction);
	virtual void Translate(float x, float y, float z);
	virtual void Rotate(float x, float y, float z);
	
	virtual math::float4x4* ModelMatrix() { return &transform; }
	virtual void SetModelMatrix(math::float4x4* transform);

	virtual void AddComponent(Component* component);
	bool HasComponent(ComponentType type) const { return components.find(type) != components.end(); }
	virtual void RemoveComponent(Component* component) {}; // TODO: Implement remove component

	const char* GetName() const { return name.c_str(); }

	bool transformChanged = false;

	std::unordered_map<ComponentType, std::vector<Component*>> components;

private:
	std::string name;

	math::float3 position = math::float3::zero;
	math::Quat rotation = math::Quat::identity;
	math::float3 scale = math::float3::one;

	math::float4x4 transform = math::float4x4::identity;
};

#endif // !_GAME_OBJECT_H

