# Contributing guidelines

## Code formatting convention
- Write in C++17.
- Use shared pointers and standard templates when applicable.
- TODO: `clang-format`
- Use camelCase for all names:
```c++
class MyClass
{
  int myMethod()
  {
    const int someValue = 547;
    return someValue + m_myField;
  }
  int m_myField;
};
```
- Prefix field names with `m_`. Keep they private then. If they need to be public, do not prefix them with `m_`.
```c++
class MyClass
{
public:
  int myField = 0;
private:
  int m_myField2 = 0;
};
```
- Use assignment operator for initializing primitive types, and initializer (`{}`) for compound types:
```c++
class MyClass
{
  int m_myField1 = 0;
  Vec2d m_myVector {0.0, 5.0};
};
```
- Use `setXX` for setters and `getXX` for getters. If the class is a frequently used type (like `EGE::Vec2d`), omit `get` prefix. For these types, make name as
short as possible, but it should still show the purpose of the class. Use `const` for getters. Move
all method definitions to source files, except they are one-line (often setter and getters).
```
class OftenUsed
{
public:
  int value() const { return m_value; }
  void setValue(int x) { m_value = value; } 

private:
  int m_value = 0;
};

class MyBigClass
{
public:
  int getField() const { return m_field; }
  void setField(int field) { m_field = field; }
  void setFieldIfNotZero(int field);
  
private:
  int m_field = 0;
};

void MyBigClass::setFieldIfNotZero(int field)
{
  if(m_field != 0)
    m_field = field;
}
```
- Use `ASSERT` (preferably `ASSERT_WITH_MESSAGE` to make message more user-friendly) from `<ege/main/Config.h>` for assertions.
- Use full names for variables (e.g `string` instead of `str`)
- Do NOT use `using [namespace] ...`, except for very common definitions (e.g `make`, `log`).
- \[TODO\]

