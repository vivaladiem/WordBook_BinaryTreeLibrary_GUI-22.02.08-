//WordIndexCard.h
// ó���� AzIndexCard���µ�, �̷��� �ϸ� �ǹ����� ����. �߻�ȭ�� ����� �ȵ� ��. �ǹ̰� �� ���� �巯���� �ؾ���!
#ifndef _WORDINDEXCARD_H
#define _WORDINDEXCARD_H
#include "Array.h"
typedef signed long int Long;
typedef struct _wordIndexCard {
	char alphabet;
	Array words;
	Long capacity;
	Long length;
} WordIndexCard;
typedef struct _word Word;

void WordIndexCard_Create(WordIndexCard *wordIndexCard, Long capacity, char alphabet);
Long WordIndexCard_Put(WordIndexCard *wordIndexCard, Word *wordLink);
Long WordIndexCard_Find(WordIndexCard *wordIndexCard, Word *wordLink);
Word* WordIndexCard_GetAt(WordIndexCard *wordIndexCard, Long index);
Long WordIndexCard_Draw(WordIndexCard *wordIndexCard, Long index);
void WordIndexCard_Arrange(WordIndexCard *wordIndexCard);
void WordIndexCard_Destroy(WordIndexCard *wordIndexCard);
int CompareWordLinks(void *one, void *other);
int CompareWordLinksBySpelling(void *one, void *other);

#endif //_WordINDEXCARD_H