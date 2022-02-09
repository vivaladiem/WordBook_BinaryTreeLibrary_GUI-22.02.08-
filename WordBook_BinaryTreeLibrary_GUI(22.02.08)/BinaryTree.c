//BinaryTree.c

#include "BinaryTree.h"
#include <stdlib.h>
#include <memory.h>


void BinaryTree_Create(BinaryTree* binaryTree) {
	binaryTree->root = NULL;
	binaryTree->length = 0;
	binaryTree->balance = 0;
}

BinaryNode* BinaryTree_Insert(BinaryTree* binaryTree, void* key, size_t size, int (*compare)(void* one, void* other)) {
	BinaryNode* node;
	BinaryNode* parent = NULL;

	// ��尡 ���Ե� ��ġ�� ã�´�.
	node = binaryTree->root;
	while (node != NULL) {
		parent = node;
		if (compare(node + 1, key) > 0) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}

	// ��带 �����Ѵ�.
	node = (BinaryNode*)malloc(sizeof(BinaryNode) + size);
	node->left = NULL;
	memcpy(node + 1, key, size);
	node->right = NULL;

	// ���踦 �δ´�.
	if (parent != NULL) {
		if (compare(parent + 1, key) > 0) {
			parent->left = node;
		}
		else {
			parent->right = node;
		}
	}
	else {
		binaryTree->root = node;
	}

	// �����˻�Ʈ���� ��뷮�� �ø���.
	binaryTree->length++;

	// �����˻�Ʈ���� �������� ���Ѵ�.
	if (compare(binaryTree->root + 1, key) > 0) {
		binaryTree->balance--;
	}
	else if (compare(binaryTree->root + 1, key) < 0) {
		binaryTree->balance++;
	}

	return node;
}

void BinaryTree_GetAt(BinaryTree* binaryTree, BinaryNode* node, void* key, size_t size) {
	memcpy(key, node + 1, size);
}

BinaryNode* Search(BinaryTree* binaryTree, void* key, int (*compare)(void* one, void* other)) {
	BinaryNode* node;

	// Ű�� ��ġ�ϴ� ��带 ã�´�.
	node = binaryTree->root;
	while (node != NULL && compare(node + 1, key) != 0) {
		if (compare(node + 1, key) > 0) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}

	// ��带 ����Ѵ�.
	return node;
}

BinaryNode* BinaryTree_Delete(BinaryTree* binaryTree, void* key, size_t size, int (*compare)(void* one, void* other)) {
	BinaryNode* node;
	BinaryNode* parent = NULL;
	BinaryNode* alternativeNode = NULL;
	BinaryNode* alternativeParent = NULL;
	BinaryNode* it;
	void* rootKey = NULL;
	BinaryNode* nodeRightChild;

	rootKey = malloc(size);
	if (binaryTree->root != NULL) {
		memcpy(rootKey, binaryTree->root + 1, size);
	}

	// ���� ��带 ã�´�.
	node = binaryTree->root;
	while (node != NULL && compare(node + 1, key) != 0) {
		parent = node;
		if (compare(node + 1, key) < 0) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}

	// ��ü ���� ��ü����� �θ� ã�´�.
	it = node->right;
	while (it != NULL) {
		alternativeParent = alternativeNode;
		alternativeNode = it;
		it = it->left;
	}

	if (alternativeNode == NULL) {
		it = node->left;
		while (it != NULL) {
			alternativeParent = alternativeNode;
			alternativeNode = it;
			it = it->right;
		}
	}


	// ��ü��带 ������.
	if (alternativeParent != NULL) {
		if (compare(alternativeParent + 1, alternativeNode + 1) > 0) {
			alternativeParent->left = alternativeNode->right;
		}
		else {
			alternativeParent->right = alternativeNode->left;
		}
	}

	// ���� ����� �θ��尡 ������ ��ü��带 �մ´�.
	if (parent != NULL) {
		if (compare(parent + 1, node + 1) < 0) {
			parent->right = alternativeNode;
		}
		else {
			parent->left = alternativeNode;
		}
	}
	else {
		// �θ� ������ ��ü��带 �����˻�Ʈ���� ��Ʈ�� �ִ´�.
		binaryTree->root = alternativeNode;
	}

	// ��ü��尡 ������ ���� ����� �ڽ��� ��ü��忡 �մ´�.
	if (alternativeNode != NULL) {
		if (node->left != alternativeNode) {
			alternativeNode->left = node->left;
		}
		if (node->right != alternativeNode) {
			alternativeNode->right = node->right;
		}
	}

	// ��带 �����.
	nodeRightChild = node->right;
	if (node != NULL) {
		free(node);
		node = NULL;
	}

	// ��뷮�� ���δ�.
	binaryTree->length--;

	// �������� �����Ѵ�.
	if (compare(rootKey, key) < 0 || nodeRightChild != NULL) {
		binaryTree->balance--;
	}
	else if (compare(rootKey, key) > 0 || nodeRightChild == NULL) {
		binaryTree->balance++;
	}

	return node;
}

void MakeBalance(BinaryTree* binaryTree, size_t size) {
	void(*keys);
	Long count;

	MakeKeys(binaryTree, &keys, &count, size);
	BinaryTree_DeleteAllItems(binaryTree);
	MakeTree(binaryTree, keys, count, size);

	if (keys != NULL) {
		free(keys);
	}
}

void MakeKeys(BinaryTree* binaryTree, void* (*keys), Long* count, size_t size) {
	Long index = 0;
	*count = 0;

	*keys = calloc(binaryTree->length, size);
	Node_MakeKeys(binaryTree->root, *keys, count, &index, size);
}

void Node_MakeKeys(BinaryNode* node, void(*keys), Long* count, Long* index, size_t size) {
	if (node != NULL) {
		Node_MakeKeys(node->left, keys, count, index, size);
		memcpy(((char(*)) keys) + ((*index) * size), node + 1, size);
		(*index)++;
		(*count)++;
		Node_MakeKeys(node->right, keys, count, index, size);
	}
}

void BinaryTree_DeleteAllItems(BinaryTree* binaryTree) {
	BinaryTree_Node_DeleteAllItems(binaryTree->root);
	binaryTree->root = NULL;
	binaryTree->length = 0;
	binaryTree->balance = 0;
}

void BinaryTree_Node_DeleteAllItems(BinaryNode* node) {
	if (node != NULL) {
		BinaryTree_Node_DeleteAllItems(node->left);
		BinaryTree_Node_DeleteAllItems(node->right);
		free(node);
	}
}
void MakeTree(BinaryTree* binaryTree, void (*keys), Long count, size_t size) {
	Long remainder;

	binaryTree->root = Node_MakeTree(keys, 0, count - 1, size);
	binaryTree->length = count;
	remainder = count % 2;
	if (remainder == 1) {
		binaryTree->balance = 0;
	}
	else {
		binaryTree->balance = 1;
	}
}

BinaryNode* Node_MakeTree(void(*keys), Long low, Long high, size_t size) {
	BinaryNode* node = NULL;
	Long middle;

	if (low <= high) {
		middle = (low + high) / 2;
		node = (BinaryNode*)malloc(sizeof(BinaryNode) + size);
		memcpy(node + 1, (char(*)) keys + (middle * size), size);
		node->left = Node_MakeTree(keys, low, middle - 1, size);
		node->right = Node_MakeTree(keys, middle + 1, high, size);
	}

	return node;
}

void BinaryTree_Destroy(BinaryTree* binaryTree) {
	BinaryTree_Node_Destroy(binaryTree->root);
}

void BinaryTree_Node_Destroy(BinaryNode* node) {
	if (node != NULL) {
		BinaryTree_Node_Destroy(node->left);
		BinaryTree_Node_Destroy(node->right);
		free(node);
	}
}