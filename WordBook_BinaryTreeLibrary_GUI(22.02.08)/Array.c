//Array.c
#include "Array.h"
#include <stdlib.h>
#include <memory.h>
#pragma warning(disable : 4996);

/* ��	Ī : Create
*  ��   �� : 
*  ��   �� : �Ҵ緮
*  ��   �� : �Ҵ緮 ũ���� �迭�� �����.
*/
void Array_Create(Array* array, Long capacity, size_t size) {
	array->front = calloc(capacity, size);
	array->capacity = capacity;
	array->length = 0;
}

/*
* ��   Ī : Store
* ��   �� : ��ġ
* ��   �� : ��ġ, ��ü
* ��   �� : �迭�� ��ġ�� ��ü�� �����Ѵ�.
*/
Long Store(Array* array, Long index, void* object, size_t size) {
	// 2. �� ���� �ִ´�.
	memcpy(((char(*))(array->front)) + (index * size), object, size);
	(array->length)++;

	return index;
}

/*
* ��		Ī : Insert
* ��		�� : ��ġ
* ��		�� : ��ġ, ��ü
* ��		�� : �迭�� ��ġ�� ��ü�� �����Ѵ�.
*/
Long Array_Insert(Array* array, Long index, void* object, size_t size) {
	// 1.��ġ ��ü�� �Է¹޴´�.

	void(*objects) = NULL;
	Long i = 0;
	Long j = 0;

	// 2. �迭�� �Ҵ緮���� �ϳ� �� ū ��ü���� �����.
	objects = calloc(array->capacity + 1, size);

	// 3. �迭�� �Ű� ���´�.
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

	// 4. �迭�� ��ü���� �����.
	if (array->front != NULL) {
		free(array->front);
	}

	// 5. �迭�� �� ��ü���� �ִ´�.
	array->front = objects;

	// 6. �迭�� �Ҵ緮�� ������Ų��.
	(array->capacity)++;

	// 7. �迭�� �� ��ü�� �ִ´�.
	memcpy(((char(*))(array->front)) + (index * size), object, size);
	
	// 8. �迭�� ��뷮�� ������Ų��.
	(array->length)++;

	// 9. ��ġ�� ����Ѵ�.
	return index;
}

/*
* ��		Ī : AppendFromFront
* ��		�� : ��ġ
* ��		�� : ��ü
* ��		�� : �迭�� �տ� ��ü�� �����δ�.
*/
Long AppendFromFront(Array* array, void* object, size_t size) {
	Long index = 0;
	void(*objects) = NULL;
	Long i = 0;
	Long j = 1;

	// 2. �迭�� �Ҵ緮���� �ϳ� �� ū ��ü�� �迭�� �����.
	objects = calloc(array->capacity + 1, size);

	// 3. ��ü�鿡 �迭�� �Ű� ���´�.
	while (i < array->length) {
		memcpy(((char(*))objects) + (j * size), ((char(*))(array->front)) + (i * size), size);
		j++;
		i++;
	}

	// 4. �迭�� ��ü���� �����.
	if (array->front != NULL) {
		free(array->front);
	}

	// 5. �迭�� ��ü�鿡 ���� ���� ��ü���� �ִ´�.
	array->front = objects;

	// 6. �迭�� �Ҵ緮�� �ø���.
	(array->capacity)++;

	// 7. �迭�� �տ� ��ü�� �ִ´�.
	memcpy(((char(*))(array->front)) + (index * size), object, size);

	// 8. �迭�� ��뷮�� ������Ų��.
	(array->length)++;

	// 9. ��ü�� ���� ��ġ�� ����Ѵ�.
	return index;
}

/*
* ��		Ī : AppendFromRear(Array *array, void *object, size_t size) {
* ��		�� : ��ġ
* ��		�� : ��ü
* ��		�� : �迭�� �ڿ� ��ü�� �����δ�.
*/
Long AppendFromRear(Array* array, void* object, size_t size) {
	Long index;
	void(*objects) = NULL;
	Long i = 0;
	
	// 2. �迭�� �Ҵ緮���� �ϳ� �� ū ��ü�� �迭�� �����.
	objects = calloc(array->capacity + 1, size);

	// 3. �迭�� ��뷮��ŭ ��ü�鿡 �Ű����´�.
	while (i < array->length) {
		memcpy(((char(*))objects) + (i * size), ((char(*))(array->front)) + (i * size), size);
		i++;
	}

	// 4. �迭�� ��ü���� �����.
	if (array->front != NULL) {
		free(array->front);
	}

	// 5. �迭�� ��ü���� �ٲ۴�.
	array->front = objects;

	// 6. �迭�� �Ҵ緮�� �ø���.
	(array->capacity)++;

	// 7. �迭�� ���� ��ü�� �ִ´�.
	index = array->capacity - 1;
	memcpy(((char(*))(array->front)) + (index * size), object, size);

	// 8. �迭�� ��뷮�� �ø���.
	(array->length)++;

	// 9. ��ü�� ���� ��ġ�� ����Ѵ�.
	return index;
}

/*
* ��		Ī : Delete
* ��		�� : ��ġ
* ��		�� : ��ġ
* ��		�� : �迭�� ��ġ�� �ִ� ��ü�� �����.
*/
Long Array_Delete(Array* array, Long index, size_t size) {
	void(*objects) = NULL;
	Long i = 0;
	Long j = 0;

	// 2. �迭�� �Ҵ緮�� 1���� ũ�� �迭�� �Ҵ緮���� �ϳ� ���� ��ü�� �迭�� �����.
	if (array->capacity > 1) {
		objects = calloc(array->capacity - 1, size);
	}

	// 3. �迭�� �Ű� ���´�.
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

	// 4. �迭�� ��ü���� �����.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}

	// 5. �迭�� �Ҵ緮�� 1���� ũ�� �迭�� ��ü���� ������ ����Ű�� �Ѵ�.
	if (array->capacity > 1) {
		array->front = objects;
	}

	// 6. �迭�� �Ҵ緮�� ���δ�.
	(array->capacity)--;

	// 7. �迭�� ��뷮�� ���δ�.
	(array->length)--;

	index = -1;

	return index;
}

/*
* ��		Ī : DeleteFromFront
* ��		�� : ��ġ
* ��		�� :
* ��		�� : �迭�� ���� ��ü�� �����.
*/
Long DeleteFromFront(Array* array, size_t size) {
	Long index = -1;
	void(*objects) = NULL;
	Long i = 1;
	Long j = 0;

	// 1. �迭�� �Ҵ緮�� 1���� ũ�� �迭�� �Ҵ緮���� �ϳ� ���� ũ���� ��ü�� �迭�� �����.
	if (array->capacity > 1) {
		objects = calloc(array->capacity - 1, size);
	}

	// 2. 2��°���� �迭�� ��뷮���� ��ü�鿡 �Ű����´�.
	while (i < array->length) {
		memcpy(((char(*))objects) + (j * size), ((char(*))(array->front)) + (i * size), size);
		j++;
		i++;
	}

	// 3. �迭�� ��ü���� �����.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}

	// 4. �迭�� �� ��ü���� ������ ����Ű�� �Ѵ�.
	if (array->capacity > 1) {
		array->front = objects;
	}

	// 5. �迭�� �Ҵ緮�� ���δ�.
	(array->capacity)--;

	// 6. �迭�� ��뷮�� ���δ�.
	(array->length)--;

	return index;
}

/*
* ��		Ī : DeleteFromRear
* ��		�� : ��ġ
* ��		�� :
* ��		�� : �迭�� �ڿ��� �����.
*/
Long DeleteFromRear(Array* array, size_t size) {
	Long index = -1;
	void(*objects) = NULL;
	Long i = 0;

	// 1. �迭�� �Ҵ緮�� 1���� ũ�� �迭�� �Ҵ緮���� �ϳ� ���� ũ���� ��ü�� �迭�� �����.
	if (array->capacity > 1) {
		objects = calloc(array->capacity - 1, size);
	}

	// 2. �迭�� ��ü���� �Ű����´�.
	while (i < array->length - 1) {
		memcpy(((char(*))objects) + (i * size), ((char(*))(array->front)) + (i * size), size);
		i++;
	}

	// 3. �迭�� ��ü���� �����.
	if (array->front != NULL) {
		free(array->front);
		array->front = NULL;
	}

	// 4. �迭�� �Ҵ緮�� 1���� ũ�� �迭�� ��ü���� ������ ���ο� ��ü��� �Ѵ�.
	if (array->capacity > 1) {
		array->front = objects;
	}

	// 5. �Ҵ緮�� ��뷮�� ���δ�.
	(array->capacity)--;
	(array->length)--;

	return index;
}

/*
* ��		Ī : Clear
* ��		�� :
* ��		�� :
* ��		�� : �迭�� �����.
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
* ��		Ī : Modify
* ��		�� : ��ġ
* ��		�� : ��ġ, ��ü
* ��		�� : �迭���� ��ġ�� ��ü�� ��ģ��.
*/
Long Modify(Array* array, Long index, void* object, size_t size) {
	memcpy(((char(*))(array->front)) + (index * size), object, size);

	return index;
}

/*
* ��		Ī : LinearSearchUnique
* ��		�� : ��ġ
* ��		�� : Ű
* ��		�� : �迭���� �������ϰ˻��Ѵ�.
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
* ��		Ī : LinearSearchDuplicate
* ��		�� : ��ġ��, ����
* ��		�� : Ű
* ��		�� : �迭���� �����ߺ��˻��Ѵ�.
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
* ��		Ī : BinarySearchUnique
* ��		�� : ��ġ
* ��		�� : Ű
* ��		�� : �������ϰ˻��Ѵ�.
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
* ��		Ī : BinarySearchDuplicate
* ��		�� : ��ġ��, ����
* ��		�� : Ű
* ��		�� : �����ߺ��˻��Ѵ�.
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

	// ������ �� �����̰� ��� ���� Ű�� �ٸ� ���� �ݺ��Ѵ�.
	while (start <= end && compare(((char(*))(array->front)) + (middle * size), key) != 0) {
		// 3.1. ���۰� ���� ���Ѵ�.
		if (compare(((char(*))(array->front)) + (middle * size), key) < 0) {
			start = middle + 1;
		}
		else {
			end = middle - 1;
		}
		// 3.2. ����� ���Ѵ�.
		middle = (start + end) / 2;
	}
	
	// ���� �͵��� ������ ã�´�.
	index = middle - 1;
	while (index >= start && compare(((char(*))(array->front)) + (index * size), key) == 0) {
		index--;
	}

	if (index < 0) {
		index = -1;
	}

	index++;

	// ��ġ���� �ִ´�.
	while (index <= end && compare(((char(*))(array->front)) + (index * size), key) == 0) {
		(*indexes)[i] = index;
		i++;
		(*count)++;
		index++;
	}

}

/*
* ��		Ī : SelectionSort
* ��		�� :
* ��		�� :
* ��		�� : ��������
*/
void SelectionSort(Array* array, size_t size, int(*compare)(void*, void*)) {
	Long minIndex;
	void* object = NULL;
	Long i = 0;
	Long j;

	object = malloc(size);

	// 1. �迭�� ��뷮 - 1��ŭ �ݺ��Ѵ�.
	while (i < array->length - 1) {
		// 1.1. �ּڰ��� ��ġ�� �����Ѵ�.
		minIndex = i;
		// 1.2. ����ĭ���� �迭�� ������ �ݺ��Ѵ�.
		j = i + 1;
		while (j < array->length) {
			// 1.2.1. �ּڰ��� ��ġ�� ã�´�.
			if (compare(((char(*))(array->front)) + (minIndex * size), ((char(*))(array->front)) + (j * size)) > 0) {
				minIndex = j;
			}
			j++;
		}

		// 1.3. �ڸ��� ���Ѵ�.
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
* ��		Ī : BubbleSort
* ��		�� : 
* ��		�� : 
* ��		�� : ���������Ѵ�.
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
* ��		Ī : InsertionSort
* ��		�� :
* ��		�� :
* ��		�� : ���������Ѵ�.
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
* ��		Ī : Merge
* ��		�� : 
* ��		�� : �迭1, �迭2
* ��		�� : �� �迭�� ������ �迭�� �����.
*/
void Merge(Array* array, Array* one, Array* other, size_t size, int(*compare) (void*, void*)) {
	Long i = 0;
	Long j = 0;
	Long k = 0;

	if (array->front != NULL) {
		free(array->front);
	}

	// 1. �Է¹��� �� �迭�� ��뷮�� ���� ũ���� ��ü�� �迭�� �����.
	array->front = calloc(one->length + other->length, size);
	array->capacity = one->length + other->length;

	// 2. �� �迭 �� ��� �� �迭�� ���� �̸� ������ �ݺ��Ѵ�.
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

	// �迭 1�� ���� �̸��� ���� �ݺ��Ѵ�.
	
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
* ��		Ī : GetAt
* ��		�� : ��ü
* ��		�� : ��ġ
* ��		�� : ��ġ�� ��ü�� �����´�.
*/
void Array_GetAt(Array* array, Long index, void* object, size_t size) {
	memcpy(object, ((char(*))(array->front)) + (index * size), size);
}

/*
* ��   Ī : Destroy
* ��   �� :
* ��   �� :
* ��   �� : �迭�� ���ش�.
*/
void Array_Destroy(Array* array) {
	if (array->front != NULL) {
		free(array->front);
	}
}