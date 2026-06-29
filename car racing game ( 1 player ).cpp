#include <iostream>
#include <conio.h>          // ( _kbhit, _getch )
#include <windows.h>        // console operating, ( SetConsoleTextAttribute, Sleep() )
#include <ctime>            // current system time, time used in random seed
#include <cstdlib>          // ( srand )
using namespace std;

// ==========================================
// Base Class
// ==========================================
class vehicle   // abstract class
{
    protected:
        int lane,   y;      
    public:
        virtual void move() = 0;  // pure virtual func, overriding in child classes 
        int getLane() 
		{
		    return lane; 
		}
        int getY()    
		{ 
		    return y;    
		}
};

// ==========================================
// PlayerCar Class 
// ==========================================
class playerCar : public vehicle 
{
    public:
        playerCar()    // seeding player's initial coordinate states onto the game-board grid
		{ 
		    lane = 1; 
			y = 10; 
		}
        void moveLeft()  
		{ 
		    if ( lane > 0 ) 
			    lane--;      // decrements the track lane value to step leftward if space exists
		}
        void moveRight() 
		{ 
		    if ( lane < 2 ) 
			    lane++;     // increments the track lane value to step rightward if space exists
		}
        void move() override 
		{ // kbhit is boolean func that treturn true when keyboard hit
            if (kbhit())   // triggers keyboard, if a user tapped any key without pausing execution
			{
                int ch = _getch();  // doesnt echoe char to screen
				// non-alphanumeric key has been pressed.
                if ( ch == 0 || ch == 224 )   // arrow keys are 2-byte extended values, 1st byte returns an escape alert prefix sequence (0 or 224).
				{ 
                    ch = _getch(); 
                    if ( ch == 75 )   
					    moveLeft();     // maps left arrow for left lanes
                    if ( ch == 77 )       // maps right arrow for right lanes
					    moveRight(); 
                }
            }
        }
};

// ==========================================
// EnemyCar Class 
// ==========================================
class enemyCar : public vehicle 
{
    private:   // encapsulation layer
        char e_char; // unique alphabet assigned 
        int e_color; // windows API terminal color attributek
    public:
        enemyCar( int l ) 
		{ 
            lane = l; 
            y = 0; 
            e_char = 'A' + ( rand() % 26 );   // offsets base ASCII value 'A' (65) by 0-25 to yield a completely random letter
            e_color = 1 + ( rand() % 15 );      // generates an arcade color token code ranging between 9 (Blue) and 14 (Yellow)
        }
        void move() override 
		{ 
		    y++;   // increments altitude coordinate, moving it downward per frame
		}  
        void reset( int l ) // recycling func, reset vehicle back to top
		{ 
            lane = l;    // assigns car to new random road lane track
            y = 0;       // tracks pointer back to the top row line
            e_char = 'A' + ( rand() % 26 ); 
            e_color = 1 + ( rand() % 15 );        // swaps to a brand new random color every single time it resets!
        }
        char getChar()   
		{ 
		    return e_char; 
		}
        int getColor()   
		{
		    return e_color; 
		} 
};

// ==========================================
// Game Engine Class
// ==========================================
class game 
{
    playerCar player;
    enemyCar enemy1, enemy2;   // entities using the same base blueprint block (Composition)
    int score;
    bool game_over;
    HANDLE hConsole; // Windows-specific resource tracking pointer used to interact with and change color states in the output window
    
    public:
        game() :    // constructor uses mem initialization list
		        enemy1 ( rand() % 3 ), 
				enemy2 ( rand() % 3 )   // random starting lane values
		{
            score = 0; 
            game_over = false;
            hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );   // connects to the active Windows command interface window to modify style changes
        }
        void drawRoad() 
		{
			COORD cursorPosition;
            cursorPosition.X = 0;
            cursorPosition.Y = 0;
            SetConsoleCursorPosition(hConsole, cursorPosition);
           // system("cls");
            for (int i = 0; i < 15; i++)  // outer matrix layout loop tracking position processing down the grid row lines
			{
                SetConsoleTextAttribute(hConsole, 8); // Grey road walls
                cout << "  |  ";
                for (int j = 0; j < 3; j++)  // inner matrix layout loop checking position track columns across the screen plane
				{
                    if (player.getY() == i && player.getLane() == j) 
					{
                        SetConsoleTextAttribute( hConsole, 15 ); // white
                        cout << "[CAR]" ; 
                    } 
                    else if ( enemy1.getY() == i && enemy1.getLane() == j ) 
					{
                        SetConsoleTextAttribute( hConsole, enemy1.getColor() ); 
                        cout << " <" << enemy1.getChar() << "> ";   // outputs bracket housing the dynamically shifting random alphabet symbol
                    } 
                    else if ( enemy2.getY() == i && enemy2.getLane() == j ) 
					{
                        SetConsoleTextAttribute( hConsole, enemy2.getColor() );   // Dynamically fetches and hooks the second hazard's color state
                        cout << " <" << enemy2.getChar() << "> "; 
                    } 
                    else 
					{
                    cout <<  "     ";   // empty tracks
                    }
                    SetConsoleTextAttribute( hConsole, 8 ); 
                    cout << "  |  ";
                }
            cout << endl;
            }
        
            SetConsoleTextAttribute( hConsole, 14 ); // Gold Scoreboard
            cout << "\n=================================" << endl;
            cout << "   SCORE: " << score << endl;
            cout << "=================================" << endl;
            SetConsoleTextAttribute( hConsole, 15 ); 
        }   
        void update()  // Calculation manager coordinating frame positions, asset cycles, and overlapping space conflicts
		{
			// individual execution queries down to all active car instances
            player.move();   
			enemy1.move(); 
			enemy2.move();
			
            if (enemy1.getY() > 14) 
			{ 
			    enemy1.reset( rand() % 3 ); 
				score++; 
			}
            if (enemy2.getY() > 14) 
			{ 
			    enemy2.reset( rand() % 3 ); 
				score++; 
			}
			  // Evaluates if the player shares an identical lane AND vertical y-coordinate index with either active enemy car.
            if ((player.getLane() == enemy1.getLane() && player.getY() == enemy1.getY()) || (player.getLane() == enemy2.getLane() && player.getY() == enemy2.getY())) 
			{
                game_over = true;
            }
        }   
        void run() 
		{
            while ( !game_over )    // Core game cycle loop: Renders text, recalculates math, and holds for 100ms
			{ 
		        drawRoad(); 
			    update(); 
				Sleep(100); 
			}
            SetConsoleTextAttribute( hConsole, 12 ); // Game over alert
            cout << "\n***********************************" << endl;
            cout << "      GAME OVER! FINAL SCORE: " << score << endl;
            cout << "***********************************" << endl;
            SetConsoleTextAttribute( hConsole, 15 ); 
        }
};
// ==========================================
// Main Entry
// ==========================================
int main() 
{ 
    srand( time(0) );   // random starting calculation table based on your laptop's system clock
    game g;     // game control framework engine object
    g.run(); 
    return 0; 
}
