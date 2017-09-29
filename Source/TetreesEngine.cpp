/*
 * TetreesEngine.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: Igor S. Buttarello
 */

/**
 * @file TetreesEngine.cpp
 * @brief The source file containing the TetreesEngine class' member functions implementation.
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <TetreesEngine.hpp>
#include <TetreesUI.hpp>
#include <TetreesUtils.hpp>

/// Prefix increment operator overload.
/**
 * Used to assist tetrominoes' rotation position tracking.
 */
rpos_t &operator++ (rpos_t &rpos){

	switch(rpos){
	case RPOS_0: rpos = RPOS_1; break;
	case RPOS_1: rpos = RPOS_2; break;
	case RPOS_2: rpos = RPOS_3; break;
	case RPOS_3: rpos = RPOS_0; break;
	default: rpos = RPOS_0;
	}
	return rpos;
}

/// Prefix decrement operator overload.
/**
 * Used to assist tetrominoes' rotation position tracking.
 */
rpos_t &operator-- (rpos_t &rpos){

	switch(rpos){
	case RPOS_0: rpos = RPOS_3; break;
	case RPOS_1: rpos = RPOS_0; break;
	case RPOS_2: rpos = RPOS_1; break;
	case RPOS_3: rpos = RPOS_2; break;
	default: rpos = RPOS_0;
	}
	return rpos;
}

Matrix2D<int> 	TetreesEngine::gameBoard;
Matrix2D<int> 	TetreesEngine::scene;
piece_t 		TetreesEngine::spawnedTetromino;
piece_t			TetreesEngine::nextTetromino;
piece_t 		TetreesEngine::tetrominoes[NUM_OF_TETROMINOES];
game_data_t 	TetreesEngine::gameData;
game_level_t	TetreesEngine::gameLevels[NUM_OF_LEVELS];

TetreesEngine::TetreesEngine()
{

	srand(time(NULL));
	gameUI = new TetreesUI(*this);
	utils = new TetreesUtils();
	gameData.gState = GAME_STATE_NOT_STARTED;
	gameData.gScore.score = SCORE_RESET;
	gameData.gScore.multiplier = 0;
	gameBoard.setMatrixDim(PLAY_FIELD_HEIGHT, PLAY_FIELD_WIDTH);
	gameBoard.zeroFill();
	scene.setMatrixDim(PLAY_FIELD_HEIGHT, PLAY_FIELD_WIDTH);
	scene.zeroFill();
	utils->startTetrominoesArray(tetrominoes);
	utils->startGameLevelsArray(gameLevels);
	gameData.gLevel = gameLevels[LVL_1];

}

/**
 * This function simply calls the member function @ref TetreesUI::setupUI "setupUI"
 * from member object @ref gameUI.
 */
void TetreesEngine::gtkSetup(GtkApplication *app, gpointer user_data)
{

	gameUI->setupUI(app, user_data);

}

/**
 * A communication channel between user interface and game engine
 * that handles @ref cmd_t "commands" sent to the engine related to what
 * @ref game_state_t "game state" should be set that moment.
 * @param c The @ref cmd_t "command" received.
 */
void TetreesEngine::issueCommand(cmd_t c){

	switch(c){
	case CMD_PLAY:
		if(
				gameData.gState == GAME_STATE_NOT_STARTED
				||
				gameData.gState == GAME_STATE_GAME_OVER
		){
			score_UpdateScore(SCORE_RESET);
			gameBoardReset();
			sceneReset();
			step_SpawnNextTetromino();
			updateScene();
		}
		gameData.gState = GAME_STATE_PLAYING;
		break;

	case CMD_PAUSE:
		if(gameData.gState != GAME_STATE_PAUSED){
			gameData.gState = GAME_STATE_PAUSED;
		}
		else{
			issueCommand(CMD_PLAY);
		}
		break;

	case CMD_RESET:
		score_UpdateScore(SCORE_RESET);
		gameBoardReset();
		sceneReset();
		gameData.gState = GAME_STATE_NOT_STARTED;
		break;

	}

}

/**
 * Changes game scene by applying one of the
 * @ref step_t "gameplay's steps" accordingly to the key
 * pressed by player OR a @ref step_t "STEP_DROP" issued
 * by the @ref gameUI "gameUI's" update event.
 * @see TetreesUI::onPlayClicked
 * @param step A @ref step_t that will somehow modify game scene.
 */
void TetreesEngine::gameStep(step_t step)
{

	switch(step){

	case STEP_DROP:
		if(!step_DetectCollision(step)){
			if(spawnedTetromino.reachedBottom){
					step_BlockSpawnedTetromino();
					score_HandleFilledRows();
					score_UpdateScore(SCORE_UPDATE);
					score_HandleLvlCompletion();
					if(score_HandleGameOver()){
						gameData.gState = GAME_STATE_GAME_OVER;
					}

			}
			else step_SpawnedTetromino_Drop();
		}
		else{
			step_BlockSpawnedTetromino();
			score_HandleFilledRows();
			score_UpdateScore(SCORE_UPDATE);
			score_HandleLvlCompletion();
			if(score_HandleGameOver()){
				gameData.gState = GAME_STATE_GAME_OVER;
			}
		}
		break;

	case STEP_ROTATE:
		if(spawnedTetromino.type != TETROMINO_O && !step_DetectCollision(step)){
			step_SpawnedTetromino_Rotate();
		}
		break;

	case STEP_SWERVE_RIGHT:
		if(!step_DetectCollision(step)){
			step_SpawnedTetromino_SwerveRight();
		}
		break;

	case STEP_SWERVE_LEFT:
		if(!step_DetectCollision(step)){
			step_SpawnedTetromino_SwerveLeft();
		}
		break;
	}

	updateScene();

}

/**
 * A getter for @ref game_state_t "gameData.gState".
 * @return The data stored by @ref game_state_t "gameData.gState".
 */
game_state_t TetreesEngine::getGameState()
{

	return gameData.gState;

}

/**
 * A getter for @ref game_state_t "gameData.gScore".
 * @return The data stored by @ref game_state_t "gameData.gScore".
 */
game_score_t TetreesEngine::getGameScore()
{

	return gameData.gScore;

}

/**
 * A getter for @ref game_state_t "gameData.gLevel".
 * @return The data stored by @ref game_state_t "gameData.gLevel".
 */
game_level_t TetreesEngine::getGameLevel()
{

	return gameData.gLevel;

}

/**
 * A getter for @ref scene.
 * @return The data stored by @ref scene.
 */
Matrix2D<int> TetreesEngine::getScene()
{

	return scene;

}

/**
 * A getter for @ref nextTetromino.
 * @return The data stored by @ref nextTetromino. The next tetromino to be spawned.
 */
piece_t TetreesEngine::getNextTetromino()
{

	return nextTetromino;

}

/**
 * Set all elements of @ref gameBoard to zero.
 */
void TetreesEngine::gameBoardReset()
{

	gameBoard.zeroFill();

}

/**
 * Set all elements of @ref gameScene to zero.
 */
void TetreesEngine::sceneReset()
{

	scene.zeroFill();

}

/**
 * Updates the color data within @ref scene by copying the color data from @ref gameBoard
 * PLUS color data from @ref spawnedTetromino, reuniting this way the necessary
 * data to @ref TetreesUI::drawScene "draw" a game scene on the screen.
 */
void TetreesEngine::updateScene()
{

	scene.zeroFill();
	for(unsigned r = 0; r < gameBoard.getRowHeight(); r++){
		for(unsigned c = 0; c < gameBoard.getColWidth(); c++){
			if(
					(r >= spawnedTetromino.boardRow)
					&&
					((r - spawnedTetromino.boardRow) < spawnedTetromino.shape.getRowHeight())
					&&
					(c >= spawnedTetromino.boardCol)
					&&
					((c - spawnedTetromino.boardCol) < spawnedTetromino.shape.getColWidth())
					&&
					(spawnedTetromino.shape((r - spawnedTetromino.boardRow),(c - spawnedTetromino.boardCol)) == 1)
			){
				scene(r,c) = spawnedTetromino.color;
			}
			else{
				scene(r,c) = gameBoard(r,c);
			}
		}
	}

}

/**
 * A recursive function that returns a specific element of
 * Pascal's triangle based on the row and column specified.
 * @param row A row of Pascal's triangle.
 * @param col A column of Pascal's triangle.
 * @return The number in Pascal's triangle pointed by the pair (row, column).
 */
unsigned TetreesEngine::getPascalTriangleElement(unsigned row, unsigned col){

	if(
			(row == 0 || row == 1)
			||
			(col == 0)
			||
			(row == col)
	) return 1;
	else return
			(getPascalTriangleElement(row - 1, col - 1)
			+ getPascalTriangleElement(row - 1, col));

}

/**
 * Move down the @ref spawnedTetromino "spawned tetromino",
 * incrementing @ref piece_t "spawnedTetromino.boardRow" by one.
 */
void TetreesEngine::step_SpawnedTetromino_Drop()
{

	++spawnedTetromino.boardRow;

}

/**
 * Rotates the @ref spawnedTetromino "spawned tetromino" by applying
 * matrix-related operations (@ref Matrix2D::transpose "transposing" and
 * @ref Matrix2D::clockwise90Rotation "rotating") to the tetromino's
 * @ref piece_t.shape "shape".
 */
void TetreesEngine::step_SpawnedTetromino_Rotate()
{

	switch(spawnedTetromino.type){
	case TETROMINO_I:
		//The I-shaped tetromino can only be transposed since its rotation axis isn't centered.
		//Thus, rotating it will cause abnormal behavior
		spawnedTetromino.shape.transpose();
		if(spawnedTetromino.rpos == RPOS_0) ++spawnedTetromino.rpos;
		else --spawnedTetromino.rpos;
		break;
	case TETROMINO_S:
	case TETROMINO_Z:
		//Both of these tetrominos have only 2 positions so clockwise rotating them more than once
		//will make their shape messy
		if(spawnedTetromino.rpos == RPOS_0){
			spawnedTetromino.shape.clockwise90Rotation();
			++spawnedTetromino.rpos;
		}
		else{
			spawnedTetromino.shape.counterclockwise90Rotation();
			--spawnedTetromino.rpos;
		}
		break;
	default:
		spawnedTetromino.shape.clockwise90Rotation();
		++spawnedTetromino.rpos;
	}

}

/**
 * Moves the @ref spawnedTetromino "spawned tetromino" to the right,
 * incrementing @ref piece_t "spawnedTetromino.boardCol" by one.
 */
void TetreesEngine::step_SpawnedTetromino_SwerveRight()
{

	++spawnedTetromino.boardCol;

}

/**
 * Moves the @ref spawnedTetromino "spawned tetromino" to the left,
 * decrementing @ref piece_t "spawnedTetromino.boardCol" by one.
 */
void TetreesEngine::step_SpawnedTetromino_SwerveLeft()
{

	--spawnedTetromino.boardCol;

}

/**
 * When the @ref spawnedTetromino "spawned tetromino" reaches a
 * position on game's board where there are no further @ref step_t "movements"
 * allowed, this function is called blocking the @ref spawnedTetromino "spawned tetromino"
 * by making it part of the blocked tetrominoes pile within game board, hence it's no
 * longer controllable by the player.
 * @remark Moving the @ref spawnedTetromino "falling piece" is allowed while it
 * doesn't reaches the bottom of the game board or collides with the pile of
 * previously blocked tetrominoes.
 */
void TetreesEngine::step_BlockSpawnedTetromino()
{

	int boardR = spawnedTetromino.boardRow;
	int boardC = spawnedTetromino.boardCol;

	for(unsigned r = 0; r < spawnedTetromino.shape.getRowHeight(); r++){
		for(unsigned c = 0; c < spawnedTetromino.shape.getColWidth(); c++){
			if(spawnedTetromino.shape(r,c) != EMPTY){
				gameBoard((boardR + r), (boardC + c)) = spawnedTetromino.color;
			}
		}
	}

}

/**
 * As a key piece of code to the TetreesEngine class, this function
 * analyzes whether a movement to be made by the @ref spawnedTetromino "spawned tetromino"
 * will or will not result in a collision.
 *
 * When player tries to execute one of the @ref step_t "steps" on the
 * @ref spawnedTetromino "spawned tetromino", this function is called in order
 * to prevent any illegal movement like an overlapping or a moving beyond game board's
 * border limits.
 * @param step The @ref step_t to be validated.
 * @return `TRUE` if the movement is allowed. `FALSE` otherwise.
 */
bool TetreesEngine::step_DetectCollision(step_t step)
{
	bool collision = false;

	switch(step){
	case STEP_DROP:

		//Check if the falling piece has hit the bottom
		if((LIMIT_BOTTOM_BORDER - spawnedTetromino.boardRow) < spawnedTetromino.shape.getRowHeight()){
			for(unsigned c = 0; c < spawnedTetromino.shape.getColWidth(); c++){
				if(spawnedTetromino.shape((LIMIT_BOTTOM_BORDER - spawnedTetromino.boardRow), c) != EMPTY){
					spawnedTetromino.reachedBottom = true;
					collision = true;
					break;
				}
			}
		}
		//Also check if it will collide with some other blocked piece
		collision = step_OverlappingCheck((spawnedTetromino.boardRow + 1), spawnedTetromino.boardCol);


		break;

	case STEP_ROTATE:

		switch(spawnedTetromino.type){
		//The tetromino I-shaped should not be rotated since its rotation axis
		//isn't centered. Instead, it should only be transposed.
		case TETROMINO_I:
			spawnedTetromino.shape.transpose();
			break;
		default:
			//Rotate the piece for the collision test
			spawnedTetromino.shape.clockwise90Rotation();
		}

		if(
				//Does the piece after rotation goes beyond game board right border?
				(step_BorderTrespassingCheck(LIMIT_RIGHT_BORDER))
				||
				//Does the piece after rotation goes beyond game board left border?
				(step_BorderTrespassingCheck(LIMIT_LEFT_BORDER))
				||
				//Does the piece after rotation goes beyond game board bottom?
				(step_BorderTrespassingCheck(LIMIT_BOTTOM_BORDER))
				||
				(step_OverlappingCheck(spawnedTetromino.boardRow, spawnedTetromino.boardCol))
		){
			collision = true;
		}
		switch(spawnedTetromino.type){
		case TETROMINO_I:
			//Transpose it back to its initial position.
			spawnedTetromino.shape.transpose();
			break;
		default:
			//Rotate it back to its initial position
			spawnedTetromino.shape.counterclockwise90Rotation();
		}

		break;

	case STEP_SWERVE_RIGHT:

		if((spawnedTetromino.boardCol + (spawnedTetromino.shape.getColWidth() - 1)) >= LIMIT_RIGHT_BORDER){
			//Piece's shape is located at the right margin of the game board. Now it's necessary to check whether the
			//tetromino itself is or is not in contact with the margin.
			for(unsigned r = 0; r < spawnedTetromino.shape.getRowHeight(); r++){
				if(spawnedTetromino.shape(r,(LIMIT_RIGHT_BORDER - spawnedTetromino.boardCol)) != EMPTY){
					collision = true;
					break;
				}
			}

		}
		else{
			//Check game board immediate right column for filled cells
			collision = step_OverlappingCheck(spawnedTetromino.boardRow, (spawnedTetromino.boardCol + 1));
		}

		break;

	case STEP_SWERVE_LEFT:

		if(spawnedTetromino.boardCol <= LIMIT_LEFT_BORDER){
			//Piece's shape is located at the left margin of the game board. Now it's necessary to check whether the
			//tetromino itself is or is not in contact with the margin.
			for(unsigned r = 0; r < spawnedTetromino.shape.getRowHeight(); r++){
				if(spawnedTetromino.shape(r,(LIMIT_LEFT_BORDER - spawnedTetromino.boardCol)) != EMPTY){
					collision = true;
					break;
				}
			}

		}
		else{
			//Check game board immediate left column for filled cells
			collision = step_OverlappingCheck(spawnedTetromino.boardRow, (spawnedTetromino.boardCol - 1));
		}

	}

	return collision;

}

/**
 * Checks if the @ref spawnedTetromino "spawned tetromino" will somehow collide with
 * another tetromino already blocked on the game board.
 *
 * A possible collision is verified by using the given position at @ref gameBoard "game board" (boardPosRow and
 * boardPosCol) as the start point (0,0) and, from this point on, comparing the surrounding area
 * (@ref TetreesDefs.__CELL__ "cells") with @ref spawnedTetromino "spawned tetromino" position in the
 * @ref gameBoard "game board".
 * @param boardPosRow The row at the @ref gameBoard "game board" to be used as start point (_**Y-axis**_).
 * @param boardPosCol The column at the @ref gameBoard "game board" to be used as start point (_**X-axis**_).
 * @return `TRUE` if a collision occurs. `FALSE` otherwise.
 */
bool TetreesEngine::step_OverlappingCheck(unsigned boardPosRow, unsigned boardPosCol)
{

	for(unsigned boardR = boardPosRow, pieceR = 0;
		boardR < (boardPosRow + spawnedTetromino.shape.getRowHeight());
		boardR++, pieceR++){
		for(unsigned boardC = boardPosCol, pieceC = 0;
			boardC < (boardPosCol + spawnedTetromino.shape.getColWidth());
			boardC++, pieceC++){
			if(
					(spawnedTetromino.shape(pieceR,pieceC) != EMPTY)
					&&
					(gameBoard(boardR,boardC) != EMPTY_CELL)
			)
				return true;
		}
	}
	return false;

}

/**
 * Checks if the movement to be performed by the @ref spawnedTetromino "spawned tetromino"
 * won't trespass the @ref gameBoard "game board's" borders limits, i.e. if it won't go off the visible
 * @ref TetreesUI.playingFieldDrawingArea "playing field area".
 *
 * A border trespass happens when any of the @ref TetreesDefs.__BLOCK__ "blocks" composing
 * the @ref spawnedTetromino "spawned tetromino" goes beyond one of the @ref limit_t
 * "border limits" of the @ref gameBoard "game board".
 * @param border The @ref limit_t "border limit" side to be checked.
 * @return `TRUE` if a trespass occurs. `FALSE` otherwise.
 */
bool TetreesEngine::step_BorderTrespassingCheck(limit_t border)
{
	switch(border){

	case LIMIT_LEFT_BORDER:
		if(spawnedTetromino.boardCol < LIMIT_LEFT_BORDER){
			for(unsigned r = 0; r < spawnedTetromino.shape.getRowHeight(); r++){
				if(spawnedTetromino.shape(r,0) != EMPTY){
					return true;
				}
			}
		}
		break;

	case LIMIT_RIGHT_BORDER:
		if((spawnedTetromino.boardCol + (spawnedTetromino.shape.getColWidth() - 1)) > LIMIT_RIGHT_BORDER){
			for(unsigned r = 0; r < spawnedTetromino.shape.getRowHeight(); r++){
				if(spawnedTetromino.shape(r,(spawnedTetromino.shape.getColWidth() - 1)) != EMPTY){
					return true;
				}
			}
		}
		break;

	case LIMIT_BOTTOM_BORDER:
		if((spawnedTetromino.boardRow + (spawnedTetromino.shape.getRowHeight() - 1)) > LIMIT_BOTTOM_BORDER){
			for(unsigned c = 0; c < spawnedTetromino.shape.getColWidth(); c++){
				if(spawnedTetromino.shape((spawnedTetromino.shape.getRowHeight() - 1),c) != EMPTY){
					return true;
				}
			}
		}
		break;

	}
	return false;

}

/**
 * Drops the next tetromino. The one that appears inside the @ref TetreesUI.nextTetrominoDrawingArea
 * "next tetromino area".
 *
 * This function simply updates the data stored in @ref spawnedTetromino with the one stored
 * in @ref nextTetromino, then it generates another tetromino data to be stored in @ref nextTetromino.
 */
void TetreesEngine::step_SpawnNextTetromino()
{


#ifndef MOCKUP

	if(gameData.gState == GAME_STATE_NOT_STARTED){
		spawnedTetromino = step_GetTetromino();
		nextTetromino = step_GetTetromino();
	}
	else{
		spawnedTetromino = nextTetromino;
		nextTetromino = step_GetTetromino();
	}
#else
	spawnedTetromino = tetrominoes[TETROMINO_I];
	nextTetromino = tetrominoes[TETROMINO_I];
#endif

}

/**
 * Randomly generates a @ref piece_t "tetromino" accordingly to the @ref t_prop
 * "spawn proportion" per level.
 * @return A random @ref piece_t "tetromino" to be stored as the @ref nextTetromino "next tetromino" to spawn.
 */
piece_t TetreesEngine::step_GetTetromino()
{

	int tetrominoIndex = (rand() % NUM_OF_TETROMINOES);
	int i = 0;

	//Verify if all tetrominos spawned until here have respected the level proportion
	//and reset it if so
	while(
			gameData.gLevel.tetrominoesProportionArray[i] == 0
			&&
			i < NUM_OF_TETROMINOES
	){
		++i;
		if(i == NUM_OF_TETROMINOES)
			//All tetrominos were spawned in the right proportion. Reset.
			gameData.gLevel = gameLevels[gameData.gLevel.lvl];
	}

	//Search for next tetromino shape that may spawn without disrupting the level proportion.
	//The previous "while" assures this one won't go in an infinite loop, since it
	//verifies that there is at least one piece to drop within the proportion.
	while(gameData.gLevel.tetrominoesProportionArray[tetrominoIndex] == 0)
		tetrominoIndex = (rand() % NUM_OF_TETROMINOES);

	--gameData.gLevel.tetrominoesProportionArray[tetrominoIndex];
	return tetrominoes[tetrominoIndex];

}

/**
 * Looks for rows completely filled with @ref TetreesDefs.__BLOCK__ "blocks",
 * marks the filled ones, removes it/them, and increments @ref game_score_t
 * "game score multiplier".
 *
 * @remark Accordingly game's rules, all filled lines should be removed and that is
 * the role of this function.
 */
void TetreesEngine::score_HandleFilledRows()
{

	gameData.gScore.multiplier = 0;

	Matrix2D<unsigned> rowsFilled(gameBoard.getRowHeight());
	for(unsigned i = 1; i <= LIMIT_PLAYING_AREA_HEIGHT; i++) rowsFilled(i) = 1;

	for(unsigned r = 1; r <= LIMIT_PLAYING_AREA_HEIGHT; r++){
		for(unsigned c = 1; c <= LIMIT_PLAYING_AREA_WIDTH; c++){
			if(gameBoard(r,c) == EMPTY_CELL){
				rowsFilled(r) = 0;
				break;
			}
		}
	}

	for(unsigned i = 1; i <= LIMIT_PLAYING_AREA_HEIGHT; i++){
		if(rowsFilled(i) == 1){
			++gameData.gScore.multiplier;
			for(unsigned r = i; r >= 1; r--){
				for(unsigned c = 1; c <= LIMIT_PLAYING_AREA_WIDTH; c++){
					if(r == 1){
						gameBoard(r,c) = COLOR_TRANSPARENT;
					}
					else{
						gameBoard(r,c) = gameBoard((r - 1),c);
					}
				}
			}
		}
	}

}

/**
 * Checks whether the level was completed or not by verifying the actual @ref gameData.gScore.score
 * "score" and comparing it with the @ref gameData.gLevel.nextLvlScore "minimum score required"
 * to reach the next of the game level (see also @ref next_lvl_score_t). In case minimum score for next
 * level was reached, the function also increments the @ref gameData.gLevel "game level".
 * @remark **Level 10** is the last and hardest level of the game, thus it won't be incremented.
 */
void TetreesEngine::score_HandleLvlCompletion()
{

	if(
			(gameData.gScore.score >= gameData.gLevel.nextLvlScore)
			&&
			(gameData.gLevel.lvl < LVL_10)
	){
		gameData.gLevel = gameLevels[++gameData.gLevel.lvl];
	}

}

/**
 * Checks if the game has reached a state where it can no longer proceed.
 *
 * The game will be over whenever the pile of @ref step_BlockSpawnedTetromino "blocked tetrominoes"
 * reaches the top of the game board.
 * @return 'TRUE' if the game is over. 'FALSE' otherwise.
 */
bool TetreesEngine::score_HandleGameOver()
{
	bool isGameOver = false;

	//Check the topmost row of the game board to see if there is any blocked piece there
	//meaning the game is over
	for(unsigned boardC = 0; boardC < LIMIT_PLAYING_AREA_WIDTH; boardC++){
		if(gameBoard(LIMIT_TOP_BORDER,boardC) != COLOR_TRANSPARENT) isGameOver = true;
	}
	//In case there is no blocked pieces reaching the top of the game board,
	//try to spawn the next tetromino and check if it will overlap any blocked tetromino
	//that lies in the second topmost row of the game board
	if(!isGameOver){
		step_SpawnNextTetromino();
		if(step_OverlappingCheck(spawnedTetromino.boardRow, spawnedTetromino.boardCol)){
				isGameOver = true;
				//Decrementing the spawned piece game board row position so it won't appear overlapping
				//the last blocked tetromino during last scene rendering
				--spawnedTetromino.boardRow;
		}
	}

	return isGameOver;

}

/**
 * Updates game's score either by resetting or incrementing it.
 * @param cmd A @ref score_t command type dictating in which way the score should be updated.
 */
void TetreesEngine::score_UpdateScore(score_t cmd)
{

	switch(cmd){
	case SCORE_RESET:
		gameData.gScore.score = 0;
		gameData.gLevel = gameLevels[LVL_1];
		break;

	case SCORE_UPDATE:
		gameData.gScore.score += score_GetPointsFromMultiplier();
		break;
	}

}

/**
 * The value returned by this function is a sum of all elements of a Pascal's Triangle
 * row. The row to be chosen within Pascal's Triangle is indicated by the value stored in
 * @ref game_score_t "gameData.gScore.multiplier" variable (e.g. row_2 == 4, row_3 == 8, row_4 == 16, and so on).
 * @note
 * Multiplier == 0:&nbsp;&nbsp;| 1 |&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--> NOT APPLICABLE<br>
 * Multiplier == 1:&nbsp;&nbsp;| 1 | 1 |&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--> NOT APPLICABLE<br>
 * Multiplier == 2:&nbsp;&nbsp;| 1 | 2 | 1 |&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--> 1 + 2 + 1 = 4<br>
 * Multiplier == 3:&nbsp;&nbsp;| 1 | 3 | 3 | 1 |&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--> 1 + 3 + 3 + 1 = 8<br>
 * Multiplier == 4:&nbsp;&nbsp;| 1 | 4 | 6 | 4 | 1 |&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--> 1 + 4 + 6 + 4 + 1 = 16<br>
 *
 * @return The sum of all elements from the specified row within Pascal's Triangle.
 */
unsigned TetreesEngine::score_GetPointsFromMultiplier(){


	unsigned mRow = gameData.gScore.multiplier;
	unsigned points;

	switch(mRow){
	//Multiplier == 0 means no line removed, thus no points earned.
	case 0:
		points = 0;
		break;
	//Multiplier == 1 means only one line removed, thus only one point is earned.
	case 1:
		points = 1;
		break;
	//Multiplier >= 2 begins a fetch within Pascal's Triangle to return a specific amount of points.
	default:
		points = 0;
		for(unsigned col = 0; col <= mRow; col++){
			points += getPascalTriangleElement(mRow, col);
		}

	}

	return points;

}
