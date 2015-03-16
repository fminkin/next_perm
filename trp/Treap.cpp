#include "Treap.hpp"

#include <algorithm>
#include <utility>
#include <cstdlib>
#include <climits>

size_t Treap::zero = 0U;


Treap::Node::Node(int value) :
value(value),
leftest(value),
rightest(value),
priority(rand()),
size(1U),
sum(value),
longestNonDecreasingPrefix(1U),
longestNonIncreasingSuffix(1U),
reverseNeeded(false),
left(nullptr),
right(nullptr)
{
}

size_t Treap::sizeOf(Node *node){
	return (node ? node->size : 0U);
}

long long Treap::sumOf(Node *node){
	return (node ? node->sum : 0U);
}

int Treap::leftestOrMinimal(Node * node){
	return (node ? node->leftest : INT_MIN);
}

int Treap::rightestOrMinimal(Node * node){
	return (node ? node->rightest : INT_MIN);
}

Treap::Node * Treap::leftChildOf(Node *node){
	return node->left;
}

Treap::Node * Treap::rightChildOf(Node *node){
	return node->right;
}

void Treap::recalcLongestEnd(Node *node, size_t & (*lengthAccess) (Node *), Node * (*lessGetter) (Node *), Node * (*largerGetter) (Node *), int(*leastGetter) (Node *), int(*largestGetter) (Node *)){

	lengthAccess(node) = 0U;
	bool longestEndStoppesInLessChild = false;

	lengthAccess(node) += lengthAccess(lessGetter(node));
	longestEndStoppesInLessChild = sizeOf(lessGetter(node)) != lengthAccess(lessGetter(node)) ||
		(largestGetter(lessGetter(node)) > node->value);
	if (!longestEndStoppesInLessChild)
	{
		lengthAccess(node) += 1U;
		if (node->value <= leastGetter(largerGetter(node)))
		{
			lengthAccess(node) += lengthAccess(largerGetter(node));
		}
	}
}

void Treap::recalc(Node *node){
	if (node){
		node->size = sizeOf(node->left) + sizeOf(node->right) + 1U;

		node->sum = sumOf(node->left) + sumOf(node->right) + node->value;

		node->leftest = (node->left ? node->left->leftest : node->value);
		node->rightest = (node->right ? node->right->rightest : node->value);

		recalcLongestEnd(node, [](Node * node) -> size_t &
		{
			return (node ? node->longestNonDecreasingPrefix : zero);
		},
			Treap::leftChildOf,
			Treap::rightChildOf,
			Treap::rightestOrMinimal,
			Treap::leftestOrMinimal
			);
		recalcLongestEnd(node,
			[](Node * node) -> size_t &
		{
			return (node ? node->longestNonIncreasingSuffix : zero);
		},
			Treap::rightChildOf,
			Treap::leftChildOf,
			Treap::leftestOrMinimal,
			Treap::rightestOrMinimal
			);
	}
}
void Treap::changeReverseNeededStatus(Node *node){
	if (node){
		node->reverseNeeded ^= true;
	}
}

void Treap::push(Node *node){
	if (node && node->reverseNeeded){
		node->reverseNeeded = false;

		std::swap(node->longestNonIncreasingSuffix, node->longestNonDecreasingPrefix);
		std::swap(node->left, node->right);

		changeReverseNeededStatus(node->left);
		changeReverseNeededStatus(node->right);
	}
}

//i -> left
std::pair<Treap::Node *, Treap::Node *> Treap::split(Node *node, size_t i){
	if (!node){
		return std::pair<Node *, Node *>(nullptr, nullptr);
	}

	push(node);

	if (sizeOf(node->left) >= i)
	{
		std::pair<Node *, Node *> temp = split(node->left, i);
		node->left = temp.second;
		recalc(node);
		return std::pair<Node *, Node *>(temp.first, node);
	}
	std::pair<Node *, Node *> temp = split(node->right, i - sizeOf(node->left) - 1U);
	node->right = temp.first;
	recalc(node);
	return std::pair<Node *, Node *>(node, temp.second);
}

std::pair<Treap::Node *, Treap::Node *> Treap::splitByElement(Node *node, int element){
	if (!node){
		return std::pair<Node *, Node *>(nullptr, nullptr);
	}

	push(node);

	if (node->value <= element)
	{
		std::pair<Node *, Node *> temp = splitByElement(node->left, element);
		node->left = temp.second;
		recalc(node);
		return std::pair<Node *, Node *>(temp.first, node);
	}
	std::pair<Node *, Node *> temp = splitByElement(node->right, element);
	node->right = temp.first;
	recalc(node);
	return std::pair<Node *, Node *>(node, temp.second);
}
Treap::Node * Treap::merge(Node *left, Node *right){
	if (!left || !right){
		return (left ? left : right);
	}

	push(left);
	push(right);

	if (left->priority > right->priority){
		left->right = merge(left->right, right);
		recalc(left);
		return left;
	}
	right->left = merge(left, right->left);
	recalc(right);
	return right;
}

void Treap::reverse(size_t i, size_t j){
	auto T1 = split(root, i);
	auto T2 = split(T1.second, j - i + 1);
	changeReverseNeededStatus(T2.first);
	root = merge(T1.first, merge(T2.first, T2.second));
}
//public methods
Treap::Treap() : root(nullptr){
}

long long Treap::subsegmentSum(size_t i, size_t j){
	auto T1 = split(root, i);
	auto T2 = split(T1.second, j - i);
	long long returnValue = T2.first->sum;
	root = merge(T1.first, merge(T2.first, T2.second));
	return returnValue;
}

void Treap::insert(int value, size_t i){
	auto T = split(root, i);
	root = merge(T.first, merge(new Node(value), T.second));
}

void Treap::assign(int newValue, size_t i){
	auto T1 = split(root, i);
	auto T2 = split(T1.second, 1U);
	T2.first->value = newValue;
	root = merge(T1.first, merge(T2.first, T2.second));
}

bool Treap::nextPermutation(size_t i, size_t j){
	/*
	|                                             root                                                                   |
	|T1.first|                                   T1.second (T2)                                                          |
	|        |                                  segment (parts)                                                |T2.second|
	|        |                  prefixParts         |             decreasingPart(suffixParts)                  |         |
	|        |                 | elementToSwap      |             middleParts           |                      |         |
	|T1.first|prefixParts.first| elementToSwap      |middleParts.first|elementToSwapWith|  suffixParts.second  |T2.second| - result of split
	*/
	auto T1 = split(root, i);
	auto T2 = split(T1.second, j - i);
	Node *segment = T2.first;
	if (segment->longestNonIncreasingSuffix == segment->size){
		changeReverseNeededStatus(segment);
		root = merge(T1.first, merge(segment, T2.second));
		return false;
	}

	auto parts = split(segment, segment->size - segment->longestNonIncreasingSuffix);
	Node *decreasingPart = parts.second;
	auto prefixParts = split(parts.first, parts.first->size - 1U);
	Node *elementToSwap = prefixParts.second;
	auto suffixParts = splitByElement(decreasingPart, elementToSwap->value);
	auto middleParts = split(suffixParts.first, suffixParts.first->size - 1U);
	Node *elementToSwapWith = middleParts.second;
	Node *newSuffix = merge(middleParts.first, merge(elementToSwap, suffixParts.second));
	changeReverseNeededStatus(newSuffix);

	segment = merge(prefixParts.first, merge(elementToSwapWith, newSuffix));
	root = merge(T1.first, merge(segment, T2.second));
	return true;
}