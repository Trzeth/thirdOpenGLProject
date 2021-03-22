#pragma once
#include <vector>

typedef uint32_t msize_t;

class ComponentBitmask
{
public:
	ComponentBitmask();

	void SetBit(msize_t bit, bool set);

	bool IsBitSet(msize_t bit);

	bool HasComponents(const ComponentBitmask& other);
private:
	typedef uint32_t munit_t;
	std::vector<munit_t> mask;
};
