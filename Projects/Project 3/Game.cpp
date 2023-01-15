#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>
using namespace std;

class GameImpl
{
public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char m_symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    int m_rows;
    int m_cols;
    struct Ship
    {
        int m_length;
        char m_symbol;
        string m_name;
    };
    vector<Ship> vShips;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    if (nRows > MAXROWS)            //if nRows are are greater than MAXROWS
        m_rows = MAXROWS;               //set m_rows to MAXROWS
    else
        m_rows = nRows;             //else set m_rows to nRows
    if (nCols > MAXCOLS)            //if nCols are are greater than MAXCOLS
        m_cols = MAXCOLS;               //set m_cols to MAXCOLS
    else
        m_cols = nCols;             //if nCols are are greater than MAXCOLS
}

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_cols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    if (m_rows - length < 0 || m_cols - length < 0)         //if m_rows or m_cols are greater than length then return false
        return false;

    if (symbol == '#')          //if the symbol is the one we are using for blocked cells
    {
        cout << "Character " << symbol << " must not be used as a ship m_symbol"<< endl;
        return false;
    }

    if (!isascii(symbol)  ||  !isprint(symbol))     //if the symbol is not acsii or is not printable return false
        return false;

    Ship newShip;           //make a variable of type Ship and initialise the appropriate values
    newShip.m_length = length;
    newShip.m_symbol = symbol;
    newShip.m_name = name;

    vShips.push_back(newShip);      //push the newShip in the vector containing all ships in the game

    return true;
}

int GameImpl::nShips() const
{
    return int(vShips.size());          //return the size of the vector containing all the ships in the game
}

int GameImpl::shipLength(int shipId) const
{
    if (shipId >= nShips())     //checks if the shipID is valid
        return -1;
    return vShips[shipId].m_length;     //returns the length of the element in the vector having the specific shipId
}

char GameImpl::shipSymbol(int shipId) const
{
    if (shipId >= nShips())     //checks if the shipID is valid
        return 'X';
    return vShips[shipId].m_symbol;     //returns the symbol of the element in the vector having the specific shipId
}

string GameImpl::shipName(int shipId) const
{
    if (shipId >= nShips())     //checks if the shipID is valid
        return "";
    return vShips[shipId].m_name;           //returns the name of the element in the vector having the specific shipId
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if (!p1->placeShips(b1))            //if the placeShip function for either board is false, then return nullptr
        return nullptr;
    if (!p2->placeShips(b2))
        return nullptr;

    bool p1Turn = true;     //if it is p1's turn
    while (!(b1.allShipsDestroyed() || b2.allShipsDestroyed()))         //and both the boards have ships
    {
        if (shouldPause)        //if the shouldPause function is true
            waitForEnter();         //then wait for enter

        if (p1Turn)     //if it is p1's turn
        {
            cout << p1->name() << "'s turn. Board for " << p2->name() << ": " << endl;      //then print on the board that it is thatplayer's turn
            if (p1->isHuman())          //if the player is human then print the display and prompt the user for attack input
            {
                b2.display(true);
                cout << "Enter the row and column to attack (e.g., 3 5): ";
            }
            else
            {
                b2.display(false);
            }

            Point p = p1->recommendAttack();
            bool shotHit = false;
            bool shipDestroyed = false;
            int shipId = 0;

            if (b2.attack(p, shotHit, shipDestroyed, shipId))       //call the attack function on the point entered
            {
                p1->recordAttackResult(p, true, shotHit, shipDestroyed, shipId);
                string hitType = "";
                if (shotHit)        //if shotHit is true
                {
                    hitType = "hit something";          //then set string equal to hit something
                    if (shipDestroyed)          //if ship destroyed set the string to indicate that
                    {
                        hitType = "destroyed the " + shipName(shipId);
                    }
                }
                else
                {
                    hitType = "missed";
                }
                cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and " << hitType << ", resulting in: " << endl;      //finally indicate the complete result
                if (p1->isHuman())
                    b2.display(true);
                else
                    b2.display(false);
            }
            else
            {
                cout << p1->name() << " wasted a shot at (" << p.r << "," << p.c << ")." << endl;  //if shotHit was false then print that the player wasted a shot at the point
            }
        }
        else            //do the same for player 2
        {
            cout << p2->name() << "'s turn. Board for " << p1->name() << ": " << endl;
            if (p2->isHuman())
            {
                b1.display(true);
                cout << "Enter the row and column to attack (e.g., 3 5): ";
            }
            else
            {
                b1.display(false); // make sure displaying correct m_board
            }

            Point p = p2->recommendAttack();
            bool shotHit = false;
            bool shipDestroyed = false;
            int shipId = 0;

            if (b1.attack(p, shotHit, shipDestroyed, shipId))
            {
                p2->recordAttackResult(p, true, shotHit, shipDestroyed, shipId);
                string hitType = "";
                if (shotHit)
                {
                    hitType = "hit something";
                    if (shipDestroyed)
                    {
                        hitType = "destroyed the " + shipName(shipId);
                    }
                }
                else
                {
                    hitType = "missed";
                }
                cout << p2->name() << " attacked (" << p.r << "," << p.c << ") and " << hitType << ", resulting in: " << endl;
                if (p2->isHuman())
                    b1.display(true);
                else
                    b1.display(false);
            }
            else
            {
                cout << p2->name() << " wasted a shot at (" << p.r << "," << p.c << ")." << endl;
            }
        }
        p1Turn = !p1Turn;
    }

    if (b1.allShipsDestroyed())         //if all the ships are destroyed for either board then reflect the opponent as winner
    {
        cout << p2->name() << " wins!" << endl;
        if (p1->isHuman())
            b2.display(false);
        return p2;
    }
    if (b2.allShipsDestroyed())
    {
        cout << p1->name() << " wins!" << endl;
        if (p2->isHuman())
            b1.display(false);
        return p1;
    }
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char m_symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the m_board"
             << endl;
        return false;
    }
    if (!isascii(m_symbol)  ||  !isprint(m_symbol))
    {
        cout << "Unprintable character with decimal value " << m_symbol
             << " must not be used as a ship m_symbol" << endl;
        return false;
    }
    if (m_symbol == 'X'  ||  m_symbol == '.'  ||  m_symbol == 'o')
    {
        cout << "Character " << m_symbol << " must not be used as a ship m_symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == m_symbol)
        {
            cout << "Ship m_symbol " << m_symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, m_symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}
