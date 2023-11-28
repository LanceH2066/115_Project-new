#include "Map.h"
#include<tuple>
#include<vector>
#include<fstream>
using namespace std;
#include<iostream>



const double WALL_WEIGHT = 100;         // #
const double PLAIN_WEIGHT = 1;         // ' '
const double GRASS_WEIGHT = 2;         // -
const double ENEMY_WEIGHT = 100;        //X
const double PLAYER_WEIGHT = 0;         //O
const double HIDDEN_WEIGHT = 100;       //enemies can not go through hidden 

Map::Map() 
{
    string filename = "Map.txt";

    ifstream file(filename);  // Open the file for reading

    if (!file.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return;
    }

    vector<string> lines;
    numRows = 0;
    numCol = 0;

    readMapFromFile(file, lines, numRows, numCol);
    createMapMatrix(lines, numRows, numCol);
    populateGraph();


    //displayMap();       //prints node weight and landtype

    //cout << "\n\n";
    //myMap->Display();   //prints adjacency list of graph

    file.close();  // Close the file when done
}

//Map Constructor Functions
void Map::readMapFromFile(ifstream& file, vector<string>& lines, int& numRows, int& numCol) {
    string line;
    int maxColumns = 0;

    while (getline(file, line)) {
        lines.push_back(line);
        maxColumns = max(maxColumns, static_cast<int>(line.length()));
    }

    numRows = static_cast<int>(lines.size());
    numCol = maxColumns;
}

void Map::createMapMatrix(const vector<string>& lines, int numRows, int numCol) {
    mapMatrix = new char* [numRows];
    for (int i = 0; i < numRows; i++) {
        mapMatrix[i] = new char[numCol];
    }

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCol; j++) {
            if (j < lines[i].length()) {
                mapMatrix[i][j] = lines[i][j];
            }
            else {
                mapMatrix[i][j] = '#';
            }
        }
    }
}

void Map::populateGraph() {
    myMap = new GraphAL(numRows * numCol);
    int enemyCount = 0;

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCol; j++) {
            char currentChar = mapMatrix[i][j];
            int currentNode = i * numCol + j;

            double weight = getSymbolWeight(currentChar);
            myMap->setNodeWeight(currentNode, weight);
            myMap->setLandType(currentNode, currentChar);

            addEdges(currentNode, weight, i, j);

            initializeEntities(currentChar, i, j, currentNode, enemyCount);
        }
    }
}

void Map::addEdges(int currentNode, double currentWeight, int i, int j) {
    addDirectedEdge(currentNode, currentWeight, i - 1, j);
    addDirectedEdge(currentNode, currentWeight, i + 1, j);
    addDirectedEdge(currentNode, currentWeight, i, j - 1);
    addDirectedEdge(currentNode, currentWeight, i, j + 1);
}

void Map::addDirectedEdge(int currentNode, double currentWeight, int i, int j) {
    if (i >= 0 && i < numRows && j >= 0 && j < numCol) {
        int adjacentNode = i * numCol + j;
        char adjacentChar = mapMatrix[i][j];
        double adjWeight = getSymbolWeight(adjacentChar);

        myMap->setDirectedEdge(currentNode, adjacentNode, adjWeight, adjacentChar);
        myMap->setDirectedEdge(adjacentNode, currentNode, currentWeight, mapMatrix[i][j]);
    }
}

void Map::initializeEntities(char currentChar, int i, int j, int currentNode, int& enemyCount) {
    if (currentChar == 'O') 
    {
        player = new Player(i, j, myMap->retrieveEdge(currentNode, currentNode));
        //player->printPos();
    }

    if (currentChar == 'X' && enemyCount < 3) 
    {
        createEnemy(i, j, currentNode, enemyCount);
    }

}

void Map::createEnemy(int i, int j, int currentNode, int& enemyCount) {
    if (enemyCount == 0) {
        enemy1 = new Enemy(i, j, myMap->retrieveEdge(currentNode, currentNode));
        //enemy1->printPos();
    }
    else if (enemyCount == 1) {
        enemy2 = new Enemy(i, j, myMap->retrieveEdge(currentNode, currentNode));
        //enemy2->printPos();
    }
    else if (enemyCount == 2) {
        enemy3 = new Enemy(i, j, myMap->retrieveEdge(currentNode, currentNode));
        //enemy3->printPos();
    }
    enemyCount++;
}

//End of Map Constructor Functions

void Map::printMatrix() {
    //cout << "Debug: numRows = " << numRows << ", numCol = " << numCol << endl;  // Debug output
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCol; j++) {
            cout << mapMatrix[i][j];
        }
        cout << endl;
    }
    //cout << "Debug: End of printMatrix function" << endl;  // Debug output

}

//Map::Map(int n)
//{
//	mapSize = n;
//	myMap = new GraphAL(n);
//	
//}

void Map::displayMap()
{
    cout << "displayMap" << endl;
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCol; j++) {
            int currentNode = i * numCol + j;
            char landtype = myMap->getNodeLandtype(currentNode);
         
            double weight = myMap->getNodeWeight(currentNode);
            cout << "Node " << currentNode << " Weight: " << weight << " Landtype: " << landtype <<endl;
        }
        cout << endl;
    }
}

nodeAL* Map::getNode(int vertex)
{
	return myMap->retrieveEdge(vertex, vertex);
}

//-----------------------------------------------------------------
tuple<int, int> countRowsAndColumns(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open the file." << endl;
        return make_tuple(0, 0);
    }

    vector<string> lines;
    string line;
    int maxColumns = 0;

    while (getline(file, line)) {
        lines.push_back(line);
        maxColumns = max(maxColumns, static_cast<int>(line.length()));
    }

    int numRows = static_cast<int>(lines.size());
    return make_tuple(numRows, maxColumns);
}

double getSymbolWeight(char symbol) 
{
    switch (symbol) 
    {
        case '#': return WALL_WEIGHT;
        case 'H': return HIDDEN_WEIGHT;
        case'-': return GRASS_WEIGHT;
        case'X' : return ENEMY_WEIGHT;
        case'O': return PLAYER_WEIGHT;
        case' ': return PLAIN_WEIGHT;
        default: return 1.0;

    }
}

void Map::movePlayerUp()
{
    int current = player->playerNode->vertex;
    int target = player->playerNode->vertex - numCol;

    if (myMap->retrieveEdge(target,target)->landType != '#' )
    {
        // update matrix
        mapMatrix[player->xPos][player->yPos] = player->beneathPlayerNode->landType;
        player->xPos--;
        mapMatrix[player->xPos][player->yPos] = player->playerNode->landType;

        // update graph
        myMap->retrieveEdge(current, current)->landType = player->beneathPlayerNode->landType;
        myMap->retrieveEdge(current, current)->weight = player->beneathPlayerNode->weight;

        player->beneathPlayerNode->landType = myMap->retrieveEdge(target, target)->landType;
        player->beneathPlayerNode->weight = myMap->retrieveEdge(target, target)->weight;
        player->beneathPlayerNode->vertex = myMap->retrieveEdge(target, target)->vertex;

        myMap->retrieveEdge(target, target)->landType = player->playerNode->landType;
        myMap->retrieveEdge(target, target)->weight = player->playerNode->weight;

        // update player class
        player->playerNode->vertex = target;
    }
    else
    {
        cout << "Thats a wall dude" << endl;
    }
}

void Map::movePlayerDown()
{
    int current = player->playerNode->vertex;
    int target = player->playerNode->vertex + numCol;

    if (myMap->retrieveEdge(target, target)->landType != '#')
    {
        // update matrix
        mapMatrix[player->xPos][player->yPos] = player->beneathPlayerNode->landType;
        player->xPos++;
        mapMatrix[player->xPos][player->yPos] = player->playerNode->landType;

        // update graph
        myMap->retrieveEdge(current, current)->landType = player->beneathPlayerNode->landType;
        myMap->retrieveEdge(current, current)->weight = player->beneathPlayerNode->weight;

        player->beneathPlayerNode->landType = myMap->retrieveEdge(target, target)->landType;
        player->beneathPlayerNode->weight = myMap->retrieveEdge(target, target)->weight;
        player->beneathPlayerNode->vertex = myMap->retrieveEdge(target, target)->vertex;

        myMap->retrieveEdge(target, target)->landType = player->playerNode->landType;        
        myMap->retrieveEdge(target, target)->weight = player->playerNode->weight;

        // update player class
        player->playerNode->vertex = target;
    }
    else
    {
        cout << "Thats a wall dude" << endl;
    }
}

void Map::movePlayerLeft()
{
    int current = player->playerNode->vertex;
    int target = player->playerNode->vertex-1;

    if (myMap->retrieveEdge(target, target)->landType != '#')
    {
        // update matrix
        mapMatrix[player->xPos][player->yPos] = player->beneathPlayerNode->landType;
        player->yPos--;
        mapMatrix[player->xPos][player->yPos] = player->playerNode->landType;

        // update graph
        myMap->retrieveEdge(current, current)->landType = player->beneathPlayerNode->landType;
        myMap->retrieveEdge(current, current)->weight = player->beneathPlayerNode->weight;

        player->beneathPlayerNode->landType = myMap->retrieveEdge(target, target)->landType;
        player->beneathPlayerNode->weight = myMap->retrieveEdge(target, target)->weight;
        player->beneathPlayerNode->vertex = myMap->retrieveEdge(target, target)->vertex;

        myMap->retrieveEdge(target, target)->landType = player->playerNode->landType;
        myMap->retrieveEdge(target, target)->weight = player->playerNode->weight;

        // update player class
        player->playerNode->vertex = target;
    }
    else
    {
        cout << "Thats a wall dude" << endl;
    }
}

void Map::movePlayerRight()
{
    int current = player->playerNode->vertex;
    int target = player->playerNode->vertex + 1;

    if (myMap->retrieveEdge(target, target)->landType != '#')
    {
        // update matrix
        mapMatrix[player->xPos][player->yPos] = player->beneathPlayerNode->landType;
        player->yPos++;
        mapMatrix[player->xPos][player->yPos] = player->playerNode->landType;

        // update graph
        myMap->retrieveEdge(current, current)->landType = player->beneathPlayerNode->landType;
        myMap->retrieveEdge(current, current)->weight = player->beneathPlayerNode->weight;

        player->beneathPlayerNode->landType = myMap->retrieveEdge(target, target)->landType;
        player->beneathPlayerNode->weight = myMap->retrieveEdge(target, target)->weight;
        player->beneathPlayerNode->vertex = myMap->retrieveEdge(target, target)->vertex;

        myMap->retrieveEdge(target, target)->landType = player->playerNode->landType;
        myMap->retrieveEdge(target, target)->weight = player->playerNode->weight;

        // update player class
        player->playerNode->vertex = target;
    }
    else
    {
        cout << "Thats a wall dude" << endl;
    }
}

void Map::moveEnemies()
{
    moveEnemy1();
    moveEnemy2();
    moveEnemy3();
}

void Map::moveEnemy1()
{
    double* d;
    int* pi;

    int enemyNodeVertex = enemy1->enemyNode->vertex;
    int playerNodeVertex = player->playerNode->vertex;

    myMap->shortestPath(enemyNodeVertex, d, pi);

    if (pi[playerNodeVertex] == -1)
    {
        cout << "No path from enemy to player" << endl;
    }
    else
    {
        // Trace back the path to the player
        vector<int> path;
        while (playerNodeVertex != enemyNodeVertex)
        {
            path.push_back(playerNodeVertex);
            playerNodeVertex = pi[playerNodeVertex];
        }

        // Move enemy1 to the next position on the path
        if (!path.empty())
        {
            int nextPosition = path.back();
            path.pop_back();

            // Output the path
            cout << "Shortest path from enemy1 to player: ";
            for (int i = path.size() - 1; i >= 0; --i)
            {
                cout << path[i];
                if (i > 0)
                    cout << " <- ";
            }
            cout << endl;

            // Update matrix
            mapMatrix[enemy1->xPos][enemy1->yPos] = enemy1->beneathEnemyNode->landType;
            enemy1->xPos = nextPosition / numCol;
            enemy1->yPos = nextPosition % numCol;
            mapMatrix[enemy1->xPos][enemy1->yPos] = enemy1->enemyNode->landType;

            // Update graph
            int current = enemyNodeVertex;
            int target = nextPosition;
            myMap->retrieveEdge(current, current)->landType = enemy1->beneathEnemyNode->landType;
            myMap->retrieveEdge(current, current)->weight = enemy1->beneathEnemyNode->weight;
            enemy1->beneathEnemyNode->landType = myMap->retrieveEdge(target, target)->landType;
            enemy1->beneathEnemyNode->weight = myMap->retrieveEdge(target, target)->weight;
            enemy1->beneathEnemyNode->vertex = myMap->retrieveEdge(target, target)->vertex;
            myMap->retrieveEdge(target, target)->landType = enemy1->enemyNode->landType;
            myMap->retrieveEdge(target, target)->weight = enemy1->enemyNode->weight;
            enemy1->enemyNode->vertex = target;

        }
    }

    delete[] d;
    delete[] pi;
}

void Map::moveEnemy2()
{
    double* d;
    int* pi;

    int enemyNodeVertex = enemy2->enemyNode->vertex;
    int playerNodeVertex = player->playerNode->vertex;

    myMap->shortestPath(enemyNodeVertex, d, pi);

    if (pi[playerNodeVertex] == -1)
    {
        cout << "No path from enemy to player" << endl;
    }
    else
    {
        // Trace back the path to the player
        vector<int> path;
        while (playerNodeVertex != enemyNodeVertex)
        {
            path.push_back(playerNodeVertex);
            playerNodeVertex = pi[playerNodeVertex];
        }

        // Move enemy2 to the next position on the path
        if (!path.empty())
        {
            int nextPosition = path.back();
            path.pop_back();

            // Output the path
            cout << "Shortest path from enemy2 to player: ";
            for (int i = path.size() - 1; i >= 0; --i)
            {
                cout << path[i];
                if (i > 0)
                    cout << " <- ";
            }
            cout << endl;

            // Update matrix
            mapMatrix[enemy2->xPos][enemy2->yPos] = enemy2->beneathEnemyNode->landType;
            enemy2->xPos = nextPosition / numCol;
            enemy2->yPos = nextPosition % numCol;
            mapMatrix[enemy2->xPos][enemy2->yPos] = enemy2->enemyNode->landType;

            // Update graph
            int current = enemyNodeVertex;
            int target = nextPosition;
            myMap->retrieveEdge(current, current)->landType = enemy2->beneathEnemyNode->landType;
            myMap->retrieveEdge(current, current)->weight = enemy2->beneathEnemyNode->weight;
            enemy2->beneathEnemyNode->landType = myMap->retrieveEdge(target, target)->landType;
            enemy2->beneathEnemyNode->weight = myMap->retrieveEdge(target, target)->weight;
            enemy2->beneathEnemyNode->vertex = myMap->retrieveEdge(target, target)->vertex;
            myMap->retrieveEdge(target, target)->landType = enemy2->enemyNode->landType;
            myMap->retrieveEdge(target, target)->weight = enemy2->enemyNode->weight;
            enemy2->enemyNode->vertex = target;

        }
    }

    delete[] d;
    delete[] pi;
}

void Map::moveEnemy3()
{
    double* d;
    int* pi;

    int enemyNodeVertex = enemy3->enemyNode->vertex;
    int playerNodeVertex = player->playerNode->vertex;

    myMap->shortestPath(enemyNodeVertex, d, pi);

    if (pi[playerNodeVertex] == -1)
    {
        cout << "No path from enemy to player" << endl;
    }
    else
    {
        // Trace back the path to the player
        vector<int> path;
        while (playerNodeVertex != enemyNodeVertex)
        {
            path.push_back(playerNodeVertex);
            playerNodeVertex = pi[playerNodeVertex];
        }

        // Move enemy3 to the next position on the path
        if (!path.empty())
        {
            int nextPosition = path.back();
            path.pop_back();

            // Output the path
            cout << "Shortest path from enemy3 to player: ";
            for (int i = path.size() - 1; i >= 0; --i)
            {
                cout << path[i];
                if (i > 0)
                    cout << " <- ";
            }
            cout << endl;

            // Update matrix
            mapMatrix[enemy3->xPos][enemy3->yPos] = enemy3->beneathEnemyNode->landType;
            enemy3->xPos = nextPosition / numCol;
            enemy3->yPos = nextPosition % numCol;
            mapMatrix[enemy3->xPos][enemy3->yPos] = enemy3->enemyNode->landType;

            // Update graph
            int current = enemyNodeVertex;
            int target = nextPosition;
            myMap->retrieveEdge(current, current)->landType = enemy3->beneathEnemyNode->landType;
            myMap->retrieveEdge(current, current)->weight = enemy3->beneathEnemyNode->weight;
            enemy3->beneathEnemyNode->landType = myMap->retrieveEdge(target, target)->landType;
            enemy3->beneathEnemyNode->weight = myMap->retrieveEdge(target, target)->weight;
            enemy3->beneathEnemyNode->vertex = myMap->retrieveEdge(target, target)->vertex;
            myMap->retrieveEdge(target, target)->landType = enemy3->enemyNode->landType;
            myMap->retrieveEdge(target, target)->weight = enemy3->enemyNode->weight;
            enemy3->enemyNode->vertex = target;

        }
    }

    delete[] d;
    delete[] pi;
}

void Map::resetPlayer()
{

}