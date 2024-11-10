import os

from src.translator import Translator


def test_folder(asm_path: str, hack_path: str):
    for file in os.listdir(asm_path):
        filebase = os.path.splitext(file)[0]
        test_file(os.path.join(asm_path, filebase) + '.asm', os.path.join(hack_path, filebase) + '.hack')


def test_file(asm_path: str, hack_path: str):
    print('Testing', asm_path, '->', hack_path)
    with open(hack_path, 'r') as file:
        expected = list(file.read().split('\n'))

    result = Translator(asm_path).instructions

    for index, (result_line, expected_line) in enumerate(zip(result, expected)):
        assert result_line == expected_line, f'{result_line} != {expected_line} in line {index}'


if __name__ == '__main__':
    test_folder('asm', 'hack')
    # test_file('asm/Add.asm', 'hack/Add.hack')
