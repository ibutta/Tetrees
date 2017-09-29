/*
 * maincontrol.cpp
 *
 *  Created on: May 26, 2017
 *      Author: Igor S. Buttarello
 */

/**
 * @file TetreesMainApp.cpp
 * @brief The source file for the main function.
 */

#include <gtk/gtk.h>
#include <TetreesEngine.hpp>

static void	activate (GtkApplication *app, gpointer user_data){

	TetreesEngine gameEngine;

	gameEngine.gtkSetup(app, user_data);

}

/// A simple main function used just to start the application.
/**
 * The main routine is short, basic, and used only to create a GtkApplication
 * object, connect it to a activate callback, and start GTK's main loop.
 */
int main(){

	GtkApplication *tetreesApp;
	int status;

	tetreesApp = gtk_application_new("com.github.ibutta.tetrees", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(tetreesApp, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run(G_APPLICATION (tetreesApp), 0, NULL);
	g_object_unref(tetreesApp);

	return status;

}
