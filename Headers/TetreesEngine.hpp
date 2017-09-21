/*
 * TetreesEngine.h
 *
 *  Created on: Jun 1, 2017
 *      Author: Igor S. Buttarello
 */

/**
 * @file TetreesEngine.hpp
 * @brief Header file for the TetreesEngine class.
 */

#ifndef TETREESENGINE_H_
#define TETREESENGINE_H_

#include <gtk/gtk.h>
#include <TetreesDefs.hpp>

class TetreesUI;
class TetreesUtils;

/// The game engine.
class TetreesEngine
{

public:

	/// The object constructor.
	TetreesEngine();
	/// Set up UI environment.
	void gtkSetup(GtkApplication* app, gpointer user_data);
	/// Send game engine a command.
	void issueCommand(cmd_t command);
	/// Changes game state.
	void gameStep(step_t step);
	game_state_t getGameState();
	game_score_t getGameScore();
	game_level_t getGameLevel();
	Matrix2D<int> getGameBoard();
	Matrix2D<int> getScene();
	piece_t getNextTetromino();

private:

	/// TetreesUI object for controlling user interface and other games elements.
	TetreesUI *gameUI;
	TetreesUtils *utils;
	static Matrix2D<int> gameBoard;
	static Matrix2D<int> scene;
	static piece_t spawnedTetromino;
	static piece_t nextTetromino;
	static piece_t tetrominoes[NUM_OF_TETROMINOES];
	static game_data_t gameData;
	static game_level_t gameLevels[NUM_OF_LEVELS];

	void gameBoardReset();
	void sceneReset();
	unsigned getPascalTriangleElement(unsigned row, unsigned col);

	void step_SpawnedTetromino_Drop();
	void step_SpawnedTetromino_Rotate();
	void step_SpawnedTetromino_SwerveRight();
	void step_SpawnedTetromino_SwerveLeft();

	/// Blocks the falling piece.
	void step_BlockSpawnedTetromino();
	bool step_DetectCollision(step_t step);
	bool step_OverlappingCheck(unsigned boardRow, unsigned boardCol);
	bool step_BorderTrespassingCheck(limit_t borderSide);
	void step_SpawnNextTetromino();
	piece_t step_GetTetromino();

	void score_HandleFilledRows();
	bool score_HandleGameOver();
	void score_HandleLvlCompletion();
	void score_UpdateScore(score_t scoreAction);

	/*
	 * Get points for score updating. The points are obtained by adding elements of Pascal's Triangle
	 * row indicated by the game score multiplier. (e.g. 2 == 4, 3 == 8, 4 == 16, and so on)
	 *
	 * Return: The sum of all elements from the specified row.
	 */
	unsigned score_GetPointsFromMultiplier();

	void updateScene();

};



#endif /* TETREESENGINE_H_ */
