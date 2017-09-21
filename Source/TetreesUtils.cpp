/*
 * TetreesUtils.cpp
 *
 *  Created on: Aug 22, 2017
 *      Author: Igor S. Buttarello
 */

#include <TetreesUtils.hpp>

TetreesUtils::TetreesUtils()
{
}

void TetreesUtils::startColorsArray(Matrix2D<rgb_t> colorsArray)
{

	for(int i = 0; i < NUM_OF_COLORS; i++){
		switch(i){

		case COLOR_TRANSPARENT:
			colorsArray(i).name = COLOR_TRANSPARENT;
			colorsArray(i).r = 1;
			colorsArray(i).g = 1;
			colorsArray(i).b = 1;
			break;

		case COLOR_LIGHT_BLUE:
			colorsArray(i).name = COLOR_LIGHT_BLUE;
			colorsArray(i).r = 0.4;
			colorsArray(i).g = 1;
			colorsArray(i).b = 1;
			break;

		case COLOR_YELLOW:
			colorsArray(i).name = COLOR_YELLOW;
			colorsArray(i).r = 1;
			colorsArray(i).g = 1;
			colorsArray(i).b = 0.2;
			break;

		case COLOR_PURPLE:
			colorsArray(i).name = COLOR_PURPLE;
			colorsArray(i).r = 0.6;
			colorsArray(i).g = 0.2;
			colorsArray(i).b = 1;
			break;

		case COLOR_GREEN:
			colorsArray(i).name = COLOR_GREEN;
			colorsArray(i).r = 0.2;
			colorsArray(i).g = 1;
			colorsArray(i).b = 0.2;
			break;

		case COLOR_RED:
			colorsArray(i).name = COLOR_RED;
			colorsArray(i).r = 1;
			colorsArray(i).g = 0;
			colorsArray(i).b = 0;
			break;

		case COLOR_BLUE:
			colorsArray(i).name = COLOR_BLUE;
			colorsArray(i).r = 0;
			colorsArray(i).g = 0;
			colorsArray(i).b = 1;
			break;

		case COLOR_ORANGE:
			colorsArray(i).name = COLOR_ORANGE;
			colorsArray(i).r = 1;
			colorsArray(i).g = 0.6;
			colorsArray(i).b = 0.2;
			break;

		case COLOR_BACKGROUND:
			colorsArray(i).name = COLOR_BACKGROUND;
			colorsArray(i).r = 0.2392;
			colorsArray(i).g = 0.2627;
			colorsArray(i).b = 0.298;
			break;

		case COLOR_GAME_PAUSED:
			colorsArray(i).name = COLOR_GAME_PAUSED;
			colorsArray(i).r = 0.6353;
			colorsArray(i).g = 0.6392;
			colorsArray(i).b = 0.6471;
			break;

		case COLOR_GAME_OVER:
			colorsArray(i).name = COLOR_GAME_PAUSED;
			colorsArray(i).r = 0; //0.349;
			colorsArray(i).g = 0;//0.3686;
			colorsArray(i).b = 0;//0.4;
			break;

		}
	}

}

color_t TetreesUtils::getColor(int ref)
{

	switch(ref){

	case COLOR_TRANSPARENT: return COLOR_TRANSPARENT;
	case COLOR_LIGHT_BLUE:  return COLOR_LIGHT_BLUE;
	case COLOR_YELLOW:      return COLOR_YELLOW;
	case COLOR_PURPLE:      return COLOR_PURPLE;
	case COLOR_GREEN:       return COLOR_GREEN;
	case COLOR_RED:         return COLOR_RED;
	case COLOR_BLUE:        return COLOR_BLUE;
	case COLOR_ORANGE:      return COLOR_ORANGE;
	case COLOR_BACKGROUND:  return COLOR_BACKGROUND;
	case COLOR_GAME_PAUSED:	return COLOR_GAME_PAUSED;
	case COLOR_GAME_OVER:	return COLOR_GAME_OVER;
	default: return COLOR_TRANSPARENT;
	}

}

std::string TetreesUtils::intToStr(unsigned num)
{

	std::ostringstream stream;
	stream << std::dec << num;
	return stream.str();

}

void TetreesUtils::startTetrominoesArray(piece_t tetrominoes[NUM_OF_TETROMINOES])
{

	for(int i = 0; i < NUM_OF_TETROMINOES; i++){

		switch(i){
			case TETROMINO_I: //Assembling the I-shaped tetromino
				/*{0, 0, 0, 0,											0 1 0 0
				   1, 1, 1, 1,		I tetromino shape					0 1 0 0
				   0, 0, 0, 0,											0 1 0 0
				   0, 0, 0, 0}											0 1 0 0
				*/

				tetrominoes[i].type = TETROMINO_I;
				tetrominoes[i].color = COLOR_LIGHT_BLUE;
				tetrominoes[i].rpos = RPOS_0;
				tetrominoes[i].boardCol = 4;
				tetrominoes[i].boardRow = 0;
				tetrominoes[i].reachedBottom = false;


				tetrominoes[i].shape.setMatrixDim(4, 4);
				for(unsigned r = 0; r < tetrominoes[i].shape.getRowHeight(); r++){
					for(unsigned c = 0; c < tetrominoes[i].shape.getColWidth(); c++){
						if(r == 1)
							tetrominoes[i].shape(r,c) = 1;
						else
							tetrominoes[i].shape(r,c) = 0;

					}
				}
				break;

			case TETROMINO_O: //Assembling the O-shaped tetromino
				/*{1, 1,                           _
				   1, 1}		O tetromino shape |_|
				*/
				tetrominoes[i].type = TETROMINO_O;
				tetrominoes[i].color = COLOR_YELLOW;
				tetrominoes[i].rpos = RPOS_0;
				tetrominoes[i].boardCol = 5;
				tetrominoes[i].boardRow = 1;
				tetrominoes[i].reachedBottom = false;


				tetrominoes[i].shape.setMatrixDim(2, 2);
				for(unsigned r = 0; r < tetrominoes[i].shape.getRowHeight(); r++){
					for(unsigned c = 0; c < tetrominoes[i].shape.getColWidth(); c++){
						tetrominoes[i].shape(r,c) = 1;
					}
				}
				break;

			case TETROMINO_T:
				/*0, 1, 0,
			      1, 1, 1,		T tetromino shape
			      0, 0, 0}
				*/
				tetrominoes[i].type = TETROMINO_T;
				tetrominoes[i].color = COLOR_PURPLE;
				tetrominoes[i].rpos = RPOS_0;
				tetrominoes[i].boardCol = 5;
				tetrominoes[i].boardRow = 1;
				tetrominoes[i].reachedBottom = false;


				tetrominoes[i].shape.setMatrixDim(3, 3);
				for(unsigned r = 0; r < tetrominoes[i].shape.getRowHeight(); r++){
					for(unsigned c = 0; c < tetrominoes[i].shape.getColWidth(); c++){
						if((r == 0 && c == 1) || (r == 1))
							tetrominoes[i].shape(r,c) = 1;
						else
							tetrominoes[i].shape(r,c) = 0;
					}
				}
				break;

			case TETROMINO_S:
				/*{0, 1, 1,												0,1,0
			       1, 1, 0,		S tetromino shape						0,1,1
			       0, 0, 0}												0,0,1
			    */
				tetrominoes[i].type = TETROMINO_S;
				tetrominoes[i].color = COLOR_GREEN;
				tetrominoes[i].rpos = RPOS_0;
				tetrominoes[i].boardCol = 5;
				tetrominoes[i].boardRow = 1;
				tetrominoes[i].reachedBottom = false;


				tetrominoes[i].shape.setMatrixDim(3, 3);
				for(unsigned r = 0; r < tetrominoes[i].shape.getRowHeight(); r++){
					for(unsigned c = 0; c < tetrominoes[i].shape.getColWidth(); c++){
						if((r == 0 && c >= 1) || (r == 1 && c <= 1))
							tetrominoes[i].shape(r,c) = 1;
						else
							tetrominoes[i].shape(r,c) = 0;
					}
				}
				break;

			case TETROMINO_Z:
				/*{1, 1, 0,
				   0, 1, 1,		Z tetromino shape
				   0, 0, 0}
				*/
				tetrominoes[i].type = TETROMINO_Z;
				tetrominoes[i].color = COLOR_RED;
				tetrominoes[i].rpos = RPOS_0;
				tetrominoes[i].boardCol = 5;
				tetrominoes[i].boardRow = 1;
				tetrominoes[i].reachedBottom = false;


				tetrominoes[i].shape.setMatrixDim(3, 3);
				for(unsigned r = 0; r < tetrominoes[i].shape.getRowHeight(); r++){
					for(unsigned c = 0; c < tetrominoes[i].shape.getColWidth(); c++){
						if((r == 0 && c <= 1) || (r == 1 && c >= 1))
							tetrominoes[i].shape(r,c) = 1;
						else
							tetrominoes[i].shape(r,c) = 0;
					}
				}
				break;

			case TETROMINO_J:
				/*{1, 0, 0,
				   1, 1, 1,		J tetromino shape
				   0, 0, 0}
				*/
				tetrominoes[i].type = TETROMINO_J;
				tetrominoes[i].color = COLOR_BLUE;
				tetrominoes[i].rpos = RPOS_0;
				tetrominoes[i].boardCol = 5;
				tetrominoes[i].boardRow = 1;
				tetrominoes[i].reachedBottom = false;


				tetrominoes[i].shape.setMatrixDim(3, 3);
				for(unsigned r = 0; r < tetrominoes[i].shape.getRowHeight(); r++){
					for(unsigned c = 0; c < tetrominoes[i].shape.getColWidth(); c++){
						if((r == 0 && c == 0) || r == 1)
							tetrominoes[i].shape(r,c) = 1;
						else
							tetrominoes[i].shape(r,c) = 0;
					}
				}
				break;

			case TETROMINO_L:
				/*{0, 0, 1,
				   1, 1, 1,		r tetromino shape
				   0, 0, 0}
				*/
				tetrominoes[i].type = TETROMINO_L;
				tetrominoes[i].color = COLOR_ORANGE;
				tetrominoes[i].rpos = RPOS_0;
				tetrominoes[i].boardCol = 5;
				tetrominoes[i].boardRow = 1;
				tetrominoes[i].reachedBottom = false;


				tetrominoes[i].shape.setMatrixDim(3, 3);
				for(unsigned r = 0; r < tetrominoes[i].shape.getRowHeight(); r++){
					for(unsigned c = 0; c < tetrominoes[i].shape.getColWidth(); c++){
						if((r == 0 && c == 2) || r == 1)
							tetrominoes[i].shape(r,c) = 1;
						else
							tetrominoes[i].shape(r,c) = 0;
					}
				}
				break;

		}

	}

}

void TetreesUtils::startGameLevelsArray(game_level_t levels[NUM_OF_LEVELS])
{

	for(int i = 0; i < NUM_OF_LEVELS; i++){

		switch(i){

		case LVL_1:
			levels[i].lvl = LVL_1;
			levels[i].nextLvlScore = NEXT_LVL_SCORE_LVL1;
			levels[i].gameSpeed = LVL_1_SPEED;

			levels[i].tetrominoesProportionArray[TETROMINO_I] = LVL1_TETROMINOES_PROPORTION[TETROMINO_I];
			levels[i].tetrominoesProportionArray[TETROMINO_O] = LVL1_TETROMINOES_PROPORTION[TETROMINO_O];
			levels[i].tetrominoesProportionArray[TETROMINO_T] = LVL1_TETROMINOES_PROPORTION[TETROMINO_T];
			levels[i].tetrominoesProportionArray[TETROMINO_S] = LVL1_TETROMINOES_PROPORTION[TETROMINO_S];
			levels[i].tetrominoesProportionArray[TETROMINO_Z] = LVL1_TETROMINOES_PROPORTION[TETROMINO_Z];
			levels[i].tetrominoesProportionArray[TETROMINO_J] = LVL1_TETROMINOES_PROPORTION[TETROMINO_J];
			levels[i].tetrominoesProportionArray[TETROMINO_L] = LVL1_TETROMINOES_PROPORTION[TETROMINO_L];
			break;

		case LVL_2:
			levels[i].lvl = LVL_2;
			levels[i].nextLvlScore = NEXT_LVL_SCORE_LVL2;
			levels[i].gameSpeed = LVL_2_SPEED;

			levels[i].tetrominoesProportionArray[TETROMINO_I] = LVL2_TETROMINOES_PROPORTION[TETROMINO_I];
			levels[i].tetrominoesProportionArray[TETROMINO_O] = LVL2_TETROMINOES_PROPORTION[TETROMINO_O];
			levels[i].tetrominoesProportionArray[TETROMINO_T] = LVL2_TETROMINOES_PROPORTION[TETROMINO_T];
			levels[i].tetrominoesProportionArray[TETROMINO_S] = LVL2_TETROMINOES_PROPORTION[TETROMINO_S];
			levels[i].tetrominoesProportionArray[TETROMINO_Z] = LVL2_TETROMINOES_PROPORTION[TETROMINO_Z];
			levels[i].tetrominoesProportionArray[TETROMINO_J] = LVL2_TETROMINOES_PROPORTION[TETROMINO_J];
			levels[i].tetrominoesProportionArray[TETROMINO_L] = LVL2_TETROMINOES_PROPORTION[TETROMINO_L];
			break;

		case LVL_3:
			levels[i].lvl = LVL_3;
			levels[i].nextLvlScore = NEXT_LVL_SCORE_LVL3;
			levels[i].gameSpeed = LVL_3_SPEED;

			levels[i].tetrominoesProportionArray[TETROMINO_I] = LVL3_TETROMINOES_PROPORTION[TETROMINO_I];
			levels[i].tetrominoesProportionArray[TETROMINO_O] = LVL3_TETROMINOES_PROPORTION[TETROMINO_O];
			levels[i].tetrominoesProportionArray[TETROMINO_T] = LVL3_TETROMINOES_PROPORTION[TETROMINO_T];
			levels[i].tetrominoesProportionArray[TETROMINO_S] = LVL3_TETROMINOES_PROPORTION[TETROMINO_S];
			levels[i].tetrominoesProportionArray[TETROMINO_Z] = LVL3_TETROMINOES_PROPORTION[TETROMINO_Z];
			levels[i].tetrominoesProportionArray[TETROMINO_J] = LVL3_TETROMINOES_PROPORTION[TETROMINO_J];
			levels[i].tetrominoesProportionArray[TETROMINO_L] = LVL3_TETROMINOES_PROPORTION[TETROMINO_L];
			break;

		case LVL_4:
			levels[i].lvl = LVL_4;
			levels[i].nextLvlScore = NEXT_LVL_SCORE_LVL4;
			levels[i].gameSpeed = LVL_4_SPEED;

			levels[i].tetrominoesProportionArray[TETROMINO_I] = LVL4_TETROMINOES_PROPORTION[TETROMINO_I];
			levels[i].tetrominoesProportionArray[TETROMINO_O] = LVL4_TETROMINOES_PROPORTION[TETROMINO_O];
			levels[i].tetrominoesProportionArray[TETROMINO_T] = LVL4_TETROMINOES_PROPORTION[TETROMINO_T];
			levels[i].tetrominoesProportionArray[TETROMINO_S] = LVL4_TETROMINOES_PROPORTION[TETROMINO_S];
			levels[i].tetrominoesProportionArray[TETROMINO_Z] = LVL4_TETROMINOES_PROPORTION[TETROMINO_Z];
			levels[i].tetrominoesProportionArray[TETROMINO_J] = LVL4_TETROMINOES_PROPORTION[TETROMINO_J];
			levels[i].tetrominoesProportionArray[TETROMINO_L] = LVL4_TETROMINOES_PROPORTION[TETROMINO_L];
			break;

		case LVL_5:
			levels[i].lvl = LVL_5;
			levels[i].nextLvlScore = NEXT_LVL_SCORE_LVL5;
			levels[i].gameSpeed = LVL_5_SPEED;

			levels[i].tetrominoesProportionArray[TETROMINO_I] = LVL5_TETROMINOES_PROPORTION[TETROMINO_I];
			levels[i].tetrominoesProportionArray[TETROMINO_O] = LVL5_TETROMINOES_PROPORTION[TETROMINO_O];
			levels[i].tetrominoesProportionArray[TETROMINO_T] = LVL5_TETROMINOES_PROPORTION[TETROMINO_T];
			levels[i].tetrominoesProportionArray[TETROMINO_S] = LVL5_TETROMINOES_PROPORTION[TETROMINO_S];
			levels[i].tetrominoesProportionArray[TETROMINO_Z] = LVL5_TETROMINOES_PROPORTION[TETROMINO_Z];
			levels[i].tetrominoesProportionArray[TETROMINO_J] = LVL5_TETROMINOES_PROPORTION[TETROMINO_J];
			levels[i].tetrominoesProportionArray[TETROMINO_L] = LVL5_TETROMINOES_PROPORTION[TETROMINO_L];
			break;

		case LVL_6:
			levels[i].lvl = LVL_6;
			levels[i].nextLvlScore = NEXT_LVL_SCORE_LVL6;
			levels[i].gameSpeed = LVL_6_SPEED;

			levels[i].tetrominoesProportionArray[TETROMINO_I] = LVL6_TETROMINOES_PROPORTION[TETROMINO_I];
			levels[i].tetrominoesProportionArray[TETROMINO_O] = LVL6_TETROMINOES_PROPORTION[TETROMINO_O];
			levels[i].tetrominoesProportionArray[TETROMINO_T] = LVL6_TETROMINOES_PROPORTION[TETROMINO_T];
			levels[i].tetrominoesProportionArray[TETROMINO_S] = LVL6_TETROMINOES_PROPORTION[TETROMINO_S];
			levels[i].tetrominoesProportionArray[TETROMINO_Z] = LVL6_TETROMINOES_PROPORTION[TETROMINO_Z];
			levels[i].tetrominoesProportionArray[TETROMINO_J] = LVL6_TETROMINOES_PROPORTION[TETROMINO_J];
			levels[i].tetrominoesProportionArray[TETROMINO_L] = LVL6_TETROMINOES_PROPORTION[TETROMINO_L];
			break;

		case LVL_7:
			levels[i].lvl = LVL_7;
			levels[i].nextLvlScore = NEXT_LVL_SCORE_LVL7;
			levels[i].gameSpeed = LVL_7_SPEED;

			levels[i].tetrominoesProportionArray[TETROMINO_I] = LVL7_TETROMINOES_PROPORTION[TETROMINO_I];
			levels[i].tetrominoesProportionArray[TETROMINO_O] = LVL7_TETROMINOES_PROPORTION[TETROMINO_O];
			levels[i].tetrominoesProportionArray[TETROMINO_T] = LVL7_TETROMINOES_PROPORTION[TETROMINO_T];
			levels[i].tetrominoesProportionArray[TETROMINO_S] = LVL7_TETROMINOES_PROPORTION[TETROMINO_S];
			levels[i].tetrominoesProportionArray[TETROMINO_Z] = LVL7_TETROMINOES_PROPORTION[TETROMINO_Z];
			levels[i].tetrominoesProportionArray[TETROMINO_J] = LVL7_TETROMINOES_PROPORTION[TETROMINO_J];
			levels[i].tetrominoesProportionArray[TETROMINO_L] = LVL7_TETROMINOES_PROPORTION[TETROMINO_L];
			break;

		case LVL_8:
			levels[i].lvl = LVL_8;
			levels[i].nextLvlScore = NEXT_LVL_SCORE_LVL8;
			levels[i].gameSpeed = LVL_8_SPEED;

			levels[i].tetrominoesProportionArray[TETROMINO_I] = LVL8_TETROMINOES_PROPORTION[TETROMINO_I];
			levels[i].tetrominoesProportionArray[TETROMINO_O] = LVL8_TETROMINOES_PROPORTION[TETROMINO_O];
			levels[i].tetrominoesProportionArray[TETROMINO_T] = LVL8_TETROMINOES_PROPORTION[TETROMINO_T];
			levels[i].tetrominoesProportionArray[TETROMINO_S] = LVL8_TETROMINOES_PROPORTION[TETROMINO_S];
			levels[i].tetrominoesProportionArray[TETROMINO_Z] = LVL8_TETROMINOES_PROPORTION[TETROMINO_Z];
			levels[i].tetrominoesProportionArray[TETROMINO_J] = LVL8_TETROMINOES_PROPORTION[TETROMINO_J];
			levels[i].tetrominoesProportionArray[TETROMINO_L] = LVL8_TETROMINOES_PROPORTION[TETROMINO_L];
			break;

		case LVL_9:
			levels[i].lvl = LVL_9;
			levels[i].nextLvlScore = NEXT_LVL_SCORE_LVL9;
			levels[i].gameSpeed = LVL_9_SPEED;

			levels[i].tetrominoesProportionArray[TETROMINO_I] = LVL9_TETROMINOES_PROPORTION[TETROMINO_I];
			levels[i].tetrominoesProportionArray[TETROMINO_O] = LVL9_TETROMINOES_PROPORTION[TETROMINO_O];
			levels[i].tetrominoesProportionArray[TETROMINO_T] = LVL9_TETROMINOES_PROPORTION[TETROMINO_T];
			levels[i].tetrominoesProportionArray[TETROMINO_S] = LVL9_TETROMINOES_PROPORTION[TETROMINO_S];
			levels[i].tetrominoesProportionArray[TETROMINO_Z] = LVL9_TETROMINOES_PROPORTION[TETROMINO_Z];
			levels[i].tetrominoesProportionArray[TETROMINO_J] = LVL9_TETROMINOES_PROPORTION[TETROMINO_J];
			levels[i].tetrominoesProportionArray[TETROMINO_L] = LVL9_TETROMINOES_PROPORTION[TETROMINO_L];
			break;

		case LVL_10:
			levels[i].lvl = LVL_10;
			levels[i].nextLvlScore = NEXT_LVL_SCORE_LVL10;
			levels[i].gameSpeed = LVL_10_SPEED;

			levels[i].tetrominoesProportionArray[TETROMINO_I] = LVL10_TETROMINOES_PROPORTION[TETROMINO_I];
			levels[i].tetrominoesProportionArray[TETROMINO_O] = LVL10_TETROMINOES_PROPORTION[TETROMINO_O];
			levels[i].tetrominoesProportionArray[TETROMINO_T] = LVL10_TETROMINOES_PROPORTION[TETROMINO_T];
			levels[i].tetrominoesProportionArray[TETROMINO_S] = LVL10_TETROMINOES_PROPORTION[TETROMINO_S];
			levels[i].tetrominoesProportionArray[TETROMINO_Z] = LVL10_TETROMINOES_PROPORTION[TETROMINO_Z];
			levels[i].tetrominoesProportionArray[TETROMINO_J] = LVL10_TETROMINOES_PROPORTION[TETROMINO_J];
			levels[i].tetrominoesProportionArray[TETROMINO_L] = LVL10_TETROMINOES_PROPORTION[TETROMINO_L];
			break;

		}

	}

}




















