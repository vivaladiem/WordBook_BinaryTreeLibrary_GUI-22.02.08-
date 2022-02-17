//WordBook.c

#include "WordBook.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)

#if 0
int main(int argc, char* argv[]) {
	WordBook wordBook;
	Word word;
	Word* index = NULL;
	Word* (*indexes) = NULL;
	Long count;
	Long i;
	Word* previous = NULL;

	printf("단어장을 생성한다.\n");
	WordBook_Create(&wordBook);

	printf("단어장을 로드한다.\n");
	count = Load(&wordBook);
	printf("\t%d개 로드\n", count);
	if (count > 0) {
		index = WordBook_First(&wordBook);
		while (index != previous) {
			printf("\t%s\t%s\t%s\t%s\n\n", index->spelling, index->meaning, index->category, index->example);
			previous = index;
			index = WordBook_Next(&wordBook);
		}

	}
	
	printf("entry를 끼워넣는다.\n");
	strcpy(word.spelling, "entry");
	strcpy(word.meaning, "진입");
	strcpy(word.category, "명사");
	strcpy(word.example, "she made her entry");
	index = WordBook_Put(&wordBook, word);
	printf("\t%s\t%s\t%s\t%s\n", index->spelling, index->meaning, index->category, index->example);

	printf("apple를 끼워넣는다.\n");
	strcpy(word.spelling, "apple");
	strcpy(word.meaning, "사과");
	strcpy(word.category, "명사");
	strcpy(word.example, "This is sweet");
	index = WordBook_Put(&wordBook, word);
	printf("\t%s\t%s\t%s\t%s\n", index->spelling, index->meaning, index->category, index->example);

	printf("entry를 끼워넣는다.\n");
	strcpy(word.spelling, "entry");
	strcpy(word.meaning, "가입");
	strcpy(word.category, "명사");
	strcpy(word.example, "countries seeking entry into EU");
	index = WordBook_Put(&wordBook, word);
	printf("\t%s\t%s\t%s\t%s\n", index->spelling, index->meaning, index->category, index->example);


	printf("철자 entry를 찾는다.\n");
	FindBySpelling(&wordBook, "entry", &indexes, &count);
	i = 0;
	while (i < count) {
		printf("\t%d\t%s\t%s\t%s\t%s\n\n", i + 1, indexes[i]->spelling, indexes[i]->meaning, indexes[i]->category,
			indexes[i]->example);
		i++;
	}

	if (indexes != NULL) {
		free(indexes);
	}


	printf("뜻 사과를 찾는다.\n");
	FindByMeaning(&wordBook, "사과", &indexes, &count);
	i = 0;
	while (i < count) {
		printf("\t%d\t%s\t%s\t%s\t%s\n\n", i + 1, indexes[i]->spelling, indexes[i]->meaning, indexes[i]->category,
			indexes[i]->example);
		i++;
	}

	printf("위에서 찾은 apple을 빼낸다.\n");
	word = WordBook_Draw(&wordBook, indexes[0]);
	printf("\t%s\t%s\t%s\t%s\n", word.spelling, word.meaning, word.category, word.example);


	if (indexes != NULL) {
		free(indexes);
	}

	
	printf("끝으로 간다.\n");
	index = WordBook_Last(&wordBook);
	printf("\t%s\t%s\t%s\t%s\n", index->spelling, index->meaning, index->category, index->example);

	printf("이전으로 간다.\n");
	index = WordBook_Previous(&wordBook);
	printf("\t%s\t%s\t%s\t%s\n", index->spelling, index->meaning, index->category, index->example);

	printf("다음으로 간다.\n");
	index = WordBook_Next(&wordBook);
	printf("\t%s\t%s\t%s\t%s\n", index->spelling, index->meaning, index->category, index->example);

	printf("처음으로 간다.\n");
	index = WordBook_First(&wordBook);
	printf("\t%s\t%s\t%s\t%s\n", index->spelling, index->meaning, index->category, index->example);

	printf("단어장을 저장한다.\n");
	count = Save(&wordBook);
	printf("\t%d개 저장\n", count);

	printf("단어장을 없앤다.\n");
	WordBook_Destroy(&wordBook);


	return 0;
}

#endif

void WordBook_Create(WordBook* wordBook) {
	Create(&wordBook->words);

	wordBook->length = 0;

	wordBook->current = NULL;
}

Long Load(WordBook* wordBook) {
	Word word;
	Node* node = NULL;
	FILE* file = NULL;
	size_t flag;

	file = fopen("Words.dat", "rb");
	if (file != NULL) {
		flag = fread(&word, sizeof(Word), 1, file);
		while (!feof(file) && flag != 0) {
			node = AppendFromTail(&wordBook->words, &word, sizeof(Word));
			wordBook->length++;
			wordBook->current = (Word*)(node + 1);
			flag = fread(&word, sizeof(Word), 1, file);
		}

		fclose(file);
	}

	return wordBook->length;
}

Long LoadMemorizedWords(WordBook *memorizedWordBook) {
	Word word;
	Node *node;
	FILE *file = NULL;
	size_t flag;

	file = fopen("MemorizedWords.dat", "rb");
	if (file != NULL) {
		flag = fread(&word, sizeof(Word), 1, file);
		while(!feof(file) && flag != 0) {
			node = AppendFromTail(&memorizedWordBook->words, &word, sizeof(Word));
			memorizedWordBook->length++;
			memorizedWordBook->current = (Word*)(node + 1);
			flag = fread(&word, sizeof(Word), 1, file);
		}

		fclose(file);
	}

	return memorizedWordBook->length;
}

Word* WordBook_Put(WordBook* wordBook, Word word) {
	Node* node = NULL;



	node = AppendFromTail(&wordBook->words, &word, sizeof(Word));

	wordBook->length++;

	wordBook->current = (Word*)(node + 1);

	return wordBook->current;
}

void FindBySpelling(WordBook* wordBook, char(*spelling), Word** (*indexes), Long* count) {
	Node* (*nodes) = NULL;
	Long i = 0;



	*indexes = NULL;

	LinearSearchDuplicate(&wordBook->words, spelling, &nodes, count, CompareSpellings);

	if (*count > 0) {
		*indexes = (Word * (*))calloc(*count, sizeof(Word*));
	}

	while (i < *count) {
		(*indexes)[i] = (Word*)(nodes[i] + 1);
		i++;
	}

	if (nodes != NULL) {
		free(nodes);
	}
}

void FindByMeaning(WordBook* wordBook, char(*meaning), Word** (*indexes), Long* count) {
	Node* (*nodes) = NULL;
	Long i = 0;


	*indexes = NULL;

	LinearSearchDuplicate(&wordBook->words, meaning, &nodes, count, CompareMeanings);

	if (*count > 0) {
		*indexes = (Word * (*))calloc(*count, sizeof(Word*));
	}

	while (i < *count) {
		(*indexes)[i] = (Word*)(nodes[i] + 1);
		i++;
	}

	if (nodes != NULL) {
		free(nodes);
	}
}

Word WordBook_Draw(WordBook* wordBook, Word* index) {
	Word word;
	Node* node = NULL;

	node = LinearSearchUnique(&wordBook->words, index, CompareWords);

	GetAt(&wordBook->words, node, &word, sizeof(Word));

	Delete(&wordBook->words, node);

	wordBook->length--;

	node = wordBook->words.current;

	if (node != NULL) {
		wordBook->current = (Word*)(node + 1);
	}
	else {
		wordBook->current = NULL;
	}

	return word;
}

Word* WordBook_First(WordBook* wordBook) {
	Node* node;

	node = First(&wordBook->words);

	wordBook->current = (Word*)(node + 1);

	return wordBook->current;
}

Word* WordBook_Previous(WordBook* wordBook) {
	Node* node;

	node = Previous(&wordBook->words);

	wordBook->current = (Word*)(node + 1);

	return wordBook->current;
}

Word* WordBook_Move(WordBook* wordBook, Word *index) {
	Node* node;

	node = LinearSearchUnique(&wordBook->words, index, CompareWords);

	node = Move(&wordBook->words, node);

	wordBook->current = (Word*)(node + 1);

	return wordBook->current;
}


Word* WordBook_Next(WordBook* wordBook) {
	Node* node;

	node = Next(&wordBook->words);

	wordBook->current = (Word*)(node + 1);

	return wordBook->current;
}


Word* WordBook_Last(WordBook* wordBook) {
	Node* node;

	node = Last(&wordBook->words);

	wordBook->current = (Word*)(node + 1);


	return wordBook->current;
}

Long Save(WordBook* wordBook) {
	Node* node = NULL;
	Node* previous = NULL;
	Word word;
	FILE* file = NULL;

	file = fopen("Words.dat", "wb");

	if (file != NULL) {

		node = First(&wordBook->words);

		while (node != previous) {

			GetAt(&wordBook->words, node, &word, sizeof(Word));

			fwrite(&word, sizeof(Word), 1, file);

			previous = node;

			node = Next(&wordBook->words);

		}

		fclose(file);
	}

	return wordBook->length;
}

Long SaveMemorizedWords(WordBook *memorizedWordBook) {
	Node *node;
	Node *previousNode = NULL;
	Word word;
	FILE *file = NULL;

	file = fopen("MemorizedWords.dat", "wb");
	
	if (file != NULL) {
		node = First(&memorizedWordBook->words);

		while(node != previousNode) {
			GetAt(&memorizedWordBook->words, node, &word, sizeof(Word));

			fwrite(&word, sizeof(Word), 1, file);

			previousNode = node;

			node = Next(&memorizedWordBook->words);
		}

		fclose(file);
	}

	return memorizedWordBook->length;
}

void WordBook_Destroy(WordBook* wordBook) {
	Destroy(&wordBook->words);
}

int CompareSpellings(void* one, void* other) {
	Word* word = (Word*)one;
	char(*spelling) = (char(*)) other;

	return strcmp(word->spelling, spelling);
}

int CompareMeanings(void* one, void* other) {
	Word* word = (Word*)one;
	char(*meaning) = (char(*)) other;

	return strcmp(word->meaning, meaning);
}

int CompareWords(void* one, void* other) {
	int result = -1;

	if (one == other) {
		result = 0;
	}

	return result;
}