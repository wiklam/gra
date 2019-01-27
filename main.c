#include"okna.h"

static GtkWidget *window,*window2;
static Dane info;
bool aktywna;
void zakoncz2(){
	aktywna = 0;
}
void nowa_gra(){
}
void klikniete(){
}
void wczytaj_gre(){
}
void poddaj_sie(){
}
void stworz_plansze(GtkWidget *widget, gpointer *data);

int main(int argc,char *argv[]){
	gtk_init(&argc,&argv);

	if(argv[1][0] == 'B')
		info.gracz=1;
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

void stworz_plansze(GtkWidget *widget, gpointer *data){
	if(aktywna){
		printf("POP UP, ze tylko jedna gra moze byc naraz uruchomiona");
		return;
	}
	info.ruchy=1;
	aktywna = 1;
	gchar naglowek[14];
	info.n = *((int*)data);
	gen(info.n,info.przyc,-1);
	if(info.gracz)
		sprintf(naglowek,"Memory %dx%d - B",info.n,info.n);
	else
		sprintf(naglowek,"Memory %dx%d - A",info.n,info.n);

	window2 =gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *image,*button;

	gtk_window_set_title(GTK_WINDOW(window2), naglowek);
	gtk_window_set_position	(GTK_WINDOW(window2),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width( GTK_CONTAINER(window2), 10);

	g_signal_connect(G_OBJECT(window2),"destroy",G_CALLBACK(zakoncz2), &info);

	GtkWidget *box0 =gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_container_add(GTK_CONTAINER(window2), box0);

	GtkWidget *box1 =gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(box0), box1);

	GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,2);
	gtk_box_pack_start(GTK_BOX(box1), box2, TRUE, TRUE, 0);
	info.mv = gtk_label_new("Move:\nPlayer A");
	info.pas = gtk_label_new("Player A Score:\n0");
	info.pbs = gtk_label_new("Player B Score:\n0");
	gtk_box_pack_start(GTK_BOX(box2), info.mv, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box2), info.pas, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box2), info.pbs, TRUE, TRUE, 0);

	GtkWidget *grid = gtk_grid_new();
	gtk_grid_set_row_spacing(GTK_GRID(grid), 0);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 0);
	gtk_grid_set_column_homogeneous	(GTK_GRID(grid), TRUE);
	gtk_box_pack_start(GTK_BOX(box1), grid, TRUE, TRUE, 0);

	GtkWidget *box3 =gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *ng = gtk_button_new_with_label("Nowa gra");
	g_signal_connect(G_OBJECT(ng),"clicked",G_CALLBACK(nowa_gra),&info);
	GtkWidget *ps = gtk_button_new_with_label("Poddaj sie");
	g_signal_connect(G_OBJECT(ps),"clicked",G_CALLBACK(poddaj_sie),&info);
	GtkWidget *wg = gtk_button_new_with_label("Wczytaj gre");
	g_signal_connect(G_OBJECT(wg),"clicked",G_CALLBACK(wczytaj_gre),&info);

	gtk_box_pack_start(GTK_BOX(box3), ng, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box3), ps, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box3), wg, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box0), box3, TRUE, TRUE, 0);

	for(int g=0;g<info.n;g++)
		for(int h=0;h<info.n;h++){
			button = gtk_button_new_with_label(NULL);
			image = gtk_image_new_from_file("obrazki/tlo.jpeg");
			info.przyc[g][h].guz=button;
			info.przyc[g][h].posX=g;
			info.przyc[g][h].posY=h;
			info.przyc[g][h].odkryty=0;
			g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(klikniete),&info.przyc[g][h]);
			gtk_grid_attach(GTK_GRID(grid),button,g,h,1,1);
			gtk_button_set_image(GTK_BUTTON(button),image);
		}
	gtk_widget_show_all(window2);
	return;
}
