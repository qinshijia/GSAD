#include <gtk/gtk.h>
#include <string.h>
#include "mytu.h"

GtkWidget 	*mainwindow;
GtkWidget 	*Tstart,*Tend; 
GtkWidget 	*Ffile;
GtkWidget  	*Topen,*Tclose;
GtkTextBuffer	*bufopen,*bufclose;


void BFS_step(GtkWidget *widget,gpointer data){
	ALGraph G;
	char find = 'e';
  	const	char *start,*end;
	const 	char *file;
	start = gtk_entry_get_text(GTK_ENTRY(Tstart));
	end = gtk_entry_get_text(GTK_ENTRY(Tend));
	file= gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(Ffile));

	printf("BFS_step\tstart %s\t end %s\tfilename %s len %d\n",start,end,file,strlen(file));
	if(file == NULL){
		gtk_message_dialog_new(gtk_widget_get_parent_window(Ffile),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,NULL);
	}
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

void showOpenaStack(EdgeNode* *open,ALGraph *G,int *in){
	int num,n;
	char value;
	char *p;
	p = &value;
	const gchar *text="How are you!";
	GtkTextIter start,end;
	EdgeNode *tempNode;
	num = *in;
	while(num--){
		tempNode = open[num];
		n = tempNode->adjvex;
		value = G->adjlist[n].vertex;
		//显示
		printf("\nshow open..%c..%d.1\n",value,n);		 
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);/*获得缓冲区开始和结束位置的Iter*/
		printf("\nshow open.....2\n");		 
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&start,p,1);/*插入文本到缓冲区*/
		printf("\nshow open.....3\n");		 

	}
	
}
void showClose(){

}
//关闭算法窗口，返回主窗口
void back_parent(GtkWidget *widget,gpointer data){
	gtk_widget_show (mainwindow);//顯示窗體
}
void exit(GtkWidget *widget,gpointer data){
	gtk_main_quit ();  //退出程序
}

void button_clicked_bfs(GtkWidget *widget,gpointer data)
{
	gtk_widget_hide(mainwindow); //隐藏主窗口

	GtkWidget *Bstep,*Bcont;  
	GtkBuilder *builder;
	GtkWidget *window,*parent;
	builder = gtk_builder_new ();//指針分配空間
	gtk_builder_add_from_file(builder,"BFS.glade",NULL);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));//獲取window串口使用權
//	parent = gtk_widget_get_parent_window(parent);

	Bstep = GTK_WIDGET (gtk_builder_get_object (builder, "Bstep"));//獲取控件使用權
	Bcont = GTK_WIDGET (gtk_builder_get_object (builder, "Bcont"));//獲取控件使用權

	Tstart = GTK_WIDGET (gtk_builder_get_object (builder, "Tstart"));
	Tend = GTK_WIDGET (gtk_builder_get_object (builder, "Tend"));
	gtk_entry_set_text(GTK_ENTRY(Tstart),"a");
	gtk_entry_set_text(GTK_ENTRY(Tend),"c");
	
	Topen = GTK_WIDGET (gtk_builder_get_object (builder, "Topen"));
	Tclose = GTK_WIDGET (gtk_builder_get_object (builder, "Tclose"));
	bufopen = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Topen));	
	bufclose = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Tclose));	

	Ffile = GTK_WIDGET(gtk_builder_get_object (builder,"Ffile"));
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(Ffile),"./");
	GtkFileFilter *filter;
	filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter,"*.txt");		//过滤 只能选择 .txt 文件
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (Ffile), filter);
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(Ffile),"a.txt" );

	gtk_builder_connect_signals (builder, NULL);//連接裏面的信號到槽
	gtk_container_add(GTK_CONTAINER(window), Bstep);
	gtk_container_add(GTK_CONTAINER(window), Bcont);
        g_signal_connect(G_OBJECT(Bstep),"clicked",G_CALLBACK(BFS_step),NULL);
        g_signal_connect(G_OBJECT(Bcont),"clicked",G_CALLBACK(BFS_next),NULL);

	g_signal_connect_swapped(G_OBJECT(window),"destroy",G_CALLBACK(back_parent),NULL );
	g_object_unref (G_OBJECT (builder));  //釋放xml內存空間
	gtk_widget_show (window);//顯示窗體
//	gtk_main ();//迴路等待
//	gtk_widget_show (mainwindow);//顯示窗體
}

int main (int argc, char *argv[])
{
	GtkWidget *Ball,*Bdfs,*Bbfs,*BlimitDfs,*Bbests,*Bcosts,*Biters;  
	GtkBuilder *builder;
	gtk_init (&argc, &argv);//gtk應用程序迴路
	builder = gtk_builder_new ();//指針分配空間
	gtk_builder_add_from_file(builder,"main.glade",NULL);
	mainwindow = GTK_WIDGET (gtk_builder_get_object (builder, "mainwindow"));//獲取window串口使用權
	gtk_window_set_title(mainwindow,"图搜索算法对比研究");

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
	g_signal_connect_swapped(G_OBJECT(mainwindow),"destroy",G_CALLBACK(exit),NULL );
	g_object_unref (G_OBJECT (builder));  //釋放xml內存空間
	gtk_widget_show (mainwindow);//顯示窗體
	gtk_main ();//迴路等待
	return 0;
}
