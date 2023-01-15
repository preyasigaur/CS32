#include "History.h"
#include "globals.h"

#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
:m_rows(nRows),m_cols(nCols){
    
    m_grid=new int*[nRows];
    for(int j=0;j<nRows;j++){
        m_grid[j]=new int [nCols];
        for(int l=0;l<nCols;l++){
            m_grid[j][l]=0;
        }
    }
}

History::~History(){
    for(int i=0;i<m_rows;i++){
        delete [] m_grid[i];
    }
    delete [] m_grid;
}

bool History::record(int r, int c){
    if(r<=0||r>m_rows){         //if the number of rows exceed the total number of rows allowed in mesa
        if(c<=0||c>m_cols){     //if the number of rows and columns both are beyond the mesa i.e. either negative or exceed the mex allowed
            return false;
        }
        return false;
    }
    else if (c<=0||c>m_cols){       //if the number of columns are not within the mesa
        return false;}
    {
        m_grid[r-1][c-1]++; //if both row and columns are within the bound of the mesa, incremement the space
        return true;
    }}
    
void History::display() const{
    clearScreen();          //clear the screen
    for(int r=0;r<m_rows;r++){ //to move through each row by incrememnting one by one
        for(int c=0; c<m_cols;c++){ //to move through each column by incrememnting one by one
            if(m_grid[r][c]==0){ //if there is zero history of attack, put a '.' in the grid
                cout<<'.';}
            else if(m_grid[r][c]<=26){ //if there have been attacks upto 26 times without the gark dying at that space,return the letters 'A' to 'Z'
                int num= m_grid[r][c];
                char letter = ((num%26) - 1) + 'A';
                cout<< letter;}
            else if(m_grid[r][c]>26){ //if there are more than 26 attacks at that space, return 'Z' for that position
                cout<<'Z';
            }
        }
        cout<<endl;
    }
    cout<<endl;
}
