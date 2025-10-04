#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool
{
public:
    ThreadPool(size_t threads);

    ~ThreadPool();

    // Metodos para anadir tareas al pool
    template <class F, class... Args>
    auto enqueue(F &&f, Args &&...args)
        -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    // Estructura de datos para los hilos
    // Coleccion de hilos trabajadores
    std::vector<std::thread> workers;
    // Cola de Tareas
    std::queue<std::function<void()>> tasks;

    // Sincronizacion
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

/*
 * Implementacion de los metodos del ThreadPool
 * Fuente: https://www.geeksforgeeks.org/cpp/thread-pool-in-cpp/
 * 1. Empaqueta la funcion f y sus argumentos args en un objeto std::packaged_task
 * 2. Obtiene un std::future asociado a la tarea empaquetada para obtener el resultado asincronamente
 * 3. Protege el acceso a la cola de tareas con un mutex
 * 4. Verifica si el ThreadPool ha sido detenido, lanzando una excepcion si es asi
 * 5. Anade la tarea empaquetada a la cola de tareas usando una lambda para capturar la tarea por valor
 * 6. Notifica a un hilo trabajador para que la ejecute
 * 7. Devuelve el std::future para que el llamador pueda obtener el resultado
 */
template <class F, class... Args>
auto ThreadPool::enqueue(F &&f, Args &&...args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type();

    // Crear un paquete de tareas
    // std::forward<F>(f) y std::forward<Args>(args)... se usan para
    // preservar el tipo de valor (lvalue/rvalue) de los argumentos
    //
    // std::bind se usa para unir la funcion y sus argumentos en un solo objeto callable
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
        // Proteger acceso a la cola
        // Guia: https://www.geeksforgeeks.org/cpp/cpp-multithreading-condition-variables/
        std::unique_lock<std::mutex> lock(queue_mutex);

        if (stop)
            throw std::runtime_error("enqueue ha sido detenico ThreadPool");

        // Funcion lambda para capturar el task por valor
        tasks.emplace([task]()
                      { (*task)(); });
    }
    condition.notify_one();
    return res;
}

#endif