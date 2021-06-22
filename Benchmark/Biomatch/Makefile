NAME = biomatch

LLVM_OPT = ../../LLVM/llvm/build/bin/opt
LLVM_LIB = ../../LLVM/llvm/build/lib/LLVMGateify.so
IR2BR = ../../Converters/i2b.py

CBMC_GC = ../../HyCC/bin/cbmc-gc
CIRCUIT_UTILS = ../../HyCC/bin/circuit-utils

llvm:
	clang -O1 -Xclang -disable-llvm-passes -S -emit-llvm $(NAME).c
	opt-7 -mem2reg -simplifycfg -indvars -loops -lcssa -loop-simplify -loop-rotate -loop-unswitch -loop-unroll -unroll-count=2500 -simplifycfg -S $(NAME).ll > tmp.ll; mv tmp.ll $(NAME).ll
	$(LLVM_OPT) -S -load $(LLVM_LIB) -naiveGateify < $(NAME).ll > tmp.ll; mv tmp.ll $(NAME).ll
	$(LLVM_OPT) -S -load $(LLVM_LIB) -lfm < $(NAME).ll > tmp.ll; mv tmp.ll $(NAME).ll
	$(LLVM_OPT) -S -load $(LLVM_LIB) -cle < $(NAME).ll > tmp.ll; mv tmp.ll $(NAME).ll
	sed -n '/define i32 @MergedFunction/,$$p' $(NAME).ll > tmp.ll; mv tmp.ll $(NAME).ll
	python3 $(IR2BR) $(NAME).ll > /dev/null
	mv $(NAME).llMergedFunction.bristol $(NAME)_llvm.bristol
	rm -f $(NAME).ll*

hycc:
	$(CBMC_GC) $(NAME).c --minimization-time-limit 10
	$(CIRCUIT_UTILS) mpc_main.circ --as-bristol $(NAME)_hycc.bristol
	rm -f *.circ
	mv mpc_main.stats $(NAME)_hycc.stats

clean:
	rm -f *.circ *.stats *.bristol *.ll *.log
