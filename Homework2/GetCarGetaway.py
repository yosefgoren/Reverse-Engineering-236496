from itertools import permutations
import subprocess
import os

def read_file_lines(file_path):
    lines = []
    try:
        with open(file_path, 'r') as file:
            lines = file.readlines()
    except FileNotFoundError:
        print(f"File '{file_path}' not found.")
    return lines


def count_file_lines(file_path):
    line_count = 0
    try:
        with open(file_path, 'r') as file:
            for _ in file:
                line_count += 1
    except FileNotFoundError:
        print(f"File '{file_path}' not found.")
    return line_count


def delete_file(file_path):
    try:
        os.remove(file_path)
        print(f"File '{file_path}' deleted successfully.")
    except FileNotFoundError:
        print(f"File '{file_path}' not found.")
    except PermissionError:
        print(f"Permission denied. Unable to delete '{file_path}'.")
    except OSError as e:
        print(f"Error occurred while deleting file '{file_path}': {e}")


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    in_file_path = "input.txt"
    perm_file_path = "perm.txt"
    exe_path = "sheep.exe"
    lines = read_file_lines(in_file_path)
    perms = permutations(lines)
    for i, perm in enumerate(perms):
        if i == 8 :
            print(perm)
        if perm[0] != "XR1\n" or perm[-1] != "XL4\n":
            continue
        with open(perm_file_path, 'w') as file:
            file.write('\n'.join(perm))
        try:
            output_file = "out_" + str(i) + ".txt"
            # Run the executable with input redirection
            subprocess.run([exe_path], stdin=open(perm_file_path, 'r'), stdout=open(output_file, 'w'))
            if count_file_lines(output_file) <= 1:
                delete_file(output_file)
            print(f"Execution completed. Output from {perm} saved to {output_file}")
        except FileNotFoundError:
            continue

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
