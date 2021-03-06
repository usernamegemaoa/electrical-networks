#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

typedef struct { double rhs; } v_data;
typedef struct { double low, cap, cost, x, rc; } a_data;

#define node(v) ((v_data *)((v)->data))
#define arc(a) ((a_data *)((a)->data))

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		puts("Digite o nome do arquivo a ser processado.");
		exit(1);
	}
	
	glp_graph *G;
	glp_vertex *v, *w;
	glp_arc *a;
	int i, ret;
	double sol;
	
	G = glp_create_graph(sizeof(v_data), sizeof(a_data));
	glp_read_mincost(G, offsetof(v_data, rhs),
	offsetof(a_data, low), offsetof(a_data, cap),
	offsetof(a_data, cost), argv[1]);
	
	ret = glp_mincost_relax4(G, offsetof(v_data, rhs),
	offsetof(a_data, low), offsetof(a_data, cap),
	offsetof(a_data, cost), 0, &sol, offsetof(a_data, x),
	offsetof(a_data, rc));
	
	printf("ret = %d; sol = %5g\n", ret, sol);
/*	
	for (i = 1; i <= G->nv; i++)
	{
		v = G->v[i];
		for (a = v->out; a != NULL; a = a->t_next)
		{
			w = a->head;
			printf("arc %d->%d: x = %5g; rc = %5g\n",
			v->i, w->i, arc(a)->x, arc(a)->rc);
		}
	}
*/
	glp_delete_graph(G);
	return 0;
}
