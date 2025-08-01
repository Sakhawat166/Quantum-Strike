
#include "iGraphics.h"
#include "iSound.h"

// ==================== Function Prototypes ====================
void LoadResources();
void loadExplosionFrames();
void initializeBossSystem();
void drawMainMenu();
void drawNameInputScreen();
void drawLevelSelectScreen();
void drawSpaceshipSelectScreen();
void checkBulletEnemyCollision();
void playerEnemyCollision();
void updateBullets();
void drawGameScreen();
void spawnPowerUps();
void initializePowerUps();
void checkBossActivation();
void spawnEnemy();
void updateEnemies();
void moveEnemies();
void handlePowerUps();
void handleMegaBomb();
void handleBossMechanics();
void drawGameOverScreen();
void loadScores();
void createScoreAnim(int x, int y, int value);
void drawScoreboardScreen();
void drawHelpPage();
void iDraw();
void iMouse(int button, int state, int mx, int my);
void iKeyboard(unsigned char, int);
void iSpecialKeyboard(unsigned char, int);
void iMouseDrag(int mx, int my);
void iMouseMove(int mx, int my);
void iMouseWheel(int button, int dir, int mx);
int main(int argc, char *argv[]);

// Template prototype for moveAndCheckPickup
template <typename Callback>
void moveAndCheckPickup(bool *active, int *x, int *y, int size, const char *label, Callback onPickup);

// ==================== Screen & Menu ====================
int screenWidth = 1500; // Default width, can be adjusted
int screenHeight = 750; // Default height, can be adjusted
int currentMenu = 0;    // 0 = Main Menu, 1 = Name Input

// ==================== Player Info ======================
char playerName[100] = "";
int nameCharIndex = 0;
int selectedLevel = 0;
int selectedShip = -1;
const int totalShips = 3;

// ==================== Player Ship ======================
int playerX = 400;
int playerY = 100;
int playerSpeed = 20;

// ==================== Bullets ==========================
const int maxBullets = 100;
int bulletX[maxBullets], bulletY[maxBullets];
bool bulletActive[maxBullets];
int bulletCount = 0;
int bulletSpeed = 30;

// ==================== Enemies ==========================
const int maxEnemies = 10;
int enemyX[maxEnemies];
int enemyY[maxEnemies];
bool enemyActive[maxEnemies];
int enemySpeed = 0.5;
int enemySize = 40;

const int maxShooterBullets = 20;
int shooterBulletX[maxShooterBullets];
int shooterBulletY[maxShooterBullets];
bool shooterBulletActive[maxShooterBullets];

int aiEnemyX = 0;
int aiEnemyY = screenHeight;
bool aiEnemyActive = false;
int aiEnemySpeed = 3;

enum EnemyType
{
    NORMAL,
    SHOOTER,
    FAST
};
EnemyType enemyType[maxEnemies];

// ==================== Explosion ========================
const int maxExplosions = 50;
Image explosionFrames[7]; // Load these once
int totalExplosionFrames = 7;

struct Explosion
{
    int x, y;
    int frameIndex;
    bool active;
};

Explosion explosions[maxExplosions];

// ==================== Player Stats =====================
int playerLife = selectedLevel;
const int maxLives = 5; 
bool isGameOver = false;
int playerHealth = 500; // 100% health
const int maxHealth = 500;

// ==================== UI Buttons =======================
int buttonWidth = 300;
int buttonHeight = 75;
int buttonX = screenWidth / 2 - buttonWidth / 2;
int buttonYStart = 483;
int buttonGap = 100;

// ==================== Score & Pause ====================
int score = 0;
bool isPaused = false;
const int maxScoreAnims = 20;

struct ScoreAnim
{
    int x, y;
    int value;
    int timer; // lifespan in frames
    bool active;
};

ScoreAnim scoreAnims[maxScoreAnims];

bool scoreMultiplierActive;
int scoreMultiplierValue = 1;
int multiplierTimer;
bool armorActive;
int armorTimer;
bool rapidFireActive;
int rapidFireTimer;
bool megaBombActive;


// ==================== Power-Ups (Improved) ========================

struct PowerUp {
    bool active;
    int x, y;
    int size;
    const char* label;
    void (*onPickup)();
};

const int totalPowerUps = 5;
PowerUp powerUps[totalPowerUps];


// ==================== Boss =============================
bool bossActive = false;

int bossX = 200;
int bossY = screenHeight - 150;
int bossWidth = 200;
int bossHeight = 200;
int bossHealth = 1000; // 1000 hits needed 
int bossBulletX[20];
int bossBulletY[20];
bool bossBulletActive[20];
int bossFireTimer = 0;
int bossstart=20*1000/500;
int bossSpeed = 3;
int bossDirection = 1; // 1 = right, -1 = left

// powerups countdown
int powerupCooldown = 0; // controls how often any power-up can appear
int healthCooldown = 0;
int multiplierCooldown = 0;
int armorCooldown = 10*1000/500;
int rapidCooldown = 0;
int bombCooldown = 0;

// ==================== Scoreboard =======================
struct ScoreEntry
{
    char name[100];
    int score;
};
ScoreEntry scores[100];
int scoreCount = 0;

const char *menuItems[] = {
    "New Game",
    "Scoreboard",
    "Help",
    "About",
    "Exit"};
const int menuCount = sizeof(menuItems) / sizeof(menuItems[0]);

// ==================== Image Declaration =========================

Image coverbase,translucent,translucent2,translucent3,gameover;
Image menubutton[5],levelbutton[5];
Image playerShip[3];
Image highlightPlayerShip[3];
Image enemy[4];
Image bomb;
Image armor;
Image HP;
Image rapid_fire;
Image bullet;
Image shooterbullet;
Image maingamebackground[5];
Image multiplier;
Image boss,aienemy;
Image helpPage,gameOverScreen,nameInputScreen;
Image aboutIcon;
Image about1, about2;
Image bossBullet;


int coverTimerID;
int musicon=1;
int channel1;

// ==================== Game Initialization ====================
void LoadResources()
{

    iLoadImage(&coverbase, "assets/coverbase.png");
    iResizeImage(&coverbase, screenWidth, screenHeight);
    iLoadImage(&gameover, "assets/gameover.png");
    iResizeImage(&gameover, screenWidth, screenHeight);

    iLoadImage(&bullet, "assets/bullet.png");
    iResizeImage(&bullet, 10, 20);

    iLoadImage(&bossBullet,"assets/shooterbullet.png");
    iResizeImage(&bossBullet, 20,20);

    iLoadImage(&about2, "assets/about2.jpg");
    iResizeImage(&about2, screenWidth, screenHeight);

    iLoadImage(&translucent, "assets/translucent.png");
    iResizeImage(&translucent,300, 400 );
    iLoadImage(&translucent2, "assets/translucent.png");
    iResizeImage(&translucent2,400, 500 );
    iLoadImage(&translucent, "assets/translucent.png");
    iResizeImage(&translucent,300, 400 );
    iLoadImage(&translucent3, "assets/translucent3.png");
    iResizeImage(&translucent3, 1200, 500 );

    iLoadImage(&menubutton[0], "assets/newgame.jpg");
    iResizeImage(&menubutton[0], buttonWidth, buttonHeight );
    iLoadImage(&menubutton[1], "assets/scoreboard.jpg");
    iResizeImage(&menubutton[1], buttonWidth, buttonHeight );
    iLoadImage(&menubutton[2], "assets/helpButton.jpg");
    iResizeImage(&menubutton[2], buttonWidth, buttonHeight );
    iLoadImage(&menubutton[3], "assets/aboutButton.jpg");
    iResizeImage(&menubutton[3], buttonWidth, buttonHeight );
    iLoadImage(&menubutton[4], "assets/exit.jpg");
    iResizeImage(&menubutton[4], buttonWidth, buttonHeight );
      for (int i = 0; i < 5; i++)
    {
        char path[100];
        sprintf(path, "assets/level%d.png", i + 1);
        iLoadImage(&levelbutton[i], path);
        iResizeImage(&levelbutton[i], buttonWidth, buttonHeight);
    }
    

    // iLoadImage(&shooterbullet, "assets/shooterbullet.png");

    // iLoadImage(&maingamebackground, "assets/backgrounds/background_01.jpg");
    // iResizeImage(&maingamebackground, screenWidth, screenHeight);

    for (int i = 0; i < 5; i++)
    {
        char path[100];
        sprintf(path, "assets/backgrounds/background_%02d.jpg", i + 1);
        iLoadImage(&maingamebackground[i], path);
        iResizeImage(&maingamebackground[i], screenWidth, screenHeight);
    }

    iLoadImage(&about1,"assets/about1.jpg");
    iResizeImage(&about1, screenWidth,screenHeight);

    iLoadImage(&enemy[0], "assets/enemy_01.png");
    iResizeImage(&enemy[0], enemySize, enemySize);

    iLoadImage(&enemy[1], "assets/enemy_02.png");
    iResizeImage(&enemy[1], enemySize, enemySize);

    iLoadImage(&enemy[2], "assets/enemy_03.png");
    iResizeImage(&enemy[2], enemySize, enemySize);

    iLoadImage(&enemy[3], "assets/enemy_04.png");
    iResizeImage(&enemy[3], enemySize, enemySize);


    iLoadImage(&aienemy, "assets/shooterspaceship.png");
    iResizeImage(&aienemy, 80, 80);

    iLoadImage(&boss, "assets/boss.png");
    iResizeImage(&boss, 300, 300);

    
        iLoadImage(&playerShip[0], "assets/spaceship_01.png");
    iResizeImage(&playerShip[0], 150, 150);

    

    
        iLoadImage(&playerShip[1], "assets/spaceship_02.png");
    iResizeImage(&playerShip[1], 150, 150);
    

    
        iLoadImage(&playerShip[2], "assets/spaceship_03.png");
    iResizeImage(&playerShip[2], 150, 150);



    iLoadImage(&highlightPlayerShip[0], "assets/spaceship_01.png");
    iResizeImage(&highlightPlayerShip[0], 300, 300);
    

    
        iLoadImage(&highlightPlayerShip[1], "assets/spaceship_02.png");
    iResizeImage(&highlightPlayerShip[1], 300, 300);
    

    
        iLoadImage(&highlightPlayerShip[2], "assets/spaceship_03.png");
    iResizeImage(&highlightPlayerShip[2], 300, 300);
    

    iLoadImage(&helpPage, "assets/help.jpg");
    iResizeImage(&helpPage,screenWidth,screenHeight);

    iLoadImage(&nameInputScreen, "assets/name_input.jpg");
    iResizeImage(&nameInputScreen, screenWidth, screenHeight);

    iLoadImage(&aboutIcon,"assets/about_icon.png");
    iResizeImage(&aboutIcon, 50,50);

    iLoadImage(&bomb, "assets/bomb.png");
    iResizeImage(&bomb, 150, 150);

    iLoadImage(&armor, "assets/armor.png");
    iResizeImage(&armor, 79, 87);

    iLoadImage(&HP, "assets/health_pack.png");
    iResizeImage(&HP, 75, 75);

    iLoadImage(&multiplier, "assets/3X.png");
    iResizeImage(&multiplier,75,75);


    iLoadImage(&rapid_fire, "assets/rapid_fire.png");
    iResizeImage(&rapid_fire,75,75);

}

void resetGameState()
{
    // Reset player
    playerX = 400;
    playerY = 100;
    playerHealth = maxHealth;
    playerLife = selectedLevel;
    isGameOver = false;

    // Reset enemies
    for (int i = 0; i < maxEnemies; i++)
        enemyActive[i] = false;

    // Reset bullets
    for (int i = 0; i < maxBullets; i++)
        bulletActive[i] = false;
    bulletCount = 0;

    // Reset power-ups
    for (int i = 0; i < totalPowerUps; i++)
        powerUps[i].active = false;

    // Reset AI enemy
    aiEnemyActive = false;

    // Reset boss
    bossActive = false;
    initializeBossSystem();

    // Reset score effects
    for (int i = 0; i < maxScoreAnims; i++)
        scoreAnims[i].active = false;

    // Reset explosion
    for (int i = 0; i < maxExplosions; i++)
        explosions[i].active = false;

    // Reset all power-up timers
    armorActive = false;
    rapidFireActive = false;
    scoreMultiplierActive = false;
    megaBombActive = false;
}


void goToMenu()
{
    currentMenu = 1;
    iPauseTimer(coverTimerID); 
}

void loadExplosionFrames()
{
    for (int i = 0; i < totalExplosionFrames; i++)
    {
        char path[100];
        sprintf(path, "assets/explosions/Explosion%d.png", i + 1);
        iLoadImage(&explosionFrames[i], path);
        iResizeImage(&explosionFrames[i], 60, 60);
    }
}



void initializeBossSystem()
{
    bossActive = false;
    bossX = 200;
    bossY = screenHeight - 200;
    bossWidth = 400;
    bossHeight = 100;
    bossHealth = 1000;
    bossFireTimer = 0;

    for (int i = 0; i < 10; i++)
    {
        bossBulletActive[i] = false;
    }
}

// Function to draw the main menu
void drawMainMenu()
{
    // iSetColor(255, 255, 255);
    // iText(screenWidth / 2 - 100, buttonYStart + menuCount * buttonGap, "SPACE SHOOTER GAME", GLUT_BITMAP_TIMES_ROMAN_24);

    iShowLoadedImage(0,0,&coverbase);

    for (int i = 0; i < menuCount; i++)
    {
        int y = buttonYStart - i * buttonGap;
        iShowLoadedImage(buttonX, y, &menubutton[i]);
        iShowLoadedImage(100, screenHeight-100, &aboutIcon);
        // iSetColor(50, 50, 255); // Button color
        // iRectangle(buttonX, y, buttonWidth, buttonHeight);

        // iSetColor(255, 255, 255); // Text color
        // iText(buttonX + 60, y + 18, menuItems[i], GLUT_BITMAP_HELVETICA_18);
        
    }
    
}

void drawNameInputScreen()
{
    // iShowLoadedImage(0, 0, &gameover);
    // iSetColor(255, 255, 255);
    // iText(screenWidth / 2 - 80, 400, "Enter Your Name:", GLUT_BITMAP_TIMES_ROMAN_24);

    // iSetColor(0, 0, 0);
    // iFilledRectangle(screenWidth / 2 - 150, 300, 300, 50); // Input Box

    
    iShowLoadedImage(0,0,&nameInputScreen);
    iSetColor(255, 255, 255);
    // iRectangle(screenWidth / 2 - 150, 300, 300, 50);
    iText(600, 500, playerName, GLUT_BITMAP_HELVETICA_18);
}

void drawLevelSelectScreen()
{
    iShowLoadedImage(0, 0, &coverbase);
    iSetColor(255, 255, 255);
    iText(screenWidth / 2-60, 550, "Select a Level", GLUT_BITMAP_HELVETICA_18);

    
       for (int i = 0; i < menuCount; i++)
    {
        int y = buttonYStart - i * buttonGap;
        iShowLoadedImage(buttonX, y, &levelbutton[i]);
    }
}

void drawSpaceshipSelectScreen()
{
    iShowLoadedImage(0, 0, &coverbase);
    iSetColor(255, 255, 255);
    iText(screenWidth / 2 - 100, 450, "Choose Your Spaceship", GLUT_BITMAP_HELVETICA_18);

    for (int i = 0; i < totalShips; i++)
    {
        int x = 200 + i * (screenWidth-400)/2;
        int y = 250;

        iShowLoadedImage(x,y,&playerShip[i]);

        iSetColor(255, 255, 255);
        char label[20];
        sprintf(label, "Ship %d", i + 1);
        iText(x + 25, y - 25, label, GLUT_BITMAP_HELVETICA_18);

        if (selectedShip == i)
        {
            // iSetColor(255, 255, 0);
            // iRectangle(x - 5, y - 5, 110, 110); // highlight selection
            // iShowLoadedImage(x,y,&highlightPlayerShip[i]);
        }
    }

    // "Start Game" button
    if (selectedShip != -1)
    {
        iSetColor(0, 200, 0);
        iFilledRectangle(screenWidth / 2 - 60, 100, 120, 40);
        iSetColor(255, 255, 255);
        iText(screenWidth / 2 - 40, 115, "Start Game", GLUT_BITMAP_HELVETICA_18);
    }
}

void checkBulletEnemyCollision()
{
    for (int i = 0; i < maxBullets; i++)
    {
        if (bulletActive[i])
        {
            for (int j = 0; j < maxEnemies; j++)
            {
                if (enemyActive[j])
                {
                    if (bulletX[i] >= enemyX[j] && bulletX[i] <= enemyX[j] + 40 &&
                        bulletY[i] >= enemyY[j] && bulletY[i] <= enemyY[j] + 40)
                    {
                        bulletActive[i] = false;
                        enemyActive[j] = false;
                        iPlaySound("music_explosion.wav",false,100);
                        score += (10 * selectedLevel) * scoreMultiplierValue;
                        createScoreAnim(enemyX[j] + 10, enemyY[j] + 20, 10); // place +10 above enemy

                        for (int k = 0; k < maxExplosions; k++)
                        {
                            if (!explosions[k].active)
                            {
                                explosions[k].x = enemyX[j];
                                explosions[k].y = enemyY[j];
                                explosions[k].frameIndex = 0;
                                explosions[k].active = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    if (bossActive)
    {
        for (int i = 0; i < maxBullets; i++)
        {
            if (bulletActive[i])
            {
                if (bulletX[i] >= bossX && bulletX[i] <= bossX + bossWidth &&
                    bulletY[i] >= bossY && bulletY[i] <= bossY + bossHeight)
                {

                    bulletActive[i] = false;
                    bossHealth--;
                    score+=5;

                    if (bossHealth <= 0)
                    {
                        bossActive = false;
                        initializeBossSystem();
                      
                        score += 1000; // Bonus score for killing boss
                        printf("BOSS DEFEATED!\n");
                    }
                }
            }
        }
    }
}

void updateBullets()
{
    if (isPaused)
        return;
    for (int i = 0; i < maxBullets; i++)
    {
        if (bulletActive[i])
        {
            
            bulletY[i] += bulletSpeed;
            if (bulletY[i] > screenHeight)
            {
                bulletActive[i] = false;
            }
        }
    }

    checkBulletEnemyCollision();
    playerEnemyCollision();
}

void gameLoop(void)
{
    if (currentMenu == 5 && !isPaused && !isGameOver)
    {

        updateBullets();
        updateEnemies();
       
        if (isKeyPressed(' '))
        {

            int bulletsPerShot = rapidFireActive ? 3 : 1;

            int bulletsFired = 0;
            for (int i = 0; i < maxBullets && bulletsFired < bulletsPerShot; i++)
            {
                if (!bulletActive[i])
                {
                    bulletX[i] = playerX+ 70 + (bulletsFired * 5);
                    // + 37 + (bulletsFired * 5);
                    bulletY[i] = playerY + 150;
                    bulletActive[i] = true;
                    bulletsFired++;
                }
            }
        }

        {
            if (isSpecialKeyPressed(GLUT_KEY_LEFT) && playerX > 0)
                playerX -= playerSpeed;
            if (isSpecialKeyPressed(GLUT_KEY_RIGHT) && playerX < screenWidth - 80)
                playerX += playerSpeed;
            if (isSpecialKeyPressed(GLUT_KEY_UP) && playerY < screenHeight - 100)
                playerY += playerSpeed;
            if (isSpecialKeyPressed(GLUT_KEY_DOWN) && playerY > 0)
                playerY -= playerSpeed;
        }
    }
}

void drawPowerUps(){
    // Draw all active power-ups
for (int i = 0; i < totalPowerUps; i++) {
    PowerUp& pu = powerUps[i];
    if (pu.active) {
        if (i == 0) {  // Health Pack
            // iSetColor(0, 255, 0);
            // iFilledRectangle(pu.x, pu.y, pu.size, pu.size);
            iShowLoadedImage(pu.x,pu.y,&HP);
        }
        else if (i == 1) {  // Multiplier
            // iSetColor(255, 215, 0);
            // iFilledEllipse(pu.x + pu.size/2, pu.y + pu.size/2, pu.size/2, pu.size/2);
            iShowLoadedImage(pu.x + pu.size/2,pu.y + pu.size/2,&multiplier);
        }
        else if (i == 2) {  // Armor
            // iSetColor(0, 200, 255);
            // iFilledRectangle(pu.x, pu.y, pu.size, pu.size);
            iShowLoadedImage(pu.x,pu.y,&armor);
        }
        else if (i == 3) {  // Rapid Fire
            // iSetColor(255, 50, 50);
            // iFilledEllipse(pu.x + pu.size/2, pu.y + pu.size/2, pu.size/2, pu.size/2);
            iShowLoadedImage(pu.x + pu.size/2, pu.y + pu.size/2,&rapid_fire);
        }
        else if (i == 4) {  // Mega Bomb
            // iSetColor(255, 0, 255);
            // iFilledRectangle(pu.x, pu.y, pu.size, pu.size);
            iShowLoadedImage(pu.x,pu.y,&bomb);
        }

        // Draw label on top (common for all)
        iSetColor(255, 255, 255);
        iText(pu.x + 5, pu.y + 10, pu.label, GLUT_BITMAP_HELVETICA_12);
    }
}

}

void drawHealthBar(){
        // Health Bar Background
    int healthBarWidth = 200;
    int healthBarHeight = 20;
    int healthX = screenWidth / 2 - healthBarWidth / 2;
    int healthY = 40;
    if(playerHealth<=0) playerHealth=0;

    // Draw background bar
    iSetColor(50, 50, 50); // Dark background
    iFilledRectangle(healthX, healthY, healthBarWidth, healthBarHeight);

    // Dynamic Color Based on Health %
    float healthPercent = (float)playerHealth / maxHealth;

    if (healthPercent > 0.6f)
        iSetColor(0, 255, 0);         // Green
    else if (healthPercent > 0.3f)
        iSetColor(255, 215, 0);       // Yellow
    else
        iSetColor(255, 50, 50);       // Red

    // Fill current health
    iFilledRectangle(healthX, healthY, healthBarWidth * healthPercent, healthBarHeight);

    // Outline
    iSetColor(255, 255, 255);
    iRectangle(healthX, healthY, healthBarWidth, healthBarHeight);

    // Label
    iSetColor(255, 255, 255);
    iText(healthX, healthY + healthBarHeight + 5, "Health", GLUT_BITMAP_HELVETICA_18);

}

void drawGameScreen()
{

    if (selectedLevel == 1)
    {
       
        iShowLoadedImage(0, 0, &maingamebackground[0]); // Background for Level 1
    }
    else if (selectedLevel == 2)
    {
       
        iShowLoadedImage(0, 0, &maingamebackground[1]); // Background for Level 2
    }
    else if (selectedLevel == 3)
    {
        
        iShowLoadedImage(0, 0, &maingamebackground[2]); // Background for Level 3
    }
    else if (selectedLevel == 4)
    {
        
        iShowLoadedImage(0, 0, &maingamebackground[3]); // Background for Level 4
    }
    else
    {
        
        iShowLoadedImage(0, 0, &maingamebackground[4]); // Background for Boss Level
        iSetColor(255, 0, 0);
        iText(screenWidth / 2 - 50, screenHeight / 2, "BOSS LEVEL", GLUT_BITMAP_HELVETICA_18);
    }

   
    iShowLoadedImage(playerX, playerY, &playerShip[selectedShip]);

    // Draw bullets
    iSetColor(255, 255, 0);
    for (int i = 0; i < maxBullets; i++)
    {
        if (bulletActive[i])
        {
            // iFilledRectangle(bulletX[i], bulletY[i], 5, 10);
            iShowLoadedImage(bulletX[i], bulletY[i], &bullet);
            // iPlaySound("bullet.wav,false,100");
        }
    }

    for (int i = 0; i < maxShooterBullets; i++)
    {
        if (shooterBulletActive[i])
        {
            iShowLoadedImage(shooterBulletX[i], shooterBulletY[i], &shooterbullet);
        }
    }

    // Player Info
    iSetColor(255, 255, 255);
    char info[100];
    sprintf(info, "Player: %s   Level: %d", playerName, selectedLevel);
    iText(20, screenHeight - 30, info, GLUT_BITMAP_HELVETICA_18);

    // Draw Enemies
    for (int i = 0; i < maxEnemies; i++)
    {
        if (enemyActive[i])
        {
            if (enemyType[i] == NORMAL)
                // iSetColor(200, 0, 0);
                iShowLoadedImage(enemyX[i], enemyY[i], &enemy[rand()%2]);
            else if (enemyType[i] == SHOOTER)
                iSetColor(0, 200, 0);
            else if (enemyType[i] == FAST)
                iShowLoadedImage(enemyX[i], enemyY[i], &enemy[rand()%2+2]);

            // iFilledRectangle(enemyX[i], enemyY[i], 40, 40);
            // iShowLoadedImage(enemyX[i], enemyY[i], &enemy);
        }
    }

    if (aiEnemyActive)
    {
        iShowLoadedImage(aiEnemyX, aiEnemyY, &aienemy);
        iText(aiEnemyX + 5, aiEnemyY + 15, "AI", GLUT_BITMAP_HELVETICA_12);
    }

    // Draw Explosions
    for (int i = 0; i < maxExplosions; i++)
    {
        if (explosions[i].active)
        {
            iShowLoadedImage(explosions[i].x - 10, explosions[i].y - 10, &explosionFrames[explosions[i].frameIndex]);
        }
    }


    drawPowerUps();
    if(armorActive) iText(screenWidth/2-50, 100, "ARMOR ACTIVE");
    
    

    char scoreStr[50];
    sprintf(scoreStr, "Score: %d", score);
    iText(screenWidth - 200, screenHeight - 30, scoreStr, GLUT_BITMAP_HELVETICA_18);

    // // Health Bar
    // iSetColor(255, 255, 255);
    // iText(screenWidth / 2 - 80, 40, "Health:", GLUT_BITMAP_HELVETICA_18);

    // iSetColor(0, 255, 0);
    // iFilledRectangle(screenWidth / 2, 40, playerHealth, 15);

    drawHealthBar();

    // Life Icons (already present)
    iText(20, 40, "Lives:", GLUT_BITMAP_HELVETICA_18);

    for (int i = 1; i <= selectedLevel; i++)
    {
        double heartX = 80 + i * 25;
        double heartY = 40;

        if (i <= playerLife)
        {
            // Filled Heart
            iSetColor(255, 0, 0);
            iFilledEllipse(heartX, heartY, 7, 10);
            iFilledEllipse(heartX + 10, heartY, 7, 10);
            double Livex[] = {heartX - 7, heartX + 17, heartX + 5};
            double Livey[] = {heartY, heartY, heartY - 15};
            iFilledPolygon(Livex, Livey, 3);
        }
        else
        {
            // Bordered/Outlined Heart
            iSetColor(255, 255, 255);
            iFilledEllipse(heartX, heartY, 7, 10);
            iFilledEllipse(heartX + 10, heartY, 7, 10);
            double Livex[] = {heartX - 7, heartX + 17, heartX + 5};
            double Livey[] = {heartY, heartY, heartY - 15};
            iFilledPolygon(Livex, Livey, 3);
        }
    }

   

    if (scoreMultiplierActive)
    {
        char multiText[50];
        sprintf(multiText, "Score x%d", scoreMultiplierValue);
        iText(screenWidth / 2 - 50, screenHeight - 30, multiText, GLUT_BITMAP_HELVETICA_18);
    }


    if (bossActive)
    {
        // Boss Body
        iShowLoadedImage(bossX, bossY, &boss);
        int barWidth = 300;
        int barHeight = 10;

        // Boss Health Bar
        iSetColor(255, 255, 255);
        iText(bossX + bossWidth / 2 - 30, bossY + bossHeight + 10, "BOSS", GLUT_BITMAP_HELVETICA_18);
        iSetColor(255, 0, 0); // Red background
        iFilledRectangle(bossX, bossY-20, barWidth, barHeight);

        iSetColor(0, 255, 0); // Green health
        int currentWidth = (bossHealth * barWidth) / 1000;
        iFilledRectangle(bossX, bossY-20, currentWidth, barHeight);


        // Boss Bullets
        for (int i = 0; i < 10; i++)
        {
            if (bossBulletActive[i])
            {
                // iSetColor(255, 100, 0);
                // iFilledRectangle(bossBulletX[i], bossBulletY[i], 5, 15);
                iShowLoadedImage(bossBulletX[i],bossBulletY[i],&bossBullet);
            }
        }
    }

    if (isPaused)
    {
        iSetColor(255, 255, 255);
        iText(screenWidth / 2 - 60, screenHeight / 2, "GAME PAUSED", GLUT_BITMAP_TIMES_ROMAN_24);
    }

    for (int i = 0; i < maxScoreAnims; i++)
    {
        if (scoreAnims[i].active)
        {
            char txt[10];
            sprintf(txt, "+%d", scoreAnims[i].value);
            iSetColor(255, 255, 0); // Yellow
            iText(scoreAnims[i].x, scoreAnims[i].y, txt, GLUT_BITMAP_HELVETICA_18);
        }
    }
}


void initializePowerUps() {
    // Health Pack
    powerUps[0] = {false, 0, 0, 30, "+HP", []() {
        playerHealth += 30;
        if (playerHealth > maxHealth)
            playerHealth = maxHealth;
    }};

    // Score Multiplier
    powerUps[1] = {false, 0, 0, 30, "x3", []() {
        scoreMultiplierActive = true;
        scoreMultiplierValue = (rand() % 2 == 0) ? 3 : 5;
        multiplierTimer = 300;
    }};

    // Armor
    powerUps[2] = {false, 0, 0, 30, "ARM", []() {
        armorActive = true;
        armorTimer = 300;
    }};

    // Rapid Fire
    powerUps[3] = {false, 0, 0, 30, "SPD", []() {
        rapidFireActive = true;
        rapidFireTimer = 200;
    }};

    // Mega Bomb
    powerUps[4] = {false, 0, 0, 30, "BOMB", []() {
        megaBombActive = true;
    }};
}





void spawnPowerUps() {
    for (int i = 0; i < totalPowerUps; i++) {
        PowerUp& pu = powerUps[i];
        if (!pu.active) {
            int spawnChance = (i == 4) ? 3 : 5; // Rarer for Mega Bomb (index 4)

            if (rand() % 100 < spawnChance) {
                pu.x = rand() % (screenWidth - pu.size);
                pu.y = screenHeight + rand() % 300;
                pu.active = true;
            }
        }
    }
}


void spawnEnemy()
{
    if (currentMenu == 5 && !isPaused && !isGameOver)
    {
    for (int i = 0; i < maxEnemies; i++)
    {
        if (!enemyActive[i])
        {
            enemyX[i] = rand() % (screenWidth - 40);
            enemyY[i] = screenHeight + rand() % 300;
            enemyActive[i] = true;

            int typeChance = rand() % 100;

            if (selectedLevel == 1)
            {
                enemyType[i] = NORMAL;
            }
            else if (selectedLevel == 2)
            {
                enemyType[i] = (typeChance < 60) ? NORMAL : FAST;
            }
            else if (selectedLevel == 3)
            {
                if (typeChance < 40)
                    enemyType[i] = NORMAL;
                else if (typeChance < 80)
                    enemyType[i] = FAST;
                else
                    enemyType[i] = SHOOTER;
            }
            else if (selectedLevel == 4 || selectedLevel == 5)
            {
                // Shooter enabled + all types
                if (typeChance < 30)
                    enemyType[i] = SHOOTER;
                else if (typeChance < 65)
                    enemyType[i] = FAST;
                else
                    enemyType[i] = NORMAL;
            }

            if ((selectedLevel == 4 || selectedLevel == 5) && !aiEnemyActive)
            {
                aiEnemyX = rand() % (screenWidth - 40);
                aiEnemyY = screenHeight + 100;
                aiEnemyActive = true;
            }

            break;
        }
    
    }
    

    spawnPowerUps();
    if (armorActive) {
        if (armorCooldown > 0) {
            armorCooldown--;
        } else {
            armorActive = false;
            armorCooldown = 10*1000/500;
        }
    }
    if(selectedLevel==5&&!bossActive){
        if(bossstart>0) bossstart--;
        else {
            bossActive=true;
            bossstart=15*1000/500;
    }}
}
  
}

void playerEnemyCollision(){
        for (int i=0; i<maxEnemies; i++){
            if (enemyX[i]+enemySize>= playerX && enemyX[i] <= playerX + 150 &&
                enemyY[i]+enemySize>= playerY && enemyY[i] <= playerY + 150){

                    if(!armorActive){
                        playerHealth--;
                        score -= 0.5*selectedLevel*scoreMultiplierValue;
                    }
                    enemyActive[i]=false;
                    //  for (int k = 0; k < maxExplosions; k++)
                    //     {
                    //         if (!explosions[k].active)
                    //         {
                    //             explosions[k].x = enemyX[i];
                    //             explosions[k].y = enemyY[i];
                    //             explosions[k].frameIndex = 0;
                    //             explosions[k].active = true;
                    //             break;
                    //         }
                    //     }
                }
        }
}

void moveEnemies()
{
    
    for (int i = 0; i < maxEnemies; i++)
    {
        if (enemyActive[i] && !isPaused)
        {
            int thisSpeed = (enemyType[i] == FAST) ? enemySpeed + 10 : enemySpeed;
            enemyY[i] -= thisSpeed;

            if (enemyY[i] < 0)
            {
                enemyActive[i] = false;
                playerHealth--;
                
                    

                if (playerHealth <= 0)
                {
                    playerLife--;
                    playerHealth = maxHealth;
                    if (playerLife <= 0)
                    {
                        isGameOver = true;
                        currentMenu = 6;
                    }
                }
            }
        }
    }

}


template <typename Callback>
void moveAndCheckPickup(bool *active, int *x, int *y, int size, const char *label, Callback onPickup)
{
    if (*active)
    {
        *y -= 5;
        if (*y < 0)
        {
            *active = false;
            return;
        }

        if (playerX < *x + size &&
            playerX + 80 > *x &&
            playerY < *y + size &&
            playerY + 60 > *y)
        {
            *active = false;
            onPickup();
        }
    }
}
void handleMegaBomb()
{
    if (megaBombActive)
    {
        for (int i = 0; i < maxEnemies; i++)
        {
            if (enemyActive[i])
            {
                enemyActive[i] = false;
                iPlaySound("explosion.wav",false,100);
                score += 10 * scoreMultiplierValue;
                createScoreAnim(enemyX[i] + 10, enemyY[i] + 20, 10); // place +10 above enemy

                for (int k = 0; k < maxExplosions; k++)
                {
                    if (!explosions[k].active)
                    {
                        explosions[k].x = enemyX[i];
                        explosions[k].y = enemyY[i];
                        explosions[k].frameIndex = 0;
                        explosions[k].active = true;
                        break;
                    }
                }
            }
        }
        megaBombActive = false;
    }
}

void handleBossMechanics()
{
   if (!bossActive){return;}

    // Move boss horizontally
    bossX += bossSpeed * bossDirection;
    if (bossX <= 0 || bossX + bossWidth >= screenWidth)
        bossDirection *= -1;



    

    // Move boss bullets
    for (int i = 0; i < 20; i++)
    {
        if (bossBulletActive[i])
        {
            bossBulletY[i] -= 5;
            if (bossBulletY[i] < 0)
                bossBulletActive[i] = false;
        }
    }

    // Boss fires bullet every 60 frames (~2s)
    bossFireTimer++;
    if (bossFireTimer >= 30)
    {
        bossFireTimer = 0;
        for (int i = 0; i < 20; i++)
        {
            if (!bossBulletActive[i])
            {
                bossBulletX[i] = bossX + rand() % bossWidth;
                bossBulletY[i] = bossY;
                bossBulletActive[i] = true;
                break;
            }
        }
    }

    // Collision with player
    for (int i = 0; i < 10; i++)
    {
        if (bossBulletActive[i] &&
            playerX < bossBulletX[i] + 5 &&
            playerX + 80 > bossBulletX[i] &&
            playerY < bossBulletY[i] + 15 &&
            playerY + 60 > bossBulletY[i])
        {

            if (!armorActive)
                playerHealth -= 20;

            bossBulletActive[i] = false;

            if (playerHealth <= 0)
            {
                playerLife--;
                playerHealth = maxHealth;
                if (playerLife <= 0)
                {
                    isGameOver = true;
                    currentMenu = 6;
                }
            }
        }
    }
}

void updateShooterBullets()
{
    for (int i = 0; i < maxShooterBullets; i++)
    {
        if (shooterBulletActive[i])
        {
            shooterBulletY[i] -= 6;

            // Collision with player
            if (shooterBulletX[i] >= playerX && shooterBulletX[i] <= playerX + 80 &&
                shooterBulletY[i] >= playerY && shooterBulletY[i] <= playerY + 60)
            {
                if (!armorActive)
                    playerHealth -= 10;

                if (playerHealth <= 0)
                {
                    playerLife--;
                    playerHealth = maxHealth;
                    if (playerLife <= 0)
                    {
                        isGameOver = true;
                        currentMenu = 6;
                    }
                }

                shooterBulletActive[i] = false;
            }

            if (shooterBulletY[i] < 0)
                shooterBulletActive[i] = false;
        }
    }
}

void updateAIEnemy()
{
    if (!aiEnemyActive || isPaused)
        return;

    // Move towards player
    if (aiEnemyX < playerX)
        aiEnemyX += aiEnemySpeed;
    else if (aiEnemyX > playerX)
        aiEnemyX -= aiEnemySpeed;

    if (aiEnemyY > playerY)
        aiEnemyY -= aiEnemySpeed;
    else if (aiEnemyY < playerY)
        aiEnemyY += aiEnemySpeed;

    // Collision with player
    if (playerX < aiEnemyX + 40 && playerX + 80 > aiEnemyX &&
        playerY < aiEnemyY + 40 && playerY + 60 > aiEnemyY)
    {

        aiEnemyActive = false; // destroy after hit

        if (!armorActive)
        {
            playerLife--;
            
                    if (playerLife <= 0)
                    {
                        isGameOver = true;
                        currentMenu = 6;
                    }
                
        }
        else score+=100;

    }

    // If off-screen
    if (aiEnemyY < 0)
    {
        aiEnemyActive = false;
    }
}


void updatePowerUps() {
    for (int i = 0; i < totalPowerUps; i++) {
        PowerUp& pu = powerUps[i];
        if (pu.active) {
            pu.y -= 3;  // Falling speed

            // Off-screen? Deactivate
            if (pu.y < 0) {
                pu.active = false;
            }
            // Collision with player
            else if (playerX < pu.x + pu.size &&
                     playerX + 120 > pu.x &&
                     playerY < pu.y + pu.size &&
                     playerY + 120 > pu.y) {

                pu.active = false;    // Hide power-up
                pu.onPickup();        // Trigger effect
            }
        }
    }
}


void updateEnemies()
{
    if (isPaused)
        return;
    moveEnemies();

    for (int i = 0; i < maxEnemies; i++)
    {
        if (enemyActive[i] && enemyType[i] == SHOOTER)
        {
            if (rand() % 100 < 3 && selectedLevel >= 4)
            {
                for (int j = 0; j < maxShooterBullets; j++)
                {
                    if (!shooterBulletActive[j])
                    {
                        shooterBulletX[j] = enemyX[i] + 20;
                        shooterBulletY[j] = enemyY[i];
                        shooterBulletActive[j] = true;
                        break;
                    }
                }
            }
        }
    }

    // handlePowerUps();
    handleMegaBomb();
    handleBossMechanics();
    updateShooterBullets();
    updateAIEnemy();
    updatePowerUps();

    // score animations
    for (int i = 0; i < maxScoreAnims; i++)
    {
        if (scoreAnims[i].active)
        {
            scoreAnims[i].y += 1; // move up
            scoreAnims[i].timer -= 1;
            if (scoreAnims[i].timer <= 0)
                scoreAnims[i].active = false;
        }
    }

    // explosions
    for (int i = 0; i < maxExplosions; i++)
    {
        if (explosions[i].active)
        {
            explosions[i].frameIndex++;
            if (explosions[i].frameIndex >= totalExplosionFrames)
            {
                explosions[i].active = false;
            }
        }
    }

    // cooldown for power-ups
    if (healthCooldown > 0)
        healthCooldown--;
    if (multiplierCooldown > 0)
        multiplierCooldown--;
    // if (armorCooldown > 0)
    //     armorCooldown--;
    if (rapidCooldown > 0)
        rapidCooldown--;
    if (bombCooldown > 0)
        bombCooldown--;
}

void drawGameOverScreen()
{
    iShowLoadedImage(0, 0, &gameover);
    

    // iText(screenWidth / 2 - 130, screenHeight / 2 - 50, "Press 'R' to Restart or 'Q' to Quit", GLUT_BITMAP_HELVETICA_18);
}

void createScoreAnim(int x, int y, int value)
{
    for (int i = 0; i < maxScoreAnims; i++)
    {
        if (!scoreAnims[i].active)
        {
            scoreAnims[i].x = x;
            scoreAnims[i].y = y;
            scoreAnims[i].value = (10*selectedLevel)*scoreMultiplierValue;
            scoreAnims[i].timer = 30; // ~50 frames = ~0.8 seconds
            scoreAnims[i].active = true;
            break;
        }
    }
}

void loadScores()
{
    FILE *fp = fopen("scoreboard.txt", "r");
    scoreCount = 0;
    if (fp != NULL)
    {
        while (fscanf(fp, "%s %d", scores[scoreCount].name, &scores[scoreCount].score) == 2)
        {
            scoreCount++;
        }
        fclose(fp);
    }

    // Sort scores descending
    for (int i = 0; i < scoreCount - 1; i++)
    {
        for (int j = i + 1; j < scoreCount; j++)
        {
            if (scores[i].score < scores[j].score)
            {
                ScoreEntry temp = scores[i];
                scores[i] = scores[j];
                scores[j] = temp;
            }
        }
    }
}

void drawScoreboardScreen()
{
    iShowLoadedImage(0, 0, &coverbase);
    iShowLoadedImage(450, 150, &translucent);
    iShowLoadedImage(800, 150, &translucent);
    iSetColor(255, 255, 255);
    iText(screenWidth / 2 - 60, 600, "TOP SCORES", GLUT_BITMAP_TIMES_ROMAN_24);

    for (int i = 0; i < scoreCount && i < 5; i++)
    {
        char name[100];
        char score[100];
        sprintf(name, "%d. %-30s", i + 1, scores[i].name);
        sprintf(score, "%-30d", scores[i].score);
        
        iText(480, 500 - i * 40, name, GLUT_BITMAP_HELVETICA_18);
        iText(830, 500 - i * 40, score, GLUT_BITMAP_HELVETICA_18);
    }

    iText(screenWidth / 2 - 100, 100, "Press 'B' to go back", GLUT_BITMAP_HELVETICA_18);
}

void drawHelpPage()
{
    iShowLoadedImage(0,0,&helpPage);
}

void iDraw()
{

    iClear();

    if(currentMenu == 0){
        iShowLoadedImage(0,0,&coverbase);
        iText(screenWidth/2-50, 100 ,"Loading...",GLUT_BITMAP_HELVETICA_18);
    }

   else if (currentMenu == 1)
    {
        drawMainMenu();
    }

    else if (currentMenu == 2)
    {
        drawNameInputScreen();
    }

    else if (currentMenu == 3)
    {
        drawLevelSelectScreen();
    }
    else if (currentMenu == 4)
    {
        drawSpaceshipSelectScreen();
    }

    else if (currentMenu == 5)
    {
        drawGameScreen();
    }

    else if (currentMenu == 6)
    {
        drawGameOverScreen();
    }

    else if (currentMenu == 7)
    {
        drawScoreboardScreen();
    }

    else if (currentMenu == 8)
    {
        drawHelpPage();
    }
    else if (currentMenu == 9){
        // iShowLoadedImage(0, 0, &coverbase);
        // iShowLoadedImage(150, 100, &translucent3);
        iShowLoadedImage(0,0, &about2);
    }

    else if (currentMenu == 10){
        iShowLoadedImage(0,0,&about1);
    }

    // future: else if (currentMenu == 2) --> Game Page etc.
}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        if(currentMenu == 0){
            if (mx >= 0 && mx <= screenWidth &&
                    my >= 0 && my <= screenHeight){
                        currentMenu = 1;
                    }
        }

        // Main Menu
        if (currentMenu == 1)
        {
            for (int i = 0; i < menuCount; i++)
            {
                int y = buttonYStart - i * buttonGap;
                if (mx >= buttonX && mx <= buttonX + buttonWidth &&
                    my >= y && my <= y + buttonHeight)
                {

                    printf("Clicked on: %s\n", menuItems[i]);

                    if (i == 0)
                    {
                        // New Game
                        currentMenu = 2;
                        nameCharIndex = 0;
                        playerName[0] = '\0';
                    }

                    else if (i == 1)
                    {
                        // Scoreboard
                        loadScores();
                        currentMenu = 7;
                    }

                    else if (i == 2)
                    { // Help
                        currentMenu = 8;
                    }
                    else if(i == 3){
                        //about
                        currentMenu = 9;
                    }

                    else if (i == 4)
                    {
                        // Exit
                        exit(0);
                    }
                }
            }

            if (mx>100 && mx<150 && my>screenHeight-100 && my<screenHeight-50){
                currentMenu = 10;
                printf("clicked");
            }
        }

        // Level Select
        else if (currentMenu == 3)
        {
            for (int i = 0; i < menuCount; i++)
            {
                int y = buttonYStart - i * buttonGap;
                if (mx >= buttonX && mx <= buttonX + buttonWidth &&
                    my >= y && my <= y + buttonHeight)
                {
               
                    selectedLevel = i+1;
                    playerLife = selectedLevel;
                    resetGameState();
                    currentMenu = 4;
                    printf("Level %d selected\n", selectedLevel);
                }
            }
        }

 // Ship Select
        else if (currentMenu == 4)
        {
            for (int i = 0; i < totalShips; i++)
            {
                int x = 200 + i * (screenWidth - 400) / 2;
                int y = 250;
                iSetColor(255,255,255);
                iRectangle(x,y,150,150);
                if (mx >= x && mx <= x + 150 && my >= y && my <= y + 150)
                {
                    selectedShip = i;
                    printf("Selected Ship: %d\n", selectedShip + 1);
                }
            }


            // enemy & bullet reset
            for (int j = 0; j < maxEnemies; j++)
                enemyActive[j] = false;
            for (int j = 0; j < maxBullets; j++)
                bulletActive[j] = false;
            bulletCount = 0;

            // Start Game
            if (selectedShip != -1)
            {
                int x = screenWidth / 2 - 60;
                int y = 100;
                if (mx >= x && mx <= x + 120 && my >= y && my <= y + 40)
                {
                    currentMenu = 5;
                    enemySpeed = 2 + selectedLevel;
                    initializeBossSystem();
                    initializePowerUps();
                    printf("Game starting...\n");
                }
            }
        }
    }
}

void iKeyboard(unsigned char key)
{
    // Name Input Screen
    if (currentMenu == 2)
    {
        if (key == '\r' || key == 13 || key == '\n' || key == 10)
        {
            currentMenu = 3;
            printf("Player Name: %s\n", playerName);
        }
        else if (key == '\b')
        {
            if (nameCharIndex > 0)
            {
                nameCharIndex--;
                playerName[nameCharIndex] = '\0';
            }
        }
        else
        {
            if (nameCharIndex < 99)
            {
                playerName[nameCharIndex] = key;
                nameCharIndex++;
                playerName[nameCharIndex] = '\0';
            }
        }
    }

    // Game screen (shoot)
    else if (currentMenu == 5)
    {

        if (key == 'p')
        {
            isPaused = !isPaused; // Toggle Pause
        }

        if (key == 'b'){
            currentMenu = 1;
        }
    }

    // Game Over screen
    else if (currentMenu == 6)
    {
        
        // Save score to file
        FILE *fp = fopen("scoreboard.txt", "a");
        if (fp != NULL)
        {
            fprintf(fp, "%s %d\n", playerName, score);
            fclose(fp);
        }

        if (key == 'b')
        {
            currentMenu = 1;
            playerLife = 3;
            isGameOver = false;
            bulletCount = 0;
            selectedLevel = 0;
            selectedShip = -1;
            score = 0;
            playerHealth=maxHealth;

            for (int i = 0; i < maxEnemies; i++)
                enemyActive[i] = false;
            for (int i = 0; i < maxBullets; i++)
                bulletActive[i] = false;
        }

        else if (key == 'q')
        {
            exit(0);
        }
    }

    // Scoreboard back
    else if (currentMenu == 7)
    {
        if (key == 'b')
        {
            currentMenu = 1;
        }
    }

    else if (currentMenu == 8)
    {
        if (key == 'b' || key == 'B')
        {
            currentMenu = 1;
        }
    }
      else if (currentMenu == 9)
    {
        if (key == 'b')
        {
            currentMenu = 1;
        }
    }

    else if(currentMenu == 10){
        if(key == 'b') currentMenu = 1;
    }

    // Global Quit
    if (key == 'q')
    {
        exit(0);
    }
    if(key == 'm') {musicon=musicon*(-1);
        if(musicon==1) iResumeSound(channel1);
    else iPauseSound(channel1);}
}

void iSpecialKeyboard(unsigned char key) {}


void iMouseDrag(int mx, int my) {}
void iMouseMove(int mx, int my) {}
void iMouseWheel(int button, int dir, int mx) {}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    LoadResources();
    loadExplosionFrames();
    coverTimerID = iSetTimer(5000, goToMenu);
    iSetTimer(16, gameLoop);
    iSetTimer(500, spawnEnemy);
    iInitializeSound();
    channel1 = iPlaySound("war.wav",true,50);
    iInitialize(screenWidth, screenHeight, "Space Shooter - Main Menu");
    return 0;
}
