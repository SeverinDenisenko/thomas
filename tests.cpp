#include <iomanip>
#include <iostream>

#include "array.hpp"
#include "round.hpp"
#include "tridiagonal.hpp"

using cmp::array;
using cmp::tridiagonal_matrix_solver;

// Testing utility

template <typename real>
void print(const array<real>& a)
{
    std::cout << "( ";
    for (size_t i = 0; i < a.size() - 1; ++i) {
        std::cout << std::fixed << std::setw(11) << a[i] << ", ";
    }
    std::cout << std::fixed << std::setw(11) << a[a.size() - 1] << " )" << std::endl;
}

template <typename real>
void print(array<real>& a, array<real>& b, array<real>& c, array<real>& d, array<real>& x, array<real>& res)
{
    std::cout << "a:        ";
    print(a);
    std::cout << "b:        ";
    print(b);
    std::cout << "c:        ";
    print(c);
    std::cout << "d:        ";
    print(d);
    std::cout << "expected: ";
    print(res);
    std::cout << "actual:   ";
    print(x);
}

template <typename real>
void print(array<real>& a, array<real>& b, array<real>& c, array<real>& d, bool good)
{
    std::cout << "a:        ";
    print(a);
    std::cout << "b:        ";
    print(b);
    std::cout << "c:        ";
    print(c);
    std::cout << "d:        ";
    print(d);
    std::cout << "expected: " << "BAD" << std::endl;
    std::cout << "actual:   " << (good ? "GOOD" : "BAD") << std::endl;
}

template <typename real>
void test(array<real>& a, array<real>& b, array<real>& c, array<real>& x, array<real>& res)
{
    tridiagonal_matrix_solver<real> solver(x.size());

    array<real> d = x;
    array<real> cc = c;
    solver.solve_fast(x, a, b, cc);
    print(a, b, c, d, x, res);

    assert(solver.good());
    assert(res == x);
    std::cout << "PASS" << std::endl << std::endl;
}

template <typename real>
void test_bad(array<real>& a, array<real>& b, array<real>& c, array<real>& x)
{
    tridiagonal_matrix_solver<real> solver(x.size());

    array<real> d = x;
    array<real> cc = c;
    solver.solve_fast(x, a, b, cc);
    print(a, b, c, d, solver.good());

    assert(!solver.good());
    std::cout << "PASS" << std::endl << std::endl;
}

template <typename real>
void test_round(array<real>& a, array<real>& b, array<real>& c, array<real>& x, array<real>& res)
{
    tridiagonal_matrix_solver<real> solver(x.size());

    array<real> d = x;
    array<real> cc = c;
    solver.solve_fast(x, a, b, cc);
    print(a, b, c, d, x, res);

    for (size_t i = 0; i < x.size(); ++i) {
        x[i] = round(x[i], 3);
    }

    assert(solver.good());
    assert(res == x);
    std::cout << "PASS" << std::endl << std::endl;
}

// Tests from assignment

void test_first()
{
    array<double> a   = {+0.000, +0.785, +9.791};
    array<double> b   = {+0.785, -4.444, -6.681};
    array<double> c   = {+5.347, +6.681, +0.000};
    array<double> x   = {+9.122, +5.565, +3.557};

    test_bad(a, b, c, x);
}

void test_second()
{
    array<float> a   = {+00.000f, +00.568f, +03.785f, +06.728f};
    array<float> b   = {+00.334f, -56.324f, +04.785f, -08.981f};
    array<float> c   = {+07.862f, -00.874f, -00.589f, +00.000f};
    array<float> x   = {+34.134f, +05.565f, +06.781f, +05.181f};
    array<float> res = {+84.718f, +00.743f, +00.836f, +00.049f};

    test_round(a, b, c, x, res);
}

void test_last()
{
    array<float> a   = {+000.000f, +007.871f, +003.385f, +005.000f, +004.871f};
    array<float> b   = {+000.687f, +004.871f, -004.844f, +006.871f, -000.987f};
    array<float> c   = {-001.313f, -007.891f, +005.676f, +008.971f, +000.000f};
    array<float> x   = {+485.000f, +007.871f, +034.000f, +071.113f, +006.210f};
    array<float> res = {-006.307f, -372.683f, -237.340f, +025.697f, +120.527f};

    test_round(a, b, c, x, res);
}

// My tests

void test_identity()
{
    array<float> a   = {0.0f, 0.0f, 0.0f};
    array<float> b   = {1.0f, 1.0f, 1.0f};
    array<float> c   = {0.0f, 0.0f, 0.0f};
    array<float> x   = {5.0f, 6.0f, 7.0f};
    array<float> res = {5.0f, 6.0f, 7.0f};

    test(a, b, c, x, res);
}

void test_symmetric()
{
    array<float> a   = {+0.0f, -1.0f, -1.0f};
    array<float> b   = {+2.0f, +2.0f, +2.0f};
    array<float> c   = {-1.0f, -1.0f, +0.0f};
    array<float> x   = {+1.0f, +1.0f, +1.0f};
    array<float> res = {3.0f / 2.0f, 2.0f, 3.0f / 2.0f};

    test(a, b, c, x, res);
}

void test_simple()
{
    array<float> a   = {00.0f, 01.0f, 02.0f};
    array<float> b   = {03.0f, 04.0f, 05.0f};
    array<float> c   = {06.0f, 07.0f, 00.0f};
    array<float> x   = {10.0f, 10.0f, 10.0f};
    array<float> res = {-15.0f, 55.0f / 6.0f, -5.0f / 3.0f};

    test(a, b, c, x, res);
}

void test_simple_double()
{
    array<double> a   = {00.0, 01.0, 02.0};
    array<double> b   = {03.0, 04.0, 05.0};
    array<double> c   = {06.0, 07.0, 00.0};
    array<double> x   = {10.0, 10.0, 10.0};
    array<double> res = {-15.0, 55.0 / 6.0, -5.0 / 3.0};

    test(a, b, c, x, res);
}

void test_simple_reverse()
{
    array<float> a   = {0.0, 2.0, 1.0};
    array<float> b   = {5.0, 4.0, 3.0};
    array<float> c   = {7.0, 6.0, 0.0};
    array<float> x   = {1.0, 1.0, 1.0};
    array<float> res = {-9.0f / 4.0f, 7.0f / 4.0f, -1.0f / 4.0f};

    test(a, b, c, x, res);
}

void test_random()
{
    array<float> a   = {0.000f, 0.401f, 0.896f, 0.087f};
    array<float> b   = {0.240f, 0.726f, 0.264f, 0.953f};
    array<float> c   = {0.020f, 0.607f, 0.071f, 0.000f};
    array<float> x   = {0.556f, 0.035f, 0.306f, 0.388f};
    array<float> res = {2.225834012376588f, 1.089991851480938f, -2.716463794296826f, 0.655123137569595f};

    test(a, b, c, x, res);
}

void test_zero()
{
    array<float> a   = {0.0f, 0.0f, 0.0f};
    array<float> b   = {0.0f, 0.0f, 0.0f};
    array<float> c   = {0.0f, 0.0f, 0.0f};
    array<float> x   = {1.0f, 2.0f, 3.0f};

    test_bad(a, b, c, x);
}

void test_small()
{
    array<double> a   = {0.000, 0.001, 0.002};
    array<double> b   = {0.003, 0.004, 0.005};
    array<double> c   = {0.006, 0.007, 0.000};
    array<double> x   = {0.010, 0.010, 0.010};
    array<double> res = {-15.0, 55.0 / 6.0, -5.0 / 3.0};

    test(a, b, c, x, res);
}

void test_strange()
{
    array<double> a   = {0.000, 1.000, 1.000};
    array<double> b   = {0.000, 1.000, 0.000};
    array<double> c   = {1.000, 1.000, 0.000};
    array<double> x   = {1.000, 1.000, 1.000};

    test_bad(a, b, c, x);
}

void test_strangest()
{
    array<double> a   = {0.000, 1.000, 1.000};
    array<double> b   = {0.000, 0.000, 0.000};
    array<double> c   = {1.000, 1.000, 0.000};
    array<double> x   = {1.000, 1.000, 1.000};

    test_bad(a, b, c, x);
}

void test_lower()
{
    array<double> a   = {0.000, 0.000, 0.000};
    array<double> b   = {1.000, 1.000, 1.000};
    array<double> c   = {1.000, 1.000, 0.000};
    array<double> x   = {1.000, 1.000, 1.000};
    array<double> res = {1.000, 0.000, 1.000};

    test(a, b, c, x, res);
}

void test_nondominant()
{
    array<double> a   = {0.000, 10.00, 10.00};
    array<double> b   = {0.001, 0.001, 0.001};
    array<double> c   = {-1.00, -1.00, 0.000};
    array<double> x   = {1.000, 1.000, 1.000};
    array<double> res = {550.050, -0.450, 5499.500};

    test_round(a, b, c, x, res);
}

int main()
{
    std::cout << "TEST Identity:" << std::endl;
    test_identity();
    std::cout << "TEST Symmetric:" << std::endl;
    test_symmetric();
    std::cout << "TEST Simple:" << std::endl;
    test_simple();
    std::cout << "TEST Simple Double:" << std::endl;
    test_simple_double();
    std::cout << "TEST Simple Reverse:" << std::endl;
    test_simple_reverse();
    std::cout << "TEST Random:" << std::endl;
    test_random();
    std::cout << "TEST Zero:" << std::endl;
    test_zero();
    std::cout << "TEST Small:" << std::endl;
    test_small();
    std::cout << "TEST Strange:" << std::endl;
    test_strange();
    std::cout << "TEST Strangest:" << std::endl;
    test_strangest();
    std::cout << "TEST Lower:" << std::endl;
    test_lower();
    std::cout << "TEST NonDominant:" << std::endl;
    test_nondominant();

    std::cout << "TEST First:" << std::endl;
    test_first();
    std::cout << "TEST Second:" << std::endl;
    test_second();
    std::cout << "TEST Last:" << std::endl;
    test_last();
    return 0;
}
