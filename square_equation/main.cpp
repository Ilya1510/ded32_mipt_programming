#include <iostream>
#include "square_equation.h"

int main() {
    double a, b, c;
    std::cout << "Enter coefficients a, b, c" << std::endl;
    std::cin >> a >> b >> c;
    std::cout << "a=" << a << ", b=" << b << ", c=" << c << std::endl;

    std::cout << "Solution in real numbers" << std::endl;
    auto solution_real = solve_square_equation<double>(a, b, c);
    writeSolution(solution_real);

    std::cout << "Solution in complex numbers" << std::endl;
    auto solution_complex = solve_square_equation<std::complex<double>>(a, b, c);
    writeSolution(solution_complex);

    return 0;
}