#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <math.h>

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

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void startGame();
void showScores();
void exitGame();
void displayGrid(int size, Position player, Position bombs[], int bombCount, float timeLeft);
void movePlayer(Position *player, char input, int size);
int checkBombCollision(Position player, Position bombs[], int bombCount);
void saveScore(const char *playerName, int score);
void loadTopScores();

int main() {
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
        scanf("%d", &option);
        getchar();
        switch (option) {
            case 1: startGame(); break;
            case 2: showScores(); printf("\nPress any key to return to menu..."); _getch(); break;
            case 3: exitGame(); return 0;
            default: printf("Invalid option, please try again.\n"); Sleep(1000); break;
        }
    }
    return 0;
}

void startGame() {
    int size, difficulty, score = 0, totalBombs = 0;
    Position player = {0, 0};
    Position bombs[MAX_BOMBS];
    char playerName[50];

    system("cls");
    gotoxy(0, 0);
    printf("Enter your name: ");
    scanf("%s", playerName);
    printf("Enter the size of the game field (max %d): ", MAX_SIZE);
    scanf("%d", &size);
    if (size > MAX_SIZE) size = MAX_SIZE;

    printf("Select difficulty:\n");
    printf("1. Beginner\n");
    printf("2. Intermediate\n");
    printf("3. Pro\n");
    printf("Choose an option: ");
    scanf("%d", &difficulty);
    if (difficulty < 1 || difficulty > 3) {
        printf("Invalid difficulty, defaulting to Beginner.\n");
        difficulty = 1;
        Sleep(1000);
    }

    srand(time(NULL));
    int round = 1;
    int baseBombs = difficulty;
    float reactionTime;

    switch (difficulty) {
        case 1: reactionTime = 5.0; break;
        case 2: reactionTime = 3.5; break;
        case 3: reactionTime = 2.0; break;
    }

    for (int i = 0; i < baseBombs; i++) {
        bombs[totalBombs].x = rand() % size;
        bombs[totalBombs].y = rand() % size;
        totalBombs++;
    }

    while (1) {
        if (round % 5 == 0) reactionTime = fmax(1.0, reactionTime - 0.2);

        for (int i = 0; i < baseBombs; i++) {
            if (totalBombs < MAX_BOMBS) {
                bombs[totalBombs].x = rand() % size;
                bombs[totalBombs].y = rand() % size;
                totalBombs++;
            }
        }

        float timeLeft = reactionTime;
        clock_t startTime = clock();
        char input = 0;

        while (((clock() - startTime) / (float)CLOCKS_PER_SEC) < reactionTime) {
            timeLeft = reactionTime - ((clock() - startTime) / (float)CLOCKS_PER_SEC);
            displayGrid(size, player, bombs, totalBombs, timeLeft);
            Sleep(100);
            if (_kbhit()) {
                input = _getch();
                break;
            }
        }

        if (!input) {
            gotoxy(0, size + 4);
            printf("\nTime's up! You didn't move in time.\n");
            break;
        }

        movePlayer(&player, input, size);

        if (checkBombCollision(player, bombs, totalBombs)) {
            gotoxy(0, size + 5);
            printf("BOOM! You lost on round %d.\n", round);
            break;
        }

        score += round * difficulty * (20 / size);
        round++;
    }

    saveScore(playerName, score);
    gotoxy(0, size + 7);
    system("cls");
    printf("Your final score: %d\n", score);
    printf("Press any key to return to the main menu...");
    _getch();
}

void displayGrid(int size, Position player, Position bombs[], int bombCount, float timeLeft) {
    gotoxy(0, 0);
    system("cls");
    printf("===== GAME FIELD =====\n");
    
    printf("   ");
    for (int x = 0; x < size; x++) {
        printf("%2d ", x);
    }
    printf("\n");

    for (int y = 0; y < size; y++) {
        printf("%2d ", y);
        for (int x = 0; x < size; x++) {
            int isBomb = 0, isPlayer = (x == player.x && y == player.y);
            for (int i = 0; i < bombCount; i++)
                if (bombs[i].x == x && bombs[i].y == y) isBomb = 1;
            if (isPlayer) {
                setColor(10); printf(" P "); setColor(7);
            } else if (isBomb) {
                setColor(12); printf(" * "); setColor(7);
            } else {
                setColor(8); printf(" . "); setColor(7);
            }
        }
        printf("\n");
    }
    setColor(11);
    printf("Time left: %.1f seconds\n", timeLeft);
    setColor(7);
}

void movePlayer(Position *player, char input, int size) {
    Position newPos = *player;
    if (input == 'w') newPos.y--;
    else if (input == 's') newPos.y++;
    else if (input == 'a') newPos.x--;
    else if (input == 'd') newPos.x++;

    if (newPos.x < 0 || newPos.x >= size || newPos.y < 0 || newPos.y >= size) {
        gotoxy(0, size + 4);
        printf("You moved outside the grid. You lose!\n");
        exit(0);
    } else {
        *player = newPos;
    }
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
    system("cls");
    loadTopScores();
}

void loadTopScores() {
    FILE *file = fopen("scores.txt", "r");
    ScoreEntry scores[MAX_TOP_SCORES];
    int count = 0;
    gotoxy(0, 0);
    system("cls");
    if (file == NULL) {
        printf("No saved scores.\n");
        return;
    }
    printf("===== TOP 10 SCORES =====\n");
    while (count < MAX_TOP_SCORES && fscanf(file, "%s %d", scores[count].name, &scores[count].score) == 2) {
        printf("%d. %s - %d\n", count + 1, scores[count].name, scores[count].score);
        count++;
    }
    fclose(file);
}

void exitGame() {
    system("cls");
    printf("Thank you for playing!\n");
    Sleep(1000);
}
