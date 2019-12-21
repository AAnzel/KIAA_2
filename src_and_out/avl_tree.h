#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <stdio.h>
#include <stdlib.h>

typedef struct cvor
{
	int kljuc;
	unsigned visina;
	unsigned br_potomaka;
	struct cvor *levo;
	struct cvor *desno;
} Cvor;

Cvor *novi_cvor (int);
unsigned visina_stabla (Cvor *);
unsigned max (unsigned, unsigned);
Cvor *LL_rotacija (Cvor *);
Cvor *RR_rotacija (Cvor *);
Cvor *LR_rotacija (Cvor *);
Cvor *RL_rotacija (Cvor *);
short faktor_ravnoteze (Cvor *);
short pretrazi (Cvor *, int);
Cvor *umetanje (Cvor *, int);
Cvor *umetni_cvor (Cvor *,int);
Cvor *minimalni_cvor (Cvor *);
Cvor *brisanje (Cvor *, int);
Cvor *obrisi_cvor (Cvor *, int);
unsigned vrati_rang (Cvor *, int);
Cvor *pronadji_x_k (Cvor *, int, unsigned);
void naredni_x_k (Cvor *, int, unsigned);
void obrisi_stablo (Cvor *);
void in_order (Cvor *);

#endif
