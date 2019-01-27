#include"okna.h"

#define CZAS_ODKRYCIA 500
#define MAKS_DL_TEKSTU 1000
static Dane info;
bool aktywna;
static GtkWidget *window;

void pokazBlad(char *komunikat){
    GtkWidget *dialog;
    dialog=gtk_message_dialog_new (GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",komunikat);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}
void zakoncz(){ closePipes(info.potoki); gtk_main_quit(); return;}
void zakoncz2(){
	FILE *fptr;
	char tekst[MAKS_DL_TEKSTU+5];
	char sl[11];
	sprintf(sl,"zapis%d.txt",info.n);
	printf("%d",info.n);;
	fptr = fopen(sl,"w");
	sprintf(tekst,"%d %d %d %d %d",info.ruchy,info.n,info.punktyA,info.punktyB,info.ruchgracza);
	for(int g=0;g<info.n;g++)
		for(int h=0;h<info.n;h++)
			sprintf(tekst,"%s %d",tekst,info.przyc[g][h].obraz);
	for(int g=0;g<info.n;g++)
		for(int h=0;h<info.n;h++)
			sprintf(tekst,"%s %d",tekst,info.przyc[g][h].odkryty);
	fprintf(fptr,"%s",tekst);
	fclose(fptr);
	aktywna = 0;
}


void nowa_gra(){
}

void wczytaj_gre(){
}
void poddaj_sie(){
}

static void klikniete(GtkWidget *widget, gpointer *data);
void stworz_plansze(GtkWidget *widget, gpointer *data);


int main(int argc,char *argv[]){
	if ((info.potoki=initPipes(argc,argv)) == NULL)
	        return 1;
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
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(zakoncz),&info.potoki);

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
	info.ruchgracza=0;
	for(int g=0;g<info.n;g++)
		for(int h=0;h<info.n;h++)
			info.przyc[g][h].odkryty=0;
	aktywna = 1;
	gchar naglowek[14];
	info.n = *((int*)data);
	gen(info.n,info.przyc,-1);
	if(info.gracz)
		sprintf(naglowek,"Memory %dx%d - B",info.n,info.n);
	else
		sprintf(naglowek,"Memory %dx%d - A",info.n,info.n);

	GtkWidget *window2 =gtk_window_new(GTK_WINDOW_TOPLEVEL);
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


static void klikniete(GtkWidget *widget, gpointer *data){
	Przycisk *zm = (Przycisk*) data;
	if(zm->odkryty || info.gracz!=info.ruchgracza || info.odkryte>=2){
		return;
	}
	odkryj(*zm);
	zm->odkryty=1;
	info.ktoreodkryl[info.odkryte]=zm;
	info.odkryte++;
	info.ruchy+=2;
	printf("%d\n",zm->obraz);
	if(info.odkryte==2){
		if((info.ktoreodkryl[0])->obraz!=(info.ktoreodkryl[1])->obraz){
			g_timeout_add(CZAS_ODKRYCIA,zakryj,info.ktoreodkryl[0]);
			g_timeout_add(CZAS_ODKRYCIA,zakryj,info.ktoreodkryl[1]);
			(info.ktoreodkryl[1])->odkryty=0;
			(info.ktoreodkryl[0])->odkryty=0;
			if(info.ruchgracza==1){
				gtk_label_set_text(GTK_LABEL(info.mv),"Move:\nPlayer A");
				info.ruchgracza=0;
			}
			else{
				gtk_label_set_text(GTK_LABEL(info.mv),"Move:\nPlayer B");
				info.ruchgracza=1;
			}
			//przekaz_tekst(1,info);
		}
		else{
			char sl[19];
			if(info.ruchgracza==1){
				info.punktyB++;
				sprintf(sl,"Player B Score:\n%d",info.punktyB);
				gtk_label_set_text(GTK_LABEL(info.pbs),sl);
			}
			else{
				info.punktyA++;
				sprintf(sl,"Player A Score:\n%d",info.punktyA);
				gtk_label_set_text(GTK_LABEL(info.pas),sl);
			}
			//przekaz_tekst(0,info);
		}
		info.odkryte=0;
	}
	else
	{
		//przekaz_tekst(0,info);
	}
	bool czywygral=0;
	for(int g=0;g<info.n;g++)
		for(int h=0;h<info.n;h++){
			if(info.przyc[g][h].odkryty==0){
				czywygral=1;
				break;
			}
		}
	if(czywygral==0)
		printf("POPUP WYGRALES");

	return;
}
