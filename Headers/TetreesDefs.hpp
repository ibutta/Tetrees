/*
 * TetreesDefs.h
 *
 *  Created on: Jun 1, 2017
 *      Author: Igor S. Buttarello.
 */

/**
 * @file TetreesDefs.hpp
 * @brief Defs header file.
 *
 * A key header file that defines several global
 * constants, macros, enums, and structs.
 */

#ifndef TETREESDEFS_H_
#define TETREESDEFS_H_

#include <iostream>
#include <string>
#include <sstream>

//Remove comment for test purposes
//#define MOCKUP


/**
 * @name Mocked Definitions
 * @{
 */

//The following defined constants are mocked. They serve only as definitions for documentation purposes.

#define __CELL__ 				/**< A cell is the space occupied by a @ref TetreesDefs.__BLOCK__ "block".*/
#define __BLOCK__ 				/**< Blocks are the smallest four-sided polygon units that compose a @ref TetreesDefs.__TETROMINO__ "tetromino".*/
#define __TETROMINO__ 			/**< A tetromino (plural is a geometric form composed by several @ref TetreesDefs.__BLOCK__ "blocks" put together.
 	 	 	 	 	 	 		There are 7 types of tetrominoes (see @ref tetromino_t).**/
#define __UPDATE_EVENT__ 		/**< This is a source event attached to the application's
						 		<a href="https://developer.gnome.org/programming-guidelines/stable/main-contexts.html.en">main context</a>
								and it's responsible for updating the game context.

								Updating the game context consists in periodically issuing a @ref step_t.STEP_DROP "DROP STEP" resulting in game progress
						  	    and forcing player's interaction in order to handle the situation (in other words, forces the player to play!).

								The update event is constantly attached and detached (registered and unregistered) from the
								<a href="https://developer.gnome.org/programming-guidelines/stable/main-contexts.html.en">main context</a>
								accordingly to the actual ref game_state_t "game state" and @ref game_level_t "game level".
								@see <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#glib-The-Main-Event-Loop.description">
								The Main Event Loop</a>*/
#define __ANIMATION_EVENT__ 	/**< Similar to the @ref TetreesDefs.__UPDATE_EVENT__ "update event" the animation event is also a
								source event attached to the application's <a href="https://developer.gnome.org/programming-guidelines/stable/main-contexts.html.en">
								main context</a> and it is responsible for executing animations which, in this project,
								are divided into steps executed separately, iteratively, and consecutively giving a "feeling of movement".

								This event is never detached from <a href="https://developer.gnome.org/programming-guidelines/stable/main-contexts.html.en">
								main context</a>. It is registered during @ref TetreesUI::setupUI "UI setup" and keeps executing with the same
								@ref ANIMATION_EVENT_INTERVAL "interval" until application is terminated, however it'll only be used to
								draw on the screen when an animation needs to be performed.
								@see <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#glib-The-Main-Event-Loop.description">
						  		The Main Event Loop</a> and <a href="https://developer.gnome.org/glib/stable/glib-The-Main-Event-Loop.html#g-timeout-add-full">
						  		g_timeout_add_full</a>*/
/**@}*/

/**
 * @name Fundamental Values
 * @{
 */
const unsigned int ANIMATION_EVENT_INTERVAL = 10;           /**< The execution interval (in ms) in which the @ref TetreesDefs.__ANIMATION_EVENT__ "animation event"
												            is executed.*/
const double BLOCK_SIDE_DIM = 30; 							/**< The size (in pixels) of each side of a block.*/
const double TETROMINO_LINE_WIDTH = 4; 						/**< @ref TetreesDefs.__TETROMINO__ "Tetromino's" line thickness (in pixels).*/
const int NUM_OF_TETROMINOES = 7; 			                /**< The seven @ref TetreesDefs.__TETROMINO__ "tetrominoes" fundamental shapes: I, O, T, S, Z, J and L.*/
const int NUM_OF_LEVELS = 10; 				                /**< Number of game levels.*/
const int NUM_OF_COLORS = 11; 				                /**< Number of colors mapped to @ref rgb_t.*/
const int PLAY_FIELD_WIDTH = 12; 			                /**< The playing area width (in cells).*/
const int PLAY_FIELD_HEIGHT = 22; 			                /**< The playing area height (in cells).
 	 	 	 	 	 	 	 	 	 	 	                @remark A cell is the space occupied by a @ref TetreesDefs.__BLOCK__ "block".*/
const int HIDDEN_CELLS_OFFSET = 1 * BLOCK_SIDE_DIM;			/**< An offset function to compensate the @ref TetreesDefs.__CELL__ "cells" that are
															outside the boundaries of the game board's visible playing area.*/
const int PLAY_FIELD_PIXEL_WIDTH =
		((BLOCK_SIDE_DIM * (PLAY_FIELD_WIDTH - 2))
				+ (TETROMINO_LINE_WIDTH / 2)); 						/**< Calculates the playing field width (in pixels)*/
const int PLAY_FIELD_PIXEL_HEIGHT =
		((BLOCK_SIDE_DIM * (PLAY_FIELD_HEIGHT - 2))
				+ (TETROMINO_LINE_WIDTH / 2));						/**< Calculates the playing field height (in pixels)*/
const int NEXT_TETROMINO_FIELD_CELL_WIDTH = 6 * BLOCK_SIDE_DIM;		/**< Calculates the next tetromino field width (in pixels)*/
const int NEXT_TETROMINO_FIELD_CELL_HEIGHT = 4 * BLOCK_SIDE_DIM;	/**< Calculates the next tetromino field height (in pixels)*/
/**@}*/

//MACROS
#define SCENE_X(pos) ((pos * BLOCK_SIDE_DIM) - HIDDEN_CELLS_OFFSET + (TETROMINO_LINE_WIDTH / 2)) 		/**< A function to find the exact X = 0 position (in pixels) of
																										a given position (@ref TetreesDefs.__CELL__ "cell") in the playing field.
																										@warning This function uses screen coordinates.*/
#define SCENE_Y(pos) SCENE_X(pos) 																		/**< A function to find the exact Y = 0 position (in pixels) of
																										a given position (@ref TetreesDefs.__CELL__ "cell") in the playing field.
																										@warning This function uses screen coordinates.*/
#define NEXT_TETROMINO_X(pos) ((pos * BLOCK_SIDE_DIM) + (TETROMINO_LINE_WIDTH / 2)) 					/**< A function to find the exact X = 0 position (in pixels) of
																										a given position (@ref TetreesDefs.__CELL__ "cell") in the next tetromino field.
																										@warning This function uses screen coordinates.*/
#define NEXT_TETROMINO_Y(pos) NEXT_TETROMINO_X(pos) 													/**< A function to find the exact Y = 0 position (in pixels) of
																										a given position (@ref TetreesDefs.__CELL__ "cell") in the next tetromino field.
																										@warning This function uses screen coordinates.*/
#define CSS_FILE_PATH "Misc/TetreesStyle.css"

const int _R_ = 0;
const int _G_ = 1;
const int _B_ = 2;

const int EMPTY_CELL = 0; /**< Used to represent a free @ref TetreesDefs.__CELL__ within @ref TetreesEngine.gameBoard "game board".*/
const int EMPTY = 0; /**< Used to represent a position within a @ref piece_t.shape "tetromino's shape" not filled by a @ref TetreesDefs.__BLOCK__ "block".*/

/// Game steps.
/**
 * Commands issued to game engine in order to
 * modify the game scene.
 */
enum step_t{
	STEP_DROP = 0, /**< Increment the @ref TetreesEngine::spawnedTetromino "spawned tetromino" position by one @ref TetreesDefs.__BLOCK__ "block" downwards.*/
	STEP_ROTATE, /**< Rotate the @ref TetreesEngine::spawnedTetromino "spawned tetromino".*/
	STEP_SWERVE_RIGHT, /**< Increment the @ref TetreesEngine::spawnedTetromino "spawned tetromino" position by one @ref TetreesDefs.__BLOCK__ "block" rightwards.*/
	STEP_SWERVE_LEFT /**< Increment the @ref TetreesEngine::spawnedTetromino "spawned tetromino" position by one @ref TetreesDefs.__BLOCK__ "block" leftwards.*/
};

/// Tetrominoes types.
/**
 * An enum representing each of the fundamental
 * elements of the game: the @ref TetreesDefs.__TETROMINO__ "tetrominoes".
 */
enum tetromino_t{
	TETROMINO_I = 0,					/**< The I-shaped tetromino.*/
	TETROMINO_O,						/**< The O-shaped tetromino.*/
	TETROMINO_T,						/**< The T-shaped tetromino.*/
	TETROMINO_S,						/**< The S-shaped tetromino.*/
	TETROMINO_Z,						/**< The Z-shaped tetromino.*/
	TETROMINO_J,						/**< The J-shaped tetromino.*/
	TETROMINO_L							/**< The L-shaped tetromino.*/
};

/// Game levels.
/**
 * All possible game levels. Beat them all
 * if you feel you're up to the challenge!
 */
enum level_num_t : int{
	LVL_1 = 0,							/**< Level 1.*/
	LVL_2,								/**< Level 2.*/
	LVL_3,								/**< Level 3.*/
	LVL_4,								/**< Level 4.*/
	LVL_5,								/**< Level 5.*/
	LVL_6,								/**< Level 6.*/
	LVL_7,								/**< Level 7.*/
	LVL_8,								/**< Level 8.*/
	LVL_9,								/**< Level 9.*/
	LVL_10								/**< Level 10 - The last one!.*/
};

/// Game speed per level.
/**
 * Game speed (in milliseconds) related to each game level.
 *
 * Each game level increases the difficulty and one way to
 * make the game more difficult is by speeding it up.
 *
 * @note These are the intervals between each occurrence of the @ref TetreesDefs.__UPDATE_EVENT__ "update event"
 * within each level of the game.
 */
enum level_speed_t : int{
	LVL_1_SPEED = 900,						/**< Update event will occur every 900 ms within level 1.*/
	LVL_2_SPEED = 700,						/**< Update event will occur every 700 ms within level 2.*/
	LVL_3_SPEED = 700,						/**< Update event will occur every 700 ms within level 3.*/
	LVL_4_SPEED = 580,						/**< Update event will occur every 580 ms within level 4.*/
	LVL_5_SPEED = 580,						/**< Update event will occur every 580 ms within level 5.*/
	LVL_6_SPEED = 400,						/**< Update event will occur every 400 ms within level 6.*/
	LVL_7_SPEED = 400,						/**< Update event will occur every 400 ms within level 7.*/
	LVL_8_SPEED = 300,						/**< Update event will occur every 300 ms within level 8.*/
	LVL_9_SPEED = 300,						/**< Update event will occur every 300 ms within level 9.*/
	LVL_10_SPEED = 150						/**< Update event will occur every 150 ms within level 10.*/
};

/**
 * @anchor t_prop
 * @name TETROMINOES_PROPORTION
 *
 * @ref TetreesDefs.__TETROMINO__ "Tetrominoes" spawning proportion per level where the
 * elements within the arrays represent the amount of
 * @ref TetreesDefs.__TETROMINO__ "tetrominoes" of each type to be spawned.
 *
 * @remark The array follows the @ref tetromino_t sequence.\n
 * E.g.: @code{.cpp}
 * //                                         I,O,T,S,Z,J,L
 * const int LVL1_TETROMINOES_PROPORTION[] = {5,1,3,2,2,3,3};
 * @endcode
 * means the first level of the game will randomly spawn @ref TetreesDefs.__TETROMINO__ "tetrominoes" with the following proportion:
 *       + 5 I-shaped tetrominoes;
 *       + 1 O-shaped tetromino;
 *       + 3 T-shaped tetrominoes;
 *       + 2 S-shaped tetrominoes;
 *       + 2 Z-shaped tetrominoes;
 *       + 3 J-shaped tetrominoes;
 *       + 3 L-shaped tetrominoes).
 *
 * @remark Zero-filled arrays means no proportion at all. Totally
 * random spawning!
 * @{
 */

//											 I,O,T,S,Z,J,L
const int LVL1_TETROMINOES_PROPORTION[] =  	{5,1,3,2,2,3,3};
const int LVL2_TETROMINOES_PROPORTION[] =  	{5,1,3,2,2,3,3};
const int LVL3_TETROMINOES_PROPORTION[] =  	{4,3,2,3,3,3,3};
const int LVL4_TETROMINOES_PROPORTION[] = 	{4,3,2,3,3,3,3};
const int LVL5_TETROMINOES_PROPORTION[] = 	{3,3,2,4,4,2,2};
const int LVL6_TETROMINOES_PROPORTION[] = 	{3,3,2,4,4,2,2};
const int LVL7_TETROMINOES_PROPORTION[] = 	{2,3,0,3,3,6,6};
const int LVL8_TETROMINOES_PROPORTION[] = 	{2,3,0,3,3,6,6};
const int LVL9_TETROMINOES_PROPORTION[] = 	{0,1,1,8,8,0,0};
const int LVL10_TETROMINOES_PROPORTION[] = 	{0,0,0,0,0,0,0};
/**@}*/

/// Scores thresholds.
/**
 * The score to be beaten per level before reaching
 * next one.
 */
enum next_lvl_score_t : unsigned{
	NEXT_LVL_SCORE_LVL1 = 	20,					/**< Level 1 goes from 0 to 19 points.*/
	NEXT_LVL_SCORE_LVL2 = 	40,					/**< Level 2 goes from 20 to 39 points.*/
	NEXT_LVL_SCORE_LVL3 = 	60,					/**< Level 3 goes from 40 to 59 points.*/
	NEXT_LVL_SCORE_LVL4 = 	90,					/**< Level 4 goes from 60 to 89 points.*/
	NEXT_LVL_SCORE_LVL5 = 	130,				/**< Level 5 goes from 90 to 129 points.*/
	NEXT_LVL_SCORE_LVL6 = 	160,				/**< Level 6 goes from 130 to 159 points.*/
	NEXT_LVL_SCORE_LVL7 = 	200,				/**< Level 7 goes from 160 to 199 points.*/
	NEXT_LVL_SCORE_LVL8 = 	240,				/**< Level 8 goes from 200 to 239 points.*/
	NEXT_LVL_SCORE_LVL9 = 	300,				/**< Level 9 goes from 240 to 299 points.*/
	NEXT_LVL_SCORE_LVL10 = 	0					/**< Level 10 goes from 300 to infinity... and beyond! Or until you give up...*/
};

/// UI possible colors.
/**
 * Available colors to be used in UI's drawing and assembling.
 */
enum color_t : int{
	COLOR_TRANSPARENT = 0,
	COLOR_LIGHT_BLUE,
	COLOR_YELLOW,
	COLOR_PURPLE,
	COLOR_GREEN,
	COLOR_RED,
	COLOR_BLUE,
	COLOR_ORANGE,
	COLOR_BACKGROUND,
	COLOR_GAME_PAUSED,
	COLOR_GAME_OVER
};

/// Game board limits.
/**
 * The game board is divided into cells (same as @ref TetreesDefs.__BLOCK__ "blocks").
 * This enum holds game board's border limits (in @ref TetreesDefs.__CELL__ "cells"),
 * which are widely used by game engine (and UI as well) during game processing.
 */
enum limit_t : unsigned{
	LIMIT_RIGHT_BORDER = PLAY_FIELD_WIDTH - 2,									/**< Rightmost visible column of the @ref TetreesEngine::gameBoard "game board".*/
	LIMIT_LEFT_BORDER = PLAY_FIELD_WIDTH - (PLAY_FIELD_WIDTH - 1),				/**< Leftmost visible column of the @ref TetreesEngine::gameBoard "game board".*/
	LIMIT_TOP_BORDER = PLAY_FIELD_HEIGHT - (PLAY_FIELD_HEIGHT - 1),				/**< Topmost visible row of the @ref TetreesEngine::gameBoard "game board".*/
	LIMIT_BOTTOM_BORDER = PLAY_FIELD_HEIGHT - 2,								/**< Bottom row of the @ref TetreesEngine::gameBoard "game board".*/
	LIMIT_PLAYING_AREA_HEIGHT = PLAY_FIELD_HEIGHT - 2,							/**< The playing area height (in @ref TetreesDefs.__CELL__ "cells").*/
	LIMIT_PLAYING_AREA_WIDTH = PLAY_FIELD_WIDTH - 2								/**< The playing area width (in @ref TetreesDefs.__CELL__ "cells").*/
};

/// Tetrominoes rotation positions.
/**
 * Used to track the exact rotation position in which the
 * @ref TetreesEngine::spawnedTetromino "spawned tetromino" finds itself.
 * It enables game engine to know exactly which is the next rotation
 * to be performed in case the rotation command (@ref STEP_ROTATE) is issued.
 */
enum rpos_t: int{
	RPOS_0 = 0,
	RPOS_1,
	RPOS_2,
	RPOS_3
};

/// Game engine commands.
/**
 * Available commands to be issued to the game engine.
 */
enum cmd_t: int{
	CMD_PLAY = 0,							/**< Start the game.*/
	CMD_PAUSE,								/**< Pause/Continue the game.*/
	CMD_RESET								/**< Reset game to its initial state.*/
};

/// Game states.
/**
 * Game states used to control several aspects of the
 * interaction between the game engine and the UI.
 */
enum game_state_t: int{
	GAME_STATE_NOT_STARTED = 0, 			/**< When game hasn't started yet, either if it has
											just been executed or if user reseted it.*/
	GAME_STATE_PLAYING, 					/**< When the game is actually being played. A gameplay state.*/
 	GAME_STATE_PAUSED, 						/**< When the game is paused.*/
	GAME_STATE_GAME_OVER, 					/**< When the game is over, i.e. when the pile of
											@ref TetreesDefs.__TETROMINO__ "tetrominoes" has reached the top of the game's
											@ref TetreesEngine::gameBoard "playing area".*/
};

/// User interface states.
/**
 * Used to control UI's elements behavior based on
 * game current state.
 */
enum ui_state_t: int{
	UI_STATE_NOT_STARTED = 0,				/**< @ref TetreesUI.playButton "Play Button" = enabled; @ref TetreesUI.pauseButton "Pause Button" = disabled; @ref TetreesUI.resetButton "Reset Button" = disabled.*/
	UI_STATE_PLAYING,						/**< @ref TetreesUI.playButton "Play Button" = disabled; @ref TetreesUI.pauseButton "Pause Button" = enabled; @ref TetreesUI.resetButton "Reset Button" = enabled.*/
	UI_STATE_PAUSED							/**< @ref TetreesUI.playButton "Play Button" = disabled; @ref TetreesUI.pauseButton "Pause Button (Continue)" = enabled; @ref TetreesUI.resetButton "Reset Button" = enabled.*/
};

/// Score commands.
/**
 * Used by game engine to control the score.
 */
enum score_t: unsigned{
	SCORE_RESET,
	SCORE_UPDATE
};

/// UI animation types.
/**
 * Possible animations types implemented. Used by UI to keep track of which animation is
 * going on.
 */
enum animation_t: int{
	ANIMATION_NONE = 0, 					/**< Used when the game is either in a playing, paused or not started @ref game_state_t "state".*/
	ANIMATION_GAME_OVER, 					/**< Used for the @ref TetreesUI::drawGameOverAnimationStep "game over animation drawing process".*/
	ANIMATION_GAME_PAUSED 					/**< Used for the @ref TetreesUI::drawPausedScene "game paused animation drawing process".*/
};

/// A matrix/vector abstraction class.
/**
 * Provides an abstraction for handling
 * matrices (multidimensional vectors) and is by far
 * the most important auxiliary piece of code implemented
 * in this project.
 */
template <class Type>
class Matrix2D
{

public:

	/// Class constructor (2D - matrix).
	/**
	 * Generates an object representing a 2D matrix.
	 * @param height The number of rows in the new matrix.
	 * @param width The number of columns in the new matrix.
	 */
	Matrix2D(unsigned height, unsigned width) : rHeight(height), cWidth(width), matrixArray(nullptr)
	{
		if(rHeight > 0 && cWidth > 0){
			matrixArray = new Type[rHeight * cWidth];
		}
	}

	/// Class constructor (1D - vector).
	/**
	 * Generates an object representing a 1D vector.
	 * @param width The number of elements of the new vector.
	 */
	Matrix2D(unsigned width) : rHeight(1), cWidth(width), matrixArray(nullptr)
	{
		if(cWidth > 0){
			matrixArray = new Type[cWidth];
		}
	}

	/// Class constructor (empty object).
	/**
	 * Generates an empty object.
	 */
	Matrix2D() : rHeight(0), cWidth(0), matrixArray(nullptr)
	{}

	/// Assign operator override.
	/*
	 * Overrides the assign operator to properly deal with
	 * copy issues.
	 */
	Matrix2D<Type> &operator= (const Matrix2D<Type> &other)
	{
		if(this != &other){
			delete[] matrixArray;
			rHeight = other.rHeight;
			cWidth = other.cWidth;
			matrixArray = new Type[rHeight * cWidth];
			std::copy(other.matrixArray, other.matrixArray + (rHeight * cWidth), matrixArray);
		}
		return *this;
	}

	/// Matrix element access.
	/**
	 * Overrides the `()` operator in order to access
	 * a matrix's element by providing its row and column indexes.
	 * @param r The row containing the element to be accessed.
	 * @param c The column containing the element to be accessed.
	 * @return The element pointed by the pair (r,c).
	 */
	const Type &operator() (unsigned r, unsigned c) const
	{
		return matrixArray[r * cWidth + c];
	}

	/// Matrix element access.
	/**
	 * Overrides the `()` operator in order to access
	 * a matrix's element by providing its row and column indexes.
	 * @param r The row containing the element to be accessed.
	 * @param c The column containing the element to be accessed.
	 * @return The element pointed by the pair (r,c).
	 */
	Type &operator() (unsigned r, unsigned c)
	{
		return matrixArray[r * cWidth + c];
	}

	/// Array element access.
	/**
	 * Overrides the `()` operator in order to access
	 * an array's element.
	 * @param i The index of the element to be accessed.
	 * @return The element pointed by i.
	 */
	const Type &operator() (unsigned i) const
	{
		return matrixArray[i];
	}

	/// Array element access.
	/**
	 * Overrides the `()` operator in order to access
	 * an array's element.
	 * @param i The index of the element to be accessed.
	 * @return The element pointed by i.
	 */
	Type &operator() (unsigned i)
	{
		return matrixArray[i];
	}

	/// Change matrix dimensions.
	/**
	 * Resizes the matrix by changing its dimensions,
	 * i.e. the number of rows and columns.
	 * @remark The matrix will be reseted and all its
	 * elements will be lost.
	 * @param newHeight The new number of rows.
	 * @param newWidth The new number of columns.
	 */
	void setMatrixDim(unsigned newHeight, unsigned newWidth)
	{
		if(newHeight > 0 && newWidth > 0){
			delete[] matrixArray;
			rHeight = newHeight;
			cWidth = newWidth;
			matrixArray = new Type[rHeight * cWidth];
		}
	}

	/// Change array size.
	/**
	 * Resizes the array by changing its number of elements.
	 * @remark The array will be reseted and all its
	 * elements will be lost.
	 * @param newWidth The new number of elements.
	 */
	void setMatrixDim(unsigned newWidth)
	{
		if(newWidth > 0){
			setMatrixDim(1, newWidth);
		}
	}

	/// Transpose the matrix.
	/**
	 * Applies the standard transpose operation to
	 * the matrix, i.e. makes columns into rows and
	 * rows into columns.
	 * @remark When applied to an array (i.e. a single rowed
	 * matrix) it does nothing.
	 */
	void transpose()
	{
		unsigned r, c;
		r = c = 0;
		Type auxMatrixArray[rHeight * cWidth];
		// Copies the main matrixArray to an auxiliary one.
		for(unsigned auxR = 0; auxR < rHeight; auxR++){
			for(unsigned auxC = 0; auxC < cWidth; auxC++){
				auxMatrixArray[auxR * cWidth + auxC] = matrixArray[auxR * cWidth + auxC];
			}
		}
		// Transposes the main matrix array.
		for(unsigned auxR = 0; auxR < rHeight; auxR++){
			for(unsigned auxC = 0; auxC < cWidth; auxC++){
				matrixArray[r * cWidth + c] = auxMatrixArray[auxR * cWidth + auxC];
				++r;
			}
			r = 0;
			++c;
		}
	}

	/// Reverse matrix's rows or the entire array.
	/**
	 * Inverts the order in which the elements of
	 * each row of the matrix (or the entire array) appear,
	 * i.e. the first element of a row will become its nth,
	 * the second its (nth - 1), and so on...
	 */
	void reverseRows()
	{
		unsigned r, c;
		r = c = 0;
		Type auxMatrixArray[rHeight * cWidth];
		// Copies the main matrixArray to an auxiliary one.
		for(unsigned auxR = 0; auxR < rHeight; auxR++){
			for(unsigned auxC = 0; auxC < cWidth; auxC++){
				auxMatrixArray[auxR * cWidth + auxC] = matrixArray[auxR * cWidth + auxC];
			}
		}
		// Reverses the rows/array.
		for(unsigned auxR = 0; auxR < rHeight; auxR++){
			for(unsigned auxC = cWidth; auxC > 0; auxC--){
				matrixArray[r * cWidth + c] = auxMatrixArray[auxR * cWidth + (auxC - 1)];
				++c;
			}
			c = 0;
			++r;
		}
	}

	/// Rotate the matrix 90 degrees clockwise.
	/**
	 * Applies the [transpose](@ref transpose) operation
	 * followed by the [reverseRows](@ref reverseRows)
	 * operation resulting in a 90 degrees clockwise rotation
	 * of the matrix.
	 * @remark If applied to an array it will result in the
	 * reversal of its elements since the transpose of an
	 * array has no effect on it.
	 */
	void clockwise90Rotation()
	{
		transpose();
		reverseRows();
	}

	/// Rotate the matrix 90 degrees counter clockwise.
	/**
	 * Applies the [reverseRows](@ref reverseRows) operation
	 * followed by the [transpose](@ref transpose) operation
	 * resulting in a 90 degrees counter clockwise rotation
	 * of the matrix.
	 * @remark If applied to an array it will result in the
	 * reversal of its elements since the transpose of an
	 * array has no effect on it.
	 */
	void counterclockwise90Rotation()
	{
		reverseRows();
		transpose();
	}

	/// Fill the matrix/array with zeroes.
	/**
	 * Sets all elements of the matrix/array to zero.
	 */
	void zeroFill()
	{
		for(unsigned l = 0; l < rHeight; l++){
			for(unsigned c = 0; c < cWidth; c++){
				matrixArray[l * cWidth + c] = 0;
			}
		}
	}

	/// Get rows amount.
	/**
	 * Returns the amount of rows present in the matrix.
	 * @remark If the object is an array, returns 1.
	 * @return The number of rows.
	 */
	unsigned getRowHeight()
	{
		return rHeight;
	}

	/// Get columns amount.
	/**
	 * Returns the amount of columns present in the matrix.
	 * @remark If the object is an array, returns the amount
	 * of elements (i.e. its length).
	 * @return The number of columns.
	 */
	unsigned getColWidth()
	{
		return cWidth;
	}

private:

	unsigned rHeight; 			/**< Matrix's number of rows.*/
	unsigned cWidth; 			/**< Matrix's number of columns/Array's length.*/
	Type *matrixArray; 			/**< The vector abstracting a matrix/array.*/

};

/// Struct representing a @ref TetreesDefs.__TETROMINO__ "tetromino".
/**
 * Used to store several data about related to
 * the @ref TetreesDefs.__TETROMINO__ "tetromino".
 */
struct piece_t{
	color_t color; 					/**< The @ref TetreesDefs.__TETROMINO__ "tetromino" color.*/
	tetromino_t type; 				/**< The @ref TetreesDefs.__TETROMINO__ "tetromino" type/shape.*/
	rpos_t rpos;					/**< The @ref TetreesDefs.__TETROMINO__ "tetromino" rotation position.*/
	unsigned boardCol;				/**< Column position of @ref piece_t.shape "tetromino's shape" first element (0,0) within the game board.*/
	unsigned boardRow;				/**< Row position of @ref piece_t.shape "tetromino's shape" first element (0,0) within the game board */
	bool reachedBottom;				/**< True when the @ref TetreesDefs.__TETROMINO__ "tetromino" is blocked at game board's bottom.*/
	Matrix2D<int> shape;			/**< Tetromino's shape related @ref TetreesUtils::startTetrominoesArray "data".*/
};

/// RGB elements of a color.
/**
 * Stores data related to RGB components of a color.
 */
struct rgb_t{
	color_t name; 			/**< The color name.*/
	double r;				/**< Red component value.*/
	double g;				/**< Green component value.*/
	double b;				/**< Blue component value.*/
};

/// Game score data type.
/**
 * A type for storing data about game's
 * score system.
 */
struct game_score_t{
	unsigned score;         			/**< The game score. Earn more. Beat your friends. Beat the game (if you're think you are up to the challenge)!*/
	unsigned multiplier;    			/**< Score multiplier. The higher the multiplier the more points you will earn.
										@note The multiplier is increased by removing multiple lines of @ref TetreesDefs.__TETROMINO__ "tetrominoes" simultaneously.*/
};

/// Game level data type.
/**
 * Stores several data related to the actual difficulty level of the game.
 */
struct game_level_t{
	int lvl;													/**< The level number.*/
	unsigned nextLvlScore;										/**< The score needed to reach next level.*/
	unsigned gameSpeed;											/**< The level speed (in ms).*/
	int tetrominoesProportionArray[NUM_OF_TETROMINOES];			/**< The @ref TetreesDefs.__TETROMINO__ "tetrominoes" spawn rate array related to this level.*/
};

/// Game data type.
/**
 * Stores several data related to game itself.
 */
struct game_data_t{
	game_state_t gState;			/**< Current game state. @see game_state_t.*/
	game_score_t gScore;			/**< Current game score related data. @see game_score_t.*/
	game_level_t gLevel;			/**< Current game level related data. @see game_level_t.*/
};

/// Type to control UI animations.
/**
 * Stores UI animation related data. This data
 * are used by @ref TetreesEngine::gameUI "UI object" so it knows when, how, and
 * for how long it should execute an animation.
 */
struct animation_control_t{
	int counter;
	int iterator;
	int drawRate;
	unsigned row;
	unsigned column;
};

#endif /* TETREESDEFS_H_ */
