SHELL := /bin/bash

YOSYS_LIB = ../Converters/Yosys/Default/asic_cell_yosys_extended.lib
YOSYS_SCRIPT = ../Converters/Yosys/Default/script.abc

YOSYS_LIB_PQ = ../Converters/Yosys/PQ/asic_cell_yosys_extended.lib

B2V = ../Converters/b2v.java
V2B = ../Converters/v2b.py

all: clean hycc llvm pp pppq

hycc:
	cd Biomatch; time make hycc
	mv Biomatch/biomatch_hycc.bristol biomatch_hycc.bristol
	mv Biomatch/biomatch_hycc.stats biomatch_hycc.stats
	cd Dummy; time make hycc
	mv Dummy/dummy_hycc.bristol dummy_hycc.bristol
	mv Dummy/dummy_hycc.stats dummy_hycc.stats
	cd Euclid; time make hycc
	mv Euclid/euclid_hycc.bristol euclid_hycc.bristol
	mv Euclid/euclid_hycc.stats euclid_hycc.stats
	cd Gauss; time make hycc
	mv Gauss/gauss_hycc.bristol gauss_hycc.bristol
	mv Gauss/gauss_hycc.stats gauss_hycc.stats

llvm:
	cd Biomatch; time make llvm
	mv Biomatch/biomatch_llvm.bristol biomatch_llvm.bristol
	cd Dummy; time make llvm
	mv Dummy/dummy_llvm.bristol dummy_llvm.bristol
	cd Euclid; time make llvm
	mv Euclid/euclid_llvm.bristol euclid_llvm.bristol
	cd Gauss; time make llvm
	mv Gauss/gauss_llvm.bristol gauss_llvm.bristol
	for example in biomatch_llvm dummy_llvm euclid_llvm gauss_llvm ; do \
		java $(B2V) $$example ; \
		echo "# Circuit Stats" > yosys_script ; \
		echo "read_verilog $$example.v" >> yosys_script ; \
		echo "tee -a $$example.stats stat -liberty $(YOSYS_LIB)" >> yosys_script ; \
		yosys -s yosys_script ; \
	done
	rm -f yosys_script
	
pp:
	for example in biomatch_llvm dummy_llvm euclid_llvm gauss_llvm ; do \
		echo "# Circuit Post Processing" > yosys_script ; \
		echo "read_verilog $$example.v" >> yosys_script ; \
		echo "hierarchy -check -top $$example" >> yosys_script ; \
		echo "proc; fsm; flatten; opt;" >> yosys_script ; \
		echo "techmap; opt;" >> yosys_script ; \
		echo "dfflibmap -liberty $(YOSYS_LIB)" >> yosys_script ; \
		echo "abc -liberty $(YOSYS_LIB) -script $(YOSYS_SCRIPT)" >> yosys_script ; \
		echo "opt; clean;" >> yosys_script ; \
		echo "opt_clean -purge" >> yosys_script ; \
		echo "tee -a $$example.pp.stats stat -liberty $(YOSYS_LIB)" >> yosys_script ; \
		echo "write_verilog -noattr -noexpr $$example.pp.v" >> yosys_script ; \
		time yosys -s yosys_script ; \
		python3 $(V2B) -v $$example.pp.v $$example.pp.bristol ; \
	done
	rm -f yosys_script
	
pppq:
	for example in biomatch_llvm dummy_llvm euclid_llvm gauss_llvm ; do \
		echo "# Circuit PQ Post Processing" > yosys_script ; \
		echo "read_verilog $$example.v" >> yosys_script ; \
		echo "hierarchy -check -top $$example" >> yosys_script ; \
		echo "proc; fsm; flatten; opt;" >> yosys_script ; \
		echo "techmap; opt;" >> yosys_script ; \
		echo "dfflibmap -liberty $(YOSYS_LIB_PQ)" >> yosys_script ; \
		echo "abc -liberty $(YOSYS_LIB_PQ) -script $(YOSYS_SCRIPT)" >> yosys_script ; \
		echo "opt; clean;" >> yosys_script ; \
		echo "opt_clean -purge" >> yosys_script ; \
		echo "tee -a $$example.pppq.stats stat -liberty $(YOSYS_LIB_PQ)" >> yosys_script ; \
		echo "write_verilog -noattr -noexpr $$example.pppq.v" >> yosys_script ; \
		time yosys -s yosys_script ; \
		python3 $(V2B) -v $$example.pppq.v $$example.pppq.bristol ; \
	done
	rm -f yosys_script

clean:
	rm -f *.sv *.v *.bristol *.stats