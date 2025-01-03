#include "board.h"
#include "game.h"
#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <iostream>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

int MainLoop()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Chess");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	loadPieces(WINDOW_WIDTH, WINDOW_HEIGHT);
	initBoard(WINDOW_WIDTH, WINDOW_HEIGHT);
	Square hoveredSquare = {};
	Square heldSquare = {};

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(DARKGRAY);
        drawBoard();
		if(!IsMouseButtonDown(0)){
        	hoveredSquare = currSquare(WINDOW_WIDTH, WINDOW_HEIGHT, GetMouseX(), GetMouseY());
			if(hoveredSquare.piece != '0' && hoveredSquare.piece != '\0'){
				highlightAttackedSquares(hoveredSquare, WINDOW_WIDTH, WINDOW_HEIGHT);
			}
			if(heldSquare.row != -1 && hoveredSquare.row != -1){
				if(heldSquare.row != hoveredSquare.row || heldSquare.col != hoveredSquare.col){
					int startIndex = heldSquare.row * 8 + heldSquare.col;
					int newIndex = hoveredSquare.row * 8 + hoveredSquare.col;
					if(isLegalMove(startIndex, newIndex)){
						playMove(startIndex, newIndex);
						updateSquare(hoveredSquare, heldSquare.piece);
						updateSquare(heldSquare, '0');

						// TODO: change/redo
						// if(isPromotion(hoveredSquare.row * 8 + hoveredSquare.col, heldSquare.piece)){
						// 	promotePawn(hoveredSquare.row * 8 + hoveredSquare.col, 'Q');
						// 	updateSquare(hoveredSquare, 'q');
						// }
					}
				}
			}
			heldSquare = {};
		}
		
		if(IsMouseButtonDown(0) && hoveredSquare.piece != '0' && hoveredSquare.piece != '\0'){
			if(heldSquare.row = -1){
				heldSquare = hoveredSquare;
			}
			highlightAttackedSquares(heldSquare, WINDOW_WIDTH, WINDOW_HEIGHT);
			drawPieces(WINDOW_WIDTH, WINDOW_HEIGHT, heldSquare);
		}
		else{
			drawPieces(WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	unloadPieces();

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
