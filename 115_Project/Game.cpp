#include "Game.h"

Game::Game()
{
    map = new Map();
}

void Game::run()
{
    string userInput;         // user controls
    bool Continue = true;   
    while (Continue)        // game loop
    {

        mainMenu();                 // print controls
        map->printMatrix();        // print map

        cout << "Input: ";
        getline(cin,userInput);

        if (userInput == "w")
        {
            map->movePlayerUp();      // move up
            map->moveEnemies();
        }
        else if (userInput == "s")
        {
            map->movePlayerDown();    // move down
            map->moveEnemies();
        }
        else if (userInput == "a")
        {
            map->movePlayerLeft();    // move left
            map->moveEnemies();
        }
        else if (userInput == "d")
        {
            map->movePlayerRight();   // move right
            map->moveEnemies();
        }
        else if (userInput == " ")
        {
            map->moveEnemies();      //skip turn
        }
        else if (userInput == "q")
        {
            Continue = false;        // quit
        }
        
        
        //map->moveEnemies();         // move enemy characters

        cin.clear();                // clear input buffer

        //system("CLS");              // clear console
    }
}

void Game::mainMenu()
{
    cout << "***********************************************************************************************" << endl;
    cout << "* Quit (q) | " << "Move Up (w) | " << "Move Down (s) | " << "Move Left (a) | " << "Move Right (d) | " << "Skip Turn (space) *" << endl;
    cout << "***********************************************************************************************" << endl;
}