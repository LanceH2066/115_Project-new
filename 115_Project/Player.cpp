#include "Player.h"

Player::Player()
{
	xPos = 0;
	yPos = 0; 
	playerNode = new nodeAL();
	beneathPlayerNode = new nodeAL();

}

Player::Player(int x, int y, nodeAL *pnode)
{
	xPos = x;
	yPos = y;
	playerNode = new nodeAL(pnode->vertex, pnode->weight, pnode->landType);
	beneathPlayerNode = new nodeAL(playerNode->vertex,10, ' ');
}

void Player::printPos()
{
	cout << xPos<< "," << yPos<< endl;
	cout << "Player Node: " << playerNode->landType << " " << playerNode->vertex << " " << playerNode->weight << endl;
	cout << "Beneath Player Node: " << beneathPlayerNode->landType << " " << beneathPlayerNode->vertex << " " << beneathPlayerNode->weight << endl;
}

Enemy::Enemy() {
	xPos = 0;
	yPos = 0;
	enemyNode = new nodeAL();
	beneathEnemyNode = new nodeAL();
}

Enemy::Enemy(int x, int y, nodeAL* eNode) {
	xPos = x;
	yPos = y;
	enemyNode = new nodeAL(eNode->vertex, eNode->weight, eNode->landType);
	beneathEnemyNode = new nodeAL(enemyNode->vertex, 100, ' ');
}

void Enemy::printPos() {
	cout << xPos << "," << yPos << endl;
	cout << "Enemy Node: " << enemyNode->landType << " " << enemyNode->vertex << " " << enemyNode->weight << endl;
	cout << "Beneath Enemy Node: " << beneathEnemyNode->landType << " " << beneathEnemyNode->vertex << " " << beneathEnemyNode->weight << endl;
}