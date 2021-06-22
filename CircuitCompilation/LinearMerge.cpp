#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Analysis/ScalarEvolutionExpressions.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include <llvm/Transforms/Utils/ValueMapper.h>

using namespace llvm;

namespace {
    struct MergeGateFunctions : public ModulePass {
        static char ID;
        using ValueToValueMapTy = ValueMap<const Value *, WeakTrackingVH>;
        using IntermediateResultMap= std::map<Value *, std::vector<Value *>>;
        struct GateFunctionData {
            unsigned int disassembly = 0;
            unsigned int gates = 0;
            unsigned int reassembly = 0;
            unsigned int size = 0;
            std::vector<Value *> input;
            std::vector<Value *> output;
        };

        MergeGateFunctions() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            errs() << "MergeGateFunctions: \n\n";
            //M.dump();
            for (Module::iterator F = M.begin(), Fe = M.end(); F != Fe; ++F) {
                if (F->isIntrinsic()) {
                    //errs() << F->getName() + " is intrinsic \n";
                    continue;
                }
                if (F->getName() == "main") {
                    //errs() << F->getName() + " needs to be skipped\n";
                    continue;
                }
                if (F->hasFnAttribute("Gate")) {
                    //errs() << "Found gated function: " + F->getName() + "\n";
                    continue;
                }
                if (F->getInstructionCount() == 0) {
                    //errs() << "Found normal function: " + F->getName() + " with 0 instructions, continue \n";
                    continue;
                }

                ripAllNonGateInst(&*F);

                //errs() << "Found normal function: " + F->getName() + " with " << F->getInstructionCount()
                //       << " instructions \n";

                IntermediateResultMap IRMap;
                Function *MergedFunc = Function::Create(F->getFunctionType(), F->getLinkage(), "MergedFunction",
                                                        nullptr);

                BasicBlock *block = BasicBlock::Create(MergedFunc->getContext(), "entry", MergedFunc);
                IRBuilder<> builder(block);
                SmallVector<BasicBlock *, 1> SVIBP = {block};

                ValueToValueMapTy ArgumentMap;
                for (auto SourceArg = F->arg_begin(), TargetArg = MergedFunc->arg_begin();
                     SourceArg != F->arg_end() && TargetArg != MergedFunc->arg_end(); SourceArg++, TargetArg++) {
                    ArgumentMap[SourceArg] = TargetArg;
                }

                for (auto &arg:MergedFunc->args()) {
                    Value *InputBits[arg.getType()->getIntegerBitWidth()];
                    for (unsigned int i = 0; i < arg.getType()->getIntegerBitWidth(); i++) {
                        InputBits[i] = builder.CreateLShr(cast<Value>(&arg), u_int64_t(i),
                                                          "ShiftedTo" + std::to_string(i) + "BitX",
                                                          true);
                        IRMap[cast<Value>(&arg)].push_back(
                                builder.CreateTrunc(InputBits[i], Type::getInt1Ty(F->getContext())));
                    }
                }


                for (BasicBlock::iterator I = F->front().begin(), ie = F->front().end(); I != ie; I++) {
                    //I->dump();

                    if (&(*I) == &(F->front().back())) {
                        errs() << "Handle return:\n";
                        //I->dump();

                        unsigned int returnSize = MergedFunc->getReturnType()->getIntegerBitWidth();

                        auto retOutputBits = IRMap[I->getOperand(0)];

                        Value *retV = builder.CreateZExt(cast<Value>(&(*retOutputBits[0])),
                                                         builder.getIntNTy(returnSize));
                        for (unsigned int i = 1; i < returnSize; i++) {
                            Value *temp = builder.CreateZExt(cast<Value>(retOutputBits[i]),
                                                             builder.getIntNTy(returnSize));
                            temp = builder.CreateShl(temp, i, "");
                            retV = builder.CreateXor(retV, temp, "");
                        }

                        builder.CreateRet(cast<Value>(&MergedFunc->front().back()));

                        std::vector<Value *> newArgs;
                        for (auto &ee: F->args()) {
                            newArgs.push_back(cast<Value>(&ee));
                        }

                        MergedFunc->addFnAttr("Gate");
                        M.getFunctionList().push_back(MergedFunc);

                        CallInst *NewCall = CallInst::Create(MergedFunc, newArgs,
                                                             MergedFunc->getName(),
                                                             cast<Instruction>(I));
                        I->getPrevNode()->getPrevNode()->replaceAllUsesWith(NewCall);
                        errs()<<"Size|Dis|Gates|Ass\n";
                        auto tmp=getGateFunctionData(MergedFunc);
                        errs()<<tmp.size<<"|"<<tmp.disassembly<<"|"<<tmp.gates<<"|"<<tmp.reassembly;
                        //M.dump();
                        break;
                    }
                    //default case begins here

                    auto *callInst = dyn_cast<CallInst>(I);
                    auto data = getGateFunctionData(callInst);
                    auto gatePointer = callInst->getCalledFunction()->front().begin();


                    ValueToValueMapTy VMap;

                    std::advance(gatePointer, data.disassembly);
                    for (unsigned int i = data.gates; i > 0; i--, gatePointer++) {
                        Instruction *NI = gatePointer->clone();
                        NI = builder.Insert(NI, "");
                        VMap[&(*gatePointer)] = &(*NI);
                    }

                    std::vector<Value *> operandBits;
                    for (unsigned int i = 0; i < callInst->getNumOperands() - 1; i++) {
                        //callInst->getOperand(i)->dump();
                        if (auto constantOperand = dyn_cast<ConstantInt>(callInst->getOperand(i))) {
                            //errs() << "Operand " << i << " is a constant\n";
                            auto constantOpAsInt = constantOperand->getZExtValue();
                            //errs() << "Value is:  " << constantOpAsInt << "\n";
                            for (unsigned int j = 1u; j > 0; j = j << 1u) {
                                (constantOpAsInt & j) ? operandBits.push_back(ConstantInt::get(builder.getInt1Ty(), 1))
                                                      : operandBits.push_back(ConstantInt::get(builder.getInt1Ty(), 0));
                            }

                        } else if (ArgumentMap.count(callInst->getOperand(i)) > 0) {
                            //errs() << "Operand " << i << " is a primitive\n";
                            operandBits.insert(operandBits.end(), IRMap[ArgumentMap[callInst->getOperand(i)]].begin(),
                                               IRMap[ArgumentMap[callInst->getOperand(i)]].end());
                        } else {
                            //errs() << "Operand " << i << " is no primitive\n";
                            operandBits.insert(operandBits.end(), IRMap[callInst->getOperand(i)].begin(),
                                               IRMap[callInst->getOperand(i)].end());
                        }
                    }

                    assert(data.input.size()==operandBits.size());
                    for (unsigned long i = 0; i < operandBits.size(); i++) {
                        VMap[data.input.at(i)] = operandBits.at(i);
                    }

                    std::vector<Value *> mappedOutputs;
                    for (auto item : data.output) {
                        mappedOutputs.push_back(VMap[item]);
                    }

                    IRMap[callInst] = mappedOutputs;

                    remapInstructionsInBlocks(SVIBP, VMap);

                    //MergedFunc->dump();
                    //errs() << "here we go again\n";
                }
            }
            return false;
        }

        void getAnalysisUsage(AnalysisUsage &AU) const
        override {
        }

    private:
        static bool comp(Use &a, Use &b) {
            return a < b;
        }

        static GateFunctionData getGateFunctionData(CallInst *gateFunctionCall) {
            return getGateFunctionData(gateFunctionCall->getCalledFunction());
        }

        static GateFunctionData getGateFunctionData(Function *gateFunction) {
            GateFunctionData Data;
            Data.size = gateFunction->getInstructionCount();
            for (auto I = gateFunction->front().begin();;) {
                if (I->getOpcode() == Instruction::LShr) {
                    if (I->getNextNode()->getOpcode() == Instruction::Trunc) {
                        Data.disassembly += 2;
                        Data.input.push_back(dyn_cast<Value>(I->getNextNode()));
                        I++;
                        I++;
                        continue;
                    } else if (I->getNextNode()->getOpcode() == Instruction::LShr) {
                        Data.disassembly += 1;
                        Data.input.push_back(dyn_cast<Value>(I));
                        I++;
                        continue;
                    } else {
                        errs() << "wie bin ich hier hergekommen\n";
                        assert(false);
                    }
                }
                break;
            }

            if (gateFunction->getReturnType() == Type::getInt1Ty(gateFunction->getContext())) {
                Data.reassembly = 1;
                Data.output.push_back(dyn_cast<Value>(gateFunction->front().back().getOperand(0)));
            } else {
                for (auto I = gateFunction->front().rbegin(), Ie = gateFunction->front().rend(); I != Ie; ++I) {
                    if (I->getOpcode() == Instruction::ZExt) {
                        Data.reassembly++;
                        Data.output.insert(Data.output.begin(), dyn_cast<Value>(I->getOperand(0)));
                        if (I->getPrevNode()->getOpcode() == Instruction::ZExt) {
                            Data.reassembly++;
                            Data.output.insert(Data.output.begin(), I->getPrevNode()->getOperand(0));
                            break;
                        }
                        continue;
                    } else {
                        Data.reassembly++;
                        continue;
                    }
                }
            }


            Data.gates = Data.size - Data.disassembly - Data.reassembly;
            return Data;
        }

        void ripAllNonGateInst(Function *F) {
            if(F->getName()=="main" || F->getInstructionCount()==0){
                return;
            }

            for (auto I = F->front().rbegin(), Ie = --F->front().rend(); I != Ie;) {
                if (!((isa<CallInst>(&*I) && dyn_cast<CallInst>(&*I)->getCalledFunction()->hasFnAttribute("Gate")) ||
                      I->getOpcode() == ReturnInst::Ret)) {
                    assert(I->getNumUses() == 0);
                    I++;
                    I->getNextNode()->eraseFromParent();
                } else {
                    I++;
                }
            }
            if (!((isa<CallInst>(&F->front().front()) && dyn_cast<CallInst>(&F->front().front())->getCalledFunction()->hasFnAttribute("Gate")) ||
                F->front().front().getOpcode() == ReturnInst::Ret)) {
                F->front().front().eraseFromParent();
            }

        }
    }; // end of struct LinearMerge
}  // end of anonymous namespace

char MergeGateFunctions::ID = 0;
static RegisterPass<MergeGateFunctions> X("lfm", "Gate Function Merger",
                                          false /* Only looks at CFG */,
                                          false /* Analysis Pass */);
