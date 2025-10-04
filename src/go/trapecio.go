package main

import (
	"fmt"
	"math" // Mathematical functions
	// System functions
)

// float64 similar a double en C++
func f(x float64) float64 {
	return 2*math.Pow(x, 2) + 3*x + 0.5
}

// Fuentes de información para Canales y Goroutines
// ref: https://www.developerro.com/2023/06/14/goroutines-channels/
func worker(id int, a, h float64, inicio, fin int, resultados chan<- float64) {
	sumaParcial := 0.0
	for i := inicio; i < fin; i++ {
		sumaParcial += f(a + float64(i)*h)
	}

	resultados <- sumaParcial
}

func calcularIntegralPorTrapecioParalelo(a, b float64, n, numGoroutines int) float64 {
	if n == 0 {
		return 0
	}

	h := (b - a) / float64(n)
	area := (f(a) + f(b)) / 2.0

	// Crear el canal para recibir resultados
	canalResultados := make(chan float64, numGoroutines)

	trapeciosPorGoroutine := (n - 1) / numGoroutines

	// Launchar goroutines
	for i := 0; i < numGoroutines; i++ {
		inicio := i*trapeciosPorGoroutine + 1
		fin := inicio + trapeciosPorGoroutine
		if i == numGoroutines-1 {
			fin = n // Asegurar que la última goroutine cubra hasta n-1
		}
		go worker(i, a, h, inicio, fin, canalResultados)
	}

	sumaTotalParcial := 0.0
	for i := 0; i < numGoroutines; i++ {
		// Go maneja el bloqueo hasta que recibe un valor
		sumaTotalParcial += <-canalResultados
	}

	area += sumaTotalParcial
	return area * h
}

func main() {
	const a = 2.0
	const b = 20.0
	const epsilon = 1e-12

	// https://pkg.go.dev/runtime#NumCPU
	numGoroutines := 4 // runtime.NumCPU()
	fmt.Printf("Usando %d goroutines.\n", numGoroutines)

	n := 1
	var resultadoAnterior, resultadoActual float64

	fmt.Println("Iniciando cálculo de la integral...")

	for {
		resultadoAnterior = resultadoActual
		resultadoActual = calcularIntegralPorTrapecioParalelo(a, b, n, numGoroutines)

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

	fmt.Println("\nConvergencia alcanzada.")
	fmt.Printf("El valor aproximado de la integral es: %.12f\n", resultadoActual)
	fmt.Printf("Numero de trapecios utilizados: %d\n", n)
}
