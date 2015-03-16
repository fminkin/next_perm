#include "Treap.hpp"

#include <algorithm>
#include <utility>
#include <cstdlib>
#include <climits>



size_t Treap::zero = 0U;


	//private methods    
Treap::Node::Node(int value) :
	value(value),
	leftest(value),
	rightest(value),
	priority(rand()),	
	size(1U),
	sum(value),
	NonDecreasingPrefix(1U),
	NonIncreasingSuffix(1U),
	ReverseNeed(false),
	left(nullptr),
	right(nullptr)
{
}
	

size_t Treap::sizeOf(Node *node){
	return (node ? node->size : 0U); //get size
}

long long Treap::sumOf(Node *node){
	return (node ? node->sum : 0U); // get sum
}
int Treap::GetLeftest(Node * node){
	return (node ? node->leftest : INT_MIN); // get the leftest child of subtree
}
int Treap::GetRightest(Node * node){    // get the rightest child of subtree
	return (node ? node->rightest : INT_MIN);
}

Treap::Node * Treap::GetLeftChild(Node *node){ //get left child of node
	return node->left;
}

Treap::Node * Treap::GetRightChild(Node *node){ //get right child of node
	return node->right;
}

void Treap::recalcLongest(Node *node, size_t & (*LengthOfFix) (Node *), Node * (*lessGetter) (Node *), Node * (*largerGetter) (Node *), int(*leastGetter) (Node *), int(*largestGetter) (Node *)){
	LengthOfFix(node) = 0U; //Length of Fix - длина префикса либо суффикса
	bool StopInRightChild = false; 

	LengthOfFix(node) += LengthOfFix(lessGetter(node));
	StopInRightChild = sizeOf(lessGetter(node)) != LengthOfFix(lessGetter(node)) || (largestGetter(lessGetter(node)) > node->value); //решаем, останавливается ли ---фикс в правом сыне
	if (!StopInRightChild){
		LengthOfFix(node) += 1U; //если нет, то добавляем актуальную вершину
		if (node->value <= leastGetter(largerGetter(node))){ //проверяем нужно ли добавить
			LengthOfFix(node) += LengthOfFix(largerGetter(node));
		}
	}
}

void Treap::recalc(Node *node){
	if (node){
		push(node->left);
		push(node->right);
		node->size = sizeOf(node->left) + sizeOf(node->right) + 1U; //обновление размера
		node->sum = sumOf(node->left) + sumOf(node->right) + node->value; //суммы
		node->leftest = (node->left ? node->left->leftest : node->value); // указателя на самый левый
		node->rightest = (node->right ? node->right->rightest : node->value); //на самый правый
		//пересчёт фиксов 
		recalcLongest(node,[](Node * node) -> size_t & 
		{
			return (node ? node->NonDecreasingPrefix : zero);
		},
			Treap::GetLeftChild,
			Treap::GetRightChild,
			Treap::GetLeftest,
			Treap::GetRightest);

		recalcLongest(node,
			[](Node * node) -> size_t &
		{
			return (node ? node->NonIncreasingSuffix : zero);
		},
			Treap::GetRightChild,
			Treap::GetLeftChild,
			Treap::GetRightest,
			Treap::GetLeftest
			);
	}
}

void Treap::changeReverseNeededStatus(Node *node){
	if (node){
		node->ReverseNeed ^= true; //"ксорим" отметку реверса
	}
}

void Treap::push(Node *node){ 
	if (node && node->ReverseNeed){
		node->ReverseNeed = false;
		std::swap(node->NonIncreasingSuffix, node->NonDecreasingPrefix);//суффикс становится префиксом и наоборот
		std::swap(node->left, node->right);//меняем всё местами
		std::swap(node->leftest, node->rightest);
		changeReverseNeededStatus(node->left); //проталкиваем реверс вниз
		changeReverseNeededStatus(node->right);
	}
}

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

	//вызывается на постфиксе для next_permutation, сплитит по значению, элементы {>value} идут налево
std::pair<Treap::Node *, Treap::Node *> Treap::splitByValue(Node *node, int element){
	if (!node)
	{
		return std::pair<Node *, Node *>(nullptr, nullptr);
	}

	push(node);

	if (node->value <= element)
	{
		std::pair<Node *, Node *> temp = splitByValue(node->left, element);
		node->left = temp.second;
		recalc(node);
		return std::pair<Node *, Node *>(temp.first, node);
	}
	std::pair<Node *, Node *> temp = splitByValue(node->right, element);
	node->right = temp.first;
	recalc(node);
	return std::pair<Node *, Node *>(node, temp.second);
}
Treap::Node * Treap::merge(Node *left, Node *right){
	push(left);
	push(right);

	if (!left || !right){
		return (left ? left : right);
	}

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
	auto T2 = split(T1.second, j - i + 1); //выделяем отрезок
	changeReverseNeededStatus(T2.first);//меняем статус на главной	
	root = merge(T1.first, merge(T2.first, T2.second));
}


Treap::Treap() : root(nullptr){
}

long long Treap::subsegmentSum(size_t i, size_t j){
	auto T1 = split(root, i);
	auto T2 = split(T1.second, j - i);//вырезаем, считаем сумму, зашиваем
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
	auto T1 = split(root, i);
	auto T2 = split(T1.second, j - i);
	Node *segment = T2.first; //вырезаем нужный сегмент.
	if (segment->NonIncreasingSuffix == segment->size) //если уже отсорчен, то просто реверс
	{
		changeReverseNeededStatus(segment);
		root = merge(T1.first, merge(segment, T2.second));
		return false;
	}
	auto parts = split(segment, segment->size - segment->NonIncreasingSuffix); //сплитим по количеству. справа нужный суффикс
	Node *decreasingPart = parts.second; 
	auto prefixParts = split(parts.first, parts.first->size - 1U); //префикс - всё, без вершины перед суффиксом
	Node *elementToSwap = prefixParts.second;
	auto suffixParts = splitByValue(decreasingPart, elementToSwap->value); //сплитим суффикс, ища элемент для свапа
	auto middleParts = split(suffixParts.first, suffixParts.first->size - 1U); 
	Node *elementToSwapWith = middleParts.second;
	Node *newSuffix = merge(middleParts.first, merge(elementToSwap, suffixParts.second)); //делаем новый суффикс
	changeReverseNeededStatus(newSuffix); //разворачиваем

	segment = merge(prefixParts.first, merge(elementToSwapWith, newSuffix)); //восстанавливаем
	root = merge(T1.first, merge(segment, T2.second));
	return true;
}
