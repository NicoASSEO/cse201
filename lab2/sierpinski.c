#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct point
{
  float x, y;
};

// Function prototypes
int nb_columns();
int nb_lines();
int grid_init(char grid[], char pixel);
void plot_point(char grid[], int x, int y, char pixel);
void grid_display(char grid[]);
int roundfl(float x);
void plot_vline(char grid[], int x, float fy0, float fy1, char pixel);
void plot_ploy_sweep(char grid[], struct point points[], int len, int x, char pixel);
void plot_poly(char grid[], struct point points[], int len, char pixel);
void plot_triangle(char grid[], struct point p1, struct point p2, struct point p3, char pixel);
struct point line_middle(struct point p1, struct point p2);
void sierpinski(char grid[], struct point p1, struct point p2, struct point p3, int n, char pixel);

int nb_columns()
{
  return 65; // Adjusting grid size
}

int nb_lines()
{
  return 33; // Adjusting grid size
}

int grid_init(char grid[], char pixel)
{
  int size = nb_columns() * nb_lines();
  for (int i = 0; i < size; i++)
  {
    grid[i] = pixel;
  }
  return 0;
}

void plot_point(char grid[], int x, int y, char pixel)
{
  if (x >= nb_columns() || x < 0 || y >= nb_lines() || y < 0)
  {
    return; // Prevents accessing invalid coordinates
  }
  grid[(nb_lines() - y - 1) * nb_columns() + x] = pixel;
}

void grid_display(char grid[])
{
  int size = nb_columns() * nb_lines();
  for (int i = 0; i < size; i++)
  {
    if (i % nb_columns() == 0 && i != 0)
    {
      printf("\n");
    }
    printf("%c ", grid[i]);
  }
  printf("\n");
}

int roundfl(float x)
{
  return (int)(x + 0.5);
}

void plot_vline(char grid[], int x, float fy0, float fy1, char pixel)
{
  int y0 = roundfl(fy0);
  if ((float)(y0 - 0.5) == (float)fy0)
  {
    y0 -= 1;
  }
  int y1 = roundfl(fy1);
  for (int i = y0; i <= y1; i++)
  {
    plot_point(grid, x, i, pixel);
  }
}

void plot_ploy_sweep(char grid[], struct point points[], int len, int x, char pixel)
{
  float vlines[len * 4];
  int top = 0;

  for (int i = 0; i < len; i++)
  {
    int j = (i == 0) ? len - 1 : i - 1;
    if ((points[i].x <= points[j].x && points[i].x <= x && x < points[j].x) ||
        (points[j].x <= points[i].x && points[j].x <= x && x < points[i].x))
    {
      float intersection = points[i].y + (x - points[i].x) * (points[j].y - points[i].y) / (points[j].x - points[i].x);
      vlines[top++] = intersection;
    }
  }

  for (int i = 0; i < top - 1; i += 2)
  {
    plot_vline(grid, x, vlines[i], vlines[i + 1], pixel);
  }
}

void plot_poly(char grid[], struct point points[], int len, char pixel)
{
  for (int x = 0; x < nb_columns(); x++)
  {
    plot_ploy_sweep(grid, points, len, x, pixel);
  }
}

void plot_triangle(char grid[], struct point p1, struct point p2, struct point p3, char pixel)
{
  struct point p[3] = {p1, p2, p3};
  plot_poly(grid, p, 3, pixel);
}

struct point line_middle(struct point p1, struct point p2)
{
  return (struct point){(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
}

void sierpinski(char grid[], struct point p1, struct point p2, struct point p3, int n, char pixel)
{
  if (n == 1)
  {
    plot_triangle(grid, p1, p2, p3, pixel);
  }
  else
  {
    struct point m12 = line_middle(p1, p2);
    struct point m23 = line_middle(p2, p3);
    struct point m31 = line_middle(p3, p1);

    sierpinski(grid, p1, m12, m31, n - 1, pixel);
    sierpinski(grid, m12, p2, m23, n - 1, pixel);
    sierpinski(grid, m31, m23, p3, n - 1, pixel);
  }
}

int main(int argc, char *argv[])
{
  char grid[nb_columns() * nb_lines()];
  grid_init(grid, ' ');

  struct point p1 = {0, 0};
  struct point p2 = {32, 32};
  struct point p3 = {64, 0};

  sierpinski(grid, p1, p2, p3, 6, '-');

  grid_display(grid);
  return 0;
}
