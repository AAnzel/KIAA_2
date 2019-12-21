#include "avl_tree.h"

/* Funkcija koja kreira novi cvor sa zadatim kljucem i praznim levim i desnim podstablom */
Cvor* novi_cvor (int kljuc) 
{ 
	Cvor* cvor = (Cvor*) malloc (sizeof(Cvor));
	
	if (cvor == NULL)
	{
		fprintf(stderr,"Greska pri alokaciji novog cvora!\n");
		exit(EXIT_FAILURE);
	}
	
	cvor->kljuc = kljuc;
	cvor->levo = NULL;
	cvor->desno = NULL;
	
	/* Inicijalno, visina novog cvora je 1 */
	cvor->visina = 1;
	
	/* Incijalno, broj potomaka je 1 */
	cvor->br_potomaka = 1;
	
	return(cvor);
}

/* Pomocna funkcija za vracanje visine stabla */
unsigned visina_stabla (Cvor *r)
{ 
	if (r == NULL)
		return 0;
	
	return r->visina;
}

/* Pomocna funkcija za vracanje maksimuma dva broja */
unsigned max (unsigned a, unsigned b)
{ 
	return (a > b) ? a : b;
}

/* Funkcija koja vrsi LL rotaciju podstabla sa korenom r */
Cvor *LL_rotacija (Cvor *r)
{ 
	Cvor *x = r->levo;
	Cvor *tmp = x->desno;
	
	unsigned x_levo_br = 0;
	unsigned r_desno_br = 0;
	
	if(x->levo != NULL)
		x_levo_br = x->levo->br_potomaka;
	
	if(r->desno != NULL)
		r_desno_br = r->desno->br_potomaka;
	
	/* Azuriranje broja potomaka */
	r->br_potomaka -= 1 + x_levo_br;
	x->br_potomaka += r_desno_br + 1;
	
	/* Rotiranje */ 
	x->desno = r;
	r->levo = tmp;

	/* Azuriranje visina */
	r->visina = max(visina_stabla(r->levo), visina_stabla(r->desno))+1;
	x->visina = max(visina_stabla(x->levo), visina_stabla(x->desno))+1;
	
	/* Vracanje novog korena */
	return x; 
}

/* Funkcija koja vrsi RR rotaciju podstabla sa korenom r */
Cvor *RR_rotacija (Cvor *r)
{ 
	Cvor *x = r->desno;
	Cvor *tmp = x->levo;
	
	int x_desno_br = 0;
	int r_levo_br = 0;
	
	if(x->desno != NULL)
		x_desno_br = x->desno->br_potomaka;
	
	if(r->levo != NULL)
		r_levo_br = r->levo->br_potomaka;
	
	/* Azuriranje broja potomaka */
	r->br_potomaka -= 1 + x_desno_br;
	x->br_potomaka += r_levo_br + 1;
	
	/* Rotiranje */
	x->levo = r;
	r->desno = tmp;
	
	/* Azuriranje visina */
	r->visina = max(visina_stabla(r->levo), visina_stabla(r->desno))+1;
	x->visina = max(visina_stabla(x->levo), visina_stabla(x->desno))+1;
	
	/* Vracanje novog korena */
	return x;
}

/* LR rotacija */
Cvor *LR_rotacija (Cvor *r)
{
	r->levo = RR_rotacija(r->levo);
	return LL_rotacija(r);
}

/* RL rotacija */
Cvor *RL_rotacija (Cvor *r)
{
	r->desno = LL_rotacija(r->desno);
	return RR_rotacija(r);
}

/* Funkcija vraca faktor ravnoteze cvora r */
short faktor_ravnoteze (Cvor *r)
{
	if (r == NULL)
		return 0;
	
	return visina_stabla(r->levo) - visina_stabla(r->desno);
}

/* Funkcija pretrazuje stablo, vraca 1 ako postoji cvor sa zadatim kljucem, 0 u suprotnom */
short pretrazi (Cvor *r, int kljuc)
{
	if (r == NULL)
		return 0;
	
	if (kljuc < r->kljuc)
	{
		return pretrazi(r->levo, kljuc);
	}
	
	else if (kljuc > r->kljuc)
	{
		return pretrazi(r->desno, kljuc);
	}
	
	else
		return 1;
}

/* Funkcija vrsi umetanje cvora sa zadatim kljucem
   NAPOMENA: ispisivanja su zakomentarisana za potrebe merenja vremena */

Cvor* umetanje (Cvor *r, int kljuc)
{
	if (r == NULL)
		return(novi_cvor(kljuc));
	
	if (kljuc < r->kljuc)
	{
		r->levo = umetanje(r->levo, kljuc);
	}
	
	else if (kljuc > r->kljuc)
	{
		r->desno = umetanje(r->desno, kljuc);
	}
	
	/* Azuriranje visine */
	r->visina = 1 + max(visina_stabla(r->levo),visina_stabla(r->desno));
	
	/* Azuriranje broja potomaka */
	r->br_potomaka ++;
	
	/* Provera faktora ravnoteze */
	short ravnoteza = faktor_ravnoteze(r);
	
	/* Ako je cvor postao kritican, potrebno je uravnoteziti stablo */
	
	/* LL rotacija */
	if (ravnoteza > 1 && faktor_ravnoteze(r->levo) >= 0)
	{
		/* printf("Kritican cvor %d --> LL rotacija.\n",r->kljuc); */
		return LL_rotacija(r);
	}
	
	/* LR rotacija */
	if (ravnoteza > 1 && faktor_ravnoteze(r->levo) < 0)
	{
		/* printf("Kritican cvor %d --> LR rotacija.\n",r->kljuc); */
		return LR_rotacija(r);
	}
	
	/* RR rotacija */
	if (ravnoteza < -1 && faktor_ravnoteze(r->desno) <= 0)
	{
		/* printf("Kritican cvor %d --> RR rotacija.\n",r->kljuc); */
		return RR_rotacija(r);
	}
	
	/* RL rotacija */
	if (ravnoteza < -1 && faktor_ravnoteze(r->desno) > 0)
	{
		/* printf("Kritican cvor %d --> RL rotacija.\n",r->kljuc); */
		return RL_rotacija(r);
	}
	
	return r;
}

Cvor *umetni_cvor (Cvor *r, int kljuc)
{
	if (pretrazi(r,kljuc) == 1)
		return r;
	else
		return umetanje(r,kljuc);
}

/* Funkcija vraca cvor sa najmanjim kljucem u stablu sa korenom r */
Cvor *minimalni_cvor (Cvor *r)
{
	Cvor *tekuci = r;
	
	/* Spustanje do najlevljeg cvora */
	while (tekuci->levo != NULL)
		tekuci = tekuci->levo;
	
	return tekuci;
}

/* Funkcija brise cvor sa trazenim kljucem iz podstabla sa korenom r i vraca koren izmenjenog stabla 
   NAPOMENA: ispisivanja su zakomentarisana za potrebe merenja vremena */
Cvor *brisanje (Cvor *r, int kljuc)
{
	if (r == NULL)
		return r;
	
	/* Ako je trazeni kljuc manji od r->kljuc, treba pretraziti levo podstablo */
	if ( kljuc < r->kljuc )
		r->levo = brisanje(r->levo, kljuc);
	
	/* Ako je trazeni kljuc veci od r->kljuc, treba pretraziti desno podstablo */
	else if( kljuc > r->kljuc )
		r->desno = brisanje(r->desno, kljuc);
	
	else
	{
		/* Ako brisemo list */
		if (r->levo == NULL && r->desno == NULL)
		{
			free (r);
			return NULL;
		}
		
		/* Ako brisemo cvor koji ima samo desno podstablo */
		if (r->levo == NULL && r->desno != NULL)
		{
			Cvor *tmp = r->desno;
			free (r);
			return tmp;
		}
		
		/* Ako brisemo cvor koji ima samo levo podstablo */
		if (r->levo != NULL && r->desno == NULL)
		{
			Cvor *tmp = r->levo;
			free (r);
			return tmp;
		}
		
		/* Ako cvor ima dva sina, trazi se njegov sledbenik (najmanji u desnom podstablu) */
		Cvor* tmp = minimalni_cvor(r->desno);
		
		r->kljuc = tmp->kljuc;
		
		r->desno = brisanje(r->desno, tmp->kljuc);
	}
	
	if (r == NULL)
		return r;
	
	/* Azuriranje visine tekuceg cvora */
	r->visina = 1 + max(visina_stabla(r->levo),visina_stabla(r->desno));
	
	/* Azuriranje broja potomaka */
	r->br_potomaka --;
	
	/* Racunanje faktora ravnoteze */
	short ravnoteza = faktor_ravnoteze(r);
	
	/* Ako je cvor postao kritican, potrebno je uravnoteziti */
	
	/* LL rotacija */ 
	if (ravnoteza > 1 && faktor_ravnoteze(r->levo) >= 0)
	{
		/* printf("Kritican cvor %d --> LL rotacija.\n",r->kljuc); */
		return LL_rotacija(r);
	}
	
	/* LR rotacija */
	if (ravnoteza > 1 && faktor_ravnoteze(r->levo) < 0)
	{
		/* printf("Kritican cvor %d --> LR rotacija.\n",r->kljuc); */
		return LR_rotacija(r);
	}
	
	/* RR rotacija */
	if (ravnoteza < -1 && faktor_ravnoteze(r->desno) <= 0)
	{
		/* printf("Kritican cvor %d --> RR rotacija.\n",r->kljuc); */
		return RR_rotacija(r);
	}
	
	/* RL rotacija */
	if (ravnoteza < -1 && faktor_ravnoteze(r->desno) > 0)
	{
		/* printf("Kritican cvor %d --> RL rotacija.\n",r->kljuc); */
		return RL_rotacija(r);
	}
	
	return r;
}

Cvor *obrisi_cvor (Cvor *r, int kljuc)
{
	if (pretrazi(r,kljuc) == 0)
		return r;
	else
		return brisanje(r,kljuc);
}

/* Funkcija pronalazi rang cvora sa zadatim kljucem (broj cvorova ciji je kljuc manji ili jednak od zadatog kljuca) */
unsigned vrati_rang (Cvor *r, int kljuc)
{
	unsigned rang = 1; 	/* Broj cvorova sa kljucem manjim ili jednakim kljuca korena */
	
	/* Ako koren ima levog sina... */
	if (r->levo != NULL)
		rang += r->levo->br_potomaka;
		
	while (r->kljuc != kljuc)
	{
		if(r->kljuc > kljuc) /* trazeni element je u levom podstablu, racunamo rang levog sina */
		{
			if (r->levo->desno != NULL)
				rang -= 1 + r->levo->desno->br_potomaka;
			else
				rang -=1;
			
			r = r->levo;
		}
		
		else if (r->kljuc < kljuc)
		{
			if (r->desno->levo != NULL)
				rang += 1 + r->desno->levo->br_potomaka;
			else
				rang += 1;
			
			r = r->desno;
		}
	}
	
	return rang;
}

Cvor *pronadji_x_k (Cvor *r, int x, unsigned k)
{
	unsigned trazeni_rang = vrati_rang(r,x) + k;
	
	unsigned rang = 1; 	/* Broj cvorova sa kljucem manjim ili jednakim kljuca korena */
	
	/* Ako koren ima levog sina... */
	if (r->levo != NULL)
		rang += r->levo->br_potomaka;
	
	while (trazeni_rang != rang)
	{	
		if(trazeni_rang < rang)
		{	
			if (r->levo == NULL)
				return NULL;
			
			if (r->levo->desno != NULL)
					rang -= 1 + r->levo->desno->br_potomaka;
			else
				rang -=1;
				
			r = r->levo;
		}
		
		else
		{	
			if (r->desno == NULL)
				return NULL;
			
			if (r->desno->levo != NULL)
				rang += 1 + r->desno->levo->br_potomaka;
			else
				rang += 1;
			
			r = r->desno;
		}
	}
	
	return r;
}

void naredni_x_k (Cvor *r, int x, unsigned k)
{
	if (pretrazi(r,x) == 0)
	{
		/* printf("Cvor sa kljucem %d nije pronadjen u stablu.\n",x); */
		return;
	}
	
	r = pronadji_x_k (r, x, k);
	
	if (r == NULL)
	{
		/* printf("Trazeni rang je van opsega.\n"); */
		return;
	}
	
	else
	{
		/* printf("Trazeni kljuc je %d.\n",r->kljuc); */
		return;
	}
}

/* Funkcija brise stablo sa korenom r */
void obrisi_stablo (Cvor *r)
{
	if(r == NULL)
		return;
	
	obrisi_stablo (r->levo);
	obrisi_stablo (r->desno);
	free(r);
}

/* In-order ispis stabla, koristi se samo za potrebe debagovanja */
void in_order(Cvor *r)
{
	if(r != NULL)
	{
		in_order(r->levo);
		printf("%d(%d) ", r->kljuc,r->br_potomaka);
		in_order(r->desno);
	}
}
