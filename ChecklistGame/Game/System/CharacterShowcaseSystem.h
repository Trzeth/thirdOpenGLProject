#pragma once
#include <thirdEngine/Framework/System.h>

class Input;
class Renderer;

class CharacterShowcaseSystem :public System
{
public:
	CharacterShowcaseSystem(World& world, Input& input, Renderer& renderer);

protected:
	void updateEntity(float dt, eid_t entity);

private:
	Input& input;
	Renderer& renderer;

	const float PI = 3.14159265358979323846264338327950288;
	float horizontalRad = PI / 2;
};
