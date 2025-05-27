/*
 * 
 * HTWG SET2: Programming Mini-Project 
 * 
 * game.h
 * 
 * Dodge The Bomb
 * 
 * By Salvador Espinosa (inm. Nummer 312905) and Francisco Villodres (inm. Nummer 312923)
 * 
 * This project is open-source. Any user is allowed to freely distribute it
 * and/or make use of it in any derived works or other projects whatsoever.
 * 
 * Enjoy!
 * 
 */

#ifndef GAME_H
#define GAME_H

#define MAX_SIZE 20
#define MAX_BOMBS 300
#define MAX_TOP_SCORES 10

typedef struct {
    int x, y;
} Position;

typedef struct {
    char name[50];
    int score;
} ScoreEntry;

void gotoxy(int x, int y);
void hideCursor();
void setColor(int color);
void startGame();
void showScores();
void exitGame();
void displayGrid(int size, Position player, Position bombs[], int bombCount, float timeLeft);
int movePlayer(Position *player, char input, int size);
int checkBombCollision(Position player, Position bombs[], int bombCount);
void saveScore(const char *playerName, int score);
void loadTopScores();

#endif
