from subprocess import Popen, PIPE
from sys import argv
import numpy as np
import matplotlib.pyplot as plt

def getopts(argv):
    opts = {}
    while argv:
        if argv[0][0] == '-':
            opts[argv[0]] = argv[1]
        argv = argv[1:]
    return opts

myargs = getopts(argv)
args = ("./build/calibrate",
        "-w", myargs["-w"],
        "-h", myargs["-h"],
        "-n", myargs["-n"],
        "-s", myargs["-s"],
        "-d", myargs["-d"],
        "-i", myargs["-i"],
        "-o", myargs["-o"],
        "-e", myargs["-e"])

p = Popen(args, stdout=PIPE)
p.wait()
output = p.stdout.read().split('\n')
for line in output:
    if(line.startswith('Calibration error:')):
        print(line + " At n = " + myargs["-n"] + ", Plotting")
        error = float(line[19:])
        plt.plot([error], [int(myargs['-n'])], marker='o', markersize=10, color="red")
        plt.axis([0, 1, 0, 50])
        plt.show()
