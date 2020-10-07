#include <iostream>
#include <string>
#include <random>
#define NORTH  0
#define EAST   1
#define SOUTH  2
#define WEST   3
#define MAZE_DIMX 15
#define MAZE_DIMY 10

using namespace std;

struct player_info {
   int x, y;   //position coordinate
   int direction; //direction facing
};

struct Maze {
    int north, south, east, west; //0, 1, 2, 3 respectively
    int isEntrance; //entrance
   int isExit; //exist
   int visited; //1 if visited, 0 if not visited
};

struct Location {
   int x, y; //position coordinate
};

void MazeMaker(Maze maze[MAZE_DIMY][MAZE_DIMX], player_info &player) //Conduct a depth first search to draw a random maze
{

   Location poz = { MAZE_DIMX / 2, MAZE_DIMY / 2 };   // Set our starting location to center of map.

   //Record of what is visited and what is not visited
   vector<Location> record;

    do {
      maze[poz.y][poz.x].visited = 1;
      vector<char> verify;
        //verifys whether a path is possible in a certain direction
      if (poz.x > 0 && maze[poz.y][poz.x - 1].visited == 0) verify.push_back('W');
      if (poz.y > 0 && maze[poz.y - 1][poz.x].visited == 0) verify.push_back('N');
      if (poz.x < (MAZE_DIMX - 1) && maze[poz.y][poz.x + 1].visited == 0) verify.push_back('E');
      if (poz.y < (MAZE_DIMY - 1) && maze[poz.y + 1][poz.x].visited == 0) verify.push_back('S');

      if (verify.size()) {
         record.push_back(poz);//add position to record
         char moveDirection = verify[rand() % verify.size()]; //randomly select new direction open position
         switch (moveDirection) { //marks the adjacent cells when there is a movement (North, East, West, South) as open
            case 'N':
               maze[poz.y][poz.x].north = 1; // mark adjacent position the user just moved into as open
               poz.y--; // move position
               maze[poz.y][poz.x].south = 1; // mark adjacent position open
               break;
            case 'W':
               maze[poz.y][poz.x].west = 1;
               poz.x--;
               maze[poz.y][poz.x].east = 1;
               break;
            case 'E':
               maze[poz.y][poz.x].east = 1;
               poz.x++;
               maze[poz.y][poz.x].west = 1;
               break;
            case 'S':
               maze[poz.y][poz.x].south = 1;
               poz.y++;
               maze[poz.y][poz.x].north = 1;
         }
      }
      else { // If there are no position left to move to, it will backtrack and pop the last set of coordinates from the record. This continues until no position is left to visit
         // retrace one step back in record if no move is possible
         poz.x = record.back().x;
         poz.y = record.back().y;
         record.pop_back();
      }
   } while (record.size());

   // Set top left hand corner as entrance. Set bottom right hand corner as exit
   maze[0][0].isEntrance = 1;
   maze[MAZE_DIMY - 1][MAZE_DIMX - 1].isExit = 1;

   // Set all visited back to zero
   for (int y = 0; y < MAZE_DIMY; y++)
      for (int x = 0; x < MAZE_DIMX; x++)
         maze[y][x].visited = 0;

   // Initial position at 0,0
   player.x = 0;
   player.y = 0;
   if (maze[0][0].east) player.direction = EAST;
   else player.direction = SOUTH;
}

void MazeMapGenerator(Maze maze[MAZE_DIMY][MAZE_DIMX], player_info &player)
{
   for (int y = 0; y < MAZE_DIMY; y++) {//The player cursor/arrowhead is set for each direction
         for (int r = 0; r < 2; r++) {
            for (int x = 0; x < MAZE_DIMX; x++) {
               char mark = ' ';
               if (player.y == y && player.x == x) {
                  switch (player.direction) {
                  case NORTH:
                     mark = '^';
                     break;
                  case EAST:
                     mark = '>';
                     break;
                  case SOUTH:
                     mark = 'V';
                     break;
                  case WEST:
                     mark = '<';
                  }
               }
               char N1 = ' ', N2 = ' ';
               switch (r) {
                  case 0:
                     if (x) {
                        if (maze[y][x].visited || maze[y][x - 1].visited) N1 = '+'; //Openings in the maze are made
                        if (y) {
                           if (maze[y-1][x].visited || maze[y-1][x - 1].visited) N1 = '+';
                        }
                     }
                     else N1 = '+';

                     if (y) {
                        if (maze[y][x].visited && !maze[y][x].north) N2 = '-'; //Horizontal walls in the maze are made
                        if (maze[y - 1][x].visited && !maze[y - 1][x].south) N2 = '-';
                     }
                     else {
                        N1 = '+';
                        N2 = '-';
                     }
                     cout << N1 << N2;
                     break;
                  case 1:
                     if (x > 0) {
                        if (!maze[y][x].west && maze[y][x].visited) cout << '|' << mark;
                        else if (!maze[y][x - 1].east && maze[y][x-1].visited) cout << '|' << mark; //Vertical walls in the maze are made
                        else cout << ' ' << mark;
                     }
                     else cout << '|' << mark;
                     break;
               }
            }
            if (r) cout << "|\n";
            else cout << "+\n";
         }
   }
   for (int x = 0; x < MAZE_DIMX; x++) cout << "+-"; //Seals the maze off
   cout << '+';
}

void POV(Maze maze[MAZE_DIMY][MAZE_DIMX], const player_info &player)
{
   int wall = 0;
   int e_pos1 = 0, e_pos2 = 1;
   int exit[6] = { 0, 0, 0, 0, 0, 0 };
   int posy = player.y;
   int posx = player.x;

   maze[player.y][player.x].visited = 1;  //Position player is standing on is noted as visited

   switch (player.direction) {
      case NORTH:
         exit[e_pos1] = maze[posy][posx].west;
         exit[e_pos2] = maze[posy][posx].east;
         while (maze[posy][posx].north && wall < 3) {
            posy--;
            if (posy >= 0) {
               wall++;
               if (wall < 3) {
                  maze[posy][posx].visited = 1;
                  e_pos1 += 2;
                  e_pos2 += 2;
                  exit[e_pos1] = maze[posy][posx].west;
                  exit[e_pos2] = maze[posy][posx].east;
               }
            }
         }
         break;
      case EAST:
         exit[e_pos1] = maze[posy][posx].north;
         exit[e_pos2] = maze[posy][posx].south;
         while (maze[posy][posx].east && wall < 3) {
            posx++;
            if (posx < MAZE_DIMX) {
               wall++;
               if (wall < 3) {
                  maze[posy][posx].visited = 1;
                  e_pos1 += 2;
                  e_pos2 += 2;
                  exit[e_pos1] = maze[posy][posx].north;
                  exit[e_pos2] = maze[posy][posx].south;
               }
            }
         }
         break;
      case SOUTH:
         exit[e_pos1] = maze[posy][posx].east;
         exit[e_pos2] = maze[posy][posx].west;
         while (maze[posy][posx].south && wall < 3) {
            posy++;
            if (posy < MAZE_DIMY) {
               wall++;
               if (wall < 3) {
                  maze[posy][posx].visited = 1; // Mark location as visited as we can see it, so we know the hall layout
                  e_pos1 += 2;
                  e_pos2 += 2;
                  exit[e_pos1] = maze[posy][posx].east;
                  exit[e_pos2] = maze[posy][posx].west;
               }
            }
         }
         break;
      case WEST:
         exit[e_pos1] = maze[posy][posx].south;
         exit[e_pos2] = maze[posy][posx].north;
         while (maze[posy][posx].west && wall < 3) {
            posx--;
            if (posx >= 0) {
               wall++;
               if (wall < 3) {
                  maze[posy][posx].visited = 1; // Mark location as visited as we can see it, so we know the hall layout
                  e_pos1 += 2;
                  e_pos2 += 2;
                  exit[e_pos1] = maze[posy][posx].south;
                  exit[e_pos2] = maze[posy][posx].north;
               }
            }
         }
         break;
   }
    
    //Straight corridor

   string first_person_maze[23] = {
      R"( +-----------------------------------------+)",
      R"( | \_                                   _/ |)",
      R"( |   \_                               _/   |)",
      R"( |     \_____________________________/     |)",
      R"( |      |\_                       _/|      |)",
      R"( |      |  \_                   _/  |      |)",
      R"( |      |    \_________________/    |      |)",
      R"( |      |     |\_           _/|     |      |)",
      R"( |      |     |  \_________/  |     |      |)",
      R"( |      |     |   |\     /|   |     |      |)",
      R"( |      |     |   |       |   |     |      |)",
      R"( |      |     |   |       |   |     |      |)",
      R"( |      |     |   |       |   |     |      |)",
      R"( |      |     |   |/     \|   |     |      |)",
      R"( |      |     | _/         \_ |     |      |)",
      R"( |      |     |/             \|     |      |)",
      R"( |      |   _/                 \_   |      |)",
      R"( |      | _/                     \_ |      |)",
      R"( |      |/                         \|      |)",
      R"( |    _/                             \_    |)",
      R"( |  _/                                 \_  |)",
      R"( |_/                                     \_|)",
      R"( +-----------------------------------------+)"
   };
   
   switch (wall) {
      case 0:  // Near wall
         for (int i = 4; i <= 17; i++) first_person_maze[i].replace(9, 27, "                           ");
         first_person_maze[18].replace(9, 27, "___________________________");
         break;
      case 1:  // Mid wall
         for (int i = 7; i <= 14; i++) first_person_maze[i].replace(15, 15, "               ");
         first_person_maze[15].replace(15, 15, "_______________");
         break;
      case 2:  // Far wall
         for (int i = 9; i <= 12; i++) first_person_maze[i].replace(19, 7, "       ");
         first_person_maze[13].replace(19, 7, "_______");
         break;
      default:
         break;
   }

   // Closest exit on the left
   if (exit[0]) {
      first_person_maze[3].replace(2, 5, "_____");
      first_person_maze[18].replace(2, 6, "______");
      first_person_maze[19].replace(6, 2, "  ");
      first_person_maze[20].replace(4, 2, "  ");
      first_person_maze[21].replace(2, 2, "  ");
   }

   // Closest exit on the right
   if (exit[1]) {
      first_person_maze[3].replace(38, 5, "_____");
      first_person_maze[18].replace(37, 6, "______");
      first_person_maze[19].replace(37, 2, "  ");
      first_person_maze[20].replace(39, 2, "  ");
      first_person_maze[21].replace(41, 2, "  ");
   }


   if (wall > 0) {
      // Middle exit on the left
      if (exit[2]) {
         for (int i = 5; i <= 17; i++) first_person_maze[i].replace(9, 1, "|");
         first_person_maze[6].replace(10, 3, "___");
         first_person_maze[15].replace(10, 4, "____");
         first_person_maze[16].replace(12, 2, "  ");
         first_person_maze[17].replace(10, 2, "  ");
      }
      // Middle exit on the right
      if (exit[3]) {
         for (int i = 5; i <= 17; i++) first_person_maze[i].replace(35, 1, "|");
         first_person_maze[6].replace(32, 3, "___");
         first_person_maze[15].replace(31, 4, "____");
         first_person_maze[16].replace(31, 2, "  ");
         first_person_maze[17].replace(33, 2, "  ");
      }
   }

   if (wall > 1) {
      // Furthest exit on the left
      if (exit[4]) {
         for (int i = 8; i <= 14; i++) first_person_maze[i].replace(15, 1, "|");
         first_person_maze[8].replace(16, 1, "_");
         first_person_maze[13].replace(16, 2, "__");
         first_person_maze[14].replace(16, 2, "  ");
      }
      // Furthest exit on the right
      if (exit[5]) {
         for (int i = 8; i <= 14; i++) first_person_maze[i].replace(29, 1, "|");
         first_person_maze[8].replace(28, 1, "_");
         first_person_maze[13].replace(27, 2, "__");
         first_person_maze[14].replace(27, 2, "  ");
      }
   }

//Print the maze out
   for (int i = 0; i < 23; i++) cout << first_person_maze[i] << endl;
}

int main(int arc, char **argv)
{

   unsigned char move = 0;
   Maze maze[MAZE_DIMY][MAZE_DIMX] = {};
   player_info player;

   player.x = 0;
   player.y = 0;
   player.direction = EAST;


   MazeMaker(maze, player);
    
    POV(maze, player);
    cout << " YOUR MOVE > ";
    cin >> move;
   
    while (move != 'q') {    // Loop until q is pressed
          if (move == 'd'){ // (turn right)
            cout << "Turn Right\n";
            player.direction++;
            if (player.direction > 3) player.direction = 0;
          }
          if (move == 'a'){ // (turn left)
            cout << "Turn Left\n";
            player.direction--;
            if (player.direction < 0) player.direction = 3;
          }
          if (move == 'w'){  // (move forward)
            cout << "Move Forward\n";
            switch (player.direction) {
            case NORTH:
               if (player.y > 0 && maze[player.y][player.x].north) player.y--;
                    break;
            case EAST:
               if ((player.x < MAZE_DIMX - 1) && maze[player.y][player.x].east) player.x++;
                    break;
            case SOUTH:
               if ((player.y < MAZE_DIMY - 1) && maze[player.y][player.x].south) player.y++;
                    break;
            case WEST:
               if (player.x > 0 && maze[player.y][player.x].west) player.x--;
                    break;
            }
          }
          if (move == 's'){  // (move backward)
            cout << "Move Backward\n";
            switch (player.direction) {
            case NORTH:
               if ((player.y < MAZE_DIMY - 1) && maze[player.y][player.x].south) player.y++;
                    break;
            case EAST:
               if (player.x > 0 && maze[player.y][player.x].west) player.x--;
                    break;
            case SOUTH:
               if (player.y > 0 && maze[player.y][player.x].north) player.y--;
                    break;
            case WEST:
               if ((player.x < MAZE_DIMX - 1) && maze[player.y][player.x].east) player.x++;
                    break;
            }
         }

      if (move == 'm') {   // map
         MazeMapGenerator(maze, player);
         cout << "\n Press ANY KEY To Continue";
         cin >> move;
         cout << endl;
      }
        
        if (player.x == MAZE_DIMX - 1 && player.y == MAZE_DIMY-1) break;
    POV(maze, player);
    cout << " YOUR MOVE > ";
    cin >> move;
   }
    cout << "CONGRATULATIONS, YOU HAVE FOUND THE EXIT!!!" << endl;
    string exithall[23] = {
       R"( +-----------------------------------------+)",
       R"( | \_                                   _/ |)",
       R"( |   \_                               _/   |)",
       R"( |     \_____________________________/     |)",
       R"( |      |***************************|      |)",
       R"( |      |***************************|      |)",
       R"( |      |***************************|      |)",
       R"( |      |***************************|      |)",
       R"( |      |***************************|      |)",
       R"( |      |*     *  **  *    *       *|      |)",
       R"( |      |*  *****    ***  ****   ***|      |)",
       R"( |      |*     ***  ****  ****   ***|      |)",
       R"( |      |*  *****    ***  ****   ***|      |)",
       R"( |      |*     *  **  *    ***   ***|      |)",
       R"( |      |***************************|      |)",
       R"( |      |***************************|      |)",
       R"( |      |***************************|      |)",
       R"( |      |***************************|      |)",
       R"( |      |***************************|      |)",
       R"( |    _/                             \_    |)",
       R"( |  _/                                 \_  |)",
       R"( |_/                                     \_|)",
       R"( +-----------------------------------------+)"
    };
    MazeMapGenerator(maze, player);
    cout << " " << endl;
    for (int i = 0; i < 23; i++) cout << exithall[i] << endl;
   return 0;
}
