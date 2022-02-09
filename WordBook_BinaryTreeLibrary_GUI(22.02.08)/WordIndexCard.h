//WordIndexCard.h
// 처음엔 AzIndexCard였는데, 이렇게 하면 의미전달 부족. 추상화가 제대로 안된 것. 의미가 딱 봐도 드러나게 해야함!
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