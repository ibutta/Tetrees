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

	/// The class constructor.
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
	Matrix2D<int> getScene();
	piece_t getNextTetromino();

private:

	TetreesUI *gameUI; /**< A TetreesUI object for controlling user interface and other game's elements.*/
	TetreesUtils *utils; /**< A TetreesUtils object that provides utility functions.*/
	static Matrix2D<int> gameBoard; /**< A Matrix2D that represents the gameBoard containing ONLY the
											already @ref step_BlockSpawnedTetromino "blocked tetrominoes".
											Each @ref __BLOCK__ "element" of this matrix contains data
											that refers to a @ref color_t "color".*/
	static Matrix2D<int> scene; /**< A Matrix2D containing all data present at @ref gameBoard, PLUS the
										@ref color_t "color" data related to the @ref spawnedTetromino
										"falling tetromino". A frame to be @ref TetreesUI::drawScene "drew" on screen.*/
	static piece_t spawnedTetromino; /**< The piece that is being controlled by player while falls.*/
	static piece_t nextTetromino; /**< The next piece to be spawned. It is showed inside the small
										@ref TetreesUI::nextTetrominoDrawingArea "rectangular area"
										upper right the @ref TetreesUI::playingFieldDrawingArea "playing field".*/
	static piece_t tetrominoes[NUM_OF_TETROMINOES]; /**< An array of @ref piece_t containing a sample of each of the 7 tetrominoes.*/
	static game_data_t gameData; /**< A struct containing several data related to the game.*/
	static game_level_t gameLevels[NUM_OF_LEVELS]; /**< An array of @ref game_level_t containing a sample of each level and related info.*/

	/// Reset game board.
	void gameBoardReset();
	/// Reset game scene/frame.
	void sceneReset();
	/// Update game scene/frame.
	void updateScene();
	/// Fetch Pascal's triangle for a specific element.
	unsigned getPascalTriangleElement(unsigned row, unsigned col);

	/// Execute a @ref STEP_DROP "drop step".
	void step_SpawnedTetromino_Drop();
	/// Execute a @ref STEP_ROTATE "rotate step".
	void step_SpawnedTetromino_Rotate();
	/// Execute a @ref STEP_SWERVE_RIGHT "right swerve step".
	void step_SpawnedTetromino_SwerveRight();
	/// Execute a @ref STEP_SWERVE_LEFT "left swerve step".
	void step_SpawnedTetromino_SwerveLeft();
	/// Blocks the falling piece.
	void step_BlockSpawnedTetromino();
	/// Detects possible collisions when @ref spawnedTetromino "spawned tetromino"'s position changes.
	bool step_DetectCollision(step_t step);
	/// Detects possible overlapping.
	bool step_OverlappingCheck(unsigned boardRow, unsigned boardCol);
	/// Detects if a movement infringes game board's border limits.
	bool step_BorderTrespassingCheck(limit_t borderSide);
	/// Drops the next tetromino.
	void step_SpawnNextTetromino();
	/// Generates a random tetromino.
	piece_t step_GetTetromino();

	/// Looks for rows filled with @ref TetreesDefs.__BLOCK__ "blocks".
	void score_HandleFilledRows();
	/// Checks if the level is completed.
	void score_HandleLvlCompletion();
	/// Checks game over conditions.
	bool score_HandleGameOver();
	/// Updates game's score.
	void score_UpdateScore(score_t scoreAction);
	/// Provides a value to update game score.
	unsigned score_GetPointsFromMultiplier();

};



#endif /* TETREESENGINE_H_ */
