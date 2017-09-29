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
	TetreesUtils();

	//UI utility member functions
	//========================================================
	void startColorsArray(Matrix2D<rgb_t> colorsArray);
	color_t getColor(int colorRef);
	std::string intToStr(unsigned intNum);
	//========================================================

	//Engine utility member functions
	//========================================================
	/// Assembles all data related to each of the 7 types of tetrominoes.
	void startTetrominoesArray(piece_t tetrominoesArray[NUM_OF_TETROMINOES]);
	void startGameLevelsArray(game_level_t levelsArray[NUM_OF_LEVELS]);
	//========================================================

};

#endif /* HEADERS_TETREESUTILS_HPP_ */
