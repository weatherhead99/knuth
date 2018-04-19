#pragma once

namespace detail {
    
    template<typename T, typename = void>
struct iterator_deref
{
    static T deref(T op)
    {
        return op;
    };
};


template<typename T>
struct iterator_deref<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type,void>::value>::type>
{
    using V = typename std::iterator_traits<T>::value_type;
    
    static V deref(T op)
    {
        return *op;
    };
    
};
}
