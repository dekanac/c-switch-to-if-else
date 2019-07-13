#include "ast.hpp"

void yyerror(string s);

LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
Module* TheModule;
map<string, AllocaInst*> NamedValues;
legacy::FunctionPassManager* TheFPM;

Value* IntNumberExprAST::codegen() const {
  	return ConstantInt::get(TheContext, APInt(32, Val));
}

Value* DoubleNumberExprAST::codegen() const {
  	return ConstantFP::get(TheContext, APFloat(Val));
}

//destructors

TypeAST::~TypeAST() {}

InnerExprAST::~InnerExprAST() {
  	for (auto i : Vec)
    	delete i;
}

FunctionAST::~FunctionAST() { delete Body; }

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

Value *BlockAST::codegen() const {
	unsigned size = Vec.size() - 1;

	for (unsigned i = 0; i < size; i++)
		Vec[i]->codegen();

	return Vec[size]->codegen();
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

Value* EqExprAST::codegen() const {
    Value *l = Vec[0]->codegen();
    Value *r = Vec[1]->codegen();
    if (!l || !r)
      return nullptr;
    Value* tmp = Builder.CreateFCmpUEQ(l, r, "lttmp");
    return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
}

Value *AssignExprAST::codegen() const {
	std::cout << "Debug pt1" << std::endl;
	return nullptr;
//TODO	
}

Value *DeclExprAST::codegen() const {
	Function *TheFunction = Builder.GetInsertBlock()->getParent();

	for (unsigned i = 0; i < Vec.size(); i++) {
		AllocaInst *Alloca = CreateEntryBlockAlloca(Types, TheFunction, Vec[i]);

		Value *tmp = NULL;
		if (Types == Type::getDoubleTy(TheContext))
		tmp = ConstantFP::get(TheContext, APFloat(0.0));
		if (Types == Type::getInt32Ty(TheContext))
		tmp = ConstantInt::get(TheContext, APInt(32, 0));
		//TODO...strings, pointers, chars
		if (tmp == NULL)
		return NULL;

		NamedValues[Vec[i]] = Alloca;
		Builder.CreateStore(tmp, Alloca);
	}

	return NULL;
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

Function *PrototypeAST::codegen() const {
	std::vector<llvm::Type*> types;

	for (unsigned i = 0; i < Args.size(); ++i) {
		types.push_back(Args[i]->type);
	}
	vector<llvm::Type *> Doubles(Args.size(), Type::getDoubleTy(TheContext));

	FunctionType *FT = FunctionType::get(Type, types, false);

	Function *F =
		Function::Create(FT, Function::ExternalLinkage, Name, TheModule);

	unsigned Idx = 0;
	for (auto &Arg : F->args())
		Arg.setName(Args[Idx++]->str);

	return F;
}

Function *FunctionAST::codegen() const {
	Function* TheFunction = TheModule->getFunction(Proto->getName());
  	
	if (TheFunction == nullptr)
    	TheFunction = Proto->codegen();

  	if (TheFunction == nullptr)
    	return nullptr;

  	if (!TheFunction->empty())
    	yyerror("Function redefinition is not allowed " + Proto->getName());


	BasicBlock *BB = BasicBlock::Create(TheContext, "entry", TheFunction);
	Builder.SetInsertPoint(BB);

	NamedValues.clear();
	for (auto &Arg : TheFunction->args()) {
		AllocaInst *Alloca =
			CreateEntryBlockAlloca(Arg.getType(), TheFunction, Arg.getName());
		NamedValues[Arg.getName()] = Alloca;
		Builder.CreateStore(&Arg, Alloca);
	}

	if (Value *RetVal = Body->codegen()) {
		Builder.CreateRet(RetVal);
		verifyFunction(*TheFunction);
		TheFPM->run(*TheFunction);
		return TheFunction;
	}
	TheFunction->eraseFromParent();

	return NULL;
}

AllocaInst *CreateEntryBlockAlloca(Type *type, Function *TheFunction, const string &VarName) {
  	IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
 	return TmpB.CreateAlloca(type, 0, VarName);
}
