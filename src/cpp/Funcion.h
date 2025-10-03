#ifndef FUNCION_H
#define FUNCION_H

#include <cmath>

// Definicion de la función a integrar.
long double f(long double x)
{
    // Ecuacion: 2x^2 + 3x + 0.5
    return 2 * std::pow(x, 2) + 3 * x + 0.5;
}

#endif