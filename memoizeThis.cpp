#include <iostream>
using namespace std;

int main()
{


 const int m = 8;
 const int n = 9;
 
  int LOOKUP_LENGTH(int X[m], int Y[n], int m, int n);
 
 int X[m] = {1, 0, 0, 1, 0, 1, 0, 1};
 int Y[n] = {0, 1, 0, 1, 1, 0, 1, 1, 0};
 int c[m][n];

 for (int i = 1; i <= m; i++)
     for (int j = 1; j<= n; j++)
          c[i][j] = -1

 cout << LOOKUP_LENGTH(X, Y, m, n);
 
 return 0;
}

 int LOOKUP_LENGTH(int X[m], int Y[n], int m, int n)
 {
  if (c[m][n]) > -1 return c[m][n];
  if (m == 0 || n == 0) //empty row or col array
     c[m][n] = 0
     else
     {
      if (X[m] = Y[m]) c[m][n] = LOOKUP_LENGTH(X, Y, m - 1, n - 1) + 1;
      else c[m][n] = max(LOOKUP_LENGTH(X, Y, m, n - 1), LOOKUP_LENGTH(X, Y, m - 1, n) );
     }
  return c[i][j];
 }
