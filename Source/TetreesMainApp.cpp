/*
 * maincontrol.cpp
 *
 *  Created on: May 26, 2017
 *      Author: Igor S. Buttarello
 */

#include <gtk/gtk.h>
#include <TetreesEngine.hpp>

static void	activate (GtkApplication *app, gpointer user_data){

	TetreesEngine gameEngine;

	gameEngine.gtkSetup(app, user_data);

}

int main(int argc, char **argv){

	GtkApplication *tetreesApp;
	int status;

	tetreesApp = gtk_application_new("com.github.ibutta.tetrees", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(tetreesApp, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run(G_APPLICATION (tetreesApp), argc, argv);
	g_object_unref(tetreesApp);

	return status;

}
