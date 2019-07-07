#include "ast.hpp"

void yyerror(string s);

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
Module* TheModule;
map<string, AllocaInst*> NamedValues;
legacy::FunctionPassManager* TheFPM;

Value* NumberExprAST::codegen() const {
  return ConstantFP::get(TheContext, APFloat(Val));
}

Value* VariableExprAST::codegen() const {
  AllocaInst* tmp = NamedValues[Name];
  if (tmp == nullptr)
    yyerror("Variable " + Name + " not exist");
  return Builder.CreateLoad(tmp, Name);
}

InnerExprAST::~InnerExprAST() {
  for (auto i : Vec)
    delete i;
}

InnerExprAST::InnerExprAST(ExprAST *e1) {
  Vec.push_back(e1);
}

InnerExprAST::InnerExprAST(ExprAST *e1, ExprAST *e2) {
  Vec.push_back(e1);
  Vec.push_back(e2);
}

InnerExprAST::InnerExprAST(ExprAST *e1, ExprAST *e2, ExprAST *e3) {
  Vec.push_back(e1);
  Vec.push_back(e2);
  Vec.push_back(e3);
}

InnerExprAST::InnerExprAST(ExprAST *e1, ExprAST *e2, ExprAST *e3, ExprAST *e4) {
  Vec.push_back(e1);
  Vec.push_back(e2);
  Vec.push_back(e3);
  Vec.push_back(e4);
}

Value* AddExprAST::codegen() const {
  Value *l = Vec[0]->codegen();
  Value *r = Vec[1]->codegen();
  if (!l || !r)
    return nullptr;
  return Builder.CreateFAdd(l, r, "addtmp");
}

Value* SubExprAST::codegen() const {
  Value *l = Vec[0]->codegen();
  Value *r = Vec[1]->codegen();
  if (!l || !r)
    return nullptr;
  return Builder.CreateFSub(l, r, "subtmp");
}

Value* MulExprAST::codegen() const {
  Value *l = Vec[0]->codegen();
  Value *r = Vec[1]->codegen();
  if (!l || !r)
    return nullptr;
  return Builder.CreateFMul(l, r, "multmp");
}

Value* DivExprAST::codegen() const {
  Value *l = Vec[0]->codegen();
  Value *r = Vec[1]->codegen();
  if (!l || !r)
    return nullptr;
  return Builder.CreateFDiv(l, r, "divtmp");
}

Value* LtExprAST::codegen() const {
  Value *l = Vec[0]->codegen();
  Value *r = Vec[1]->codegen();
  if (!l || !r)
    return nullptr;
  Value* tmp = Builder.CreateFCmpULT(l, r, "lttmp");
  return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
}

Value* GtExprAST::codegen() const {
  Value *l = Vec[0]->codegen();
  Value *r = Vec[1]->codegen();
  if (!l || !r)
    return nullptr;
  Value* tmp = Builder.CreateFCmpUGT(l, r, "gttmp");
  return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
}

Value* IfExprAST::codegen() const {
  Value* CondV = Vec[0]->codegen();
  if (CondV == nullptr)
    return nullptr;
  
  Value *IfCondV = Builder.CreateFCmpONE(CondV, ConstantFP::get(TheContext, APFloat(0.0)), "ifcond");

  Function* TheFunction = Builder.GetInsertBlock()->getParent();
  BasicBlock* ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
  BasicBlock* ElseBB = BasicBlock::Create(TheContext, "else");
  BasicBlock* MergeBB = BasicBlock::Create(TheContext, "ifcont");
  
  Builder.CreateCondBr(IfCondV, ThenBB, ElseBB);

  Builder.SetInsertPoint(ThenBB);
  Value* ThenV = Vec[1]->codegen();
  if (ThenV == nullptr)
    return nullptr;
  Builder.CreateBr(MergeBB);
  ThenBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);
  Value* ElseV = Vec[2]->codegen();
  if (ElseV == nullptr)
    return nullptr;
  Builder.CreateBr(MergeBB);
  ElseBB = Builder.GetInsertBlock();

  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  PHINode *PHI = Builder.CreatePHI(Type::getDoubleTy(TheContext), 2, "iftmp");
  PHI->addIncoming(ThenV, ThenBB);
  PHI->addIncoming(ElseV, ElseBB);
  
  return PHI;
}

AllocaInst *CreateEntryBlockAlloca(Function *TheFunction,
				   const string &VarName) {
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                 TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(Type::getDoubleTy(TheContext), 0,
                           VarName);
}
