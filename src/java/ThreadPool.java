import java.util.concurrent.*;

public class ThreadPool {
    private final ExecutorService executor;
    private final int numHilos;
    
    public ThreadPool(int numHilos) {
        this.numHilos = numHilos;
        this.executor = Executors.newFixedThreadPool(numHilos);
    }
    
    public <T> Future<T> enqueue(Callable<T> task) {
        return executor.submit(task);
    }
    
    public int getNumHilos() {
        return numHilos;
    }
    
    public void shutdown() {
        executor.shutdown();
    }
}