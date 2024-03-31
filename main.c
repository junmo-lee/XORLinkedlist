#include "xorlist.h"

void list_printList(t_xorlist* list) 
{
	t_node* curr = list->head;
	t_node* prev = NULL;
	t_node* next;
	int idx = 0;
	printf("XOR LinkeList, size : %d\n", list->size);
	while (curr != NULL) {
		next = XOR(prev, curr->xptr);
		printf("idx  : %11d, data : %11d, xptr : %11p\n", idx++, curr->data, curr->xptr);
		printf("prev : %11p, cur  : %11p, next : %11p\n", prev, curr, next);
		prev = curr;
		curr = next;
	}
	printf("\n");
}

int main(void)
{
	t_xorlist *arr = malloc(sizeof(t_xorlist));
	if (!arr)
		return (0);
	arr->head = NULL;
	arr->size = 0;

	printf("\n---test append---\n");
	printf("append 1\n");list_append(arr, 1);
	printf("append 222\n");list_append(arr, 222);
	printf("append 333\n");list_append(arr, 333);
	printf("append 444\n");list_append(arr, 444);
	list_printList(arr);

	printf("\n---test insert---\n");
	printf("insert 321 at 2\n");list_insert(arr, 321, 2);
	printf("insert 777 at %d\n", arr->size);list_insert(arr, 777, arr->size);
	list_printList(arr);

	printf("\n---test get---\n");
	for(int i = 0; i < arr->size; i++)
		printf("%d index : %d\n", i, list_get(arr, i));

	int testcase[3] = {0, 321, 1};
	printf("\n---test index---\n");
	for(int i = 0; i < sizeof(testcase) / sizeof(int); i++)
		printf("%d is %d index\n", testcase[i], list_index(arr, testcase[i]));

	printf("\n---test printNthFromLast---\n");
	for(int i = 1; i <= arr->size; i++)
	{
		printf("%d : ", i);list_printNthFromLast(arr, i);
	}

	printf("\n---test remove---\n");
	printf("remove 222\n");list_remove(arr, 222);
	list_printList(arr);
	printf("remove 444\n");list_remove(arr, 444);
	list_printList(arr);

	printf("\n---test pop---\n");
	printf("pop 1, ret : %d\n",list_pop(arr, 1));
	list_printList(arr);
	printf("pop 2, ret : %d\n",list_pop(arr, 2));
	list_printList(arr);

	t_xorlist* emptylist = malloc(sizeof(t_xorlist));
	emptylist->head = NULL;
	emptylist->size = 0;
	printf("\n---test isEmpty---\n");
	printf("arr : %d\n",list_isEmpty(arr));
	printf("emptylist : %d\n",list_isEmpty(emptylist));

	list_clear(arr);
	free(arr);
	list_clear(emptylist);
	free(emptylist);

#ifdef __WIN32
	_CrtDumpMemoryLeaks();
#endif
#ifdef __APPLE__
	system("leaks --list -- main");
#endif
	return 0;
}
