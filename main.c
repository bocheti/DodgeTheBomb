/*
 * 
 * HTWG SET2: Programming Mini-Project 
 * 
 * main.c
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

#include <stdio.h>
#include "game.h"

int main() {
    char inputBuffer[100];
    int option;
    hideCursor();

    while (1) {
        system("cls");
        gotoxy(0, 0);
        printf("===== MAIN MENU =====\n");
        printf("1. Play\n");
        printf("2. View Scores\n");
        printf("3. Exit\n");
        printf("Choose an option: ");

        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL &&
            sscanf(inputBuffer, "%d", &option) == 1) {

            switch (option) {
                case 1:
                    startGame();
                    break;
                case 2:
                    showScores();
                    printf("\nPress any key to return to menu...");
                    _getch();
                    break;
                case 3:
                    exitGame();
                    return 0;
                default:
                    printf("Invalid option. Please choose 1, 2, or 3.\n");
                    Sleep(1000);
                    break;
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            Sleep(1000);
        }
    }
    return 0;
}
