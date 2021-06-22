#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Transforms/Utils/ValueMapper.h>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include <llvm/Transforms/Utils/Cloning.h>

using namespace llvm;

namespace {
    struct PhiRemove : public FunctionPass {
        static char ID;

        PhiRemove() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            BasicBlock &entryBlock = F.front();
            errs() << "Current function is: " << F.getName() << "\n";
            recursiveDecend(&entryBlock);
            /* wenn die funktion rekrusivedecend aufgerufen wird,
             * dann guck ob die letzte instruction ein branch ist..
             * wenn das ein branch ist, dann guck ob es ein zwei wege oder ein wege branch ist
             *   -gibt es mehr als zwei wege branches?
             *      wenn es ein zwei wege branch ist rufe auf inlinetwowaybranch
             *      die guck ob die letzte instruction beider branches der gleiche basicblock ist
             *          wenn dem so ist dann kannst du die inlinen
             *          wenn nicht dann ruft du rekursiv recursive descend mit dem derzeitigen block auf
             *      wenn es ein direkter sprung ist,
             *          dann ruf inlineOneWayBranch auf
             *          guck ob die erste instruction im neuen branch eine phi node ist
             *              wenn ja,
             *                  phi node translate beide blocks,
             *                  generiere select instruction mit condition
             *                      -woher bekomme ich die?
             *              wenn nein
             *                  WTF warum tun wir das eigentlich
             *          guck ob letzte instructions branch ist, und rufe rekursivedescend wieder auf
             */
            return false;
        }

        static void recursiveDecend(BasicBlock *Bb) {
            errs() << "RekDesc\n";
            if (auto branch = dyn_cast<BranchInst>(&Bb->back())) {
                if (branch->getNumSuccessors() == 1) {
                    inlineOneWayBranch(Bb, branch->getSuccessor(0));
                } else if (branch->getNumSuccessors() == 2) {
                    inlineTwoWayBranch(Bb, branch->getSuccessor(0), branch->getSuccessor(1));
                } else {
                    errs() << "Strange Branch: \n";
                    branch->dump();
                    Bb->dump();
                }
            }

        }

        static void inlineTwoWayBranch(BasicBlock *entry, BasicBlock *Bb1, BasicBlock *Bb2) {
            errs() << "ITWB\n";
            if (auto branch1 = dyn_cast<BranchInst>(&Bb1->back())) {
                if (auto branch2 = dyn_cast<BranchInst>(&Bb2->back())) {
                    errs() << "Branch1: \n";
                    branch1->dump();
                    errs() << "Branch2: \n";
                    branch2->dump();
                    if (branch1->getNumSuccessors() == 1 && branch2->getNumSuccessors() == 1 &&
                        branch1->getSuccessor(0) == branch2->getSuccessor(0)) {
                        errs() << "Branches branch to same block\n";
                        auto *branchInst = dyn_cast<BranchInst>(&entry->back());
                        entry->getInstList().splice(entry->getInstList().end(), Bb1->getInstList(),
                                                    Bb1->getInstList().begin(), --Bb1->getInstList().end());

                        entry->getInstList().splice(entry->getInstList().end(), Bb2->getInstList(),
                                                    Bb2->getInstList().begin(), --Bb2->getInstList().end());

                        entry->dump();
                        branchInst->getCondition();

                        while (isa<PHINode>(branch1->getSuccessor(0)->front())) {
                            Instruction *a = &branch1->getSuccessor(0)->front();
                            SelectInst::Create(branchInst->getCondition(),
                                               a->DoPHITranslation(branch1->getSuccessor(0), Bb1),
                                               a->DoPHITranslation(branch2->getSuccessor(0), Bb2), "",
                                               branch1->getSuccessor(0)->getFirstNonPHIOrDbgOrLifetime());
                            branch1->getSuccessor(0)->front().replaceAllUsesWith(
                                    branch1->getSuccessor(0)->front().getNextNode());
                            branch1->getSuccessor(0)->getInstList().pop_front();
                        }
                        entry->getInstList().splice(entry->getInstList().end(),
                                                    branch1->getSuccessor(0)->getInstList(),
                                                    branch1->getSuccessor(0)->getInstList().begin(),
                                                    branch1->getSuccessor(0)->getInstList().end());

                        auto safe = branch1->getSuccessor(0);
                        branchInst->eraseFromParent();
                        Bb1->eraseFromParent();
                        Bb2->eraseFromParent();
                        safe->eraseFromParent();
                        recursiveDecend(entry);
                        //} else {
                        /*ab hier muss der ersatz fall rein, welcher block hat 2 succs ?
                                     * recursive inline
                                     * rekursiveDescend(BranchMit2Succs)
                                     * */
                        //}
                    }
                }
            }
        }

        static void inlineOneWayBranch(BasicBlock *entry, BasicBlock *Bb) {
            errs() << "IOWB\n";

            if (isa<PHINode>(&Bb->back())) {
                errs() << "Was Phi node!\n";
            } else {
                errs() << "Should not happen?\n";
            }
        }

    };
}  // end of anonymous namespace

char PhiRemove::ID = 0;
static RegisterPass<PhiRemove> X("phiRemove", "Swap Phi for Select",
                                 false /* Only looks at CFG */,
                                 false /* Analysis Pass */);
