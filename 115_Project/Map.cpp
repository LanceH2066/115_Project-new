#include "Map.h"
#include<tuple>
#include<vector>
#include<fstream>
using namespace std;
#include<iostream>
#include<ctime>
#include<cstdlib>



const double WALL_WEIGHT = 100;         // #
const double PLAIN_WEIGHT = 1;         // ' '
const double GRASS_WEIGHT = 2;         // -
const double ENEMY_WEIGHT = 100;        //X
const double PLAYER_WEIGHT = 1;         //O
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
    //cout << "\n\n";

    //print hidden tiles
    cout << "Hidden Tiles: ";
    for (int i : hiddenTiles) {
        cout << i << " ";
    }
    cout << endl;


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
        myMap->setDirectedEdge(adjacentNode, currentNode, currentWeight, myMap->retrieveEdge(currentNode, currentNode)->landType);
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

    if (currentChar == 'H') {
        hiddenTiles.push_back(currentNode);
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

void Map::updateEdges(int current, int target)
{
    // Edges going into current 
    myMap->retrieveEdge(current - numCol, current)->landType = myMap->getNodeLandtype(current);
    myMap->retrieveEdge(current - numCol, current)->weight = myMap->getNodeWeight(current);

    myMap->retrieveEdge(current + numCol, current)->landType = myMap->getNodeLandtype(current);
    myMap->retrieveEdge(current + numCol, current)->weight = myMap->getNodeWeight(current);

    myMap->retrieveEdge(current - 1, current)->landType = myMap->getNodeLandtype(current);
    myMap->retrieveEdge(current - 1, current)->weight = myMap->getNodeWeight(current);

    myMap->retrieveEdge(current + 1, current)->landType = myMap->getNodeLandtype(current);
    myMap->retrieveEdge(current + 1, current)->weight = myMap->getNodeWeight(current);

    // Edges going into target
    myMap->retrieveEdge(target - numCol, target)->landType = myMap->getNodeLandtype(target);
    myMap->retrieveEdge(target - numCol, target)->weight = myMap->getNodeWeight(target);

    myMap->retrieveEdge(target + numCol, target)->landType = myMap->getNodeLandtype(target);
    myMap->retrieveEdge(target + numCol, target)->weight = myMap->getNodeWeight(target);

    myMap->retrieveEdge(target - 1, target)->landType = myMap->getNodeLandtype(target);
    myMap->retrieveEdge(target - 1, target)->weight = myMap->getNodeWeight(target);

    myMap->retrieveEdge(target + 1, target)->landType = myMap->getNodeLandtype(target);
    myMap->retrieveEdge(target + 1, target)->weight = myMap->getNodeWeight(target);

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

        // update nodes
        myMap->retrieveEdge(current, current)->landType = player->beneathPlayerNode->landType;
        myMap->retrieveEdge(current, current)->weight = player->beneathPlayerNode->weight;

        player->beneathPlayerNode->landType = myMap->retrieveEdge(target, target)->landType;
        player->beneathPlayerNode->weight = myMap->retrieveEdge(target, target)->weight;
        player->beneathPlayerNode->vertex = myMap->retrieveEdge(target, target)->vertex;

        myMap->retrieveEdge(target, target)->landType = player->playerNode->landType;
        myMap->retrieveEdge(target, target)->weight = player->playerNode->weight;

        // update edges
        updateEdges(current, target);

        // update player vertex
        player->playerNode->vertex = target;
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

        // update edges
        updateEdges(current, target);

        // update player vertex
        player->playerNode->vertex = target;
    }

}

void Map::movePlayerLeft()
{
    int current = player->playerNode->vertex;
    int target = player->playerNode->vertex - 1;

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

        // update edges
        updateEdges(current, target);

        // update player vertex
        player->playerNode->vertex = target;
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

        // update edges
        updateEdges(current, target);

        // update player vertex
        player->playerNode->vertex = target;
    }
}

void Map::movePlayerToHidden() 
{
    int currentVertex = player->playerNode->vertex;
    int targetVertex = hiddenTiles [rand() % hiddenTiles.size()];

    int newX = targetVertex / numCol;
    int newY = targetVertex % numCol;

    player->xPos = newX;
    player->yPos = newY;

    //update Matrix
    mapMatrix[newX][newY] = player->playerNode->landType;

    //update graph

    player->beneathPlayerNode->landType = myMap->retrieveEdge(targetVertex, targetVertex)->landType;
    player->beneathPlayerNode->weight = myMap->retrieveEdge(targetVertex, targetVertex)->weight;
    player->beneathPlayerNode->vertex = myMap->retrieveEdge(targetVertex, targetVertex)->vertex;

    myMap->retrieveEdge(targetVertex, targetVertex)->landType = player->playerNode->landType;
    myMap->retrieveEdge(targetVertex, targetVertex)->weight = player->playerNode->weight;

    // Update Edges
    updateEdges(currentVertex, targetVertex);

    //update player vertex
    player->playerNode->vertex = targetVertex;


}

void Map::moveEnemies()
{
    if(player->beneathPlayerNode->landType != 'H')
    {
		moveEnemy1();

		if (player->beneathPlayerNode->landType != 'H')
		{
			moveEnemy2();
		}
        else
        {
			moveEnemyRand(enemy2);
        }
            
		if (player->beneathPlayerNode->landType != 'H')
		{
			moveEnemy3();
		}
		else
		{
			moveEnemyRand(enemy3);
		}
    }
    else
    {
        //randmove
        moveEnemyRand(enemy1);
        moveEnemyRand(enemy2);
        moveEnemyRand(enemy3);
    }

}

void Map::moveEnemyRand(Enemy* enemy) 
{
    bool moved = false;
    
    while (!moved)
    {
        int randNum = rand() % 4;

        if ((randNum == 0) && (isValidNode(enemy->enemyNode->vertex - numCol) == true)) // MOVE UP
        {
			int current = enemy->enemyNode->vertex;
			int target = enemy->enemyNode->vertex - numCol;

				// update matrix
				mapMatrix[enemy->xPos][enemy->yPos] = enemy->beneathEnemyNode->landType;
				enemy->xPos--;
				mapMatrix[enemy->xPos][enemy->yPos] = enemy->enemyNode->landType;

				// update graph
				myMap->retrieveEdge(current, current)->landType = enemy->beneathEnemyNode->landType;
				myMap->retrieveEdge(current, current)->weight = enemy->beneathEnemyNode->weight;

				enemy->beneathEnemyNode->landType = myMap->retrieveEdge(target, target)->landType;
				enemy->beneathEnemyNode->weight = myMap->retrieveEdge(target, target)->weight;
				enemy->beneathEnemyNode->vertex = myMap->retrieveEdge(target, target)->vertex;

				myMap->retrieveEdge(target, target)->landType = enemy->enemyNode->landType;
				myMap->retrieveEdge(target, target)->weight = enemy->enemyNode->weight;

                // update edges
                updateEdges(current,target);

				// update enemy class
				enemy->enemyNode->vertex = target;

            moved = true;
        }
        else if ((randNum == 1) && (isValidNode(enemy->enemyNode->vertex + numCol) == true)) // MOVE DOWN
        {
			int current = enemy->enemyNode->vertex;
			int target = enemy->enemyNode->vertex + numCol;

			// update matrix
			mapMatrix[enemy->xPos][enemy->yPos] = enemy->beneathEnemyNode->landType;
			enemy->xPos++;
			mapMatrix[enemy->xPos][enemy->yPos] = enemy->enemyNode->landType;

			// update graph
			myMap->retrieveEdge(current, current)->landType = enemy->beneathEnemyNode->landType;
			myMap->retrieveEdge(current, current)->weight = enemy->beneathEnemyNode->weight;

			enemy->beneathEnemyNode->landType = myMap->retrieveEdge(target, target)->landType;
			enemy->beneathEnemyNode->weight = myMap->retrieveEdge(target, target)->weight;
			enemy->beneathEnemyNode->vertex = myMap->retrieveEdge(target, target)->vertex;

			myMap->retrieveEdge(target, target)->landType = enemy->enemyNode->landType;
			myMap->retrieveEdge(target, target)->weight = enemy->enemyNode->weight;

            // update edges
            updateEdges(current, target);

			// update enemy class
			enemy->enemyNode->vertex = target;

			moved = true;
        }
		else if ((randNum == 2) && (isValidNode(enemy->enemyNode->vertex - 1) == true)) // MOVE LEFT
		{
			int current = enemy->enemyNode->vertex;
			int target = enemy->enemyNode->vertex - 1;

			// update matrix
			mapMatrix[enemy->xPos][enemy->yPos] = enemy->beneathEnemyNode->landType;
			enemy->yPos--;
			mapMatrix[enemy->xPos][enemy->yPos] = enemy->enemyNode->landType;

			// update graph
			myMap->retrieveEdge(current, current)->landType = enemy->beneathEnemyNode->landType;
			myMap->retrieveEdge(current, current)->weight = enemy->beneathEnemyNode->weight;

			enemy->beneathEnemyNode->landType = myMap->retrieveEdge(target, target)->landType;
			enemy->beneathEnemyNode->weight = myMap->retrieveEdge(target, target)->weight;
			enemy->beneathEnemyNode->vertex = myMap->retrieveEdge(target, target)->vertex;

			myMap->retrieveEdge(target, target)->landType = enemy->enemyNode->landType;
			myMap->retrieveEdge(target, target)->weight = enemy->enemyNode->weight;

            // update edges
            updateEdges(current, target);

			// update enemy class
			enemy->enemyNode->vertex = target;

            moved = true;
		}
		else if ((randNum == 3) && (isValidNode(enemy->enemyNode->vertex + 1) == true)) // MOVE RIGHT
		{
			int current = enemy->enemyNode->vertex;
			int target = enemy->enemyNode->vertex + 1;

			// update matrix
			mapMatrix[enemy->xPos][enemy->yPos] = enemy->beneathEnemyNode->landType;
			enemy->yPos++;
			mapMatrix[enemy->xPos][enemy->yPos] = enemy->enemyNode->landType;

			// update graph
			myMap->retrieveEdge(current, current)->landType = enemy->beneathEnemyNode->landType;
			myMap->retrieveEdge(current, current)->weight = enemy->beneathEnemyNode->weight;

			enemy->beneathEnemyNode->landType = myMap->retrieveEdge(target, target)->landType;
			enemy->beneathEnemyNode->weight = myMap->retrieveEdge(target, target)->weight;
			enemy->beneathEnemyNode->vertex = myMap->retrieveEdge(target, target)->vertex;

			myMap->retrieveEdge(target, target)->landType = enemy->enemyNode->landType;
			myMap->retrieveEdge(target, target)->weight = enemy->enemyNode->weight;

            // update edges
            updateEdges(current, target);

			// update enemy class
			enemy->enemyNode->vertex = target;

            moved = true;
		}
        else
        {
            cout << randNum;
        }
    }
}

bool Map::isValidNode(int target) {
    cout << "Target: " << target << endl;
    if (myMap->retrieveEdge(target, target)->landType == 'H') {
        return false;
    }
    else if (myMap->retrieveEdge(target, target)->landType == 'X') {
        return false;
    }
    else if (myMap->retrieveEdge(target, target)->landType == '#') {
        return false;
    }
    else if (myMap->retrieveEdge(target, target)->landType == 'O' && player->beneathPlayerNode->landType == 'H') {
        return false;
    }
    else {
        return true;
    }
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

            //check if another enemy is already in the nextPosition 
            int potentialX = nextPosition / numCol;
            int potentialY = nextPosition % numCol;
            if (mapMatrix[potentialX][potentialY] == 'X') {
                return;
            }

            // Update matrix
            mapMatrix[enemy1->xPos][enemy1->yPos] = enemy1->beneathEnemyNode->landType;
            enemy1->xPos = nextPosition / numCol;
            enemy1->yPos = nextPosition % numCol;
            mapMatrix[enemy1->xPos][enemy1->yPos] = enemy1->enemyNode->landType;

            // Update graph
            int current = enemyNodeVertex;
            int target = nextPosition;

            //enemy has found the player
            if (myMap->retrieveEdge(target, target)->landType == 'O') {    
                //update current position for when enemy moves
                myMap->retrieveEdge(current, current)->landType = enemy1->beneathEnemyNode->landType;
                myMap->retrieveEdge(current, current)->weight = enemy1->beneathEnemyNode->weight;

                //change enemy to have beneathPlayerNode values
                enemy1->beneathEnemyNode->landType = player->beneathPlayerNode->landType;
                enemy1->beneathEnemyNode->weight = player->beneathPlayerNode->weight;
                enemy1->beneathEnemyNode->vertex = player->beneathPlayerNode->vertex;

                myMap->retrieveEdge(target, target)->landType = enemy1->enemyNode->landType;
                myMap->retrieveEdge(target, target)->weight = enemy1->enemyNode->weight;
                enemy1->enemyNode->vertex = target;

                updateEdges(current, target);

                //move player to random H
                movePlayerToHidden();
                return;
            }

            myMap->retrieveEdge(current, current)->landType = enemy1->beneathEnemyNode->landType;
            myMap->retrieveEdge(current, current)->weight = enemy1->beneathEnemyNode->weight;
            enemy1->beneathEnemyNode->landType = myMap->retrieveEdge(target, target)->landType;
            enemy1->beneathEnemyNode->weight = myMap->retrieveEdge(target, target)->weight;
            enemy1->beneathEnemyNode->vertex = myMap->retrieveEdge(target, target)->vertex;
            myMap->retrieveEdge(target, target)->landType = enemy1->enemyNode->landType;
            myMap->retrieveEdge(target, target)->weight = enemy1->enemyNode->weight;
            enemy1->enemyNode->vertex = target;

            updateEdges(current, target);
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

            //check if another enemy is already in the nextPosition 
            int potentialX = nextPosition / numCol;
            int potentialY = nextPosition % numCol;
            if (mapMatrix[potentialX][potentialY] == 'X') {
                return;
            }

            // Update matrix
            mapMatrix[enemy2->xPos][enemy2->yPos] = enemy2->beneathEnemyNode->landType;
            enemy2->xPos = nextPosition / numCol;
            enemy2->yPos = nextPosition % numCol;
            mapMatrix[enemy2->xPos][enemy2->yPos] = enemy2->enemyNode->landType;

            // Update graph
            int current = enemyNodeVertex;
            int target = nextPosition;

            //enemy has found the player
            if (myMap->retrieveEdge(target, target)->landType == 'O') {
                //update current position for when enemy moves
                myMap->retrieveEdge(current, current)->landType = enemy2->beneathEnemyNode->landType;
                myMap->retrieveEdge(current, current)->weight = enemy2->beneathEnemyNode->weight;

                //change enemy to have beneathPlayerNode values
                enemy2->beneathEnemyNode->landType = player->beneathPlayerNode->landType;
                enemy2->beneathEnemyNode->weight = player->beneathPlayerNode->weight;
                enemy2->beneathEnemyNode->vertex = player->beneathPlayerNode->vertex;

                myMap->retrieveEdge(target, target)->landType = enemy2->enemyNode->landType;
                myMap->retrieveEdge(target, target)->weight = enemy2->enemyNode->weight;
                enemy2->enemyNode->vertex = target;

                updateEdges(current, target);

                //move player to random H
                movePlayerToHidden();
                return;
            }

            myMap->retrieveEdge(current, current)->landType = enemy2->beneathEnemyNode->landType;
            myMap->retrieveEdge(current, current)->weight = enemy2->beneathEnemyNode->weight;
            enemy2->beneathEnemyNode->landType = myMap->retrieveEdge(target, target)->landType;
            enemy2->beneathEnemyNode->weight = myMap->retrieveEdge(target, target)->weight;
            enemy2->beneathEnemyNode->vertex = myMap->retrieveEdge(target, target)->vertex;
            myMap->retrieveEdge(target, target)->landType = enemy2->enemyNode->landType;
            myMap->retrieveEdge(target, target)->weight = enemy2->enemyNode->weight;
            enemy2->enemyNode->vertex = target;

            updateEdges(current, target);
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

            //check if another enemy is already in the nextPosition 
            int potentialX = nextPosition / numCol;
            int potentialY = nextPosition % numCol;
            if (mapMatrix[potentialX][potentialY] == 'X') {
                return;
            }

            // Update matrix
            mapMatrix[enemy3->xPos][enemy3->yPos] = enemy3->beneathEnemyNode->landType;
            enemy3->xPos = nextPosition / numCol;
            enemy3->yPos = nextPosition % numCol;
            mapMatrix[enemy3->xPos][enemy3->yPos] = enemy3->enemyNode->landType;

            // Update graph
            int current = enemyNodeVertex;
            int target = nextPosition;

            //enemy has found the player
            if (myMap->retrieveEdge(target, target)->landType == 'O') {
                //update current position for when enemy moves
                myMap->retrieveEdge(current, current)->landType = enemy3->beneathEnemyNode->landType;
                myMap->retrieveEdge(current, current)->weight = enemy3->beneathEnemyNode->weight;

                //change enemy to have beneathPlayerNode values
                enemy3->beneathEnemyNode->landType = player->beneathPlayerNode->landType;
                enemy3->beneathEnemyNode->weight = player->beneathPlayerNode->weight;
                enemy3->beneathEnemyNode->vertex = player->beneathPlayerNode->vertex;

                myMap->retrieveEdge(target, target)->landType = enemy3->enemyNode->landType;
                myMap->retrieveEdge(target, target)->weight = enemy3->enemyNode->weight;
                enemy3->enemyNode->vertex = target;

                updateEdges(current, target);

                //move player to random H
                movePlayerToHidden();
                return;
            }

            myMap->retrieveEdge(current, current)->landType = enemy3->beneathEnemyNode->landType;
            myMap->retrieveEdge(current, current)->weight = enemy3->beneathEnemyNode->weight;
            enemy3->beneathEnemyNode->landType = myMap->retrieveEdge(target, target)->landType;
            enemy3->beneathEnemyNode->weight = myMap->retrieveEdge(target, target)->weight;
            enemy3->beneathEnemyNode->vertex = myMap->retrieveEdge(target, target)->vertex;
            myMap->retrieveEdge(target, target)->landType = enemy3->enemyNode->landType;
            myMap->retrieveEdge(target, target)->weight = enemy3->enemyNode->weight;
            enemy3->enemyNode->vertex = target;

            updateEdges(current, target);
        }
    }

    delete[] d;
    delete[] pi;
}
