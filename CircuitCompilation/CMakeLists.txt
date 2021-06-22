add_llvm_library( LLVMGateify MODULE BUILDTREE_ONLY
  PhiRemove.cpp
  NaiveGateify.cpp
  GateifyUtils.cpp
  LinearMerge.cpp
  ArrayOpt.cpp
  ConstantLogicElim.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
