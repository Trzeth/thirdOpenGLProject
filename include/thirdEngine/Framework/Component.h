#pragma once
class Component
{
public:
	// ref: https://stackoverflow.com/questions/1504251/heap-corruption-what-could-the-cause-be
	// Polymorphic baseclasses without virtual destructors
	virtual ~Component() { };
};
