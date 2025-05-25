#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>   // For _getch()
#include <windows.h> // For gotoxy() and cursor hiding

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

// Cursor movement
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Hide the blinking cursor
void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

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
        getchar();  // Clear newline left by scanf

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
                printf("Invalid option, please try again.\n");
                Sleep(1000); // Wait for 1 second
                break;
        }

        system("cls"); // Clear screen between menu actions
    }

    return 0;
}

void startGame() {
    int size, difficulty, score = 0;
    Position player = {0, 0};
    Position bombs[MAX_BOMBS];
    char playerName[50];

    system("cls");
    gotoxy(0, 0);
    printf("Enter your name: ");
    scanf("%s", playerName);

    printf("Enter the size of the game field (e.g., 10): ");
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
        Sleep(1000);
    }

    srand(time(NULL));
    int round = 1;

    while (1) {
        int bombCount = (round * difficulty) + (round / 2);
        if (bombCount > MAX_BOMBS) bombCount = MAX_BOMBS;

        for (int i = 0; i < bombCount; i++) {
            bombs[i].x = rand() % size;
            bombs[i].y = rand() % size;
        }

        displayGrid(size, player, bombs, bombCount);
        gotoxy(0, size + 2);
        printf("Use W/A/S/D to move (Round %d | Score: %d): ", round, score);
        char input = _getch();

        movePlayer(&player, input, size);

        if (checkBombCollision(player, bombs, bombCount)) {
            gotoxy(0, size + 4);
            printf("BOOM! You lost on round %d.\n", round);
            break;
        }

        score += round * difficulty * (20 / size);
        round++;
    }

    saveScore(playerName, score);
    gotoxy(0, size + 6);
    system("cls");
    printf("Your final score: %d\n", score);
    printf("Press any key to return to the main menu...");
    _getch();
}

void displayGrid(int size, Position player, Position bombs[], int bombCount) {
    gotoxy(0, 0);
    system("cls");
    printf("===== GAME FIELD =====\n");
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
        printf("%d. %s - %d points\n", count + 1, scores[count].name, scores[count].score);
        count++;
    }
    fclose(file);
}

void exitGame() {
    gotoxy(0, 0);
    printf("\nThanks for playing. See you next time!\n");
}
