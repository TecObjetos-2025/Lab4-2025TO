#include "CalculadorIntegralPool.h"
#include "ThreadPool.h"
#include "Funcion.h"
#include <thread>
#include <vector>
#include <numeric>

CalculadorIntegralPool::CalculadorIntegralPool(long double a, long double b, long long n, ThreadPool &pool)
    : a(a), b(b), n(n), pool(pool)
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

/**
void CalculadorIntegral::worker(long long inicio, long long fin, long double &resultado)
{
    long double suma_local = 0.0L;
    for (long long i = inicio; i < fin; ++i)
    {
        suma_local += f(a + i * h);
    }
    resultado = suma_local;
}
*/

long double CalculadorIntegralPool::calcular()
{
    if (n == 0)
        return 0;

    long double area = (f(a) + f(b)) / 2.0L;

    int num_hilos = pool.getNum_hilos();

    long long trapecios_por_hilo = (n - 1) / num_hilos;

    std::vector<std::future<long double>> resultados_futuros;

    for (int i = 0; i < num_hilos; ++i)
    {
        long long inicio = 1 + i * trapecios_por_hilo;
        long long fin = (i == num_hilos - 1) ? n : inicio + trapecios_por_hilo;

        // Envolver la tarea en una lambda para capturar los parámetros por valor
        // Nota: antes era la funcion worker, ahora es una lambda
        resultados_futuros.emplace_back(
            pool.enqueue([this, inicio, fin]
                         {
                long double suma_local = 0.0L;
                for (long long j = inicio; j < fin; ++j) 
                {
                    suma_local += f(a + j * h);
                }
            return suma_local; }));
    }

    long double suma_total_parcial = 0.0L;

    // Recoger los resultados de las tareas
    for (auto &futuro : resultados_futuros)
    {
        suma_total_parcial += futuro.get();
    }

    area += suma_total_parcial;

    return area * h;
}