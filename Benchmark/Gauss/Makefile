NAME_LLVM = gauss_llvm
LLVM_OPT = ../../LLVM/llvm/build/bin/opt
LLVM_LIB = ../../LLVM/llvm/build/lib/LLVMGateify.so
IR2BR = ../../Converters/i2b.py

NAME_HYCC = gauss_hycc
CBMC_GC = ../../HyCC/bin/cbmc-gc
CIRCUIT_UTILS = ../../HyCC/bin/circuit-utils

llvm:
	clang -O1 -Xclang -disable-llvm-passes -S -emit-llvm $(NAME_LLVM).cpp
	opt-7 -O3 -S -disable-slp-vectorization -disable-inlining $(NAME_LLVM).ll > tmp.ll; mv tmp.ll $(NAME_LLVM).ll
	$(LLVM_OPT) -S -load $(LLVM_LIB) -phiRemove < $(NAME_LLVM).patch > tmp.ll; mv tmp.ll $(NAME_LLVM).ll
	$(LLVM_OPT) -S -load $(LLVM_LIB) -naiveGateify < $(NAME_LLVM).ll > tmp.ll; mv tmp.ll $(NAME_LLVM).ll
	$(LLVM_OPT) -S -load $(LLVM_LIB) -a2m < $(NAME_LLVM).ll > tmp.ll; mv tmp.ll $(NAME_LLVM).ll
	$(LLVM_OPT) -S -load $(LLVM_LIB) -lfm < $(NAME_LLVM).ll > tmp.ll; mv tmp.ll $(NAME_LLVM).ll
	$(LLVM_OPT) -S -load $(LLVM_LIB) -cle < $(NAME_LLVM).ll > tmp.ll; mv tmp.ll $(NAME_LLVM).ll
	sed -n '/define i32 @MergedFunction/,$$p' $(NAME_LLVM).ll > tmp.ll; mv tmp.ll $(NAME_LLVM).ll
	python3 $(IR2BR) $(NAME_LLVM).ll > /dev/null
	mv $(NAME_LLVM).llMergedFunction.bristol $(NAME_LLVM).bristol
	rm -f $(NAME_LLVM).ll*

hycc:
	$(CBMC_GC) $(NAME_HYCC).c --minimization-time-limit 10
	$(CIRCUIT_UTILS) mpc_main.circ --as-bristol $(NAME_HYCC).bristol
	rm -f *.circ
	mv mpc_main.stats $(NAME_HYCC).stats

clean:
	rm -f *.circ *.stats *.bristol *.ll *.log
