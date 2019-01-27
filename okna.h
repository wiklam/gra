#include<stdio.h>
#include<stdbool.h>
#include<gtk/gtk.h>
#include"plansza.h"
typedef struct dane{
	bool ruchgracza,gracz;
	int n,odkryte,punktyA,punktyB,ruchy;
	Przycisk przyc[8][8];
	Przycisk *ktoreodkryl[2];
	GtkWidget *mv,*pas,*pbs;
} Dane;

void pokaz_instrukcje(GtkWidget *widget, gpointer *data);
void zamknij(GtkWidget *widget);
void zakoncz();
