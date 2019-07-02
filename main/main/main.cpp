// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Game.h"
#ifndef LINUX
#include <windows.h>
#endif


/*
====================
Main
====================
*/

#ifndef LINUX
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
//int main()
{
    // ------ Vars ------

    // Class for drawing, using SDL for rendering
    IO mIO;
    int mScreenHeight = mIO.GetScreenHeight();

    // Pieces
    Pieces mPieces;

    // Board
    Board mBoard(&mPieces, mScreenHeight);

    // Game
    Game mGame(&mBoard, &mPieces, &mIO, mScreenHeight);

    // Get the actual clock milliseconds (SDL)
    unsigned long mTime1 = SDL_GetTicks();

    // ------ Main Loop ------
    while (!mIO.IsKeyDown(SDLK_ESCAPE))
    {
        // ------ Draw ------
        mIO.ClearScreen();      // Clear screen
        mGame.DrawScene();      // Draw stuff
        mIO.UpdateScreen();     // Put graphic context in screen

        // ------ Input ------
        int mKey = mIO.Pollkey();

        switch(mKey)
        {
            case (SDLK_RIGHT):
            {
                if (mBoard.IsPossibleMovement(mGame.mPosX + 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
                    mGame.mPosX++;
                break;
            }
            case (SDLK_LEFT):
            {
                if (mBoard.IsPossibleMovement(mGame.mPosX - 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
                    mGame.mPosX--;
                break;
             }
            case (SDLK_DOWN):
            {
                if (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
                    mGame.mPosY++;
                break;
            }
            case (SDLK_x):
            {
                // Check collision from up to down
                while (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation))
                {
                    mGame.mPosY++;
                }
                mBoard.StorePiece(mGame.mPosX, mGame.mPosY - 1, mGame.mPiece, mGame.mRotation);
                mBoard.DeletePossibleLines();

                if (mBoard.IsGameOver())
                {
                    mIO.Getkey();
                    exit(0);
                }
                mGame.CreateNewPiece();
                break;
            }
            case (SDLK_z):
            {
                if (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY, mGame.mPiece, (mGame.mRotation + 1) % 4))
                    mGame.mRotation = (mGame.mRotation + 1) % 4;
                break;
            }
        }

        // ------ Vertical movement ------
        unsigned long mTime2 = SDL_GetTicks();
        if ((mTime2 - mTime1) > WAIT_TIME)
        {
            if (mBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
            {
                mGame.mPosY++;
            }
            else
            {
                mBoard.StorePiece(mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation);
                mBoard.DeletePossibleLines();

                if (mBoard.IsGameOver())
                {
                    mIO.Getkey();
                    exit(0);
                }
                mGame.CreateNewPiece();
            }
            mTime1 = SDL_GetTicks();
        }
    }
    return 0;
}

