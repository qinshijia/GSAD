#include <gtk/gtk.h>
#include <glib.h>
#include <string.h>
#include "mytu.h"

GtkWidget 	*mainwindow,*Algwindow;
GtkWidget 	*Tstart,*Tend; 
GtkWidget 	*Ffile;
GtkTextBuffer	*bufopen,*bufclose;
GtkTextBuffer	*buffind,*bufopenNum,*bufcloseNum;
GtkTextTag 	*underline;

void Textview_clear(){

	GtkTextIter Gstart,Gend;

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&Gstart,&Gend);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(bufopen),&Gstart,&Gend);

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufclose),&Gstart,&Gend);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(bufclose),&Gstart,&Gend);

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufclose),&Gstart,&Gend);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(bufclose),&Gstart,&Gend);

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffind),&Gstart,&Gend);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffind),&Gstart,&Gend);

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopenNum),&Gstart,&Gend);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(bufopenNum),&Gstart,&Gend);

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufcloseNum),&Gstart,&Gend);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(bufcloseNum),&Gstart,&Gend);
	
}

void Algorithm_start(GtkWidget *widget,gpointer data){
	ALGraph G;
	char find = 'e';
  	const	char *start,*end;
	const 	char *file;
	start = gtk_entry_get_text(GTK_ENTRY(Tstart));
	end = gtk_entry_get_text(GTK_ENTRY(Tend));
	file= gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(Ffile));

	Textview_clear();

	if(file == NULL){
		gtk_message_dialog_new(gtk_widget_get_parent_window(Ffile),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,NULL);
	}
	CreatALGraph(&G,file);
	show(&G);
	if(!(strcmp(data,"BFS"))){

		BFS(&G,*end);

	}else if(!(strcmp(data,"DFS"))){

		DFS(&G,*end);

	}else if(!(strcmp(data,"LimitDFS"))){

		LimitDFS(&G,*end);

	}else if(!(strcmp(data,"IterDFS"))){

		IterDFS(&G,*end);

	}else if(!(strcmp(data,"CostSearch"))){

		CostSearch(&G,*end);

	}else if(!(strcmp(data,"BestSearch"))){

		BestSearch(&G,*end);
		
	}
	printf("Algorithm_start\tstart %s\t end %s\tfilename %s\n",start,end,file);
}
void showResult(int findFlag,int openNum,int closeNum){
	printf("showResult.......0\n");
	char buffer[8];
	GtkTextIter start,end;

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffind),&start,&end);
	memset(buffer,'\0',sizeof(buffer));
	if(findFlag == 1){
		strcpy(buffer,"是");
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffind),&end,buffer,strlen(buffer));//显示行号
		
	}else{
		strcpy(buffer,"否");
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffind),&end,buffer,strlen(buffer));//显示行号
		
	}
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopenNum),&start,&end);
	memset(buffer,'\0',sizeof(buffer));
	sprintf(buffer,"%d",openNum);
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopenNum),&end,buffer,strlen(buffer));//显示行号
	
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufcloseNum),&start,&end);
	memset(buffer,'\0',sizeof(buffer));
	sprintf(buffer,"%d",closeNum);
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufcloseNum),&end,buffer,strlen(buffer));//显示行号

	while(gtk_events_pending())
		gtk_main_iteration();	//为了能刷新文本框的内容

	
}

void showOpenaQueue(EdgeNode* *open,ALGraph *G,int in,int out){
	//sleep(1);
	int n,row,empty,col;
	char value;
	char *p,text[27];
	gchar *temp;
	p = &value;
	GtkTextIter start,end;
	EdgeNode *tempNode;

	col = 1;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);
	row = gtk_text_buffer_get_line_count(bufopen);			//获取行号

	gtk_text_iter_backward_line(&end);				//向后移一行
	col = gtk_text_iter_get_chars_in_line(&end);			//获取该行字符数
	printf("text is .....%d....\n",col);

	value = '0' + row;
	memset(text,'\0',sizeof(text));
	text[0] = value;
	strcat(text,":\t");
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text));//显示行号



	empty = in;
	if(empty < out){
		empty = empty + MaxVertexNum;
	}
	in = out;
	while(empty > in  ){
		empty--;
		if(out == MaxVertexNum){
			out = 0;
		}
		memset(text,'\0',sizeof(text));
		tempNode = open[out];
		out++;
		n = tempNode->adjvex;
		value = G->adjlist[n].vertex;
		text[0] = value;
		strcat(text," ");
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);//显示open表的内容
		gtk_text_buffer_insert_with_tags(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text),underline,NULL);
//		gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text));/*插入文本到缓冲区*/
		system("pause");
	}
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,"\n",1);/*插入文本到缓冲区*/
	while(gtk_events_pending())
		gtk_main_iteration();	//为了能刷新文本框的内容
}
void showOpenaStack(EdgeNode* *open,ALGraph *G,int in){

	int n,row,num;
	char value;
	char *p,text[27];
	GtkTextIter start,end;
	EdgeNode *tempNode;

	p = &value;
	num = 0;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);
	row = gtk_text_buffer_get_line_count(bufopen);			//获取行号
	value = '0' + row;
	memset(text,'\0',sizeof(text));
	text[0] = value;
	strcat(text,":\t");
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text));//显示行号

	while(num < in){
		printf("showOpenaStack.....in = %d\n",in);
		memset(text,'\0',sizeof(text));
		tempNode = open[num];
		num++;
		n = tempNode->adjvex;
		value = G->adjlist[n].vertex;
		text[0] = value;
		strcat(text," ");
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);//显示open表的内容
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text));/*插入文本到缓冲区*/
	}
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,"\n",1);/*插入文本到缓冲区*/
	while(gtk_events_pending())
		gtk_main_iteration();	//为了能刷新文本框的内容
}

void showClose(EdgeNode* *open,ALGraph *G,int in){
	int n,row;
	char value;
	char *p,text[27];
	p = &value;
	GtkTextIter start,end;
	EdgeNode *tempNode;
	if(in <= 0){
		printf("open stack is empty!\n");
		return;
	}
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufclose),&start,&end);
	row = gtk_text_buffer_get_line_count(bufclose);			//获取行号
	value = '0' + row;
	memset(text,'\0',sizeof(text));
	text[0] = value;
	strcat(text,":\t");
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufclose),&end,text,strlen(text));//显示行号
	while(in--){
		memset(text,'\0',sizeof(text));
		tempNode = open[in];
		n = tempNode->adjvex;
		value = G->adjlist[n].vertex;
		text[0] = value;
		strcat(text," ");
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufclose),&start,&end);//显示open表的内容
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufclose),&end,text,strlen(text));/*插入文本到缓冲区*/
	}
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufclose),&end,"\n",1);/*插入文本到缓冲区*/
	while(gtk_events_pending())
		gtk_main_iteration();	//为了能刷新文本框的内容

}


//关闭算法窗口，返回主窗口

void back_parent(GtkWidget *widget,gpointer data){
	gtk_widget_show (mainwindow);//顯示窗體
}
void exit(GtkWidget *widget,gpointer data){
	gtk_main_quit ();  //退出程序
}

void clicked_Algorithm(GtkWidget *widget,gpointer data)
{
	gtk_widget_hide(mainwindow); //隐藏主窗口

	printf("bfs..............1\n");
	GtkWidget *Bstep;  
	GtkWidget *Topen,*Tclose;
	GtkWidget *Tfind,*TopenNum,*TcloseNum;
	GtkBuilder *builder;
	GtkWidget *parent;
	
	builder = gtk_builder_new ();//指針分配空間
	gtk_builder_add_from_file(builder,"BFS.glade",NULL);
	Algwindow = GTK_WIDGET (gtk_builder_get_object (builder, "window"));//獲取window串口使用權

	Bstep = GTK_WIDGET (gtk_builder_get_object (builder, "Bstep"));//獲取控件使用權

	printf("bfs..............2\n");
	Tstart = GTK_WIDGET (gtk_builder_get_object (builder, "Tstart"));
	Tend = GTK_WIDGET (gtk_builder_get_object (builder, "Tend"));
	gtk_entry_set_text(GTK_ENTRY(Tstart),"a");
	gtk_entry_set_text(GTK_ENTRY(Tend),"c");
	
	printf("bfs..............3\n");
	Topen = GTK_WIDGET (gtk_builder_get_object (builder, "Topen"));
	Tclose = GTK_WIDGET (gtk_builder_get_object (builder, "Tclose"));
	Tfind = GTK_WIDGET (gtk_builder_get_object (builder, "Tfind"));
	TopenNum = GTK_WIDGET (gtk_builder_get_object (builder, "TopenNum"));
	TcloseNum = GTK_WIDGET (gtk_builder_get_object (builder, "TcloseNum"));
	bufopen = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Topen));	
	bufclose = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Tclose));	
	buffind = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Tfind));	
	bufopenNum = gtk_text_view_get_buffer(GTK_TEXT_VIEW(TopenNum));	
	bufcloseNum = gtk_text_view_get_buffer(GTK_TEXT_VIEW(TcloseNum));	
	gtk_text_tag_new("1");
	underline = gtk_text_buffer_create_tag (bufopen, "underline", "underline",PANGO_UNDERLINE_SINGLE, NULL);
	
	
	printf("bfs..............4\n");
	Ffile = GTK_WIDGET(gtk_builder_get_object (builder,"Ffile"));
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(Ffile),"./");
	GtkFileFilter *filter;
	filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter,"*.txt");		//过滤 只能选择 .txt 文件
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (Ffile), filter);
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(Ffile),"a.txt" );

	printf("bfs..............5\n");
	if(!(strcmp(data,"BFS"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"广度优先搜索算法");

	}else if(!(strcmp(data,"DFS"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"深度优先搜索算法");

	}else if(!(strcmp(data,"LimitDFS"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"深度受限搜索算法");

	}else if(!(strcmp(data,"IterDFS"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"迭代搜索算法");

	}else if(!(strcmp(data,"CostSearch"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"等代价搜索算法");

	}else if(!(strcmp(data,"BestSearch"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"最佳优先搜索算法");
		
	}
	gtk_builder_connect_signals (builder, NULL);//連接裏面的信號到槽
	gtk_container_add(GTK_CONTAINER(Algwindow), Bstep);
        g_signal_connect(G_OBJECT(Bstep),"clicked",G_CALLBACK(Algorithm_start),data);
	printf("bfs..............6\n");

	g_signal_connect_swapped(G_OBJECT(Algwindow),"destroy",G_CALLBACK(back_parent),NULL );
	g_object_unref (G_OBJECT (builder));  //釋放xml內存空間
	gtk_widget_show (Algwindow);//顯示窗體
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
        g_signal_connect(G_OBJECT(Ball),"clicked",G_CALLBACK(clicked_Algorithm),"ALL");
        g_signal_connect(G_OBJECT(Bbfs),"clicked",G_CALLBACK(clicked_Algorithm),"BFS");
        g_signal_connect(G_OBJECT(Bdfs),"clicked",G_CALLBACK(clicked_Algorithm),"DFS");
        g_signal_connect(G_OBJECT(BlimitDfs),"clicked",G_CALLBACK(clicked_Algorithm),"LimitDFS");
        g_signal_connect(G_OBJECT(Bbests),"clicked",G_CALLBACK(clicked_Algorithm),"BestSearch");
        g_signal_connect(G_OBJECT(Bcosts),"clicked",G_CALLBACK(clicked_Algorithm),"CostSearch");
        g_signal_connect(G_OBJECT(Biters),"clicked",G_CALLBACK(clicked_Algorithm),"IterDFS");
	g_signal_connect_swapped(G_OBJECT(mainwindow),"destroy",G_CALLBACK(exit),NULL );
	g_object_unref (G_OBJECT (builder));  //釋放xml內存空間
	gtk_widget_show (mainwindow);//顯示窗體
	GDK_THREADS_ENTER();
	gtk_main ();//迴路等待
	GDK_THREADS_LEAVE();
	return 0;
}
