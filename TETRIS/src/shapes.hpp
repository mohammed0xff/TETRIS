#pragma once

#include <string>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>


enum class blockShape {
    Empty,
    I,
    O,
    S,
    Z,
    T,
    L
};


class Shapes
{
public:
    static const std::vector<std::vector<bool>> L_Shape;
    static const std::vector<std::vector<bool>> O_Shape;
    static const std::vector<std::vector<bool>> T_Shape;
    static const std::vector<std::vector<bool>> S_Shape;
    static const std::vector<std::vector<bool>> Z_Shape;
    static const std::vector<std::vector<bool>> I_Shape;


public:
    static std::vector<std::vector<bool>> getShapesBlock(blockShape shape) {
        switch (shape)
        {
        case blockShape::I:
            return I_Shape;
        case blockShape::O:
            return O_Shape;
        case blockShape::S:
            return S_Shape;
        case blockShape::Z:
            return Z_Shape;
        case blockShape::T:
            return T_Shape;
        case blockShape::L:
            return L_Shape;
        default:
            return I_Shape;
        }
    }

    static blockShape getDifferentShape(blockShape shape) {
        blockShape newShape;
        do {
            newShape = static_cast<blockShape>((rand() % 6) + 1);
        } while (newShape == shape);
        return newShape;
    }

};
