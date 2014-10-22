#include <gtk/gtk.h>
#include <string.h>
#include "mytu.h"
/*
void button_clicked_dfs(GtkWidget *widget,gpointer data)
{
//gtk_main_quit ();  //退出程序
	GtkWidget *btn;  
	GtkBuilder *builder;
	GtkWidget *window;
	builder = gtk_builder_new ();//指針分配空間
	gtk_builder_add_from_file(builder,"DFS.glade",NULL);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));//獲取window串口使用權
	btn = GTK_WIDGET (gtk_builder_get_object (builder, "all"));//獲取控件使用權
	gtk_builder_connect_signals (builder, NULL);//連接裏面的信號到槽
	gtk_container_add(GTK_CONTAINER(window), btn);
  //      g_signal_connect(G_OBJECT(btn),"clicked",G_CALLBACK(button_clicked_cb),NULL);
	g_object_unref (G_OBJECT (builder));  //釋放xml內存空間
	gtk_widget_show (window);//顯示窗體
	gtk_main ();//迴路等待
}
*/

GtkWidget *mainwindow;
GtkWidget *Tstart,*Tend; 
GtkWidget *Ffile;



void BFS_step(GtkWidget *widget,gpointer data){
	ALGraph G;
	char find = 'e';
//	char fileName[27]; 
//	CreatALGraph(&G,fileName);
//	show(&G);
//	BFS(&G,find);
  	const	char *start,*end;
	const 	char *file;
	start = gtk_entry_get_text(GTK_ENTRY(Tstart));
	end = gtk_entry_get_text(GTK_ENTRY(Tend));
	file= gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(Ffile));
//	fileName =(char *) file;
	CreatALGraph(&G,file);
	show(&G);
	BFS(&G,*end);


	printf("BFS_step\tstart %s\t end %s\tfilename %s\n",start,end,file);
}
void BFS_next(GtkWidget *widget,gpointer data){
	g_print("BFS_next");
	ALGraph G;
	char find = 'e';
	char fileName[27] = "a.txt";
	CreatALGraph(&G,fileName);
	show(&G);
	DFS(&G,find);
	printf("BFS_next\n");
}
void button_clicked_bfs(GtkWidget *widget,gpointer data)
{
//gtk_main_quit ();  //退出程序
	GtkWidget *Bstep,*Bcont;  
	GtkBuilder *builder;
	GtkWidget *window;
	builder = gtk_builder_new ();//指針分配空間
	gtk_builder_add_from_file(builder,"BFS.glade",NULL);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));//獲取window串口使用權

	Bstep = GTK_WIDGET (gtk_builder_get_object (builder, "Bstep"));//獲取控件使用權
	Bcont = GTK_WIDGET (gtk_builder_get_object (builder, "Bcont"));//獲取控件使用權

	Tstart = GTK_WIDGET (gtk_builder_get_object (builder, "Tstart"));
	Tend = GTK_WIDGET (gtk_builder_get_object (builder, "Tend"));
	gtk_entry_set_text(GTK_ENTRY(Tstart),"a");
	gtk_entry_set_text(GTK_ENTRY(Tend),"c");

	Ffile = GTK_WIDGET(gtk_builder_get_object (builder,"Ffile"));
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(Ffile),"./");
	GtkFileFilter *filter;
	filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter,"*.txt");		//过滤 只能选择 .txt 文件
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (Ffile), filter);

	gtk_builder_connect_signals (builder, NULL);//連接裏面的信號到槽
	gtk_container_add(GTK_CONTAINER(window), Bstep);
	gtk_container_add(GTK_CONTAINER(window), Bcont);
        g_signal_connect(G_OBJECT(Bstep),"clicked",G_CALLBACK(BFS_step),NULL);
        g_signal_connect(G_OBJECT(Bcont),"clicked",G_CALLBACK(BFS_next),NULL);

	g_object_unref (G_OBJECT (builder));  //釋放xml內存空間
	gtk_widget_show (window);//顯示窗體
	gtk_main ();//迴路等待
}

int main (int argc, char *argv[])
{
	GtkWidget *Ball,*Bdfs,*Bbfs,*BlimitDfs,*Bbests,*Bcosts,*Biters;  
	GtkBuilder *builder;
	gtk_init (&argc, &argv);//gtk應用程序迴路
	builder = gtk_builder_new ();//指針分配空間
	gtk_builder_add_from_file(builder,"main.glade",NULL);
	mainwindow = GTK_WIDGET (gtk_builder_get_object (builder, "mainwindow"));//獲取window串口使用權

	Ball = GTK_WIDGET (gtk_builder_get_object (builder, "all"));	//獲取控件使用權
	Bdfs = GTK_WIDGET (gtk_builder_get_object (builder, "DFS"));
	Bbfs= GTK_WIDGET (gtk_builder_get_object (builder, "BFS"));
	BlimitDfs= GTK_WIDGET (gtk_builder_get_object (builder, "LimitDFS"));
	Bbests = GTK_WIDGET (gtk_builder_get_object (builder, "BestSearch"));
	Bcosts = GTK_WIDGET (gtk_builder_get_object (builder, "CostSearch"));
	Biters = GTK_WIDGET (gtk_builder_get_object (builder, "IterDFS"));

	gtk_builder_connect_signals (builder, NULL);//連接裏面的信號到槽
	gtk_container_add(GTK_CONTAINER(mainwindow), Bbfs);
        g_signal_connect(G_OBJECT(Bbfs),"clicked",G_CALLBACK(button_clicked_bfs),NULL);
	g_object_unref (G_OBJECT (builder));  //釋放xml內存空間
	gtk_widget_show (mainwindow);//顯示窗體
	gtk_main ();//迴路等待
	return 0;
}
