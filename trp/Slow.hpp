#pragma once

#include "IPermutation.hpp"

#include <vector>


class Slow : public IPermutation{
	std::vector <int> elements;

public:
	Slow();
	void insert(int value, size_t index);
	void assign(int value, size_t index);
	long long subsegmentSum(size_t left, size_t right);
	bool nextPermutation(size_t left, size_t right);
};
