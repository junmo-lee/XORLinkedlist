#ifndef XORLIST_H
# define XORLIST_H

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include <stdbool.h>

# ifdef _WIN32
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
# endif

typedef struct s_node
{
	int				data;
	struct s_node*	xptr; // prev node ptr ^ next node ptr
} t_node;

typedef struct s_xorlist
{
	t_node	*head;
	int		size;
}	t_xorlist;

t_node*	XOR(t_node* a, t_node* b);
int		list_insert(t_xorlist* list, int data, int index);
int		list_append(t_xorlist* list, int data);
int		list_pop(t_xorlist* list, int index);
int		list_remove(t_xorlist* list, int x);
int		list_get(t_xorlist* list, int index);
int		list_index(t_xorlist* list, int data);
bool	list_isEmpty(t_xorlist* list);
int		list_size(t_xorlist* list);
int		list_clear(t_xorlist* list);
void	list_printNthFromLast(t_xorlist* list, int n);

#endif