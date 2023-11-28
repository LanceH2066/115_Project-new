#pragma once
using namespace std;
#include<iostream>
#include<vector>
#include<fstream>
#include "GraphAL.h"
#include "Player.h"

class Map
{
private:
	Player* player;
	int mapSize;
	GraphAL* myMap;
	Enemy* enemy1, *enemy2, *enemy3;
	
public:

	Map();
	//Map(int n);

	nodeAL* getNode(int vertex);
	void displayMap();

	//creating matrix
	int numRows;
	int numCol;
	char** mapMatrix;

	void printMatrix();

	//creating Graph
	//void updateGraph();
	
	//Player Movement
	void movePlayerUp();
	void movePlayerDown();
	void movePlayerLeft();
	void movePlayerRight();

	void resetPlayer();
	//Enemy Movement
	void moveEnemies();
	void moveEnemy1();
	void moveEnemy2();
	void moveEnemy3();

	//Rewriting Map Constructor
	void readMapFromFile(ifstream& file, vector<string>& lines, int& numRows, int& numCol);
	void createMapMatrix(const vector<string>& lines, int numRows, int numCol);
	void populateGraph();
	void addEdges(int currentNode, double currentWeight, int i, int j);
	void addDirectedEdge(int currentNode, double currentWeight, int i, int j);
	void initializeEntities(char currentChar, int i, int j, int currentNode, int& enemyCount);
	void createEnemy(int i, int j, int currentNode, int& enemyCount);
};

tuple<int, int> countRowsAndColumns(const string& filename);
double getSymbolWeight(char symbol);
