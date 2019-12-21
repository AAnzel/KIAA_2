#include "avl_tree.h"

int main() 
{ 
	Cvor *r = NULL; 
	int n, i, kljuc;
	
	/* Jedan od primera sa vezbi:
		n = 8
		cvorovi: 20, 9, 15, 8, 5, 12, 11, 10	
	*/
	
	printf("Unesite broj cvorova: ");
	scanf("%d", &n);
	
	printf("Unosite kljuceve:\n");
	
	for(i=0; i<n; i++)
	{
		scanf("%d", &kljuc);
		r = umetni_cvor (r, kljuc);
	}
	
	printf("Dobijeno stablo 'cvor(br_potomaka)':\n");
	in_order(r);

	printf("\nCvor za brisanje: ");
	scanf("%d", &kljuc);
	
	r = obrisi_cvor (r, kljuc);
	
	printf("Stablo nakon brisanja 'cvor(br_potomaka)':\n");
	in_order(r);
	
	
	printf("\nUnesite elemente za funkciju Naredni(x,k): ");
	scanf("%d%d", &kljuc, &i);
	
	naredni_x_k (r, kljuc, i);
	
	obrisi_stablo (r);
	
	return 0; 
} 
