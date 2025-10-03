#include "CalculadorIntegral.h"
#include "Funcion.h"
#include <thread>
#include <vector>
#include <numeric>

CalculadorIntegral::CalculadorIntegral(long double a, long double b, long long n, int num_hilos)
    : a(a), b(b), n(n), num_hilos(num_hilos)
{
    if (n > 0)
    {
        h = (b - a) / n;
    }
    else
    {
        h = 0;
    }
}

void CalculadorIntegral::worker(long long inicio, long long fin, long double &resultado)
{
    long double suma_local = 0.0L;
    for (long long i = inicio; i < fin; ++i)
    {
        suma_local += f(a + i * h);
    }
    resultado = suma_local;
}

long double CalculadorIntegral::calcular()
{
    if (n == 0)
        return 0;

    long double area = (f(a) + f(b)) / 2.0L;

    std::vector<std::thread> hilos;
    std::vector<long double> resultados_parciales(num_hilos);
    long long trapecios_por_hilo = (n - 1) / num_hilos;

    for (int i = 0; i < num_hilos; ++i)
    {
        long long inicio = 1 + i * trapecios_por_hilo;
        long long fin = (i == num_hilos - 1) ? n : inicio + trapecios_por_hilo;

        hilos.emplace_back(
            &CalculadorIntegral::worker, this, inicio, fin, std::ref(resultados_parciales[i]));
    }

    for (auto &hilo : hilos)
    {
        hilo.join();
    }

    area += std::accumulate(resultados_parciales.begin(), resultados_parciales.end(), 0.0L);

    return area * h;
}