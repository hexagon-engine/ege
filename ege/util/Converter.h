/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <memory>

namespace EGE
{

class Object;

// Converter is responsible for
// creating object in in()!
template<class I, class O = I>
class Converter
{
public:
    typedef I InputStreamType;
    typedef O OutputStreamType;

    virtual bool in(InputStreamType& input, std::shared_ptr<Object>& object) const = 0;
    virtual bool out(OutputStreamType& output, const Object& object) const = 0;
};

namespace Internal
{

template<class I, class O>
struct _ConverterInput
{
    std::shared_ptr<Object>& object;
    const Converter<I, O>& converter;

    _ConverterInput(std::shared_ptr<Object>& _o, const Converter<I, O>& _c)
    : object(_o), converter(_c) {}
};

template<class I, class O>
struct _ConverterOutput
{
    Object& object;
    const Converter<I, O>& converter;

    _ConverterOutput(Object& _o, const Converter<I, O>& _c)
    : object(_o), converter(_c) {}
};

}

template<class I, class O>
Internal::_ConverterInput<I, O> objectIn(std::shared_ptr<Object>& _o, const Converter<I, O>& _c)
{
    return Internal::_ConverterInput<I, O>(_o, _c);
}

template<class I, class O>
Internal::_ConverterOutput<I, O> objectOut(Object& _o, const Converter<I, O>& _c)
{
    return Internal::_ConverterOutput<I, O>(_o, _c);
}

template<class I, class O>
O convertTo(const I& input, const Converter<I, O>& inputConverter, const Converter<I, O>& outputConverter)
{
    std::shared_ptr<Object> tmp;
    inputConverter.in(input, tmp);
    O output;
    outputConverter.out(output, *tmp.get());
    return output;
}

} //namespace EGE

template<class I, class _I, class O>
I& operator>>(_I& stream, const EGE::Internal::_ConverterInput<I, O>& in)
{
    in.converter.in(stream, in.object);
    return stream;
}

template<class I, class _O, class O>
O& operator<<(_O& stream, const EGE::Internal::_ConverterOutput<I, O>& out)
{
    out.converter.out(stream, out.object);
    return stream;
}
