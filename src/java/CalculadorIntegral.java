import java.math.BigDecimal;
import java.math.MathContext;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class CalculadorIntegral {
    private final BigDecimal a, b, h;
    private final long n;
    private final int numHilos;
    private static final MathContext MC = new MathContext(100); // Mayor precisión
    
    public CalculadorIntegral(BigDecimal a, BigDecimal b, long n, int numHilos) {
        this.a = a;
        this.b = b;
        this.n = n;
        this.numHilos = numHilos;
        
        if (n > 0) {
            BigDecimal nBig = new BigDecimal(n);
            this.h = b.subtract(a, MC).divide(nBig, MC);
        } else {
            this.h = BigDecimal.ZERO;
        }
    }
    
    private BigDecimal worker(long inicio, long fin) {
        BigDecimal sumaLocal = BigDecimal.ZERO;
        for (long i = inicio; i < fin; i++) {
            BigDecimal x = a.add(h.multiply(new BigDecimal(i), MC), MC);
            sumaLocal = sumaLocal.add(Funcion.f(x), MC);
        }
        return sumaLocal;
    }
    
    public BigDecimal calcular() {
        if (n == 0) return BigDecimal.ZERO;
        
        // área = (f(a) + f(b)) / 2
        BigDecimal fa = Funcion.f(a);
        BigDecimal fb = Funcion.f(b);
        BigDecimal area = fa.add(fb, MC).divide(new BigDecimal("2"), MC);
        
        ExecutorService executor = Executors.newFixedThreadPool(numHilos);
        List<Future<BigDecimal>> futures = new ArrayList<>();
        long trapezoidesPorHilo = (n - 1) / numHilos;
        
        try {
            for (int i = 0; i < numHilos; i++) {
                final long inicio = 1 + i * trapezoidesPorHilo;
                final long fin = (i == numHilos - 1) ? n : inicio + trapezoidesPorHilo;
                
                Future<BigDecimal> future = executor.submit(() -> worker(inicio, fin));
                futures.add(future);
            }
            
            // Recoger resultados
            for (Future<BigDecimal> future : futures) {
                try {
                    area = area.add(future.get(), MC);
                } catch (Exception e) {
                    throw new RuntimeException("Error en hilo", e);
                }
            }
        } finally {
            executor.shutdown();
        }
        
        return area.multiply(h, MC);
    }
}