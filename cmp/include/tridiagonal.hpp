#pragma once

#include <cfenv>
#include <cstdint>

#include "array.hpp"
#include "debug.hpp"
#include "forceinline.hpp"
#include "isclose.hpp"
#include "restrict.hpp"

/*
 * Convention:
 *
 * In Ax = d matrix equation:
 *     a - subdiagonal of matrix A.
 *     b - main diagonal of matrix A.
 *     c - superdiagonal of matrix A.
 *     d - rigth part of the equation.
 *     x - equation solution.
 *
 * All arrays have length `n` and `a` & `c` must be padded with 0.
 *
 * All arrays indexed as rows in matrix A. If n - dimention of the matrix A:
 *     a - indexed [1, n - 1], a[0] = any
 *     b - indexed [0, n - 1]
 *     c - indexed [0, n - 2], a[n - 1] = any
 *     d - indexed [0, n - 1]
 *     x - indexed [0, n - 1]
 *
 * Solution `x` always passed by reference.
 * In some cases, described below, `d` is passed inside `x` and reused.
 */

/*
 * Performance & constantness notes:
 *
 * This is very important to not do any memory management on main computation path.
 * Idealy this object have to be created only once during computation.
 *
 * This abstraction is best we can do in terms of performanse vs usability when
 * we have different requarements on preserving `a`, `b`, `c` & `d`.
 * CPU caches are damn expensive, so there is no need to fill them with unnecessary data.
 *
 * There are 3 ways:
 *  1. Use `solve_fast(...)`
 *     Initialy `x` is `d` and reused. Also `c` is reused. This option is best for performanse
 *     int terms of CPU cache locality. Default ctor `tridiagonal_matrix_solver()` cat be used.
 *  2. Use `solve(...)`
 *     Initialy `x` is `d` and reused. This time `c` is preserved. Additional space have to be
 *     allocated using `tridiagonal_matrix_solver(size_t reusable)` ctor (reusable is system size).
 *  3. Use `solve_slow(...)`
 *     This preserves everything. Additional space have to be allocated as in `solve(...)`
 *     and `d` vector must be supplied separatly.
 *
 * Check for division by 0 every time is kinda stupud, this messing up prefetch & speculative execution.
 * So here I'm using floating point enviroment exceptions. This is a client responsibility to check
 * for `good()` after `solve(...)` to check for success.
 */

/*
 * Interrupt safety notes:
 *
 * This code is not interrupt-safe, correctness of output in case of interrupt is not guaranteed.
 */

/*
 * Multithreading notes:
 *
 * This code intended to use in single thread.
 */

namespace cmp
{

template <typename real = float>
class tridiagonal_matrix_solver
{
public:
    tridiagonal_matrix_solver(size_t reusable)
        : reusable_(reusable)
    {
    }

    tridiagonal_matrix_solver()
        : reusable_()
    {
    }

    void solve_fast(array<real>& x, const array<real>& a, const array<real>& b, array<real>& c)
    {
        // This checks not too expencive, but they may induce compiler garbage into main path.
        // Anyway, this is a programmer responsibility to check for sanity. So only in debug.
        if constexpr (debug()) {
            assert(x.size() == a.size());
            assert(x.size() == b.size());
            assert(x.size() == c.size());
        }

        if (!good_determinant(a, b, c)) {
            return;
        }

        prepare();

        solve(x.size(), x.data(), a.data(), b.data(), c.data());

        check();
    }

    void solve(array<real>& x, const array<real>& a, const array<real>& b, const array<real>& c)
    {
        if constexpr (debug()) {
            assert((x.size() <= reusable_.size()) && "Not enough reusable space for me!");
            assert(x.size() == a.size());
            assert(x.size() == b.size());
            assert(x.size() == c.size());
        }

        if (!good_determinant(a, b, c)) {
            return;
        }

        prepare();

        solve(x.size(), x.data(), a.data(), b.data(), c.data(), reusable_.data());

        check();
    }

    void
    solve_slow(array<real>& x, const array<real>& a, const array<real>& b, const array<real>& c, const array<real>& d)
    {
        if constexpr (debug()) {
            assert((x.size() <= reusable_.size()) && "Not enough reusable space for me!");
            assert(x.size() == a.size());
            assert(x.size() == b.size());
            assert(x.size() == c.size());
            assert(x.size() == d.size());
        }

        if (!good_determinant(a, b, c)) {
            return;
        }

        prepare();

        solve(x.size(), x.data(), a.data(), b.data(), c.data(), d.data(), reusable_.data());

        check();
    }

    bool good() const
    {
        return good_;
    }

private:
    void forceinline prepare()
    {
        std::feclearexcept(FE_ALL_EXCEPT);
        good_ = true;
    }

    void forceinline check()
    {
        if (std::fetestexcept(FE_DIVBYZERO)) {
            [[unlikely]] good_ = false;
        }
    }

    // Wery difficult to design a general algorithm for checking is matrix have a 0 determinant or not.
    // Here we proposing that this is another module responsibility to normalize input data.
    bool forceinline good_determinant(const array<real>& a, const array<real>& b, const array<real>& c)
    {
        real det = determinant(a.size(), a.data(), b.data(), c.data());

        if (isclose(det, real(0))) {
            good_ = false;
            return false;
        }

        return true;
    }

    // This function is kinda usless in case of matricies with small values.
    // But I think this realisation is beautefull (and fast), so I leave it.
    real determinant(const size_t n, const real* restrict a, const real* restrict b, const real* restrict c)
    {
        real f1;
        real f2;
        real tmp;

        f1 = real(1);
        f2 = *b;
        ++a;
        ++b;

        for (size_t i = 1; i < n; ++i) {
            tmp = *b * f2 - *a * *c * f1;
            f1  = f2;
            f2  = tmp;

            ++a;
            ++b;
            ++c;
        }

        return f2;
    }

    static void forceinline solve(
        const size_t n,
        real* restrict x,
        const real* restrict a,
        const real* restrict b,
        const real* restrict c,
        const real* restrict d,
        real* restrict r)
    {
        size_t i;

        *r = *c / *b;
        *x = *d / *b;

        for (i = 1; i < n; ++i) {
            ++a;
            ++b;
            ++c;
            ++d;
            ++x;

            real w = real(1) / (*b - *a * *r);
            ++r;
            *r = *c * w;
            *x = (*d - *a * *(x - 1)) * w;
        }

        for (i = 1; i < n; ++i) {
            --r;
            --x;
            *x -= *r * *(x + 1);
        }
    }

    static void forceinline solve(
        const size_t n,
        real* restrict x,
        const real* restrict a,
        const real* restrict b,
        const real* restrict c,
        real* restrict r)
    {
        *r = *c / *b;
        *x = *x / *b;

        for (size_t i = 1; i < n; ++i) {
            ++a;
            ++b;
            ++c;
            ++x;

            real w = real(1) / (*b - *a * *r);
            ++r;
            *r = *c * w;
            *x = (*x - *a * *(x - 1)) * w;
        }

        for (size_t i = 1; i < n; ++i) {
            --x;
            --r;
            *x -= *r * *(x + 1);
        }
    }

    static void forceinline
    solve(const size_t n, real* restrict x, const real* restrict a, const real* restrict b, real* restrict c)
    {
        *c = *c / *b;
        *x = *x / *b;

        for (size_t i = 1; i < n; ++i) {
            ++a;
            ++b;
            ++x;

            real w = real(1) / (*b - *a * *c);
            ++c;
            *c *= w;
            *x = (*x - *a * *(x - 1)) * w;
        }

        for (size_t i = 1; i < n; ++i) {
            --c;
            --x;
            *x -= *c * *(x + 1);
        }
    }

    array<real> reusable_;
    bool good_;
};
} // namespace cmp