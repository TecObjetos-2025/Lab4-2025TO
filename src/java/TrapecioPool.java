import java.math.BigDecimal;
import java.math.MathContext;
import java.math.RoundingMode;

public class TrapecioPool {
    private static final MathContext MC = new MathContext(100);
    
    public static void main(String[] args) {
        final BigDecimal a = new BigDecimal("2.0");
        final BigDecimal b = new BigDecimal("20.0");
        final BigDecimal epsilon = new BigDecimal("1e-40"); // Tolerancia más estricta
        
        final int numHilos = Runtime.getRuntime().availableProcessors();
        ThreadPool pool = new ThreadPool(numHilos);
        
        System.out.println("=== MÉTODO DEL TRAPECIO - POOL DE THREADS ===");
        System.out.println("Usando pool de " + numHilos + " hilos.");
        System.out.println("Función: f(x) = 2x² + 3x + 0.5");
        System.out.println("Límites: [" + a + ", " + b + "]");
        System.out.println("Tolerancia: " + epsilon);
        System.out.println("\nIniciando cálculo...");
        System.out.println("Trapecios         | Área Aproximada");
        System.out.println("------------------|----------------------------------------");
        
        long n = 1;
        BigDecimal resultadoAnterior = BigDecimal.ZERO;
        BigDecimal resultadoActual;
        int iteracion = 0;
        final int MAX_ITERACIONES = 40; // Más iteraciones
        int iteracionesSinCambio = 0;
        final int MAX_ITERACIONES_SIN_CAMBIO = 3;
        
        try {
            // Primera iteración
            CalculadorIntegralPool calculador = new CalculadorIntegralPool(a, b, n, pool);
            resultadoActual = calculador.calcular();
            System.out.printf("%-16d | %s%n", n, formatDecimal(resultadoActual));
            n *= 2;
            iteracion++;
            
            do {
                resultadoAnterior = resultadoActual;
                
                calculador = new CalculadorIntegralPool(a, b, n, pool);
                resultadoActual = calculador.calcular();
                
                System.out.printf("%-16d | %s%n", n, formatDecimal(resultadoActual));
                
                // Calcular diferencia
                BigDecimal diferencia = resultadoActual.subtract(resultadoAnterior, MC).abs();
                
                iteracion++;
                
                // Verificar si hay cambio significativo
                if (diferencia.compareTo(epsilon) <= 0) {
                    iteracionesSinCambio++;
                    if (iteracionesSinCambio >= MAX_ITERACIONES_SIN_CAMBIO) {
                        System.out.println("\n Convergencia completa alcanzada (sin cambios significativos).");
                        break;
                    }
                } else {
                    iteracionesSinCambio = 0; // Reset counter
                }
                
                if (iteracion >= MAX_ITERACIONES) {
                    System.out.println("\nLímite máximo de iteraciones alcanzado.");
                    break;
                }
                
                n *= 2;
                
                // Protección por rendimiento
                if (n > 100_000_000L) {
                    System.out.println("\n Límite de trapecios alcanzado por rendimiento.");
                    break;
                }
                
            } while (true);
            
            System.out.println("\n--- RESULTADO FINAL ---");
            System.out.println("Valor de la integral: " + formatDecimal(resultadoActual));
            System.out.println("Trapecios utilizados: " + n);
            System.out.println("Iteraciones totales: " + iteracion);
            
        } finally {
            pool.shutdown();
        }
    }
    
    private static String formatDecimal(BigDecimal value) {
        String plain = value.toPlainString();
        
        // Asegurar que siempre muestre decimales
        if (!plain.contains(".")) {
            plain = plain + ".0";
        }
        
        // Limitar a 60 decimales máximo para mejor visualización
        if (plain.contains(".")) {
            String[] parts = plain.split("\\.");
            if (parts.length == 2 && parts[1].length() > 60) {
                plain = parts[0] + "." + parts[1].substring(0, 60);
            }
        }
        
        return plain;
    }
}