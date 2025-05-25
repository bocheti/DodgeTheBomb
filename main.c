#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  // To capture keys without waiting for Enter (Windows only)

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

void startGame();
void showScores();
void exitGame();
void displayGrid(int size, Position player, Position bombs[], int bombCount);
void movePlayer(Position *player, char input, int size);
int checkBombCollision(Position player, Position bombs[], int bombCount);
void saveScore(const char *playerName, int score);
void loadTopScores();

int main() {
    int option;

    do {
        printf("\n===== MAIN MENU =====\n");
        printf("1. Play\n");
        printf("2. View Scores\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                startGame();
                break;
            case 2:
                showScores();
                break;
            case 3:
                exitGame();
                break;
            default:
                printf("Invalid option, please try again.\n");
        }
    } while (option != 3);

    return 0;
}

void startGame() {
    int size, difficulty, score = 0;
    Position player = {0, 0};
    Position bombs[MAX_BOMBS];
    char playerName[50];

    printf("\nEnter your name: ");
    scanf("%s", playerName);
    
    printf("\nEnter the size of the game field (e.g., 10): ");
    scanf("%d", &size);
    if (size > MAX_SIZE) size = MAX_SIZE;

    printf("Select difficulty:\n");
    printf("1. Beginner\n");
    printf("2. Intermediate\n");
    printf("3. Pro\n");
    printf("4. GamingLegend\n");
    printf("Choose an option: ");
    scanf("%d", &difficulty);

    if (difficulty < 1 || difficulty > 4) {
        printf("Invalid difficulty, defaulting to Beginner.\n");
        difficulty = 1;
    }

    srand(time(NULL));
    int round = 1;

    while (1) {
        int bombCount = (round * difficulty) + (round / 2);
        if (bombCount > MAX_BOMBS) bombCount = MAX_BOMBS;

        // Random bomb generation
        for (int i = 0; i < bombCount; i++) {
            bombs[i].x = rand() % size;
            bombs[i].y = rand() % size;
        }

        displayGrid(size, player, bombs, bombCount);
        printf("Use W/A/S/D to move: ");
        char input = _getch();  // Capture input without waiting for Enter

        movePlayer(&player, input, size);

        if (checkBombCollision(player, bombs, bombCount)) {
            printf("\nBOOM! You lost on round %d.\n", round);
            break;
        }

        score += round * difficulty * (20 / size);
        round++;
    }

    saveScore(playerName, score);
    printf("\nYour final score: %d\n", score);
}

void displayGrid(int size, Position player, Position bombs[], int bombCount) {
    printf("\n===== GAME FIELD =====\n");
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

    // Sort scores from highest to lowest
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

void showScores() {
    loadTopScores();
}

void loadTopScores() {
    FILE *file = fopen("scores.txt", "r");
    ScoreEntry scores[MAX_TOP_SCORES];
    int count = 0;

    if (file == NULL) {
        printf("No saved scores.\n");
        return;
    }

    printf("\n===== TOP 10 SCORES =====\n");
    while (count < MAX_TOP_SCORES && fscanf(file, "%s %d", scores[count].name, &scores[count].score) == 2) {
        printf("%d. %s - %d points\n", count + 1, scores[count].name, scores[count].score);
        count++;
    }
    fclose(file);
}

void exitGame() {
    printf("\nThanks for playing. See you next time!\n");
}
