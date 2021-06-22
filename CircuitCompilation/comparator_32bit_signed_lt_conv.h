//Gates: 300
//Wires: 364
//Input Width X 32
//Input Width Y 32
//Output Width Z 1
//CreateFunction
ArrayRef<Type *> args = {Type::getInt32Ty(F->getContext()),Type::getInt32Ty(F->getContext())};
std::string FunctionName = "Extern_comparator_32bit_signed_lt";
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
Value* W_341 = builder.CreateNot(YInput0to31bits[27]);;
Value* W_359 = builder.CreateNot(XInput0to31bits[17]);;
Value* W_349 = builder.CreateNot(XInput0to31bits[28]);;
Value* W_321 = builder.CreateNot(XInput0to31bits[16]);;
Value* W_360 = builder.CreateNot(YInput0to31bits[17]);;
Value* W_355 = builder.CreateNot(YInput0to31bits[19]);;
Value* W_362 = builder.CreateNot(XInput0to31bits[14]);;
Value* W_330 = builder.CreateNot(XInput0to31bits[23]);;
Value* W_358 = builder.CreateNot(XInput0to31bits[1]);;
Value* W_356 = builder.CreateNot(XInput0to31bits[25]);;
Value* W_226 = builder.CreateNot(YInput0to31bits[14]);;
Value* W_346 = builder.CreateNot(XInput0to31bits[29]);;
Value* W_261 = builder.CreateNot(YInput0to31bits[8]);;
Value* W_347 = builder.CreateNot(YInput0to31bits[31]);;
Value* W_342 = builder.CreateNot(YInput0to31bits[2]);;
Value* W_343 = builder.CreateNot(XInput0to31bits[11]);;
Value* W_333 = builder.CreateNot(XInput0to31bits[30]);;
Value* W_326 = builder.CreateNot(XInput0to31bits[31]);;
Value* W_334 = builder.CreateNot(XInput0to31bits[2]);;
Value* W_241 = builder.CreateNot(YInput0to31bits[10]);;
Value* W_246 = builder.CreateNot(YInput0to31bits[26]);;
Value* W_202 = builder.CreateNot(YInput0to31bits[4]);;
Value* W_323 = builder.CreateNot(XInput0to31bits[15]);;
Value* W_322 = builder.CreateNot(YInput0to31bits[1]);;
Value* W_357 = builder.CreateNot(XInput0to31bits[7]);;
Value* W_354 = builder.CreateNot(XInput0to31bits[9]);;
Value* W_340 = builder.CreateNot(XInput0to31bits[12]);;
Value* W_328 = builder.CreateNot(XInput0to31bits[22]);;
Value* W_345 = builder.CreateNot(YInput0to31bits[23]);;
Value* W_324 = builder.CreateNot(XInput0to31bits[13]);;
Value* W_238 = builder.CreateNot(YInput0to31bits[20]);;
Value* W_250 = builder.CreateNot(YInput0to31bits[16]);;
Value* W_167 = builder.CreateNot(YInput0to31bits[12]);;
Value* W_350 = builder.CreateNot(XInput0to31bits[10]);;
Value* W_337 = builder.CreateNot(XInput0to31bits[5]);;
Value* W_253 = builder.CreateNot(YInput0to31bits[22]);;
Value* W_352 = builder.CreateNot(YInput0to31bits[11]);;
Value* W_351 = builder.CreateNot(XInput0to31bits[19]);;
Value* W_200 = builder.CreateNot(YInput0to31bits[5]);;
Value* W_325 = builder.CreateNot(XInput0to31bits[18]);;
Value* W_248 = builder.CreateNot(YInput0to31bits[6]);;
Value* W_222 = builder.CreateNot(YInput0to31bits[30]);;
Value* W_339 = builder.CreateNot(YInput0to31bits[29]);;
Value* W_204 = builder.CreateNot(YInput0to31bits[25]);;
Value* W_224 = builder.CreateNot(YInput0to31bits[28]);;
Value* W_353 = builder.CreateNot(XInput0to31bits[6]);;
Value* W_338 = builder.CreateNot(XInput0to31bits[21]);;
Value* W_344 = builder.CreateNot(XInput0to31bits[20]);;
Value* W_198 = builder.CreateNot(YInput0to31bits[24]);;
Value* W_206 = builder.CreateNot(YInput0to31bits[13]);;
Value* W_336 = builder.CreateNot(XInput0to31bits[26]);;
Value* W_332 = builder.CreateNot(XInput0to31bits[3]);;
Value* W_361 = builder.CreateNot(XInput0to31bits[27]);;
Value* W_335 = builder.CreateNot(YInput0to31bits[15]);;
Value* W_243 = builder.CreateNot(YInput0to31bits[18]);;
Value* W_348 = builder.CreateNot(YInput0to31bits[9]);;
Value* W_275 = builder.CreateNot(XInput0to31bits[0]);;
Value* W_329 = builder.CreateNot(YInput0to31bits[3]);;
Value* W_331 = builder.CreateNot(YInput0to31bits[21]);;
Value* W_327 = builder.CreateNot(YInput0to31bits[7]);;
Value* W_280 = builder.CreateAnd(W_321, YInput0to31bits[16]);
Value* W_281 = builder.CreateAnd(W_322, XInput0to31bits[1]);
Value* W_283 = builder.CreateAnd(W_323, YInput0to31bits[15]);
Value* W_285 = builder.CreateAnd(W_324, YInput0to31bits[13]);
Value* W_307 = builder.CreateAnd(W_325, YInput0to31bits[18]);
Value* W_316 = builder.CreateAnd(W_326, YInput0to31bits[31]);
Value* W_308 = builder.CreateAnd(W_327, XInput0to31bits[7]);
Value* W_282 = builder.CreateAnd(W_328, YInput0to31bits[22]);
Value* W_314 = builder.CreateAnd(W_329, XInput0to31bits[3]);
Value* W_320 = builder.CreateAnd(W_330, YInput0to31bits[23]);
Value* W_284 = builder.CreateAnd(W_331, XInput0to31bits[21]);
Value* W_319 = builder.CreateAnd(W_332, YInput0to31bits[3]);
Value* W_287 = builder.CreateAnd(W_333, YInput0to31bits[30]);
Value* W_318 = builder.CreateAnd(W_334, YInput0to31bits[2]);
Value* W_288 = builder.CreateAnd(W_335, XInput0to31bits[15]);
Value* W_289 = builder.CreateAnd(W_336, YInput0to31bits[26]);
Value* W_290 = builder.CreateAnd(W_337, YInput0to31bits[5]);
Value* W_291 = builder.CreateAnd(W_338, YInput0to31bits[21]);
Value* W_292 = builder.CreateAnd(W_339, XInput0to31bits[29]);
Value* W_293 = builder.CreateAnd(W_340, YInput0to31bits[12]);
Value* W_294 = builder.CreateAnd(W_341, XInput0to31bits[27]);
Value* W_295 = builder.CreateAnd(W_342, XInput0to31bits[2]);
Value* W_296 = builder.CreateAnd(W_343, YInput0to31bits[11]);
Value* W_297 = builder.CreateAnd(W_344, YInput0to31bits[20]);
Value* W_313 = builder.CreateAnd(W_345, XInput0to31bits[23]);
Value* W_298 = builder.CreateAnd(W_346, YInput0to31bits[29]);
Value* W_302 = builder.CreateAnd(W_347, XInput0to31bits[31]);
Value* W_230 = builder.CreateAnd(W_348, XInput0to31bits[9]);
Value* W_299 = builder.CreateAnd(W_349, YInput0to31bits[28]);
Value* W_300 = builder.CreateAnd(W_350, YInput0to31bits[10]);
Value* W_301 = builder.CreateAnd(W_351, YInput0to31bits[19]);
Value* W_303 = builder.CreateAnd(W_352, XInput0to31bits[11]);
Value* W_312 = builder.CreateAnd(W_353, YInput0to31bits[6]);
Value* W_304 = builder.CreateAnd(W_354, YInput0to31bits[9]);
Value* W_305 = builder.CreateAnd(W_355, XInput0to31bits[19]);
Value* W_306 = builder.CreateAnd(W_356, YInput0to31bits[25]);
Value* W_309 = builder.CreateAnd(W_357, YInput0to31bits[7]);
Value* W_310 = builder.CreateAnd(W_358, YInput0to31bits[1]);
Value* W_311 = builder.CreateAnd(W_359, YInput0to31bits[17]);
Value* W_315 = builder.CreateAnd(W_360, XInput0to31bits[17]);
Value* W_317 = builder.CreateAnd(W_361, YInput0to31bits[27]);
Value* W_286 = builder.CreateAnd(W_362, YInput0to31bits[14]);
Value* W_78 = builder.CreateNot(W_280);
Value* W_274 = builder.CreateNot(W_281);
Value* W_277 = builder.CreateNot(W_282);
Value* W_278 = builder.CreateNot(W_283);
Value* W_188 = builder.CreateNot(W_284);
Value* W_239 = builder.CreateNot(W_285);
Value* W_279 = builder.CreateNot(W_286);
Value* W_269 = builder.CreateNot(W_287);
Value* W_139 = builder.CreateNot(W_288);
Value* W_255 = builder.CreateNot(W_289);
Value* W_251 = builder.CreateNot(W_290);
Value* W_256 = builder.CreateNot(W_291);
Value* W_186 = builder.CreateNot(W_292);
Value* W_86 = builder.CreateNot(W_293);
Value* W_141 = builder.CreateNot(W_294);
Value* W_271 = builder.CreateNot(W_295);
Value* W_258 = builder.CreateNot(W_296);
Value* W_257 = builder.CreateNot(W_297);
Value* W_262 = builder.CreateNot(W_298);
Value* W_263 = builder.CreateNot(W_299);
Value* W_259 = builder.CreateNot(W_300);
Value* W_264 = builder.CreateNot(W_301);
Value* W_268 = builder.CreateNot(W_302);
Value* W_157 = builder.CreateNot(W_303);
Value* W_260 = builder.CreateNot(W_304);
Value* W_128 = builder.CreateNot(W_305);
Value* W_244 = builder.CreateNot(W_306);
Value* W_265 = builder.CreateNot(W_307);
Value* W_180 = builder.CreateNot(W_308);
Value* W_266 = builder.CreateNot(W_309);
Value* W_272 = builder.CreateNot(W_310);
Value* W_74 = builder.CreateNot(W_311);
Value* W_267 = builder.CreateNot(W_312);
Value* W_184 = builder.CreateNot(W_313);
Value* W_270 = builder.CreateNot(W_314);
Value* W_182 = builder.CreateNot(W_315);
Value* W_130 = builder.CreateNot(W_316);
Value* W_254 = builder.CreateNot(W_317);
Value* W_273 = builder.CreateNot(W_318);
Value* W_132 = builder.CreateNot(W_319);
Value* W_276 = builder.CreateNot(W_320);
Value* W_235 = builder.CreateAnd(W_254, W_255);
Value* W_227 = builder.CreateAnd(W_256, W_257);
Value* W_229 = builder.CreateAnd(W_258, W_259);
Value* W_231 = builder.CreateAnd(W_260, W_261);
Value* W_228 = builder.CreateAnd(W_262, W_263);
Value* W_76 = builder.CreateAnd(W_264, W_265);
Value* W_233 = builder.CreateAnd(W_266, W_267);
Value* W_148 = builder.CreateAnd(W_268, W_269);
Value* W_146 = builder.CreateAnd(W_270, W_271);
Value* W_191 = builder.CreateAnd(W_272, W_273);
Value* W_234 = builder.CreateAnd(W_274, W_275);
Value* W_150 = builder.CreateAnd(W_276, W_277);
Value* W_236 = builder.CreateAnd(W_278, W_279);
Value* W_237 = builder.CreateAnd(W_256, XInput0to31bits[20]);
Value* W_240 = builder.CreateAnd(W_258, XInput0to31bits[10]);
Value* W_242 = builder.CreateAnd(W_264, XInput0to31bits[18]);
Value* W_232 = builder.CreateAnd(W_260, XInput0to31bits[8]);
Value* W_247 = builder.CreateAnd(W_266, XInput0to31bits[6]);
Value* W_249 = builder.CreateAnd(W_74, XInput0to31bits[16]);
Value* W_221 = builder.CreateAnd(W_268, XInput0to31bits[30]);
Value* W_223 = builder.CreateAnd(W_262, XInput0to31bits[28]);
Value* W_252 = builder.CreateAnd(W_276, XInput0to31bits[22]);
Value* W_245 = builder.CreateAnd(W_254, XInput0to31bits[26]);
Value* W_225 = builder.CreateAnd(W_278, XInput0to31bits[14]);
Value* W_208 = builder.CreateAnd(W_221, W_222);
Value* W_209 = builder.CreateAnd(W_223, W_224);
Value* W_210 = builder.CreateAnd(W_225, W_226);
Value* W_72 = builder.CreateAnd(W_150, W_227);
Value* W_113 = builder.CreateAnd(W_148, W_228);
Value* W_211 = builder.CreateAnd(W_229, W_230);
Value* W_207 = builder.CreateAnd(W_229, W_231);
Value* W_212 = builder.CreateAnd(W_229, W_232);
Value* W_199 = builder.CreateAnd(W_233, XInput0to31bits[5]);
Value* W_213 = builder.CreateAnd(W_234, YInput0to31bits[0]);
Value* W_203 = builder.CreateAnd(W_235, XInput0to31bits[25]);
Value* W_205 = builder.CreateAnd(W_236, XInput0to31bits[13]);
Value* W_214 = builder.CreateAnd(W_237, W_238);
Value* W_84 = builder.CreateAnd(W_236, W_239);
Value* W_215 = builder.CreateAnd(W_240, W_241);
Value* W_216 = builder.CreateAnd(W_242, W_243);
Value* W_197 = builder.CreateAnd(W_235, W_244);
Value* W_217 = builder.CreateAnd(W_245, W_246);
Value* W_218 = builder.CreateAnd(W_247, W_248);
Value* W_219 = builder.CreateAnd(W_249, W_250);
Value* W_201 = builder.CreateAnd(W_233, W_251);
Value* W_220 = builder.CreateAnd(W_252, W_253);
Value* W_168 = builder.CreateAnd(W_197, W_198);
Value* W_189 = builder.CreateAnd(W_199, W_200);
Value* W_169 = builder.CreateAnd(W_201, W_202);
Value* W_192 = builder.CreateAnd(W_203, W_204);
Value* W_193 = builder.CreateAnd(W_205, W_206);
Value* W_166 = builder.CreateAnd(W_84, XInput0to31bits[12]);
Value* W_194 = builder.CreateAnd(W_207, XInput0to31bits[8]);
Value* W_195 = builder.CreateAnd(W_197, XInput0to31bits[24]);
Value* W_196 = builder.CreateAnd(W_201, XInput0to31bits[4]);
Value* W_119 = builder.CreateNot(W_208);
Value* W_185 = builder.CreateNot(W_209);
Value* W_165 = builder.CreateNot(W_210);
Value* W_175 = builder.CreateNot(W_211);
Value* W_177 = builder.CreateNot(W_207);
Value* W_178 = builder.CreateNot(W_212);
Value* W_190 = builder.CreateNot(W_213);
Value* W_187 = builder.CreateNot(W_214);
Value* W_176 = builder.CreateNot(W_215);
Value* W_116 = builder.CreateNot(W_216);
Value* W_159 = builder.CreateNot(W_217);
Value* W_179 = builder.CreateNot(W_218);
Value* W_181 = builder.CreateNot(W_219);
Value* W_183 = builder.CreateNot(W_220);
Value* W_153 = builder.CreateAnd(W_166, W_167);
Value* W_154 = builder.CreateAnd(W_168, XInput0to31bits[24]);
Value* W_155 = builder.CreateAnd(W_169, XInput0to31bits[4]);
Value* W_143 = builder.CreateAnd(W_175, W_176);
Value* W_170 = builder.CreateAnd(W_177, W_178);
Value* W_102 = builder.CreateAnd(W_179, W_180);
Value* W_171 = builder.CreateAnd(W_181, W_182);
Value* W_99 = builder.CreateAnd(W_183, W_184);
Value* W_172 = builder.CreateAnd(W_185, W_186);
Value* W_173 = builder.CreateAnd(W_187, W_188);
Value* W_160 = builder.CreateNot(W_168);
Value* W_107 = builder.CreateNot(W_189);
Value* W_162 = builder.CreateNot(W_169);
Value* W_174 = builder.CreateAnd(W_190, W_191);
Value* W_158 = builder.CreateNot(W_192);
Value* W_164 = builder.CreateNot(W_193);
Value* W_156 = builder.CreateNot(W_194);
Value* W_161 = builder.CreateNot(W_195);
Value* W_163 = builder.CreateNot(W_196);
Value* W_138 = builder.CreateNot(W_153);
Value* W_140 = builder.CreateNot(W_154);
Value* W_111 = builder.CreateNot(W_155);
Value* W_142 = builder.CreateAnd(W_156, W_157);
Value* W_124 = builder.CreateAnd(W_158, W_159);
Value* W_151 = builder.CreateAnd(W_160, W_161);
Value* W_152 = builder.CreateAnd(W_162, W_163);
Value* W_126 = builder.CreateAnd(W_164, W_165);
Value* W_96 = builder.CreateNot(W_170);
Value* W_144 = builder.CreateNot(W_171);
Value* W_147 = builder.CreateNot(W_172);
Value* W_149 = builder.CreateNot(W_173);
Value* W_145 = builder.CreateNot(W_174);
Value* W_125 = builder.CreateAnd(W_138, W_139);
Value* W_123 = builder.CreateAnd(W_140, W_141);
Value* W_91 = builder.CreateAnd(W_142, W_143);
Value* W_134 = builder.CreateAnd(W_144, W_76);
Value* W_135 = builder.CreateAnd(W_145, W_146);
Value* W_136 = builder.CreateAnd(W_147, W_148);
Value* W_137 = builder.CreateAnd(W_149, W_150);
Value* W_133 = builder.CreateNot(W_151);
Value* W_131 = builder.CreateNot(W_152);
Value* W_122 = builder.CreateAnd(W_123, W_124);
Value* W_81 = builder.CreateAnd(W_125, W_126);
Value* W_120 = builder.CreateAnd(W_131, W_132);
Value* W_70 = builder.CreateAnd(W_133, W_113);
Value* W_127 = builder.CreateNot(W_134);
Value* W_121 = builder.CreateNot(W_135);
Value* W_129 = builder.CreateNot(W_136);
Value* W_104 = builder.CreateNot(W_137);
Value* W_117 = builder.CreateAnd(W_120, W_121);
Value* W_112 = builder.CreateNot(W_122);
Value* W_115 = builder.CreateAnd(W_127, W_128);
Value* W_118 = builder.CreateAnd(W_129, W_130);
Value* W_109 = builder.CreateAnd(W_112, W_113);
Value* W_114 = builder.CreateAnd(W_115, W_116);
Value* W_110 = builder.CreateNot(W_117);
Value* W_67 = builder.CreateAnd(W_118, W_119);
Value* W_88 = builder.CreateNot(W_109);
Value* W_106 = builder.CreateAnd(W_110, W_111);
Value* W_108 = builder.CreateNot(W_114);
Value* W_101 = builder.CreateAnd(W_106, W_107);
Value* W_105 = builder.CreateAnd(W_108, W_72);
Value* W_100 = builder.CreateAnd(W_101, W_102);
Value* W_103 = builder.CreateNot(W_105);
Value* W_95 = builder.CreateNot(W_100);
Value* W_98 = builder.CreateAnd(W_103, W_104);
Value* W_94 = builder.CreateAnd(W_95, W_96);
Value* W_97 = builder.CreateAnd(W_98, W_99);
Value* W_90 = builder.CreateNot(W_94);
Value* W_93 = builder.CreateNot(W_97);
Value* W_89 = builder.CreateAnd(W_90, W_91);
Value* W_92 = builder.CreateAnd(W_93, W_70);
Value* W_85 = builder.CreateNot(W_89);
Value* W_87 = builder.CreateNot(W_92);
Value* W_83 = builder.CreateAnd(W_85, W_86);
Value* W_65 = builder.CreateAnd(W_87, W_88);
Value* W_82 = builder.CreateAnd(W_83, W_84);
Value* W_80 = builder.CreateNot(W_82);
Value* W_79 = builder.CreateAnd(W_80, W_81);
Value* W_77 = builder.CreateNot(W_79);
Value* W_75 = builder.CreateAnd(W_77, W_78);
Value* W_73 = builder.CreateAnd(W_75, W_76);
Value* W_71 = builder.CreateAnd(W_73, W_74);
Value* W_69 = builder.CreateAnd(W_71, W_72);
Value* W_68 = builder.CreateAnd(W_69, W_70);
Value* W_66 = builder.CreateNot(W_68);
Value* W_64 = builder.CreateAnd(W_66, W_67);
Value* W_363 = builder.CreateAnd(W_64, W_65);
//Begin reassembly
Value * retV = builder.CreateZExtOrTrunc(W_363,builder.getIntNTy(1));
builder.CreateRet(retV);
}
