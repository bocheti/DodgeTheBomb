#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h> // Para capturar teclas sin presionar Enter (solo en Windows)

#define MAX_SIZE 20 // Tamaño máximo del campo
#define MAX_BOMBS 10 // Máximo de bombas por ronda

typedef struct {
    int x, y;
} Position;

void initializeGame(int size);
void displayGrid(int size, Position player, Position bombs[], int bombCount);
void movePlayer(Position *player, char input, int size);
int checkBombCollision(Position player, Position bombs[], int bombCount);

int main() {
    int size;
    Position player = {0, 0};
    Position bombs[MAX_BOMBS];
    
    srand(time(NULL));

    printf("Ingrese el tamaño del campo de juego (Ej: 10): ");
    scanf("%d", &size);

    if (size > MAX_SIZE) size = MAX_SIZE;

    initializeGame(size);

    char input;
    int round = 1;

    while (1) {
        int bombCount = round + 2 > MAX_BOMBS ? MAX_BOMBS : round + 2;

        // Generar posiciones aleatorias para bombas
        for (int i = 0; i < bombCount; i++) {
            bombs[i].x = rand() % size;
            bombs[i].y = rand() % size;
        }

        displayGrid(size, player, bombs, bombCount);

        printf("Usa W/A/S/D para moverte: ");
        input = _getch(); // Capturar entrada sin esperar "Enter"

        movePlayer(&player, input, size);

        if (checkBombCollision(player, bombs, bombCount)) {
            printf("\n¡BOOM! Has perdido en la ronda %d.\n", round);
            break;
        }

        round++;
    }

    return 0;
}

void initializeGame(int size) {
    printf("\nIniciando juego con campo de %dx%d...\n", size, size);
}

void displayGrid(int size, Position player, Position bombs[], int bombCount) {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int isBomb = 0, isPlayer = (x == player.x && y == player.y);

            for (int i = 0; i < bombCount; i++)
                if (bombs[i].x == x && bombs[i].y == y) isBomb = 1;

            if (isPlayer) printf(" P ");
            else if (isBomb) printf(" * ");
            else printf(" . ");
        }
        printf("\n");
    }
}

void movePlayer(Position *player, char input, int size) {
    if (input == 'w' && player->y > 0) player->y--;
    else if (input == 's' && player->y < size - 1) player->y++;
    else if (input == 'a' && player->x > 0) player->x--;
    else if (input == 'd' && player->x < size - 1) player->x++;
}

int checkBombCollision(Position player, Position bombs[], int bombCount) {
    for (int i = 0; i < bombCount; i++)
        if (player.x == bombs[i].x && player.y == bombs[i].y)
            return 1;
    return 0;
}
