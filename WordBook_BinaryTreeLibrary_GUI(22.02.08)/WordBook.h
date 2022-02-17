//WordBook.h

#ifndef _WORDBOOK_H
#define _WORDBOOK_H

#include "LinkedList.h"
typedef signed long int Long;
typedef struct _word {
	char spelling[32];
	char meaning[64];
	char category[16];
	char example[64];
} Word;
typedef struct _wordBook {
	LinkedList words;
	Long length;
	Word* current;
} WordBook;

// 함수 정의
void WordBook_Create(WordBook* wordBook);
Long Load(WordBook* wordBook);
Long LoadMemorizedWords(WordBook *memorizedWordBook);
Word* WordBook_Put(WordBook* wordBook, Word word);
void FindBySpelling(WordBook* wordBook, char(*spelling), Word** (*indexes), Long* count);
void FindByMeaning(WordBook* wordBook, char(*meaning), Word** (*indexes), Long* count);
Word WordBook_Draw(WordBook* wordBook, Word* index);
Word* WordBook_First(WordBook* wordBook);
Word* WordBook_Previous(WordBook* wordBook);
Word* WordBook_Move(WordBook* wordBook, Word *index);
Word* WordBook_Next(WordBook* wordBook);
Word* WordBook_Last(WordBook* wordBook);
Long Save(WordBook* wordBook);
Long SaveMemorizedWords(WordBook *memorizedWordBook);
void WordBook_Destroy(WordBook* wordBook);
int CompareSpellings(void* one, void* other);
int CompareMeanings(void* one, void* other);
int CompareWords(void* one, void* other);
#endif //_WORDBOOK_H