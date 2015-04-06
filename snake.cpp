//Program: Snake Game
//Author: Terry Carson
//Date: Oct 23 2014
#include <iostream>
#include <ctime> //provides time() function
#include <vector> //used to store snake's coordinates
#include <stdio.h> //printf() is used for its speed advantage over cout
#include <windows.h> //provides Sleep() function
#include <conio.h> //provides getch() and kbhit() functions
using namespace std;

struct Cell {
       int x, y, type;
};

struct Coord {
       int x, y;
       Coord(int x, int y): x(x) ,y(y) {}
};

class Game {
    public:
        bool crash;
        char currdir, lastchar;
        int xmax, ymax;

        //Initialize x, y plane
        void init() { 
             system("CLS");
        
             for (y = 0; y < 20; y++) {
                for (x = 0; x < 40; x++) {
                     cell[x][y].type = 1;
                }
             }
                
             for (x = 0; x < 40; x++) {
                cell[x][0].type = 0;
                cell[x][19].type = 0;
             }
    
             for (y = 0; y < 20; y++) {
                cell[0][y].type = 0;
                cell[39][y].type = 0;
             }
        }
                
        //Initialize variables related to the snake
        void initsnake() {
             sx = 1;
             sy = 1;
             slength = 0;
             snake.clear(); //Reset the snake's coords
             snake.push_back(Coord(1, 1)); //Add the first position to snake's coords
             cell[1][1].type = 3;
             crash = false;
             currdir = 'd'; //If not initialized, there will be issues
             lastchar = 'd'; //If not initialized, there will be issues
        }
        
        //Move cursor to x,y coords
        void gotoxy(int x, int y) {
             HANDLE con;
             COORD cursor;
             
             cursor.X = x;
             cursor.Y = y;
             con = GetStdHandle(STD_OUTPUT_HANDLE);
             SetConsoleCursorPosition(con, cursor);
        }

        //Draw the board
        void draw() {
             int x, y;
             
             for (y = 0; y < 20; y++) {
                      for (x = 0; x < 40; x++) {
                          switch(cell[x][y].type) {
                              case 0: //Barrier
                                printf("x");
                                break;
                              case 1: //Empty
                                printf(" ");
                                break;
                              case 3: //Snake
                                printf("o");
                                break;
                          }
                      }
                      printf("\n");
             }
        }
        
        //Generate x, y coords for food
        void genfood () {
             int min(1), xmax(38), ymax(18);
        
             do {
                 x = min + (rand() % (int)(xmax - min + 1));
                 y = min + (rand() % (int)(ymax - min + 1));
             } while (cell[x][y].type != 1);
             
             gotoxy(x, y);
             printf("+");
             cell[x][y].type = 4;
        }

        //Check if the snake has touched the wall
        bool crashed() {
             if ((sx >= 39) || (sy >= 19) || (sx <= 0) || (sy <= 0)) {
                return true;
             } else {
                return false;
             }
        }
        
        //Move the snake's coords
        void move(char dir) {
             switch (dir) {
                    case 'w':
                         sy--;
                         lastchar = 'w';
                         break;
                    case 's':
                         sy++;
                         lastchar = 's';
                         break;
                    case 'a':
                         sx--;
                         lastchar = 'a';
                         break;
                    case 'd':
                         sx++;
                         lastchar = 'd';
                         break;
                    default: //In case the user hits a key other than w, a, s, d
                         switch (lastchar) {
                                case 'w':
                                     sy--;
                                     lastchar = 'w';
                                     break;
                                case 's':
                                     sy++;
                                     lastchar = 's';
                                     break;
                                case 'a':
                                     sx--;
                                     lastchar = 'a';
                                     break;
                                case 'd':
                                     sx++;
                                     lastchar = 'd';
                                     break;
                                default:
                                     printf("Error!\n");
                                     break;
                         }
                    break;
             } 
        }
        
        //Set snake's direction
        void setdir(char input) {
             if (input == 'w') {
                switch (currdir) {
                       case 'a':
                            currdir = 'w';
                            break;
                       case 'd':
                            currdir = 'w';
                            break;
                       default:
                            break;
                }
             } else if (input == 's') {
                switch (currdir) {
                       case 'a':
                            currdir = 's';
                            break;
                       case 'd':
                            currdir = 's';
                            break;
                       default:
                            break;
                }     
             } else if (input == 'a') {
                 switch (currdir) {
                       case 'w':
                            currdir = 'a';
                            break;
                       case 's':
                            currdir = 'a';
                            break;
                       default:
                            break;
                }
             } else if (input == 'd') {
                 switch (currdir) {
                       case 'w':
                            currdir = 'd';
                            break;
                       case 's':
                            currdir = 'd';
                            break;
                       default:
                            break;
                }
             } else {
                 currdir = lastchar;
             }
        }

        //Update the snake's coords and redraw
        void updatesnake() {
             crash = crashed();
             
             if ((cell[sx][sy].type == 3) && (slength > 0)) { //If the snake has hit itself
                crash = true;
             }
             
             if (!eat()) {
                snake.insert(snake.begin(), Coord(sx, sy)); //Update vector's first value
                cell[snake[snake.size()- 1].x][snake[snake.size() - 1].y].type = 1; //Set previous cell to blank
                erasesnake(); //Erase previous cell
                snake.pop_back(); //Erase vector's last value
             } else {
                slength++;
                snake.insert(snake.begin(), Coord(sx, sy)); //Update vector's first value
                snake.push_back(Coord(snake[0 + slength].x, snake[0 + slength].y)); //Add a new coordinate to the back of the vector
                genfood();
             }    
             
             Sleep(60);
             
             for (int i = 0; i < snake.size(); i++) {
                 cell[snake[i].x][snake[i].y].type = 3; // Set each cell to the snake type
             }
             
             drawsnake();  
             gotoxy(0, 20);
             printf("Score: %u", slength * 10);    
        }
        
    private:
        Cell cell[40][20];
        int x, y, sx, sy, slength;
        vector <Coord> snake;
        
        //Check if the snake has touched food
        bool eat() {
             if (cell[sx][sy].type == 4) {
                return true;
             } else {
                return false;
             }
        }
        
        //Draw the snake
        void drawsnake() {
             if (slength == 0) {
                 gotoxy(snake[0].x, snake[0].y);
                 printf("o");
                 gotoxy(snake[1].x, snake[1].y);
                 printf(" ");
             } else {
                 for (int i = 0; i < snake.size(); i++) {
                     gotoxy(snake[i].x, snake[i].y);
                     printf("o");
                 }
             }
        }
        
        //Erase snake after it moves
        void erasesnake() {
             if (slength > 0) {
                 gotoxy(snake[snake.size() - 1].x, snake[snake.size() - 1].y); //Erase the previous cell
                 printf(" ");
             }
        }     
};

int main() {
    Game game;
    char choice;
    
    do {
       srand((unsigned)time(0)); //Seed the random generator to get different values each time
       game.init();
       game.initsnake();
       game.draw();
       game.genfood();

       while (!game.crash) {
              game.setdir(getch());
              do {
                  game.move(game.currdir);
                  game.updatesnake();
              } while ((!kbhit()) && (!game.crash));
       }
       
       game.gotoxy(0, 21);
       printf("Would you like to play again? (y/n): ");
       cin >> choice;
    } while ((choice == 'y') || (choice == 'Y'));
    
    system("pause");
    return 0;
}