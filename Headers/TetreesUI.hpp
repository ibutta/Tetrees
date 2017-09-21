/*
 * TetreesUI.h
 *
 *  Created on: Jun 1, 2017
 *      Author: Igor S. Buttarello
 */

/**
 * @file TetreesUI.hpp
 * @brief Header file for the TetreesUI class.
 */

#ifndef TETREESUI_H_
#define TETREESUI_H_

#include <gtk/gtk.h>
#include <TetreesDefs.hpp>

class TetreesEngine;
class TetreesUtils;

class TetreesUI
{

public:

	TetreesUI(TetreesEngine);
	/// User interface (UI) assembly.
	void setupUI(GtkApplication*, gpointer);

private:

	static TetreesEngine *gameEngine;
	static TetreesUtils *utils;
	static Matrix2D<rgb_t> colors;
	static Matrix2D<int> gameScene;
	static piece_t nextTetromino;
	static game_score_t gameScore;
	static game_level_t gameLevel;
	static animation_t animationMode;
	static animation_control_t animationControl;

	static gboolean onDrawPlayingFieldDrawingArea(GtkWidget*, cairo_t*, gpointer);
	static gboolean onDrawNextTetrominoDrawingArea(GtkWidget*, cairo_t*, gpointer);
	static void onPlayClicked(GtkButton*);
	static void onPauseClicked(GtkButton*);
	static void onResetClicked(GtkButton*);
	static void drawScene(cairo_t*);
	static void drawPausedScene(cairo_t*);
	static void drawNextTetromino(cairo_t*);
	static void drawGameOverAnimationStep(cairo_t*);
	static void drawRowRemovalAnimationStep(cairo_t*);
	static gboolean animationHandler(GtkWidget*);
	static gboolean updateGameContext(GtkWidget*);

	static void contextHandler(gpointer);
	static gboolean onKeyPress(GtkWidget*, GdkEventKey*, gpointer);
	static void setUIState(ui_state_t);
	static void updateScore();

};


#endif /* TETREESUI_H_ */
