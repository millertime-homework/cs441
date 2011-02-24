#!/usr/bin/python

# Copyright 2011 Russell Miller
# HW3 - CS441 A.I.
# 2/23/2011 - 

import sys,re
from subprocess import Popen,PIPE

def main():
    """Handle argv"""
    if len(sys.argv) != 3:
        print("SYNTAX: python hw3.py inputfile path/to/picosat")
        exit()
    n,truths = getN(sys.argv[1])
    C,W,L,R = getArrs(n)
    rules = []
    rules += doRule12(L,R,n)
    rules += doRule34(L,R,n)
    rules += doRule56(C,W,L,R,n)
    rules += doRule78(C,W,L,R,n)
    rules += doCConstraint(truths,C,n)
    nvars = 4 * (n * n)
    nclauses = len(rules)
    finished_output = ["p cnf " + str(nvars) + " " + str(nclauses) + '\n'] + rules
    fn = sys.argv[1]
    outputfile = "pico" + fn[4:]
    f = open(outputfile, 'w')
    f.write('\n'.join(finished_output))
    f.close()
    pico_cmd = sys.argv[2] + " " + outputfile
    sat = Popen(pico_cmd, shell=True, stdout=PIPE)
    sat_output = sat.stdout.read()
    unsat = sat_output.find("UNSATISFIABLE")
    if unsat != -1:
        print("UNSATISFIABLE")
        exit()
    first_l = str((n * n * 2) + 1)
    start = sat_output.find(' ' + first_l + ' ')
    if start == -1:
        start = sat_output.find('-' + first_l + ' ')
    end = sat_output.find(' ' + str(nvars))
    if end == -1:
        end = sat_output.find('-' + str(nvars))
    final_filename = "soln" + fn[4:]
    makeOutputFile(sat_output[start:end+len(str(nvars))+1],final_filename,n)

def getN(filename):
    """Open the input file and get n and truth table"""
    f = open(filename)
    s = f.read()
    f.close()
    l = s.split('\n')
    # stupid empty lines!
    i = 0
    for line in l:
        if len(line) == 0:
            del l[i]
        i += 1
    n = len(l)
    # convert each line of input file to a list of chars
    truths = []
    for line in l:
        linelist = []
        for char in line:
            linelist.append(char)
        truths.append(linelist)
    return n,truths

def getArrs(n):
    """Build 4 2D arrays size nxn"""
    val = 1
    C = []
    for i in range(n):
        C.append([])
        for j in range(n):
            C[i].append(val)
            val += 1
    W = []
    for i in range(n):
        W.append([])
        for j in range(n):
            W[i].append(val)
            val += 1
    L = []
    for i in range(n):
        L.append([])
        for j in range(n):
            L[i].append(val)
            val += 1
    R = []
    for i in range(n):
        R.append([])
        for j in range(n):
            R[i].append(val)
            val += 1
    return C,W,L,R

def doRule12(L,R,n):
    """Do rule 1 and 2"""
    rules = []
    for i in range(n):
        s1 = ""
        s2 = ""
        for j in range(n):
            s1 += str(L[i][j]) + " "
            s2 += str(R[i][j]) + " "
        s1 += "0"
        s2 += "0"
        rules.append(s1)
        rules.append(s2)
    return rules

def doRule34(L,R,n):
    """Do rule 3 and 4"""
    rules = []
    for k in range(n):
        for i in range(n):
            for j in range(n):
                if (i != j):
                    rules.append(str(0-L[i][k]) + " " + str(0-L[j][k]) + " 0")
                    rules.append(str(0-R[i][k]) + " " + str(0-R[j][k]) + " 0")
    return rules

def doRule56(C,W,L,R,n):
    """Do rule 5 and 6"""
    rules = []
    for h in range(n):
        for i in range(n):
            for j in range(n):
                for k in range(n):
                    rule = str(0-L[h][j]) + " " + str(0-R[i][k]) + " "
                    rule += str(0-C[h][i]) + " " + str(W[j][k]) + " 0"
                    rules.append(rule)
                    rule2 = str(0-L[h][j]) + " " + str(0-R[i][k]) + " "
                    rule2 += str(C[h][i]) + " " + str(0-W[j][k]) + " 0"
                    rules.append(rule2)
    return rules

def doRule78(C,W,L,R,n):
    """Do rule 7 and 8"""
    rules = []
    for i in range(n):
        for j in range(n):
            for k in range(n):
                for m in range(n):
                    if ((k < i) and (m > j)) or ((k > i) and (m < j)):
                        rules.append(str(0-W[i][j]) + " " + str(0-W[k][m]) + " 0")
    return rules

def doCConstraint(truths,C,n):
    """Encode the singleton connection constraints"""
    rules = []
    for i in range(n):
        for j in range(n):
            if truths[i][j] == 't':
                rules.append(str(C[i][j]) + " 0")
            elif truths[i][j] == 'f':
                rules.append(str(0-C[i][j]) + " 0")
            else:
                print("Error!")
                exit()
    return rules

def makeOutputFile(sat,filename,n):
    """Parse the output of the SAT Solver and create an output file"""
    clean = ''.join(sat.split('\n'))
    clean = clean.replace('v ',' ')
    v = map(int, clean.split(' '))
    l = v[:len(v)/2]
    r = v[len(v)/2:]
    lside = []
    for i in range(n):
        lside.append(0)
    lvals = []
    for num in l:
        if num > 0:
            lvals.append(num)
    for num in lvals:
        c = num - (n * n * 2) - 1
        lside[c%n] = (c/n) + 1
    rside = []
    for i in range(n):
        rside.append(0)
    rvals = []
    for num in r:
        if num > 0:
            rvals.append(num)
    for num in rvals:
        c = num - (n * n * 3) - 1
        rside[c%n] = (c/n) + 1
    s = ''
    for i in range(n):
        s += str(lside[i]) + ' ' + str(rside[i]) + '\n'
    f = open(filename, 'w')
    f.write(s)
    f.close()

if __name__=="__main__":
    main()
