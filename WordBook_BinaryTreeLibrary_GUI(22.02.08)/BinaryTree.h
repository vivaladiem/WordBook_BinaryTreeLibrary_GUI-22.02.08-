//BinaryTree.h

#ifndef _BINARYTREE_H
#define _BINARYTREE_H
#include <stddef.h>

typedef signed long int Long;
typedef struct _binaryNode BinaryNode;
typedef struct _binaryNode {
	BinaryNode* left;
	BinaryNode* right;
} BinaryNode;
typedef struct _binaryTree {
	BinaryNode* root;
	Long length;
	Long balance;
} BinaryTree;

void BinaryTree_Create(BinaryTree* binaryTree);
BinaryNode* BinaryTree_Insert(BinaryTree* binaryTree, void* key, size_t size, int (*compare)(void* one, void* other));
void BinaryTree_GetAt(BinaryTree* binaryTree, BinaryNode* node, void* key, size_t size);
BinaryNode* Search(BinaryTree* binaryTree, void* key, int (*compare)(void* one, void* other));
BinaryNode* BinaryTree_Delete(BinaryTree* binaryTree, void* key, size_t size, int (*compare)(void* one, void* other));
void MakeBalance(BinaryTree* binaryTree, size_t size);
void MakeKeys(BinaryTree* binaryTree, void* (*keys), Long* count, size_t size);
void Node_MakeKeys(BinaryNode* node, void(*keys), Long* count, Long* index, size_t size);
void BinaryTree_DeleteAllItems(BinaryTree* binaryTree);
void BinaryTree_Node_DeleteAllItems(BinaryNode *node);
void MakeTree(BinaryTree* binaryTree, void(*keys), Long count, size_t size);
BinaryNode* Node_MakeTree(void(*keys), Long low, Long high, size_t size);
void BinaryTree_Destroy(BinaryTree* binaryTree);
void BinaryTree_Node_Destroy(BinaryNode* node);

#endif //_BINARYTREE_H