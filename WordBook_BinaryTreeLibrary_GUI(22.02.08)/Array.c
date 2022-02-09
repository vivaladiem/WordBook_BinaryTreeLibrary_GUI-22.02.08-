//Array.c
#include "Array.h"
#include <stdlib.h>
#include <memory.h>
#pragma warning(disable : 4996);

/* 명	칭 : Create
*  출   력 : 
*  입   력 : 할당량
*  설   명 : 할당량 크기의 배열을 만든다.
*/
void Array_Create(Array* array, Long capacity, size_t size) {
	array->front = calloc(capacity, size);
	array->capacity = capacity;
	array->length = 0;
}

/*
* 명   칭 : Store
* 출   력 : 위치
* 입   력 : 위치, 개체
* 설   명 : 배열의 위치에 개체를 저장한다.
*/
Long Store(Array* array, Long index, void* object, size_t size) {
	// 2. 빈 곳에 넣는다.
	memcpy(((char(*))(array->front)) + (index * size), object, size);
	(array->length)++;

	return index;
}

/*
* 명		칭 : Insert
* 출		력 : 위치
* 입		력 : 위치, 개체
* 설		명 : 배열의 위치에 개체를 삽입한다.
*/
Long Array_Insert(Array* array, Long index, void* object, size_t size) {
	// 1.위치 개체를 입력받는다.

	void(*objects) = NULL;
	Long i = 0;
	Long j = 0;

	// 2. 배열의 할당량보다 하나 더 큰 개체들을 만든다.
	objects = calloc(array->capacity + 1, size);

	// 3. 배열을 옮겨 적는다.
	while (i < index) {
		memcpy(((char(*))objects) + (j * size), ((char(*))(array->front)) + (i * size), size);
		j++;
		i++;
	}
	j++;
	while (i < array->length) {
		memcpy(((char(*))objects) + (j * size), ((char(*))(array->front)) + (i * size), size);
		j++;
		i++;
	}

	// 4. 배열의 개체들을 지운다.
	if (array->front != NULL) {
		free(array->front);
	}

	// 5. 배열에 새 개체들을 넣는다.
	array->front = objects;

	// 6. 배열의 할당량을 증가시킨다.
	(array->capacity)++;

	// 7. 배열에 새 개체를 넣는다.
	memcpy(((char(*))(array->front)) + (index * size), object, size);
	
	// 8. 배열의 사용량을 증가시킨다.
	(array->length)++;

	// 9. 위치를 출력한다.
	return index;
}

/*
* 명		칭 : AppendFromFront
* 출		력 : 위치
* 입		력 : 개체
* 설		명 : 배열의 앞에 개체를 덧붙인다.
*/
Long AppendFromFront(Array* array, void* object, size_t size) {
	Long index = 0;
	void(*objects) = NULL;
	Long i = 0;
	Long j = 1;

	// 2. 배열의 할당량보다 하나 더 큰 개체들 배열을 만든다.
	objects = calloc(array->capacity + 1, size);

	// 3. 개체들에 배열을 옮겨 적는다.
	while (i < array->length) {
		memcpy(((char(*))objects) + (j * size), ((char(*))(array->front)) + (i * size), size);
		j++;
		i++;
	}

	// 4. 배열의 개체들을 지운다.
	if (array->front != NULL) {
		free(array->front);
	}

	// 5. 배열의 개체들에 새로 만든 개체들을 넣는다.
	array->front = objects;

	// 6. 배열의 할당량을 늘린다.
	(array->capacity)++;

	// 7. 배열의 앞에 개체를 넣는다.
	memcpy(((char(*))(array->front)) + (index * size), object, size);

	// 8. 배열의 사용량을 증가시킨다.
	(array->length)++;

	// 9. 개체를 넣은 위치를 출력한다.
	return index;
}

/*
* 명		칭 : AppendFromRear(Array *array, void *object, size_t size) {
* 출		력 : 위치
* 입		력 : 개체
* 설		명 : 배열의 뒤에 개체를 덧붙인다.
*/
Long AppendFromRear(Array* array, void* object, size_t size) {
	Long index;
	void(*objects) = NULL;
	Long i = 0;
	
	// 2. 배열의 할당량보다 하나 더 큰 개체들 배열을 만든다.
	objects = calloc(array->capacity + 1, size);

	// 3. 배열의 사용량만큼 개체들에 옮겨적는다.
	while (i < array->length) {
		memcpy(((char(*))objects) + (i * size), ((char(*))(array->front)) + (i * size), size);
		i++;
	}

	// 4. 배열의 개체들을 지운다.
	if (array->front != NULL) {
		free(array->front);
	}

	// 5. 배열의 개체들을 바꾼다.
	array->front = objects;

	// 6. 배열의 할당량을 늘린다.
	(array->capacity)++;

	// 7. 배열의 끝에 개체를 넣는다.
	index = array->capacity - 1;
	memcpy(((char(*))(array->front)) + (index * size), object, size);

	// 8. 배열의 사용량을 늘린다.
	(array->length)++;

	// 9. 개체를 넣은 위치를 출력한다.
	return index;
}

/*
* 명		칭 : Delete
* 출		력 : 위치
* 입		력 : 위치
* 설		명 : 배열의 위치에 있는 개체를 지운다.
*/
Long Array_Delete(Array* array, Long index, size_t size) {
	void(*objects) = NULL;
	Long i = 0;
	Long j = 0;

	// 2. 배열의 할당량이 1보다 크면 배열의 할당량보다 하나 적은 개체들 배열을 만든다.
	if (array->capacity > 1) {
		objects = calloc(array->capacity - 1, size);
	}

	// 3. 배열을 옮겨 적는다.
	while (i < index) {
		memcpy(((char(*))objects) + (j * size), ((char(*))(array->front)) + (i * size), size);
		j++;
		i++;
	}
	i++;
	while (i < array->length) {
		memcpy(((char(*))objects) + (j * size), ((char(*))(array->front)) + (i * size), size);
		j++;
		i++;
	}

	// 4. 배열의 개체들을 지운다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}

	// 5. 배열의 할당량이 1보다 크면 배열이 개체들의 시작을 가리키게 한다.
	if (array->capacity > 1) {
		array->front = objects;
	}

	// 6. 배열의 할당량을 줄인다.
	(array->capacity)--;

	// 7. 배열의 사용량을 줄인다.
	(array->length)--;

	index = -1;

	return index;
}

/*
* 명		칭 : DeleteFromFront
* 출		력 : 위치
* 입		력 :
* 설		명 : 배열의 앞의 개체를 지운다.
*/
Long DeleteFromFront(Array* array, size_t size) {
	Long index = -1;
	void(*objects) = NULL;
	Long i = 1;
	Long j = 0;

	// 1. 배열의 할당량이 1보다 크면 배열의 할당량보다 하나 적은 크기의 개체들 배열을 만든다.
	if (array->capacity > 1) {
		objects = calloc(array->capacity - 1, size);
	}

	// 2. 2번째부터 배열의 사용량까지 개체들에 옮겨적는다.
	while (i < array->length) {
		memcpy(((char(*))objects) + (j * size), ((char(*))(array->front)) + (i * size), size);
		j++;
		i++;
	}

	// 3. 배열의 개체들을 지운다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}

	// 4. 배열이 새 개체들의 시작을 가리키게 한다.
	if (array->capacity > 1) {
		array->front = objects;
	}

	// 5. 배열의 할당량을 줄인다.
	(array->capacity)--;

	// 6. 배열의 사용량을 줄인다.
	(array->length)--;

	return index;
}

/*
* 명		칭 : DeleteFromRear
* 출		력 : 위치
* 입		력 :
* 설		명 : 배열의 뒤에서 지운다.
*/
Long DeleteFromRear(Array* array, size_t size) {
	Long index = -1;
	void(*objects) = NULL;
	Long i = 0;

	// 1. 배열의 할당량이 1보다 크면 배열의 할당량보다 하나 적은 크기의 개체들 배열을 만든다.
	if (array->capacity > 1) {
		objects = calloc(array->capacity - 1, size);
	}

	// 2. 배열의 개체들을 옮겨적는다.
	while (i < array->length - 1) {
		memcpy(((char(*))objects) + (i * size), ((char(*))(array->front)) + (i * size), size);
		i++;
	}

	// 3. 배열의 개체들을 지운다.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}

	// 4. 배열의 할당량이 1보다 크면 배열의 개체들의 시작을 새로운 개체들로 한다.
	if (array->capacity > 1) {
		array->front = objects;
	}

	// 5. 할당량과 사용량을 줄인다.
	(array->capacity)--;
	(array->length)--;

	return index;
}

/*
* 명		칭 : Clear
* 출		력 :
* 입		력 :
* 설		명 : 배열을 지운다.
*/
void Clear(Array* array) {
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}

	array->capacity = 0;
	array->length = 0;
}

/*
* 명		칭 : Modify
* 출		력 : 위치
* 입		력 : 위치, 개체
* 설		명 : 배열에서 위치의 개체를 고친다.
*/
Long Modify(Array* array, Long index, void* object, size_t size) {
	memcpy(((char(*))(array->front)) + (index * size), object, size);

	return index;
}

/*
* 명		칭 : LinearSearchUnique
* 출		력 : 위치
* 입		력 : 키
* 설		명 : 배열에서 선형유일검색한다.
*/
Long Array_LinearSearchUnique(Array* array, void* key, size_t size, int (*compare)(void*, void*)) {
	Long index = -1;
	Long i = 0;
	
	while (i < array->length && compare(((char(*))(array->front)) + (i * size), key) != 0) {
		i++;
	}

	if (i < array->length) {
		index = i;
	}

	return index;
}

/*
* 명		칭 : LinearSearchDuplicate
* 출		력 : 위치들, 개수
* 입		력 : 키
* 설		명 : 배열에서 선형중복검색한다.
*/
void Array_LinearSearchDuplicate(Array* array, void* key, Long* (*indexes), Long* count, size_t size, int (*compare) (void*, void*)) {
	Long i = 0;
	Long j = 0;
	*count = 0;

	*indexes = (Long(*)) calloc(array->length, sizeof(Long));
	while (i < array->length) {
		if (compare(((char(*))(array->front)) + (i * size), key) == 0) {
			(*indexes)[j] = i;
			j++;
			(*count)++;
		}
		i++;
	}
}

/*
* 명		칭 : BinarySearchUnique
* 출		력 : 위치
* 입		력 : 키
* 설		명 : 이진유일검색한다.
*/
Long BinarySearchUnique(Array* array, void* key, size_t size, int (*compare)(void*, void*)) {
	Long index = -1;
	Long start = 0;
	Long end;
	Long middle;

	end = array->length - 1;
	middle = (start + end) / 2;

	while (start <= end && compare(((char(*))(array->front)) + (middle * size), key) != 0) {
		if (compare(((char(*))(array->front)) + (middle * size), key) < 0) {
			start = middle + 1;
		}
		else {
			end = middle - 1;
		}
		middle = (start + end) / 2;
	}

	if (start <= end) {
		index = middle;
	}

	return index;
}

/*
* 명		칭 : BinarySearchDuplicate
* 출		력 : 위치들, 개수
* 입		력 : 키
* 설		명 : 이진중복검색한다.
*/
void BinarySearchDuplicate(Array* array, void* key, Long* (*indexes), Long* count, size_t size, int (*compare)(void*, void*)) {
	Long start = 0;
	Long end;
	Long middle;
	Long index;
	Long i = 0;
	*count = 0;

	*indexes = calloc(array->length, sizeof(Long));

	end = array->length;
	middle = (start + end) / 2;

	// 시작이 끝 이하이고 가운데 값이 키와 다른 동안 반복한다.
	while (start <= end && compare(((char(*))(array->front)) + (middle * size), key) != 0) {
		// 3.1. 시작과 끝을 정한다.
		if (compare(((char(*))(array->front)) + (middle * size), key) < 0) {
			start = middle + 1;
		}
		else {
			end = middle - 1;
		}
		// 3.2. 가운데를 구한다.
		middle = (start + end) / 2;
	}
	
	// 같은 것들의 시작을 찾는다.
	index = middle - 1;
	while (index >= start && compare(((char(*))(array->front)) + (index * size), key) == 0) {
		index--;
	}

	if (index < 0) {
		index = -1;
	}

	index++;

	// 위치들을 넣는다.
	while (index <= end && compare(((char(*))(array->front)) + (index * size), key) == 0) {
		(*indexes)[i] = index;
		i++;
		(*count)++;
		index++;
	}

}

/*
* 명		칭 : SelectionSort
* 출		력 :
* 입		력 :
* 설		명 : 선택정렬
*/
void SelectionSort(Array* array, size_t size, int(*compare)(void*, void*)) {
	Long minIndex;
	void* object = NULL;
	Long i = 0;
	Long j;

	object = malloc(size);

	// 1. 배열의 사용량 - 1만큼 반복한다.
	while (i < array->length - 1) {
		// 1.1. 최솟값의 위치를 리셋한다.
		minIndex = i;
		// 1.2. 다음칸부터 배열의 끝까지 반복한다.
		j = i + 1;
		while (j < array->length) {
			// 1.2.1. 최솟값의 위치를 찾는다.
			if (compare(((char(*))(array->front)) + (minIndex * size), ((char(*))(array->front)) + (j * size)) > 0) {
				minIndex = j;
			}
			j++;
		}

		// 1.3. 자리를 정한다.
		memcpy(object, ((char(*))(array->front)) + (i * size), size);
		memcpy(((char(*))(array->front)) + (i * size), ((char(*))(array->front)) + (minIndex * size), size);
		memcpy(((char(*))(array->front)) + (minIndex * size), object, size);
		i++;
	}
	if (object != NULL) {
		free(object);
	}
}

/*
* 명		칭 : BubbleSort
* 출		력 : 
* 입		력 : 
* 기		능 : 버블정렬한다.
*/
void BubbleSort(Array* array, size_t size, int (*compare) (void*, void*)) {
	Long count = 1;
	Long i;
	Long j;
	void* object = NULL;
	object = malloc(size);

	i = array->length - 2;

	while (i >= 0 && count > 0) {
		count = 0;
		j = 0;
		while (j <= i) {
			if (compare(((char(*))(array->front)) + (j * size), ((char(*))(array->front)) + ((j + 1) * size)) > 0) {
				memcpy(object, ((char(*))(array->front)) + (j * size), size);
				memcpy(((char(*))(array->front)) + (j * size), ((char(*))(array->front)) + ((j + 1) * size), size);
				memcpy(((char(*))(array->front)) + ((j + 1) * size), object, size);
				count = 1;
			}
			j++;
		}
		i--;
	}
	if (object != NULL) {
		free(object);
	}
}

/*
* 명		칭 : InsertionSort
* 출		력 :
* 입		력 :
* 기		능 : 삽입정렬한다.
*/
void InsertionSort(Array* array, size_t size, int (*compare) (void*, void*)) {
	void* object = NULL;
	Long i = 1;
	Long j;

	object = malloc(size);

	while (i < array->length) {
		memcpy(object, ((char(*))(array->front)) + (i * size), size);

		j = i - 1;

		while (j >= 0 && compare(((char(*))(array->front)) + (j * size), object) > 0) {
			memcpy(((char(*))(array->front)) + ((j + 1) * size), ((char(*))(array->front)) + (j * size), size);
			j--;
		}

		memcpy(((char(*))(array->front)) + ((j + 1) * size), object, size);
		i++;
	}

	if (object != NULL) {
		free(object);
	}
}

/*
* 명		칭 : Merge
* 출		력 : 
* 입		력 : 배열1, 배열2
* 기		능 : 두 배열을 병합해 배열을 만든다.
*/
void Merge(Array* array, Array* one, Array* other, size_t size, int(*compare) (void*, void*)) {
	Long i = 0;
	Long j = 0;
	Long k = 0;

	if (array->front != NULL) {
		free(array->front);
	}

	// 1. 입력받은 두 배열의 사용량을 합한 크기의 개체들 배열을 만든다.
	array->front = calloc(one->length + other->length, size);
	array->capacity = one->length + other->length;

	// 2. 두 배열 중 어느 한 배열의 끝에 이를 때까지 반복한다.
	while (i < one->length && j < other->length) {
		if (compare(((char(*))(one->front)) + (i * size), ((char(*))(other->front)) + (j * size)) <= 0) {
			memcpy(((char(*))(array->front)) + (k * size), ((char(*))(one->front)) + (i * size), size);
			k++;
			i++;
		}
		else {
			memcpy(((char(*))(array->front)) + (k * size), ((char(*))(other->front)) + (j * size), size);
			k++;
			j++;
		}
	}

	// 배열 1의 끝에 이르는 동안 반복한다.
	
	while (i < one->length) {
		memcpy(((char(*))(array->front)) + (k * size), ((char(*))(one->front)) + (i * size), size);
		k++;
		i++;
	}

	while (j < other->length) {
		memcpy(((char(*))(array->front)) + (k * size), ((char(*))(other->front)) + (j * size), size);
		k++;
		j++;
	}

	array->length = one->length + other->length;
}

/*
* 명		칭 : GetAt
* 출		력 : 개체
* 입		력 : 위치
* 설		명 : 위치의 개체를 가져온다.
*/
void Array_GetAt(Array* array, Long index, void* object, size_t size) {
	memcpy(object, ((char(*))(array->front)) + (index * size), size);
}

/*
* 명   칭 : Destroy
* 출   력 :
* 입   력 :
* 설   명 : 배열을 없앤다.
*/
void Array_Destroy(Array* array) {
	if (array->front != NULL) {
		free(array->front);
	}
}