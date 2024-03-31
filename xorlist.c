#include "xorlist.h"

/**
 * @brief 두 개의 노드 포인터 주소를 XOR 연산하여 반환합니다.
 *
 * @param a XOR 연산을 수행할 첫 번째 노드
 * @param b XOR 연산을 수행할 두 번째 노드
 * @return XOR 연산 결과로 얻은 노드 포인터
 */
t_node* XOR(t_node* a, t_node* b) {
	return (t_node*)((uintptr_t)(a) ^ (uintptr_t)(b));
}

/**
 * @brief XOR 리스트에 지정된 인덱스에 주어진 데이터를 가진 새로운 노드를 삽입합니다.
 *
 * @param list XOR 리스트.
 * @param data 삽입할 데이터.
 * @param index 새로운 노드를 삽입할 인덱스.
 * @return 삽입이 성공 : 0, 실패 : 1, 매개변수 오류 : -1 을 반환합니다.
 */
int list_insert(t_xorlist* list, int data, int index)
{
	if (list == NULL || index <= 0 || index > list->size)
		return (-1);
	else if (index == list->size)
		return (list_append(list, data));
	t_node* new_node = (t_node*)malloc(sizeof(t_node));
	if (!new_node) // 할당실패시 1리턴
		return (1);
	new_node->data = data;
	t_node* cur = list->head;
	t_node* prev = NULL;
	t_node* next;
	for(int i = 0; i < index && cur != NULL; i++)
	{
		next = XOR(prev, cur->xptr);
		prev = cur;
		cur = next;
	}
	//prev와 curr 사이에 newnode가 들어오는 형식임.
	new_node->xptr = XOR(prev, cur);
	// 원래 prev의 xptr은 prev_prev=(prev->xptr^cur) ^ cur 이었기 때문에
	// prev_prev^newnode 로 바꿔줘야함
	prev->xptr = XOR(XOR(prev->xptr, cur), new_node);
	// cur 의 xptr 은 prev ^ next=(cur->xptr^prev) 였었는데,
	// new_node ^ next로 바꿔줘야함. 
	cur->xptr = XOR(new_node, XOR(cur->xptr, prev));
	list->size++;
	return (0);
}

/**
 * @brief XOR 리스트에 새로운 노드를 추가합니다.
 *
 * @param list XOR 리스트 구조체 포인터
 * @param data 추가할 데이터
 * @return 성공 : 0, 실패 : 1, list가 NULL : -1을 반환합니다.
 */
int list_append(t_xorlist* list, int data)
{
	if (list == NULL)
		return (-1);
	t_node* new_node = (t_node*)malloc(sizeof(t_node));
	if (!new_node)
		return (1);
	new_node->xptr = NULL;
	new_node->data = data;

	if (list->head == NULL)
	{
		new_node->xptr = NULL;
		list->head = new_node;
	}
	else
	{
		t_node* cur = list->head;
		t_node* prev = NULL;
		t_node* next;
		int		cnt = 0;
	
		while (cur != NULL && cnt < list->size) 
		{
			next = XOR(prev, cur->xptr);
			prev = cur;
			cur = next;
			cnt++;
		}
		cur = new_node;
		new_node->xptr = XOR(prev, NULL);
		// 원래 prev의 xptr은 prev_prev ^ NULL 이었기 때문에, (prev->xptr^0)^newnode 하면됨
		prev->xptr = XOR(XOR(prev->xptr, NULL), new_node);
	}
	list->size++;
	return (0);
}


/**
 * @brief 리스트에서 지정된 인덱스의 노드를 제거하는 함수입니다.
 *
 * @param list XOR 리스트 구조체 포인터
 * @param index 제거할 노드의 인덱스
 * @return 성공 : 제거된 노드의 데이터 값, 실패 : 1, 매개변수 오류 : -1 을 반환합니다.
 */
int list_pop(t_xorlist* list, int index)
{
	if (list == NULL || index < 0 || index > list->size)
		return (-1);
	t_node* cur = list->head;
	t_node* prev = NULL;
	t_node* next;
	int		cnt = 0;

	while (cur != NULL && cnt < list->size)
	{
		next = XOR(prev, cur->xptr);
		if (cnt == index)
			break ;
		prev = cur;
		cur = next;
		cnt++;
	}
	if (cur == NULL)
		return (-1);
	else
	{
		int ret = cur->data;
		// next의 xptr은 next_next ^ cur
		// next_next ^ prev 로 바꾸어 줘야함
		//t_node* next_next = XOR(next->xptr, cur);
		if (next != NULL) // 마지막 노드를 지울 때는 next_next가 없음
			next->xptr = XOR(XOR(next->xptr, cur), prev);
		// prev의 xptr은 prev_prev ^ cur
		// prev_prev ^ next 로 바꾸어 줘야함
		//t_node* prev_prev = XOR(prev->xptr, cur);
		if (prev == NULL) // 첫 노드를 지울 때는 prev_prev 가 없음
			list->head = next; // prev == NULL 이므로, 위에서 next->xptr = XOR(next_next, NULL) 로 이미 바뀜
		else
			prev->xptr = XOR(XOR(prev->xptr, cur), next);
		memset(cur, 0, sizeof(t_node));
		free(cur);
		list->size--;
		return (ret);
	}
}

/**
 * @brief 리스트에서 특정 값을 제거하는 함수입니다.
 *
 * @param list XOR 리스트 구조체 포인터
 * @param x 제거할 값
 * @return 제거 성공 : 0, 제거할 값이 리스트에 없을 경우 : 1, list가 NULL : -1을 반환합니다.
 */
int list_remove(t_xorlist* list, int x)
{
	if (list == NULL)
		return (-1);
	t_node* cur = list->head;
	t_node* prev = NULL;
	t_node* next;
	int cnt = 0;

	while (cur != NULL && cnt < list->size)
	{
		next = XOR(prev, cur->xptr);
		if (cur->data == x)
			break ;
		prev = cur;
		cur = next;
		cnt++;
	}

	if (cur == NULL)
		return (1);
	else
	{
		// list_pop과 동일한 방법으로 삭제
		if (next != NULL)
			next->xptr = XOR(XOR(next->xptr, cur), prev);
		if (prev == NULL)
			list->head = next;
		else
			prev->xptr = XOR(XOR(prev->xptr, cur), next);
		memset(cur, 0, sizeof(t_node));
		free(cur);
		list->size--;
		return (0);
	}
}

/**
 * @brief XOR 리스트에서 지정된 인덱스에 있는 요소를 검색합니다.
 *
 * @param list XOR 리스트.
 * @param index 검색할 요소의 인덱스입니다.
 * @return 검색 성공 : 인덱스의 데이터 값, 매개변수 오류 : -1을 반환합니다.
 */
int list_get(t_xorlist* list, int index)
{
	if (list == NULL || index < 0 || index > list->size)
		return (-1);
	t_node* cur = list->head;
	t_node* prev = NULL;
	t_node* next;
	int cnt = 0;

	while (cur != NULL && cnt < list->size)
	{
		next = XOR(prev, cur->xptr);
		if (cnt == index)
			break ;
		prev = cur;
		cur = next;
		cnt++;
	}
	if (cur == NULL) 
		return (-1);
	else
		return cur->data;
}

/**
 * @brief XOR 리스트에서 특정 데이터의 인덱스를 찾는 함수입니다.
 * 
 * @param list XOR 리스트 구조체 포인터
 * @param data 찾을 데이터
 * @return 성공 : 데이터의 인덱스, 실패 : -1 을 반환합니다.
 */
int list_index(t_xorlist* list, int data) 
{
	t_node* cur = list->head;
	t_node* prev = NULL;
	t_node* next = NULL;
	int cnt = 0;

	while (cur != NULL && cnt < list->size)
	{
		next = XOR(prev, cur->xptr);
		if (cur->data == data)
			return (cnt);
		prev = cur;
		cur = next;
		cnt++;
	}
	return (-1);
}

/**
 * @brief 리스트가 비어 있는지 확인하는 함수입니다.
 * 
 * @param list 확인할 리스트
 * @return 리스트가 비어 있으면 true, 아니면 false를 반환합니다.
 */
bool list_isEmpty(t_xorlist* list)
{
	return (list->size == 0);
}


/**
 * @brief XOR 리스트의 크기를 반환하는 함수입니다.
 * 
 * @param list XOR 리스트의 포인터
 * @return 성공 : XOR 리스트의 크기, 리스트가 NULL : -1을 반환합니다.
 */
int list_size(t_xorlist* list)
{
	if (list == NULL)
		return (-1);
	else
		return (list->size);
}

/**
 * @brief XOR 리스트의 모든 노드를 제거하는 함수입니다.
 * 
 * @param list XOR 리스트 구조체 포인터
 * @return 성공 : 0, 실패 : -1을 반환합니다.
 */
int list_clear(t_xorlist* list)
{
	if (list == NULL)
		return (-1);
	t_node* cur = list->head;
	t_node* prev = NULL;
	t_node* next = NULL;
	int		cnt = 0;

	while (cnt < list->size)
	{
		if (prev != NULL)
		{
			memset(prev, 0, sizeof(t_node));
			free(prev);
		}
		if (cur == NULL)
			break ;
		next = XOR(prev, cur->xptr);
		prev = cur;
		cur = next;
		cnt++;
	}
	list->size = 0;
	return (0);
}

/**
 * 주어진 XOR 리스트에서 뒤에서 n번째 노드의 값을 출력하는 함수입니다.
 *
 * @param list XOR 리스트의 포인터
 * @param n 뒤에서 n번째 노드의 인덱스
 */
void list_printNthFromLast(t_xorlist* list, int n)
{
	if (n < 0 || n > list->size)
		printf("Index Error!\n");
	else
		printf("%d\n", list_get(list, list->size - n));
}