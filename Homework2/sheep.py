steps = ["XR1" ,"ED3" ,"DL1" ,"FD1" ,"RR3" ,"QU1" ,"PU2" ,"OU2" ,"XL4"]
import itertools
import os
options = list(itertools.permutations(steps))
f = open("sheep.txt", "w")

os.system("sheep.exe")