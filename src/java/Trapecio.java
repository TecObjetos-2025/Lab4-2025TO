package java;

public class Trapecio {

    // Definición de la función a integrar
    public static double f(double x) {
        return 2 * Math.pow(x, 2) + 3 * x + 0.5;
    }

    // Método para calcular la integral de forma secuencial
    public static double calcularIntegralSecuencial(double a, double b, int n) {
        double h = (b - a) / n;
        double area = (f(a) + f(b)) / 2.0;

        // Suma las áreas de los trapecios intermedios
        for (int i = 1; i < n; ++i) {
            area += f(a + i * h);
        }

        return area * h;
    }

    public static void main(String[] args) {
        // Condiciones iniciales
        final double a = 2.0;
        final double b = 20.0;
        int n = 1; // Solo 1 trapecio inicialmente

        double resultadoAnterior = 0.0;
        double resultadoActual = calcularIntegralSecuencial(a, b, n);

        System.out.println("Iniciando cálculo de la integral...");

        // Bucle hasta que la integral converja
        while (resultadoAnterior != resultadoActual) {
            resultadoAnterior = resultadoActual;
            n *= 2; // Para el siguiente paso, duplicamos el número de trapecios
            resultadoActual = calcularIntegralSecuencial(a, b, n);

            // Usando printf para formatear la salida similar a C++
            System.out.printf("Trapecios: %-10d | Área: %.10f%n", n, resultadoActual);
        }

        System.out.println("\nConvergencia alcanzada.");
        System.out.printf("El valor aproximado de la integral es: %.10f%n", resultadoActual);
        System.out.println("Número de trapecios utilizado: " + n);

        // Cálculo analítico para comparación (opcional)
        double areaAnalitica = calcularAreaAnalitica(a, b);
        System.out.printf("Valor analítico exacto: %.10f%n", areaAnalitica);
        System.out.printf("Error: %.10f%n", Math.abs(resultadoActual - areaAnalitica));
    }

    // Método adicional para calcular el área analíticamente
    public static double calcularAreaAnalitica(double a, double b) {
        // Integral de 2x² + 3x + 0.5 es (2/3)x³ + (3/2)x² + 0.5x
        double integralA = (2.0 / 3.0) * Math.pow(a, 3) + (3.0 / 2.0) * Math.pow(a, 2) + 0.5 * a;
        double integralB = (2.0 / 3.0) * Math.pow(b, 3) + (3.0 / 2.0) * Math.pow(b, 2) + 0.5 * b;
        return integralB - integralA;
    }
}
t
