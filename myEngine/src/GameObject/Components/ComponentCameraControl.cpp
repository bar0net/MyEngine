#include "ComponentCameraControl.h"

#include "Application.h"
#include "GameObject/GameObject.h"
#include "Module/ModuleInput.h"
#include "Module/ModuleTime.h"
#include "Module/ModuleEditor.h"

#include "_Vendor/MathGeoLib/Math/float4.h"



void CameraControl::Update()
{
	if (!enabled) return;
	if (App->input->GetKey(KeyCode::LSHIFT)) ratio = 2.0F;
	else ratio = 1.0F;

	// Keyboard Translations
	if (App->input->GetKey(KeyCode::Q)) gameObject->Translate( gameObject->Up() * velocity * App->time->DeltaTime() * ratio);
	if (App->input->GetKey(KeyCode::E)) gameObject->Translate(-gameObject->Up() * velocity * App->time->DeltaTime() * ratio);

	if (App->input->GetKey(KeyCode::W))	gameObject->Translate( gameObject->Front() * velocity * App->time->DeltaTime() * ratio);
	if (App->input->GetKey(KeyCode::S))	gameObject->Translate(-gameObject->Front() * velocity * App->time->DeltaTime() * ratio);

	if (App->input->GetKey(KeyCode::D)) gameObject->Translate( gameObject->Right() * velocity * App->time->DeltaTime() * ratio);
	if (App->input->GetKey(KeyCode::A)) gameObject->Translate(-gameObject->Right() * velocity * App->time->DeltaTime() * ratio);

	// Keyboard Rotations
	if (App->input->GetKey(KeyCode::J)) gameObject->Rotate(0, angularVelocity * App->time->DeltaTime() * ratio, 0);
	if (App->input->GetKey(KeyCode::L)) gameObject->Rotate(0, -angularVelocity * App->time->DeltaTime() * ratio, 0);

	if (App->input->GetKey(KeyCode::I))	gameObject->Rotate( angularVelocity * App->time->DeltaTime() * ratio, 0, 0);
	if (App->input->GetKey(KeyCode::K)) gameObject->Rotate(-angularVelocity * App->time->DeltaTime() * ratio, 0, 0);

	if (App->input->GetKey(KeyCode::U))	gameObject->Rotate(0, 0, angularVelocity * App->time->DeltaTime() * ratio);
	if (App->input->GetKey(KeyCode::O)) gameObject->Rotate(0, 0, -angularVelocity * App->time->DeltaTime() * ratio);

	// Mouse Translations
	if (App->input->GetMouseWheel() != 0) 
		gameObject->Translate(gameObject->Front() * velocity * App->time->DeltaTime() * ratio * App->input->GetMouseWheel());

	if (mouse_enabled && App->input->GetMouseButton(MouseButton::MOUSE_LEFT) && !App->input->GetMouseButton(MouseButton::MOUSE_RIGHT) && !App->input->GetKey(KeyCode::LALT))
	{
		float x = 0;
		float y = 0;
		App->input->GetMouseMovement(&x, &y);
		gameObject->Translate(-x * gameObject->Right() * ratio + y * gameObject->Up() * ratio);
	}

	// Rotate Around Model
	else if (mouse_enabled && App->input->GetMouseButton(MouseButton::MOUSE_LEFT) && !App->input->GetMouseButton(MouseButton::MOUSE_RIGHT) && App->input->GetKey(KeyCode::LALT))
	{
		if (App->editor->GetInspectedObject() == nullptr)
		{
			LOGDEBUG("Please select an object in the inspector to orbit around.");
		}
		else
		{
			float x = 0;
			float y = 0;
			App->input->GetMouseMovement(&x, &y);

			float4x4 model = *(gameObject->ModelMatrix());
			float3 obj_pos = App->editor->GetInspectedObject()->Position();

			model.SetTranslatePart(model.TranslatePart() - obj_pos);
			if (worldYorbit)	model = float4x4::RotateY(-x * ratio) * model;
			else				model = float4x4::RotateAxisAngle(gameObject->Up(), 0.1F) * model;
			model.SetTranslatePart(model.TranslatePart() + obj_pos);

			gameObject->SetModelMatrix( &model );
		}
	}

	// Mouse Rotations
	else if (mouse_enabled && !App->input->GetMouseButton(MouseButton::MOUSE_LEFT) && App->input->GetMouseButton(MouseButton::MOUSE_RIGHT))
	{
		float x = 0;
		float y = 0;
		App->input->GetMouseMovement(&x, &y);
		gameObject->Rotate(-y * ratio, -x * ratio, 0);
	}
}