# EGE::SquareMatrix

## Syntax

```
#include <ege/util/EquationSystem.h>

class SquareMatrix
{
  ...
};
```

## Member functions

* `SquareMatrix(const std::vector<std::vector<double>>& values)`  
Construct a square matrix from `vector` of `vector`s. The values are evaluated by rows.  

`{ {a,b}, {c,d} }` is evaluated to

```
| a b |
| c d |
```

* `SquareMatrix(const std::initializer_list<std::initializer_list<double>>& values)`
Construct a square matrix from `initializer_list` of `initializer_list`s. The values are evaluated by rows.  

`{ {a,b}, {c,d} }` is evaluated to

```
| a b |
| c d |
```

* `inline double& value(size_t x, size_t y)`  
*Returns*: A reference to value on `x`,`y`.

* `inline const double& value(size_t x, size_t y) const`  
*Returns*: A read-only reference to value on `x`,`y`.

* `inline size_t size() const`  
*Returns*: A size of side of the matrix (e.g if matrix is 4x4, `size()` returns `4`).

* `void setColumn(size_t column, const std::vector<double>& values)`  
Sets a column with specified index (`column`) to `values`. It's useful for solving equation systems.

* `double det() const`  
*Returns*: A calculated determinant of matrix.

* `std::string toString() const`  
*Returns*: A human-readable string representing the matrix.

# EGE::EquationSystemResult
An array for storing equation system results. The `EquationSystemResult` is internally an array of `EquationResult`s for each variable.  
*See*: `EGE::EquationResult`

## Member functions

* `explicit EquationSystemResult(size_t size)`  
Construct an `EquationSystemResult` with specified size. The internal array is resized to specified `size`.

* `EquationResult resultFor(size_t variable) const`  
Returns an `EquationResult` for specified `variable`.

* `void setResult(size_t variable, const EquationResult& result)`  
Sets a result for specified `variable` to `result`.

# EGE::GenericEquationSystem
A class for solving equation systems.

## Pure-virtual members
* `virtual EquationSystemResult solve() const = 0`  
Solves an equation system.

# EGE::LinearEquationSystem
A class for solving linear (Cramer) equation systems.

## Member functions
* `LinearEquationSystem(const SquareMatrix& as, const std::vector<double>& bs)`  
Construct an equation from `a` factors (for variables) and `b` factors (as right side). The size of `as` should be the same as `bs`.  

The equation is now of the form:
```
  <--------------------- as -------------------> <- bs ->
{ a_11*x_0 + a_12*x_1 + a_13*x_2 + ... + a1n*dx_n =  b1
{ a_21*x_0 + a_22*x_1 + a_23*x_2 + ... + a2n*x_n  =  b2
{ a_31*x_0 + a_32*x_1 + a_33*x_2 + ... + a3n*x_n  =  b3
{ ...
{ a_n1*x_0 + a_n2*x_1 + a_n3*x_2 + ... + ann*x_n  =  bn
```

* `virtual EquationSystemResult solve() const`  
Solve an equation system. The results are in the form `EquationSystemResult{x_1, x_2, x_3, ..., x_n}`

* `inline size_t solutions() const`  
*Returns*: A solution count. It should be equal to equation count.

# EGE::EquationSystem namespace
A namespace for equation-system related convenience functions.

## Member functions
* `EquationSystemResult linear(const SquareMatrix& as, const std::vector<double>& bs)`  
Solves a linear equation system with factors `as` and `bs`, as in `LinearEquationSystem`.

