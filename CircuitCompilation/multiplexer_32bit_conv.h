//Gates: 97
//Wires: 162
//Input Width X 1
//Input Width Y 64
//Output Width Z 32
//Have Split X,1,Y,32,32
//CreateFunction
ArrayRef<Type *> args = {Type::getInt1Ty(F->getContext()),Type::getInt32Ty(F->getContext()),Type::getInt32Ty(F->getContext())};
std::string FunctionName = "Extern_multiplexer_32bit";
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
Value *z = argNames++;
z->setName("z");
BasicBlock *block = BasicBlock::Create(F->getContext(), "entry", func);
IRBuilder<> builder(block);
//Begin disassembling
Value* XInput0to0bits[1];

XInput0to0bits[0] = builder.CreateLShr(x, u_int64_t(0), "ShiftedTo"+std::to_string(0) + "BitX", true);
//XInput0to0bits[0] = builder.CreateLShr(x, u_int64_t(0), "ShiftedTo"+std::to_string(0) + "BitX", true);

Value* YInput0to31bits[32];
for (int j=0; j<32;j++){
	YInput0to31bits[j] = builder.CreateLShr(y, u_int64_t(j), "ShiftedTo"+std::to_string(j) + "BitY", true);
	YInput0to31bits[j] = builder.CreateTruncOrBitCast(YInput0to31bits[j],Type::getInt1Ty(F->getContext()));
}
Value* YInput32to63bits[32];
for (int j=0; j<32;j++){
	YInput32to63bits[j] = builder.CreateLShr(z, u_int64_t(j), "ShiftedTo"+std::to_string(j) + "BitY", true);
	YInput32to63bits[j] = builder.CreateTruncOrBitCast(YInput32to63bits[j],Type::getInt1Ty(F->getContext()));
}
//Inline gates
Value* W_65 = builder.CreateNot(XInput0to0bits[0]);;
Value* W_66 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[0]);
Value* W_67 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[1]);
Value* W_68 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[2]);
Value* W_69 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[3]);
Value* W_70 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[4]);
Value* W_71 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[5]);
Value* W_72 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[6]);
Value* W_73 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[7]);
Value* W_74 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[8]);
Value* W_75 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[9]);
Value* W_76 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[10]);
Value* W_77 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[11]);
Value* W_78 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[12]);
Value* W_79 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[13]);
Value* W_80 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[14]);
Value* W_81 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[15]);
Value* W_82 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[16]);
Value* W_83 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[17]);
Value* W_84 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[18]);
Value* W_85 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[19]);
Value* W_86 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[20]);
Value* W_87 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[21]);
Value* W_88 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[22]);
Value* W_89 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[23]);
Value* W_90 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[24]);
Value* W_91 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[25]);
Value* W_92 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[26]);
Value* W_93 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[27]);
Value* W_94 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[28]);
Value* W_95 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[29]);
Value* W_96 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[30]);
Value* W_97 = builder.CreateAnd(XInput0to0bits[0], YInput0to31bits[31]);
Value* W_98 = builder.CreateAnd(W_65, YInput32to63bits[0]);
Value* W_99 = builder.CreateAnd(W_65, YInput32to63bits[1]);
Value* W_100 = builder.CreateAnd(W_65, YInput32to63bits[2]);
Value* W_101 = builder.CreateAnd(W_65, YInput32to63bits[3]);
Value* W_102 = builder.CreateAnd(W_65, YInput32to63bits[4]);
Value* W_103 = builder.CreateAnd(W_65, YInput32to63bits[5]);
Value* W_104 = builder.CreateAnd(W_65, YInput32to63bits[6]);
Value* W_105 = builder.CreateAnd(W_65, YInput32to63bits[7]);
Value* W_106 = builder.CreateAnd(W_65, YInput32to63bits[8]);
Value* W_107 = builder.CreateAnd(W_65, YInput32to63bits[9]);
Value* W_108 = builder.CreateAnd(W_65, YInput32to63bits[10]);
Value* W_109 = builder.CreateAnd(W_65, YInput32to63bits[11]);
Value* W_110 = builder.CreateAnd(W_65, YInput32to63bits[12]);
Value* W_111 = builder.CreateAnd(W_65, YInput32to63bits[13]);
Value* W_112 = builder.CreateAnd(W_65, YInput32to63bits[14]);
Value* W_113 = builder.CreateAnd(W_65, YInput32to63bits[15]);
Value* W_114 = builder.CreateAnd(W_65, YInput32to63bits[16]);
Value* W_115 = builder.CreateAnd(W_65, YInput32to63bits[17]);
Value* W_116 = builder.CreateAnd(W_65, YInput32to63bits[18]);
Value* W_117 = builder.CreateAnd(W_65, YInput32to63bits[19]);
Value* W_118 = builder.CreateAnd(W_65, YInput32to63bits[20]);
Value* W_119 = builder.CreateAnd(W_65, YInput32to63bits[21]);
Value* W_120 = builder.CreateAnd(W_65, YInput32to63bits[22]);
Value* W_121 = builder.CreateAnd(W_65, YInput32to63bits[23]);
Value* W_122 = builder.CreateAnd(W_65, YInput32to63bits[24]);
Value* W_123 = builder.CreateAnd(W_65, YInput32to63bits[25]);
Value* W_124 = builder.CreateAnd(W_65, YInput32to63bits[26]);
Value* W_125 = builder.CreateAnd(W_65, YInput32to63bits[27]);
Value* W_126 = builder.CreateAnd(W_65, YInput32to63bits[28]);
Value* W_127 = builder.CreateAnd(W_65, YInput32to63bits[29]);
Value* W_128 = builder.CreateAnd(W_65, YInput32to63bits[30]);
Value* W_129 = builder.CreateAnd(W_65, YInput32to63bits[31]);
Value* W_130 = builder.CreateXor(W_66, W_98);
Value* W_131 = builder.CreateXor(W_67, W_99);
Value* W_132 = builder.CreateXor(W_68, W_100);
Value* W_133 = builder.CreateXor(W_69, W_101);
Value* W_134 = builder.CreateXor(W_70, W_102);
Value* W_135 = builder.CreateXor(W_71, W_103);
Value* W_136 = builder.CreateXor(W_72, W_104);
Value* W_137 = builder.CreateXor(W_73, W_105);
Value* W_138 = builder.CreateXor(W_74, W_106);
Value* W_139 = builder.CreateXor(W_75, W_107);
Value* W_140 = builder.CreateXor(W_76, W_108);
Value* W_141 = builder.CreateXor(W_77, W_109);
Value* W_142 = builder.CreateXor(W_78, W_110);
Value* W_143 = builder.CreateXor(W_79, W_111);
Value* W_144 = builder.CreateXor(W_80, W_112);
Value* W_145 = builder.CreateXor(W_81, W_113);
Value* W_146 = builder.CreateXor(W_82, W_114);
Value* W_147 = builder.CreateXor(W_83, W_115);
Value* W_148 = builder.CreateXor(W_84, W_116);
Value* W_149 = builder.CreateXor(W_85, W_117);
Value* W_150 = builder.CreateXor(W_86, W_118);
Value* W_151 = builder.CreateXor(W_87, W_119);
Value* W_152 = builder.CreateXor(W_88, W_120);
Value* W_153 = builder.CreateXor(W_89, W_121);
Value* W_154 = builder.CreateXor(W_90, W_122);
Value* W_155 = builder.CreateXor(W_91, W_123);
Value* W_156 = builder.CreateXor(W_92, W_124);
Value* W_157 = builder.CreateXor(W_93, W_125);
Value* W_158 = builder.CreateXor(W_94, W_126);
Value* W_159 = builder.CreateXor(W_95, W_127);
Value* W_160 = builder.CreateXor(W_96, W_128);
Value* W_161 = builder.CreateXor(W_97, W_129);
//Begin reassembly
Value * retV = builder.CreateZExtOrTrunc(W_130,builder.getIntNTy(32));
Value * temp = builder.CreateZExtOrTrunc(W_131, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 1, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_132, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 2, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_133, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 3, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_134, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 4, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_135, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 5, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_136, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 6, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_137, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 7, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_138, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 8, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_139, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 9, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_140, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 10, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_141, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 11, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_142, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 12, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_143, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 13, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_144, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 14, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_145, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 15, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_146, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 16, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_147, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 17, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_148, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 18, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_149, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 19, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_150, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 20, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_151, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 21, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_152, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 22, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_153, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 23, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_154, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 24, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_155, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 25, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_156, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 26, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_157, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 27, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_158, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 28, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_159, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 29, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_160, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 30, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_161, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 31, "");
retV = builder.CreateXor(retV, temp,"");
builder.CreateRet(retV);
}
