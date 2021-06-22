#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Transforms/Utils/ValueMapper.h>
#include <llvm/Analysis/MemorySSA.h>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"

using namespace llvm;

namespace {
    struct ArrayToMultiplexer : public ModulePass {
        static char ID;

        ArrayToMultiplexer() : ModulePass(ID) {}

        using ValuePositionMap= std::map<int, Instruction *>;
        using ArrayAnalysisResult=std::map<AllocaInst *, std::vector<std::vector<Value *>>>;

        bool runOnModule(Module &M) override {
            errs() << "ArrayToMultiplexer:\n";
            for (Module::iterator F = M.begin(), Fe = M.end(); F != Fe; ++F) {
                //FIXME write correct function discriminator!!
                //errs() << "Looking at: " << F->getName() << "\n";

                if (F->isIntrinsic()) {
                    //errs() << F->getName() + " is intrinsic \n";
                    continue;
                }
                if (F->getName() == "main") {
                    //errs() << F->getName() + " needs to be skipped\n";
                    continue;
                }
                if (F->hasFnAttribute("Gate")) {
                    //errs() << F->getName() + " is already Gate Function\n";
                    continue;
                }
                if (F->getInstructionCount() == 0) {
                    //errs() << F->getName() << " has 0 instructions, skip\n";
                    continue;
                }

                //TODO: probably better as an required analysis pass!
                //      or do on the fly computation, as no future store information is needed
                //      migh also simplify program logic, as no "curren array search" would need to be performed
                //      the last entrys are always the current entrys!!
                auto arrayData = analyzeArrays(&*F);

                //for (const auto &allocs: arrayData) {
                    //errs() << "The array is:\n";
                    //allocs.first->dump();
                    //int i = 0;
                    //for (const auto &elems: allocs.second) {
                        //errs() << "Pos " << i << " updated:\n";
                        //for (auto versions: elems) {
                        //    versions->dump();
                        //}
                        //i++;
                    //}
                //}

                for (BasicBlock::iterator I = F->front().begin(), ie = F->front().end(); I != ie; ++I) {

                    if (auto *elemPtr = dyn_cast<GetElementPtrInst>(I)) {
                        elemPtr->dump();
                        assert(dyn_cast<ArrayType>(cast<AllocaInst>(elemPtr->getOperand(0))->getAllocatedType()));
                        if (elemPtr->getNumOperands() != 3) {
                            errs() << "Incompatible number of Operands at:\n";
                            elemPtr->dump();
                            assert(false);
                        }

                        if (auto storeIn = dyn_cast<StoreInst>(elemPtr->use_begin()->getUser())) {
                            continue;
                        } else if (auto loadIn = dyn_cast<LoadInst>(elemPtr->use_begin()->getUser())) {
                            if (auto constantPlace = dyn_cast<ConstantInt>(elemPtr->getOperand(2))) {
                                Value *v = nullptr;
                                for (auto elem: arrayData.at(cast<AllocaInst>(elemPtr->getOperand(0))).at(
                                        constantPlace->getSExtValue())) {
                                    if (elem < loadIn) {
                                        v = elem;
                                        continue;
                                    }
                                    break;
                                }

                                for (auto user: loadIn->users()) {
                                    user->dump();
                                    v->dump();
                                }
                                loadIn->replaceAllUsesWith(v);
                                //errs() << "<------------------------------>\n" <<
                                //       "This should not occur in normal use\n" <<
                                //       "<------------------------------>\n";
                            } else if (auto variablePlace = dyn_cast<Value>(elemPtr->getOperand(2))) {
                                //errs() << "Load from non Constant, look if position from constant signextend\n";
                                if (auto constantSext = dyn_cast<ConstantInt>(
                                        cast<Instruction>(elemPtr->getOperand(2))->getOperand(0))) {
                                    errs() << "Forward value directly\n";
                                    assert(false);
                                } else {
                                    //errs() << "Generate MX tree\n";
                                    CallInst *call = generateMXTree(loadIn, arrayData);
                                    call->insertBefore(loadIn);
                                    M.getFunctionList().push_back(call->getCalledFunction());
                                    loadIn->replaceAllUsesWith(call);
                                    assert(loadIn->getNumUses()==0);
                                    loadIn->eraseFromParent();
                                }
                            } else {
                                errs() << "Use was neither load nor store, abort\n";
                                assert(false);
                            }

                        }
                    }

                }
            }
            //ripNonGateFuncCalls(F);
            return false;
        }

        void getAnalysisUsage(AnalysisUsage &AU) const

        override {
            return;
        }

        ArrayAnalysisResult analyzeArrays(Function *F) {
            ArrayAnalysisResult result;
            if (F->getBasicBlockList().size() > 1) {
                errs() << "Fractured Function, compilation aborted, number of basicblocks: "
                       << F->getBasicBlockList().size();
                assert(false);
            }
            for (auto &I: F->front()) {
                switch (I.getOpcode()) {
                    case AllocaInst::Alloca:
                        assert(cast<AllocaInst>(I).getAllocatedType()->isArrayTy() &&
                               "Only arrays are supported right now");
                        //I.dump();
                        //errs() << "Array has "<< cast<ArrayType>(cast<AllocaInst>(I).getAllocatedType())->getNumElements() << " elements\n";
                        result[cast<AllocaInst>(&I)] = std::vector<std::vector<Value *>>(
                                cast<ArrayType>(cast<AllocaInst>(I).getAllocatedType())->getNumElements());
                        //errs() << "next\n";
                        continue;
                    case GetElementPtrInst::GetElementPtr:
                        //I.dump();
                        if (I.getNumUses() > 1) {
                            errs() << "GetElemPtr used more than once\n";
                            I.dump();
                            //can be combated by either:
                            //appending and then sorting update list BAD MOVE
                            //cloning the instruction and replace its use better move;
                            //cast<Instruction>(I.uses().begin()->get())->insertBefore(I.clone());

                            assert(false);
                        }

                        if (auto load = dyn_cast<LoadInst>(*I.users().begin())) {
                            continue;
                        } else if (auto store = dyn_cast<StoreInst>(*I.users().begin())) {
                            if (auto val = dyn_cast<ConstantInt>(I.getOperand(2))) {
                                result[cast<AllocaInst>(I.getOperand(0))][cast<ConstantInt>(
                                        I.getOperand(2))->getZExtValue()].push_back(
                                        I.users().begin()->getOperand(0));
                            } else if (auto val = dyn_cast<Value>(I.getOperand(2))) {
                                errs() << "Store to unknown pos, abort";
                                //insert update instruction for every array pos
                                //add all results to update list
                                assert(false);
                                /*errs() << "only save would b Sext has constant operand\n";
                                if(auto sextCall = dyn_cast<CallInst>(I.getOperand(2))){
                                    errs()<<"Ok, was SextCall, Operand Constant?\n";
                                    sextCall->getOperand(0)->dump();
                                }*/
                            } else {
                                errs() << "Third Operand of";
                                I.dump();
                                errs() << "was neither Constant nor Variable, but";
                                I.getOperand(2)->dump();
                                assert(false);
                            }

                        } else {
                            errs() << "User of ";
                            I.dump();
                            errs() << "was neither Store nor Load, but";
                            I.users().begin()->dump();
                        }
                }
                continue;
            }
            return result;
        }

        CallInst *generateMXTree(LoadInst *loadInst, ArrayAnalysisResult &arrayAnalysisResult) {
            auto *elementPtrInst = cast<GetElementPtrInst>(loadInst->getOperand(0));
            auto *allocaInst = cast<AllocaInst>(elementPtrInst->getOperand(0));
            std::vector<Value *> currentArray = getCurrentArray(arrayAnalysisResult[allocaInst], loadInst);


            int depth = std::ceil(std::log2( allocaInst->getAllocatedType()->getArrayNumElements()));

            std::vector<Type *> argAccumulator;
            argAccumulator.push_back(elementPtrInst->getOperand(2)->getType());
            for (auto elem :currentArray) {
                argAccumulator.push_back(elem->getType());
            }
            ArrayRef<Type *> args = argAccumulator;

            std::string fn = "Extern_multiplexer_32bitSize";
            fn.append(std::to_string(2 << depth));
            StringRef FunctionName = fn;
            //Constant *c = M.getOrInsertFunction(FunctionName, FunctionType::get(
            //        llvm::IntegerType::getInt32Ty(F->getContext()), args, false));
            //auto func = cast<Function>(c);
            Function *func = Function::Create(
                    FunctionType::get(loadInst->getType(), args, false),
                    llvm::GlobalValue::InternalLinkage, fn, nullptr);
            func->addFnAttr("Gate");
            func->arg_begin()->setName("DecisionBit");
            BasicBlock *block = BasicBlock::Create(func->getContext(), "entry", func);

            IRBuilder<> builder(block);

            ValueToValueMapTy VMap;

            //Value *DecBit[depth];
            auto DecBit = (Value **) malloc(64 * sizeof(Value **));
            for (
                    int j = 0;
                    j < 64; j++) {
                DecBit[j] = builder.CreateLShr(func->arg_begin(), u_int64_t(j),
                                               "Bit" + std::to_string(j) + "ofDecBit", true);
                DecBit[j] = builder.CreateTruncOrBitCast(DecBit[j],
                                                         Type::getInt1Ty(func->getContext()));
            }


            Value *ZOutput0to31bits[1 << depth][32];

            for (int i = depth - 1; i >= 0; i--) {
                Function::arg_iterator arg = func->arg_begin();
                for (int j = 0; j < (2 << i); j = j + 2) {

                    Value *XInput0to31bits[32];
                    Value *YInput0to31bits[32];

                    if (i == depth - 1) {//lowest line of multiplexer needs disassembled inputs
                        std::advance(arg, 1);
                        for (int k = 0; k < 32; k++) {
                            XInput0to31bits[k] = builder.CreateLShr(arg, u_int64_t(k),
                                                                    "Bit" +
                                                                    std::to_string(k) +
                                                                    "ofXMulti" +
                                                                    std::to_string(j) +
                                                                    "Layer" +
                                                                    std::to_string(i),
                                                                    true);
                            XInput0to31bits[k] = builder.CreateTruncOrBitCast(
                                    XInput0to31bits[k],
                                    Type::getInt1Ty(
                                            func->getContext()));
                        }

                        std::advance(arg, 1);
                        for (int k = 0; k < 32; k++) {
                            YInput0to31bits[k] = builder.CreateLShr(arg, u_int64_t(k),
                                                                    "Bit" +
                                                                    std::to_string(k) +
                                                                    "ofYMulti" +
                                                                    std::to_string(
                                                                            j + 1) +
                                                                    "Layer" +
                                                                    std::to_string(i),
                                                                    true);
                            YInput0to31bits[k] = builder.CreateTruncOrBitCast(
                                    YInput0to31bits[k],
                                    Type::getInt1Ty(
                                            func->getContext()));
                        }

                        builder.SetInsertPoint(builder.GetInsertBlock(),
                                               builder.GetInsertBlock()->end());

                        Value *W_65 = builder.CreateNot(DecBit[0]);

                        Value* NegatedDecBitAnds[32];
                        for(int k=0;k<32;k++){
                            NegatedDecBitAnds[k]=builder.CreateAnd(W_65, XInput0to31bits[k]);;
                        }
                        /*
                        Value *W_66 = builder.CreateAnd(W_65, XInput0to31bits[0]);
                        Value *W_67 = builder.CreateAnd(W_65, XInput0to31bits[1]);
                        Value *W_68 = builder.CreateAnd(W_65, XInput0to31bits[2]);
                        Value *W_69 = builder.CreateAnd(W_65, XInput0to31bits[3]);
                        Value *W_70 = builder.CreateAnd(W_65, XInput0to31bits[4]);
                        Value *W_71 = builder.CreateAnd(W_65, XInput0to31bits[5]);
                        Value *W_72 = builder.CreateAnd(W_65, XInput0to31bits[6]);
                        Value *W_73 = builder.CreateAnd(W_65, XInput0to31bits[7]);
                        Value *W_74 = builder.CreateAnd(W_65, XInput0to31bits[8]);
                        Value *W_75 = builder.CreateAnd(W_65, XInput0to31bits[9]);
                        Value *W_76 = builder.CreateAnd(W_65, XInput0to31bits[10]);
                        Value *W_77 = builder.CreateAnd(W_65, XInput0to31bits[11]);
                        Value *W_78 = builder.CreateAnd(W_65, XInput0to31bits[12]);
                        Value *W_79 = builder.CreateAnd(W_65, XInput0to31bits[13]);
                        Value *W_80 = builder.CreateAnd(W_65, XInput0to31bits[14]);
                        Value *W_81 = builder.CreateAnd(W_65, XInput0to31bits[15]);
                        Value *W_82 = builder.CreateAnd(W_65, XInput0to31bits[16]);
                        Value *W_83 = builder.CreateAnd(W_65, XInput0to31bits[17]);
                        Value *W_84 = builder.CreateAnd(W_65, XInput0to31bits[18]);
                        Value *W_85 = builder.CreateAnd(W_65, XInput0to31bits[19]);
                        Value *W_86 = builder.CreateAnd(W_65, XInput0to31bits[20]);
                        Value *W_87 = builder.CreateAnd(W_65, XInput0to31bits[21]);
                        Value *W_88 = builder.CreateAnd(W_65, XInput0to31bits[22]);
                        Value *W_89 = builder.CreateAnd(W_65, XInput0to31bits[23]);
                        Value *W_90 = builder.CreateAnd(W_65, XInput0to31bits[24]);
                        Value *W_91 = builder.CreateAnd(W_65, XInput0to31bits[25]);
                        Value *W_92 = builder.CreateAnd(W_65, XInput0to31bits[26]);
                        Value *W_93 = builder.CreateAnd(W_65, XInput0to31bits[27]);
                        Value *W_94 = builder.CreateAnd(W_65, XInput0to31bits[28]);
                        Value *W_95 = builder.CreateAnd(W_65, XInput0to31bits[29]);
                        Value *W_96 = builder.CreateAnd(W_65, XInput0to31bits[30]);
                        Value *W_97 = builder.CreateAnd(W_65, XInput0to31bits[31]);
                        */

                        Value* PositiveInputAnds[32];
                        for(int k=0;k<32;k++){
                            PositiveInputAnds[k]= builder.CreateAnd(DecBit[0], YInput0to31bits[k]);
                        }

                        /*
                        Value *W_98 = builder.CreateAnd(DecBit[0], YInput0to31bits[0]);
                        Value *W_99 = builder.CreateAnd(DecBit[0], YInput0to31bits[1]);
                        Value *W_100 = builder.CreateAnd(DecBit[0], YInput0to31bits[2]);
                        Value *W_101 = builder.CreateAnd(DecBit[0], YInput0to31bits[3]);
                        Value *W_102 = builder.CreateAnd(DecBit[0], YInput0to31bits[4]);
                        Value *W_103 = builder.CreateAnd(DecBit[0], YInput0to31bits[5]);
                        Value *W_104 = builder.CreateAnd(DecBit[0], YInput0to31bits[6]);
                        Value *W_105 = builder.CreateAnd(DecBit[0], YInput0to31bits[7]);
                        Value *W_106 = builder.CreateAnd(DecBit[0], YInput0to31bits[8]);
                        Value *W_107 = builder.CreateAnd(DecBit[0], YInput0to31bits[9]);
                        Value *W_108 = builder.CreateAnd(DecBit[0], YInput0to31bits[10]);
                        Value *W_109 = builder.CreateAnd(DecBit[0], YInput0to31bits[11]);
                        Value *W_110 = builder.CreateAnd(DecBit[0], YInput0to31bits[12]);
                        Value *W_111 = builder.CreateAnd(DecBit[0], YInput0to31bits[13]);
                        Value *W_112 = builder.CreateAnd(DecBit[0], YInput0to31bits[14]);
                        Value *W_113 = builder.CreateAnd(DecBit[0], YInput0to31bits[15]);
                        Value *W_114 = builder.CreateAnd(DecBit[0], YInput0to31bits[16]);
                        Value *W_115 = builder.CreateAnd(DecBit[0], YInput0to31bits[17]);
                        Value *W_116 = builder.CreateAnd(DecBit[0], YInput0to31bits[18]);
                        Value *W_117 = builder.CreateAnd(DecBit[0], YInput0to31bits[19]);
                        Value *W_118 = builder.CreateAnd(DecBit[0], YInput0to31bits[20]);
                        Value *W_119 = builder.CreateAnd(DecBit[0], YInput0to31bits[21]);
                        Value *W_120 = builder.CreateAnd(DecBit[0], YInput0to31bits[22]);
                        Value *W_121 = builder.CreateAnd(DecBit[0], YInput0to31bits[23]);
                        Value *W_122 = builder.CreateAnd(DecBit[0], YInput0to31bits[24]);
                        Value *W_123 = builder.CreateAnd(DecBit[0], YInput0to31bits[25]);
                        Value *W_124 = builder.CreateAnd(DecBit[0], YInput0to31bits[26]);
                        Value *W_125 = builder.CreateAnd(DecBit[0], YInput0to31bits[27]);
                        Value *W_126 = builder.CreateAnd(DecBit[0], YInput0to31bits[28]);
                        Value *W_127 = builder.CreateAnd(DecBit[0], YInput0to31bits[29]);
                        Value *W_128 = builder.CreateAnd(DecBit[0], YInput0to31bits[30]);
                        Value *W_129 = builder.CreateAnd(DecBit[0], YInput0to31bits[31]);
                         */

                        for(int k=0;k<32;k++){
                            ZOutput0to31bits[j / 2][k] = builder.CreateXor(NegatedDecBitAnds[k], PositiveInputAnds[k]);
                        }

                        /*
                        ZOutput0to31bits[j / 2][0] = builder.CreateXor(W_66, W_98);
                        ZOutput0to31bits[j / 2][1] = builder.CreateXor(W_67, W_99);
                        ZOutput0to31bits[j / 2][2] = builder.CreateXor(W_68, W_100);
                        ZOutput0to31bits[j / 2][3] = builder.CreateXor(W_69, W_101);
                        ZOutput0to31bits[j / 2][4] = builder.CreateXor(W_70, W_102);
                        ZOutput0to31bits[j / 2][5] = builder.CreateXor(W_71, W_103);
                        ZOutput0to31bits[j / 2][6] = builder.CreateXor(W_72, W_104);
                        ZOutput0to31bits[j / 2][7] = builder.CreateXor(W_73, W_105);
                        ZOutput0to31bits[j / 2][8] = builder.CreateXor(W_74, W_106);
                        ZOutput0to31bits[j / 2][9] = builder.CreateXor(W_75, W_107);
                        ZOutput0to31bits[j / 2][10] = builder.CreateXor(W_76, W_108);
                        ZOutput0to31bits[j / 2][11] = builder.CreateXor(W_77, W_109);
                        ZOutput0to31bits[j / 2][12] = builder.CreateXor(W_78, W_110);
                        ZOutput0to31bits[j / 2][13] = builder.CreateXor(W_79, W_111);
                        ZOutput0to31bits[j / 2][14] = builder.CreateXor(W_80, W_112);
                        ZOutput0to31bits[j / 2][15] = builder.CreateXor(W_81, W_113);
                        ZOutput0to31bits[j / 2][16] = builder.CreateXor(W_82, W_114);
                        ZOutput0to31bits[j / 2][17] = builder.CreateXor(W_83, W_115);
                        ZOutput0to31bits[j / 2][18] = builder.CreateXor(W_84, W_116);
                        ZOutput0to31bits[j / 2][19] = builder.CreateXor(W_85, W_117);
                        ZOutput0to31bits[j / 2][20] = builder.CreateXor(W_86, W_118);
                        ZOutput0to31bits[j / 2][21] = builder.CreateXor(W_87, W_119);
                        ZOutput0to31bits[j / 2][22] = builder.CreateXor(W_88, W_120);
                        ZOutput0to31bits[j / 2][23] = builder.CreateXor(W_89, W_121);
                        ZOutput0to31bits[j / 2][24] = builder.CreateXor(W_90, W_122);
                        ZOutput0to31bits[j / 2][25] = builder.CreateXor(W_91, W_123);
                        ZOutput0to31bits[j / 2][26] = builder.CreateXor(W_92, W_124);
                        ZOutput0to31bits[j / 2][27] = builder.CreateXor(W_93, W_125);
                        ZOutput0to31bits[j / 2][28] = builder.CreateXor(W_94, W_126);
                        ZOutput0to31bits[j / 2][29] = builder.CreateXor(W_95, W_127);
                        ZOutput0to31bits[j / 2][30] = builder.CreateXor(W_96, W_128);
                        ZOutput0to31bits[j / 2][31] = builder.CreateXor(W_97, W_129);
                         */

                        builder.SetInsertPoint(
                                dyn_cast<Instruction>(YInput0to31bits[31])->getNextNode());
                    } else {//from here on use Old Outputs as new inputs
                        builder.SetInsertPoint(builder.GetInsertBlock(),
                                               builder.GetInsertBlock()->end());
                        Value *W_65 = builder.CreateNot(DecBit[depth - i - 1]);

                        Value* NegatedDecBitAnds[32];
                        for(int k=0;k<32;k++){
                            NegatedDecBitAnds[k]=builder.CreateAnd(W_65, ZOutput0to31bits[j][k]);
                        }

                        /*
                        Value *W_66 = builder.CreateAnd(W_65, ZOutput0to31bits[j][0]);
                        Value *W_67 = builder.CreateAnd(W_65, ZOutput0to31bits[j][1]);
                        Value *W_68 = builder.CreateAnd(W_65, ZOutput0to31bits[j][2]);
                        Value *W_69 = builder.CreateAnd(W_65, ZOutput0to31bits[j][3]);
                        Value *W_70 = builder.CreateAnd(W_65, ZOutput0to31bits[j][4]);
                        Value *W_71 = builder.CreateAnd(W_65, ZOutput0to31bits[j][5]);
                        Value *W_72 = builder.CreateAnd(W_65, ZOutput0to31bits[j][6]);
                        Value *W_73 = builder.CreateAnd(W_65, ZOutput0to31bits[j][7]);
                        Value *W_74 = builder.CreateAnd(W_65, ZOutput0to31bits[j][8]);
                        Value *W_75 = builder.CreateAnd(W_65, ZOutput0to31bits[j][9]);
                        Value *W_76 = builder.CreateAnd(W_65, ZOutput0to31bits[j][10]);
                        Value *W_77 = builder.CreateAnd(W_65, ZOutput0to31bits[j][11]);
                        Value *W_78 = builder.CreateAnd(W_65, ZOutput0to31bits[j][12]);
                        Value *W_79 = builder.CreateAnd(W_65, ZOutput0to31bits[j][13]);
                        Value *W_80 = builder.CreateAnd(W_65, ZOutput0to31bits[j][14]);
                        Value *W_81 = builder.CreateAnd(W_65, ZOutput0to31bits[j][15]);
                        Value *W_82 = builder.CreateAnd(W_65, ZOutput0to31bits[j][16]);
                        Value *W_83 = builder.CreateAnd(W_65, ZOutput0to31bits[j][17]);
                        Value *W_84 = builder.CreateAnd(W_65, ZOutput0to31bits[j][18]);
                        Value *W_85 = builder.CreateAnd(W_65, ZOutput0to31bits[j][19]);
                        Value *W_86 = builder.CreateAnd(W_65, ZOutput0to31bits[j][20]);
                        Value *W_87 = builder.CreateAnd(W_65, ZOutput0to31bits[j][21]);
                        Value *W_88 = builder.CreateAnd(W_65, ZOutput0to31bits[j][22]);
                        Value *W_89 = builder.CreateAnd(W_65, ZOutput0to31bits[j][23]);
                        Value *W_90 = builder.CreateAnd(W_65, ZOutput0to31bits[j][24]);
                        Value *W_91 = builder.CreateAnd(W_65, ZOutput0to31bits[j][25]);
                        Value *W_92 = builder.CreateAnd(W_65, ZOutput0to31bits[j][26]);
                        Value *W_93 = builder.CreateAnd(W_65, ZOutput0to31bits[j][27]);
                        Value *W_94 = builder.CreateAnd(W_65, ZOutput0to31bits[j][28]);
                        Value *W_95 = builder.CreateAnd(W_65, ZOutput0to31bits[j][29]);
                        Value *W_96 = builder.CreateAnd(W_65, ZOutput0to31bits[j][30]);
                        Value *W_97 = builder.CreateAnd(W_65, ZOutput0to31bits[j][31]);
                        */

                        Value* PositveDecBitAnds[32];
                        for(int k=0;k<32;k++){
                            PositveDecBitAnds[k] = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][k]);
                        }

                        /*
                        Value *W_98 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][0]);
                        Value *W_99 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][1]);
                        Value *W_100 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][2]);
                        Value *W_101 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][3]);
                        Value *W_102 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][4]);
                        Value *W_103 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][5]);
                        Value *W_104 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][6]);
                        Value *W_105 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][7]);
                        Value *W_106 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][8]);
                        Value *W_107 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][9]);
                        Value *W_108 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][10]);
                        Value *W_109 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][11]);
                        Value *W_110 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][12]);
                        Value *W_111 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][13]);
                        Value *W_112 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][14]);
                        Value *W_113 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][15]);
                        Value *W_114 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][16]);
                        Value *W_115 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][17]);
                        Value *W_116 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][18]);
                        Value *W_117 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][19]);
                        Value *W_118 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][20]);
                        Value *W_119 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][21]);
                        Value *W_120 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][22]);
                        Value *W_121 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][23]);
                        Value *W_122 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][24]);
                        Value *W_123 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][25]);
                        Value *W_124 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][26]);
                        Value *W_125 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][27]);
                        Value *W_126 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][28]);
                        Value *W_127 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][29]);
                        Value *W_128 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][30]);
                        Value *W_129 = builder.CreateAnd(DecBit[depth - i - 1],ZOutput0to31bits[j + 1][31]);
                        */

                        for(int k=0; k < 32; k++){
                            ZOutput0to31bits[j / 2][k] = builder.CreateXor(NegatedDecBitAnds[k], PositveDecBitAnds[k]);
                        }
                        /*
                        ZOutput0to31bits[j / 2][0] = builder.CreateXor(W_66, W_98);
                        ZOutput0to31bits[j / 2][1] = builder.CreateXor(W_67, W_99);
                        ZOutput0to31bits[j / 2][2] = builder.CreateXor(W_68, W_100);
                        ZOutput0to31bits[j / 2][3] = builder.CreateXor(W_69, W_101);
                        ZOutput0to31bits[j / 2][4] = builder.CreateXor(W_70, W_102);
                        ZOutput0to31bits[j / 2][5] = builder.CreateXor(W_71, W_103);
                        ZOutput0to31bits[j / 2][6] = builder.CreateXor(W_72, W_104);
                        ZOutput0to31bits[j / 2][7] = builder.CreateXor(W_73, W_105);
                        ZOutput0to31bits[j / 2][8] = builder.CreateXor(W_74, W_106);
                        ZOutput0to31bits[j / 2][9] = builder.CreateXor(W_75, W_107);
                        ZOutput0to31bits[j / 2][10] = builder.CreateXor(W_76, W_108);
                        ZOutput0to31bits[j / 2][11] = builder.CreateXor(W_77, W_109);
                        ZOutput0to31bits[j / 2][12] = builder.CreateXor(W_78, W_110);
                        ZOutput0to31bits[j / 2][13] = builder.CreateXor(W_79, W_111);
                        ZOutput0to31bits[j / 2][14] = builder.CreateXor(W_80, W_112);
                        ZOutput0to31bits[j / 2][15] = builder.CreateXor(W_81, W_113);
                        ZOutput0to31bits[j / 2][16] = builder.CreateXor(W_82, W_114);
                        ZOutput0to31bits[j / 2][17] = builder.CreateXor(W_83, W_115);
                        ZOutput0to31bits[j / 2][18] = builder.CreateXor(W_84, W_116);
                        ZOutput0to31bits[j / 2][19] = builder.CreateXor(W_85, W_117);
                        ZOutput0to31bits[j / 2][20] = builder.CreateXor(W_86, W_118);
                        ZOutput0to31bits[j / 2][21] = builder.CreateXor(W_87, W_119);
                        ZOutput0to31bits[j / 2][22] = builder.CreateXor(W_88, W_120);
                        ZOutput0to31bits[j / 2][23] = builder.CreateXor(W_89, W_121);
                        ZOutput0to31bits[j / 2][24] = builder.CreateXor(W_90, W_122);
                        ZOutput0to31bits[j / 2][25] = builder.CreateXor(W_91, W_123);
                        ZOutput0to31bits[j / 2][26] = builder.CreateXor(W_92, W_124);
                        ZOutput0to31bits[j / 2][27] = builder.CreateXor(W_93, W_125);
                        ZOutput0to31bits[j / 2][28] = builder.CreateXor(W_94, W_126);
                        ZOutput0to31bits[j / 2][29] = builder.CreateXor(W_95, W_127);
                        ZOutput0to31bits[j / 2][20] = builder.CreateXor(W_96, W_128);
                        ZOutput0to31bits[j / 2][31] = builder.CreateXor(W_97, W_129);
                        */

                    }
                }
            }

            //Begin reassembly
            builder.SetInsertPoint(builder.GetInsertBlock(), builder.GetInsertBlock()->end());
            Value *retV = builder.CreateZExtOrTrunc(ZOutput0to31bits[0][0],
                                                    builder.getIntNTy(32));

            for(int i=1;i<32;i++){
                Value *temp = builder.CreateZExtOrTrunc(ZOutput0to31bits[0][i],
                                                        builder.getIntNTy(32));
                temp = builder.CreateShl(temp, i, "");
                retV = builder.CreateXor(retV, temp, "");
            }
            builder.CreateRet(retV);

            currentArray.insert(currentArray.begin(),elementPtrInst->getOperand(2));
            return CallInst::Create(func, currentArray, FunctionName);

        }

        std::vector<Value *>getCurrentArray(const std::vector<std::vector<Value *>> &arrayupdates, LoadInst *loadInst) {
            std::vector<Value *> res;
            for (auto &pos: arrayupdates) {
                Value *currentUpdate;
                for (auto update: pos) {
                    if (update < loadInst) {
                        currentUpdate = update;
                    } else { break; }
                }
                res.push_back(currentUpdate);
            }

            return res;
        }

    }; // end of struct ArrayOpt
}  // end of anonymous namespace

char ArrayToMultiplexer::ID = 0;
static RegisterPass<ArrayToMultiplexer> X("a2m", "Arrays to Multiplexer",
                                          false /* Only looks at CFG */,
                                          false /* Analysis Pass */);
