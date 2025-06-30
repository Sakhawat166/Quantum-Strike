#include "iGraphics.h"

/*
function iDraw() is called again and again by the system.
*/

#define MAX_ENEMIES 100
Image spaceship, background , bullet,enemy,blust,scoreimage,menubg,start,levels,about,startbold,aboutbold,levelsbold,level1,level2,level3,level1bold,level2bold,level3bold,back,title;
int shipX = 300, shipY = 5, spaceship_width = 130 , spaceship_height = 130;
int speed = 20;
int bullet_active = 0, fire=0, bulletX = shipX+spaceship_width/2-10, bulletY = shipY + spaceship_height;
int screen_width = 800, screen_height= 600;
int enemy_active = 0;
int score = 0;
int enemy_ship[4];
int gamestate=0,levelstate=1,mousestate=0;
int imgcount=60, incr=10, imgpos[100], bgY=0;
Image bgImages[60];


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
void bg_render();
void move_bg();
void initialize_imgpos();



void score_count() {
    char score_text[20];
    sprintf(score_text, "Score: %d/%d", score, MAX_ENEMIES);

    int boxX = 10;
    int boxY = screen_height - 50;
    int boxWidth = 160;
    int boxHeight = 30;


//    iShowLoadedImage(boxX,boxY,&scoreimage);

    
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
    if(gamestate==1){
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
            bulletX = shipX+spaceship_width/2-11;
            bulletY = shipY + spaceship_height/2+40;
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

void initialize_imgpos(){
    int i,j=0;
    for(i=0;i<imgcount;i++){
     
        imgpos[i]=j;
        j=j+incr;
    }
}
void move_bg(){
    for(int i=0;i<imgcount;i++){
        imgpos[i]-=incr;
    }
     for(int i=0;i<imgcount;i++){
        if(imgpos[i]<0) 
        imgpos[i]=screen_height-incr;
    }
}
void bg_render(){
    for(int i=0;i<imgcount;i++){
        
        iShowLoadedImage(0,imgpos[i],&bgImages[i]);
      
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

    iLoadImage(&menubg, "assets/menubg.jpg");
    iResizeImage(&menubg, 800, 600);

    iLoadImage(&start, "assets/start.png");
    iResizeImage(&start, 170, 40);
    iLoadImage(&levels, "assets/levels.png");
    iResizeImage(&levels, 170, 40);
    iLoadImage(&about, "assets/about.png");
    iResizeImage(&about, 170, 40);
    iLoadImage(&startbold, "assets/startbold.png");
    iResizeImage(&startbold, 170, 40);
    iLoadImage(&levelsbold, "assets/levelsbold.png");
    iResizeImage(&levelsbold, 170, 40);
    iLoadImage(&aboutbold, "assets/aboutbold.png");
    iResizeImage(&aboutbold, 170, 40);

    iLoadImage(&level1, "assets/level1.png");
    iResizeImage(&level1, 170, 40);
    iLoadImage(&level2, "assets/level2.png");
    iResizeImage(&level2, 170, 40);
    iLoadImage(&level3, "assets/level3.png");
    iResizeImage(&level3, 170, 40);
    iLoadImage(&level1bold, "assets/level1bold.png");
    iResizeImage(&level1bold, 170, 40);
    iLoadImage(&level2bold, "assets/level2bold.png");
    iResizeImage(&level2bold, 170, 40);
    iLoadImage(&level3bold, "assets/level3bold.png");
    iResizeImage(&level3bold, 170, 40);

    iLoadImage(&back, "assets/back.png");
    iResizeImage(&back, 50, 50);

    
    iLoadImage(&title, "assets/title.png");
    iResizeImage(&title, 550, 120);

    int j=0;
    char bgimagepath[100];
    for (int i = imgcount-1; i>=0; i--) {
        sprintf(bgimagepath, "assets/images/row-%d-column-1.png", i+1);
            
        iLoadImage(&bgImages[j], bgimagepath);
        iResizeImage(&bgImages[j],screen_width ,incr);
        j++;
      
    }




}

void iDraw() {
    iClear();
    if(gamestate==0){
    //menu

        iShowLoadedImage(0, 0, &menubg);
        iShowLoadedImage(120, 450, &title);
        if(mousestate==1) iShowLoadedImage(300, 350, &startbold);
        else iShowLoadedImage(300, 350, &start);
        if(mousestate==2) iShowLoadedImage(300, 300, &levelsbold);
        else iShowLoadedImage(300, 300, &levels);
        if(mousestate==3) iShowLoadedImage(300, 250, &aboutbold);
        else iShowLoadedImage(300, 250, &about);
 

  
    }
    else if(gamestate==2){
    //levels
    
        iShowLoadedImage(0, 0, &menubg);
        if(mousestate==1) iShowLoadedImage(300, 350, &level1bold);
        else iShowLoadedImage(300, 350, &level1);
        if(mousestate==2) iShowLoadedImage(300, 300, &level2bold);
        else iShowLoadedImage(300, 300, &level2);
        if(mousestate==3) iShowLoadedImage(300, 250, &level3bold);
        else iShowLoadedImage(300, 250, &level3);
        iShowLoadedImage(50, 550, &back);
        
    }

    else if(gamestate==3){
    //about
        iShowLoadedImage(50, 550, &back);
        iText(200,200," this is about the about of whatabout of the about of this game");

    }

    else if(gamestate==1){
    // Draw background first
    // iShowLoadedImage(0, 0, &background);
     bg_render();


    // Draw spaceship
    iShowLoadedImage(shipX, shipY, &spaceship);
    

    if (bullet_active){
        iSetColor(255,255,255);
        iShowLoadedImage(bulletX, bulletY, &bullet);
       
    }
    enemy_make();
    move_enemy();

    blust_make();

    score_count();
    }

    
    
}





/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
            if (mx >= 300 && mx <= 470 && my >= 350 && my <= 390)
            mousestate = 1;
        else if (mx >= 300 && mx <= 470 && my >= 300 && my <= 340)
            mousestate = 2;
        else if (mx >= 300 && mx <= 470 && my >= 250 && my <= 290)
            mousestate = 3;
            else mousestate = 0;
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
    // Main menu buttons
    if (gamestate == 0) {
        if (mx >= 300 && mx <= 470 && my >= 350 && my <= 390)
            gamestate = 1; // Start Game
        else if (mx >= 300 && mx <= 470 && my >= 300 && my <= 340)
            gamestate = 2; // Level Select
        else if (mx >= 300 && mx <= 470 && my >= 250 && my <= 290)
            gamestate = 3; // About
    }

    // Level select menu
    else if (gamestate == 2) {
        if (mx >= 300 && mx <= 470 && my >= 350 && my <= 390)
            levelstate = 1;
        else if (mx >= 300 && mx <= 470 && my >= 300 && my <= 340)
            levelstate = 2;
        else if (mx >= 300 && mx <= 470 && my >= 250 && my <= 290)
            levelstate = 3;

        // Back button
        if (mx >= 50 && mx <= 100 && my >= 550 && my <= 600)
            gamestate = 0;
    }
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

    if(key == 'q') exit(0);



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
    initialize_imgpos();
    iSetTimer(20,move_bullet);
    iSetTimer(30,auto_fire);
    iSetTimer(100,move_enemy);
    iSetTimer(20,blust_update);
    iSetTimer(1000,enemy_add_row);
    iSetTimer(70 ,move_bg);
    iInitialize(800, 600, "Space Shooter");
    return 0;
}
