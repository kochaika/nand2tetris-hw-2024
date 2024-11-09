package main

import (
	"bufio"
	"os"
	"testing"
)

func filesAreEqual(filePath1, filePath2 string) (bool, error) {
	file1, err := os.Open(filePath1)
	if err != nil {
		return false, err
	}
	defer file1.Close()

	file2, err := os.Open(filePath2)
	if err != nil {
		return false, err
	}
	defer file2.Close()

	scanner1 := bufio.NewScanner(file1)
	scanner2 := bufio.NewScanner(file2)

	for scanner1.Scan() {
		if !scanner2.Scan() {
			return false, nil
		}
		if scanner1.Text() != scanner2.Text() {
			return false, nil
		}
	}

	if scanner2.Scan() {
		return false, nil
	}

	if err := scanner1.Err(); err != nil {
		return false, err
	}
	if err := scanner2.Err(); err != nil {
		return false, err
	}

	return true, nil
}

func TestMainProgram(t *testing.T) {
	tests := []struct {
		filePathInput  string
		filePathOutput string
		expectedOutput string
	}{
		{"06/Add.asm", "06/Add_actual.hack", "06/Add_expected.hack"},
		{"06/MaxL.asm", "06/MaxL_actual.hack", "06/MaxL_expected.hack"},
		{"06/RectL.asm", "06/RectL_actual.hack", "06/RectL_expected.hack"},
		{"06/PongL.asm", "06/PongL_actual.hack", "06/PongL_expected.hack"},
		{"06/Max.asm", "06/Max_actual.hack", "06/Max_expected.hack"},
		{"06/Rect.asm", "06/Rect_actual.hack", "06/Rect_expected.hack"},
		{"06/Pong.asm", "06/Pong_actual.hack", "06/Pong_expected.hack"},
	}

	for _, test := range tests {
		t.Run(test.filePathInput, func(t *testing.T) {
			os.Args = []string{"main", test.filePathInput, test.filePathOutput}
			main()

			areEqual, err := filesAreEqual(test.filePathOutput, test.expectedOutput)
			if err != nil {
				t.Fatalf("Error comparing files: %v", err)
			}
			if !areEqual {
				t.Errorf("Output file %s does not match expected file %s", test.filePathOutput, test.expectedOutput)
			}
		})
	}
}
