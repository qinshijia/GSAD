#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include "algorithm.h"
#include "allAlg.h"
//main.c
//master main.c
GtkWidget 	*mainwindow,*Algwindow;
GtkWidget 	*Tstart,*Tend,*Tdeep; 
GtkWidget 	*Ffile;
GtkTextBuffer	*bufopen,*bufclose;
GtkTextBuffer	*buffind,*bufopenNum,*bufcloseNum,*bufcost,*bufpath;
GtkTextTag 	*addOpen_style,*addClose_style;

extern GtkWidget	*TallOpen[6],*TallClose[6],*TallFind[6];	
extern int DEEPLIMIT; 

//清除文本框内容
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
	
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufcost),&Gstart,&Gend);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(bufcost),&Gstart,&Gend);

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufpath),&Gstart,&Gend);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(bufpath),&Gstart,&Gend);
}
void showError(char *message){
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,message,NULL);
	gtk_dialog_run(GTK_DIALOG(dialog)); 
	gtk_widget_destroy(dialog);
}
/*
void dialog()
{
	GtkWidget *button;
	GtkWidget *dialog;
	 
	dialog=gtk_dialog_new( );
	 
	button=gtk_button_new_with_label(“Yes”);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),button,TRUE,TRUE,0);
	gtk_signal_connect(GTK_OBJECT(button),”clicked”,G_CALLBACK(gtk_main_quit),NULL);
	gtk_widget_show(button);
	 
	button=gtk_button_new_with_label(“No”);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area),button,TRUE,TRUE,0);
//	gtk_signal_connect(GTK_OBJECT(button),”clicked”,G_CALLBACK(destroy),dialog);
	gtk_widget_show(button);
	gtk_widget_show(dialog);
}
*/
void showPath(Result res){
	int n,emptyClose,inClose,findFlag;
	char target;
	char buffer[16];
	EdgeNode *tempNode;
	GtkTextIter start,end;

	findFlag = res.findFlag;
	target 	 = res.target; 
	if(findFlag == 0){
		memset(buffer,'\0',sizeof(buffer));
		buffer[0] = target;
		strcat(buffer,"\tnot find!");
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufpath),&start,&end);	
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufpath),&end,buffer,strlen(buffer)); 
		return;
		
	}


	inClose = res.inClose;
	emptyClose = 0;
	n = valueIndex(target);
	do{							//输出Close表中所有的节点
		emptyClose = Pop(Close,&inClose,&tempNode,0);
		if(emptyClose != -1 && tempNode->adjvex == n){
			n = tempNode->adjvex;
			memset(buffer,'\0',sizeof(buffer));
			buffer[0] = G->adjlist[n].vertex;
			gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufpath),&start,&end);	
			tempNode= tempNode->father;
			if(tempNode == NULL){
				gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufpath),&end,buffer,strlen(buffer)); 
				break;
			}else{
				strcat(buffer,"\t->\t");
				gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufpath),&end,buffer,strlen(buffer)); 
			}
			n = tempNode->adjvex;
		}
	}while(emptyClose != -1);

}

//在文本框显示搜索结果结果
void showResult(Result res){
	char buffer[8];
	int findFlag,openNum,closeNum,cost;
	GtkTextIter start,end;
	findFlag = res.findFlag;
	openNum = res.numOpen;
	closeNum = res.numClose;
	cost = res.cost;
	
	//搜索结果
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(buffind),&start,&end);
	memset(buffer,'\0',sizeof(buffer));
	if(findFlag == 1){
		strcpy(buffer,"是");
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffind),&end,buffer,strlen(buffer)); 
		
	}else{
		strcpy(buffer,"否");
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(buffind),&end,buffer,strlen(buffer)); 
		
	}
	//open表扩展节点数
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopenNum),&start,&end);
	memset(buffer,'\0',sizeof(buffer));
	sprintf(buffer,"%d",openNum);
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopenNum),&end,buffer,strlen(buffer)); 

	//close表扩展节点数
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufcloseNum),&start,&end);
	memset(buffer,'\0',sizeof(buffer));
	sprintf(buffer,"%d",closeNum);
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufcloseNum),&end,buffer,strlen(buffer)); 

	//花费代价
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufcost),&start,&end);
	memset(buffer,'\0',sizeof(buffer));
	sprintf(buffer,"%d",cost);
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufcost),&end,buffer,strlen(buffer)); 


	//搜索结果路径
	showPath(res);

	while(gtk_events_pending())
		gtk_main_iteration();	//刷新文本框的内容
}

//开始执行算法
void Algorithm_start(GtkWidget *widget,gpointer data){
	int showFlag,runFlag;
  	const	char *start,*end,*deep;
	const 	char *file;
	Result  res;

	//获取输入的值
	showFlag = 1;
	start = gtk_entry_get_text(GTK_ENTRY(Tstart));			//起始点
	end = gtk_entry_get_text(GTK_ENTRY(Tend));			//目标点
	deep = gtk_entry_get_text(GTK_ENTRY(Tdeep));			//深度
	file= gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(Ffile));	//文件位置

	Textview_clear();
	
	DEEPLIMIT = atoi(deep);

	//判断输入合法性
	runFlag = CreatALGraph(file);		//建立邻接点表
	if(runFlag == -1){
		return;
	}
	runFlag = strcmp(start,"");
	if(!runFlag){
		showError("请输入起点！");
		return;
	}
	runFlag = strcmp(end,"");
	if(!runFlag){
		showError("请输入目标点！");
		return;
	}
	runFlag = strcmp(deep,"");
	if(!runFlag){
		showError("请输入受限深度！");
		return;
	}
	runFlag = valueIndex(*start);
	if(runFlag == -1){
		showError("请输入正确的起点！");
		return;
	}


	if(!(strcmp(data,"BFS"))){			//广度优先搜索算法

		res = BFS(*start,*end,showFlag);
		showResult(res);

	}else if(!(strcmp(data,"DFS"))){		//深度优先搜索算法

		res = DFS(*start,*end,showFlag);
		showResult(res);

	}else if(!(strcmp(data,"LimitDFS"))){		//深度受限搜索算法

		res = DLS(*start,*end,showFlag);
		showResult(res);

	}else if(!(strcmp(data,"IterDFS"))){		//迭代法

		res = IDS(*start,*end,showFlag);
		showResult(res);

	}else if(!(strcmp(data,"CostSearch"))){		//等代价

		res = UCS(*start,*end,showFlag);
		showResult(res);

	}else if(!(strcmp(data,"BestSearch"))){		//最佳优先

		res = BestFS(*start,*end,showFlag);
		showResult(res);
	}
}

//显示open队列表内容
void showOpenaQueue(EdgeNode* *open,int in,int out){
	//sleep(1);
	int n,row,preNum,empty,num,addFlag,len;
	char value;
	char text[27];
	addFlag = 0;
	len = 0;
	GtkTextIter start,end;
	EdgeNode *tempNode;

	preNum = 1;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);
	row = gtk_text_buffer_get_line_count(bufopen);			//获取行号
	memset(text,'\0',sizeof(text));
	sprintf(text,"%d",row);
	len = strlen(text);

	gtk_text_iter_backward_line(&end);				//向后移一行
	preNum = gtk_text_iter_get_chars_in_line(&end);			//获取该行字符数
	preNum = (preNum-len-2)/2;					//计算元素个数

	strcat(text,":\t");
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text));//显示行号



	empty = in;
	if(empty < out){
		empty = empty + MaxVertexNum;
	}
	num = empty - out;		//当前open表的中个数
	if(num > preNum){		//open表中添加数据
		addFlag = 1;
	}
	in = in - 1;
	while(num--){
		if(out == MaxVertexNum){
			out = 0;
		}
		memset(text,'\0',sizeof(text));
		tempNode = open[out];
		n = tempNode->adjvex;
		value = G->adjlist[n].vertex;
		text[0] = value;
		strcat(text," ");
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);	//显示open表的内容
		if(addFlag&&(out == in)){						//open表内容增加,高亮显示
			gtk_text_buffer_insert_with_tags(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text),addOpen_style,NULL);

		}else{
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text)); 
		}
		out++;
	}
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,"\n",1); 
	while(gtk_events_pending())
		gtk_main_iteration();	//刷新文本框的内容
}

//显示open表栈内容
void showOpenaStack(EdgeNode* *open,int in){

	int n,row,len,num,preNum,addFlag;
	char value;
	char text[27];
	GtkTextIter start,end;
	EdgeNode *tempNode;

	num = 0;
	len = 0;
	preNum = 0;
	addFlag = 0;
	memset(text,'\0',sizeof(text));
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);
	row = gtk_text_buffer_get_line_count(bufopen);			//获取行号
	sprintf(text,"%d",row);
	len = strlen(text);

	gtk_text_iter_backward_line(&end);				//向后移一行
	preNum = gtk_text_iter_get_chars_in_line(&end);			//获取该行字符数
	preNum = (preNum-len-2)/2;					//计算元素个数
	
	if(preNum < in){
		addFlag = 1;
	}
	strcat(text,":\t");
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text));//显示行号

	in = in - 1;
	while(num <= in){
		memset(text,'\0',sizeof(text));
		tempNode = open[num];
		n = tempNode->adjvex;
		value = G->adjlist[n].vertex;
		text[0] = value;
		strcat(text," ");
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufopen),&start,&end);//显示open表的内容
		if(addFlag&&(num == in)){					//open表内容增加
			gtk_text_buffer_insert_with_tags(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text),addOpen_style,NULL);

		}else{
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,text,strlen(text)); 
		}
		num++;
	}
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufopen),&end,"\n",1); 
	while(gtk_events_pending())
		gtk_main_iteration();	//刷新文本框的内容
}

//显示close表内容
void showClose(EdgeNode* *open,int in){
	int n,row,num;
	char value;
	char text[27];
	GtkTextIter start,end;
	EdgeNode *tempNode;

	num = 0;
	memset(text,'\0',sizeof(text));
	if(in <= 0){
		printf("open stack is empty!\n");
		return;
	}
	row = gtk_text_buffer_get_line_count(bufclose);				//获取行号
	sprintf(text,"%d",row);
	strcat(text,":\t");
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufclose),&start,&end);
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufclose),&end,text,strlen(text));//显示行号

	in = in - 1;
	while(num <= in){
		memset(text,'\0',sizeof(text));
		tempNode = open[num];
		n = tempNode->adjvex;
		value = G->adjlist[n].vertex;
		text[0] = value;
		strcat(text," ");
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(bufclose),&start,&end);	//显示open表的内容
		if(num == in){								//open表内容增加
			gtk_text_buffer_insert_with_tags(GTK_TEXT_BUFFER(bufclose),&end,text,strlen(text),addClose_style,NULL);

		}else{
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufclose),&end,text,strlen(text)); 
		}
		num++;
	}
	gtk_text_buffer_insert(GTK_TEXT_BUFFER(bufclose),&end,"\n",1); 
	while(gtk_events_pending())
		gtk_main_iteration();	//刷新文本框的内容

}

//关闭算法窗口，返回主窗口
void back_parent(GtkWidget *widget,gpointer data){
	gtk_widget_show (mainwindow);
}

//退出程序
void exit_GraphSearch(GtkWidget *widget,gpointer data){
	freeGraph();				//释放邻接表
	gtk_main_quit ();  			//退出程序
}

//算法窗口
void clicked_Algorithm(GtkWidget *widget,gpointer data)
{
	gtk_widget_hide(mainwindow); //隐藏主窗口

	GtkWidget *Bstep;  
	GtkWidget *Topen,*Tclose;
	GtkWidget *Tfind,*TopenNum,*TcloseNum,*Tcost,*Tpath;
	GtkBuilder *builder;
	GtkWidget *Ldeep;
	
	//算法窗口
	builder = gtk_builder_new (); 
	gtk_builder_add_from_file(builder,"algorithm.glade",NULL);
	Algwindow = GTK_WIDGET (gtk_builder_get_object (builder, "window")); 

	//执行按钮
	Bstep = GTK_WIDGET (gtk_builder_get_object (builder, "Bstep")); 
	
	//输入文本框
	Tstart = GTK_WIDGET (gtk_builder_get_object (builder, "Tstart"));
	Tend = GTK_WIDGET (gtk_builder_get_object (builder, "Tend"));
	gtk_entry_set_text(GTK_ENTRY(Tstart),"a");
	gtk_entry_set_text(GTK_ENTRY(Tend),"c");
	

	//结果文本框
	Topen = GTK_WIDGET (gtk_builder_get_object (builder, "Topen"));
	Tclose = GTK_WIDGET (gtk_builder_get_object (builder, "Tclose"));
	Tfind = GTK_WIDGET (gtk_builder_get_object (builder, "Tfind"));
	TopenNum = GTK_WIDGET (gtk_builder_get_object (builder, "TopenNum"));
	TcloseNum = GTK_WIDGET (gtk_builder_get_object (builder, "TcloseNum"));
	Tcost = GTK_WIDGET (gtk_builder_get_object (builder, "Tcost"));
	Tpath = GTK_WIDGET (gtk_builder_get_object (builder, "Tpath"));
	bufopen = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Topen));	
	bufclose = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Tclose));	
	buffind = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Tfind));	
	bufopenNum = gtk_text_view_get_buffer(GTK_TEXT_VIEW(TopenNum));	
	bufcloseNum = gtk_text_view_get_buffer(GTK_TEXT_VIEW(TcloseNum));	
	bufcost = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Tcost));	
	bufpath = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Tpath));	
	addOpen_style = gtk_text_buffer_create_tag (bufopen, "foreground", "foreground","red", NULL);	//显示红色字体
	addClose_style = gtk_text_buffer_create_tag (bufclose, "foreground", "foreground","blue", NULL);//显示蓝色字体
	

	//文件选择
	Ffile = GTK_WIDGET(gtk_builder_get_object (builder,"Ffile"));
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(Ffile),"./");
	GtkFileFilter *filter;
	filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter,"*.txt");		//过滤 只能选择 .txt 文件
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (Ffile), filter);
	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(Ffile),"testData.txt" );//默认a.txt

	//深度设置
	Ldeep = GTK_WIDGET (gtk_builder_get_object (builder, "Ldeep"));
	Tdeep = GTK_WIDGET (gtk_builder_get_object (builder, "Tdeep"));
	gtk_entry_set_text(GTK_ENTRY(Tdeep),"3");
	gtk_widget_hide(Ldeep); 
	gtk_widget_hide(Tdeep); 


	if(!(strcmp(data,"BFS"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"广度优先搜索算法");

	}else if(!(strcmp(data,"DFS"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"深度优先搜索算法");

	}else if(!(strcmp(data,"LimitDFS"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"深度受限搜索算法");
		gtk_widget_show(Ldeep); 
		gtk_widget_show(Tdeep); 

	}else if(!(strcmp(data,"IterDFS"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"迭代搜索算法");

	}else if(!(strcmp(data,"CostSearch"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"等代价搜索算法");

	}else if(!(strcmp(data,"BestSearch"))){

		gtk_window_set_title(GTK_WINDOW(Algwindow),"最佳优先搜索算法");
		
	}
	gtk_builder_connect_signals (builder, NULL); 
	gtk_container_add(GTK_CONTAINER(Algwindow), Bstep);
        g_signal_connect(G_OBJECT(Bstep),"clicked",G_CALLBACK(Algorithm_start),data);

	g_signal_connect_swapped(G_OBJECT(Algwindow),"destroy",G_CALLBACK(back_parent),NULL );
	g_object_unref (G_OBJECT (builder));   
	gtk_widget_show (Algwindow); 
}

int main (int argc, char *argv[])
{
	GtkWidget *Ball,*Bdfs,*Bbfs,*BlimitDfs,*Bbests,*Bcosts,*Biters;  
	GtkBuilder *builder;
	gtk_init (&argc, &argv); 
	builder = gtk_builder_new (); 
	gtk_builder_add_from_file(builder,"main.glade",NULL);
	mainwindow = GTK_WIDGET (gtk_builder_get_object (builder, "mainwindow")); 
	gtk_window_set_title(GTK_WINDOW(mainwindow),"图搜索算法对比研究");

	CreateFile();
	G = (ALGraph *)malloc(sizeof(ALGraph));

	// 选择按钮
	Ball = GTK_WIDGET (gtk_builder_get_object (builder, "all"));	 
	Bdfs = GTK_WIDGET (gtk_builder_get_object (builder, "DFS"));
	Bbfs= GTK_WIDGET (gtk_builder_get_object (builder, "BFS"));
	BlimitDfs= GTK_WIDGET (gtk_builder_get_object (builder, "LimitDFS"));
	Bbests = GTK_WIDGET (gtk_builder_get_object (builder, "BestSearch"));
	Bcosts = GTK_WIDGET (gtk_builder_get_object (builder, "CostSearch"));
	Biters = GTK_WIDGET (gtk_builder_get_object (builder, "IterDFS"));

	//连接信号源
	gtk_builder_connect_signals (builder, NULL); 
	gtk_container_add(GTK_CONTAINER(mainwindow), Bbfs);
        g_signal_connect(G_OBJECT(Ball),"clicked",G_CALLBACK(clicked_All),"ALL");
        g_signal_connect(G_OBJECT(Bbfs),"clicked",G_CALLBACK(clicked_Algorithm),"BFS");
        g_signal_connect(G_OBJECT(Bdfs),"clicked",G_CALLBACK(clicked_Algorithm),"DFS");
        g_signal_connect(G_OBJECT(BlimitDfs),"clicked",G_CALLBACK(clicked_Algorithm),"LimitDFS");
        g_signal_connect(G_OBJECT(Bbests),"clicked",G_CALLBACK(clicked_Algorithm),"BestSearch");
        g_signal_connect(G_OBJECT(Bcosts),"clicked",G_CALLBACK(clicked_Algorithm),"CostSearch");
        g_signal_connect(G_OBJECT(Biters),"clicked",G_CALLBACK(clicked_Algorithm),"IterDFS");
	g_signal_connect_swapped(G_OBJECT(mainwindow),"destroy",G_CALLBACK(exit_GraphSearch),NULL );
	g_object_unref (G_OBJECT (builder));   
	gtk_widget_show (mainwindow); 
	gtk_main (); 
	return 0;
}
