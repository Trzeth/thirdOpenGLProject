#pragma once
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>
#include <thirdEngine/Renderer/Camera.h>

class CameraComponent :public Component
{
public:
	CameraComponent() { }

	typedef Camera Data;
	Data data;
};

class CameraComponentConstructor :public DefaultComponentConstructor<CameraComponent> {
	using DefaultComponentConstructor<CameraComponent>::DefaultComponentConstructor;
};
