/*
 * TetreesUI.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: Igor S. Buttarello
 */

/**
 * @file TetreesUI.cpp
 * @brief The source file containing the TetreesUI class' member functions implementation.
 */

#include <TetreesEngine.hpp>
#include <TetreesUI.hpp>
#include <TetreesUtils.hpp>
#include <fontconfig/fontconfig.h>
#include <TetreesStrings.hpp>

GtkWidget *mainWindow; 												/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkWindow.html">
																	GTK Window</a> widget which is the top level <a href="https://developer.gnome.org/gtk3/stable/GtkContainer.html">
																	container</a> to all other widgets within this application.*/
GtkWidget *playingFieldFrame; 										/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkFrame.html">GTK Frame</a>.
																	Just a secondary widget to properly arrange the @ref playingFieldDrawingArea "playing field"
																	within the @ref mainWindow "main window".*/
GtkWidget *nextTetrominoFrame; 										/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkFrame.html">GTK Frame</a>.
																	Just a secondary widget to properly arrange the @ref nextTetrominoDrawingArea "next tetromino field"
																	within the @ref mainWindow "main window".*/
GtkWidget *mainGrid; 												/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkGrid">GTK Grid</a> used to arrange children widgets in rows and
																	columns within the @ref mainWindow "main window".*/
GtkWidget *box01; 													/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkBox">GTK Box</a> used to pack widgets. Named this way because
 																	of its position within the @ref mainGrid "main grid", i.e row 0 and column 1.*/
GtkWidget *buttonBox; 												/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkButtonBox">GTK Button Box</a> to pack the buttons widgets
																	(@ref playButton "Play", @ref pauseButton "Pause", and @ref resetButton "Reset" buttons).*/
GtkWidget *playButton; 												/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkButton">GTK Button</a> widget that starts the game
																	when @ref TetreesUI::onPlayClicked "clicked".*/
GtkWidget *pauseButton; 											/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkButton">GTK Button</a> widget that pauses/resumes the game
																	when @ref TetreesUI::onPlayClicked "clicked" during gameplay.*/
GtkWidget *resetButton; 											/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkButton">GTK Button</a> widget that when @ref TetreesUI::onPlayClicked "clicked"
																	resets the game to its initial state.*/
GtkWidget *playingFieldDrawingArea; 								/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkDrawingArea.html">GTK Drawing Area</a>
																	widget used to display the actual state of the @ref TetreesEngine::gameBoard "game board"
																	(namely, a @ref TetreesEngine::scene "scene").*/
GtkWidget *scoreLabel; 												/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkLabel.html">GTK Label</a> that displays
																	the word "Score".*/
GtkWidget *scoreFrame; 												/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkFrame.html">GTK Frame</a> Just a secondary widget
																	to properly arrange the @ref scoreTextView "game score's displaying area" within the @ref mainWindow "main window".*/
GtkWidget *scoreTextView; 											/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkTextView.html">GTK Text View</a> that displays
																	stylized text informing the @ref TetreesUI::gameScore "game score".*/
GtkWidget *lvlLabel; 												/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkLabel.html">GTK Label</a> that displays
																	the actual @ref TetreesUI::gameLevel "game level".*/
GtkWidget *nextTetrominoDrawingArea; 								/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkDrawingArea.html">GTK Drawing Area</a>
																	widget used to show the @ref TetreesEngine::nextTetromino "next tetromino" to be spawned.*/

GtkTextBuffer 	*scoreTextBuf; 										/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkTextBuffer.html">GTK Text Buffer</a>
																	widget used as a buffer for the text displayed at the @ref scoreTextView "score Text View".*/
GtkTextIter		scoreIter; 											/**< A <a href="https://developer.gnome.org/gtk3/stable/GtkTextIter.html">GTK Text Iter</a>
																	widget used as an iterator for text operations within @ref scoreTextView "score Text View".*/

GdkDisplay *display; 												/**< A <a href="https://developer.gnome.org/gdk3/stable/GdkDisplay.html">GDK Display</a> used
																	for integration with CSS.*/
GdkScreen *screen; 													/**< A <a href="https://developer.gnome.org/gdk3/stable/GdkScreen.html">GDK Screen</a> used
																	for integration with CSS.*/
GtkCssProvider *cssProvider; 										/**< A <a href="https://developer.gnome.org/gdk3/stable/GdkCssProvider.html">GDK CSS Provider</a> which
							 	 	 	 	 	 	 	 	 	 	stores the CSS file itself.*/

guint updateEventID; 												/**< Stores the ID returned by
																	<a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#g-timeout-add-full">g_timeout_add_full</a>
																	when registering the @ref TetreesDefs.__UPDATE_EVENT__ "update event" to application's
																	<a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#glib-The-Main-Event-Loop.description">
																	main loop</a>.
																	@see @ref TetreesUI::onPlayClicked "onPlayClicked()" and @ref TetreesUI::contextHandler "contextHandler()".*/
std::string filePath_8bitFont = "Misc/PressStart2P.ttf"; 			/**< A string with path to the TTF file containing a 8-bit stylized font.
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 			@see The font used in this project is the <a href="https://www.dafont.com/press-start-2p.font">Press Start 2P</a>.*/

const FcChar8 *file = (const FcChar8*)filePath_8bitFont.c_str(); 	/**< A char type representation of the @ref filePath_8bitFont "TFF file path" to be used within
																	fontconfig.h functions.*/

TetreesEngine 		*TetreesUI::gameEngine;
TetreesUtils		*TetreesUI::utils;
Matrix2D<rgb_t> 	TetreesUI::colors;
Matrix2D<int> 		TetreesUI::gameScene;
piece_t				TetreesUI::nextTetromino;
game_score_t		TetreesUI::gameScore;
game_level_t		TetreesUI::gameLevel;
animation_t			TetreesUI::animationMode;
animation_control_t TetreesUI::animationControl;

/**
 * Creates an object of the TetreesUI class and initiates
 * its member variables.
 * @param engine The @ref TetreesEngine "game engine" to be used by this UI object.
 */
TetreesUI::TetreesUI(TetreesEngine engine)
{

	gameEngine = &engine;
	utils = new TetreesUtils();
	gameScore.score = 0;
	gameLevel.lvl = LVL_1;
	gameLevel.nextLvlScore = NEXT_LVL_SCORE_LVL1;
	gameLevel.gameSpeed = LVL_1_SPEED;
	animationMode = ANIMATION_NONE;
	animationControl.counter = 0;
	animationControl.iterator = 0;
	animationControl.drawRate = 0;
	animationControl.row = 0;
	animationControl.column = 0;
	colors.setMatrixDim(NUM_OF_COLORS);
	utils->startColorsArray(colors);
	updateEventID = 0;
	FcConfigAppFontAddFile(FcConfigGetCurrent(), file);

}

/**
 * A callback function that handles a <a href="https://developer.gnome.org/gtk3/stable/GtkWidget.html#GtkWidget-draw">
 * "draw event"</a> sent by GTK. When the @ref playingFieldDrawingArea
 * "playing field" is marked for redrawing, this function properly handles the "draw" signal
 * emitted accordingly to the @ref animationMode "animation" currently selected.
 * @param widget The object which received the signal.
 * @param cr The cairo drawing context.
 * @param user_data User data set when the signal handler was connected (NOT USED).
 * @return Always returns <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#G-SOURCE-REMOVE:CAPS">G_SOURCE_REMOVE</a>
 * (a macro for `FALSE`) assuring this function will only execute once.
 * @see Please, for further information see the <a href="https://developer.gnome.org/gtk3/stable/">GTK+ 3 reference manual</a>
 * and the <a href="https://www.cairographics.org/manual/">Cairo Graphics manual</a>.
 */
gboolean TetreesUI::onDrawPlayingFieldDrawingArea(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{

	switch(animationMode){

	case ANIMATION_NONE:
		updateScore();
		drawScene(cr);
		break;

	case ANIMATION_GAME_OVER:
		drawGameOverAnimationStep(cr);
		break;

	case ANIMATION_GAME_PAUSED:
		drawPausedScene(cr);
		break;

	}

	return G_SOURCE_REMOVE;

}

/**
 * A callback function that handles a <a href="https://developer.gnome.org/gtk3/stable/GtkWidget.html#GtkWidget-draw">
 * "draw event"</a> sent by GTK. When the @ref nextTetrominoDrawingArea
 * "next tetromino field" is marked for redrawing, this function properly handles the draw signal
 * emitted.
 * @param widget The <a href="https://developer.gnome.org/gtk3/stable/GtkWidget.html">GTK Widget</a> which received the signal.
 * @param cr The <a href="https://www.cairographics.org/manual/cairo-cairo-t.html">cairo drawing context</a>.
 * @param user_data User data set when the signal handler was connected (NOT USED).
 * @return Always returns <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#G-SOURCE-REMOVE:CAPS">G_SOURCE_REMOVE</a>
 * (a macro for `FALSE`) assuring this function will only execute once.
 * @see Please, for further information see the <a href="https://developer.gnome.org/gtk3/stable/">GTK+ 3 reference manual</a>
 * and the <a href="https://www.cairographics.org/manual/">Cairo Graphics manual</a>.
 */
gboolean TetreesUI::onDrawNextTetrominoDrawingArea(
		GtkWidget *widget,
		cairo_t *cr,
		gpointer user_data)
{

	drawNextTetromino(cr);
	return G_SOURCE_REMOVE;

}

/**
 * Draws the actual gameplay's @ref gameScene "scene" inside the @ref playingFieldDrawingArea "playing field drawing area".
 * In other words, this functions is responsible for drawing a frame containing all the already blocked tetrominoes at the
 * game board as well as the falling tetromino in the position that it founds itself by the time of the drawing.
 *
 * The drawing of a @ref gameScene "scene" occurs every time a @ref TetreesEngine::gameStep "game step"
 * is issued making the game context changes somehow.
 * @see @ref updateGameContext and @ref onKeyPress "onKeyPress".
 * @param cr The <a href="https://www.cairographics.org/manual/cairo-cairo-t.html">cairo drawing context</a> in which the scene will be drawn.
 */
void TetreesUI::drawScene(cairo_t* cr)
{

	gameScene = gameEngine->getScene();
	color_t cellColor;

	cairo_set_source_rgba(
			cr,
			colors(COLOR_BACKGROUND).r,
			colors(COLOR_BACKGROUND).g,
			colors(COLOR_BACKGROUND).b,
			0.9
	);
	cairo_paint(cr);
	cairo_set_line_width(cr, TETROMINO_LINE_WIDTH);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL);

	for(unsigned r = LIMIT_TOP_BORDER; r <= LIMIT_BOTTOM_BORDER; r++){
		for(unsigned c = LIMIT_LEFT_BORDER; c <= LIMIT_RIGHT_BORDER; c++){
			if(gameScene(r,c) != COLOR_TRANSPARENT){
				cairo_set_source_rgb(cr, 0, 0, 0);
				cairo_rectangle(
						cr,
						SCENE_X(c),
						SCENE_Y(r),
						(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2)),
						(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2))
				);
				cairo_stroke_preserve(cr);
				cellColor = utils->getColor(gameScene(r,c));
				cairo_set_source_rgb(
						cr,
						colors(cellColor).r,
						colors(cellColor).g,
						colors(cellColor).b
				);
				cairo_fill(cr);
			}
		}
	}

}

/**
 * When the player pauses the game, the game's current state changes to @ref game_state_t.GAME_STATE_PAUSED "GAME PAUSED" and
 * also the @ref animationMode "animation mode" changes to @ref animation_t.ANIMATION_GAME_PAUSED "GAME PAUSED ANIMATION"
 * which leads the @ref onDrawPlayingFieldDrawingArea member function to call this function instead of the
 * commonly called @ref drawScene.
 *
 * A paused scene is nothing but the most recently @ref TetreesEngine::scene "scene" produced by the @ref gameEngine
 * "game's engine" draw in grey scale plus a @ref TetreesStrings.MSG_GAME_PAUSED "Game Paused message"
 * wrote atop the drawn scene.
 * @param cr The <a href="https://www.cairographics.org/manual/cairo-cairo-t.html">cairo drawing context</a> in which the scene will be drawn.
 */
void TetreesUI::drawPausedScene(cairo_t *cr)
{

	gameScene = gameEngine->getScene();

	cairo_set_source_rgba(
			cr,
			colors(COLOR_BACKGROUND).r,
			colors(COLOR_BACKGROUND).g,
			colors(COLOR_BACKGROUND).b,
			0.9
	);
	cairo_paint(cr);
	cairo_set_line_width(cr, TETROMINO_LINE_WIDTH);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL);

	for(unsigned r = LIMIT_TOP_BORDER; r <= LIMIT_BOTTOM_BORDER; r++){
		for(unsigned c = LIMIT_LEFT_BORDER; c <= LIMIT_RIGHT_BORDER; c++){
			if(gameScene(r,c) != COLOR_TRANSPARENT){
				cairo_set_source_rgb(cr, 0, 0, 0);
				cairo_rectangle(
						cr,
						SCENE_X(c),
						SCENE_Y(r),
						(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2)),
						(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2))
				);
				cairo_stroke_preserve(cr);
				cairo_set_source_rgb(
						cr,
						colors(COLOR_GAME_PAUSED).r,
						colors(COLOR_GAME_PAUSED).g,
						colors(COLOR_GAME_PAUSED).b
				);
				cairo_fill(cr);
			}
		}
	}

	//Shows "GAME PAUSED" message on the screen
	//===================================================
	cairo_set_source_rgb(cr, 1, 0, 0);
	cairo_select_font_face(
			cr,
			"PressStart2P",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_NORMAL
	);

	//Empirically obtained formula for font dimension
	//and text position
	//-----------------------------------------------
	double fontSize = ((32 * BLOCK_SIDE_DIM) / 40);
	cairo_set_font_size(cr, fontSize);
	cairo_move_to(
			cr,
			(BLOCK_SIDE_DIM / 2),
			((PLAY_FIELD_HEIGHT * BLOCK_SIDE_DIM) / 2)
	);
	//-----------------------------------------------

	cairo_show_text(cr, MSG_GAME_PAUSED);
	//===================================================
}

/**
 * Draws @ref nextTetromino "the next tetromino" inside @ref nextTetrominoDrawingArea "next tetromino's drawing area".
 * Whenever a call to <a href="https://developer.gnome.org/gtk3/stable/GtkWidget.html#gtk-widget-queue-draw">
 * gtk_widget_queue_draw</a> is made, this function checks if the game is currently in a @ref game_state_t.GAME_STATE_PLAYING
 * "playing" or @ref game_state_t.GAME_STATE_PAUSED "paused" state and, if so, draws the @ref nextTetromino "next tetromino"
 * inside @ref nextTetrominoDrawingArea "next tetromino's drawing area".
 * @param cr The <a href="https://www.cairographics.org/manual/cairo-cairo-t.html">cairo drawing context</a> in which the @ref
 * nextTetromino "next tetromino" will be drawn.
 */
void TetreesUI::drawNextTetromino(cairo_t *cr)
{

	color_t cellColor;
	unsigned matrixDim;
	const unsigned dim4x4 = 16;
	const unsigned dim3x3 = 9;
	const unsigned dim2x2 = 4;

	cairo_set_source_rgba(
			cr,
			colors(COLOR_BACKGROUND).r,
			colors(COLOR_BACKGROUND).g,
			colors(COLOR_BACKGROUND).b,
			0.9
	);
	cairo_paint(cr);
	cairo_set_line_width(cr, TETROMINO_LINE_WIDTH);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL);

	if(
			gameEngine->getGameState() == GAME_STATE_PLAYING
			||
			gameEngine->getGameState() == GAME_STATE_PAUSED
	){

		if(gameEngine->getNextTetromino().type != nextTetromino.type)
		nextTetromino = gameEngine->getNextTetromino();
		matrixDim = (nextTetromino.shape.getRowHeight()
						* nextTetromino.shape.getColWidth());

		//In order to center next tetromino's shape within next tetromino
		//drawing area, the drawing method diverge for each tetromino type
		//based on its shape dimension
		for(unsigned r = 0; r < nextTetromino.shape.getRowHeight(); r++){
			for(unsigned c = 0; c < nextTetromino.shape.getColWidth(); c++){
				if(nextTetromino.shape(r,c) != EMPTY){
					cairo_set_source_rgb(cr, 0, 0, 0);
					switch(matrixDim){
					case dim4x4: //That would be the I-shaped tetromino
						cairo_rectangle(
								cr,
								(NEXT_TETROMINO_X(c) + BLOCK_SIDE_DIM),
								(NEXT_TETROMINO_Y(r) + (BLOCK_SIDE_DIM / 2)),
								(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2)),
								(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2))
						);
						break;
					case dim2x2: //That would be the O-shaped tetromino
						cairo_rectangle(
								cr,
								(NEXT_TETROMINO_X(c) + (2 * BLOCK_SIDE_DIM)),
								(NEXT_TETROMINO_Y(r) + BLOCK_SIDE_DIM),
								(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2)),
								(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2))
						);
						break;
					case dim3x3: //That would be all others tetrominos
						cairo_rectangle(
								cr,
								((NEXT_TETROMINO_X(c) + BLOCK_SIDE_DIM) + (BLOCK_SIDE_DIM / 2)),
								(NEXT_TETROMINO_Y(r) + BLOCK_SIDE_DIM),
								(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2)),
								(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2))
						);
						break;
					}
					cairo_stroke_preserve(cr);
					animationMode == ANIMATION_GAME_PAUSED?
							cellColor = COLOR_GAME_PAUSED
							: cellColor = nextTetromino.color;
					cairo_set_source_rgb(
							cr,
							colors(cellColor).r,
							colors(cellColor).g,
							colors(cellColor).b
					);
					cairo_fill(cr);
				}
			}
		}
	}

}

/**
 * This function is responsible for executing each of the several drawing steps needed to
 * accomplish the game over animation. As the game over condition is reached -
 * which occurs when the @ref TetreesEngine::step_BlockSpawnedTetromino "blocked tetrominoes"
 * pile reaches the @ref limit_t.LIMIT_TOP_BORDER "top border limit" of the
 * @ref TetreesEngine::gameBoard "game board" - the game's state changes to a
 * @ref game_state_t.GAME_STATE_GAME_OVER "game over state" and the @ref updateGameContext
 * function configures the @ref animationControl variable with data necessary to execute
 * the game over animation. Thus, the drawing control is passed to the @ref animationController
 * member function which plays a role similar to @ref updateGameContext but is called
 * by the @ref TetreesDefs.__ANIMATION_EVENT__ "animation event" instead.
 * @param cr The <a href="https://www.cairographics.org/manual/cairo-cairo-t.html">cairo drawing context</a> where animation
 * steps will be drawn.
 */
void TetreesUI::drawGameOverAnimationStep(cairo_t *cr)
{

	unsigned row,col;
	row = animationControl.row;
	col = animationControl.column;

	gameScene(row,col) = COLOR_GAME_OVER;

	cairo_set_source_rgba(
			cr,
			colors(COLOR_BACKGROUND).r,
			colors(COLOR_BACKGROUND).g,
			colors(COLOR_BACKGROUND).b,
			0.9
	);
	cairo_paint(cr);
	cairo_set_line_width(cr, TETROMINO_LINE_WIDTH);
	cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL);

	for(unsigned r = LIMIT_TOP_BORDER; r <= LIMIT_BOTTOM_BORDER; r++){
		for(unsigned c = LIMIT_LEFT_BORDER; c <= LIMIT_RIGHT_BORDER; c++){
			if(gameScene(r,c) != COLOR_TRANSPARENT){
				cairo_set_source_rgb(cr, 0, 0, 0);
				cairo_rectangle(cr,
						SCENE_X(c),
						SCENE_Y(r),
						(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2)),
						(BLOCK_SIDE_DIM - (TETROMINO_LINE_WIDTH / 2))
				);
				cairo_stroke_preserve(cr);
				cairo_set_source_rgba(
						cr,
						colors(COLOR_GAME_OVER).r,
						colors(COLOR_GAME_OVER).g,
						colors(COLOR_GAME_OVER).b,
						0.9
				);
				cairo_fill(cr);
			}
		}
	}

	//Shows "GAME OVER" message on the screen
	//===================================================
	if(animationControl.iterator <= 0){
		cairo_set_source_rgb(cr, 1, 0, 0);
		cairo_select_font_face(
				cr,
				"PressStart2P",
				CAIRO_FONT_SLANT_NORMAL,
				CAIRO_FONT_WEIGHT_BOLD
		);

		//Empirically obtained formula for font dimension
		//and text position
		//-----------------------------------------------
		double fontSize = ((36 * BLOCK_SIDE_DIM) / 40);
		cairo_set_font_size(cr, fontSize);
		cairo_move_to(
				cr,
				(BLOCK_SIDE_DIM / 2),
				((PLAY_FIELD_HEIGHT * BLOCK_SIDE_DIM) / 2)
		);
		//-----------------------------------------------

		cairo_show_text(cr, MSG_GAME_OVER);
	}
	//===================================================

}

/**
 * This function controls the execution of each step composing an animation. When the
 * @ref animationMode variable is set to some @ref animation_t "type of animation", this function uses
 * the @animationControl variable and iteratively command the screen to be redrawn accordingly the
 * data stored by it.
 * @note
 * This is the member function registered within the @ref TetreesDefs.__ANIMATION_EVENT__
 * "animation event".
 * @return Always returns <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#G-SOURCE-CONTINUE:CAPS">G_SOURCE_CONTINUE</a>
 * since the event which this function is attached to is permanently executed.
 */
gboolean TetreesUI::animationController()
{

	switch(animationMode){

	case ANIMATION_GAME_OVER:
		if(
				(animationControl.iterator)
				&&
				(animationControl.counter % animationControl.drawRate == 0)
		){
			gtk_widget_queue_draw(mainWindow);
			--animationControl.iterator;
			--animationControl.column;
			if(animationControl.column == 0){
				--animationControl.row;
				animationControl.column = LIMIT_RIGHT_BORDER;
			}
		}
		++animationControl.counter;
		break;

	default:
		break;
	}

	return G_SOURCE_CONTINUE;

}

/**
 * With a central role, this function updates game's context based on the @ref game_state_t "state" the game finds itself.
 *
 * This function is periodically called by the @ref TetreesDefs.__UPDATE_EVENT__ "update event"
 * in order to redraw the user interface as well as updating game's context by @ref TetreesEngine::gameStep "issuing" a
 * @ref step_t.STEP_DROP "DROP STEP" to @ref gameEngine "game's engine". This step is automatically issued as
 * part of the gameplay and its @ref level_speed_t "periodicity" varies accordingly to the actual @ref gameLevel "game level".
 *
 * Another role of this function is to detect changes at @ref game_state_t "game's state" and feed
 * the @ref animationControl variable with correct data so the actual animation can execute properly.
 * @note
 * This is the member function registered within the @ref TetreesDefs.__UPDATE_EVENT__
 * "update event".
 * @return <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#G-SOURCE-CONTINUE:CAPS">G_SOURCE_CONTINUE</a>
 * when the @ref TetreesDefs.__UPDATE_EVENT__ "update event" is still attached to the
 * <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#glib-The-Main-Event-Loop.description">main loop</a>.
 * <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#G-SOURCE-REMOVE:CAPS">G_SOURCE_REMOVE</a> otherwise.
 */
gboolean TetreesUI::updateGameContext()
{

	gboolean ret = G_SOURCE_CONTINUE;

	switch(gameEngine->getGameState()){

	case GAME_STATE_PLAYING:
		gameEngine->gameStep(STEP_DROP);
		//A level increase may result in a faster game speed,
		//so checking and updating game level variable is necessary.
		if(gameEngine->getGameLevel().lvl != gameLevel.lvl){
			gameLevel = gameEngine->getGameLevel();
			g_source_remove(updateEventID);
			ret = G_SOURCE_REMOVE;
		}
		gtk_widget_queue_draw(mainWindow);
		break;

	case GAME_STATE_PAUSED:
		break;

	case GAME_STATE_NOT_STARTED:
		ret = G_SOURCE_REMOVE;
		break;

	case GAME_STATE_GAME_OVER:
		ret = G_SOURCE_REMOVE;
		setUIState(UI_STATE_NOT_STARTED);
		gtk_widget_queue_draw(mainWindow);
		animationMode = ANIMATION_GAME_OVER;
		animationControl.counter = 0;
		animationControl.iterator = 200;
		animationControl.drawRate = 1;
		animationControl.row = LIMIT_BOTTOM_BORDER;
		animationControl.column = LIMIT_RIGHT_BORDER;
		break;

	}

	return ret;

}

/**
 * This function will be executed when a call to <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#g-source-remove">
 * g_source_remove</a>@ref updateEventID "(updateEventID)" is made in order to unregister (detach) the @ref TetreesDefs.__UPDATE_EVENT__ "update event"
 * from application's <a href="https://developer.gnome.org/programming-guidelines/stable/main-contexts.html.en">main context</a>.
 *
 * In case the game is currently in a @ref game_state_t.GAME_STATE_PLAYING "playing state" by the time this function is called,
 * it will register again the @ref TetreesDefs.__UPDATE_EVENT__ "update event" within the
 * <a href="https://developer.gnome.org/programming-guidelines/stable/main-contexts.html.en">main context</a> by
 * using a new interval (if that is the case) to call @ref updateGameContext().
 * @see <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#g-timeout-add-full">g_timeout_add_full</a>
 */
void TetreesUI::contextHandler()
{

	updateEventID = 0;

	switch(gameEngine->getGameState()){
	case GAME_STATE_PLAYING:
		updateEventID = g_timeout_add_full(
				G_PRIORITY_HIGH,
				gameLevel.gameSpeed,
				(GSourceFunc)updateGameContext,
				nullptr,
				(GDestroyNotify)contextHandler
		);
		break;
	default:
		break;
	}

}

/**
 * This is the callback function to a <a href="https://developer.gnome.org/gtk3/stable/GtkWidget.html#GtkWidget-key-press-event">
 * key press event</a> within the @ref mainWindow "main window" scope. It maps each key to its respective action during gameplay.
 * @param widget The object which received the signal (not used).
 * @param event The <a href="https://developer.gnome.org/gdk3/stable/gdk3-Event-Structures.html#GdkEventKey">GdkEventKey</a>
 * which triggered this signal (the key itself).
 * @param user_data User data set when the signal handler was connected (not used).
 * @return Always returns <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#G-SOURCE-REMOVE:CAPS">
 * G_SOURCE_REMOVE</a> since this is a one time only event.
 */
gboolean TetreesUI::onKeyPress(GtkWidget 	*widget,
							   GdkEventKey 	*event,
							   gpointer 	user_data)
{

	if(gameEngine->getGameState() == GAME_STATE_PLAYING){
		switch(event->keyval){
		case GDK_KEY_Right:
			gameEngine->gameStep(STEP_SWERVE_RIGHT);
			break;

		case GDK_KEY_Left:
			gameEngine->gameStep(STEP_SWERVE_LEFT);
			break;

		case GDK_KEY_Up:
			gameEngine->gameStep(STEP_ROTATE);
			break;

		case GDK_KEY_Down:
			gameEngine->gameStep(STEP_DROP);
			break;
		}

		gtk_widget_queue_draw(mainWindow);
	}

	return G_SOURCE_REMOVE;

}

/**
 * Sets user interface to a specific state depending on what is happening into the game that moment.
 * This function basically enables and disables interface buttons accordingly to the @ref ui_state_t
 * "UI state" passed as parameter.
 * @param uiState The @ref ui_state_t "UI state" in which user interface must be set to.
 */
void TetreesUI::setUIState(ui_state_t uiState)
{

	switch(uiState){
	case UI_STATE_NOT_STARTED:
		gtk_button_set_label(GTK_BUTTON(pauseButton), LABEL_PAUSE_BUTTON);
		gtk_widget_set_sensitive(GTK_WIDGET(playButton), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(pauseButton), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(resetButton), FALSE);
		break;

	case UI_STATE_PAUSED:
		if(gameEngine->getGameState() == GAME_STATE_PLAYING)
			gtk_button_set_label(GTK_BUTTON(pauseButton), LABEL_CONTINUE_BUTTON);
		else gtk_button_set_label(GTK_BUTTON(pauseButton), LABEL_PAUSE_BUTTON);
		break;

	case UI_STATE_PLAYING:
		gtk_widget_set_sensitive(GTK_WIDGET(playButton), FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(pauseButton), TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(resetButton), TRUE);
		break;
	}

}

/**
 * Updates the value displayed at the @ref scoreTextView "score text view". This function
 * fetches @ref gameEngine "game engine" for the updated score and updates the value
 * shown inside @ref scoreTextView "score text view".
 */
void TetreesUI::updateScore()
{

	gameScore.score = gameEngine->getGameScore().score;

	std::string scoreArray = utils->intToStr(gameScore.score);
	GtkTextIter		beginIter;
	GtkTextIter		endIter;

	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(scoreTextBuf), &beginIter, &endIter);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(scoreTextBuf), &beginIter, &endIter);
	gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(scoreTextBuf), &scoreIter, 0);
	gtk_text_buffer_insert_with_tags_by_name(GTK_TEXT_BUFFER(scoreTextBuf),
											 &scoreIter,
											 scoreArray.c_str(),
											 -1,
											 "abovePixelsTag",
											 "centreJustifyTag",
											 "fontTag",
											 "fgColorTag",
											 nullptr
	);

	std::string lvlNum = utils->intToStr(gameLevel.lvl + 1);
	std::string lvlStr = LABEL_LVL_LABEL;
	lvlStr.append(lvlNum);
	gtk_label_set_text(GTK_LABEL(lvlLabel), lvlStr.c_str());

}

/**
 * This is a callback function that handles the <a href="https://developer.gnome.org/gtk3/stable/GtkButton.html#GtkButton-clicked">
 * clicked signal</a> emitted by the @ref playButton "Play button" when it's clicked. A @ref cmd_t.CMD_PLAY "play command"
 * is @ref TetreesEngine::issueCommand "issued" to @ref gameEngine "game engine" and the game begins.
 */
void TetreesUI::onPlayClicked()
{

	if(
			gameEngine->getGameState() == GAME_STATE_NOT_STARTED
			||
			gameEngine->getGameState() == GAME_STATE_GAME_OVER

	){

		if(!updateEventID)
			updateEventID = g_timeout_add_full(
					G_PRIORITY_HIGH,
					gameLevel.gameSpeed,
					(GSourceFunc)updateGameContext,
					nullptr,
					(GDestroyNotify)contextHandler
			);

		animationMode = ANIMATION_NONE;
		gameEngine->issueCommand(CMD_PLAY);
		setUIState(UI_STATE_PLAYING);
		gtk_widget_queue_draw(mainWindow);

	}

}

/**
 * This is a callback function that handles the <a href="https://developer.gnome.org/gtk3/stable/GtkButton.html#GtkButton-clicked">
 * clicked signal</a> emitted by the @ref pauseButton "Pause button" when it's clicked. A @ref cmd_t.CMD_PAUSE "pause command"
 * is @ref TetreesEngine::issueCommand "issued" to @ref gameEngine "game engine" and the game is then paused.
 * @note
 * If the game is already paused, the @ref cmd_t.CMD_PAUSE "pause command" resumes the game instead.
 */
void TetreesUI::onPauseClicked()
{

	setUIState(UI_STATE_PAUSED);
	gameEngine->issueCommand(CMD_PAUSE);
	if(animationMode != ANIMATION_GAME_PAUSED)
		animationMode = ANIMATION_GAME_PAUSED;
	else
		animationMode = ANIMATION_NONE;
	gtk_widget_queue_draw(mainWindow);

}

/**
 * This is a callback function that handles the <a href="https://developer.gnome.org/gtk3/stable/GtkButton.html#GtkButton-clicked">
 * clicked signal</a> emitted by the @ref resetButton "Reset button" when it's clicked. A @ref cmd_t.CMD_RESET "reset command"
 * is @ref TetreesEngine::issueCommand "issued" to @ref gameEngine "game engine" and the game is set back to its initial state, i.e.
 * the @ref gameScore "score" and @ref TetreesEngine::gameBoard "game board" are cleared.
 */
void TetreesUI::onResetClicked()
{

	gameEngine->issueCommand(CMD_RESET);
	setUIState(UI_STATE_NOT_STARTED);
	animationMode = ANIMATION_NONE;
	gtk_widget_queue_draw(mainWindow);

}

/**
 * Create and put together all UI elements. By using the <a href="https://developer.gnome.org/gtk3/stable/">GTK+ 3 toolkit</a>,
 * this function creates several <a href="https://developer.gnome.org/gtk3/stable/GtkWidget.html">GTK Widgets</a>
 * from different types and hierarchically arrange them in order to provide a graphical interface for the user
 * (player) to interact with.
 * @param app The <a href="https://developer.gnome.org/gtk3/stable/GtkApplication.html">GTK Application</a> object reference.
 * @param user_data User data (`**argv`) received passed to the application when it is executed (not used in this application).
 */
void TetreesUI::setupUI(GtkApplication *app, gpointer user_data)
{

	//============
	//MAIN WINDOW
	//============
	//***************************************************************************************
	// Setup the main window widget, which is the top container for
	//all other widgets
	mainWindow = gtk_application_window_new(app);
	gtk_widget_set_name(GTK_WIDGET(mainWindow), CSS_NAME_MAIN_WINDOW);
	gtk_window_set_title(GTK_WINDOW(mainWindow), LABEL_TITLE);
	gtk_window_set_resizable(GTK_WINDOW(mainWindow), FALSE);
	gtk_window_set_position(GTK_WINDOW(mainWindow),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(mainWindow), 50);

	gtk_widget_add_events(mainWindow, GDK_KEY_PRESS_MASK);

	g_signal_connect(mainWindow, "key-press-event", G_CALLBACK(onKeyPress), nullptr);
	//**************************************************************************************

	//==========
	//MAIN GRID
	//==========
	//***************************************************************************************
	mainGrid = gtk_grid_new();
	gtk_widget_set_name(GTK_WIDGET(mainGrid), CSS_NAME_MAIN_GRID);
	gtk_container_add(GTK_CONTAINER(mainWindow), mainGrid);
	//***************************************************************************************

	//====================
	//PLAYING FIELD FRAME
	//====================
	//***************************************************************************************
	playingFieldFrame = gtk_frame_new(nullptr);
	gtk_widget_set_name(
			GTK_WIDGET(playingFieldFrame),
			CSS_NAME_PLAYING_FIELD_FRAME
	);
	gtk_grid_attach(GTK_GRID(mainGrid), playingFieldFrame, 0, 0, 1, 4);
	//***************************************************************************************

	//===========================
	//PLAYING FIELD DRAWING AREA
	//===========================
	//***************************************************************************************
	playingFieldDrawingArea = gtk_drawing_area_new();
	gtk_widget_set_name(
			GTK_WIDGET(playingFieldDrawingArea),
			CSS_NAME_PLAYING_FIELD_DRAWING_AREA
	);
	gtk_widget_set_size_request(
			playingFieldDrawingArea,
			PLAY_FIELD_PIXEL_WIDTH,
			PLAY_FIELD_PIXEL_HEIGHT
	);
	gtk_container_add(
			GTK_CONTAINER(playingFieldFrame),
			playingFieldDrawingArea
	);
	g_signal_connect(
			playingFieldDrawingArea,
			"draw",
			G_CALLBACK(onDrawPlayingFieldDrawingArea),
			nullptr
	);

	g_timeout_add(ANIMATION_EVENT_INTERVAL,(GSourceFunc)animationController, nullptr);
	//***************************************************************************************

	//=======
	//BOX 01
	//=======
	//***************************************************************************************
	box01 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous(GTK_BOX(box01), FALSE);
	gtk_widget_set_margin_start(GTK_WIDGET(box01), 5);
	gtk_grid_attach(GTK_GRID(mainGrid), box01, 1, 0, 1, 1);
	//***************************************************************************************

	//=====================
	//NEXT TETROMINO FRAME
	//=====================
	//***************************************************************************************
	nextTetrominoFrame = gtk_frame_new(nullptr);
	gtk_widget_set_name(
			GTK_WIDGET(nextTetrominoFrame),
			CSS_NAME_NEXT_TETROMINO_FRAME
	);
	gtk_widget_set_size_request(
			GTK_WIDGET(nextTetrominoFrame),
			NEXT_TETROMINO_FIELD_CELL_WIDTH,
			NEXT_TETROMINO_FIELD_CELL_HEIGHT
	);
	gtk_box_pack_start(
			GTK_BOX(box01),
			nextTetrominoFrame,
			FALSE,
			FALSE,
			0
	);
	//***************************************************************************************

	//============================
	//NEXT TETROMINO DRAWING AREA
	//============================
	//***************************************************************************************
	nextTetrominoDrawingArea = gtk_drawing_area_new();
	gtk_widget_set_name(
			GTK_WIDGET(nextTetrominoDrawingArea),
			CSS_NAME_NEXT_TETROMINO_DRAWING_AREA
	);
	gtk_container_add(
			GTK_CONTAINER(nextTetrominoFrame),
			nextTetrominoDrawingArea
	);

	g_signal_connect(
			nextTetrominoDrawingArea,
			"draw",
			G_CALLBACK(onDrawNextTetrominoDrawingArea),
			nullptr
	);
	//***************************************************************************************

	//============
	//SCORE LABEL
	//============
	//***************************************************************************************
	scoreLabel = gtk_label_new(LABEL_SCORE_LABEL);
	gtk_widget_set_name(
			GTK_WIDGET(scoreLabel),
			CSS_NAME_SCORE_LABEL
	);
	//***************************************************************************************

	//============
	//SCORE FRAME
	//============
	//***************************************************************************************
	scoreFrame = gtk_frame_new(nullptr);
	gtk_widget_set_name(
			GTK_WIDGET(scoreFrame),
			CSS_NAME_SCORE_FRAME
	);
	gtk_frame_set_label_widget(
			GTK_FRAME(scoreFrame),
			scoreLabel
	);
	gtk_frame_set_label_align(
			GTK_FRAME(scoreFrame),
			0.5,
			1
	);
	gtk_widget_set_valign(
			GTK_WIDGET(scoreFrame),
			GTK_ALIGN_START
	);
	gtk_widget_set_size_request(
			GTK_WIDGET(scoreFrame),
			50,
			80
	);
	gtk_widget_set_margin_top(
			GTK_WIDGET(scoreFrame),
			20
	);
	gtk_box_pack_start(
			GTK_BOX(box01),
			scoreFrame,
			FALSE,
			FALSE,
			0
	);
	//***************************************************************************************

	//================
	//SCORE TEXT VIEW
	//================
	//***************************************************************************************
	scoreTextView = gtk_text_view_new();
	gtk_widget_set_name(
			GTK_WIDGET(scoreTextView),
			CSS_NAME_SCORE_TEXT_VIEW
	);
	gtk_widget_set_margin_top(GTK_WIDGET(scoreTextView), 3);
	gtk_widget_set_margin_bottom(GTK_WIDGET(scoreTextView), 3);
	gtk_widget_set_margin_start(GTK_WIDGET(scoreTextView), 3);
	gtk_widget_set_margin_end(GTK_WIDGET(scoreTextView), 3);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(scoreTextView), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(scoreTextView), FALSE);

	scoreTextBuf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(scoreTextView));
	gtk_text_buffer_create_tag(
			GTK_TEXT_BUFFER(scoreTextBuf),
			"centreJustifyTag",
			"justification",
			GTK_JUSTIFY_CENTER,
			nullptr
	);
	gtk_text_buffer_create_tag(
			GTK_TEXT_BUFFER(scoreTextBuf),
			"fontTag",
			"font",
			"PressStart2P 24",
			nullptr
	);
	gtk_text_buffer_create_tag(
			GTK_TEXT_BUFFER(scoreTextBuf),
			"fgColorTag",
			"foreground",
			"green",
			nullptr
	);
	gtk_text_buffer_create_tag(
			GTK_TEXT_BUFFER(scoreTextBuf),
			"abovePixelsTag",
			"pixels-above-lines",
			12,
			nullptr
	);
	gtk_container_add(GTK_CONTAINER(scoreFrame), scoreTextView);
	//***************************************************************************************

	//============
	//LEVEL LABEL
	//============
	//***************************************************************************************
	lvlLabel = gtk_label_new(LABEL_LVL_LABEL);
	gtk_widget_set_name(GTK_WIDGET(lvlLabel), CSS_NAME_LVL_LABEL);
	gtk_widget_set_halign(GTK_WIDGET(lvlLabel), GTK_ALIGN_CENTER);
	gtk_widget_set_margin_top(GTK_WIDGET(lvlLabel), 100);
	gtk_box_pack_start(
			GTK_BOX(box01),
			GTK_WIDGET(lvlLabel),
			FALSE,
			FALSE,
			0
	);
	//***************************************************************************************

	//============
	//PLAY BUTTON
	//============
	//***************************************************************************************
	playButton = gtk_button_new_with_label(LABEL_PLAY_BUTTON);
	gtk_widget_set_name(GTK_WIDGET(playButton), CSS_NAME_PLAY_BUTTON);
	gtk_widget_set_size_request(GTK_WIDGET(playButton), 130, 0);

	g_signal_connect(
			playButton,
			"clicked",
			G_CALLBACK(onPlayClicked),
			nullptr
	);
	//***************************************************************************************

	//=============
	//PAUSE BUTTON
	//=============
	//***************************************************************************************
	pauseButton = gtk_button_new_with_label(LABEL_PAUSE_BUTTON);
	gtk_widget_set_name(GTK_WIDGET(pauseButton), CSS_NAME_PAUSE_BUTTON);
	gtk_widget_set_size_request(GTK_WIDGET(pauseButton), 130, 0);
	gtk_widget_set_sensitive(GTK_WIDGET(pauseButton), FALSE);

	g_signal_connect(
			pauseButton,
			"clicked",
			G_CALLBACK(onPauseClicked),
			nullptr
	);
	//***************************************************************************************

	//=============
	//RESET BUTTON
	//=============
	//***************************************************************************************
	resetButton = gtk_button_new_with_label(LABEL_RESET_BUTTON);
	gtk_widget_set_name(GTK_WIDGET(resetButton), CSS_NAME_RESET_BUTTON);
	gtk_widget_set_size_request(GTK_WIDGET(resetButton), 130, 0);
	gtk_widget_set_sensitive(GTK_WIDGET(resetButton), FALSE);

	g_signal_connect(
			resetButton,
			"clicked",
			G_CALLBACK(onResetClicked),
			nullptr
	);
	//***************************************************************************************

	//===========
	//BUTTON BOX
	//===========
	//***************************************************************************************
	buttonBox = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
	gtk_box_pack_start(GTK_BOX(buttonBox), playButton, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(buttonBox), pauseButton, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(buttonBox), resetButton, FALSE, FALSE, 0);
	gtk_box_set_spacing(GTK_BOX(buttonBox), 10);
	gtk_widget_set_margin_top(GTK_WIDGET(buttonBox), 150);
	gtk_box_pack_start(GTK_BOX(box01), buttonBox, FALSE, FALSE, 0);
	//***************************************************************************************

	//=============
	//CSS PROVIDER
	//=============
	//***************************************************************************************
	cssProvider = gtk_css_provider_new();
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);
	gtk_style_context_add_provider_for_screen(
			screen,
			GTK_STYLE_PROVIDER(cssProvider),
			GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
	);

	gtk_css_provider_load_from_file(
			cssProvider,
			g_file_new_for_path(CSS_FILE_PATH),
			nullptr
	);
	//***************************************************************************************

	gtk_widget_show_all(mainWindow);

}

