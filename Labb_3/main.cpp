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
    double exp(double x)
    {
        Pow<Exp/2> p;

        double result = p.exp(x) * p.exp(x);

        if(Exp % 2 == 0)    return result;
        else if(Exp > 0)    return x * result;    
        else                return result / x;    
    }
};

template<>
struct Pow<1>
{
    double exp(double x)
    {
        return x;
    }
};

template<>
struct Pow<0>
{
    double exp(double x)
    {
        return 1.0;
    }
};

int main()
{
    Pow<-5> p;

    std::cout << p.exp(2);

    return 0;
}