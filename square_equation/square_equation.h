//
// Created by ilya on 06.11.18.
//

#ifndef SQUARE_EQUATION_SQUARE_EQUATION_H
#define SQUARE_EQUATION_SQUARE_EQUATION_H

#include <iostream>
#include <limits>
#include <vector>
#include <complex>

template <class Field, std::size_t Degree>
class EquationSolution {
    static const std::size_t INF = std::numeric_limits<std::size_t>::max();
    Field roots_[Degree];
    std::size_t number_of_roots_;

    EquationSolution(std::size_t number_of_roots) {
        number_of_roots_ = number_of_roots;
    }
public:

    bool IsInfinitySolution() const {
        return number_of_roots_ == INF;
    }

    template <class FieldFrom, std::size_t DegreeFrom>
    static EquationSolution FromSolution(EquationSolution<FieldFrom, DegreeFrom> solutionFrom) {
        if (solutionFrom.IsInfinitySolution()) {
            return InfinitySolution();
        }
        std::vector<Field> rootsFrom = solutionFrom.GetRoots();
        return EquationSolution(rootsFrom.begin(), rootsFrom.size());
    }

    static EquationSolution InfinitySolution() {
        return EquationSolution(INF);
    }

    static EquationSolution EmptySolution() {
        return EquationSolution(0);
    }

    template <class InputIt>
    EquationSolution(InputIt rootIt, std::size_t number_of_roots)
            : number_of_roots_(number_of_roots) {
        for (std::size_t i = 0; i < number_of_roots_; ++i, ++rootIt) {
            roots_[i] = *rootIt;
        }
    }

    std::size_t GetNumberOfRoots() const {
        return number_of_roots_;
    }

    std::vector<Field> GetRoots() const {
        if (IsInfinitySolution()) {
            return std::vector<Field>();
        }
        return std::vector<Field>(roots_, roots_ + number_of_roots_);
    }
};


template <class Field>
using SquareEquationSolution = EquationSolution<Field, static_cast<std::size_t>(2)>;

template <class Field>
using LinearEquationSolution = EquationSolution<Field, static_cast<std::size_t>(1)>;

// solve ax + b = 0
template <class Field>
LinearEquationSolution<Field> solve_linear_equation(Field a, Field b) {
    if (a == Field(0)) {
        if (b == Field(0)) {
            return LinearEquationSolution<Field>::InfinitySolution();
        } else {
            return LinearEquationSolution<Field>::EmptySolution();
        }
    } else {
        Field root = -b / a;
        return LinearEquationSolution<Field>(&root, 1);
    }
}

// solve ax^2 + bx + c = 0
template <class Field>
SquareEquationSolution<Field> solve_square_equation(Field a, Field b, Field c) {
    if (a == Field(0)) {
        return SquareEquationSolution<Field>::FromSolution(solve_linear_equation(b, c));
    } else {
        Field sqrt_d = std::sqrt(b * b - a * c * Field(4));
        if (sqrt_d != sqrt_d) { // isNan
            return SquareEquationSolution<Field>::EmptySolution();
        }
        Field roots[] = {(-b - sqrt_d) / (Field(2) * a), (-b + sqrt_d) / (Field(2) * a)};
        return SquareEquationSolution<Field>(roots, 2);
    }
}

template <class Field>
void writeSolution(SquareEquationSolution<Field> solution) {
    if (solution.IsInfinitySolution()) {
        std::cout << "Infinity solution" << std::endl;
    } else {
        auto roots_count = solution.GetNumberOfRoots();
        std::cout << "Number of roots roots: " << roots_count << std::endl;
        if (roots_count > 0) {
            std::cout << "Roots: ";
            for (auto root: solution.GetRoots()) {
                std::cout << root << " ";
            }
            std::cout << std::endl;
        }
    }
}

#endif //SQUARE_EQUATION_SQUARE_EQUATION_H
