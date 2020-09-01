/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

namespace EGE
{

class ObjectMap;

template<class InputStreamType, class OutputStreamType = InputStreamType>
class Converter
{
public:
    virtual bool in(InputStreamType& input, ObjectMap& object) const = 0;
    virtual bool out(OutputStreamType& output, const ObjectMap& object) const = 0;
};

namespace Internal
{

template<class I, class O>
struct _ConverterInput
{
    ObjectMap& object;
    const Converter<I, O>& converter;

    _ConverterInput(ObjectMap& _o, const Converter<I, O>& _c)
    : object(_o), converter(_c) {}
};

template<class I, class O>
struct _ConverterOutput
{
    ObjectMap& object;
    const Converter<I, O>& converter;

    _ConverterOutput(ObjectMap& _o, const Converter<I, O>& _c)
    : object(_o), converter(_c) {}
};

}

template<class I, class O>
Internal::_ConverterInput<I, O> objectIn(ObjectMap& _o, const Converter<I, O>& _c)
{
    return Internal::_ConverterInput<I, O>(_o, _c);
}

template<class I, class O>
Internal::_ConverterOutput<I, O> objectOut(ObjectMap& _o, const Converter<I, O>& _c)
{
    return Internal::_ConverterOutput<I, O>(_o, _c);
}

} //namespace EGE

template<class I, class O>
I& operator>>(I& stream, const EGE::Internal::_ConverterInput<I, O>& in)
{
    in.converter.in(stream, in.object);
    return stream;
}

template<class I, class O>
O& operator<<(O& stream, const EGE::Internal::_ConverterOutput<I, O>& out)
{
    out.converter.out(stream, out.object);
    return stream;
}
