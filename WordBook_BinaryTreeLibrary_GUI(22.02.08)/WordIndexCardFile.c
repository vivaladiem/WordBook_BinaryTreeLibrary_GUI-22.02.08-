//WordIndexCardFile.c
#include "WordIndexCardFile.h"
#include "WordIndexCard.h"
#include "WordBook.h"
#include "BinaryTree.h"
#include <string.h>

void WordIndexCardFile_Create(WordIndexCardFile *wordIndexCardFile) {
	BinaryTree_Create(&wordIndexCardFile->wordIndexCards);
	wordIndexCardFile->length = 0;
}

WordIndexCard* WordIndexCardFile_Put(WordIndexCardFile *wordIndexCardFile, Word *wordLink) {
	WordIndexCard wordIndexCard;
	WordIndexCard *wordIndexCardLink;
	char alphabet;
	BinaryNode *node = NULL;

	alphabet = wordLink->spelling[0];
	if (alphabet >= 'a' && alphabet <= 'z') {
		alphabet -= ('a'-'A');
	}

	wordIndexCard.alphabet = alphabet;
	node = Search(&wordIndexCardFile->wordIndexCards, &wordIndexCard, CompareWordIndexCardsByAlphabet);

	if (node == NULL) {
		WordIndexCard_Create(&wordIndexCard, 64, alphabet);
		node = BinaryTree_Insert(&wordIndexCardFile->wordIndexCards, &wordIndexCard, sizeof(WordIndexCard), CompareWordIndexCardsByAlphabet);
		wordIndexCardFile->length++;
	}

	wordIndexCardLink = (WordIndexCard*)(node + 1);
	WordIndexCard_Put(wordIndexCardLink, wordLink);
	
	return wordIndexCardLink;
}

WordIndexCard* WordIndexCardFile_Find(WordIndexCardFile *wordIndexCardFile, char alphabet) {
	WordIndexCard *wordIndexCardLink = NULL;
	WordIndexCard wordIndexCard;
	BinaryNode *node = NULL;

	if (alphabet >= 'a' && alphabet <= 'z') {
		alphabet -= 'a' - 'A';
	}

	wordIndexCard.alphabet = alphabet;

	node = Search(&wordIndexCardFile->wordIndexCards, &wordIndexCard, CompareWordIndexCardsByAlphabet);

	if (node != NULL) {
		wordIndexCardLink = (WordIndexCard*)(node + 1);
	}

	return wordIndexCardLink;
}

WordIndexCard* WordIndexCardFile_Draw(WordIndexCardFile *wordIndexCardFile, Word *wordLink, char alphabet) {
	BinaryNode *node;
	WordIndexCard wordIndexCard;
	WordIndexCard *wordIndexCardLink;
	Long index;

	if (alphabet >= 'a' && alphabet <= 'z') {
		alphabet -= 'a' - 'A';
	}

	wordIndexCard.alphabet = alphabet;

	node = Search(&wordIndexCardFile->wordIndexCards, &wordIndexCard, CompareWordIndexCardsByAlphabet);

	wordIndexCardLink = (WordIndexCard*)(node + 1);

	index = WordIndexCard_Find(wordIndexCardLink, wordLink);

	WordIndexCard_Draw(wordIndexCardLink, index);

	if (wordIndexCardLink->length < 1) {
		WordIndexCard_Destroy(wordIndexCardLink);
		BinaryTree_Delete(&wordIndexCardFile->wordIndexCards, wordIndexCardLink, sizeof(WordIndexCard), CompareWordIndexCardsByAlphabet);
		wordIndexCardFile->length--;
		wordIndexCardLink = NULL;
	}

	return wordIndexCardLink;
}

void WordIndexCardFile_Arrange(WordIndexCardFile *wordIndexCardFile) {
	MakeBalance(&wordIndexCardFile->wordIndexCards, sizeof(WordIndexCard));
	WordIndexCardFile_Node_Arrange(wordIndexCardFile->wordIndexCards.root);
}

void WordIndexCardFile_Node_Arrange(BinaryNode *node) {
	if (node != NULL) {
		WordIndexCardFile_Node_Arrange(node->left);
		WordIndexCardFile_Node_Arrange(node->right);
		WordIndexCard_Arrange((WordIndexCard*)(node + 1));
	}
}

void MakeList(WordIndexCardFile *wordIndexCardFile, WordIndexCard *(*wordIndexCards), Long *count) {
	MakeKeys(&wordIndexCardFile->wordIndexCards, wordIndexCards, count, sizeof(WordIndexCard));
}

void WordIndexCardFile_Destroy(WordIndexCardFile *wordIndexCardFile) {
	WordIndexCardFile_Node_Destroy(wordIndexCardFile->wordIndexCards.root);
	BinaryTree_Destroy(&wordIndexCardFile->wordIndexCards);
}

void WordIndexCardFile_Node_Destroy(BinaryNode *node) {
	if (node != NULL) {
		WordIndexCardFile_Node_Destroy(node->left);
		WordIndexCardFile_Node_Destroy(node->right);
		WordIndexCard_Destroy((WordIndexCard*)(node + 1));
	}
}

int CompareWordIndexCardsByAlphabet(void *one, void *other) {
	WordIndexCard* one_ = (WordIndexCard*)one;
	WordIndexCard* other_ = (WordIndexCard*)other;
	int result;

	if (one_->alphabet > other_->alphabet) {
		result = 1;
	}
	else if (one_->alphabet == other_->alphabet) {
		result = 0;
	}
	else {
		result = -1;
	}

	return result;
}