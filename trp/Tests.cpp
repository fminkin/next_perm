#include "Tests.hpp"
#include <cstdlib>

int Test::rand()
{
	return seed = (1723 * seed + 817623);
}

Test::Test(size_t queryNumber, int minimalNumber, int maximalNumber) : queryNumber(queryNumber), minimalNumber(minimalNumber), maximalNumber(maximalNumber)
{
}

void Test::srand(unsigned int seed)
{
	this->seed = seed;
}


std::vector <long long> Test::operator()(IPermutation *testObject)
{
	std::vector<long long> result;
	size_t currentSize = 0;
	for (size_t i = 0; i < queryNumber; ++i)
	{
		int value;
		size_t position;
		size_t left;
		size_t right;

		switch ((currentSize ? rand() % 4 + 1 : 1))
		{
		case 1:
			value = rand() % (maximalNumber - minimalNumber + 1) + minimalNumber;
			position = rand() % (currentSize + 1);
			testObject->insert(value, position);
			++currentSize;
			break;
		case 2:
			value = rand() % (maximalNumber - minimalNumber + 1) + minimalNumber;
			position = rand() % currentSize;
			testObject->assign(value, position);
			break;
		case 3:
			right = rand() % currentSize + 1;
			left = rand() % right;
			result.push_back(testObject->subsegmentSum(left, right));
			break;
		case 4:
			right = rand() % currentSize + 1;
			left = rand() % right;
			result.push_back(testObject->nextPermutation(left, right));
		};
	}
	return result;
}