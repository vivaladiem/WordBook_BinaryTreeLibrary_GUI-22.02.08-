//WordIndexCardFile.h

#ifndef _WORDINDEXCARDFILE_H
#define _WORDINDEXCARDFILE_H
#include "BinaryTree.h"

typedef signed long int Long;
typedef struct _wordIndexCardFile {
	BinaryTree wordIndexCards;
	Long length;
} WordIndexCardFile;

typedef struct _word Word;
typedef struct _wordIndexCard WordIndexCard;

void WordIndexCardFile_Create(WordIndexCardFile *wordIndexCardFile);
WordIndexCard* WordIndexCardFile_Put(WordIndexCardFile *wordIndexCardFile, Word *wordLink);
WordIndexCard* WordIndexCardFile_Find(WordIndexCardFile *wordIndexCardFile, char alphabet);
WordIndexCard* WordIndexCardFile_Draw(WordIndexCardFile *wordIndexCardFile, Word *wordLink, char alphabet);
void WordIndexCardFile_Arrange(WordIndexCardFile *wordIndexCardFile);
void WordIndexCardFile_Node_Arrange(BinaryNode *node);
void MakeList(WordIndexCardFile *wordIndexCardFile, WordIndexCard *(*wordIndexCards), Long *count);
void WordIndexCardFile_Destroy(WordIndexCardFile *wordIndexCardFile);
void WordIndexCardFile_Node_Destroy(BinaryNode *node);
int CompareWordIndexCardsByAlphabet(void *one, void *other);

#endif //_WORDINDEXCARDFILE_H