// src/trapecio.cpp
#include <iostream>
#include <cmath>
#include <iomanip>

// Definicion de la función a integrar.
double f(double x)
{
    return 2 * std::pow(x, 2) + 3 * x + 0.5;
}

// Método para calcular la integral de forma secuencial.
double calcularIntegralSecuencial(double a, double b, int n)
{
    double h = (b - a) / n;
    double area = (f(a) + f(b)) / 2.0;

    // Suma las áreas de los trapecios intermedios.
    for (int i = 1; i < n; ++i)
    {
        area += f(a + i * h);
    }

    return area * h;
}

int main()
{
    // Condiciones iniciales.
    const double a = 2.0;
    const double b = 20.0;
    int n = 1; // Solo 1 trapecio inicialmente.

    double resultadoAnterior = 0.0;
    double resultadoActual = calcularIntegralSecuencial(a, b, n);

    std::cout << "Iniciando cálculo de la integral..." << std::endl;
    // Para mejorar la presentación de los números decimales.
    std::cout << std::fixed << std::setprecision(10);

    // Bucle hasta que la integral converja.
    while (resultadoAnterior != resultadoActual)
    {
        resultadoAnterior = resultadoActual;
        n *= 2; // para el siguiente paso, duplicamos el número de trapecios.
        resultadoActual = calcularIntegralSecuencial(a, b, n);

        // Usando setw y left para alinear la salida.
        std::cout << "Trapecios: " << std::setw(10) << std::left << n
                  << "| Área: " << resultadoActual << std::endl;
    }

    std::cout << "\nConvergencia alcanzada." << std::endl;
    std::cout << "El valor aproximado de la integral es: " << resultadoActual << std::endl;
    std::cout << "Número de trapecios utilizado: " << n << std::endl;

    return 0;
}