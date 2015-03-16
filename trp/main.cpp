#include "tests.hpp"
#include "treap.hpp"
#include "slow.hpp"

#include <vector>
#include <cstdio>
#include<iostream>
int main()
{
	int n, min, max;
	std::cin >> n >> min >> max;
	Test _Test(n, min, max);
	_Test.srand(5);
	std::vector <long long> treapResult = _Test(new Treap());
	_Test.srand(5);
	std::vector <long long> slowResult = _Test(new Slow());
	for (size_t i = 0; i < treapResult.size(); ++i){
		if (treapResult[i] != slowResult[i]){
			std::cout << "FAILED\n";
		}
	}
	std::cout << "Done\n";
	return 0;
}