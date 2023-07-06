from sys import argv
import subprocess
import shellcode

def run_exe_with_initial_input(input_file, exe_file):
	# Run the executable on the input file
	process = subprocess.Popen([exe_file], stdin=subprocess.PIPE, shell=True)

	filein = open(input_file, 'rb').read()
	file_input_lines = [s for s in filein.split(b'\n')]
	with open("run_on_input_log.txt", 'w') as f:
		while True:
			next_input = input().encode('ascii') if len(file_input_lines) == 0 else file_input_lines.pop(0)
			# next_input = input()
			next_input = bytes(next_input)+b"\n"
			# Pass the user input to the executable
			print(f"next_input: {next_input}", file=f)
			process.stdin.write(next_input)
			process.stdin.flush()

if __name__ == "__main__":
	"""
	runs the input exe file on the input file until the input file ends,
		then gets input from user and passes it to the exe.
	"""
	#check if different filenames given as cmd arg:
	exec_filename = "hw4_client.exe"
	input_filename = "shellcode.bin"
	for arg in argv[1:]:
		if arg.endswith(".bin"):
			input_filename = arg
		elif arg.endswith(".exe"):
			exec_filename = arg
	shellcode.hw4_p3_filegen(input_filename, "shellcode.S")
	
	run_exe_with_initial_input(input_filename, exec_filename)