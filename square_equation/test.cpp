#include "square_equation.h"
#include "gtest/gtest.h"

const double EPS = 0.00001;

TEST(SolveSquareEquationTest, RealNumbers) {
    auto solution_real = solve_square_equation<double>(1, 3, 2);
    int true_roots[] = {-2, -1};
    size_t count_roots = solution_real.GetNumberOfRoots();
    EXPECT_EQ(2, count_roots);
    int id = 0;
    for (auto root: solution_real.GetRoots()) {
        EXPECT_EQ(root, true_roots[id++]);
    }
}

TEST(SolveSquareEquationTest, ComplexNumbers) {
    double a = 1, b = 2, c = 4;
    auto solution_complex = solve_square_equation<
            std::complex<double>>(a, b, c);
    std::complex<double> true_ans[] = {
            std::complex<double>(-1, -1.73205),
            std::complex<double>(-1, 1.73205)};
    EXPECT_EQ(2, solution_complex.GetNumberOfRoots());
    int id = 0;
    for (auto root: solution_complex.GetRoots()) {
        ASSERT_TRUE(std::abs(root.real() - true_ans[id].real()) < EPS);
        ASSERT_TRUE(std::abs(root.imag() - true_ans[id++].imag()) < EPS);
    }
}

TEST(SolveLinearEquationTest, RealNumbers) {
    auto solution_real = solve_square_equation<double>(0, 3, 6);
    int true_roots[] = {-2};
    size_t count_roots = solution_real.GetNumberOfRoots();
    EXPECT_EQ(1, count_roots);
    int id = 0;
    for (auto root: solution_real.GetRoots()) {
        EXPECT_EQ(root, true_roots[id++]);
    }
}

TEST(InfinitySolutionTest, RealNumbers) {
    auto solution_real = solve_square_equation<double>(0, 0, 0);
    size_t count_roots = solution_real.GetNumberOfRoots();
    ASSERT_EQ(count_roots, std::numeric_limits<size_t>::max());
}

TEST(EmptySolutionTest, ComplexNumbers) {
    auto a = std::complex<double>(0.0, 0);
    auto b = std::complex<double>(0.0, 0);
    auto c = std::complex<double>(2.0, 5);
    auto solution_real = solve_square_equation<std::complex<double>>
            (a, b, c);
    size_t count_roots = solution_real.GetNumberOfRoots();
    ASSERT_EQ(count_roots, 0);
}