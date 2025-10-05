import java.math.BigDecimal;
import java.math.MathContext;

public class Funcion {
    private static final MathContext MC = new MathContext(100); // Mayor precisión
    
    public static BigDecimal f(BigDecimal x) {
        // f(x) = 2x² + 3x + 0.5
        BigDecimal xCuadrado = x.multiply(x, MC);
        BigDecimal termino1 = new BigDecimal("2").multiply(xCuadrado, MC);
        BigDecimal termino2 = new BigDecimal("3").multiply(x, MC);
        BigDecimal termino3 = new BigDecimal("0.5");
        
        return termino1.add(termino2, MC).add(termino3, MC);
    }
    
    public static MathContext getMathContext() {
        return MC;
    }
}