#include"okna.h"

void pokaz_instrukcje(GtkWidget *widget, gpointer *data){
	GtkWidget *window =gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"instrukcja");
	gtk_window_set_position	(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width( GTK_CONTAINER(window), 10);
	gtk_window_set_default_size(GTK_WINDOW(window),400,600);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(zamknij), window);
	gtk_widget_show_all(window);
	return;
}
void zamknij(GtkWidget *widget, gpointer *data){
	GtkWidget *zm = (GtkWidget*) data;
	gtk_widget_destroy(GTK_WIDGET(zm));
	return;
}
