import sys
import re


def translate(text, conversion_dict, before=None):
    """
    Translate words from a text using a conversion dictionary

    Arguments:
        text: the text to be translated
        conversion_dict: the conversion dictionary
        before: a function to transform the input
        (by default it will fall back to the identity function)
    """
    # if empty:
    if not text:
        return text
    # preliminary transformation:
    before = before or (lambda x: x)
    t = before(text)
    # print(sorted(conversion_dict.items(),key=lambda x: x[1],reverse=True))
    # print(conversion_dict.items())
    i = 0
    out = ""
    while i < len(text):
        if text[i].isnumeric():
            if (text[i:i + 4] == "2 1 ") or (text[i:i + 4] == "1 1 "):
                out = out + text[i:i + 3]
                i = i + 3
            elif text[i].isnumeric():
                j = 0
                while text[i + j].isnumeric():
                    j = j + 1
                out = out + str(conversion_dict.get(int(text[i:i + j])))
                i = i + j
        else:
            out = out + text[i]
            i = i + 1

    return out.split(",")


with open(sys.argv[1], 'r') as file:
    lines = file.readlines()

    result = [(re.match('define( internal)? i(?P<OWidth>\d+) @(?P<Name>.+)(?P<Args>\((i\d+.*\w+)+\)).*{',
                        line).groupdict(), i) for
              line, i in
              zip(lines, range(0, len(lines))) if
              re.match('define( internal)? i(?P<OWidth>\d+) @(?P<Name>.+)(?P<Args>\((i\d+.*\w+)+\)).*{',
                       line) is not None]

    for r in result:
        r[0]["Args"] = re.findall(r'(?<=i)\d+', r[0]["Args"])

    for res, pos in result:

        functionlength = lines[pos:].index('}\n')
        outlist = []
        resultbits = []
        part = 0
        print("Next")
        print(lines[pos])
        for line in lines[pos + 2:pos + functionlength]:
            # Dissassembly part
            if part == 0:
                #  %ShiftedTo0BitX = lshr exact i32 %x, 0
                instruction = re.match('\s+%(?P<Value>.+) = lshr exact i\d+ %?(?P<Op1>.+), (?P<Op2>.+)', line)
                if instruction is not None:
                    # print(instruction, "is lshr, in dissasembly part")
                    continue
                #  %0 = trunc i32 %ShiftedTo0BitX to i1
                instruction = re.match('\s+%(?P<Value>.+) = trunc i\d+ %(?P<Op1>.+) to i1', line)
                if instruction is not None:
                    # print(instruction, "is trunc, in dissasembly part")
                    continue

                print(line, "should be AND, XOR, OR")
                part = part + 1
            # gate part

            if part == 1:
                instruction = re.match('\s+%(?P<OutWire>\d+) = xor i1 %(?P<InWire1>\d+), %(?P<InWire2>\d+)', line)
                if instruction is not None:
                    # print("2 1 "+str(result.group("InWire1"))+" "+ str(result.group("InWire2"))+" "+ str(result.group("OutWire"))+" XOR")
                    outlist.append("2 1 " + str(instruction.group("InWire1")) + " " + str(
                        instruction.group("InWire2")) + " " + str(
                        instruction.group("OutWire")) + " XOR" + "\n")
                    continue
                instruction = re.match('\s+%(?P<OutWire>\d+) = or i1 %(?P<InWire1>\d+), %(?P<InWire2>\d+)', line)
                if instruction is not None:
                    # print("2 1 "+str(result.group("InWire1"))+" "+ str(result.group("InWire2"))+" "+ str(result.group("OutWire"))+" XOR")
                    outlist.append("2 1 " + str(instruction.group("InWire1")) + " " + str(
                        instruction.group("InWire2")) + " " + str(
                        instruction.group("OutWire")) + " OR" + "\n")
                    continue
                instruction = re.match('\s+%(?P<OutWire>\d+) = and i1 %(?P<InWire1>\d+), %(?P<InWire2>\d+)', line)
                if instruction is not None:
                    # print("2 1 " + str(result.group("InWire1")) + " " + str(result.group("InWire2")) + " " + str(result.group("OutWire")) + " AND")
                    outlist.append("2 1 " + str(instruction.group("InWire1")) + " " + str(
                        instruction.group("InWire2")) + " " + str(
                        instruction.group("OutWire")) + " AND" + "\n")
                    continue
                instruction = re.match('\s+%(?P<OutWire>\d+) = xor i1 %(?P<InWire1>\d+), true', line)
                if instruction is not None:
                    # print("1 1 " + str(result.group("InWire1")) + " " + str(result.group("OutWire")) + " INV")
                    outlist.append(
                        "1 1 " + str(instruction.group("InWire1")) + " " + str(
                            instruction.group("OutWire")) + " INV" + "\n")
                    continue
                instruction = re.match('\s+%(?P<OutWire>\d+) = xor i1 true, %(?P<InWire1>\d+)', line)
                if instruction is not None:
                    # print("1 1 " + str(result.group("InWire1")) + " " + str(result.group("OutWire")) + " INV")
                    outlist.append(
                        "1 1 " + str(instruction.group("InWire1")) + " " + str(
                            instruction.group("OutWire")) + " INV" + "\n")
                    continue
                print(line, "should be zeroextend")
                part = part + 1
            # reassembly part

            if part == 2:
                #  %219 = zext i1 %67 to i32
                instruction = re.match('\s+%(?P<Value>.+) = zext i1 %(?P<Op>\d+) to i32', line)
                if instruction is not None:
                    resultbits.append(instruction.group("Op"))
                    # print(instruction, " is zeroextend in reassembly")
                    continue

                #  %220 = shl i32 %219, 1
                instruction = re.match('\s+%(?P<Value>\d+) = shl i32 %(?P<Op>\d+), (?P<ShiftAmount>\d+)', line)
                if instruction is not None:
                    # print(instruction, " is shl in reasssembly")
                    continue

                #  %221 = xor i32 %218, %220
                instruction = re.match('\s+%(?P<Value>\d+) = xor i32 %(?P<Op1>\d+), %(?P<Op2>\d+)', line)
                if instruction is not None:
                    # print(instruction, " is xor in reasssembly")
                    continue

                print(line, "should be return, or i missed something")

        if line == "}\n":
            break

        resultbits = set(map(int, resultbits))

        print(resultbits)

        wirelist = (re.findall('[2|1] 1 (\d+ \d+ ?\d+?) ', ",".join(outlist)))
        wirelist = [w.split(" ") for w in wirelist]
        flatten = lambda l: [item for sublist in l for item in sublist]
        wirelistNoResults = list(set(map(int, list(set(flatten(wirelist))))).difference(resultbits))
        wirelist = list(set(map(int, list(set(flatten(wirelist))))))
        wire_dict = dict(list(zip(wirelistNoResults, range(0, int(len(wirelistNoResults))))))
        result_dict = dict(list(zip(sorted(resultbits), range(int(len(wirelistNoResults)), int(len(wirelist))))))
        final_dict = {**wire_dict, **result_dict}

        retval = translate(",".join(outlist), final_dict)

        if len(sys.argv) > 2 and sys.argv[2] == "fashion":
            retval = [str(len(outlist)) + " " + str(len(wirelist)) + "\n",
                      str(len(res["Args"])) + " " + " ".join(res["Args"]) + "\n",
                      "1 " + str(res["OWidth"]) + "\n", "\n"] + retval
        else:
            retval = [str(len(outlist)) + " " + str(len(wirelist)) + "\n",
                      " ".join(res["Args"]) + " " + str(res["OWidth"]) + "\n", "\n"] + retval

        with open(str(sys.argv[1])+res["Name"] + ".bristol", "w+") as output:
            [output.write(line) for line in retval]
