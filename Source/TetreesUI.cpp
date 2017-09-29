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

cairo_surface_t *playingSurface;

GtkWidget *mainWindow;
GtkWidget *playingFieldFrame;
GtkWidget *nextTetrominoFrame;
GtkWidget *mainGrid;
GtkWidget *box01;	//row 0, column 1 of mainGrid
GtkWidget *buttonBox;
GtkWidget *playButton;
GtkWidget *pauseButton;
GtkWidget *resetButton;
GtkWidget *playingFieldDrawingArea;
GtkWidget *scoreLabel;
GtkWidget *scoreFrame;
GtkWidget *scoreTextView;
GtkWidget *lvlLabel;
GtkWidget *nextTetrominoDrawingArea; /**< Shows the @ref TetreesEngine::nextTetromino "next tetromino" to be spawned.*/

GtkTextBuffer 	*scoreTextBuf;
GtkTextIter		scoreIter;

GdkDisplay *display;
GdkScreen *screen;
GtkCssProvider *cssProvider;

guint updateEventID;
std::string filePath_8bitFont = "Misc/PressStart2P.ttf";
const FcChar8 *file = (const FcChar8*)filePath_8bitFont.c_str();

TetreesEngine 		*TetreesUI::gameEngine;
TetreesUtils		*TetreesUI::utils;
Matrix2D<rgb_t> 	TetreesUI::colors;
Matrix2D<int> 		TetreesUI::gameScene;
piece_t				TetreesUI::nextTetromino;
game_score_t		TetreesUI::gameScore;
game_level_t		TetreesUI::gameLevel;
animation_t			TetreesUI::animationMode;
animation_control_t TetreesUI::animationControl;

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

gboolean TetreesUI::onDrawNextTetrominoDrawingArea(
		GtkWidget *widget,
		cairo_t *cr,
		gpointer user_data)
{

	drawNextTetromino(cr);
	return G_SOURCE_REMOVE;

}

/**
 * Draw the scene.
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

		nextTetromino = gameEngine->getNextTetromino();
		matrixDim = (nextTetromino.shape.getRowHeight()
						* nextTetromino.shape.getColWidth());

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

void TetreesUI::drawRowRemovalAnimationStep(cairo_t *cr)
{



}

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
											 NULL
	);

	std::string lvlNum = utils->intToStr(gameLevel.lvl + 1);
	std::string lvlStr = LABEL_LVL_LABEL;
	lvlStr.append(lvlNum);
	gtk_label_set_text(GTK_LABEL(lvlLabel), lvlStr.c_str());

}

void TetreesUI::onPlayClicked(GtkButton* button)
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
					NULL,
					(GDestroyNotify)contextHandler
			);

		animationMode = ANIMATION_NONE;
		gameEngine->issueCommand(CMD_PLAY);
		setUIState(UI_STATE_PLAYING);
		gtk_widget_queue_draw(mainWindow);

	}

}

void TetreesUI::onPauseClicked(GtkButton *button)
{

	setUIState(UI_STATE_PAUSED);
	gameEngine->issueCommand(CMD_PAUSE);
	if(animationMode != ANIMATION_GAME_PAUSED)
		animationMode = ANIMATION_GAME_PAUSED;
	else
		animationMode = ANIMATION_NONE;
	gtk_widget_queue_draw(mainWindow);

}

void TetreesUI::onResetClicked(GtkButton *button)
{

	gameEngine->issueCommand(CMD_RESET);
	setUIState(UI_STATE_NOT_STARTED);
	animationMode = ANIMATION_NONE;
	gtk_widget_queue_draw(mainWindow);

}

gboolean TetreesUI::animationHandler(GtkWidget *widget)
{

	switch(animationMode){

	case ANIMATION_NONE:
		break;

	case ANIMATION_GAME_PAUSED:
		break;

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

	}

	return G_SOURCE_CONTINUE;

}

gboolean TetreesUI::updateGameContext(GtkWidget *widget)
{

	gboolean ret = G_SOURCE_CONTINUE;

	switch(gameEngine->getGameState()){

	case GAME_STATE_PLAYING:
		gameEngine->gameStep(STEP_DROP);
		//A level up may result in a faster game speed,
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

void TetreesUI::contextHandler(gpointer data)
{

	updateEventID = 0;

	switch(gameEngine->getGameState()){
	case GAME_STATE_PLAYING:
		updateEventID = g_timeout_add_full(
				G_PRIORITY_HIGH,
				gameLevel.gameSpeed,
				(GSourceFunc)updateGameContext,
				NULL,
				(GDestroyNotify)contextHandler
		);
		break;
	case GAME_STATE_PAUSED:
		break;
	case GAME_STATE_NOT_STARTED:
		break;
	case GAME_STATE_GAME_OVER:
		break;

	}

}

/**
 * Create and put together all UI elements.
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

	g_signal_connect(mainWindow, "key-press-event", G_CALLBACK(onKeyPress), NULL);
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
	playingFieldFrame = gtk_frame_new(NULL);
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
			NULL
	);

	g_timeout_add(10,(GSourceFunc)animationHandler, NULL);
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
	nextTetrominoFrame = gtk_frame_new(NULL);
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
			NULL
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
	scoreFrame = gtk_frame_new(NULL);
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
			NULL
	);
	gtk_text_buffer_create_tag(
			GTK_TEXT_BUFFER(scoreTextBuf),
			"fontTag",
			"font",
			"PressStart2P 24",
			NULL
	);
	gtk_text_buffer_create_tag(
			GTK_TEXT_BUFFER(scoreTextBuf),
			"fgColorTag",
			"foreground",
			"green",
			NULL
	);
	gtk_text_buffer_create_tag(
			GTK_TEXT_BUFFER(scoreTextBuf),
			"abovePixelsTag",
			"pixels-above-lines",
			12,
			NULL
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

	g_signal_connect(
			playButton,
			"clicked",
			G_CALLBACK(onPlayClicked),
			NULL
	);
	//***************************************************************************************

	//=============
	//PAUSE BUTTON
	//=============
	//***************************************************************************************
	pauseButton = gtk_button_new_with_label(LABEL_PAUSE_BUTTON);
	gtk_widget_set_name(GTK_WIDGET(pauseButton), CSS_NAME_PAUSE_BUTTON);
	gtk_widget_set_size_request(GTK_WIDGET(pauseButton), 154, 0);
	gtk_widget_set_sensitive(GTK_WIDGET(pauseButton), FALSE);

	g_signal_connect(
			pauseButton,
			"clicked",
			G_CALLBACK(onPauseClicked),
			NULL
	);
	//***************************************************************************************

	//=============
	//RESET BUTTON
	//=============
	//***************************************************************************************
	resetButton = gtk_button_new_with_label(LABEL_RESET_BUTTON);
	gtk_widget_set_name(GTK_WIDGET(resetButton), CSS_NAME_RESET_BUTTON);
	gtk_widget_set_sensitive(GTK_WIDGET(resetButton), FALSE);

	g_signal_connect(
			resetButton,
			"clicked",
			G_CALLBACK(onResetClicked),
			NULL
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
			NULL
	);
	//***************************************************************************************

	gtk_widget_show_all(mainWindow);

}

