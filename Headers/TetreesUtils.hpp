/*
 * TetreesUtils.hpp
 *
 *  Created on: Jul 28, 2017
 *      Author: Igor S. Buttarello
 */

/**
 * @file TetreesUtils.hpp
 * @brief Header file for the TetreesUtils class.
 */

#ifndef HEADERS_TETREESUTILS_HPP_
#define HEADERS_TETREESUTILS_HPP_

#include <TetreesDefs.hpp>

class TetreesUtils
{

public:
	/// The class constructor.
	TetreesUtils();

	//UI utility member functions
	//========================================================

	/// Fills an array with color related data.
	void startColorsArray(Matrix2D<rgb_t> colorsArray);
	/// Retrieves a color based on its reference.
	color_t getColor(int colorRef);
	/// Converts an integer to char.
	std::string intToStr(unsigned intNum);

	//========================================================

	//Engine utility member functions
	//========================================================
	/// Assembles all data related to each of the 7 types of tetrominoes.

	/// Fills an array with data regarding each type of tetrominoes available.
	void startTetrominoesArray(piece_t tetrominoesArray[NUM_OF_TETROMINOES]);
	/// Fills an array with data regarding each level of the game.
	void startGameLevelsArray(game_level_t levelsArray[NUM_OF_LEVELS]);

	//========================================================

};

#endif /* HEADERS_TETREESUTILS_HPP_ */
