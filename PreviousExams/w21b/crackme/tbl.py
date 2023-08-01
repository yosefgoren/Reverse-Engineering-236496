N_ROWS_LIKE_N_COLS = None

class Table:
	def __init__(self, n_cols:int=8, n_rows:int=N_ROWS_LIKE_N_COLS, default_value:str = '.'):
		if n_rows == N_ROWS_LIKE_N_COLS:
			n_rows = n_cols
		self.n_cols = n_cols
		self.n_rows = n_rows
		self.table = [[default_value for _ in range(n_cols)] for _ in range(n_rows)]
	
	def __str__(self) -> str:
		seperator_row = "+---"*self.n_cols+"+"+"\n"
		res = seperator_row
		for row in self.table:
			res += "|"
			for cell in row:
				res += " "+cell+" |"
			res += "\n"
			res += seperator_row
		return res

	def show(self):
		print(self)

t = Table(n_cols=5)
t.show()

"""
  0 1 2 3 4 5 6 7 8 9
0 X X X X X X X X X X
1 X   . . .         X
2 X                 X
3 X             X   X
4 X     X X X X X   X
5 E       X P X X   X
6 X       X   . X   X
7 X       X   . X   X
8 X . .   X         X
9 X X X X X X X X X X

D 1
R 1
D 2
R 2
U 7
L 7
D 7
R 1
U 3
L 2

"""

