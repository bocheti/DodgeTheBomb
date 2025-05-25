#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  // Para capturar teclas sin esperar Enter (solo en Windows)

#define MAX_SIZE 20
#define MAX_BOMBS 30
#define MAX_TOP_SCORES 10

typedef struct {
    int x, y;
} Position;

typedef struct {
    char name[50];
    int score;
} ScoreEntry;

void iniciarJuego();
void mostrarPuntuaciones();
void salir();
void displayGrid(int size, Position player, Position bombs[], int bombCount);
void movePlayer(Position *player, char input, int size);
int checkBombCollision(Position player, Position bombs[], int bombCount);
void saveScore(const char *playerName, int score);
void loadTopScores();

int main() {
    int opcion;

    do {
        printf("\n===== MENÚ PRINCIPAL =====\n");
        printf("1. Jugar\n");
        printf("2. Ver puntuaciones\n");
        printf("3. Salir\n");
        printf("Elige una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                iniciarJuego();
                break;
            case 2:
                mostrarPuntuaciones();
                break;
            case 3:
                salir();
                break;
            default:
                printf("Opción inválida, intenta de nuevo.\n");
        }
    } while (opcion != 3);

    return 0;
}

void iniciarJuego() {
    int size, difficulty, score = 0;
    Position player = {0, 0};
    Position bombs[MAX_BOMBS];
    char playerName[50];

    printf("\nIngrese su nombre: ");
    scanf("%s", playerName);
    
    printf("\nIngrese el tamaño del campo de juego (ej. 10): ");
    scanf("%d", &size);
    if (size > MAX_SIZE) size = MAX_SIZE;

    printf("Seleccione dificultad:\n");
    printf("1. Principiante\n");
    printf("2. Medio\n");
    printf("3. Pro\n");
    printf("4. LeyendaDelGaming\n");
    printf("Elige una opción: ");
    scanf("%d", &difficulty);

    if (difficulty < 1 || difficulty > 4) {
        printf("Dificultad inválida, eligiendo Principiante por defecto.\n");
        difficulty = 1;
    }

    srand(time(NULL));
    int round = 1;

    while (1) {
        int bombCount = (round * difficulty) + (round / 2);
        if (bombCount > MAX_BOMBS) bombCount = MAX_BOMBS;

        // Generación aleatoria de bombas
        for (int i = 0; i < bombCount; i++) {
            bombs[i].x = rand() % size;
            bombs[i].y = rand() % size;
        }

        displayGrid(size, player, bombs, bombCount);
        printf("Usa W/A/S/D para moverte: ");
        char input = _getch();  // Captura la entrada sin esperar "Enter"

        movePlayer(&player, input, size);

        if (checkBombCollision(player, bombs, bombCount)) {
            printf("\n¡BOOM! Has perdido en la ronda %d.\n", round);
            break;
        }

        score += round * difficulty * (20 / size);
        round++;
    }

    saveScore(playerName, score);
    printf("\nTu puntuación final: %d\n", score);
}

void displayGrid(int size, Position player, Position bombs[], int bombCount) {
    printf("\n===== CAMPO DE JUEGO =====\n");
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

void saveScore(const char *playerName, int score) {
    ScoreEntry scores[MAX_TOP_SCORES + 1];
    int count = 0;

    FILE *file = fopen("scores.txt", "r");
    if (file != NULL) {
        while (count < MAX_TOP_SCORES && fscanf(file, "%s %d", scores[count].name, &scores[count].score) == 2) {
            count++;
        }
        fclose(file);
    }

    strcpy(scores[count].name, playerName);
    scores[count].score = score;
    count++;

    // Ordenar las puntuaciones de mayor a menor
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[j].score > scores[i].score) {
                ScoreEntry temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }

    file = fopen("scores.txt", "w");
    for (int i = 0; i < (count > MAX_TOP_SCORES ? MAX_TOP_SCORES : count); i++) {
        fprintf(file, "%s %d\n", scores[i].name, scores[i].score);
    }
    fclose(file);
}

void mostrarPuntuaciones() {
    loadTopScores();
}

void loadTopScores() {
    FILE *file = fopen("scores.txt", "r");
    ScoreEntry scores[MAX_TOP_SCORES];
    int count = 0;

    if (file == NULL) {
        printf("No hay puntuaciones guardadas.\n");
        return;
    }

    printf("\n===== TOP 10 PUNTUACIONES =====\n");
    while (count < MAX_TOP_SCORES && fscanf(file, "%s %d", scores[count].name, &scores[count].score) == 2) {
        printf("%d. %s - %d puntos\n", count + 1, scores[count].name, scores[count].score);
        count++;
    }
    fclose(file);
}

void salir() {
    printf("\nGracias por jugar. ¡Hasta la próxima!\n");
}
