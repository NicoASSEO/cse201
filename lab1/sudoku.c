#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

int read_sudoku(const char *path, int grid[])
{
  int fd = open(path, O_RDONLY);
  int reading = read(fd, grid, 324);
  if (reading != 324)
  {
    printf("Error");
    return -1;
  }
  return 0;
}

void print_sudoku(int grid[])
{
  printf("---------------------------------------- \n");
  for (int i = 0; i < 9; i++)
  {
    printf(" | ");
    for (int j = 0; j < 9; j++)
    {
      if (j == 3 || j == 6 || j == 9)
      {
        printf(" | ");
      }
      if (grid[i * 9 + j] == 0)
      {
        printf(" . ");
      }
      else
      {
        printf(" %d ", grid[i * 9 + j]);
      }
    }
    printf(" | \n");

    if (i == 2 || i == 5)
    {
      printf("---------------------------------------- \n");
    }
  }

  printf("---------------------------------------- \n");
}

bool valid(int grid[], int row, int col, int try)
{
  for (int i = 0; i < 9; i++)
  {
    // Check column
    if (grid[i * 9 + col] == try)
    {
      return false;
    }
    // Check row
    if (grid[row * 9 + i] == try)
    {
      return false;
    }
  }

  // Check square

  // Initialise coordinate of upper left cornmer
  int square_col = 3 * (col / 3);
  int square_row = 3 * (row / 3);
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if (grid[9 * (square_row + i) + square_col + j] == try)
      {
        return false;
      }
    }
  }
  return true;
}

bool solve(int grid[], int row, int col)
{
  if (row == 9)
  {
    return true; // that means we've finished the sudoku, so it works.
  }
  else if (col == 9)
  {
    return solve(grid, row + 1, 0); // go to new row and start from first element
  }
  else if (grid[row * 9 + col] != 0)
  {
    return solve(grid, row, col + 1); // go to next value in the row
  }
  else
  {
    for (int i = 1; i <= 9; i++)
    {
      if (valid(grid, row, col, i))
      {
        grid[row * 9 + col] = i; // give it new attempted value
        if (solve(grid, row, col + 1))
        {
          return true; // means we've worked through all the possible cases
        }
        else
        {
          grid[row * 9 + col] = 0; // set value back to 0 and restart
        }
      }
    }
  }
  return false;
}

int main(int argc, char *argv[])
{
  int grid[81];
  read_sudoku(argv[1], grid);
  print_sudoku(grid);
  if (solve(grid, 0, 0))
  {
    printf("Here is the completed sudoku: \n");
    print_sudoku(grid);
    return 0;
  }
  else
  {
    printf("There is no solution to this sudoku.\n");
    return -1;
  }
}
