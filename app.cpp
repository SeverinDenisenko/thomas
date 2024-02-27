#include <fstream>
#include <iomanip>
#include <iostream>

#include "array.hpp"
#include "tridiagonal.hpp"

using cmp::array;
using cmp::tridiagonal_matrix_solver;

using real = double;

int main()
{
    std::ifstream input("input.dat");
    if (!input) {
        input.close();
        std::cerr << "Bad input file." << std::endl;
        return 1;
    }

    // Error handling may be done, but we will pretend that we don't care.
    size_t n;
    if (input.peek() == EOF) {
        std::cerr << "Bad input file." << std::endl;
        return 1;
    }
    input >> n;

    array<real> a(n);
    array<real> b(n);
    array<real> c(n);
    array<real> x(n);

    // Error handling may be done, but we will pretend that we don't care.
    for (size_t i = 1; i < n; ++i) {
        if (input.peek() == EOF) {
            std::cerr << "Bad input file." << std::endl;
            return 1;
        }
        input >> a[i];
    }
    for (size_t i = 0; i < n; ++i) {
        if (input.peek() == EOF) {
            std::cerr << "Bad input file." << std::endl;
            return 1;
        }
        input >> b[i];
    }
    for (size_t i = 0; i < n - 1; ++i) {
        if (input.peek() == EOF) {
            std::cerr << "Bad input file." << std::endl;
            return 1;
        }
        input >> c[i];
    }
    for (size_t i = 0; i < n; ++i) {
        if (input.peek() == EOF) {
            std::cerr << "Bad input file." << std::endl;
            return 1;
        }
        input >> x[i];
    }

    input.close();
    std::cout << "Data read." << std::endl;

    tridiagonal_matrix_solver<real> solver;
    solver.solve_fast(x, a, b, c);
    if (!solver.good()) {
        std::cerr << "Can't solve. Bad input system. Zero determinant or division by zero." << std::endl;
        return 1;
    }

    std::cout << "Solution found." << std::endl;

    std::ofstream output("output.dat");

    output << n << "\n";
    for (size_t i = 0; i < n; ++i) {
        output << std::setprecision(3) << std::fixed << std::setw(11) << x[i];
        std::cout << std::setprecision(3) << std::fixed << std::setw(11) << x[i];
    }
    output << "\n";

    output.close();

    std::cout << "Solution writen." << std::endl;

    return 0;
}
