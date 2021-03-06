#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>

/* dica de execução para filtrar arcos com correntes não nulas
 * 
 * ./okalg ../inputs/book_50_arc.min | grep -v "arc.*->.*: x =     0.*" | grep "arc"
 * 
 */

typedef struct { double rhs, pi; } v_data;
typedef struct { double low, cap, cost, x; } a_data;

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
	ret = glp_mincost_okalg(G, offsetof(v_data, rhs),
		offsetof(a_data, low), offsetof(a_data, cap),
		offsetof(a_data, cost), &sol, offsetof(a_data, x),
		offsetof(v_data, pi));
	printf("ret = %d; sol = %5g\n", ret, sol);

	for (i = 1; i <= G->nv; i++)
	{ 
		v = G->v[i];
		printf("node %d: pi = %5g\n", i, node(v)->pi);
	  for (a = v->out; a != NULL; a = a->t_next)
		{
			w = a->head;
			printf("arc %d->%d: x = %5g; lambda = %5g\n",
				v->i, w->i, arc(a)->x,
				arc(a)->cost - (node(v)->pi - node(w)->pi));
		}
	}

	glp_delete_graph(G);
	return 0;
}
