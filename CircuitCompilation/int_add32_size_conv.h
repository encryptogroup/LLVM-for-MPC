//Gates: 154
//Wires: 218
//Input Width X 32
//Input Width Y 32
//Output Width Z 32
//CreateFunction
ArrayRef<Type *> args = {Type::getInt32Ty(F->getContext()),Type::getInt32Ty(F->getContext())};
std::string FunctionName = "Extern_int_add32_size";
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
Value* W_186 = builder.CreateXor(XInput0to31bits[0], YInput0to31bits[0]);
Value* W_64 = builder.CreateAnd(XInput0to31bits[0], YInput0to31bits[0]);
Value* W_65 = builder.CreateXor(YInput0to31bits[1], W_64);
Value* W_187 = builder.CreateXor(XInput0to31bits[1], W_65);
Value* W_66 = builder.CreateXor(XInput0to31bits[1], W_64);
Value* W_67 = builder.CreateAnd(W_66, W_65);
Value* W_68 = builder.CreateXor(W_67, W_64);
Value* W_69 = builder.CreateXor(YInput0to31bits[2], W_68);
Value* W_188 = builder.CreateXor(XInput0to31bits[2], W_69);
Value* W_70 = builder.CreateXor(XInput0to31bits[2], W_68);
Value* W_71 = builder.CreateAnd(W_70, W_69);
Value* W_72 = builder.CreateXor(W_71, W_68);
Value* W_73 = builder.CreateXor(YInput0to31bits[3], W_72);
Value* W_189 = builder.CreateXor(XInput0to31bits[3], W_73);
Value* W_74 = builder.CreateXor(XInput0to31bits[3], W_72);
Value* W_75 = builder.CreateAnd(W_74, W_73);
Value* W_76 = builder.CreateXor(W_75, W_72);
Value* W_77 = builder.CreateXor(YInput0to31bits[4], W_76);
Value* W_190 = builder.CreateXor(XInput0to31bits[4], W_77);
Value* W_78 = builder.CreateXor(XInput0to31bits[4], W_76);
Value* W_79 = builder.CreateAnd(W_78, W_77);
Value* W_80 = builder.CreateXor(W_79, W_76);
Value* W_81 = builder.CreateXor(YInput0to31bits[5], W_80);
Value* W_191 = builder.CreateXor(XInput0to31bits[5], W_81);
Value* W_82 = builder.CreateXor(XInput0to31bits[5], W_80);
Value* W_83 = builder.CreateAnd(W_82, W_81);
Value* W_84 = builder.CreateXor(W_83, W_80);
Value* W_85 = builder.CreateXor(YInput0to31bits[6], W_84);
Value* W_192 = builder.CreateXor(XInput0to31bits[6], W_85);
Value* W_86 = builder.CreateXor(XInput0to31bits[6], W_84);
Value* W_87 = builder.CreateAnd(W_86, W_85);
Value* W_88 = builder.CreateXor(W_87, W_84);
Value* W_89 = builder.CreateXor(YInput0to31bits[7], W_88);
Value* W_193 = builder.CreateXor(XInput0to31bits[7], W_89);
Value* W_90 = builder.CreateXor(XInput0to31bits[7], W_88);
Value* W_91 = builder.CreateAnd(W_90, W_89);
Value* W_92 = builder.CreateXor(W_91, W_88);
Value* W_93 = builder.CreateXor(YInput0to31bits[8], W_92);
Value* W_194 = builder.CreateXor(XInput0to31bits[8], W_93);
Value* W_94 = builder.CreateXor(XInput0to31bits[8], W_92);
Value* W_95 = builder.CreateAnd(W_94, W_93);
Value* W_96 = builder.CreateXor(W_95, W_92);
Value* W_97 = builder.CreateXor(YInput0to31bits[9], W_96);
Value* W_195 = builder.CreateXor(XInput0to31bits[9], W_97);
Value* W_98 = builder.CreateXor(XInput0to31bits[9], W_96);
Value* W_99 = builder.CreateAnd(W_98, W_97);
Value* W_100 = builder.CreateXor(W_99, W_96);
Value* W_101 = builder.CreateXor(YInput0to31bits[10], W_100);
Value* W_196 = builder.CreateXor(XInput0to31bits[10], W_101);
Value* W_102 = builder.CreateXor(XInput0to31bits[10], W_100);
Value* W_103 = builder.CreateAnd(W_102, W_101);
Value* W_104 = builder.CreateXor(W_103, W_100);
Value* W_105 = builder.CreateXor(YInput0to31bits[11], W_104);
Value* W_197 = builder.CreateXor(XInput0to31bits[11], W_105);
Value* W_106 = builder.CreateXor(XInput0to31bits[11], W_104);
Value* W_107 = builder.CreateAnd(W_106, W_105);
Value* W_108 = builder.CreateXor(W_107, W_104);
Value* W_109 = builder.CreateXor(YInput0to31bits[12], W_108);
Value* W_198 = builder.CreateXor(XInput0to31bits[12], W_109);
Value* W_110 = builder.CreateXor(XInput0to31bits[12], W_108);
Value* W_111 = builder.CreateAnd(W_110, W_109);
Value* W_112 = builder.CreateXor(W_111, W_108);
Value* W_113 = builder.CreateXor(YInput0to31bits[13], W_112);
Value* W_199 = builder.CreateXor(XInput0to31bits[13], W_113);
Value* W_114 = builder.CreateXor(XInput0to31bits[13], W_112);
Value* W_115 = builder.CreateAnd(W_114, W_113);
Value* W_116 = builder.CreateXor(W_115, W_112);
Value* W_117 = builder.CreateXor(YInput0to31bits[14], W_116);
Value* W_200 = builder.CreateXor(XInput0to31bits[14], W_117);
Value* W_118 = builder.CreateXor(XInput0to31bits[14], W_116);
Value* W_119 = builder.CreateAnd(W_118, W_117);
Value* W_120 = builder.CreateXor(W_119, W_116);
Value* W_121 = builder.CreateXor(YInput0to31bits[15], W_120);
Value* W_201 = builder.CreateXor(XInput0to31bits[15], W_121);
Value* W_122 = builder.CreateXor(XInput0to31bits[15], W_120);
Value* W_123 = builder.CreateAnd(W_122, W_121);
Value* W_124 = builder.CreateXor(W_123, W_120);
Value* W_125 = builder.CreateXor(YInput0to31bits[16], W_124);
Value* W_202 = builder.CreateXor(XInput0to31bits[16], W_125);
Value* W_126 = builder.CreateXor(XInput0to31bits[16], W_124);
Value* W_127 = builder.CreateAnd(W_126, W_125);
Value* W_128 = builder.CreateXor(W_127, W_124);
Value* W_129 = builder.CreateXor(YInput0to31bits[17], W_128);
Value* W_203 = builder.CreateXor(XInput0to31bits[17], W_129);
Value* W_130 = builder.CreateXor(XInput0to31bits[17], W_128);
Value* W_131 = builder.CreateAnd(W_130, W_129);
Value* W_132 = builder.CreateXor(W_131, W_128);
Value* W_133 = builder.CreateXor(YInput0to31bits[18], W_132);
Value* W_204 = builder.CreateXor(XInput0to31bits[18], W_133);
Value* W_134 = builder.CreateXor(XInput0to31bits[18], W_132);
Value* W_135 = builder.CreateAnd(W_134, W_133);
Value* W_136 = builder.CreateXor(W_135, W_132);
Value* W_137 = builder.CreateXor(YInput0to31bits[19], W_136);
Value* W_205 = builder.CreateXor(XInput0to31bits[19], W_137);
Value* W_138 = builder.CreateXor(XInput0to31bits[19], W_136);
Value* W_139 = builder.CreateAnd(W_138, W_137);
Value* W_140 = builder.CreateXor(W_139, W_136);
Value* W_141 = builder.CreateXor(YInput0to31bits[20], W_140);
Value* W_206 = builder.CreateXor(XInput0to31bits[20], W_141);
Value* W_142 = builder.CreateXor(XInput0to31bits[20], W_140);
Value* W_143 = builder.CreateAnd(W_142, W_141);
Value* W_144 = builder.CreateXor(W_143, W_140);
Value* W_145 = builder.CreateXor(YInput0to31bits[21], W_144);
Value* W_207 = builder.CreateXor(XInput0to31bits[21], W_145);
Value* W_146 = builder.CreateXor(XInput0to31bits[21], W_144);
Value* W_147 = builder.CreateAnd(W_146, W_145);
Value* W_148 = builder.CreateXor(W_147, W_144);
Value* W_149 = builder.CreateXor(YInput0to31bits[22], W_148);
Value* W_208 = builder.CreateXor(XInput0to31bits[22], W_149);
Value* W_150 = builder.CreateXor(XInput0to31bits[22], W_148);
Value* W_151 = builder.CreateAnd(W_150, W_149);
Value* W_152 = builder.CreateXor(W_151, W_148);
Value* W_153 = builder.CreateXor(YInput0to31bits[23], W_152);
Value* W_209 = builder.CreateXor(XInput0to31bits[23], W_153);
Value* W_154 = builder.CreateXor(XInput0to31bits[23], W_152);
Value* W_155 = builder.CreateAnd(W_154, W_153);
Value* W_156 = builder.CreateXor(W_155, W_152);
Value* W_157 = builder.CreateXor(YInput0to31bits[24], W_156);
Value* W_210 = builder.CreateXor(XInput0to31bits[24], W_157);
Value* W_158 = builder.CreateXor(XInput0to31bits[24], W_156);
Value* W_159 = builder.CreateAnd(W_158, W_157);
Value* W_160 = builder.CreateXor(W_159, W_156);
Value* W_161 = builder.CreateXor(YInput0to31bits[25], W_160);
Value* W_211 = builder.CreateXor(XInput0to31bits[25], W_161);
Value* W_162 = builder.CreateXor(XInput0to31bits[25], W_160);
Value* W_163 = builder.CreateAnd(W_162, W_161);
Value* W_164 = builder.CreateXor(W_163, W_160);
Value* W_165 = builder.CreateXor(YInput0to31bits[26], W_164);
Value* W_212 = builder.CreateXor(XInput0to31bits[26], W_165);
Value* W_166 = builder.CreateXor(XInput0to31bits[26], W_164);
Value* W_167 = builder.CreateAnd(W_166, W_165);
Value* W_168 = builder.CreateXor(W_167, W_164);
Value* W_169 = builder.CreateXor(YInput0to31bits[27], W_168);
Value* W_213 = builder.CreateXor(XInput0to31bits[27], W_169);
Value* W_170 = builder.CreateXor(XInput0to31bits[27], W_168);
Value* W_171 = builder.CreateAnd(W_170, W_169);
Value* W_172 = builder.CreateXor(W_171, W_168);
Value* W_173 = builder.CreateXor(YInput0to31bits[28], W_172);
Value* W_214 = builder.CreateXor(XInput0to31bits[28], W_173);
Value* W_174 = builder.CreateXor(XInput0to31bits[28], W_172);
Value* W_175 = builder.CreateAnd(W_174, W_173);
Value* W_176 = builder.CreateXor(W_175, W_172);
Value* W_177 = builder.CreateXor(YInput0to31bits[29], W_176);
Value* W_215 = builder.CreateXor(XInput0to31bits[29], W_177);
Value* W_178 = builder.CreateXor(XInput0to31bits[29], W_176);
Value* W_179 = builder.CreateAnd(W_178, W_177);
Value* W_180 = builder.CreateXor(W_179, W_176);
Value* W_181 = builder.CreateXor(YInput0to31bits[30], W_180);
Value* W_216 = builder.CreateXor(XInput0to31bits[30], W_181);
Value* W_182 = builder.CreateXor(XInput0to31bits[30], W_180);
Value* W_183 = builder.CreateAnd(W_182, W_181);
Value* W_184 = builder.CreateXor(W_183, W_180);
Value* W_185 = builder.CreateXor(YInput0to31bits[31], W_184);
Value* W_217 = builder.CreateXor(XInput0to31bits[31], W_185);
//Begin reassembly
Value * retV = builder.CreateZExtOrTrunc(W_186,builder.getIntNTy(32));
Value * temp = builder.CreateZExtOrTrunc(W_187, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 1, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_188, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 2, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_189, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 3, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_190, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 4, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_191, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 5, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_192, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 6, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_193, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 7, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_194, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 8, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_195, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 9, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_196, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 10, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_197, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 11, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_198, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 12, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_199, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 13, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_200, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 14, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_201, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 15, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_202, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 16, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_203, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 17, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_204, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 18, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_205, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 19, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_206, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 20, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_207, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 21, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_208, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 22, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_209, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 23, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_210, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 24, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_211, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 25, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_212, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 26, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_213, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 27, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_214, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 28, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_215, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 29, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_216, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 30, "");
retV = builder.CreateXor(retV, temp,"");
temp = builder.CreateZExtOrTrunc(W_217, builder.getIntNTy(32));
temp = builder.CreateShl(temp, 31, "");
retV = builder.CreateXor(retV, temp,"");
builder.CreateRet(retV);
}
