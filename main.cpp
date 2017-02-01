/** 
Name: Kevin Du
Eid: kd9357
*/

#include <ncurses.h>    // the ncurses library
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <unistd.h> // sleep

#include <list>
#include "timehelper.h"

#include "Ship.h"
#include "Projectile.h"
#include "Obstacle.h"
using std::list;

const int NUM_ROWS = 20;
const int NUM_COLS = 35;
const int RIGHT_BORDER_BOUNDARY = NUM_COLS + 2;
const int BOTTOM_BORDER_BOUNDARY = NUM_ROWS + 2;
const int Y_START = 2;
const int X_START = 1;
const int NUM_ENEMY_ROWS = 2;
const int ENEMY_START = X_START + 12;
const int PLAYER_START = NUM_COLS / 2;
const int MAX_PLAYER_PROJECTILES = 5;
const int ENEMY_MOVEMENT_RATE = 20;
const int ENEMY_PROJ_MOV_RATE = 2;

const int MAX_WAVES = 3;
//for every wave, a new enemy row will appear, and they will move faster

void printBorders() {
    //print border, i = x variable, j = y variable
    //top
    for(int i = X_START; i < RIGHT_BORDER_BOUNDARY; i++)
        mvaddch(1, i, '_');
    //left
    for(int j = Y_START; j < BOTTOM_BORDER_BOUNDARY; j++)
        mvaddch(j, 0, '|');
    //right
    for(int j = Y_START; j < BOTTOM_BORDER_BOUNDARY; j++)
        mvaddch(j, RIGHT_BORDER_BOUNDARY, '|');
    //bottom
    for(int i = X_START; i < RIGHT_BORDER_BOUNDARY; i++)
        mvaddch(BOTTOM_BORDER_BOUNDARY, i, '_');
}

void printLists(list<GameObject*> obj) {
    for(list<GameObject*>::iterator iter = obj.begin(); iter != obj.end(); iter++) {
        GameObject * temp = *iter;
	    mvaddch(temp->getY(), temp->getX(), temp->getShape());
    }
}

bool startup() {
    mvprintw(0, 0, "Welcome to Space Raiders.");
    mvprintw(1, 0, "Enemy ships have appeared on our radar. It's your job to take them out!");
    mvprintw(2, 0, "Use the left and right direction keys to move your ship.");
    mvprintw(3, 0, "Use the space key to fire your laser. You can only fire 5 shots at a time.");
    mvprintw(4, 0, "Every enemy you defeat will add 1 point to your score.");
    mvprintw(5, 0, "You will lose if your ship is destroyed, or a single enemy slips past you.");
    mvprintw(6, 0, "We've set up some barriers (~) to help you out. They'll last for 5 hits before breaking.");
    mvprintw(7, 0, "Destory all enemies to achieve victory!");
    mvprintw(8, 0, "Press the space key to begin, or press 'q' to exit:");
    int ch = 0;
    while(true) {
        ch = getch();
        if(ch == 'q') {
            return true;
        }
        else if(ch == ' ') {
            return false;
        }
    }
}

list<GameObject *> createHorde(int phaseNum) {
    bool alternateShips = true;
    list<GameObject *> enemies;
    for(int j = NUM_ENEMY_ROWS + Y_START + phaseNum - 1; j >= Y_START; j--) {
       for(int i = ENEMY_START; i < RIGHT_BORDER_BOUNDARY; i+= 2) {
            if(alternateShips) {
                GameObject * eShipW = new EnemyW(j, i);
                //use maps instead of lists? to keep track of position easier
                //need to delete Enemy when destroyed
                enemies.push_back(eShipW);
                alternateShips = false; 
            }
            else {
               GameObject* eShipu = new Enemyu(j, i);
               enemies.push_back(eShipu);
               alternateShips = true;
            }
        }
        //flip for variety's sake
        if(alternateShips)
            alternateShips = false;
        else
            alternateShips = true;
    } 
    return enemies;
}

//hardcoded unfortunately, won't change with different board sizes
list<GameObject *> createObstacles() {
    list<GameObject *> barriers;
    for(int i = 7; i <= 9; i++) {
      GameObject * bar = new Obstacle(NUM_ROWS - 4, i);
      barriers.push_back(bar);
    }
    for(int i = 17; i <= 19; i++) {
	   GameObject * bar = new Obstacle(NUM_ROWS - 5, i);
	   barriers.push_back(bar);
    }
    for(int i = 27; i <= 29; i++) {
      GameObject * bar = new Obstacle(NUM_ROWS - 4, i);
      barriers.push_back(bar);
    }
  return barriers;
}

void freeMemory(list<GameObject *> obj) {
    GameObject * temp;
    list<GameObject * >::iterator iter = obj.begin();
    while(!obj.empty()) {
        temp = *iter;
        iter = obj.erase(iter);
        delete temp;
    }
}

void play(int phaseNum, int points, list<GameObject *> barrier) {
    //initially 50
    const int delay = 50; /// milliseconds

    timeout(delay); /// set how long between keypresses (more or less)

    //create the game space and ships(hardcoded as of right now)
    list<GameObject *> enemies;
    GameObject * player;
    //create the ships
    enemies = createHorde(phaseNum);
    player = new PlayerShip(BOTTOM_BORDER_BOUNDARY - 1, PLAYER_START);
    list<GameObject *> enemyProjectiles;
    list<GameObject *> playerProjectiles; 
    bool enemiesMoveLeft = true;
    bool reachedBoundary = true;
    int ch = 0;
    bool quit = false;
    int elapsedTime = getElapsedTime();
    bool win = false;
    bool exited = true;
    int counter = -1;
    //play the game
    while (!quit && phaseNum < MAX_WAVES){
        exited = false;
        counter++;
        ch = getch();
        erase(); /// erase the screen (after getch())
        if ( ch != ERR) { /// user has a keypress
            /// this is to delay until the next tick
            elapsedTime = getElapsedTime();
            if (elapsedTime < delay){
                nsleep(delay-elapsedTime);
            }
        }
        mvprintw(0, 0, "Points: %i.   'q' to quit.\n", points);
        /// limit to one move per tick?
        switch(ch){
            case KEY_RIGHT:
                if(player->getX() < RIGHT_BORDER_BOUNDARY - 1) 
                    player->movXBy(1);
                break;
            case KEY_LEFT: 
                if(player->getX() > X_START)
                    player->movXBy(-1); 
                break;
            case ' ':
            //create playerprojectile
            //push onto list, limit size to 5
                if(playerProjectiles.size() < 5) {
                    GameObject * projP = new PProjectile(player->getY(), player->getX());
                    playerProjectiles.push_back(projP);
                }
                break;
            case 'q': 
                quit = true;
                exited = true;
                break;
        }

        //move the enemies
        //need to check for time and pos
        //enemies move once every 20 ticks, or once a second
        if(!(counter % (ENEMY_MOVEMENT_RATE - phaseNum))) {
            for(list<GameObject*>::iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
                GameObject * sEnemy = *iter;
                int enemyPos = sEnemy->getX();
                if(enemyPos + 1 == RIGHT_BORDER_BOUNDARY && !enemiesMoveLeft) {
                    reachedBoundary = true;
                    enemiesMoveLeft = true;
                    break;
                }
                else if(enemyPos == 1 && enemiesMoveLeft) {
                    reachedBoundary = true;
                    enemiesMoveLeft = false;
                    break;
                }
            }
            //move down one, if reached the bottom end game
            if(reachedBoundary) {
                for(list<GameObject*>::iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
                    GameObject * sEnemy = *iter;
                    int enemyPos = sEnemy->getY();
                    if(enemyPos + 1 >= BOTTOM_BORDER_BOUNDARY) {
                        iter = enemies.erase(iter);
                        delete sEnemy;
                        quit = true;
                        break;
                    }
                    else {
                        //everyone moves down
                        sEnemy->movYBy(1);
                    }
                }
                reachedBoundary = false;
            }
            //move left or right instead
            else {
                if(enemiesMoveLeft) {
                    //move all enemies left once
                    for(list<GameObject*>::iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
                       GameObject * sEnemy = *iter;
                       sEnemy->movXBy(-1);
                    }
                }
                else {
                    //move all enemies right once
                    for(list<GameObject*>::iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
                       GameObject * sEnemy = *iter;
                       sEnemy->movXBy(1);
                    }
                }
            }
        }
        //calculate fire chance, add onto projectile list
        for(list<GameObject*>::iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
            int val = rand() % 10000 + 1;
            GameObject * sEnemy = *iter;
            if(sEnemy->getShape() == 'W' && val < 150) {
                //will create new ! projectile
                //push onto enemyprojectile list
                GameObject * projW = new WProjectile(sEnemy->getY(), sEnemy->getX());
                enemyProjectiles.push_back(projW);
            }
            else if(sEnemy->getShape() == 'u' && val < 75) {
                //create * projectile
                GameObject * proju = new uProjectile(sEnemy->getY(), sEnemy->getX());
                enemyProjectiles.push_back(proju);
            }
        }  

        //Projectiles move
        //Enemy projectiles once every 2 ticks, player projectiles once every tick
        if(!(counter % ENEMY_PROJ_MOV_RATE)) {
            for(list<GameObject*>::iterator iter = enemyProjectiles.begin(); iter != enemyProjectiles.end(); iter++) {
                GameObject * pEnemy = *iter;
                pEnemy->movYBy(1);
                //out of bounds then delete
                if(pEnemy->getY() > BOTTOM_BORDER_BOUNDARY) {
                    iter = enemyProjectiles.erase(iter);
                    delete pEnemy;
                }
            }
        }
        for(list<GameObject*>::iterator iter = playerProjectiles.begin(); iter != playerProjectiles.end(); iter++) {
            GameObject * pPlayer = *iter;
            pPlayer->movYBy(-1);
            if(pPlayer->getY() < Y_START) {
                iter = enemyProjectiles.erase(iter);
                delete pPlayer;
            }
        }  

        //Calculate Collisions, make sure lasers only affect opposing sides & obstacles
        for(list<GameObject*>::iterator iter = enemyProjectiles.begin(); iter != enemyProjectiles.end(); iter++) {
            GameObject * pEnemy = *iter;
            if(pEnemy->isCollision(*player)){//need to dereference?
                //enemy laser hit player, lose
                iter = enemyProjectiles.erase(iter);
                delete pEnemy;
                quit = true;
                break;
            }
	        //iterate through barriers
	        for(list<GameObject *>::iterator itBarrier = barrier.begin(); itBarrier != barrier.end(); itBarrier++) {
	            GameObject * bar = *itBarrier;
	            if(pEnemy->isCollision(*bar)) {
		            bar->damage();
		            iter = enemyProjectiles.erase(iter);
		            delete pEnemy;
		            //if the barrier is depleted, remove
		            if(!bar->getHealth()) {
		                barrier.erase(itBarrier);
		                delete bar;
		            }
		            break;
	            }
	        }
        }
        //player laser hits enemy, destory enemy, remove both from list, update points
        for(list<GameObject*>::iterator iter = playerProjectiles.begin(); iter != playerProjectiles.end(); iter++) {
	    GameObject * pPlayer = *iter;
            //iterate through enemies
            for(list<GameObject*>::iterator itEnemy = enemies.begin(); itEnemy != enemies.end(); itEnemy++) {
                GameObject * sEnemy = *itEnemy;
                if(pPlayer->isCollision(*sEnemy)) {
                    points++;
                    //delete/erase enemy + projectile from lists
                    iter = playerProjectiles.erase(iter);
                    enemies.erase(itEnemy);
                    delete pPlayer;
                    delete sEnemy;
                    break;
                }
            }
    	    //iterate through barriers
    	    for(list<GameObject *>::iterator itBarrier = barrier.begin(); itBarrier != barrier.end(); itBarrier++) {
    	        GameObject * bar = *itBarrier;
    	        if(pPlayer->isCollision(*bar)) {
    		        bar->damage();
    		        iter = playerProjectiles.erase(iter);
    		        delete pPlayer;
    		        if(!bar->getHealth()) {
    		            barrier.erase(itBarrier);
    		            delete bar;
    		        }     
    		        break;
    	        }
    	    }
        }
        //enemy collides with player, set quit = true
        for(list<GameObject*>::iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
            GameObject * sEnemy = *iter;
            if(sEnemy->isCollision(*player)) {
                quit = true;
                iter = enemies.erase(iter);
                delete sEnemy;
                break;
            }
        }
        //if the horde has been destroyed, win
        if(enemies.empty()) {
            quit = true;
            win = true;
            phaseNum++;//move on to next phase
        }

        //print player
        mvaddch(player->getY(), player->getX(), player->getShape());
	    printLists(barrier);
        printLists(enemyProjectiles);
        printLists(enemies);
        printLists(playerProjectiles);

        printBorders();

        refresh();
        move(0, 0);
    }
    erase();
    bool finish = false;
    //Make win/lose screen and print out points, or move on to next phase
    //player finished a phase
    if(win && phaseNum != MAX_WAVES) {
        mvprintw(0, 0, "Wave %i defeated. Press the space key to begin the next wave or 'q' to exit.", phaseNum + 1);
        while(!finish) {
            ch = getch();
            if(ch == ' ') {
                play(phaseNum, points, barrier);
                finish = true;
                erase();
            }
            else if(ch == 'q') {
                finish = true;
                erase();
            }
        }
    }
    //otherwise, player finished and not prematurely exited
    else if(!exited || phaseNum == MAX_WAVES) {
        //if the game has been completed
        if(win) {
            mvprintw(0, 0, "All of the enemies have been destoryed. You win!\nScore: %i\nPress 'q' to exit\n", points);
        }
        //or if the player lost
        else {
            mvprintw(0, 0, "You lose!\nScore: %i\nPress 'q' to exit\n", points);
        }
        while(!finish && !exited) {
            ch = getch();
            if(ch == 'q') {
                finish = true;
                erase();
            }
        }
    }
    freeMemory(enemies);
    freeMemory(enemyProjectiles);
    freeMemory(playerProjectiles);
    freeMemory(barrier);
    delete player;
     /*
    //free memory
    for(list<GameObject*>::iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
        GameObject* temp = *iter;
        iter = enemies.erase(iter);
        delete temp;
    }
    for(list<GameObject*>::iterator iter = enemyProjectiles.begin(); iter != enemyProjectiles.end(); iter++) {
        GameObject * temp = *iter;
        iter = enemyProjectiles.erase(iter);
        delete temp;
    }
    for(list<GameObject*>::iterator iter = playerProjectiles.begin(); iter != playerProjectiles.end(); iter++) {
        GameObject * temp = *iter;
        iter = playerProjectiles.erase(iter);
        delete temp;
    }    
    delete player;
    */
}

int main (int argc, char const *argv[]) {
    srand (time(NULL)); /// seed to some random number based on time
    if ( initscr() == NULL ) { /// init the screen, defaults to stdscr
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    noecho(); /// don't echo keypresses
    keypad(stdscr, TRUE); /// set the keypad to stdscr
    nodelay(stdscr, false);
    curs_set(0); /// set cursor to invisible (if allowed by the os)

    //intial splash screen
    bool quit = false;
    quit = startup();
    if(!quit) {
        list<GameObject *> barrier;
        barrier = createObstacles();
        play(0, 0, barrier);
    }
    endwin();
    return 0;
}
