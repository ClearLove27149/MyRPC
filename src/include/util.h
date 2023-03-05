//
// Created by 26685 on 2023/3/4.
//

#ifndef MYRPC_UTIL_H
#define MYRPC_UTIL_H

#include <functional>
#include <string>
#include <iostream>

using namespace std;
// function_traits

// 原型
template<typename T>
struct function_traits;

// 普通函数
template<typename ReturnType, typename... Args>
struct function_traits<ReturnType(Args...)>
{
    enum { arity = sizeof...(Args) };
    using return_type = ReturnType;
    using function_type = ReturnType(Args...);
    using stl_function_type = std::function<function_type>;
    using pointer = ReturnType(*)(Args...);
    //using all_args = std::tuple<Args...>;

    template<size_t I>
    struct args
    {
        static_assert(I < arity, "index is out of range, index must less than sizeof Args");
        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;

    };
    using tuple_type = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
    using bare_tuple_type = std::tuple<std::remove_const_t<std::remove_reference_t<Args>>...>;
};
//
template <class F, class = std::make_index_sequence<function_traits<F>::arity>>
struct all_args;

template <class F, size_t... Is>
struct all_args<F, std::index_sequence<Is...>> {
    using type = std::tuple<typename function_traits<F>::template args<Is>::type...>;
};

template <class F>
using all_args_t = typename all_args<F>::type;

// 函数指针
template<typename ReturnType, typename... Args>
struct function_traits<ReturnType(*)(Args...)> : function_traits<ReturnType(Args...)> {};

// std::function
template<typename ReturnType, typename... Args>
struct function_traits<std::function<ReturnType(Args...)>> : function_traits<ReturnType(Args...)> {};
// 成员函数
#define FUNCTION_TRAITS(...)\
template <typename ReturnType, typename ClassType, typename... Args>\
struct function_traits<ReturnType(ClassType::*)(Args...) __VA_ARGS__> : function_traits<ReturnType(Args...)>{};\

FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)

// 函数对象
template<typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())> {};

template<typename Function>
typename function_traits<Function>::stl_function_type to_function(const Function& lambda)
{
    return static_cast<typename function_traits<Function>::stl_function_type>(lambda);
}

template<typename Function>
typename function_traits<Function>::stl_function_type to_function(Function&& lambda)
{
    return static_cast<typename function_traits<Function>::stl_function_type>(std::forward<Function>(lambda));
}

template<typename Function>
typename function_traits<Function>::pointer to_function_pointer(const Function& lambda)
{
    return static_cast<typename function_traits<Function>::pointer>(lambda);
}

int func(int a, string b)
{
    return 0;
}
template<typename T>
void printType()
{
    cout << typeid(T).name() << endl;
}

float(*cast_func)(int, int, int, int);
float free_function(const string& a, int b)
{
    return (float)a.size() / b;
}
struct AA
{
    int f(int a, int b)volatile { return a + b; }
    int operator()(int)const { return 0; }
};

void test_function_traits() {
    {
//        // 获取函数类型
//        function_traits<decltype(func)>::function_type;     // int __cdecl(int, string)
//        // 获取函数返回值
//        function_traits<decltype(func)>::return_type;       // int
//        // 获取函数的参数个数
//        function_traits<decltype(func)>::arity;             // 2
//        // 获取函数第一个入参类型
//        function_traits<decltype(func)>::args<0>::type;     // int
//        // 获取函数第二个入参类型
//        function_traits<decltype(func)>::args<1>::type;     // string
//
//        cout << typeid(function_traits<decltype(func)>::function_type).name() << endl;
//        cout << typeid(function_traits<decltype(func)>::return_type).name() << endl;
    }
    {
        cout << "----------- 2 ----------" << endl;
        std::function<int(int)> f = [](int a) {return a; };
        printType<function_traits<std::function<int(int)>>::function_type>();
        //printType<all_args<std::function<int(int)>>>();
        printType<function_traits<std::function<int(int)>>::args<0>::type>();
        printType<function_traits<decltype(f)>::function_type>();
        printType<function_traits<decltype(free_function)>::function_type>();
        printType<function_traits<decltype(cast_func)>::function_type>();
        printType<function_traits<AA>::function_type>();
        using T = decltype(&AA::f);

        printType<function_traits<decltype(&AA::f)>::function_type>();
        static_assert(std::is_same<function_traits<decltype(f)>::return_type, int>::value, "");
    }
}
#endif //MYRPC_UTIL_H
