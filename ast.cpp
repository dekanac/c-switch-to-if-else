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

Value* VariableExprAST::codegen() const {
	AllocaInst* tmp = NamedValues[Name];
	if (tmp == nullptr)
		yyerror("Variable " + Name + " does not exist!");
	return Builder.CreateLoad(tmp, Name);
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
    
    Value *tmp = nullptr;
    for(auto i: Vec){
        tmp = i->codegen();
        if(tmp == nullptr)
            yyerror("Codegen err");
    }
    return tmp;
}

Value* AddExprAST::codegen() const {
	Value *l = Vec[0]->codegen();
	Value *r = Vec[1]->codegen();
    if (!l || !r)
		return nullptr;
    
    Type* typel = l->getType();
    Type* typer = r->getType();
    if(typel != typer)
        yyerror("Types must match!");
    if(typel == Type::getDoubleTy(TheContext))
        return Builder.CreateFAdd(l, r, "addtmp");
    else if(typel == Type::getInt32Ty(TheContext))
        return Builder.CreateAdd(l, r, "addtmp");
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
}

Value* SubExprAST::codegen() const {
	Value *l = Vec[0]->codegen();
	Value *r = Vec[1]->codegen();
	if (!l || !r)
		return nullptr;
    
    Type* typel = l->getType();
    Type* typer = r->getType();
    if(typel != typer)
        yyerror("Types must match!");
    if(typel == Type::getDoubleTy(TheContext))
        return Builder.CreateFSub(l, r, "subtmp");
    else if(typel == Type::getInt32Ty(TheContext))
        return Builder.CreateSub(l, r, "subtmp");
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
}

Value* MulExprAST::codegen() const {
	Value *l = Vec[0]->codegen();
	Value *r = Vec[1]->codegen();
	if (!l || !r)
		return nullptr;
    
    Type* typel = l->getType();
    Type* typer = r->getType();
    if(typel != typer)
        yyerror("Types must match!");
    if(typel == Type::getDoubleTy(TheContext))
        return Builder.CreateFMul(l, r, "multmp");
    else if(typel == Type::getInt32Ty(TheContext))
        return Builder.CreateMul(l, r, "multmp");
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
}

Value* DivExprAST::codegen() const {
	Value *l = Vec[0]->codegen();
	Value *r = Vec[1]->codegen();
	if (!l || !r)
		return nullptr;
    
    Type* typel = l->getType();
    Type* typer = r->getType();
    if(typel != typer)
        yyerror("Types must match!");
    if(typel == Type::getDoubleTy(TheContext))
        return Builder.CreateFDiv(l, r, "divtmp");
    else if(typel == Type::getInt32Ty(TheContext)){
        yyerror("Can not divide 2 integers!");
        return nullptr;
    }
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
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

Value* AssignExprAST::codegen() const {
	Value *Val = Vec[0]->codegen();
	if (Val == nullptr)
		return nullptr;

	AllocaInst* alloca = NamedValues[VarName];
	if (alloca == nullptr)
		yyerror("Variable " + VarName + " does not exist");

	Builder.CreateStore(Val, alloca);
	
	return Val;
}

Value *DeclExprAST::codegen() const {
	Function *TheFunction = Builder.GetInsertBlock()->getParent();

	Value *tmp;
	for (unsigned i = 0; i < Vec.size(); i++) {
		AllocaInst *Alloca = CreateEntryBlockAlloca(Types, TheFunction, Vec[i]);

		tmp = nullptr;
		if (Types == Type::getDoubleTy(TheContext))
		tmp = ConstantFP::get(TheContext, APFloat(0.0));
		if (Types == Type::getInt32Ty(TheContext))
		tmp = ConstantInt::get(TheContext, APInt(32, 0));
		//TODO...strings, pointers, chars
		if (tmp == nullptr)
		return nullptr;
		
		NamedValues[Vec[i]] = Alloca;
		Builder.CreateStore(tmp, Alloca);
	}

	return tmp;
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

	FunctionType *FT = FunctionType::get(Type, types, false);

	Function *F =
		Function::Create(FT, Function::ExternalLinkage, Name, TheModule);

	unsigned Idx = 0;
	for (auto &Arg : F->args())
		Arg.setName(Args[Idx++]->VarName);

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
    
	//TODO fix retval
	if (Value *RetVal = Body->codegen()) {
		Builder.CreateRet(RetVal);
		verifyFunction(*TheFunction);
		TheFPM->run(*TheFunction);
		return TheFunction;
	}
	TheFunction->eraseFromParent();

	return NULL;
}

void TheFpmAndModuleInit(){
    
    TheModule = new Module("swi2else", TheContext);
    TheFPM = new legacy::FunctionPassManager(TheModule);
    
    //TheFPM->add(createInstructionCombiningPass());
    TheFPM->add(createReassociatePass());
    TheFPM->add(createNewGVNPass());
    TheFPM->add(createCFGSimplificationPass());
    //TheFPM->add(createPromoteMemoryToRegisterPass());

    TheFPM->doInitialization();
    
}


AllocaInst *CreateEntryBlockAlloca(Type *type, Function *TheFunction, const string &VarName) {
  	IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
 	return TmpB.CreateAlloca(type, 0, VarName);
}
