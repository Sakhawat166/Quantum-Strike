#include "iGraphics.h"

/*
function iDraw() is called again and again by the system.
*/

#define MAX_ENEMIES 100
Image spaceship, background , bullet,enemy,blust,scoreimage;
int shipX = 300, shipY = 5, spaceship_width = 130 , spaceship_height = 130;
int speed = 20;
int bullet_active = 0, fire=0, bulletX = shipX+spaceship_width/2-10, bulletY = shipY + spaceship_height;
int screen_width = 800, screen_height= 600;
int enemy_active = 0;
int score = 0;
int enemy_ship[4];

//>>>>>>>>>>>>>>> function prototypes <<<<<<<<<<<<<<//
 
void enemy_set(int index, int col, int row);
void move_enemy();
void enemy_make();
void enemy_add_row();
void auto_fire();
void move_bullet();
void bullet_hit_enemy();
void blust_make();
void blust_set(int x, int y);
void blust_update();
void score_count();



void score_count() {
    char score_text[20];
    sprintf(score_text, "Score: %d/%d", score, MAX_ENEMIES);

    int boxX = 10;
    int boxY = screen_height - 50;
    int boxWidth = 160;
    int boxHeight = 30;


   iShowLoadedImage(boxX,boxY,&scoreimage);

    
    iSetColor(255, 255, 255); 
    iText(boxX + 10, boxY + 8, score_text, GLUT_BITMAP_HELVETICA_18);
}


//>>>>>>>>>>>>>>>>>>>>>>>>>> Blust setup <<<<<<<<<<<<<<<<<<<<<<<//

struct blast{
    int x,y,timer,active;
};

struct blast blasts[MAX_ENEMIES];

void blust_set (int x, int y){
    for (int i= 0; i<MAX_ENEMIES; i++){
        if (!blasts[i].active){
            blasts[i].x = x;
            blasts[i].y = y;
            blasts[i].timer = 5;
            blasts[i].active = 1;
            break;
        }
    }
}


void blust_make(){
    for (int i=0; i<MAX_ENEMIES; i++){
        if (blasts[i].active){
            iShowLoadedImage(blasts[i].x,blasts[i].y,&blust);
        }
    }
}


void blust_update(){
    for (int i=0; i<MAX_ENEMIES; i++){
        if (blasts[i].active){
            blasts[i].timer--;

            if (blasts[i].timer<=0){
                blasts[i].active = 0;
            }
        }
    }
}

//>>>>>>>>>>>>>>>>>>>>>>>>>> Enemy setup <<<<<<<<<<<<<<<<<<<<<<<// 
int row,col;
struct enemy{
  int x,y,speed,alive;
};

struct enemy enemies[MAX_ENEMIES];


void enemy_set(int index, int col, int row){


  enemies[index].x = col;
  enemies[index].y=row;
  enemies[index].speed = 1;
  enemies[index].alive = 1;
}


void move_enemy(){
  for (int i=0; i< MAX_ENEMIES; i++){

     if (enemies[i].alive && enemy_active){
      enemies[i].y -= enemies[i].speed;
     }
    
     if ( enemies[i].y<0 ){
      enemies[i].alive = 0;
            }
        
    }

    bullet_hit_enemy();
}


void enemy_make(){
        for (int i=0; i<MAX_ENEMIES; i++){
            if (enemies[i].alive){
                iShowLoadedImage(enemies[i].x,enemies[i].y, &enemy);
            }
        }
}

int current_row = 0, total_rows=50;
void enemy_add_row() {
    if (current_row >= total_rows) return;

    int enemies_in_row =  2 * current_row + 1;
    int start_x = (screen_width - enemies_in_row * 60) / 2;

    for (int col = 0; col < enemies_in_row; col++) {
        int index = current_row * 11 + col;
        if (index < MAX_ENEMIES) {
            enemy_set(index, start_x + col * 60, screen_height);
        }
    }

    current_row++;
}



//>>>>>>>>>>>>>>>>>>>>>>>> Bullet <<<<<<<<<<<<<<<<<<<<//

void auto_fire(){
    if (fire) {
        if (bullet_active == 0){ 
            bullet_active= 1;
            bulletX = shipX+spaceship_width/2;
            bulletY = shipY + spaceship_height/2;
    }
}
}


void move_bullet(){
    if (bullet_active){
        bulletY +=100;
        if (bulletY>screen_height) bullet_active = 0;
    }
}

//>>>>>>>>>>>>>>> Bullet + enemy <<<<<<<<<<<<<<<<<<//

void bullet_hit_enemy(){
    if (!bullet_active) return;

    for (int i = 0; i<MAX_ENEMIES; i++){
        if (enemies[i].alive &&
    bulletX + 50 >= enemies[i].x && bulletX <= enemies[i].x + 50 &&
    bulletY + 50 >= enemies[i].y && bulletY <= enemies[i].y + 50)
{
    enemies[i].alive = 0;
    bullet_active = 0;
    blust_set(enemies[i].x,enemies[i].y);
    score ++;
    break;
}

    }
}


//>>>>>>>>>>>>>>>>> image load <<<<<<<<<<<<<<<<//
void Loadassets() {
    iLoadImage(&spaceship, "spaceship new.PNG");
    iResizeImage(&spaceship, spaceship_width, spaceship_height); 

    iLoadImage(&background, "background new.jpg");
    iResizeImage(&background, 800, 600);

    iLoadImage(&bullet,"bullet.PNG");
    iResizeImage(&bullet, 30, 60);

    iLoadImage(&enemy, "enemy.PNG");
    iResizeImage(&enemy,40,40);

    iLoadImage(&blust, "blust.PNG");
    iResizeImage(&blust,50,50);

    iLoadImage(&scoreimage,"score.PNG");
}

void iDraw() {
    iClear();
    
    // Draw background first
    iShowLoadedImage(0, 0, &background);

    // Draw spaceship
    iShowLoadedImage(shipX, shipY, &spaceship);
    

    if (bullet_active){
        iSetColor(255,255,255);
        iShowLoadedImage(bulletX,bulletY, &bullet);
       
    }

    enemy_make();
    move_enemy();

    blust_make();

    score_count();
    
}





/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
}

/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    // place your codes here
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }
}

/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if (key == ' '){
    fire = !fire;
    enemy_active = !enemy_active;
        
}


}

/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key)
{
    if (key == GLUT_KEY_LEFT){
        if (shipX+spaceship_width/2>0) shipX -= speed; 
        //if (!enemy_active) enemy_active = !enemy_active;
    }

    if (key == GLUT_KEY_RIGHT){ 
        if (shipX + spaceship_width < screen_width)
    shipX += speed;

    //if (!enemy_active) enemy_active = !enemy_active;
    
    }
}



int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    iSetTransparency(1);
    Loadassets();
    iSetTimer(20,move_bullet);
    iSetTimer(30,auto_fire);
    iSetTimer(100,move_enemy);
    iSetTimer(20,blust_update);
    iSetTimer(1000,enemy_add_row);
    iInitialize(800, 600, "Space Shooter");
    return 0;
}
