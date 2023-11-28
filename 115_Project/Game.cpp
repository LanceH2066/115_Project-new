#include "Game.h"
#include <tuple>

Game::Game()
{
    map = new Map();
}

void Game::run()
{
    char userInput;         // user controls
    bool Continue = true;   
    while (Continue)        // game loop
    {

        mainMenu();                 // print controls
        map->printMatrix();        // print map

        cout << "Input: ";
        cin >> userInput;

        if (userInput == 'w')
        {
            map->movePlayerUp();      // move up
        }
        else if (userInput == 's')
        {
            map->movePlayerDown();    // move down
        }
        else if (userInput == 'a')
        {
            map->movePlayerLeft();    // move left
        }
        else if (userInput == 'd')
        {
            map->movePlayerRight();   // move right
        }
        else if (userInput == 'z')
        {
                                     // skip turn
        }
        else if (userInput == 'q')
        {
            Continue = false;        // quit
        }
        
        map->moveEnemies();         // move enemy characters

        cin.clear();                // clear input buffer
        system("CLS");              // clear console
    }
}

void Game::mainMenu()
{
    cout << "***********************************************************************************************" << endl;
    cout << "* Quit (q) | " << "Move Up (w) | " << "Move Down (s) | " << "Move Left (a) | " << "Move Right (d) | " << "Skip Turn (space) *" << endl;
    cout << "***********************************************************************************************" << endl;
}