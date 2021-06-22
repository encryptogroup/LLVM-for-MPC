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
    struct ConstantLogicElim : public FunctionPass {
        static char ID;

        ConstantLogicElim() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {

            if (!F.hasFnAttribute("Gate"))
                return true;

            errs() << "Eliminate or Precompute logic\n";
            for (auto I = F.front().begin(), Ie = F.front().end(); I != Ie; ++I) {
                switch (I->getOpcode()) {
                    case BinaryOperator::Xor:
                        if (isa<ConstantInt>(I->getOperand(0)) && isa<ConstantInt>(I->getOperand(1))) {
                            I->replaceAllUsesWith(ConstantInt::get(Type::getInt1Ty(I->getContext()),
                                                                   ((cast<ConstantInt>(
                                                                           I->getOperand(0))->getZExtValue()) ^
                                                                    (cast<ConstantInt>(
                                                                            I->getOperand(1))->getZExtValue()))));
                            I--;
                            I->getNextNode()->eraseFromParent();
                        }
                        if (auto cons1 = dyn_cast<ConstantInt>(I->getOperand(0))) {
                            //errs() << "Left operand of XOR is constant\n";
                            if (!cons1->getZExtValue()) {//constant is true
                                I->replaceAllUsesWith(I->getOperand(1));
                                //errs() << "replaced ";
                                //I->dump();
                                I--;
                                I->getNextNode()->eraseFromParent();
                                continue;
                            } else {//constant is true (inv)
                                //errs() << "kept ";
                                //I->dump();
                                continue;
                            }
                        }
                        if (auto cons2 = dyn_cast<ConstantInt>(I->getOperand(1))) {
                            //errs() << "Right operand of XOR is constant\n";
                            if (!cons2->getZExtValue()) {//constant is true
                                I->replaceAllUsesWith(I->getOperand(0));
                                //errs() << "replaced ";
                                //I->dump();
                                I--;
                                I->getNextNode()->eraseFromParent();
                                continue;
                            } else {//constant is true (inv)
                                //errs() << "kept ";
                                //I->dump();
                                continue;
                            }
                        }
                        if (I->getOperand(0) == I->getOperand(1)) {
                            I->replaceAllUsesWith(ConstantInt::get(Type::getInt1Ty(I->getContext()), 0));
                            I--;
                            I->getNextNode()->eraseFromParent();
                            continue;
                        }
                        continue;
                    case BinaryOperator::And:
                        if (auto cons1 = dyn_cast<ConstantInt>(I->getOperand(0))) {
                            //errs() << "Left operand of AND is constant\n";
                            //if (cons1->getSExtValue()) {//constant is true
                            //    I->replaceAllUsesWith(I->getOperand(1));
                            //} else {//constant is false
                            //    I->replaceAllUsesWith(I->getOperand(0));
                            //}
                            I->replaceAllUsesWith(I->getOperand(cons1->getZExtValue()));
                            //errs() << "replaced ";
                            //I->dump();
                            I--;
                            I->getNextNode()->eraseFromParent();
                            continue;
                        }
                        if (auto cons2 = dyn_cast<ConstantInt>(I->getOperand(1))) {
                            //errs() << "Right operand of AND is constant\n";
                            //if (cons2->getSExtValue()) {//constant is true
                            //    I->replaceAllUsesWith(I->getOperand(0));
                            //} else {//constant is false
                            //    I->replaceAllUsesWith(I->getOperand(1));
                            //}
                            I->replaceAllUsesWith(I->getOperand(1 - cons2->getZExtValue()));
                            //errs() << "replaced ";
                            //I->dump();
                            I--;
                            I->getNextNode()->eraseFromParent();
                            continue;
                        }
                        if (I->getOperand(0) == I->getOperand(1)) {
                            I->replaceAllUsesWith(I->getOperand(0));
                            I--;
                            I->getNextNode()->eraseFromParent();
                            continue;
                        }
                        if (isa<ConstantInt>(I->getOperand(0)) && isa<ConstantInt>(I->getOperand(1))) {
                            //I->dump();
                            I->replaceAllUsesWith(ConstantInt::get(Type::getInt1Ty(I->getContext()), (u_int64_t) (
                                    (cast<ConstantInt>(I->getOperand(0))->getZExtValue()) &
                                    (cast<ConstantInt>(I->getOperand(1))->getZExtValue()))));
                            I--;
                            I->getNextNode()->eraseFromParent();
                            continue;
                        }
                        continue;
                    case BinaryOperator::Or:
                        if (isa<ConstantInt>(I->getOperand(0)) && isa<ConstantInt>(I->getOperand(1))) {
                            //I->dump();
                            I->replaceAllUsesWith(ConstantInt::get(Type::getInt1Ty(I->getContext()),
                                                                   ((cast<ConstantInt>(
                                                                           I->getOperand(0))->getZExtValue()) |
                                                                    (cast<ConstantInt>(
                                                                            I->getOperand(1))->getZExtValue()))));
                            I--;
                            I->getNextNode()->eraseFromParent();
                        }
                        if (auto cons1 = dyn_cast<ConstantInt>(I->getOperand(0))) {
                            //errs() << "Left operand of OR is constant\n";
                            if (cons1->getZExtValue()) {//constant is true
                                I->replaceAllUsesWith(ConstantInt::get(Type::getInt1Ty(I->getContext()), 1));
                                //errs() << "replaced ";
                                //I->dump();
                                I--;
                                I->getNextNode()->eraseFromParent();
                                continue;
                            } else {//constant is false
                                //I->replaceAllUsesWith(ConstantInt::get(Type::getInt1Ty(I->getContext()), 1));
                                I->replaceAllUsesWith(I->getOperand(1));
                                I--;
                                I->getNextNode()->eraseFromParent();
                                continue;
                            }
                        }
                        if (auto cons2 = dyn_cast<ConstantInt>(I->getOperand(1))) {
                            //errs() << "Right operand of OR is constant\n";
                            if (cons2->getZExtValue()) {//constant is true
                                I->replaceAllUsesWith(ConstantInt::get(Type::getInt1Ty(I->getContext()), 1));
                                //errs() << "replaced ";
                                //I->dump();
                                I--;
                                I->getNextNode()->eraseFromParent();
                                continue;
                            } else {//constant is false
                                I->replaceAllUsesWith(I->getOperand(0));
                                I--;
                                I->getNextNode()->eraseFromParent();
                                continue;
                            }
                        }
                        if (I->getOperand(0) == I->getOperand(1)) {
                            I->replaceAllUsesWith(I->getOperand(0));
                            I--;
                            I->getNextNode()->eraseFromParent();
                            continue;
                        }
                        continue;
                    default :
                        continue;
                }
            }

            errs() << "Remove unnecessary Computations\n";
            int counter = 0;
            for (auto I = F.front().rbegin(), Ie = F.front().rend(); I != Ie; ++I) {
                if (I->getNumUses() == 0 && !isa<ReturnInst>(&*I)) {
                    I--;
                    I->getPrevNode()->eraseFromParent();
                    counter++;
                    continue;
                }
            }
            errs() << "Removed: " << counter << " instructions\n";
            return false;
        }


    }; // end of struct Constant logic elimination
}  // end of anonymous namespace

char ConstantLogicElim::ID = 0;
static RegisterPass<ConstantLogicElim> X("cle", "constant logic elimination",
                                         false /* Only looks at CFG */,
                                         false /* Analysis Pass */);
