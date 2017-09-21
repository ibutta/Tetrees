/*
 * TetreesEngine.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: Igor S. Buttarello
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <TetreesEngine.hpp>
#include <TetreesUI.hpp>
#include <TetreesUtils.hpp>

//Prefix increment operator overloading to assist tracking tetrominoes' rotation position
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

//Prefix decrement operator overloading to assist tracking tetrominoes' rotation position
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

void TetreesEngine::gameBoardReset()
{

	gameBoard.zeroFill();

}

void TetreesEngine::sceneReset()
{

	scene.zeroFill();

}

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
 * This function simply calls the member function @ref setupUI
 * from member object @ref gameUI.
 */
void TetreesEngine::gtkSetup(GtkApplication *app, gpointer user_data)
{

	gameUI->setupUI(app, user_data);

}

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
 * Changes game state accordingly to the key pressed
 * by the user during gameplay OR a @ref step_t "STEP_DROP" issued
 * by the update event, which is managed by the TetreesUI
 * class. @see TetreesUI::onPlayClicked
 * @param step A @ref step_t that will change game actual state.
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

void TetreesEngine::step_SpawnedTetromino_Drop()
{

	++spawnedTetromino.boardRow;

}

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

void TetreesEngine::step_SpawnedTetromino_SwerveRight()
{

	++spawnedTetromino.boardCol;

}

void TetreesEngine::step_SpawnedTetromino_SwerveLeft()
{

	--spawnedTetromino.boardCol;

}

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

unsigned TetreesEngine::score_GetPointsFromMultiplier(){

	unsigned mRow = gameData.gScore.multiplier;
	unsigned points;

	switch(mRow){
	case 0:
		points = 0;
		break;
	case 1:
		points = 1;
		break;
	default:
		points = 0;
		for(unsigned col = 0; col <= mRow; col++){
			points += getPascalTriangleElement(mRow, col);
		}

	}

	return points;

}

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

Matrix2D<int> TetreesEngine::getGameBoard()
{

	return gameBoard;

}

Matrix2D<int> TetreesEngine::getScene()
{

	return scene;

}

piece_t TetreesEngine::getNextTetromino()
{

	return nextTetromino;

}

game_state_t TetreesEngine::getGameState()
{

	return gameData.gState;

}

game_score_t TetreesEngine::getGameScore()
{

	return gameData.gScore;

}

game_level_t TetreesEngine::getGameLevel()
{

	return gameData.gLevel;

}

