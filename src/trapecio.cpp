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
    return 0;
}