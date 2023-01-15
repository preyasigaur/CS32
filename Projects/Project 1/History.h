#ifndef HISTORY_H
#define HISTORY_H

class History
{
  public:
    History(int nRows, int nCols);
    ~History();
    bool record(int r, int c);
    void display() const;

  private:
    int** m_grid;
    int m_rows;
    int m_cols;
};

#endif
