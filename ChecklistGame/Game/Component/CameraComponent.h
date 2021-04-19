#pragma once
#include <thirdEngine/Framework/Component.h>
#include <thirdEngine/Framework/DefaultComponentConstructor.h>
#include <thirdEngine/Renderer/Camera.h>

class CameraComponent :public Component
{
public:
	CameraComponent() :isEnable(false) { }

	typedef Camera Data;
	Data data;

	bool isEnable;
};

class CameraComponentConstructor :public DefaultComponentConstructor<CameraComponent> {
	using DefaultComponentConstructor<CameraComponent>::DefaultComponentConstructor;
};
