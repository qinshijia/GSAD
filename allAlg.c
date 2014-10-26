#include <gtk/gtk.h>
#include <glib.h>
#include <string.h>
#include "Algorithm.h"
extern GtkWidget	*mainwindow;
GtkWidget 	*TallStart,*TallEnd; 
GtkWidget	*Allwindow;
GtkWidget	*FallFile;
GtkWidget	*TallOpen[6],*TallClose[6],*TallFind[6];	



void all_start(){
	ALGraph G;
	int showFlag;
	char find = 'e';
  	const	char *start,*end;
	const 	char *file;
	int numOpen[6],numClose[6],isFind[6];
	start = gtk_entry_get_text(GTK_ENTRY(TallStart));
	end = gtk_entry_get_text(GTK_ENTRY(TallEnd));
	file= gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(FallFile));

	printf("all-start.....start = %c,end = %c,file = %s\n",*start,*end,file);

	showFlag = 0;
	CreatALGraph(file);
	show(&G);

		BFS(*start,*end,showFlag);




//gtk_entry_set_text(GTK_ENTRY(TallStart),"w");
}



//回到主窗口
void back_main(GtkWidget *widget,gpointer data){
	gtk_widget_show (mainwindow);//顯示窗體
}

//所有算法对比研究窗口
void clicked_All(GtkWidget *widget,gpointer data){
	gtk_widget_hide(mainwindow); //隐藏主窗口

	GtkWidget *Bstart;  
	GtkWidget *Topen,*Tclose;
	GtkWidget *Tfind[6],*TopenNum[6],*TcloseNum[6];
	GtkBuilder *builder;

	builder = gtk_builder_new ();//指針分配空間
	gtk_builder_add_from_file(builder,"all.glade",NULL);
	Allwindow = GTK_WIDGET (gtk_builder_get_object (builder, "window"));//獲取window串口使用權
	gtk_window_set_title(GTK_WINDOW(Allwindow),"图搜索算法对比研究");

	FallFile = GTK_WIDGET(gtk_builder_get_object (builder,"Ffile"));
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(FallFile),"./");
	GtkFileFilter *filter;
	filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter,"*.txt");		//过滤 只能选择 .txt 文件
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (FallFile), filter);
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(FallFile),"a.txt" );

	Bstart = GTK_WIDGET (gtk_builder_get_object (builder, "Bstart"));//獲取控件使用權

	TallStart = GTK_WIDGET (gtk_builder_get_object (builder,"TallStart"));//獲取控件使用權
	TallEnd = GTK_WIDGET (gtk_builder_get_object (builder, "TallEnd"));//獲取控件使用權
	gtk_entry_set_text(GTK_ENTRY(TallStart),"a");
	gtk_entry_set_text(GTK_ENTRY(TallEnd),"c");

	gtk_builder_connect_signals (builder, NULL);//連接裏面的信號到槽
	gtk_container_add(GTK_CONTAINER(Allwindow), Bstart);
        g_signal_connect(G_OBJECT(Bstart),"clicked",G_CALLBACK(all_start),data);
	g_signal_connect_swapped(G_OBJECT(Allwindow),"destroy",G_CALLBACK(back_main),NULL );
	g_object_unref (G_OBJECT (builder));  //釋放xml內存空間
	gtk_widget_show (Allwindow);//顯示窗體
}

