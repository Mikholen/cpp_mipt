#include <boost/multi_array.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using Grid = boost::multi_array<int, 2>;

static const int N = 10;

int count_neighbors(const Grid &g, int x, int y)
{
    int cnt = 0;

    for (int dx = -1; dx <= 1; ++dx)
    {
        for (int dy = -1; dy <= 1; ++dy)
        {
            if (dx == 0 && dy == 0)
                continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < N && ny >= 0 && ny < N)
                cnt += g[nx][ny];
        }
    }

    return cnt;
}

void print_grid(const Grid &g)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            std::cout << (g[i][j] ? "█" : ".");
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

Grid step(const Grid &current)
{
    Grid next(boost::extents[N][N]);

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            int alive = current[i][j];
            int neighbors = count_neighbors(current, i, j);

            if (alive)
            {
                next[i][j] = (neighbors == 2 || neighbors == 3);
            }
            else
            {
                next[i][j] = (neighbors == 3);
            }
        }
    }

    return next;
}

int main()
{
    Grid grid(boost::extents[N][N]);
    std::fill(grid.data(), grid.data() + grid.num_elements(), 0);


    grid[1][2] = 1;
    grid[2][3] = 1;
    grid[3][1] = 1;
    grid[3][2] = 1;
    grid[3][3] = 1;

    grid[7][7] = 1;
    grid[7][8] = 1;
    grid[8][7] = 1;
    grid[8][8] = 1;

    for (int iter = 0; iter < 30; ++iter)
    {
        std::cout << "Iteration: " << iter << "\n";
        print_grid(grid);

        grid = step(grid);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}