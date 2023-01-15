#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

class Timer         //creates a Timer class
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
                std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
        : Player(nm, g), m_lastCellAttacked(0, 0){}

bool AwfulPlayer::placeShips(Board& b)
{
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
        {
            return false;
        }
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
    {
        m_lastCellAttacked.c--;
    }
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
        {
            m_lastCellAttacked.r--;
        }
        else
        {
            m_lastCellAttacked.r = game().rows() - 1;
        }
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point, bool, bool, bool, int){}

void AwfulPlayer::recordAttackByOpponent(Point){}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
public:
    HumanPlayer(std::string nm, const Game& g)
            : Player(nm, g)                 //initaliaze the HumanPlayer's name and game
    {}
    virtual ~HumanPlayer() {}
    virtual bool isHuman() const { return true; }
    virtual bool placeShips(Board& b);
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    virtual Point recommendAttack();
};

bool HumanPlayer::placeShips(Board& b)
{
    cout << name() << " must place " << game().nShips() << " ships." << endl;       //tells the user which ship it has to place
    b.display(false);
    for (int i = 0; i < game().nShips(); i++)
    {
        char direction;
        Direction d = HORIZONTAL;
        do
        {
            cout << "Enter h or v for direction of " << game().shipName(i) << "(length " <<  game().shipLength(i) << "): ";     //gets the direction in which to place the ship
            cin >> direction;
            if (direction == 'h')
                d = HORIZONTAL;
            else if (direction == 'v')
                d = VERTICAL;
            else
                cout << "Direction must be h or v." << endl;            //it prompts the user until it get's the input h or v
        }
        while (!(direction == 'h' || direction == 'v'));

        int row = 0, col = 0;
        bool valid = true;
        do
        {
            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
            getLineWithTwoIntegers(row, col);
            Point p(row, col);
            if (!game().isValid(p) || !b.placeShip(p, i, d))
            {
                valid = false;
                cout << "The ship cannot be placed there." << endl;         //prompts the user to enter the point until they enter a valid point
            }
            else
            {
                valid = true;
            }
        }
        while (!valid);
        b.display(false);
    }
    return true;
}

void HumanPlayer::recordAttackResult(Point, bool, bool, bool, int){}

void HumanPlayer::recordAttackByOpponent(Point){}

Point HumanPlayer::recommendAttack()
{
    int row, col = 0;
    getLineWithTwoIntegers(row, col);

    return Point(row, col);
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player
{
public:
    MediocrePlayer(std::string nm, const Game& g)
            : Player(nm, g), state(true), m_previousHit(0,0)
    {
        for (int r = 0; r < g.rows(); r++)
        {
            for (int c = 0; c < g.cols(); c++)
            {
                Point p = Point(r, c);
                m_allPossibleHits.push_back(p);
            }
        }
    }
    virtual ~MediocrePlayer() {}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    bool findPoint(const Point& p1, bool erase, vector<Point>& cont);
    void addSurroundingPoints(Point prevPoint);
private:
    bool state;
    Point m_previousHit;
    vector<Point> m_possibleHits;
    vector<Point> m_allPossibleHits;
};

// *********************** used in Mediocre and Good Player ************************************** //
bool recurPlaceShips(Board& b, const Game& g, int start, int end)       //places the ships in a recursive fashion for the mediocre player
{
    if (start == end)
        return true;

    for (int r = 0; r < g.rows(); r++)
    {
        for (int c = 0; c < g.cols(); c++)
        {
            Point p = Point(r, c);
            if (b.placeShip(p, start, HORIZONTAL))
            {
                if (!recurPlaceShips(b, g, start+1, end))
                {
                    b.unplaceShip(p, start, HORIZONTAL);
                }
                else
                {
                    return true;
                }
            }
            if (b.placeShip(p, start, VERTICAL))
            {
                if (!recurPlaceShips(b, g, start+1, end))
                {
                    b.unplaceShip(p, start, VERTICAL);
                } else
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool MediocrePlayer::placeShips(Board& b)
{
    int numOfTries = 0;
    while (numOfTries < 50)
    {
        b.block();
        const Game& g = game();
        if (recurPlaceShips(b, g, 0, game().nShips()))
        {
            b.unblock();
            return true;
        }
        b.unblock();
        numOfTries++;
    }
    return false;
}

// *********************** used in Mediocre and Good Player ************************************** //
bool equalPoints(Point p1, Point p2)        //chceks if the two points are equal
{
    return ((p1.r == p2.r) && (p1.c == p2.c));
}

bool MediocrePlayer::findPoint(const Point& p1, bool erase, vector<Point>& cont)
{
    vector<Point>::iterator p = cont.begin();
    for (; p != cont.end(); p++)
    {
        if (equalPoints(*p, p1))
        {
            break;
        }
    }
    if (p != cont.end())
    {
        if (erase)
            cont.erase(p);
        return true;
    }
    return false;
}

Point MediocrePlayer::recommendAttack()         //finds the point where attack is recommended by checking if the state boolean is true or false
{
    int row = 0, col = 0;
    Point attack = Point(row,col);
    if (state)
    {
        if (m_allPossibleHits.empty())
        {
            int randNum = randInt(int(m_possibleHits.size()));
            attack = m_possibleHits[randNum];
            m_possibleHits.erase(m_possibleHits.begin()+randNum);
        }
        else
        {
            int randNum = randInt(int(m_allPossibleHits.size()));
            attack = m_allPossibleHits[randNum];
            m_allPossibleHits.erase(m_allPossibleHits.begin() + randNum);
        }
    }
    else
    {
        int randNum = randInt(int(m_possibleHits.size()));
        attack = m_possibleHits[randNum];
        m_possibleHits.erase(m_possibleHits.begin()+randNum);
    }
    return attack;
}

void MediocrePlayer::addSurroundingPoints(Point prevPoint){
    int r = prevPoint.r;
    int c = prevPoint.c;
    for (int u = r-1; u >= 0 && u >= r-4; u--)
    {
        Point p(u, c);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_possibleHits.push_back(p);
        }
    }
    for (int d = r+1; d < game().rows() && d <= r+4; d++)
    {
        Point p(d, c);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_possibleHits.push_back(p);
        }
    }
    
    for (int l = c-1; l >= 0 && l >= c-4; l--)
    {
        Point p(r, l);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_possibleHits.push_back(p);
        }
    }
    for (int rig = c+1; rig < game().cols() && rig <= c+4; rig++)
    {
        Point p(r, rig);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_possibleHits.push_back(p);
        }
    }
}

void MediocrePlayer::recordAttackResult(Point p, bool, bool shotHit, bool shipDestroyed, int){
    if (state)
    {
        if (shotHit)
        {
            if (!shipDestroyed)
            {
                state = false;
                m_previousHit = p;
                addSurroundingPoints(m_previousHit);
            }
            else
            {
                state = true;
                for (vector<Point>::iterator i = m_possibleHits.begin(); i != m_possibleHits.end(); i++)
                {
                    m_allPossibleHits.push_back(*i);
                }
                if (!m_possibleHits.empty())
                {
                    m_possibleHits.erase(m_possibleHits.begin(), m_possibleHits.end());
                }
            }
        }
        else
        {
            state = true;
        }
    }
    else
    {
        if (shotHit)
        {
            if (!shipDestroyed)
            {
                state = false;
            }
            else
            {
                state = true;
                for (vector<Point>::iterator i = m_possibleHits.begin(); i != m_possibleHits.end(); i++)
                {
                    m_allPossibleHits.push_back(*i);
                }
                if (!m_possibleHits.empty())
                {
                    m_possibleHits.erase(m_possibleHits.begin(), m_possibleHits.end());
                }
            }
        }
        else if (!m_possibleHits.empty())
        {
            state = false;
        } else
        {
            state = true;
        }
    }
}

void MediocrePlayer::recordAttackByOpponent(Point){}


//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player {
public:
    GoodPlayer(std::string nm, const Game& g);
    virtual ~GoodPlayer() {}
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    bool findPoint(const Point& p1, bool erase, vector<Point>& cont);
    void addSurroundingPoints(Point prevPoint);
    void removeSurroundingPoints(Point prevPoint, std::string direction);
    void getDiagonalPoints(Point prevPoint);
private:
    bool state;
    int maxLength;
    Point m_previousHit;
    vector<Point> m_possibleHits;
    vector<int> m_possibleLengths;
    vector<Point> m_diagonalPoints;
    vector<Point> m_allPossibleHits;
};

GoodPlayer::GoodPlayer(std::string nm, const Game& g)
        : Player(nm, g), state(true), maxLength(0)
{
    for (int r = 0; r < g.rows(); r++)
    {
        for (int c = 0; c < g.cols(); c++)
        {
            Point p = Point(r, c);
            if (g.rows()/2 == r && g.cols()/2 == c)
            {
                m_diagonalPoints.push_back(p);
                getDiagonalPoints(p);
            }
            else
            {
                m_allPossibleHits.push_back(p);
            }
        }
    }

    for (int i = 0; i < game().nShips(); i++)
    {
        int len = game().shipLength(i);
        m_possibleLengths.push_back(len);
    }
    sort(m_possibleLengths.begin(), m_possibleLengths.end());
    if (!m_possibleLengths.empty())
    {
        maxLength = m_possibleLengths[m_possibleLengths.size() - 1];
    }
}

bool GoodPlayer::placeShips(Board& b)
{
    Timer timer;
    timer.start();
    int numOfTries = 0;
    while (numOfTries < 50)
    {
        b.block();
        const Game& g = game();
        if (recurPlaceShips(b, g, 0, game().nShips()))
        {
            b.unblock();
            cerr << "GoodPlayer::placeShips()" << timer.elapsed() << endl;
            return true;
        }
        b.unblock();
        numOfTries++;
    }

    cerr << "GoodPlayer::placeShips()" << timer.elapsed() << endl;
    return false;
}

bool GoodPlayer::findPoint(const Point& p1, bool erase, vector<Point>& cont)
{
    vector<Point>::iterator p = cont.begin();
    for (; p != cont.end(); p++)
    {
        if (equalPoints(*p, p1))
            break;
    }
    if (p != cont.end())
    {
        if (erase)
        {
            cont.erase(p);
        }
        return true;
    }
    return false;
}

void GoodPlayer::addSurroundingPoints(Point prevPoint)
{
    int r = prevPoint.r;
    int c = prevPoint.c;

    for (int u = r-1; u >= 0 && u > r-maxLength; u--)
    {
        Point p(u, c);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_possibleHits.push_back(p);
        }
    }
    for (int d = r+1; d < game().rows() && d < r+maxLength; d++)
    {
        Point p(d, c);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_possibleHits.push_back(p);
        }
    }
    for (int l = c-1; l >= 0 && l > c-maxLength; l--)
    {
        Point p(r, l);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_possibleHits.push_back(p);
        }
    }
    for (int rig = c+1; rig < game().cols() && rig < c+maxLength; rig++)
    {
        Point p(r, rig);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_possibleHits.push_back(p);
        }
    }
}

void GoodPlayer::removeSurroundingPoints(Point prevPoint, std::string direction)
{
    int r = prevPoint.r;
    int c = prevPoint.c;

    if (direction == "up")
    {
        for (vector<Point>::iterator p = m_possibleHits.begin(); p != m_possibleHits.end(); p++)
        {
            Point currP = *p;
            if (c == currP.c && r > currP.r)
            {
                m_allPossibleHits.push_back(currP);
                p = m_possibleHits.erase(p);
                p--;
            }
        }
    }
    else if (direction == "down")
    {
        for (vector<Point>::iterator p = m_possibleHits.begin(); p != m_possibleHits.end(); p++)
        {
            Point currP = *p;
            if (c == currP.c && r < currP.r)
            {
                m_allPossibleHits.push_back(currP);
                p = m_possibleHits.erase(p);
                p--;
            }
        }
    }
    else if (direction == "left")
    {
        for (vector<Point>::iterator p = m_possibleHits.begin(); p != m_possibleHits.end(); p++)
        {
            Point currP = *p;
            if (c > currP.c && r == currP.r)
            {
                m_allPossibleHits.push_back(currP);
                p = m_possibleHits.erase(p);
                p--;
            }
        }
    }
    else if (direction == "right")
    {
        for (vector<Point>::iterator p = m_possibleHits.begin(); p != m_possibleHits.end(); p++)
        {
            Point currP = *p;
            if (c < currP.c && r == currP.r)
            {
                m_allPossibleHits.push_back(currP);
                p = m_possibleHits.erase(p);
                p--;
            }
        }
    }
}

void GoodPlayer::getDiagonalPoints(Point prevPoint)
{
    int row = prevPoint.r-1;
    int col = prevPoint.c-1;
    while (row >= 0 && col >= 0)
    {
        Point p(row, col);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_diagonalPoints.push_back(p);
        }
        row--;
        col--;
    }

    row = prevPoint.r -1;
    col = prevPoint.c + 1;
    while (row >= 0 && col < game().cols())
    {
        Point p(row, col);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
            m_diagonalPoints.push_back(p);
        row--;
        col++;
    }

    row = prevPoint.r + 1;
    col = prevPoint.c - 1;
    while (row < game().rows() && col >= 0)
    {
        Point p(row, col);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_diagonalPoints.push_back(p);
        }
        row++;
        col--;
    }

    row = prevPoint.r + 1;
    col = prevPoint.c + 1;
    while (row < game().rows() && col < game().cols())
    {
        Point p(row, col);
        if (findPoint(p, true, m_allPossibleHits) && !findPoint(p, false, m_possibleHits))
        {
            m_diagonalPoints.push_back(p);
        }
        row++; col++;
    }
}

Point GoodPlayer::recommendAttack()
{
    Timer timer;
    timer.start();

    int row = 0, col = 0;
    Point attack = Point(row,col);

    if (state)
    {
        if (m_diagonalPoints.empty())
        {
            if (m_possibleHits.empty())
            {
                int randNum = randInt(int(m_allPossibleHits.size()));
                attack = m_allPossibleHits[randNum];
                vector<Point>::iterator it = m_allPossibleHits.begin() + randNum;
                m_allPossibleHits.erase(it);
            }
            else
            {
                int randNum = randInt(int(m_possibleHits.size()));
                attack = m_possibleHits[randNum];
                vector<Point>::iterator it = m_possibleHits.begin()+randNum;
                m_possibleHits.erase(it);
            }
        }
        else
        {
            int randNum = randInt(int(m_diagonalPoints.size()));
            attack = m_diagonalPoints[randNum];
            vector<Point>::iterator it = m_diagonalPoints.begin() + randNum;
            m_diagonalPoints.erase(it);
        }
    }
    else
    {
        if (m_possibleHits.empty())
        {
            if (m_diagonalPoints.empty())
            {
                int randNum = randInt(int(m_allPossibleHits.size()));
                attack = m_allPossibleHits[randNum];
                vector<Point>::iterator it = m_allPossibleHits.begin() + randNum;
                m_allPossibleHits.erase(it);
            }
            else
            {
                int randNum = randInt(int(m_diagonalPoints.size()));
                attack = m_diagonalPoints[randNum];
                vector<Point>::iterator it = m_diagonalPoints.begin() + randNum;
                m_diagonalPoints.erase(it);
            }
        }
        else
        {
            int randNum = randInt(int(m_possibleHits.size()));
            attack = m_possibleHits[randNum];
            vector<Point>::iterator it = m_possibleHits.begin()+randNum;
            m_possibleHits.erase(it);
        }
    }

    cerr << "GoodPlayer::recommendAttack(): " << timer.elapsed() << endl;
    return attack;
}

void GoodPlayer::recordAttackResult(Point p, bool, bool shotHit, bool shipDestroyed, int shipId){
    Timer timer;
    timer.start();

    if (state)
    {
        if (shotHit)
        {
            if (!shipDestroyed)
            {
                state = false;
                m_previousHit = p;
                addSurroundingPoints(p);
            }
            else
            {
                state = true;
                for (vector<Point>::iterator i = m_possibleHits.begin(); i != m_possibleHits.end(); i++)
                {
                    m_allPossibleHits.push_back(*i);
                }
                if (!m_possibleHits.empty())
                {
                    m_possibleHits.erase(m_possibleHits.begin(), m_possibleHits.end());
                }
                int len = game().shipLength(shipId);
                vector<int>::iterator p = find(m_possibleLengths.begin(), m_possibleLengths.end(), len);
                if (p != m_possibleLengths.end())
                {
                    m_possibleLengths.erase(p);
                }
                if (!m_possibleLengths.empty())
                    maxLength = m_possibleLengths[m_possibleLengths.size()-1];
            }
        }
        else
        {
            state = true;
            getDiagonalPoints(p);
        }
    }
    else
    {
        if (shotHit)
        {
            if (!shipDestroyed)
            {
                state = false;
            }
            else
            {
                state = true;
                for (vector<Point>::iterator i = m_possibleHits.begin(); i != m_possibleHits.end(); i++){
                    m_allPossibleHits.push_back(*i);
                }
                if (!m_possibleHits.empty())
                {
                    m_possibleHits.erase(m_possibleHits.begin(), m_possibleHits.end());
                }
                int len = game().shipLength(shipId);
                vector<int>::iterator p = find(m_possibleLengths.begin(), m_possibleLengths.end(), len);
                if (p != m_possibleLengths.end())
                {
                    m_possibleLengths.erase(p);
                }
                if (!m_possibleLengths.empty())
                {
                    maxLength = m_possibleLengths[m_possibleLengths.size()-1];
                }
            }
        }
        else if (!m_possibleHits.empty())
        {
            state = false;
            if (m_previousHit.c == p.c)
            {
                removeSurroundingPoints(p, "left");
            }
            else if (m_previousHit.c == p.c)
            {
                removeSurroundingPoints(p, "right");
            }
            else if (m_previousHit.r == p.r)
            {
                removeSurroundingPoints(p, "up");
            }
            else if (m_previousHit.r == p.r)
            {
                removeSurroundingPoints(p, "down");
            }
        }
        else
        {
            state = true;
        }
    }
    cerr << "GoodPlayer::recordAttackResult(): " << timer.elapsed() << endl;
}

void GoodPlayer::recordAttackByOpponent(Point){}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] =
    {
            "human", "awful", "mediocre", "good"
    };

    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  && type != types[pos]; pos++);
    switch (pos)
    {
        case 0:  return new HumanPlayer(nm, g);
        case 1:  return new AwfulPlayer(nm, g);
        case 2:  return new MediocrePlayer(nm, g);
        case 3:  return new GoodPlayer(nm, g);
        default: return nullptr;
    }
}
