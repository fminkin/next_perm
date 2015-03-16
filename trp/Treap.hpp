#pragma once
#include "IPermutation.hpp"

#include <algorithm>
#include <utility>
#include <cstdlib>
#include <climits>


class Treap : public IPermutation{
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
		size_t longestNonDecreasingPrefix;
		size_t longestNonIncreasingSuffix;
		bool reverseNeeded;

		Node *left, *right;

		Node(int value);
	};

	Node *root;

	size_t sizeOf(Node *node);

	long long sumOf(Node *node);

	static Node * leftChildOf(Node *node);

	static Node * rightChildOf(Node *node);

	static int leftestOrMinimal(Node *node);

	static int rightestOrMinimal(Node *node);

	void recalcLongestEnd(Node *node, size_t & (*lengthAccess) (Node *), Node * (*lessGetter) (Node *), Node * (*largerGetter) (Node *),
		int(*leastGetter) (Node *), int(*largestGetter) (Node *));

	void recalc(Node *node);

	void changeReverseNeededStatus(Node *node);

	void push(Node *node);

	//i values goes to left
	std::pair<Node *, Node *> split(Node *node, size_t i);


	//is called only if node-referenced array is decreasingly sorted 
	//Greater (strictly) elements goes to left
	std::pair<Node *, Node *> splitByElement(Node *node, int element);

	Node * merge(Node *left, Node *right);

	void reverse(size_t i, size_t j);


public:

	Treap();

	long long subsegmentSum(size_t left, size_t right);

	void insert(int value, size_t index);

	void assign(int value, size_t index);

	bool nextPermutation(size_t i, size_t j);


};