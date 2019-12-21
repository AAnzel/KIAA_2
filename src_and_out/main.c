#include <stdio.h>
#include <stdlib.h>
#include "avl_tree.h"
#include <time.h>

/* Ove makroe menjati za dobijanje razlicitih rezultata.
	RAZMACI predstavljaju korak od jednog n-a do drugog */

#define BROJ 100
#define RAZMACI 100
#define IZLAZNI_FAJL "izlaz_3.txt"

/* Funkcija za racunanje proteklog vremena */

struct timespec razlika (struct timespec *pocetak, struct timespec *kraj)
{
	struct timespec tmp;
	
	if ((kraj->tv_nsec - pocetak->tv_nsec) < 0)
	{
		tmp.tv_sec = kraj->tv_sec - pocetak->tv_sec-1;
		tmp.tv_nsec = 1000000000 + kraj->tv_nsec - pocetak->tv_nsec;	
	}
	
	else
	{
		tmp.tv_sec = kraj->tv_sec - pocetak->tv_sec;
		tmp.tv_nsec = kraj->tv_nsec - pocetak->tv_nsec;
	}
	
	return tmp;
}

/* Funkcija koja racuna vremena izvrsavanja operacija za svako prosledjeno n (ulaz) i ispisuje u fajl izlaz */

void racunaj (int ulaz, FILE *izlaz)
{
	int i;
	Cvor *r = NULL;
	int *za_umetanje, *za_brisanje;
	struct timespec pocetak, kraj;
	
	double ukupno_vreme = 0;
	
	if ((za_umetanje = (int *) malloc (sizeof(int) * ulaz)) == NULL)
	{
		fprintf (stderr, "Greska_malloc_1");
		exit (EXIT_FAILURE);
	}
	
	if ((za_brisanje = (int *) malloc (sizeof(int) * ulaz)) == NULL)
	{
		fprintf (stderr, "Greska_malloc_2");
		exit (EXIT_FAILURE);
	}
	
	srand(time(NULL));
	
	/* Generisanje proizvoljnih elemenata za umetanje i brisanje */
	
	for(i=0; i<ulaz; i++)
	{	
		za_umetanje[i] = rand();
		za_brisanje[i] = rand();
	}
	
	/* n puta radimo umetanja */
	
	for(i=0; i<ulaz; i++)
	{
		if (clock_gettime (CLOCK_MONOTONIC, &pocetak) < 0)
		{
			fprintf (stderr, "Greska_11\n");
			exit (EXIT_FAILURE);
		}
		
		r = umetni_cvor(r,za_umetanje[i]);
		
		if (clock_gettime (CLOCK_MONOTONIC, &kraj) < 0)
		{
			fprintf (stderr, "Greska_21\n");
			exit (EXIT_FAILURE);
		}
		
		ukupno_vreme += (((double) razlika(&pocetak, &kraj).tv_sec)*1000000000 + (double) razlika(&pocetak, &kraj).tv_nsec);
	}
	
	/* n/2 puta radimo brisanja */
	
	for(i=0; i<ulaz/2; i++)
	{
		if (clock_gettime (CLOCK_MONOTONIC, &pocetak) < 0)
		{
			fprintf (stderr, "Greska_12\n");
			exit (EXIT_FAILURE);
		}
		
		r = obrisi_cvor(r, za_brisanje[i]);
		
		if (clock_gettime (CLOCK_MONOTONIC, &kraj) < 0)
		{
			fprintf (stderr, "Greska_22\n");
			exit (EXIT_FAILURE);
		}
		
		ukupno_vreme += (((double) razlika(&pocetak, &kraj).tv_sec)*1000000000 + (double) razlika(&pocetak, &kraj).tv_nsec);
	}
	
	/* n/3 puta radimo trazenje naredni_x_k gde je x neki od elemenata za umetanje a k je dobijen formulom: za_brisanje[i]/20, proizvoljno odabrano */
	
	for (i=0; i<ulaz/3; i++)
	{
		if (clock_gettime (CLOCK_MONOTONIC, &pocetak) < 0)
		{
			fprintf (stderr, "Greska_13\n");
			exit (EXIT_FAILURE);
		}
		
		naredni_x_k (r, za_umetanje[i], za_brisanje[i]/20);
		
		if (clock_gettime (CLOCK_MONOTONIC, &kraj) < 0)
		{
			fprintf (stderr, "Greska_23\n");
			exit (EXIT_FAILURE);
		}
		
		ukupno_vreme += (((double) razlika(&pocetak, &kraj).tv_sec)*1000000000 + (double) razlika(&pocetak, &kraj).tv_nsec);
	}
	
	/* Aritmeticku sredinu svih vremena upisujemo u dati izlazni fajl */
	
	fprintf (izlaz, "%d %lf\n", ulaz, ukupno_vreme/(ulaz + ulaz/2 + ulaz/3));
	
	obrisi_stablo (r);
	free (za_umetanje);
	free (za_brisanje);
}

int main()
{
	FILE *izlaz;
	int ulazi[BROJ], i;
	
	ulazi[0] = 100;
	
	for (i=1; i<BROJ; i++)
		ulazi[i] = RAZMACI + ulazi[i-1];
	
	if((izlaz = fopen(IZLAZNI_FAJL,"w")) == NULL)
	{
		fprintf(stderr,"Greska1: fopen(), mode w\n");
		exit(EXIT_FAILURE);
	}
	
	for (i=0; i<BROJ; i++)
		racunaj (ulazi[i], izlaz);
	
	fclose(izlaz);
	
	return 0;
}
