package main

import (
	"fmt"
	"math"
	"runtime"
)

func f(x float64) float64 {
	return 2*math.Pow(x, 2) + 3*x + 0.5
}

// Crear paquetes de trabajo
type Job struct {
	inicio int
	fin    int
	a      float64
	h      float64
}

type Result struct {
	sumaParcial float64
}

// Similar a producir-consumir con un pool de trabajadores
func worker(id int, jobs <-chan Job, results chan<- Result) {
	for job := range jobs {
		sumaParcial := 0.0
		for i := job.inicio; i < job.fin; i++ {
			sumaParcial += f(job.a + float64(i)*job.h)
		}
		results <- Result{sumaParcial: sumaParcial}
	}
}

func calcularIntegralConPool(a, b float64, n, numWorkers int, jobs chan Job, results chan Result) float64 {
	if n <= 1 {
		if n == 1 {
			return (f(a) + f(b)) / 2.0 * (b - a)
		}
		return 0
	}

	h := (b - a) / float64(n) // Tamanio de paso
	area := (f(a) + f(b)) / 2.0

	rangoTotal := n - 1
	if rangoTotal <= 0 {
		return area * h
	}

	// LOGICA DE DIVISION CORREGIDA
	// Con redondeo hacia arriba
	// Garantiza que el tamaño del trozo sea suficiente para cubrir todo el rango.
	chunkSize := (rangoTotal + numWorkers - 1) / numWorkers
	numJobs := 0

	for i := 1; i < n; i += chunkSize {
		fin := i + chunkSize
		if fin > n {
			fin = n // Para que no se pase
		}

		jobs <- Job{inicio: i, fin: fin, a: a, h: h}
		numJobs++
	}

	// Recoger resultados
	sumaTotalParcial := 0.0
	for i := 0; i < numJobs; i++ {
		result := <-results
		sumaTotalParcial += result.sumaParcial
	}

	area += sumaTotalParcial
	return area * h
}

func main() {
	const a = 2.0
	const b = 20.0
	const epsilon = 1e-10

	// https://pkg.go.dev/runtime#NumCPU
	numWorkers := runtime.NumCPU()
	fmt.Printf("Usando un 'pool' de %d workers.\n", numWorkers)

	// Crear el pool
	jobs := make(chan Job, numWorkers)       // Buffer de tareas
	results := make(chan Result, numWorkers) // buffer de Resultados

	// Workers en segundo plano
	for w := 1; w <= numWorkers; w++ {
		go worker(w, jobs, results)
	}

	var n int64 = 1
	var resultadoAnterior, resultadoActual float64

	fmt.Println("Iniciando cálculo de la integral...")

	for {
		resultadoAnterior = resultadoActual

		// Usando pool por cada iteracion
		resultadoActual = calcularIntegralConPool(a, b, int(n), numWorkers, jobs, results)

		fmt.Printf("Trapecios: %-12d | Area: %.12f\n", n, resultadoActual)

		if math.Abs(resultadoActual-resultadoAnterior) < epsilon && n > 1 {
			break
		}

		if n > math.MaxInt32/2 { // Limite de seguridad para evitar overflow
			fmt.Println("Alerta: Número máximo de trapecios alcanzado sin convergencia.")
			break
		}
		n *= 2
	}

	close(jobs)

	fmt.Println("\nConvergencia alcanzada.")
	fmt.Printf("El valor aproximado de la integral es: %.12f\n", resultadoActual)
	fmt.Printf("Numero de trapecios utilizados: %d\n", n)
}
