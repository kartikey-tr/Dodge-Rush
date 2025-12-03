#include <stdio.h>
#include <conio.h>  
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// --- HELPER: Move Cursor ---
void goToXY(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// --- HELPER: Hide Cursor ---
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// --- HELPER: File Handling ---
int getHighScore() {
    int hs = 0;
    FILE *f = fopen("highscore.txt", "r");
    if (f) { fscanf(f, "%d", &hs); fclose(f); }
    return hs;
}

void saveHighScore(int score) {
    FILE *f = fopen("highscore.txt", "w");
    if (f) { fprintf(f, "%d", score); fclose(f); }
}

int main() {
    SetConsoleOutputCP(65001); 
    srand(time(0));
    hideCursor();

    const char* PLAYER = "▲"; 
    const char* ENEMY  = "█";
    const char* WALL   = "║";
    const char* CRASH  = "X"; 

    bool playAgain = false;

    do {
        system("cls"); 

        // --- MENU ---
        int difficulty = 2;
        int highScore = getHighScore();
        
        printf("\n  ╔═════════════════════╗\n");
        printf("  ║    DODGE RUSH       ║\n");
        printf("  ╠═════════════════════╣\n");
        printf("  ║ High Score: %-6d  ║\n", highScore);
        printf("  ╚═════════════════════╝\n\n");
        printf("  Select Difficulty:\n\n");
        printf("  [1] Baby Mode   (Very Slow)\n");
        printf("  [2] Easy        (Slow)\n");
        printf("  [3] Normal      (Medium)\n");
        printf("  [4] Hard        (Fast)\n");
        printf("  [5] Nightmare   (Insane)\n\n");
        printf("  > Press number key to start...");
        
        char diffInput = _getch(); 
        if(diffInput >= '1' && diffInput <= '5') {
            difficulty = diffInput - '0';
        }

        // --- GAME VARIABLES ---
        int x = 1;              
        int lives = 3;          
        int score = 0;
        int enemyGrid[10]; 
        for(int i=0; i<10; i++) enemyGrid[i] = -1; 
        
        int frameCounter = 0;   
        int gameSpeed = 15 - (difficulty * 2); 
        if (gameSpeed < 1) gameSpeed = 1;

        bool running = true;
        system("cls"); 

        while (running) {
            
            goToXY(0, 0); 

            // --- INPUT ---
            if (_kbhit()) {
                char key = _getch();
                if (key == -32 || key == 224) { // Arrow keys
                    key = _getch(); 
                    if (key == 75 && x > 0) x--; 
                    if (key == 77 && x < 2) x++; 
                }
                else { // WASD
                    if ((key == 'a' || key == 'A') && x > 0) x--;
                    if ((key == 'd' || key == 'D') && x < 2) x++;
                }
            }

            // --- DRAWING ---
            // Fixed alignment: Inner width is strictly 13 spaces for ALL rows.
            printf("╔═════════════╗\n"); 

            for (int i = 0; i < 10; i++) {
                int enemyPos = enemyGrid[i];
                
                // Left:   1 space  + Enemy + 11 spaces = 13
                // Mid:    6 spaces + Enemy + 6 spaces  = 13
                // Right:  11 spaces+ Enemy + 1 space   = 13
                // Empty:  13 spaces                    = 13
                
                if (enemyPos == 0)      printf("%s %s           %s\n", WALL, ENEMY, WALL); 
                else if (enemyPos == 1) printf("%s      %s      %s\n", WALL, ENEMY, WALL);
                else if (enemyPos == 2) printf("%s           %s %s\n", WALL, ENEMY, WALL);
                else                    printf("%s             %s\n", WALL, WALL); 
            }
            
            // Player Draw
            const char* playerIcon = PLAYER;
            if (enemyGrid[9] == x) playerIcon = CRASH; 

            if (x == 0)      printf("%s %s           %s\n", WALL, playerIcon, WALL); 
            else if (x == 1) printf("%s      %s      %s\n", WALL, playerIcon, WALL);
            else if (x == 2) printf("%s           %s %s\n", WALL, playerIcon, WALL);
            
            printf("╚═════════════╝\n");
            
            // Stats Panel
            printf("Score:      %-6d\n", score); 
            printf("High Score: %-6d\n", highScore); 
            printf("Difficulty: %-6d\n", difficulty);
            printf("Lives:      %-6d\n", lives);

            // --- COLLISION ---
            if (enemyGrid[9] != -1 && enemyGrid[9] == x) {
                Beep(500, 100); 
                lives--;
                enemyGrid[9] = -1; 
                
                if (lives <= 0) {
                    Beep(200, 500);
                    printf("\n   GAME OVER!   \n");
                    if (score > highScore) {
                        saveHighScore(score);
                        printf(" NEW HIGH SCORE!\n");
                    }
                    running = false;
                }
            }

            // --- LOGIC ---
            Sleep(20); 
            frameCounter++;
            
            if (frameCounter >= gameSpeed) {
                // Shift enemies down
                for (int i = 9; i > 0; i--) {
                    enemyGrid[i] = enemyGrid[i-1];
                }

                if (enemyGrid[9] != -1 && enemyGrid[9] != x) score++;

                // --- FIXED SPAWN LOGIC ---
                int newPos = -1;
                
                // Rule 1: STRICTLY forbid spawning if the row below (grid[1]) has an enemy.
                // This prevents "long" vertical obstacles. 
                // We check grid[1] because grid[0] just became grid[1] in the loop above.\
                
                if (enemyGrid[1] == -1) {
                    
                    int spawnChance = 35 + (difficulty * 5); 
                    if (rand() % 100 < spawnChance) {
                        newPos = rand() % 3;
                    }
                }
                
                enemyGrid[0] = newPos;
                frameCounter = 0; 
            }
        }

        printf("\nPress ENTER to Restart, ESC to Quit...");
        while(1) {
            char c = _getch();
            if (c == 13) { playAgain = true; break; } 
            if (c == 27) { playAgain = false; break; } 
        }

    } while (playAgain);

    return 0;
}
