//Gates: 96
//Wires: 160
//Input Width X 32
//Input Width Y 32
//Output Width Z 1
//CreateFunction
ArrayRef<Type *> args = {Type::getInt32Ty(F->getContext()),Type::getInt32Ty(F->getContext())};
std::string FunctionName = "Extern_comparator_32bit_neq";
FunctionCallee c = M.getOrInsertFunction(FunctionName,FunctionType::get(llvm::IntegerType::getInt1Ty(F->getContext()),args, false));
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
Value* W_64 = builder.CreateXor(XInput0to31bits[0], YInput0to31bits[0]);
Value* W_65 = builder.CreateXor(XInput0to31bits[1], YInput0to31bits[1]);
Value* W_66 = builder.CreateXor(XInput0to31bits[2], YInput0to31bits[2]);
Value* W_67 = builder.CreateXor(XInput0to31bits[3], YInput0to31bits[3]);
Value* W_68 = builder.CreateXor(XInput0to31bits[4], YInput0to31bits[4]);
Value* W_69 = builder.CreateXor(XInput0to31bits[5], YInput0to31bits[5]);
Value* W_70 = builder.CreateXor(XInput0to31bits[6], YInput0to31bits[6]);
Value* W_71 = builder.CreateXor(XInput0to31bits[7], YInput0to31bits[7]);
Value* W_72 = builder.CreateXor(XInput0to31bits[8], YInput0to31bits[8]);
Value* W_73 = builder.CreateXor(XInput0to31bits[9], YInput0to31bits[9]);
Value* W_74 = builder.CreateXor(XInput0to31bits[10], YInput0to31bits[10]);
Value* W_75 = builder.CreateXor(XInput0to31bits[11], YInput0to31bits[11]);
Value* W_76 = builder.CreateXor(XInput0to31bits[12], YInput0to31bits[12]);
Value* W_77 = builder.CreateXor(XInput0to31bits[13], YInput0to31bits[13]);
Value* W_78 = builder.CreateXor(XInput0to31bits[14], YInput0to31bits[14]);
Value* W_79 = builder.CreateXor(XInput0to31bits[15], YInput0to31bits[15]);
Value* W_80 = builder.CreateXor(XInput0to31bits[16], YInput0to31bits[16]);
Value* W_81 = builder.CreateXor(XInput0to31bits[17], YInput0to31bits[17]);
Value* W_82 = builder.CreateXor(XInput0to31bits[18], YInput0to31bits[18]);
Value* W_83 = builder.CreateXor(XInput0to31bits[19], YInput0to31bits[19]);
Value* W_84 = builder.CreateXor(XInput0to31bits[20], YInput0to31bits[20]);
Value* W_85 = builder.CreateXor(XInput0to31bits[21], YInput0to31bits[21]);
Value* W_86 = builder.CreateXor(XInput0to31bits[22], YInput0to31bits[22]);
Value* W_87 = builder.CreateXor(XInput0to31bits[23], YInput0to31bits[23]);
Value* W_88 = builder.CreateXor(XInput0to31bits[24], YInput0to31bits[24]);
Value* W_89 = builder.CreateXor(XInput0to31bits[25], YInput0to31bits[25]);
Value* W_90 = builder.CreateXor(XInput0to31bits[26], YInput0to31bits[26]);
Value* W_91 = builder.CreateXor(XInput0to31bits[27], YInput0to31bits[27]);
Value* W_92 = builder.CreateXor(XInput0to31bits[28], YInput0to31bits[28]);
Value* W_93 = builder.CreateXor(XInput0to31bits[29], YInput0to31bits[29]);
Value* W_94 = builder.CreateXor(XInput0to31bits[30], YInput0to31bits[30]);
Value* W_95 = builder.CreateXor(XInput0to31bits[31], YInput0to31bits[31]);
Value* W_96 = builder.CreateNot(W_64);
Value* W_97 = builder.CreateNot(W_65);
Value* W_98 = builder.CreateNot(W_66);
Value* W_99 = builder.CreateNot(W_67);
Value* W_100 = builder.CreateNot(W_68);
Value* W_101 = builder.CreateNot(W_69);
Value* W_102 = builder.CreateNot(W_70);
Value* W_103 = builder.CreateNot(W_71);
Value* W_104 = builder.CreateNot(W_72);
Value* W_105 = builder.CreateNot(W_73);
Value* W_106 = builder.CreateNot(W_74);
Value* W_107 = builder.CreateNot(W_75);
Value* W_108 = builder.CreateNot(W_76);
Value* W_109 = builder.CreateNot(W_77);
Value* W_110 = builder.CreateNot(W_78);
Value* W_111 = builder.CreateNot(W_79);
Value* W_112 = builder.CreateNot(W_80);
Value* W_113 = builder.CreateNot(W_81);
Value* W_114 = builder.CreateNot(W_82);
Value* W_115 = builder.CreateNot(W_83);
Value* W_116 = builder.CreateNot(W_84);
Value* W_117 = builder.CreateNot(W_85);
Value* W_118 = builder.CreateNot(W_86);
Value* W_119 = builder.CreateNot(W_87);
Value* W_120 = builder.CreateNot(W_88);
Value* W_121 = builder.CreateNot(W_89);
Value* W_122 = builder.CreateNot(W_90);
Value* W_123 = builder.CreateNot(W_91);
Value* W_124 = builder.CreateNot(W_92);
Value* W_125 = builder.CreateNot(W_93);
Value* W_126 = builder.CreateNot(W_94);
Value* W_127 = builder.CreateNot(W_95);
Value* W_128 = builder.CreateAnd(W_96, W_97);
Value* W_129 = builder.CreateAnd(W_98, W_99);
Value* W_130 = builder.CreateAnd(W_100, W_101);
Value* W_131 = builder.CreateAnd(W_102, W_103);
Value* W_132 = builder.CreateAnd(W_104, W_105);
Value* W_133 = builder.CreateAnd(W_106, W_107);
Value* W_134 = builder.CreateAnd(W_108, W_109);
Value* W_135 = builder.CreateAnd(W_110, W_111);
Value* W_136 = builder.CreateAnd(W_112, W_113);
Value* W_137 = builder.CreateAnd(W_114, W_115);
Value* W_138 = builder.CreateAnd(W_116, W_117);
Value* W_139 = builder.CreateAnd(W_118, W_119);
Value* W_140 = builder.CreateAnd(W_120, W_121);
Value* W_141 = builder.CreateAnd(W_122, W_123);
Value* W_142 = builder.CreateAnd(W_124, W_125);
Value* W_143 = builder.CreateAnd(W_126, W_127);
Value* W_144 = builder.CreateAnd(W_128, W_129);
Value* W_145 = builder.CreateAnd(W_130, W_131);
Value* W_146 = builder.CreateAnd(W_132, W_133);
Value* W_147 = builder.CreateAnd(W_134, W_135);
Value* W_148 = builder.CreateAnd(W_136, W_137);
Value* W_149 = builder.CreateAnd(W_138, W_139);
Value* W_150 = builder.CreateAnd(W_140, W_141);
Value* W_151 = builder.CreateAnd(W_142, W_143);
Value* W_152 = builder.CreateAnd(W_144, W_145);
Value* W_153 = builder.CreateAnd(W_146, W_147);
Value* W_154 = builder.CreateAnd(W_148, W_149);
Value* W_155 = builder.CreateAnd(W_150, W_151);
Value* W_156 = builder.CreateAnd(W_152, W_153);
Value* W_157 = builder.CreateAnd(W_154, W_155);
Value* W_158 = builder.CreateAnd(W_156, W_157);
Value* W_159 = builder.CreateNot(W_158);
//Begin reassembly
Value * retV = builder.CreateZExtOrTrunc(W_159,builder.getIntNTy(1));
builder.CreateRet(retV);
}
