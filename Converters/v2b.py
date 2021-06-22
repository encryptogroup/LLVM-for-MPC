#!/usr/bin/python3

# Author: Mark Will 
# Copyright (c) 2020 Acronis Asia R&D Pte Ltd 
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import argparse
import logging
import re
import subprocess
from functools import reduce

# Handle arguments
parser = argparse.ArgumentParser()
parser.add_argument(
    "input_file",
    help = "File to be converted", 
    type = str
)
parser.add_argument(
    "output_file", 
    help = "Bristol output file", 
    type = str
)
parser.add_argument(
    "-t", 
    "--top_module", 
    help = "Top module of the design", 
    type = str
)
parser.add_argument(
    "-a", 
    "--add_file", 
    help = "Add more files to the design", 
    action = 'append', 
    type = str
)
parser.add_argument(
    "-l", 
    "--lang", 
    help = """ 
        Input langauge. Note vhdl requires Yosys to be built with Verific support, 
        probably better to convert to Verilog for this converter
    """, 
    type = str, 
    choices = ["verilog", "vhdl"], 
    default = "verilog"
)
parser.add_argument(
    "-v", 
    "--verbose", 
    action = "store_true"
) 
args = parser.parse_args()

if args.verbose:
    logging.basicConfig(level=logging.INFO)
else:
    logging.basicConfig(level=logging.ERROR)

if args.top_module == None:
    tm = re.findall(r"([^/.]+)\.[^/]*$", args.input_file)
    if len(tm) != 1:
        tm = ['']
    args.top_module = tm[0]
    logging.info("Missing top module, assuming input filename: %s" % args.top_module)


# Convert yosys verilog output to Bristol format
# Note only one module should be here.
wires = []
inputs = []
outputs = []
assigns = []
logging.info("Loading design from Yosys")
filename = args.input_file
gates_one_line = ""
with open(filename, 'r') as f:
    modules = 0
    while True:
        line = f.readline()
        if not line:
            break
        line = line.strip()
        if line.startswith('wire'):
            wires.append(line[5:-1])
        elif line.startswith('input'):
            inputs.append(line[6:-1])
        elif line.startswith('output'):
            outputs.append(line[7:-1])
        elif line.startswith('assign'):
            assigns.append(line[7:-1])
        elif line.startswith('module'):
            modules += 1
        elif len(re.findall(r"^(/\*.*)", line)) != 1 and len(re.findall(r"(module|endmodule)", line)) != 1 and len(line.strip()) != 0:
            gates_one_line = gates_one_line + line.strip('\n')
    assert modules == 1
    assert len(assigns) == 0



num_input_wires = 0
num_output_wires = 0
wire_mapping = {}
input_wire = 0
prop_lens = {}

# Inputs are first wires
logging.info("Processing input wires")

def getIOProp(line):
    line = line.strip()
    arr = re.findall(r"\[([0-9]+)\:([0-9]+)\]", line)
    c = 1
    if arr:
        c += (reduce(lambda a,b: a + int(b), [0] + list(arr[0])))
    name = re.findall(r"[^ ]*$", line)[0]
    return (name, c)


for ip in inputs:
    name, l = getIOProp(ip)
    num_input_wires += l
    prop_lens[name] = l
    if l == 1:
        wire_mapping[name] = input_wire
        input_wire += 1
    else:
        for i in range(l):
            wire_mapping["%s[%s]" % (name, i)] = input_wire
            input_wire += 1

# Fix wires to offset with input_wires
# Sort, in string mode should be fine
logging.info("Processing internal wires")
wires.sort()
num_wires = 0
for i in range(len(wires)):
    name, l = getIOProp(wires[i])
    prop_lens[name] = l
    if l == 1:
        wire_mapping[name] = num_wires + num_input_wires
        num_wires += 1
    else:
        for j in range(l):
            wire_mapping["%s[%s]" % (name, j)] = num_wires + num_input_wires
            num_wires += 1

# Outputs are last wires, note gnd and high require a wire
logging.info("Processing output wires")
output_wire = num_input_wires + num_wires
output_wire_names = []
for op in outputs:
    name, l = getIOProp(op)
    num_output_wires += l
    prop_lens[name] = l
    if l == 1:
        wire_mapping[name] = output_wire
        output_wire_names.append(name)
        output_wire += 1
    else:
        for i in range(l):
            wire_mapping["%s[%s]" % (name, i)] = output_wire
            output_wire_names.append("%s[%s]" % (name, i))
            output_wire += 1

total_num_wires = sum([num_input_wires, num_output_wires, num_wires])

gates = gates_one_line.split(';')

# Process assignments
logging.info("Processing assigments")
class Instruction(object):
    def __init__(self, inputs, output, cmd):
        self.inputs = list(map(lambda a: a.strip(), inputs))
        # Only a single output is supported
        self.output = output.strip()
        self.cmd = cmd
    def __str__(self):
        io = ' '.join(list(map(lambda a: str(wire_mapping[a]), self.inputs + [self.output])))
        return "%d 1 %s %s" % (len(self.inputs), io, self.cmd) 

code = []
try:
    for gate in gates:
        if len(re.findall(r"(AND|ANDN|IV|INV|XOR|XNOR|NAND|NANDN|NOR|OR)", gate)) != 1 and len(gate.strip()) > 0:
                logging.error("Could not parse this gate:\n" + gate)
        # AND
        is_and = re.findall(r"(AND)", gate)
        if len(is_and) > 0:
            operation_side = re.findall(r".*\(.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\).*\)", gate)
            code.append(Instruction([operation_side[0][0], operation_side[0][1]], operation_side[0][2], "AND"))
            continue
        # ANDN
        is_andn = re.findall(r"(ANDN)", gate)
        if len(is_andn) > 0:
            operation_side = re.findall(r".*\(.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\).*\)", gate)
            code.append(Instruction([operation_side[0][0], operation_side[0][1]], operation_side[0][2], "ANDN"))
            continue
        # NOT (INV)
        is_inv = re.findall(r"(IV|INV)", gate)
        if len(is_inv) > 0:
            operation_side = re.findall(r"\(.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\).*\)", gate)
            code.append(Instruction([operation_side[0][0]], operation_side[0][1], "INV"))
            continue
        # XOR
        is_xor = re.findall(r"(XOR)", gate)
        if len(is_xor) > 0:
            operation_side = re.findall(r"\(.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\).*\)", gate)
            code.append(Instruction([operation_side[0][0], operation_side[0][1]], operation_side[0][2], "XOR"))
            continue
        # XNOR
        is_xnor = re.findall(r"(XNOR)", gate)
        if len(is_xnor) > 0:
            operation_side = re.findall(r"\(.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\).*\)", gate)
            code.append(Instruction([operation_side[0][0], operation_side[0][1]], operation_side[0][2], "XNOR"))
            continue
        # NAND
        is_nand = re.findall(r"(NAND)", gate)
        if len(is_nand) > 0:
            operation_side = re.findall(r"\(.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\).*\)", gate)
            code.append(Instruction([operation_side[0][0], operation_side[0][1]], operation_side[0][2], "NAND"))
            continue
        # NAND
        is_nandn = re.findall(r"(NANDN)", gate)
        if len(is_nandn) > 0:
            operation_side = re.findall(r"\(.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\).*\)", gate)
            code.append(Instruction([operation_side[0][0], operation_side[0][1]], operation_side[0][2], "NANDN"))
            continue
        # NOR
        is_nor = re.findall(r"(NOR)", gate)
        if len(is_nor) > 0:
            operation_side = re.findall(r"\(.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\).*\)", gate)
            code.append(Instruction([operation_side[0][0], operation_side[0][1]], operation_side[0][2], "NOR"))
            continue
        # OR
        is_or = re.findall(r"(OR)", gate)
        if len(is_or) > 0:
            operation_side = re.findall(r"\(.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\),.*\.[A-Z]\((.*)\).*\)", gate)
            code.append(Instruction([operation_side[0][0], operation_side[0][1]], operation_side[0][2], "OR"))
            continue

except Exception as e:
    logging.error(e)
    exit()

# Apply some quick optimisations
#- Remove output wires which are not used and not connected to an output port
while False:
    output_wires = set(map(lambda a: a.output, code))
    input_wires = []
    remove_wires = []
    remove_wires_mappings = []
    for instr in code:
        input_wires += instr.inputs
    input_wires = set(input_wires)
    for output_wire in output_wires:
        if output_wire not in input_wires and output_wire not in output_wire_names and wire_mapping[output_wire] >= num_input_wires:
            remove_wires.append(output_wire)
            remove_wires_mappings.append(wire_mapping[output_wire])
            del wire_mapping[output_wire]
    if len(remove_wires) == 0:
        break
    # Remove from code
    i = len(code) - 1
    while i >= 0:
        instr = code[i]
        if instr.output in remove_wires:
            del code[i]
        i -= 1
    total_num_wires -= len(remove_wires_mappings)
    # Adjust wire mappings
    for wire in wire_mapping:
        curr = wire_mapping[wire]
        if curr < num_input_wires:
            continue
        num = reduce(lambda c, a: c + int(a < curr), [0] + remove_wires_mappings)
        wire_mapping[wire] -= num

# Output the Bristol Format
logging.info("Outputting the Bristol formatted netlist to %s" % args.output_file)
with open(args.output_file, 'w') as f:
    f.write("%s %s\n" % (len(code), total_num_wires))
    f.write("%s %s\n" % (num_input_wires, num_output_wires))
    f.write("\n")
    for instr in code:
        f.write("%s\n" % str(instr))
    f.write("\n\n")
