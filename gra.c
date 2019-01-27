#include"gra.h"

#define CZAS_ODKRYCIA 500
#define MAKS_DL_TEKSTU 10000

void nowa_gra(GtkWidget *widget, gpointer *data){
	Dane *wsk = (Dane *) data;
	bool czywszystkozakryte=0;
	for(int g=0;g<wsk->n;g++)
		for(int h=0;h<wsk->n;h++){
			if((wsk->przyc[g][h]).odkryty==0){
				czywszystkozakryte=1;
			}
		}
	if(czywszystkozakryte){
		//WYSLIJ KOMUNIKAT ZE NAJPIERW NALEZY DOKONCZYC AKTUALNA ROZGRYWKE LUB SIE PODDAC
		return;
	}
	wsk->punktyA=wsk->punktyB=wsk->odkryte=0;
	wsk->ruchy=0;
	gtk_label_set_text(GTK_LABEL(wsk->pas),"Player A Score:\n0");
	gtk_label_set_text(GTK_LABEL(wsk->pbs),"Player B Score:\n0");
	for(int g=0;g<wsk->n;g++)
		for(int h=0;h<wsk->n;h++){
			g_timeout_add(1,zakryj,&wsk->przyc[g][h]);
			(wsk->przyc[g][h]).odkryty=0;
		}
		gen(wsk->n,wsk->przyc,-1);
	for(int g=0;g<wsk->n;g++){
		for(int h=0;h<wsk->n;h++){
			printf("%d ",(wsk->przyc[h][g]).obraz);
		}
		printf("\n");
	}
	//przekaz_tekst(0,*wsk);
		return;
}
/*
void przekaz_tekst(bool zak,Dane zm){
	char tekst[MAKS_DL_TEKSTU+5];
	if(zak==1){
		sprintf(tekst,"z%d%d%d%d",(zm.ktoreodkryl[0])->posX,(zm.ktoreodkryl[0])->posY,(zm.ktoreodkryl[1])->posX,(zm.ktoreodkryl[1])->posY);
		sprintf(tekst,"%s%d %d %d %d %d",tekst,zm.ruchy,zm.n,zm.punktyA,zm.punktyB,zm.ruchgracza);
	}
	else
		sprintf(tekst,"%d %d %d %d %d",zm.ruchy,zm.n,zm.punktyA,zm.punktyB,zm.ruchgracza);
	for(int g=0;g<zm.n;g++)
		for(int h=0;h<zm.n;h++)
			sprintf(tekst,"%s %d",tekst,(zm.przyc[g][h]).obraz);
	for(int g=0;g<zm.n;g++)
		for(int h=0;h<zm.n;h++)
			sprintf(tekst,"%s %d",tekst,(zm.przyc[g][h]).odkryty);
	sendStringToPipe(zm.potoki, tekst);
	return;
}

gboolean pobierz_tekst(gpointer data){
	Dane *zm = (Dane*) data;
	int zliczaj=0;
	char wejscie[MAKS_DL_TEKSTU+5];															//chyba 342 jest maksymalne
	int RUCHY=0,N=0,PUNKTYA=0,PUNKTYB=0,RUCHGRACZA=0,licz=0;
	int xx=-1,yy=-1,x2=-1,y2=-1;
	if(getStringFromPipe(zm->potoki,wejscie,MAKS_DL_TEKSTU)) {
		if(wejscie[0]=='z'){																//jesli przeciwnik odkryl niedobrana pare to po sprawdzeniu pewnych warunkow zostana one zakryte
			xx=wejscie[1]-'0',yy=wejscie[2]-'0',x2=wejscie[3]-'0',y2=wejscie[4]-'0';
			licz=5;
		}																					//z pliku odczytuje informacje o rozgrywce przeciwnika
		while(wejscie[licz]!=' '){
			RUCHY=RUCHY*10+wejscie[licz]-'0';
			licz++;
		}
		licz++;
		N=wejscie[licz]-'0';
		licz+=2;
		while(wejscie[licz]!=' '){
			PUNKTYA=PUNKTYA*10+wejscie[licz]-'0';
			licz++;
		}
		licz++;
		while(wejscie[licz]!=' '){
			PUNKTYB=PUNKTYB*10+wejscie[licz]-'0';
			licz++;
		}
		licz++;
		RUCHGRACZA=wejscie[licz]-'0';
		licz++;
		if(N!=zm->n){																			//jesli sa uruchomione naraz rozgrywki dla dwoch roznych plansz zostaja one wylaczone
			printf("Uruchomiono dwie rozne rozgrywki");
			przekaz_tekst(0,*zm);
			zakoncz();
		}
		if(zm->ruchy>RUCHY && RUCHY!=0)															//jesli gracz, ktory wyslal informacje ma mniej ruchow od drugiego gracza uaktualnia informacje zgodne z drugim grazcem (nie liczac rozpoczecia nowej rozgrywki lub wznowienia starej dlatego rozna od zera)
			przekaz_tekst(0,*zm);
		else{
			if(zm->ruchy==0 && RUCHY==1)														//jak wznawiam aby plansza ktora wznawiam nie zakryla sie cala
				return TRUE;
			for(int g=0;g<N;g++)
				for(int h=0;h<N;h++){
					(zm->przyc[g][h]).obraz=0;
					while(wejscie[licz]==' ')
						licz++;
					while(wejscie[licz]!=' '){
						(zm->przyc[g][h]).obraz=(zm->przyc[g][h]).obraz*10+wejscie[licz]-'0';
						licz++;
					}
				}
			for(int g=0;g<N;g++)																//uaktualniam plansze graczy
				for(int h=0;h<N;h++){
					licz++;
					(zm->przyc[g][h]).odkryty=wejscie[licz]-'0';
					if((zm->przyc[g][h]).odkryty || (xx==g && yy==h) || (x2==g && y2==h)){
						odkryj((zm->przyc[g][h]));
						zliczaj++;
					}
					else
						g_timeout_add(1,zakryj,&(zm->przyc[g][h]));
					licz++;
				}
			for(int g=0;g<zm->n;g++){
				for(int h=0;h<zm->n;h++){
					printf("%d ",(zm->przyc[g][h]).obraz);
				}
				printf("\n");
			}
			zm->ruchy=RUCHY;																		//ukatualniam dane
			zm->punktyA=PUNKTYA;
			zm->punktyB=PUNKTYB;
			zm->ruchgracza=RUCHGRACZA;
			char sl[19];
			sprintf(sl,"Player A Score:\n%d",zm->punktyA);
			gtk_label_set_text(GTK_LABEL(zm->pas),sl);
			sprintf(sl,"Player B Score:\n%d",zm->punktyB);
			gtk_label_set_text(GTK_LABEL(zm->pbs),sl);
			if(zm->ruchgracza==0)
				gtk_label_set_text(GTK_LABEL(zm->mv),"Move:\nPlayer A");
			else
				gtk_label_set_text(GTK_LABEL(zm->mv),"Move:\nPlayer B");
			if(wejscie[0]=='z'){										//zakrywam po jakims czasie zla pare kart, ktora odkryl przeciwnik
				g_timeout_add(CZAS_ODKRYCIA,zakryj,&zm->przyc[xx][yy]);
				g_timeout_add(CZAS_ODKRYCIA,zakryj,&zm->przyc[x2][y2]);
			}
			if(zliczaj%2==1 && zm->gracz==zm->ruchgracza){
				zm->odkryte=1;
				int miejsce=bezpary(zm->przyc,zm->n);
				zm->ktoreodkryl[0]=&zm->przyc[miejsce%zm->n][miejsce/zm->n];
				printf("\n znalezione: %d %d\n",(zm->ktoreodkryl[0])->posX,(zm->ktoreodkryl[0])->posY);
			}
		}
		printf("meh %d %d %d %d %d %c\n",RUCHY,N,PUNKTYA,PUNKTYB,RUCHGRACZA,wejscie[0]);
	}
	return TRUE;
}
*/
