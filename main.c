#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<stdbool.h>
#include<gtk/gtk.h>
int gracz;
GtkWidget *window;
void zakoncz(){
	gtk_main_quit();
	return;
}
void stworz_plansze(){

}
void pokaz_instrukcje(){

}
int main(int argc,char *argv[]){
	gtk_init(&argc,&argv);

	if(argv[1][0] == 'B')
		gracz=1;
	gchar naglowek[10];
	sprintf(naglowek,"Player %c",argv[1][0]);
	window =gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW(window),naglowek);
	gtk_window_set_position	(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width( GTK_CONTAINER(window), 10);
	gtk_window_set_default_size(GTK_WINDOW(window),400,600);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(zakoncz),NULL);

	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(window), box);
	GtkWidget *pt = gtk_label_new("Memory");
	GtkWidget *ee = gtk_button_new_with_label("Poziom:\nEasy - 4 x 4");
	GtkWidget *mm = gtk_button_new_with_label("Poziom:\nMedium - 6 x 6");
	GtkWidget *hh = gtk_button_new_with_label("Poziom:\nHard - 8 x 8");
	GtkWidget *ii = gtk_button_new_with_label("Instrukcja");
	int e=2,m=4,h=6;
	g_signal_connect(G_OBJECT(ee),"clicked",G_CALLBACK(stworz_plansze),&e);
	g_signal_connect(G_OBJECT(mm),"clicked",G_CALLBACK(stworz_plansze),&m);
	g_signal_connect(G_OBJECT(hh),"clicked",G_CALLBACK(stworz_plansze),&h);
	g_signal_connect(G_OBJECT(ii),"clicked",G_CALLBACK(pokaz_instrukcje),NULL);

	gtk_box_pack_start(GTK_BOX(box), pt, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), ee, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), mm, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), hh, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), ii, TRUE, TRUE, 0);

	gtk_widget_show_all(window);

	gtk_main();
	return 0;
}
