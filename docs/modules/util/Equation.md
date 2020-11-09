# EGE::EquationResult

## Syntax

```
#include <ege/util/Equation.h>

struct EquationResult
{
  ...
};
```

## Members

* `enum Count`
Enumeration for defining equation solution count (None - 0, Discrete - <1;inf), Infinite - inf).

## Member fields

* `std::vector<double> results`  
The solution array. It's applicable only for `Discrete` solution count.

* `Count resultCount`  
The solution count (None, Discrete or Infinite)

## Member functions

* `EquationResult(Count _count = None)`  
Construct an EquationResult from solution count (`None`, `Discrete`, `Infinite`).

* `EquationResult(std::vector<double> _results)`  
Construct an EquationResult from solution array. The count is set to `Discrete`

* `EquationResult(double _result)`  
Construct an EquationResult from single solution. The count is set to `Discrete`.  
`EquationResult(result)` is equivalent to `EquationResult({result})`.

* `bool operator==(Count _count) const`  
Check if `EquationResult` count is `_count`.

* `bool operator==(std::vector<double> _results) const`  
Check if `EquationResult` has the solutions same as `_results`.

* `bool operator==(double _result) const`  
Check if `EquationResult` has a single solution equal to `_result`.

# EGE::GenericEquation

## Syntax

```
#include <ege/util/Equation.h>

class GenericEquation
{
  ...
};
```

## Pure-virtual members

* `virtual EquationResult solve() const = 0`  
Solve the equation and return result.


# EGE::LinearEquation
The class reprezenting linear equation.

## Member functions

* `LinearEquation(double a, double b)`  
Construct a linear equation (line) from `a` and `b` factors. The equation (line) will be now `ax + b = 0`.

* `LinearEquation(Vec2d pa, Vec2d pb)`  
Construct a linear equation (line) from 2 points. The points must have different `x` coordinate, otherwise behaviour is undefined.

* `virtual EquationResult solve() const override`  
Solve equation (find `x` for which `y=0`).

* `double a()`  
*Returns*: `a` factor.

* `double b()`  
*Returns*: `b` factor.

# EGE::SquareEquation
The class reprezenting square equation.

## Member functions

* `SquareEquation(double a, double b, double c)`  
Construct a square equation from `a`, `b`, and `c` factors. The equation will be now `ax^2 + bx + c = 0`.

* `virtual EquationResult solve() const`  
Solve equation (find `x` for which `y=0`).

* `double delta() const`  
*Returns*: `delta` (`b^2-4ac`).

* `double a() const`  
*Returns*: `a` factor.

* `double b() const`  
*Returns*: `b` factor.

* `double c() const`  
*Returns*: `b` factor.

# EGE::Equation namespace
The namespace for convenience functions.

## Member functions
* `EquationResult linear(double a, double b)`  
Solves a linear equation.  
*Result*: A solution of linear equation `ax + b = 0`.

* `EquationResult square(double a, double b, double c)`  
Solves a square equation.  
*Result*: A solution of square equation `ax^2 + bx + c = 0`.

