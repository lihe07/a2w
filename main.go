package main

import (
	"fmt"
	"os/exec"
  "os"
)

func ConvertAvifToWebp(fromPath, toPath string) error {
	cmd := exec.Command("./a2w", fromPath, toPath)

  cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr

	if err := cmd.Run(); err != nil {
		return fmt.Errorf("failed to convert %s to %s: %v", fromPath, toPath, err)
	}

	return nil
}


func main() {
  res := ConvertAvifToWebp("./hato.avif", "./hato.webp")

  if res != nil {
    fmt.Printf("%v\n", res);
  }
}
