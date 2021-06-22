#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
    struct NaiveGateify : public ModulePass {
        static char ID;

        NaiveGateify() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            errs() << "NaiveGateify: \n\n";
            for (Module::iterator F = M.begin(), Fe = M.end(); F != Fe; ++F) {
                if (F->isIntrinsic()) {
                    //errs() << F->getName() + " is intrinsic \n";
                    continue;
                }
                if(F->getName()=="main"){
                    errs() << F->getName() + " needs to be skipped\n";
                    continue;
                }
                if (F->hasFnAttribute("Gate")) {
                    //errs() << F->getName() + " is already Gate Function\n";
                    continue;
                }
                for (Function::iterator b = F->begin(), be = F->end(); b != be; ++b) {
                    for (BasicBlock::iterator i = b->begin(), ie = b->end(); i != ie; ++i) {
                        //fixme LLVM supports vector operands, I don't, maybe check for vector operand
                        if (auto *cmpInst = dyn_cast<CmpInst>(i))   {
                                //errs() << "Found Compare Instruction: \n";
                                //i->dump();

                                if (cmpInst->getOpcode() == llvm::CmpInst::ICmp &&
                                    cmpInst->getPredicate() == llvm::CmpInst::ICMP_EQ) {
                                    //include part handles everything, returns the new function, named "func"
                                    #include <comparator_32bit_eq_conv.h>
                                    auto callIns = llvm::CallInst::Create(func,
                                                                          {cmpInst->getOperand(0),
                                                                           cmpInst->getOperand(1)},
                                                                          FunctionName, cmpInst->getNextNode());

                                    cmpInst->replaceAllUsesWith(callIns);
                                    cmpInst->dropAllReferences();
                                    i++;
                                    cmpInst->removeFromParent();
                                }
                                if (cmpInst->getOpcode() == llvm::CmpInst::ICmp &&
                                    cmpInst->getPredicate() == llvm::CmpInst::ICMP_NE) {
                                    #include <comparator_32bit_ne_conv.h>
                                    //compare->viewCFG();
                                    auto callIns = llvm::CallInst::Create(func,
                                                                          {cmpInst->getOperand(0),
                                                                           cmpInst->getOperand(1)},
                                                                          FunctionName, cmpInst->getNextNode());

                                    cmpInst->replaceAllUsesWith(callIns);
                                    cmpInst->dropAllReferences();
                                    i++;
                                    cmpInst->removeFromParent();
                                }

                                if (cmpInst->getOpcode() == llvm::CmpInst::ICmp &&
                                    cmpInst->getPredicate() == llvm::CmpInst::ICMP_SLT) {

                                    #include <comparator_32bit_signed_lt_conv.h>
                                    auto callIns = llvm::CallInst::Create(func,
                                                                          {cmpInst->getOperand(0),
                                                                           cmpInst->getOperand(1)},
                                                                          FunctionName, cmpInst->getNextNode());

                                    cmpInst->replaceAllUsesWith(callIns);
                                    cmpInst->dropAllReferences();
                                    i++;
                                    cmpInst->removeFromParent();
                                }
                                if (cmpInst->getOpcode() == llvm::CmpInst::ICmp &&
                                    cmpInst->getPredicate() == llvm::CmpInst::ICMP_SLE) {

                                    #include <comparator_32bit_signed_lteq_conv.h>

                                    auto callIns = llvm::CallInst::Create(func,
                                                                          {cmpInst->getOperand(0),
                                                                           cmpInst->getOperand(1)},
                                                                          FunctionName, cmpInst->getNextNode());

                                    cmpInst->replaceAllUsesWith(callIns);
                                    cmpInst->dropAllReferences();
                                    i++;
                                    cmpInst->removeFromParent();
                                }
                                if (cmpInst->getOpcode() == llvm::CmpInst::ICmp &&
                                    cmpInst->getPredicate() == llvm::CmpInst::ICMP_ULT) {

                                    #include <comparator_32bit_unsigned_lt_conv.h>

                                    auto callIns = llvm::CallInst::Create(func,
                                                                          {cmpInst->getOperand(0),
                                                                           cmpInst->getOperand(1)},
                                                                          FunctionName, cmpInst->getNextNode());

                                    cmpInst->replaceAllUsesWith(callIns);
                                    cmpInst->dropAllReferences();
                                    i++;
                                    cmpInst->removeFromParent();
                                }
                                if (cmpInst->getOpcode() == llvm::CmpInst::ICmp &&
                                    cmpInst->getPredicate() == llvm::CmpInst::ICMP_ULE) {
                                    #include <comparator_32bit_signed_lteq_conv.h>

                                    auto callIns = llvm::CallInst::Create(func,
                                                                          {cmpInst->getOperand(0),
                                                                           cmpInst->getOperand(1)},
                                                                          FunctionName, cmpInst->getNextNode());

                                    cmpInst->replaceAllUsesWith(callIns);
                                    cmpInst->dropAllReferences();
                                    i++;
                                    cmpInst->removeFromParent();
                                }

                                if (cmpInst->getOpcode() == llvm::CmpInst::ICmp &&
                                    cmpInst->getPredicate() == llvm::CmpInst::ICMP_SGT) {
                                    #include <comparator_32bit_signed_lt_conv.h>

                                    auto callIns = llvm::CallInst::Create(func,
                                                                          {cmpInst->getOperand(1),
                                                                           cmpInst->getOperand(0)},
                                                                          FunctionName, cmpInst->getNextNode());

                                    cmpInst->replaceAllUsesWith(callIns);
                                    cmpInst->dropAllReferences();
                                    i++;
                                    cmpInst->removeFromParent();
                                }
                                if (cmpInst->getOpcode() == llvm::CmpInst::ICmp &&
                                    cmpInst->getPredicate() == llvm::CmpInst::ICMP_SGE) {
                                    #include <comparator_32bit_signed_lteq_conv.h>

                                    auto callIns = llvm::CallInst::Create(func,
                                                                          {cmpInst->getOperand(1),
                                                                           cmpInst->getOperand(0)},
                                    FunctionName, cmpInst->getNextNode());

                                    cmpInst->replaceAllUsesWith(callIns);
                                    cmpInst->dropAllReferences();
                                    i++;
                                    cmpInst->removeFromParent();
                                }
                                if (cmpInst->getOpcode() == llvm::CmpInst::ICmp &&
                                    cmpInst->getPredicate() == llvm::CmpInst::ICMP_UGT) {
                                    #include <comparator_32bit_unsigned_lt_conv.h>

                                    auto callIns = llvm::CallInst::Create(func,
                                                                          {cmpInst->getOperand(1),
                                                                           cmpInst->getOperand(0)},
                                                                          FunctionName, cmpInst->getNextNode());

                                    cmpInst->replaceAllUsesWith(callIns);
                                    cmpInst->dropAllReferences();
                                    i++;
                                    cmpInst->removeFromParent();
                                }
                                if (cmpInst->getOpcode() == llvm::CmpInst::ICmp &&
                                    cmpInst->getPredicate() == llvm::CmpInst::ICMP_UGE) {

                                    #include <comparator_32bit_signed_lteq_conv.h>

                                    auto callIns = llvm::CallInst::Create(func,
                                                                          {cmpInst->getOperand(1),
                                                                           cmpInst->getOperand(0)},
                                                                          FunctionName, cmpInst->getNextNode());


                                    cmpInst->replaceAllUsesWith(callIns);
                                    cmpInst->dropAllReferences();
                                    i++;
                                    cmpInst->removeFromParent();
                                }
                            }
                        if (auto *binOp = dyn_cast<BinaryOperator>(i)) {
                            if (binOp->getOpcode() == llvm::BinaryOperator::BinaryOps::Add) {
                                //include part handles everything, returns the new function, named "func"
                                #include <int_add32_size_conv.h>
                                auto callIns = llvm::CallInst::Create(func,
                                                                      {binOp->getOperand(0), binOp->getOperand(1)},
                                                                      FunctionName, binOp->getNextNode());

                                binOp->replaceAllUsesWith(callIns);
                                binOp->dropAllReferences();
                                i++;
                                binOp->removeFromParent();
                            }
                            if (binOp->getOpcode() == llvm::BinaryOperator::BinaryOps::Sub) {

                                #include <int_sub32_size_conv.h>
                                auto callIns = llvm::CallInst::Create(func,
                                                                      {binOp->getOperand(0), binOp->getOperand(1)},
                                                                      FunctionName, binOp->getNextNode());

                                binOp->replaceAllUsesWith(callIns);
                                binOp->dropAllReferences();
                                i++;
                                binOp->removeFromParent();
                            }
                            if (binOp->getOpcode() == llvm::BinaryOperator::BinaryOps::Mul) {
                                #include <int_mul32_size_conv.h>
                                auto callIns = llvm::CallInst::Create(func,
                                                                      {binOp->getOperand(0), binOp->getOperand(1)},
                                                                      FunctionName, binOp->getNextNode());

                                binOp->replaceAllUsesWith(callIns);
                                binOp->dropAllReferences();
                                i++;
                                binOp->removeFromParent();
                            }
                            if (binOp->getOpcode() == llvm::BinaryOperator::BinaryOps::SDiv || binOp->getOpcode() == llvm::BinaryOperator::BinaryOps::UDiv) {

                                #include <int_div32_size_conv.h>

                                auto callIns = llvm::CallInst::Create(func,
                                                                      {binOp->getOperand(0), binOp->getOperand(1)},
                                                                      FunctionName, binOp->getNextNode());

                                binOp->replaceAllUsesWith(callIns);
                                binOp->dropAllReferences();
                                i++;
                                binOp->removeFromParent();
                            }


                            if (binOp->getOpcode() == llvm::BinaryOperator::BinaryOps::Xor) {
                                #include <binary_32bit_xor_conv.h>
                                auto callIns = llvm::CallInst::Create(func,
                                                                      {binOp->getOperand(0), binOp->getOperand(1)},
                                                                      FunctionName, binOp->getNextNode());

                                binOp->replaceAllUsesWith(callIns);
                                binOp->dropAllReferences();
                                i++;
                                binOp->removeFromParent();
                            }
                            if (binOp->getOpcode() == llvm::BinaryOperator::BinaryOps::And) {
                                #include <binary_32bit_and_conv.h>
                                auto callIns = llvm::CallInst::Create(func,
                                                                      {binOp->getOperand(0), binOp->getOperand(1)},
                                                                      FunctionName, binOp->getNextNode());

                                binOp->replaceAllUsesWith(callIns);
                                binOp->dropAllReferences();
                                i++;
                                binOp->removeFromParent();
                            }

                            if (binOp->getOpcode() == llvm::BinaryOperator::BinaryOps::Shl) {
                                errs()<<"Found shift, currently only constant shifts are supported via multiplication \n";
                                if(!isa<ConstantInt>(binOp->getOperand(1))){ errs() << "Found shift with variable size \n"; continue;}
                                auto shiftamount = cast<ConstantInt>(binOp->getOperand(1));
                                binOp->setOperand(1,llvm::ConstantInt::get(binOp->getContext(), llvm::APInt(32, shiftamount->getSExtValue()<<1, false)));
                                #include <int_mul32_size_conv.h>
                                auto callIns = llvm::CallInst::Create(func,
                                                                      {binOp->getOperand(0), binOp->getOperand(1)},
                                                                      FunctionName, binOp->getNextNode());
                                binOp->replaceAllUsesWith(callIns);
                                binOp->dropAllReferences();
                                i++;
                                binOp->removeFromParent();
                            }
                        }

                        //works in a pinch, does not actually copy sign to new highest bit,
                        if(auto *signExtend=dyn_cast<SExtInst>(i)){
                            if(signExtend->getOperand(0)->getType()!=Type::getInt32Ty(F->getContext()) || signExtend->getType()!=Type::getInt64Ty(F->getContext()))
                                continue;
                            #include <ZExt32to64_conv.h>

                            auto callIns = llvm::CallInst::Create(func,
                                                                  {signExtend->getOperand(0)},
                                                                  FunctionName, signExtend->getNextNode());

                            signExtend->replaceAllUsesWith(callIns);
                            signExtend->dropAllReferences();
                            i++;
                            signExtend->removeFromParent();
                        }

                        if(auto *zExtend=dyn_cast<ZExtInst>(i)){
                            if(zExtend->getOperand(0)->getType()!=Type::getInt32Ty(F->getContext()) || zExtend->getType()!=Type::getInt64Ty(F->getContext()))
                                continue;

                            #include <ZExt32to64_conv.h>

                            auto callIns = llvm::CallInst::Create(func,
                                                                  {zExtend->getOperand(0)},
                                                                  FunctionName, zExtend->getNextNode());

                            zExtend->replaceAllUsesWith(callIns);
                            zExtend->dropAllReferences();
                            i++;
                            zExtend->removeFromParent();
                        }


                        /*if (auto *callInst = dyn_cast<CallInst>(i)){
                            if(callInst->getCalledFunction()!=nullptr && callInst->getCalledFunction()->isIntrinsic() &&
                               callInst->getCalledFunction()->hasName() && callInst->getCalledFunction()->getName().compare("llvm.sadd.with.overflow")){
                                //errs()<<"ALS OB\n";
                            }
                        }*/
                        if (auto *selectInst = dyn_cast<SelectInst>(i)){
                            #include <multiplexer_32bit_conv.h>
                            //func->dump();
                            //selectInst->getOperand(0)->dump();
                            //selectInst->getOperand(1)->dump();
                            //selectInst->getOperand(2)->dump();
                            auto callIns = llvm::CallInst::Create(func,
                                                                  {selectInst->getOperand(0), selectInst->getOperand(1),selectInst->getOperand(2)},
                                                                  FunctionName, selectInst->getNextNode());

                            selectInst->replaceAllUsesWith(callIns);
                            selectInst->dropAllReferences();
                            i++;
                            selectInst->removeFromParent();
                        }
                    }

                }
            }
            return true;
        }

        void getAnalysisUsage(AnalysisUsage &AU) const override {
            return;
        }
    }; // end of struct NaiveGateify
}  // end of anonymous namespace

char NaiveGateify::ID = 0;
static RegisterPass<NaiveGateify> X("naiveGateify", "Gateifier",
                               false /* Only looks at CFG */,
                               false /* Analysis Pass */);
