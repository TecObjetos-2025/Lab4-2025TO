#ifndef CALCULADORINTEGRAL_H
#define CALCULADORINTEGRAL_H

class ThreadPool;

class CalculadorIntegralPool
{
private:
    ThreadPool &pool;
    long long n;
    long double a, b, h;

    // void worker(long long inicio, long long fin, long double &resultado);

public:
    CalculadorIntegralPool(long double a, long double b, long long n, ThreadPool &pool);

    long double calcular();
};

#endif