import subprocess

def server_shell():
	restart_flag = False
	with subprocess.Popen(["python", "run_on_input.py"], stdin=subprocess.PIPE, shell=True) as process:
		while True:
			usrin = input()
			if usrin == "exit":
				break
			if usrin == "restart":
				restart_flag = True
				break
			cmd = ". > $null ; "+usrin
			process.stdin.write(cmd.encode('ascii')+b"\n")
			process.stdin.flush()
	if restart_flag:
		server_shell()

if __name__ == "__main__":
	server_shell()