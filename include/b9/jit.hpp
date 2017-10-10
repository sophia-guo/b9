#ifndef B9_JIT_INCL
#define B9_JIT_INCL

#include <b9.hpp>
#include <b9/core.hpp>

#include "ilgen/MethodBuilder.hpp"
#include "ilgen/TypeDictionary.hpp"

namespace b9 {

class FunctionSpec;
class JitConfig;

class MethodBuilder : public TR::MethodBuilder {
 public:
  MethodBuilder(TR::TypeDictionary *types, VirtualMachine *virtualMachine, const JitConfig & config);

  virtual bool buildIL();

 protected:
  TR::IlType *stackType;
  TR::IlType *stackPointerType;
  TR::IlType *stackElementType;
  TR::IlType *stackElementPointerType;
  TR::IlType *int64PointerType;
  TR::IlType *int32PointerType;
  TR::IlType *int16PointerType;

 private:
  VirtualMachine *virtualMachine_;
  int32_t topLevelProgramIndex;
  int32_t maxInlineDepth;
  int32_t firstArgumentIndex;

  void defineFunctions();
  void defineStructures(TR::TypeDictionary *types);
  void defineLocals(Instruction *program);
  void defineParameters(Instruction *program);

  void createBuilderForBytecode(TR::BytecodeBuilder **bytecodeBuilderTable,
                                uint8_t bytecode, int64_t bytecodeIndex);
  bool generateILForBytecode(
      TR::BytecodeBuilder **bytecodeBuilderTable, Instruction *program,
      uint8_t bytecode, long bytecodeIndex,
      TR::BytecodeBuilder *jumpToBuilderForInlinedReturn);

  bool inlineProgramIntoBuilder(
      int32_t programIndex, bool isTopLevel,
      TR::BytecodeBuilder *currentBuilder = 0,
      TR::BytecodeBuilder *jumpToBuilderForInlinedReturn = 0);

  TR::IlValue *pop(TR::BytecodeBuilder *builder);
  void push(TR::BytecodeBuilder *builder, TR::IlValue *value);
  void drop(TR::BytecodeBuilder *builder);

  TR::IlValue *loadVarIndex(TR::BytecodeBuilder *builder, int varindex);
  void storeVarIndex(TR::BytecodeBuilder *builder, int varindex,
                     TR::IlValue *value);

  void handle_bc_push_constant(TR::BytecodeBuilder *builder,
                               TR::BytecodeBuilder *nextBuilder);
  void handle_bc_push_string(TR::BytecodeBuilder *builder,
                             TR::BytecodeBuilder *nextBuilder);
  void handle_bc_drop(TR::BytecodeBuilder *builder,
                      TR::BytecodeBuilder *nextBuilder);
  void handle_bc_push_from_var(TR::BytecodeBuilder *builder,
                               TR::BytecodeBuilder *nextBuilder);
  void handle_bc_pop_into_var(TR::BytecodeBuilder *builder,
                              TR::BytecodeBuilder *nextBuilder);
  void handle_bc_sub(TR::BytecodeBuilder *builder,
                     TR::BytecodeBuilder *nextBuilder);
  void handle_bc_add(TR::BytecodeBuilder *builder,
                     TR::BytecodeBuilder *nextBuilder);
  void handle_bc_call(TR::BytecodeBuilder *builder,
                      TR::BytecodeBuilder *nextBuilder);
  void handle_bc_jmp(TR::BytecodeBuilder *builder,
                     TR::BytecodeBuilder **bytecodeBuilderTable,
                     Instruction *program, long bytecodeIndex);
  void handle_bc_jmp_eq(TR::BytecodeBuilder *builder,
                        TR::BytecodeBuilder **bytecodeBuilderTable,
                        Instruction *program, long bytecodeIndex,
                        TR::BytecodeBuilder *nextBuilder);
  void handle_bc_jmp_neq(TR::BytecodeBuilder *builder,
                         TR::BytecodeBuilder **bytecodeBuilderTable,
                         Instruction *program, long bytecodeIndex,
                         TR::BytecodeBuilder *nextBuilder);
  void handle_bc_jmp_lt(TR::BytecodeBuilder *builder,
                        TR::BytecodeBuilder **bytecodeBuilderTable,
                        Instruction *program, long bytecodeIndex,
                        TR::BytecodeBuilder *nextBuilder);
  void handle_bc_jmp_le(TR::BytecodeBuilder *builder,
                        TR::BytecodeBuilder **bytecodeBuilderTable,
                        Instruction *program, long bytecodeIndex,
                        TR::BytecodeBuilder *nextBuilder);
  void handle_bc_jmp_gt(TR::BytecodeBuilder *builder,
                        TR::BytecodeBuilder **bytecodeBuilderTable,
                        Instruction *program, long bytecodeIndex,
                        TR::BytecodeBuilder *nextBuilder);
  void handle_bc_jmp_ge(TR::BytecodeBuilder *builder,
                        TR::BytecodeBuilder **bytecodeBuilderTable,
                        Instruction *program, long bytecodeIndex,
                        TR::BytecodeBuilder *nextBuilder);
};

class Compiler {
public:
  Compiler (VirtualMachine *virtualMachine, const JitConfig & jitConfig);
  uint8_t *generateCode(const FunctionSpec & functionSpec);

private:
  TR::TypeDictionary types_;
  VirtualMachine *virtualMachine_;
  MethodBuilder methodBuilder_;
  const JitConfig & jitConfig_;
};

}  // namespace b9

#endif
