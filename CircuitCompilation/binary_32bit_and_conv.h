//Gates: 32
//Wires: 96
//Input Width X 32
//Input Width Y 32
//Output Width Z 32
//CreateFunction
ArrayRef<Type *> args = {Type::getInt32Ty(F->getContext()),Type::getInt32Ty(F->getContext())};
std::string FunctionName = "Extern_binary_32bit_and";
FunctionCallee c = M.getOrInsertFunction(FunctionName,FunctionType::get(llvm::IntegerType::getInt32Ty(F->getContext()),args, false));
auto func = cast<Function>(c.getCallee());
func->addFnAttr("Gate");
if (c.getCallee()->hasNUsesOrMore(1)) {
} else {
Function::arg_iterator argNames = func->arg_begin();
Value *x = argNames++;
x->setName("x");
Value *y = argNames++;
y->setName("y");
BasicBlock *block = BasicBlock::Create(F->getContext(), "entry", func);
IRBuilder<> builder(block);
//Begin disassembling
Value* XInput0to31bits[32];
for (int j=0; j<32;j++){
	XInput0to31bits[j] = builder.CreateLShr(x, u_int64_t(j), "ShiftedTo"+std::to_string(j) + "BitX", true);
	XInput0to31bits[j] = builder.CreateTruncOrBitCast(XInput0to31bits[j],Type::getInt1Ty(F->getContext()));
}
Value* YInput0to31bits[32];
for (int j=0; j<32;j++){
	YInput0to31bits[j] = builder.CreateLShr(y, u_int64_t(j), "ShiftedTo"+std::to_string(j) + "BitY", true);
	YInput0to31bits[j] = builder.CreateTruncOrBitCast(YInput0to31bits[j],Type::getInt1Ty(F->getContext()));
}
//Inline gates
Value* W_64 = builder.CreateAnd(XInput0to31bits[0], YInput0to31bits[0]);
Value* W_65 = builder.CreateAnd(XInput0to31bits[1], YInput0to31bits[1]);
Value* W_66 = builder.CreateAnd(XInput0to31bits[2], YInput0to31bits[2]);
Value* W_67 = builder.CreateAnd(XInput0to31bits[3], YInput0to31bits[3]);
Value* W_68 = builder.CreateAnd(XInput0to31bits[4], YInput0to31bits[4]);
Value* W_69 = builder.CreateAnd(XInput0to31bits[5], YInput0to31bits[5]);
Value* W_70 = builder.CreateAnd(XInput0to31bits[6], YInput0to31bits[6]);
Value* W_71 = builder.CreateAnd(XInput0to31bits[7], YInput0to31bits[7]);
Value* W_72 = builder.CreateAnd(XInput0to31bits[8], YInput0to31bits[8]);
Value* W_73 = builder.CreateAnd(XInput0to31bits[9], YInput0to31bits[9]);
Value* W_74 = builder.CreateAnd(XInput0to31bits[10], YInput0to31bits[10]);
Value* W_75 = builder.CreateAnd(XInput0to31bits[11], YInput0to31bits[11]);
Value* W_76 = builder.CreateAnd(XInput0to31bits[12], YInput0to31bits[12]);
Value* W_77 = builder.CreateAnd(XInput0to31bits[13], YInput0to31bits[13]);
Value* W_78 = builder.CreateAnd(XInput0to31bits[14], YInput0to31bits[14]);
Value* W_79 = builder.CreateAnd(XInput0to31bits[15], YInput0to31bits[15]);
Value* W_80 = builder.CreateAnd(XInput0to31bits[16], YInput0to31bits[16]);
Value* W_81 = builder.CreateAnd(XInput0to31bits[17], YInput0to31bits[17]);
Value* W_82 = builder.CreateAnd(XInput0to31bits[18], YInput0to31bits[18]);
Value* W_83 = builder.CreateAnd(XInput0to31bits[19], YInput0to31bits[19]);
Value* W_84 = builder.CreateAnd(XInput0to31bits[20], YInput0to31bits[20]);
Value* W_85 = builder.CreateAnd(XInput0to31bits[21], YInput0to31bits[21]);
Value* W_86 = builder.CreateAnd(XInput0to31bits[22], YInput0to31bits[22]);
Value* W_87 = builder.CreateAnd(XInput0to31bits[23], YInput0to31bits[23]);
Value* W_88 = builder.CreateAnd(XInput0to31bits[24], YInput0to31bits[24]);
Value* W_89 = builder.CreateAnd(XInput0to31bits[25], YInput0to31bits[25]);
Value* W_90 = builder.CreateAnd(XInput0to31bits[26], YInput0to31bits[26]);
Value* W_91 = builder.CreateAnd(XInput0to31bits[27], YInput0to31bits[27]);
Value* W_92 = builder.CreateAnd(XInput0to31bits[28], YInput0to31bits[28]);
Value* W_93 = builder.CreateAnd(XInput0to31bits[29], YInput0to31bits[29]);
Value* W_94 = builder.CreateAnd(XInput0to31bits[30], YInput0to31bits[30]);
Value* W_95 = builder.CreateAnd(XInput0to31bits[31], YInput0to31bits[31]);
//Begin reassembly
Value * retV = builder.CreateZExtOrTrunc(W_64,builder.getIntNTy(32));
Value * temp = builder.CreateZExtOrTrunc(W_65, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 1, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_66, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 2, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_67, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 3, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_68, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 4, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_69, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 5, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_70, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 6, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_71, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 7, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_72, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 8, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_73, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 9, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_74, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 10, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_75, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 11, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_76, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 12, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_77, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 13, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_78, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 14, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_79, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 15, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_80, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 16, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_81, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 17, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_82, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 18, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_83, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 19, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_84, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 20, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_85, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 21, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_86, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 22, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_87, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 23, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_88, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 24, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_89, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 25, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_90, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 26, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_91, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 27, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_92, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 28, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_93, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 29, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_94, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 30, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_95, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 31, "");
retV = builder.CreateXor(retV, temp,"");
builder.CreateRet(retV);
}
