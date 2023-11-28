#pragma once
#include<iostream>
#include "GraphAL.h"
using namespace std;

class Player
{
public:
	Player();
	Player(int x, int y, nodeAL* pnode );

	void printPos();

	int xPos;
	int yPos;
	nodeAL* playerNode;
	nodeAL* beneathPlayerNode;

};

class Enemy {
public:
	int xPos;
	int yPos;
	nodeAL* enemyNode;
	nodeAL* beneathEnemyNode;

	Enemy();
	Enemy(int x, int y, nodeAL* eNode);

	void printPos();
};