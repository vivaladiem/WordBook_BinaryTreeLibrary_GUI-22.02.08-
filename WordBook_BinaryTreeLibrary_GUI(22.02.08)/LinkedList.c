//LinkedList.c
#include "LinkedList.h"
#include <stdlib.h>
#pragma warning(disable : 4996)


/*
* ��		Ī : Create
* ��		�� :
* ��		�� :
* ��		�� : ���Ḯ��Ʈ�� �����Ѵ�.
*/
void Create(LinkedList* linkedList) {
	linkedList->head = NULL;
	linkedList->tail = NULL;
	linkedList->length = 0;
	linkedList->current = NULL;
}

/*
* ��		Ī : InsertBefore
* ��		�� : ��� ��ġ
* ��		�� : ��� ��ġ, ���Ե� ��ü
* ��		�� : �Է¹��� ����� �տ� ��ü�� �����Ѵ�.
*/
Node* InsertBefore(LinkedList* linkedList, Node* index, void* object, size_t size) {

	// �� ��带 �����Ѵ�.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);

	// �� ��忡 ��ü�� �ִ´�.
	memcpy(linkedList->current + 1, object, size);

	// ���踦 �����Ѵ�.
	linkedList->current->next = index;
	if (linkedList->head != index) {
		linkedList->current->previous = index->previous;
		index->previous->next = linkedList->current;
	}
	else {
		linkedList->current->previous = linkedList->current;
		linkedList->head = linkedList->current;
	}
	
	// ��ġ�� �տ� �� ��带 �ִ´�.
	index->previous = linkedList->current;

	// ��뷮�� �ø���.
	(linkedList->length)++;

	// �� ����� ��ġ�� ����Ѵ�.
	return linkedList->current;
}

/*
* ��		Ī : InsertAfter
* ��		�� : ���Ե� ����� ��ġ
* ��		�� : ��� ��ġ, ���Ե� ��ü
* ��		�� : �Է¹��� ��ġ�� ������ ��ü�� �����Ѵ�.
*/
Node* InsertAfter(LinkedList* linkedList, Node* index, void* object, size_t size) {

	// �� ��带 �����Ѵ�.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);

	// �� ��忡 ��ü�� �ִ´�.
	memcpy(linkedList->current + 1, object, size);
	
	// ���踦 �����Ѵ�.
	linkedList->current->previous = index;
	if (linkedList->tail != index) {
		linkedList->current->next = index->next;
		index->next->previous = linkedList->current;
	}
	else {
		linkedList->current->next = linkedList->current;
		linkedList->tail = linkedList->current;
	}

	// �ڿ� �����Ѵ�.
	index->next = linkedList->current;

	// ��뷮�� �ø���.
	(linkedList->length)++;

	// �� ����� ��ġ�� ����Ѵ�.
	return linkedList->current;
}

/*
* ��		Ī : AppendFromHead
* ��		�� : ��� ��ġ
* ��		�� : ��ü
* ��		�� : ���Ḯ��Ʈ�� �Ӹ��� ��ü�� �����δ�.
*/
Node* AppendFromHead(LinkedList* linkedList, void* object, size_t size) {
	
	// �� ��带 �����Ѵ�.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	
	// �� ��忡 ��ü�� �ִ´�.
	memcpy(linkedList->current + 1, object, size);

	// ���踦 �����Ѵ�.
	linkedList->current->previous = linkedList->current;
	if (linkedList->head != NULL) {
		linkedList->current->next = linkedList->head;
		linkedList->head->previous = linkedList->current;
	}
	else {
		linkedList->current->next = linkedList->current;
		linkedList->tail = linkedList->current;
	}

	// �Ӹ��� �����δ�.
	linkedList->head = linkedList->current;

	// ��뷮�� �ø���.
	(linkedList->length)++;

	// �� ����� ��ġ�� ����Ѵ�.
	return linkedList->current;
}

/*
* ��		Ī : AppendFromTail
* ��		�� : ��� ��ġ
* ��		�� : ��ü
* ��		�� : ���Ḯ��Ʈ�� ������ ��ü�� �����δ�.
*/
Node* AppendFromTail(LinkedList* linkedList, void* object, size_t size) {
	// �� ��带 �����Ѵ�.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);

	// �� ��忡 ��ü�� �ִ´�.
	memcpy(linkedList->current + 1, object, size);

	// ���踦 �����Ѵ�.
	linkedList->current->next = linkedList->current;
	if (linkedList->tail != NULL) {
		linkedList->current->previous = linkedList->tail;
		linkedList->tail->next = linkedList->current;
	}
	else {
		linkedList->current->previous = linkedList->current;
		linkedList->head = linkedList->current;
	}

	// ������ �����δ�.
	linkedList->tail = linkedList->current;

	// ���Ḯ��ƮƷ ��뷮�� �ø���.
	(linkedList->length)++;

	// �� ����� ��ġ�� ����Ѵ�.
	return linkedList->current;
}

/*
* ��		Ī : Delete
* ��		�� : ��� ��ġ
* ��		�� : ��� ��ġ
* ��		�� : �Է¹��� ��ġ�� ��带 �����.
*/
Node* Delete(LinkedList* linkedList, Node* index) {
	
	// ���� ������ ���踦 �����Ѵ�.
	if (linkedList->head != index && linkedList->tail != index) {
		index->previous->next = index->next;
		index->next->previous = index->previous;
		linkedList->current = index->next;
	}
	else if (linkedList->head != index && linkedList->tail == index) {
		index->previous->next = index->previous;
		linkedList->tail = index->previous;
		linkedList->current = index->previous;
	}
	else if (linkedList->head == index && linkedList->tail != index) {
		index->next->previous = index->next;
		linkedList->head = index->next;
		linkedList->current = index->next;
	}
	else if (linkedList->head == index && linkedList->tail == index) {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}

	// ��ġ�� ��带 �����.
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	
	// ���Ḯ��Ʈ�� ��뷮�� ���δ�.
	(linkedList->length)--;

	// ������ ����� ��ġ�� ����Ѵ�.
	return index;
}

/*
* ��		Ī : DeleteFromHead
* ��		�� : ��� ��ġ
* ��		�� :
* ��		�� : ���Ḯ��Ʈ�� �Ӹ��� ��带 �����.
*/
Node* DeleteFromHead(LinkedList* linkedList) {
	Node* it = NULL;

	it = linkedList->head;
	if (linkedList->tail != it) {
		it->next->previous = it->next;
		linkedList->head = it->next;
		linkedList->current = linkedList->head;
	}
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}

	// ������ ��� ��带 �����.
	if (it != NULL) {
		free(it);
		it = NULL;
	}

	// ���Ḯ��Ʈ�� ��뷮�� ���δ�.
	(linkedList->length)--;

	return it;
}

/*
* ��		Ī : DeleteFromTail
* ��		�� : ������ ����� ��ġ
* ��		�� : 
* ��		�� : ���Ḯ��Ʈ ���� ��带 �����.
*/
Node* DeleteFromTail(LinkedList* linkedList) {
	Node* it = NULL;
	
	it = linkedList->tail;

	// ��� ������ ���� ���踦 �����Ѵ�.
	if (linkedList->head != NULL) {
		it->previous->next = it->previous;
		linkedList->tail = it->previous;
		linkedList->current = linkedList->tail;
	}
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}

	// ���� ���� ��带 �����.
	if (it != NULL) {
		free(it);
		it = NULL;
	}

	// ��뷮�� ���δ�.
	(linkedList->length)--;

	return it;
}

/*
* ��		Ī : DeleteAllItems
* ��		�� :
* ��		�� :
* ��		�� : ���Ḯ��Ʈ�� ��� �׸��� �����Ѵ�.
*/
void DeleteAllItems(LinkedList* linkedList) {
	Node* it = NULL;
	Node* previous = NULL;

	it = linkedList->head;
	while (it != previous) {
		linkedList->head = it->next;
		previous = it;
		if (it != NULL) {
			free(it);
		}
		it = linkedList->head;
	}
	linkedList->head = NULL;
	linkedList->tail = NULL;
	linkedList->length = 0;
	linkedList->current = NULL;
}

/*
* ��		Ī : LinearSearchUnique
* ��		�� : ��� ��ġ
* ��		�� : Ű
* ��		�� : �ߺ� ���� ���� �˻��Ѵ�.
*/
Node* LinearSearchUnique(LinkedList* linkedList, void* key, int (*compare)(void*, void*)) {
	Node* index = NULL;
	Node* it = NULL;
	Node* previous = NULL;

	it = linkedList->head;
	// Ű�� ��ġ�ϴ� ��带 ã�´�.
	while (it != previous && compare(it + 1, key) != 0) {
		previous = it;
		it = it->next;
	}

	if (it != previous) {
		index = it;
	}

	// ��ġ�� ����Ѵ�.
	return index;
}


/*
* ��		Ī : LinearSearchDuplicate
* ��		�� : ��ġ��, ����
* ��		�� : Ű
* ��		�� : ���Ḯ��Ʈ���� �ߺ��� ����� �����˻��Ѵ�.
*/
void LinearSearchDuplicate(LinkedList* linkedList, void* key, Node** (*indexes), Long* count, int(*compare)(void*, void*)) {
	Node* it = NULL;
	Node* previous = NULL;
	Long i = 0;
	*count = 0;

	// ��뷮 ũ���� ��ġ�� �迭�� �����.
	*indexes = (Node * (*)) calloc(linkedList->length, sizeof(Node*));

	// ó������ ������ �ݺ��Ѵ�.
	it = linkedList->head;
	while (it != previous) {
		// Ű�� ��ġ�ϸ� ��ġ�� ���� ������ ����.
		if (compare(it + 1, key) == 0) {
			(*indexes)[i] = it;
			i++;
			(*count)++;
		}
		previous = it;
		it = it->next;
	}
}

/*
* ��		Ī : First
* ��		�� : ��� ��ġ
* ��		�� :
* ��		�� : ���Ḯ��Ʈ���� ó������ �̵��Ѵ�.
*/
Node* First(LinkedList* linkedList) {
	linkedList->current = linkedList->head;
	return linkedList->current;
}

/*
* ��		Ī : Previous
* ��		�� : ��� ��ġ
* ��		�� :
* ��		�� : ���Ḯ��Ʈ���� �������� �̵��Ѵ�.
*/
Node* Previous(LinkedList* linkedList) {
	linkedList->current = linkedList->current->previous;
	return linkedList->current;
}

/*
* ��		Ī : Next
* ��		�� : ��� ��ġ
* ��		�� :
* ��		�� : ���Ḯ��Ʈ���� �������� �̵��Ѵ�.
*/
Node* Next(LinkedList* linkedList) {
	linkedList->current = linkedList->current->next;
	return linkedList->current;
}
/*
* ��		Ī : Last
* ��		�� : ��� ��ġ
* ��		�� :
* ��		�� : ���Ḯ��Ʈ���� ������ �̵��Ѵ�.
*/
Node* Last(LinkedList* linkedList) {
	linkedList->current = linkedList->tail;
	return linkedList->current;
}

/*
* ��		Ī : Move
* ��		�� : ��� ��ġ
* ��		�� : ��� ��ġ
* ��		�� : ���Ḯ��Ʈ���� �Է¹��� ��ġ�� �̵��Ѵ�.
*/
Node* Move(LinkedList* linkedList, Node* index) {
	linkedList->current = index;
	return linkedList->current;
}


/*
* ��		Ī : GetAt
* ��		�� : ��ü
* ��		�� : ��ġ
* ��		�� : ��ġ�� ��ü�� ����Ѵ�.
*/
void GetAt(LinkedList* linkedList, Node* index, void* object, size_t size) {
	memcpy(object, index + 1, size);
}

/*
* ��		Ī : Destroy
* ��		�� :
* ��		�� :
* ��		�� : ���Ḯ��Ʈ�� ���ش�.
*/
void Destroy(LinkedList* linkedList) {
	Node* it = NULL;
	Node* previous = NULL;

	it = linkedList->head;
	while (it != previous) {
		linkedList->head = it->next;
		previous = it;
		if (it != NULL) {
			free(it);
			it = NULL;
		}
		it = linkedList->head;
	}

	linkedList->head = NULL;
	linkedList->tail = NULL;
	linkedList->length = 0;
	linkedList->current = NULL;
}