// src/trapecio.cpp
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <vector>
#include <thread>
#include <numeric>

// Definicion de la función a integrar.
long double f(long double x)
{
    // Ecuacion: 2x^2 + 3x + 0.5
    return 2 * std::pow(x, 2) + 3 * x + 0.5;
}

// Funcion para hilos
void calcularSumaParcial(
    long double a, long double h,
    long long inicio, long long fin,
    long double &resultado)
{
    long double suma_local = 0.0L;
    for (long long i = inicio; i < fin; ++i)
    {
        suma_local += f(a + i * h);
    }
    resultado = suma_local;
}

long double calcularIntegralParalelo(long double a, long double b, long long n, int num_hilos)
{
    if (n == 0)
        return 0;

    long double h = (b - a) / n;
    long double area = (f(a) + f(b)) / 2.0L;

    std::vector<std::thread> hilos;
    std::vector<long double> resultados_parciales(num_hilos);

    long long trapecios_por_hilo = (n - 1) / num_hilos;

    for (int i = 0; i < num_hilos; ++i)
    {
        long long inicio = 1 + i * trapecios_por_hilo;
        long long fin = (i == num_hilos - 1) ? n : inicio + trapecios_por_hilo;

        hilos.emplace_back(
            calcularSumaParcial, a, h, inicio, fin, std::ref(resultados_parciales[i]));
        // std::ref es necesario porque std::thread por defecto copia todos los argumentos.
        // Si no se usa el hilo recibe una copia del valor y cualquier modificación no afecta al original.
    }

    // Esperar a los hilos
    for (auto &hilo : hilos)
    {
        hilo.join();
    }

    // Sumar los resultados parciales al area total
    long double suma_total_parcial = 0.0L;
    for (const auto &res : resultados_parciales)
    {
        suma_total_parcial += res;
    }

    area += suma_total_parcial;

    return area * h;
}

/*
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
*/

int main()
{
    // Condiciones iniciales.
    const long double a = 2.0L;
    const long double b = 20.0L;

    // Tolerancia para la convergencia
    const long double epsilon = 1e-12L;

    const int num_hilos = std::thread::hardware_concurrency();
    std::cout << "Usando " << num_hilos << " hilos." << std::endl;

    long long n = 1; // Solo 1 trapecio inicialmente.
    long double resultadoAnterior;
    long double resultadoActual = calcularIntegralParalelo(a, b, n, num_hilos);

    std::cout << "Iniciando cálculo de la integral..." << std::endl;
    // Para mejorar la presentación de los números decimales.
    std::cout << std::fixed << std::setprecision(12);

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
        resultadoActual = calcularIntegralParalelo(a, b, n, num_hilos);

        std::cout << "Trapecios: " << std::setw(12) << std::left << n
                  << "| Area: " << resultadoActual << std::endl;

        // La nueva condicion comprueba que la diferencia absoluta es mayor que la tolerancia
    } while (std::abs(resultadoActual - resultadoAnterior) > epsilon);

    std::cout << "\nConvergencia alcanzada." << std::endl;
    std::cout << "El valor aproximado de la integral es: " << resultadoActual << std::endl;
    std::cout << "Número de trapecios utilizado: " << n << std::endl;

    return 0;
}