#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>

using namespace std;

class BoardImpl
{
public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;
private:
    const Game& m_game;
    char m_board[MAXROWS][MAXCOLS];
    struct ShipInfo{
        int m_id;
        int m_numOfHits;
        char m_symbol;
    };
    vector<ShipInfo> shipInfo;
    int m_numOfShipsDestroyed;
};

BoardImpl::BoardImpl(const Game& g)
        : m_game(g), m_numOfShipsDestroyed(0)
{
    for (int r = 0; r < g.rows(); r++)
        for (int c = 0; c < g.cols(); c++)
        {
            m_board[r][c] = '.';            // places a dot for all places on the board thus setting it up
        }
}

void BoardImpl::clear()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            m_board[r][c] = '.';                // places a dot for all places on the board
        }
    }
}

void BoardImpl::block()
{
    int totalPoints = m_game.rows() * m_game.cols();            //total number of points is equal to the number of rows in game multiplied by the number of columns
    int totalBlocked = 0;               //starting a counter of total cells blocked
    while (totalBlocked < totalPoints/2)            //while total cells blocked is less than half
    {
        int randNum = randInt(totalPoints);         //generate a random point
        int col = randNum / m_game.cols();
        int row = randNum % m_game.rows();
        if (m_game.isValid(Point(row, col)))                //if the point generated is valid
        {
            if (m_board[row][col] == '.')           //and it is empty
            {
                m_board[row][col] = '#';            //then block it
                totalBlocked++;
            }
        }
    }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)     //rows are within the game board
        for (int c = 0; c < m_game.cols(); c++)          //columns are within the game board
        {
            if (m_board[r][c] == '#')           //if the spot in the board is blocked then unblock it
                m_board[r][c] = '.';
        }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId >= m_game.nShips())          //if the shipId is invalid, return false
        return false;

    int shipLength = m_game.shipLength(shipId);         //storing the length of ship in a variable
    char shipSymbol = m_game.shipSymbol(shipId);            //storing the symbol of ship in a variable

    if (!m_game.isValid(topOrLeft))         //if the point entered is valid or not
        return false;

    if ((topOrLeft.r + shipLength > m_game.rows() && dir == VERTICAL) || (topOrLeft.c + shipLength > m_game.cols() && dir == HORIZONTAL))
        return false;

    for (vector<ShipInfo>::iterator i = shipInfo.begin(); i != shipInfo.end(); i++)
    {
        if (i->m_id == shipId)          //if the particular ship has already been placed, return false
            return false;
        if (i->m_symbol == shipSymbol)
            return false;
    }
    ShipInfo newShip;           //create the ship to be pakced and assign it the appropriate data
    newShip.m_id = shipId;
    newShip.m_numOfHits = 0;
    newShip.m_symbol = shipSymbol;

    if (dir == VERTICAL){
        for (int r = topOrLeft.r; r < topOrLeft.r+shipLength; r++)
        {
            if (m_board[r][topOrLeft.c] != '.')
            {                   //if the length of the hsip in that direction exceeds the number of columns on the board, return false
                for (int i = r-1; i >= topOrLeft.r; i--)
                {
                    m_board[i][topOrLeft.c] = '.';
                }
                return false;
            }
            m_board[r][topOrLeft.c] = shipSymbol;           //if all is fine, and there's enough space to put the shit horizontally then add the ship to the board
        }
    }
    else if (dir == HORIZONTAL)
    {
        for (int c = topOrLeft.c; c < topOrLeft.c+shipLength; c++)
        {              //if the length of the hsip in that direction exceeds the number of columns on the board, return false
            if (m_board[topOrLeft.r][c] != '.')
            {
                for (int i = c-1; i >= topOrLeft.c; i--)
                {
                    m_board[topOrLeft.r][i] = '.';
                }
                return false;
            }
            m_board[topOrLeft.r][c] = shipSymbol;                //if all is fine, and there's enough space to put the shit horizontally then add the ship to the board
        }
    }
    shipInfo.push_back(newShip);            //push the new ship into the vector conatining all the ships on the board

    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId >= m_game.nShips())          //if the shipId is invalid i.e. exceeds the number of ships in the board, return false
        return false;

    if (!m_game.isValid(topOrLeft))             //if the point provided is invalid, return false
        return false;

    int shipLength = m_game.shipLength(shipId);             //storing the ship length in a variable
    char shipSymbol = m_game.shipSymbol(shipId);            //storing the ship symbol in a variable

    if (dir == VERTICAL){
        for (int r = topOrLeft.r; r < topOrLeft.r+shipLength; r++)      //traversing through the board in the particular direction starting at the point given
        {
            if (m_board[r][topOrLeft.c] != shipSymbol)          //checks if all the points do indeed contain the ship by checking their entries against the ship symbol
            {
                for (int i = r-1; i >= topOrLeft.r; i--)
                {
                    m_board[i][topOrLeft.c] = shipSymbol;
                }
                return false;
            }
            m_board[r][topOrLeft.c] = '.';             //if yes, then replace them by '.' indicating empty spot
        }
    }
    else if (dir == HORIZONTAL)
    {
        for (int c = topOrLeft.c; c < topOrLeft.c+shipLength; c++)      //traversing through the board in the particular direction starting at the point given
        {
            if (m_board[topOrLeft.r][c] != shipSymbol)      //checks if all the points do indeed contain the ship by checking their entries against the ship symbol
            {
                for (int i = c-1; i >= topOrLeft.c; i--)
                {
                    m_board[topOrLeft.r][i] = shipSymbol;
                }
                return false;
            }
            m_board[topOrLeft.r][c] = '.';          //if yes, then replace them by '.' indicating empty spot
        }
    }

    vector<ShipInfo>::iterator deleteShipInd = shipInfo.begin();            //sets an iterator for the shipInfo vector and initializes it to the beginning of the vector
    for (; deleteShipInd != shipInfo.end(); deleteShipInd++)
    {
        if (deleteShipInd->m_id == shipId)          //if the shipId is found in the vector
            break;
    }

    shipInfo.erase(deleteShipInd);          //erase the shipId
    return true;            //upon successful execution return trues
}

void BoardImpl::display(bool shotsOnly) const
{
    for (int i = 0; i < m_game.rows(); i++)
    {
        if (i == 0)
        {
            cout << "  ";
            for (int j = 0; j < m_game.cols(); j++)
            {
                cout <<  j;
            }
            cout << endl;
        }
        cout << i << " ";
        for (int j = 0; j < m_game.cols(); j++)
        {
            if (shotsOnly && (m_board[i][j] == '.' || m_board[i][j] == 'X' || m_board[i][j] == 'o'))
                cout << m_board[i][j];
            else if (!shotsOnly)
                cout << m_board[i][j];
            else
                cout << ".";
        }
        cout << endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if (!m_game.isValid(p))     //checks if the point is valid
        return false;

    char symbolHit = m_board[p.r][p.c];         //sets the ship that's hit, it's symbol to a variable symbolHit

    if (symbolHit == 'X' || symbolHit == 'o')           //if the point has previously been attacked return false
        return false;

    if (symbolHit != '.')                   //if the point has a ship part
    {
        shotHit = true;             //shotHit is true
        m_board[p.r][p.c] = 'X';
    }
    else                //if the point is empty
    {
        shotHit = false;
        m_board[p.r][p.c] = 'o';
    }

    if (shotHit){           //if shot is hit
        vector<ShipInfo>::iterator i;           //iterate through the shipInfo vector
        for (i = shipInfo.begin(); i != shipInfo.end(); i++)
        {
            if (i->m_symbol == symbolHit)           //if the symbol of the ship attacked matches the symbol in the vector
            {
                i->m_numOfHits += 1;            //increment the number of hits of that ship element by one
                break;
            }
        }
        if (i != shipInfo.end())
        {
            if (i->m_numOfHits == m_game.shipLength(i->m_id))           //if number of hits of that elemnt is equal to its length then the ship has been destroyed
            {
                shipDestroyed = true;
                shipId = i->m_id;           //store that element's m_id to shipId
                m_numOfShipsDestroyed += 1;         //increment the number of ships destroyed by 1
            }
        }
    }

    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    return m_numOfShipsDestroyed == shipInfo.size();            //if the number of ships destroyed in equal to the shipInfo vector's size, then all the ship's have been destroyed.
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
