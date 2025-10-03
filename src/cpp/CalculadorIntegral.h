#ifndef CALCULADORINTEGRAL_H
#define CALCULADORINTEGRAL_H

class CalculadorIntegral
{
private:
    int num_hilos;
    long long n;
    long double a, b, h;

    void worker(long long inicio, long long fin, long double &resultado);

public:
    CalculadorIntegral(long double a, long double b, long long n, int num_hilos);

    long double calcular();
};

#endif