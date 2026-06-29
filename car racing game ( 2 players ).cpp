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
		void setup( int initial_lane ) // CHANGE: distinct lanes for 2 players
        {
            lane = initial_lane;
            y = 10;
        }
        void moveLeft()  
		{ 
		    if ( lane > 0 ) 
			    lane--;      // decrements the track lane value to step leftward if space exists
		}
        void moveRight() 
		{ 
		    if ( lane < 4 )     
			    lane++;     // increments the track lane value to step rightward if space exists
		}
        void move() override 
        {
        	// Input logic moved to the central game execution manager to prevent input blocking
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
            e_color = 1 + ( rand() % 14 );      // generates an arcade color token code ranging between 9 (Blue) and 14 (Yellow)
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
            e_color = 1 + ( rand() % 14 );        // swaps to a brand new random color every single time it resets!
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
    playerCar player1, player2;
    enemyCar enemy1, enemy2, enemy3;   // entities using the same base blueprint block (Composition)
    int score1, score2;
    bool game_over;
    HANDLE hConsole; // Windows-specific resource tracking pointer used to interact with and change color states in the output window
    
    public:
        game() :    // constructor uses mem initialization list
		        enemy1 ( rand() % 5 ), 
				enemy2 ( rand() % 5 ),
				enemy3 ( rand() % 5 )  
		{
            score1 = 0; 
            score2 = 0;
			
			player1.setup( 2 );  // setting lanes, starting point
			player2.setup( 4 );
			 
            game_over = false;
            hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );   // connects to the active Windows command interface window to modify style changes
        }
        void drawRoad() 
		{
            // Replaced system("cls") with a smooth cursor reset to stop the fluctuation glitch!
            COORD cursorPosition;
            cursorPosition.X = 0;
            cursorPosition.Y = 0;
            SetConsoleCursorPosition(hConsole, cursorPosition);
            
            for (int i = 0; i < 20; i++)  // outer matrix layout loop tracking position processing down the grid row lines
			{
                SetConsoleTextAttribute(hConsole, 8); // Grey road walls
                cout << "  |  ";
                for (int j = 0; j < 5; j++)  // inner matrix layout loop checking position track columns across the screen plane
				{
                    if (player1.getY() == i && player1.getLane() == j) 
					{
                        SetConsoleTextAttribute( hConsole, 15 ); 
                        cout << " [1] " ; 
                    } 
                    else if (player2.getY() == i && player2.getLane() == j) 
					{
                        SetConsoleTextAttribute( hConsole, 15 ); 
                        cout << " [2] " ; 
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
                    else if ( enemy3.getY() == i && enemy3.getLane() == j ) 
					{
                        SetConsoleTextAttribute( hConsole, enemy3.getColor() );   // Dynamically fetches and hooks the second hazard's color state
                        cout << " <" << enemy3.getChar() << "> "; 
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
            cout << "\n=====================================================" << endl;
            cout << "     Player 1 (A/D): " << score1 << "    |    Player 2 (</>): " << score2 << endl; 
            cout << "=====================================================" << endl;
            SetConsoleTextAttribute( hConsole, 15 ); 
        }
        
        void update()  // Calculation manager coordinating frame positions, asset cycles, and overlapping space conflicts
		{
			if (_kbhit())   // Checks if a key was hit
			{
                int ch = _getch();  //Changed from getche() to _getch() so it never prints 'a' or 's' on the screen!
                if ( ch == 0 || ch == 224 )   // arrow keys prefix sequence
				{ 
                    ch = _getch();  
                    if ( ch == 75 ) player2.moveLeft();     // Left arrow -> Moves Player 2
                    if ( ch == 77 ) player2.moveRight();    // Right arrow -> Moves Player 2
                }
                else 
                {
                    if ( ch == 'a' || ch == 'A' ) player1.moveLeft();  // 'A' key -> Moves Player 1
                    if ( ch == 'd' || ch == 'D' ) player1.moveRight(); // 'D' key -> Moves Player 1
                }
            }
			
			// individual execution queries down to all active car instances
			enemy1.move(); 
			enemy2.move();
			enemy3.move();
			
            if (enemy1.getY() > 19) 
			{ 
			    enemy1.reset( rand() % 5 ); 
				score1++; 
				score2++;
			}
            if (enemy2.getY() > 19) 
			{ 
			    enemy2.reset( rand() % 5 ); 
				score1++; 
				score2++;
			}
            if (enemy3.getY() > 19) 
			{ 
			    enemy3.reset( rand() % 5 ); 
				score1++; 
				score2++;
			}
			
            // Collision detection
            if ((player1.getLane() == enemy1.getLane() && player1.getY() == enemy1.getY()) || 
                (player1.getLane() == enemy2.getLane() && player1.getY() == enemy2.getY()) ||
                (player1.getLane() == enemy3.getLane() && player1.getY() == enemy3.getY()) ||
				 
                (player2.getLane() == enemy1.getLane() && player2.getY() == enemy1.getY()) || 
                (player2.getLane() == enemy2.getLane() && player2.getY() == enemy2.getY()) ||
                (player2.getLane() == enemy3.getLane() && player2.getY() == enemy3.getY())) 
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
				Sleep(60); 
			}
            SetConsoleTextAttribute( hConsole, 12 ); // Game over alert
            cout << "\n*****************************************************" << endl;
            cout << "                     GAME OVER!                " << endl;
            cout << "*****************************************************" << endl;
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
