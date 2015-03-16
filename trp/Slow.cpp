#include "Slow.hpp"

#include <vector>
#include <algorithm>
#include<numeric>
Slow::Slow()
{
}

void Slow::insert(int value, size_t index)
{
	elements.insert(elements.begin() + index, value);
}

void Slow::assign(int value, size_t index)
{
	elements[index] = value;
}

long long Slow::subsegmentSum(size_t left, size_t right)
{
	return std::accumulate(elements.begin() + left, elements.begin() + right, 0LL);
}

bool Slow::nextPermutation(size_t left, size_t right)
{
	return std::next_permutation(elements.begin() + left, elements.begin() + right);
}