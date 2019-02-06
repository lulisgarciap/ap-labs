package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	//matrix:= make(data_type, len, capacity)   -> make allocates space in mem
	matrix:= make([][]uint8,dy)
	for i:= range matrix{ //recorre row por row (array of arrays)
		matrix[i]= make([]uint8,dx)
		for j:= range matrix[i]{
			matrix[i][j] = uint8(i*j)
		}
	} 
	return matrix
}

func main() {
	pic.Show(Pic)
}

