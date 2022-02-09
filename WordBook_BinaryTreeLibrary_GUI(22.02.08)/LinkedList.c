//LinkedList.c
#include "LinkedList.h"
#include <stdlib.h>
#pragma warning(disable : 4996)


/*
* 명		칭 : Create
* 출		력 :
* 입		력 :
* 기		능 : 연결리스트를 생성한다.
*/
void Create(LinkedList* linkedList) {
	linkedList->head = NULL;
	linkedList->tail = NULL;
	linkedList->length = 0;
	linkedList->current = NULL;
}

/*
* 명		칭 : InsertBefore
* 출		력 : 노드 위치
* 입		력 : 노드 위치, 삽입될 개체
* 기		능 : 입력받은 노드의 앞에 개체를 삽입한다.
*/
Node* InsertBefore(LinkedList* linkedList, Node* index, void* object, size_t size) {

	// 새 노드를 생성한다.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);

	// 새 노드에 개체를 넣는다.
	memcpy(linkedList->current + 1, object, size);

	// 관계를 설정한다.
	linkedList->current->next = index;
	if (linkedList->head != index) {
		linkedList->current->previous = index->previous;
		index->previous->next = linkedList->current;
	}
	else {
		linkedList->current->previous = linkedList->current;
		linkedList->head = linkedList->current;
	}
	
	// 위치의 앞에 새 노드를 넣는다.
	index->previous = linkedList->current;

	// 사용량을 늘린다.
	(linkedList->length)++;

	// 새 노드의 위치를 출력한다.
	return linkedList->current;
}

/*
* 명		칭 : InsertAfter
* 출		력 : 삽입된 노드의 위치
* 입		력 : 노드 위치, 삽입될 개체
* 기		능 : 입력받은 위치의 다음에 개체를 삽입한다.
*/
Node* InsertAfter(LinkedList* linkedList, Node* index, void* object, size_t size) {

	// 새 노드를 생성한다.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);

	// 새 노드에 개체를 넣는다.
	memcpy(linkedList->current + 1, object, size);
	
	// 관계를 설정한다.
	linkedList->current->previous = index;
	if (linkedList->tail != index) {
		linkedList->current->next = index->next;
		index->next->previous = linkedList->current;
	}
	else {
		linkedList->current->next = linkedList->current;
		linkedList->tail = linkedList->current;
	}

	// 뒤에 삽입한다.
	index->next = linkedList->current;

	// 사용량을 늘린다.
	(linkedList->length)++;

	// 새 노드의 위치를 출력한다.
	return linkedList->current;
}

/*
* 명		칭 : AppendFromHead
* 출		력 : 노드 위치
* 입		력 : 개체
* 기		능 : 연결리스트의 머리에 개체를 덧붙인다.
*/
Node* AppendFromHead(LinkedList* linkedList, void* object, size_t size) {
	
	// 새 노드를 생성한다.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	
	// 새 노드에 개체를 넣는다.
	memcpy(linkedList->current + 1, object, size);

	// 관계를 설정한다.
	linkedList->current->previous = linkedList->current;
	if (linkedList->head != NULL) {
		linkedList->current->next = linkedList->head;
		linkedList->head->previous = linkedList->current;
	}
	else {
		linkedList->current->next = linkedList->current;
		linkedList->tail = linkedList->current;
	}

	// 머리에 덧붙인다.
	linkedList->head = linkedList->current;

	// 사용량을 늘린다.
	(linkedList->length)++;

	// 새 노드의 위치를 출력한다.
	return linkedList->current;
}

/*
* 명		칭 : AppendFromTail
* 출		력 : 노드 위치
* 입		력 : 개체
* 기		능 : 연결리스트의 꼬리에 개체를 덧붙인다.
*/
Node* AppendFromTail(LinkedList* linkedList, void* object, size_t size) {
	// 새 노드를 생성한다.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);

	// 새 노드에 개체를 넣는다.
	memcpy(linkedList->current + 1, object, size);

	// 관계를 설정한다.
	linkedList->current->next = linkedList->current;
	if (linkedList->tail != NULL) {
		linkedList->current->previous = linkedList->tail;
		linkedList->tail->next = linkedList->current;
	}
	else {
		linkedList->current->previous = linkedList->current;
		linkedList->head = linkedList->current;
	}

	// 꼬리에 덧붙인다.
	linkedList->tail = linkedList->current;

	// 연결리스트틔 사용량을 늘린다.
	(linkedList->length)++;

	// 새 노드의 위치를 출력한다.
	return linkedList->current;
}

/*
* 명		칭 : Delete
* 출		력 : 노드 위치
* 입		력 : 노드 위치
* 기		능 : 입력받은 위치의 노드를 지운다.
*/
Node* Delete(LinkedList* linkedList, Node* index) {
	
	// 인접 노드와의 관계를 설정한다.
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

	// 위치의 노드를 지운다.
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	
	// 연결리스트의 사용량을 줄인다.
	(linkedList->length)--;

	// 지워진 노드의 위치를 출력한다.
	return index;
}

/*
* 명		칭 : DeleteFromHead
* 출		력 : 노드 위치
* 입		력 :
* 기		능 : 연결리스트의 머리의 노드를 지운다.
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

	// 기존의 헤드 노드를 지운다.
	if (it != NULL) {
		free(it);
		it = NULL;
	}

	// 연결리스트의 사용량을 줄인다.
	(linkedList->length)--;

	return it;
}

/*
* 명		칭 : DeleteFromTail
* 출		력 : 삭제된 노드의 위치
* 입		력 : 
* 기		능 : 연결리스트 꼬리 노드를 지운다.
*/
Node* DeleteFromTail(LinkedList* linkedList) {
	Node* it = NULL;
	
	it = linkedList->tail;

	// 노드 유무에 따라 관계를 설정한다.
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

	// 기존 꼬리 노드를 지운다.
	if (it != NULL) {
		free(it);
		it = NULL;
	}

	// 사용량을 줄인다.
	(linkedList->length)--;

	return it;
}

/*
* 명		칭 : DeleteAllItems
* 출		력 :
* 입		력 :
* 기		능 : 연결리스트의 모든 항목을 삭제한다.
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
* 명		칭 : LinearSearchUnique
* 출		력 : 노드 위치
* 입		력 : 키
* 기		능 : 중복 없이 선형 검색한다.
*/
Node* LinearSearchUnique(LinkedList* linkedList, void* key, int (*compare)(void*, void*)) {
	Node* index = NULL;
	Node* it = NULL;
	Node* previous = NULL;

	it = linkedList->head;
	// 키와 일치하는 노드를 찾는다.
	while (it != previous && compare(it + 1, key) != 0) {
		previous = it;
		it = it->next;
	}

	if (it != previous) {
		index = it;
	}

	// 위치를 출력한다.
	return index;
}


/*
* 명		칭 : LinearSearchDuplicate
* 출		력 : 위치들, 개수
* 입		력 : 키
* 기		능 : 연결리스트에서 중복을 허용해 선형검색한다.
*/
void LinearSearchDuplicate(LinkedList* linkedList, void* key, Node** (*indexes), Long* count, int(*compare)(void*, void*)) {
	Node* it = NULL;
	Node* previous = NULL;
	Long i = 0;
	*count = 0;

	// 사용량 크기의 위치들 배열을 만든다.
	*indexes = (Node * (*)) calloc(linkedList->length, sizeof(Node*));

	// 처음부터 끝까지 반복한다.
	it = linkedList->head;
	while (it != previous) {
		// 키와 일치하면 위치를 적고 개수를 센다.
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
* 명		칭 : First
* 출		력 : 노드 위치
* 입		력 :
* 기		능 : 연결리스트에서 처음으로 이동한다.
*/
Node* First(LinkedList* linkedList) {
	linkedList->current = linkedList->head;
	return linkedList->current;
}

/*
* 명		칭 : Previous
* 출		력 : 노드 위치
* 입		력 :
* 기		능 : 연결리스트에서 이전으로 이동한다.
*/
Node* Previous(LinkedList* linkedList) {
	linkedList->current = linkedList->current->previous;
	return linkedList->current;
}

/*
* 명		칭 : Next
* 출		력 : 노드 위치
* 입		력 :
* 기		능 : 연결리스트에서 다음으로 이동한다.
*/
Node* Next(LinkedList* linkedList) {
	linkedList->current = linkedList->current->next;
	return linkedList->current;
}
/*
* 명		칭 : Last
* 출		력 : 노드 위치
* 입		력 :
* 기		능 : 연결리스트에서 끝으로 이동한다.
*/
Node* Last(LinkedList* linkedList) {
	linkedList->current = linkedList->tail;
	return linkedList->current;
}

/*
* 명		칭 : Move
* 출		력 : 노드 위치
* 입		력 : 노드 위치
* 기		능 : 연결리스트에서 입력받은 위치로 이동한다.
*/
Node* Move(LinkedList* linkedList, Node* index) {
	linkedList->current = index;
	return linkedList->current;
}


/*
* 명		칭 : GetAt
* 출		력 : 개체
* 입		력 : 위치
* 기		능 : 위치의 개체를 출력한다.
*/
void GetAt(LinkedList* linkedList, Node* index, void* object, size_t size) {
	memcpy(object, index + 1, size);
}

/*
* 명		칭 : Destroy
* 출		력 :
* 입		력 :
* 기		능 : 연결리스트를 없앤다.
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