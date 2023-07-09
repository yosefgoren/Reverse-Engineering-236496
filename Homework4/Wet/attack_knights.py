import subprocess

def server_shell():
	restart_flag = False
	
	commands = [
		"echo 'Fires: False' > config/attack.config",
		"echo 'Rivals: True' >> config/attack.config",
		"echo 'Knights Infected: False' >> config/attack.config",
		"echo 'Robber Hunted: False' >> config/attack.config",
		"cat config/attack.config",
	]

	with subprocess.Popen(["python", "run_on_input.py"], stdin=subprocess.PIPE, shell=True) as process:
		for orig_cmd in commands:
			cmd = ". > $null ; "+orig_cmd
			process.stdin.write(cmd.encode('ascii')+b"\n")
			process.stdin.flush()
	if restart_flag:
		server_shell()

if __name__ == "__main__":
	server_shell()