#include <iostream>

/*
    2^16 = 2^8 * 2^8
    2^8  = 2^4 * 2^4
    2^4  = 2^2 * 2^2
    2^2  = 2 * 2

    2^-5 = 2^-2 * 2^-2 * 2^-1 = 2^-2 * 2^-2 / 2
    2^-2 = 2^-1 * 2^-1
    2^-1 = 2^0 * 2^0 / 2 = 1 * 1 / 2
*/

template<int Exp>
struct Pow
{
    double operator()(double base)
    {
        Pow<Exp/2> p;

        double result = p(base) * p(base);

        if(Exp % 2 == 0)    return result;
        else if(Exp > 0)    return base * result;    
        else                return result / base;    
    }
};

template<>
struct Pow<1>
{
    double operator()(double base)
    {
        return base;
    }
};

template<>
struct Pow<0>
{
    double operator()(double base)
    {
        return 1.0;
    }
};

int main()
{
    Pow<5> p;

    std::cout << p(2) << "\n";

    return 0;
}