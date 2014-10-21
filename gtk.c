#include <gtk/gtk.h>

void button_clicked_cb(GtkWidget *widget,gpointer data)
{
	        gtk_main_quit ();  //退出程序
}

int main (int argc, char *argv[])
{
	GtkWidget *btn;  
	GtkBuilder *builder;
	GtkWidget *window;
	gtk_init (&argc, &argv);//gtk應用程序迴路
	builder = gtk_builder_new ();//指針分配空間
	gtk_builder_add_from_file(builder,"main.glade",NULL);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "mainwindow"));//獲取window串口使用權
	btn = GTK_WIDGET (gtk_builder_get_object (builder, "all"));//獲取控件使用權
	gtk_builder_connect_signals (builder, NULL);//連接裏面的信號到槽
	gtk_container_add(GTK_CONTAINER(window), btn);
        g_signal_connect(G_OBJECT(btn),"clicked",G_CALLBACK(button_clicked_cb),NULL);
	g_object_unref (G_OBJECT (builder));  //釋放xml內存空間
	gtk_widget_show (window);//顯示窗體
	gtk_main ();//迴路等待
	return 0;
}
