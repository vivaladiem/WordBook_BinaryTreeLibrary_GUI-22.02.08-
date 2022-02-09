//WordIndexCard.c
#include "WordIndexCard.h"
#include "WordBook.h"
#include "Array.h"
#include <string.h>

void WordIndexCard_Create(WordIndexCard *wordIndexCard, Long capacity, char alphabet) {
	if (alphabet >= 'a' && alphabet <= 'z') {
		alphabet -= 'a'-'A';
	}

	wordIndexCard->alphabet = alphabet;
	Array_Create(&wordIndexCard->words, capacity, sizeof(Word*));
	wordIndexCard->capacity = capacity;
	wordIndexCard->length = 0;
}

Long WordIndexCard_Put(WordIndexCard *wordIndexCard, Word *wordLink) {
	Long index;

	if (wordIndexCard->length < wordIndexCard->capacity) {
		index = Store(&wordIndexCard->words, wordIndexCard->length, &wordLink, sizeof(Word*));
	}
	else {
		index = AppendFromRear(&wordIndexCard->words, &wordLink, sizeof(Word*));
		wordIndexCard->capacity++;
	}

	wordIndexCard->length++;

	return index;
}

Long WordIndexCard_Find(WordIndexCard *wordIndexCard, Word *wordLink) {
	Long index;

	index = Array_LinearSearchUnique(&wordIndexCard->words, wordLink, sizeof(Word*), CompareWordLinks);

	return index;
}

Word* WordIndexCard_GetAt(WordIndexCard *wordIndexCard, Long index) {
	Word *wordLink;

	Array_GetAt(&wordIndexCard->words, index, &wordLink, sizeof(Word*));

	return wordLink;
}

Long WordIndexCard_Draw(WordIndexCard *wordIndexCard, Long index) {
	index = Array_Delete(&wordIndexCard->words, index, sizeof(Word*));
	
	wordIndexCard->capacity--;
	wordIndexCard->length--;

	return index;
}

void WordIndexCard_Arrange(WordIndexCard *wordIndexCard) {
	InsertionSort(&wordIndexCard->words, sizeof(Word*), CompareWordLinksBySpelling);
}

void WordIndexCard_Destroy(WordIndexCard *wordIndexCard) {
	Array_Destroy(&wordIndexCard->words);
}

int CompareWordLinks(void *one, void *other) {
	Word* *one_ = (Word**)one;
	int result = -1;

	if (*one_ == other) {
		result = 0;
	}

	return result;
}

int CompareWordLinksBySpelling(void *one, void *other) {
	int result;
	Word* *one_ = (Word**)one;
	Word* *other_ = (Word**)other;

	return strcmp((*one_)->spelling, (*other_)->spelling);
}