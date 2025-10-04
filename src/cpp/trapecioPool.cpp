// src/trapecio.cpp
#include <iostream>
#include <iomanip>
#include <limits>
#include <thread>
#include "CalculadorIntegralPool.h"
#include "ThreadPool.h"

int main()
{
    // Condiciones iniciales.
    const long double a = 2.0L;
    const long double b = 20.0L;

    // Tolerancia para la convergencia
    const long double epsilon = 1e-12L;

    const int num_hilos = std::thread::hardware_concurrency();

    ThreadPool pool(num_hilos);

    std::cout << "Usando pool de " << num_hilos << " hilos." << std::endl;

    long long n = 1; // Solo 1 trapecio inicialmente.
    long double resultadoAnterior = 0.0L;
    long double resultadoActual;

    std::cout << "Iniciando cálculo de la integral..." << std::endl;
    // Para mejorar la presentación de los números decimales.
    std::cout << std::fixed << std::setprecision(12);

    do
    {
        resultadoAnterior = resultadoActual;

        CalculadorIntegral calculador(a, b, n, pool);
        resultadoActual = calculador.calcular();

        std::cout << "Trapecios: " << std::setw(12) << std::left << n
                  << "| Área: " << resultadoActual << std::endl;

        // Limite de seguridad
        if (n > std::numeric_limits<long long>::max() / 2)
        {
            std::cout << "\nLimite de trapecios alcanzado. Deteniendo el calculo..." << std::endl;
            break;
        }
        n *= 2; // Duplicar numero de trapecios

        // La nueva condicion comprueba que la diferencia absoluta es mayor que la tolerancia
    } while (std::abs(resultadoActual - resultadoAnterior) > epsilon);

    std::cout << "\nConvergencia alcanzada." << std::endl;
    std::cout << "El valor aproximado de la integral es: " << resultadoActual << std::endl;
    std::cout << "Número de trapecios utilizado: " << n << std::endl;

    return 0;
}