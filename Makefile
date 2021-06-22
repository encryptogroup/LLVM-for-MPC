all: init compile benchmark

init:
	git submodule update --init --recursive
	cp -r CircuitCompilation LLVM/llvm/lib/Transforms/
	echo "add_subdirectory(CircuitCompilation)" >> LLVM/llvm/lib/Transforms/CMakeLists.txt
	
compile:
	mkdir -p LLVM/llvm/build
	cd LLVM/llvm/build; cmake .. -DLLVM_TARGETS_TO_BUILD="X86"; make -j
	cd HyCC; make minisat2-download; make

benchmark:
	cd Benchmark; make all
	