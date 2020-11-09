# EGE::Converter

## Syntax

```
#include <ege/util/Converter.h>

template<class I, class O = I>
class Converter
{
  public:
    typedef I InputStreamType;
    typedef O OutputStreamType;
}
```

## Pure-virtual members

* `virtual bool in(InputStreamType& input, std::shared_ptr<Object>& object) const = 0`
The method is overwritten by derived classes. It's intended to parse `input` of converter-specified format and save the result in `object`.
The converter is responsible for allocating space for `object`.

*Returns*: `true` if parsing succeeded, `false` otherwise.

* `virtual bool out(OutputStreamType& output, const Object& object) const = 0`
The method is overwritten by derived classes. It's intended to save `object` in converter-specified format to `output`.

*Returns*: `true` if generation succeeded, `false` otherwise.

## Example usage


```
class MyConverter : public EGE::Converter<std::string>
{
public:
  virtual bool in(std::string& input, std::shared_ptr<EGE::Object>& object) const override
  {
    std::shared_ptr<EGE::ObjectMap> objectMap = make<EGE::ObjectMap>();
    objectMap->addObject("myString", make<EGE::ObjectString>(input));
    object = objectMap;
    return true;
  }

  virtual bool out(std::string& output, const EGE::Object& object) const override
  {
    if(object.isMap())
    {
        EGE::ObjectMap& objectMap = (EGE::ObjectMap&)object;
        auto myString = objectMap.getObject("myString").lock();
        if(!myString)
            return false;

        output = myString->asString();
        return true;
    }
    else
    {
        return false;
    }
  }
};

MyConverter converter;
std::shared_ptr<EGE::Object> object;
std::string input;
converter.in(input, object);
std::string output;
converter.out(output, *object.get());

// NOTE: It won't be true for every converter!
std::cout << input << " == " << output << std::endl;
```

# EGE::objectIn

## Syntax

```
#include <ege/util/Converter.h>

template<class I, class O>
Internal::_ConverterInput<I, O> objectIn(std::shared_ptr<Object>& object, const Converter<I,O>& converter);
```

## Description
*Returns*: The object that can be passed to `converter`'s input stream.

## Example usage
```
MyConverter converter;
std::string myString = "foo";
std::shared_ptr<EGE::Object> object;
myString >> EGE::objectIn(object, converter);
std::cout << object->toString() << std::endl; // {"myString":"foo"};
```

# EGE::objectOut

## Syntax

```
#include <ege/util/Converter.h>

template<class I, class O>
Internal::_ConverterOutput<I, O> objectOut(Object& object, const Converter<I,O>& converter);
```

## Description
*Returns*: The object that can be passed to `converter`'s output stream.

## Example usage
```
MyConverter converter;
EGE::ObjectMap object;
object.addString("myObject", "foo");
myString << EGE::objectOut(object, converter);
std::cout << myString << std::endl; // foo
```

# EGE::convertTo

## Syntax

```
#include <ege/util/Converter.h>

template<class I, class O>
bool convertTo(const I& input, O& output, const Converter<I, O>& inputConverter, const Converter<I, O>& outputConverter)
```

## Description
Converts `input` to object of type `O` using `inputConverter` and `outputConverter`.
*Params*:
  * `input` - The input object.
  * `inputConverter` - `Converter` used to parse the `input`.
  * `outputConverter` - `Converter` used to generate `output`.
*Returns*: `true` if conversion succeeded, `false` otherwise.

## Example usage

```
// Convert JSON to some My format.
std::ifstream json("test.json");
std::ofstream my("test.my");
EGE::convertTo(json, my, EGE::JSONConverter(), MyConverter());
```

