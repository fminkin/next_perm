#pragma once
#include "IPermutation.hpp"
#include <algorithm>
#include <utility>
#include <cstdlib>
#include <climits>

class Treap : public IPermutation
{
private:
	static size_t zero;
	struct Node
	{
		int value;
		int leftest;
		int rightest;
		size_t priority;
		size_t size;
		long long sum;
		size_t NonDecreasingPrefix;
		size_t NonIncreasingSuffix;
		bool ReverseNeed;
		Node *left, *right;
		Node(int value);
	};
	Node *root;
	size_t sizeOf(Node *node);
	long long sumOf(Node *node);
	static Node * GetLeftChild(Node *node);
	static Node * GetRightChild(Node *node);
	static int GetLeftest(Node *node);
	static int GetRightest(Node *node);
	void recalcLongest(Node *node, size_t & (*LengthOfFix) (Node *), Node * (*lessGetter) (Node *), Node * (*largerGetter) (Node *), int(*leastGetter) (Node *), int(*largestGetter) (Node *));
	//вершина, функция(чтобы избежать копипасты от поиска префикса\суффикса), далее функции геттеры
	void recalc(Node *node);
	void changeReverseNeededStatus(Node *node);
	void push(Node *node);

	//i слева
	std::pair<Node *, Node *> split(Node *node, size_t i);

	std::pair<Node *, Node *> splitByValue(Node *node, int element);
	Node * merge(Node *left, Node *right);
	void reverse(size_t i, size_t j);
public:
	Treap();
	long long subsegmentSum(size_t left, size_t right);
	void insert(int value, size_t index);
	void assign(int value, size_t index);
	bool nextPermutation(size_t i, size_t j);
};
