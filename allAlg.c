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
	Result res;
	int showFlag;
	char snumOpen[4],snumClose[4],sfindFlag[4];
  	const	char *start,*end;
	const 	char *file;
	start = gtk_entry_get_text(GTK_ENTRY(TallStart));
	end = gtk_entry_get_text(GTK_ENTRY(TallEnd));
	file= gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(FallFile));

	showFlag = 0;
	CreatALGraph(file);

	//广度优先搜索算法
	res = BFS(*start,*end,showFlag);
	sprintf(snumOpen,"%d",res.numOpen);
	sprintf(snumClose,"%d",res.numClose);
	if(res.findFlag == 1){
		strcpy(sfindFlag,"是");
	}else{
		strcpy(sfindFlag,"否");
	}
	gtk_entry_set_text(GTK_ENTRY(TallOpen[0]),snumOpen);
	gtk_entry_set_text(GTK_ENTRY(TallClose[0]),snumClose);
	gtk_entry_set_text(GTK_ENTRY(TallFind[0]),sfindFlag);

	//深度优先搜索算法	
	res = DFS(*start,*end,showFlag);
	sprintf(snumOpen,"%d",res.numOpen);
	sprintf(snumClose,"%d",res.numClose);
	if(res.findFlag == 1){
		strcpy(sfindFlag,"是");
	}else{
		strcpy(sfindFlag,"否");
	}
	gtk_entry_set_text(GTK_ENTRY(TallOpen[1]),snumOpen);
	gtk_entry_set_text(GTK_ENTRY(TallClose[1]),snumClose);
	gtk_entry_set_text(GTK_ENTRY(TallFind[1]),sfindFlag);

	//深度受限搜索算法
	res = LimitDFS(*start,*end,showFlag);
	sprintf(snumOpen,"%d",res.numOpen);
	sprintf(snumClose,"%d",res.numClose);
	if(res.findFlag == 1){
		strcpy(sfindFlag,"是");
	}else{
		strcpy(sfindFlag,"否");
	}
	gtk_entry_set_text(GTK_ENTRY(TallOpen[2]),snumOpen);
	gtk_entry_set_text(GTK_ENTRY(TallClose[2]),snumClose);
	gtk_entry_set_text(GTK_ENTRY(TallFind[2]),sfindFlag);

	//迭代搜索算法
	res = IterDFS(*start,*end,showFlag);
	sprintf(snumOpen,"%d",res.numOpen);
	sprintf(snumClose,"%d",res.numClose);
	if(res.findFlag == 1){
		strcpy(sfindFlag,"是");
	}else{
		strcpy(sfindFlag,"否");
	}
	gtk_entry_set_text(GTK_ENTRY(TallOpen[3]),snumOpen);
	gtk_entry_set_text(GTK_ENTRY(TallClose[3]),snumClose);
	gtk_entry_set_text(GTK_ENTRY(TallFind[3]),sfindFlag);

	//等代价搜索算法
	res = CostSearch(*start,*end,showFlag);
	sprintf(snumOpen,"%d",res.numOpen);
	sprintf(snumClose,"%d",res.numClose);
	if(res.findFlag == 1){
		strcpy(sfindFlag,"是");
	}else{
		strcpy(sfindFlag,"否");
	}
	gtk_entry_set_text(GTK_ENTRY(TallOpen[4]),snumOpen);
	gtk_entry_set_text(GTK_ENTRY(TallClose[4]),snumClose);
	gtk_entry_set_text(GTK_ENTRY(TallFind[4]),sfindFlag);

	//最佳优先搜索算法
	res = BestSearch(*start,*end,showFlag);
	sprintf(snumOpen,"%d",res.numOpen);
	sprintf(snumClose,"%d",res.numClose);
	if(res.findFlag == 1){
		strcpy(sfindFlag,"是");
	}else{
		strcpy(sfindFlag,"否");
	}
	gtk_entry_set_text(GTK_ENTRY(TallOpen[5]),snumOpen);
	gtk_entry_set_text(GTK_ENTRY(TallClose[5]),snumClose);
	gtk_entry_set_text(GTK_ENTRY(TallFind[5]),sfindFlag);
}



//回到主窗口
void back_main(GtkWidget *widget,gpointer data){
	gtk_widget_show (mainwindow); 
}

//所有算法对比研究窗口
void clicked_All(GtkWidget *widget,gpointer data){
	gtk_widget_hide(mainwindow);  

	GtkWidget *Bstart;  
	GtkBuilder *builder;

	builder = gtk_builder_new (); 
	gtk_builder_add_from_file(builder,"all.glade",NULL);
	Allwindow = GTK_WIDGET (gtk_builder_get_object (builder, "window")); 
	gtk_window_set_title(GTK_WINDOW(Allwindow),"图搜索算法对比研究");

	//文件选择
	FallFile = GTK_WIDGET(gtk_builder_get_object (builder,"Ffile"));
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(FallFile),"./");
	GtkFileFilter *filter;
	filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter,"*.txt");		//过滤 只能选择 .txt 文件
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (FallFile), filter);
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(FallFile),"a.txt" );

	//执行按钮
	Bstart = GTK_WIDGET (gtk_builder_get_object (builder, "Bstart")); 


	//输入框
	TallStart = GTK_WIDGET (gtk_builder_get_object (builder,"TallStart")); 
	TallEnd = GTK_WIDGET (gtk_builder_get_object (builder, "TallEnd")); 
	gtk_entry_set_text(GTK_ENTRY(TallStart),"a");	//起始点默认 a
	gtk_entry_set_text(GTK_ENTRY(TallEnd),"c");	//目标点默认c

	//open表扩展节点数	
	TallOpen[0] = GTK_WIDGET (gtk_builder_get_object (builder,"TallOpen1")); 
	TallOpen[1] = GTK_WIDGET (gtk_builder_get_object (builder,"TallOpen2"));
	TallOpen[2] = GTK_WIDGET (gtk_builder_get_object (builder,"TallOpen3"));
	TallOpen[3] = GTK_WIDGET (gtk_builder_get_object (builder,"TallOpen4"));
	TallOpen[4] = GTK_WIDGET (gtk_builder_get_object (builder,"TallOpen5"));
	TallOpen[5] = GTK_WIDGET (gtk_builder_get_object (builder,"TallOpen6"));

	//close表扩展节点数
	TallClose[0] = GTK_WIDGET (gtk_builder_get_object (builder,"TallClose1"));
	TallClose[1] = GTK_WIDGET (gtk_builder_get_object (builder,"TallClose2"));
	TallClose[2] = GTK_WIDGET (gtk_builder_get_object (builder,"TallClose3"));
	TallClose[3] = GTK_WIDGET (gtk_builder_get_object (builder,"TallClose4"));
	TallClose[4] = GTK_WIDGET (gtk_builder_get_object (builder,"TallClose5"));
	TallClose[5] = GTK_WIDGET (gtk_builder_get_object (builder,"TallClose6"));

	//搜索结果
	TallFind[0] = GTK_WIDGET (gtk_builder_get_object (builder,"TallFind1"));
	TallFind[1] = GTK_WIDGET (gtk_builder_get_object (builder,"TallFind2"));
	TallFind[2] = GTK_WIDGET (gtk_builder_get_object (builder,"TallFind3"));
	TallFind[3] = GTK_WIDGET (gtk_builder_get_object (builder,"TallFind4"));
	TallFind[4] = GTK_WIDGET (gtk_builder_get_object (builder,"TallFind5"));
	TallFind[5] = GTK_WIDGET (gtk_builder_get_object (builder,"TallFind6"));

	gtk_builder_connect_signals (builder, NULL); 
	gtk_container_add(GTK_CONTAINER(Allwindow), Bstart);
        g_signal_connect(G_OBJECT(Bstart),"clicked",G_CALLBACK(all_start),data);
	g_signal_connect_swapped(G_OBJECT(Allwindow),"destroy",G_CALLBACK(back_main),NULL );
	g_object_unref (G_OBJECT (builder));   
	gtk_widget_show (Allwindow); 
}
