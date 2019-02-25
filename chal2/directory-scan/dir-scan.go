package main
import "io/ioutil"
import "fmt"
import "os"
//   go build dir-scan.go
//   ./dir-scan <directorypath/>

func scanDir(dir string) error{
	var directory,other, den int
	files, _:= ioutil.ReadDir(dir)
	for _, file:= range files{
		fileos, _ := os.Lstat(dir+file.Name()) 
		if fileos.Mode()& os.ModeSymlink !=0 {
			den++
		}else if(file.IsDir()){
			directory++	
		}else{
			other++		
		}
	}
	fmt.Println("Path:",dir)
	//fmt.Println("Number of files:",len(files))
	fmt.Println("Directories: ",directory)
	fmt.Println("Symbolic Links:",den)
	fmt.Println("Other files: ",other)
	return nil
}
func main(){
	
	var path string
	path = os.Args[1]	
	scanDir(path)
	}
