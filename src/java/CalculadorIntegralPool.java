import java.math.BigDecimal;
import java.math.MathContext;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Future;

public class CalculadorIntegralPool {
    private final BigDecimal a, b, h;
    private final long n;
    private final ThreadPool pool;
    private static final MathContext MC = new MathContext(100); // Mayor precisión
    
    public CalculadorIntegralPool(BigDecimal a, BigDecimal b, long n, ThreadPool pool) {
        this.a = a;
        this.b = b;
        this.n = n;
        this.pool = pool;
        
        if (n > 0) {
            BigDecimal nBig = new BigDecimal(n);
            this.h = b.subtract(a, MC).divide(nBig, MC);
        } else {
            this.h = BigDecimal.ZERO;
        }
    }
    
    public BigDecimal calcular() {
        if (n == 0) return BigDecimal.ZERO;
        
        BigDecimal fa = Funcion.f(a);
        BigDecimal fb = Funcion.f(b);
        BigDecimal area = fa.add(fb, MC).divide(new BigDecimal("2"), MC);
        
        int numHilos = pool.getNumHilos();
        long trapezoidesPorHilo = (n - 1) / numHilos;
        List<Future<BigDecimal>> futures = new ArrayList<>();
        
        for (int i = 0; i < numHilos; i++) {
            final long inicio = 1 + i * trapezoidesPorHilo;
            final long fin = (i == numHilos - 1) ? n : inicio + trapezoidesPorHilo;
            
            futures.add(pool.enqueue(() -> {
                BigDecimal sumaLocal = BigDecimal.ZERO;
                for (long j = inicio; j < fin; j++) {
                    BigDecimal x = a.add(h.multiply(new BigDecimal(j), MC), MC);
                    sumaLocal = sumaLocal.add(Funcion.f(x), MC);
                }
                return sumaLocal;
            }));
        }
        
        // Recoger resultados
        for (Future<BigDecimal> future : futures) {
            try {
                area = area.add(future.get(), MC);
            } catch (Exception e) {
                throw new RuntimeException("Error en hilo del pool", e);
            }
        }
        
        return area.multiply(h, MC);
    }
}