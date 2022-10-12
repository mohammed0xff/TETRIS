#include "shapes.h"


const std::vector<std::vector<bool>> Shapes::L_Shape =
{
    {0, 0, 1},
    {1, 1, 1},
    {0, 0, 0},
};

const std::vector<std::vector<bool>> Shapes::I_Shape
{
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

const std::vector<std::vector<bool>> Shapes::T_Shape =
{
    {0, 1, 0},
    {1, 1, 1},
    {0, 0, 0},
};

const std::vector<std::vector<bool>> Shapes::O_Shape =
{
    {1, 1},
    {1, 1},
};

const std::vector<std::vector<bool>> Shapes::S_Shape =
{
    {0, 1, 1},
    {1, 1, 0},
    {0, 0, 0},
};

const std::vector<std::vector<bool>> Shapes::Z_Shape =
{
    {1, 1, 0},
    {0, 1, 1},
    {0, 0, 0},
};
