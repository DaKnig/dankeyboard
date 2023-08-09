#include "renderer.h"

#include <gtk/gtk.h>
#include <stdio.h>

#define PROJECT_NAME "dankeyboard"
#define PROJECT_ID   "null.daknig.dankeyboard"

static void cb_activate(GtkApplication *app, gpointer _);

int main(int argc, char **argv) {
	GtkApplication *app;
	app = gtk_application_new(PROJECT_ID, G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(cb_activate), NULL);

	int status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	return status;
}

static void cb_activate(GtkApplication *app, gpointer _) {
	(void)_;

	GtkWidget *window = gtk_window_new();
	gtk_window_set_default_size(GTK_WINDOW(window), 360, 294);
	gtk_window_set_application(GTK_WINDOW(window), app);

	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
	gtk_widget_set_hexpand(box, TRUE);
	gtk_widget_set_vexpand(box, TRUE);
	gtk_window_set_child(GTK_WINDOW(window), box);

	GtkWidget *label = gtk_label_new("hello world!");
	gtk_widget_set_hexpand(label, TRUE);
	gtk_widget_set_vexpand(label, TRUE);
	gtk_widget_set_parent(label, box);

	gtk_window_present(GTK_WINDOW(window));
}
