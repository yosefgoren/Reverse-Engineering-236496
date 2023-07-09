import os
import pathlib

os.system("echo archer>__users_py_input.txt")
os.system("echo FKGXJP0OCE1LKT3D >> __users_py_input.txt")
os.system("echo USER >> __users_py_input.txt")

exe_name = "client.exe" if pathlib.Path('./client.exe').exists() else "hw4_client.exe"
os.system(f"{exe_name} < __users_py_input.txt")