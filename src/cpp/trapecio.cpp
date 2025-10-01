// src/trapecio.cpp
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

// Definicion de la función a integrar.
long double f(long double x)
{
    // Ecuacion: 2x^2 + 3x + 0.5
    return 2 * std::pow(x, 2) + 3 * x + 0.5;
}

// Método para calcular la integral de forma secuencial.
long double calcularIntegralSecuencial(long double a, long double b, long long n)
{
    if (n == 0)
        return 0;                // Evitar division por 0
    long double h = (b - a) / n; // Tamanio de paso
    long double area = (f(a) + f(b)) / 2.0;

    // Suma las áreas de los trapecios intermedios.
    for (long long i = 1; i < n; ++i)
    {
        area += f(a + i * h);
    }

    return area * h;
}

int main()
{
    // Condiciones iniciales.
    const long double a = 2.0;
    const long double b = 20.0;

    // Tolerancia para la convergencia
    const long double epsilon = 1e-10;

    long long n = 1; // Solo 1 trapecio inicialmente.
    long double resultadoAnterior;
    long double resultadoActual = calcularIntegralSecuencial(a, b, n);

    std::cout << "Iniciando cálculo de la integral..." << std::endl;
    // Para mejorar la presentación de los números decimales.
    std::cout << std::fixed << std::setprecision(10);

    do
    {
        // Limite de seguridad

        if (n > std::numeric_limits<long long>::max() / 2)
        {
            std::cout << "\nLimite de trapecios alcanzado. Deteniendo el calculo..." << std::endl;
            break;
        }

        resultadoAnterior = resultadoActual;
        n *= 2; // Duplicar numero de trapecios
        resultadoActual = calcularIntegralSecuencial(a, b, n);

        std::cout << "Trapecios: " << std::setw(12) << std::left << n
                  << "| Area: " << resultadoActual << std::endl;

        // La nueva condicion comprueba que la diferencia absoluta es mayor que la tolerancia
    } while (std::abs(resultadoActual - resultadoAnterior) > epsilon);

    std::cout << "\nConvergencia alcanzada." << std::endl;
    std::cout << "El valor aproximado de la integral es: " << resultadoActual << std::endl;
    std::cout << "Número de trapecios utilizado: " << n << std::endl;

    return 0;
}