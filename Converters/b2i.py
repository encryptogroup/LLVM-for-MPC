import sys
import re


# format
# fixme hardcoded two input one output circuits
# expand to variable input size circuits
# python3 b2i.py file [split]
# split: X,(\d+,)*Y(, \d+)*

def getXYSplit(lis):
    splitpoint = lis.index("Y")
    XSplit = [int(x) for x in lis[1:splitpoint]]
    YSplit = [int(y) for y in lis[splitpoint + 1:]]
    return [XSplit, YSplit]


with open(sys.argv[1], 'r') as file:
    output = open(sys.argv[1][:-8] + "_conv" + sys.argv[1][-8:], "w+")

    lines = file.readlines()
    result = re.match('(?P<Gates>\d+)\s+(?P<Wires>\d+)', lines[0])
    Gates = result.group("Gates")
    Wires = result.group("Wires")
    output.write("//Gates: " + Gates + "\n")
    output.write("//Wires: " + Wires + "\n")
    result = re.match('(?P<XWidth>\d+)\s+(?P<YWidth>\d+)\s+(?P<ZWidth>\d+)', lines[1])
    XWidth = result.group("XWidth")
    output.write("//Input Width X " + XWidth + "\n")
    YWidth = result.group("YWidth")
    output.write("//Input Width Y " + YWidth + "\n")
    ZWidth = result.group("ZWidth")
    output.write("//Output Width Z " + ZWidth + "\n")

    split = []
    if len(sys.argv) > 2:
        output.write("//Have Split " + sys.argv[2] + "\n")
        split = getXYSplit(sys.argv[2].split(","))
        if sum(split[0]) != int(XWidth) or sum(split[1]) != int(YWidth):
            print("No viable Split for inputs")
            exit(2)
    else:
        split = [[int(XWidth)], [int(YWidth)]]

    output.write("//CreateFunction\n")
    output.write("ArrayRef<Type *> args = {")
    [output.write("Type::getInt" + str(x) + "Ty(F->getContext()),") for x in split[0]]
    [output.write("Type::getInt" + str(y) + "Ty(F->getContext()),") for y in split[1][0:-1]]
    output.write("Type::getInt" + str(split[1][-1]) + "Ty(F->getContext())};\n")
    output.write("std::string FunctionName = \"Extern_" + sys.argv[1].split("/")[-1][0:-8] + "\";\n")
    output.write("Constant *c = M.getOrInsertFunction(FunctionName,FunctionType::get(llvm::IntegerType::getInt"+str(ZWidth)+"Ty(F->getContext()),args, false));\n")


    output.write("auto func = cast<Function>(c);\n")
    output.write("func->addFnAttr(\"Gate\");\n")
    output.write("if (c->hasNUsesOrMore(1)) {\n")
    output.write("} else {\n")
    output.write("Function::arg_iterator argNames = func->arg_begin();\n")
    output.write("Value *x = argNames++;\n")
    output.write("x->setName(\"x\");\n")
    output.write("Value *y = argNames++;\n")
    output.write("y->setName(\"y\");\n")

    output.write("BasicBlock *block = BasicBlock::Create(F->getContext(), \"entry\", func);\n")
    output.write("IRBuilder<> builder(block);\n")


    output.write("//Begin disassembling\n")
    counter = 0
    for x in split[0]:
        print("Value* XInput" + str(counter) + "to" + str(counter + int(x) - 1) + "bits[" + str(x) + "];\n")
        output.write("Value* XInput" + str(counter) + "to" + str(counter + int(x) - 1) + "bits[" + str(x) + "];\n")

        output.write("for (int j=0; j<" + str(x) + ";j++){\n")
        output.write("\tXInput" + str(counter) + "to" + str(
            counter + int(
                x) - 1) + "bits[j] = builder.CreateLShr(x, u_int64_t(j), \"ShiftedTo\"+std::to_string(j) + \"BitX\", true);\n")
        output.write(
            "\tXInput" + str(counter) + "to" + str(
                counter + int(x) - 1) + "bits[j] = builder.CreateTruncOrBitCast(XInput" + str(counter) + "to" + str(counter + int(x) - 1) + "bits[j],Type::getInt1Ty(F->getContext()));\n")
        output.write("}\n")
        counter = counter + int(x)

    counter = 0
    for y in split[1]:
        print("Value* YInput" + str(counter) + "to" + str(counter + int(y) - 1) + "bits[" + str(y) + "];\n")
        output.write("Value* YInput" + str(counter) + "to" + str(counter + int(y) - 1) + "bits[" + str(y) + "];\n")
        output.write("for (int j=0; j<" + str(y) + ";j++){\n")
        output.write("\tYInput" + str(counter) + "to" + str(
            counter + int(
                y) - 1) + "bits[j] = builder.CreateLShr(y, u_int64_t(j), \"ShiftedTo\"+std::to_string(j) + \"BitY\", true);\n")
        output.write(
            "\tYInput" + str(counter) + "to" + str(
                counter + int(y) - 1) + "bits[j] = builder.CreateTruncOrBitCast(YInput" + str(counter) + "to" + str(
                counter + int(y) - 1) + "bits[j],Type::getInt1Ty(F->getContext()));\n")
        output.write("}\n")
        counter = counter + int(y)

    output.write("//Inline gates\n")
    outstring = ""
    for line in lines[3:]:
        result = re.match(
            '(?P<NumberInWires>\d+)\s(?P<NumberOutWires>\d+)\s((?P<InWireId1>\d+)\s((?P<InWireId2>\d+)\s)?(?P<OutWireId>\d+)\s(?P<Function>[A-Z]+))',
            line)
        if (result.group("InWireId2")):  # If second wire exists
            outstring += "Value* W_" + result.group("OutWireId") + " = builder.Create" + result.group(
                "Function").lower().capitalize() + \
                         "(W_" + result.group("InWireId1") + ", W_" + result.group("InWireId2") + ")\n"
        else:  # if only one wire exists inv
            outstring += "Value* W_" + result.group("OutWireId") + " = builder.Create" + result.group(
                "Function").lower().capitalize() + \
                         "(W_" + result.group("InWireId1") + ")\n"

    outstring = outstring.replace("Inv", "Not")
    outstring = outstring.replace(")", ");")

    step = 0
    for l in split[0]:
        for i in range(step, int(l)):
            outstring = outstring.replace("(W_" + str(i)+",", "(XInput" + str(step) + "to" + str(step+l-1) + "bits["+str(i)+"],")
            outstring = outstring.replace(", W_" + str(i)+");", ", XInput" + str(step) + "to" + str(step+l-1) + "bits["+str(i)+"]);")
            outstring = outstring.replace("(W_" + str(i)+")", "(XInput" + str(step) + "to" + str(step+l-1) + "bits["+str(i)+"]);")
        step += int(l)

    step = 0

    for l in split[1]:
        for i in range(sum(split[0])+step, sum(split[0])+step+int(l)):
            outstring = outstring.replace("(W_" + str(i)+",", "(YInput" + str(step) + "to" + str(step+l-1) + "bits["+str(i-step-sum(split[0]))+"],")
            outstring = outstring.replace(", W_" + str(i)+");", ", YInput" + str(step) + "to" + str(step+l-1) + "bits["+str(i-step-sum(split[0]))+"]);")
            outstring = outstring.replace("(W_" + str(i)+")", "(YInput" + str(step) + "to" + str(step+l-1) + "bits["+str(i-step-sum(split[0]))+"]);")
        step += int(l)
    output.write(outstring)
    output.write("//Begin reassembly\n")

    output.write("Value * retV = builder.CreateZExtOrTrunc(W_" + str(int(Wires) - int(ZWidth)) +
                 ",builder.getIntNTy(" + str(ZWidth) + "));\n")
    # create reassembly
    for i in range(1, int(ZWidth)):
        if i == 1:
            output.write("Value * temp = builder.CreateZExtOrTrunc(W_" + str(int(Wires) - int(ZWidth) + i) +
                         ", builder.getIntNTy(" + str(ZWidth) + "));\n")
        else:
            output.write("temp = builder.CreateZExtOrTrunc(W_" + str(int(Wires) - int(ZWidth) + i) +
                         ", builder.getIntNTy(" + str(ZWidth) + "));\n")
        output.write("temp = builder.CreateShl(temp, " + str(i) + ", \"\");\n")
        output.write("retV = builder.CreateXor(retV, temp,\"\");\n")
    output.write("builder.CreateRet(retV);\n")
    output.write("}\n")
    output.close()
