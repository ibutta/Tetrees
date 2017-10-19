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

	/// The class constructor.
	TetreesUI(TetreesEngine);
	/// User interface (UI) assembly.
	void setupUI(GtkApplication*, gpointer);

private:

	static TetreesEngine *gameEngine; 							/**< A @ref TetreesEngine object that controls all game's rules during gameplay.*/
	static TetreesUtils *utils; 								/**< A TetreesUtils object that provides utility functions.*/
	static Matrix2D<rgb_t> colors; 								/**< An array containing @ref rgb_t "data" related to all colors available to be used by UI.*/
	static Matrix2D<int> gameScene; 							/**< Stores the actual @ref TetreesEngine::scene "game scene" to be drawn on screen.*/
	static piece_t nextTetromino; 								/**< Stores the @ref TetreesEngine::nextTetromino "next tetromino" which is drew inside
																@ref TetreesUI.nextTetrominoDrawingArea "next tetromino field" top right next the @ref TetreesEngine::gameBoard "game board".**/
	static game_score_t gameScore; 								/**< Stores the updated @ref game_data_t.gScore "game score" to be displayed at the @ref TetreesUI.scoreLabel "score label".*/
	static game_level_t gameLevel; 								/**< Stores the present @ref game_level_t "level" of the game.*/
	static animation_t animationMode; 							/**< Stores which @ref animation_t "animation" must be executed by the UI.*/
	static animation_control_t animationControl; 				/**< Stores several @ref animation_control_t "control data" about animations executed by the
																@ref TetreesEngine::gameUI "UI object".*/

	/// Handles the **draw** event for the playing field (game board).
	static gboolean onDrawPlayingFieldDrawingArea(GtkWidget*, cairo_t*, gpointer);
	/// Handles the **draw** event for the next tetromino field.
	static gboolean onDrawNextTetrominoDrawingArea(GtkWidget*, cairo_t*, gpointer);
	/// Draws gameplay's actual scene.
	static void drawScene(cairo_t*);
	/// Draws actual scene when game is paused.
	static void drawPausedScene(cairo_t*);
	/// Draws the next tetromino to be spawned.
	static void drawNextTetromino(cairo_t*);
	/// Draws game over animation.
	static void drawGameOverAnimationStep(cairo_t*);
	/// Controls animation execution.
	static gboolean animationController();
	/// Periodically changes game context.
	static gboolean updateGameContext();
	/// Handles update event's removal.
	static void contextHandler();
	/// Handles key binding.
	static gboolean onKeyPress(GtkWidget*, GdkEventKey*, gpointer);
	/// Modifies UI's functionality.
	static void setUIState(ui_state_t);
	/// Updates the score value shown.
	static void updateScore();
	/// Handles the **clicked** signal emitted by the Play button.
	static void onPlayClicked();
	/// Handles the **clicked** signal emitted by the Pause button.
	static void onPauseClicked();
	/// Handles the **clicked** signal emitted by the Reset button.
	static void onResetClicked();

};


#endif /* TETREESUI_H_ */
