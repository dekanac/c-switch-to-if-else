#include "ast.hpp"
//TODO lifespan of vars not working
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
        return Builder.CreateUDiv(l, r, "divtmp");
    }
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
}
//TODO FIX TYPES FOR LT GT and EQ operators
Value* LtExprAST::codegen() const {
    Value *l = Vec[0]->codegen();
    Value *r = Vec[1]->codegen();
    if (!l || !r)
      return nullptr;
    Type* typel = l->getType();
    Type* typer = r->getType();
    if (typel != typer)
        yyerror("Types must match!");
    if (typel == Type::getDoubleTy(TheContext)){
        Value* tmp = Builder.CreateFCmpULT(l, r, "lttmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else if (typel == Type::getInt32Ty(TheContext)) {
        Value* tmp = Builder.CreateICmpULT(l, r, "lttmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
    
}

Value* GtExprAST::codegen() const {
    Value *l = Vec[0]->codegen();
    Value *r = Vec[1]->codegen();
    if (!l || !r)
      return nullptr;
    Type* typel = l->getType();
    Type* typer = r->getType();
    if (typel != typer)
        yyerror("Types must match!");
    if (typel == Type::getDoubleTy(TheContext)){
        Value* tmp = Builder.CreateFCmpUGT(l, r, "gttmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else if (typel == Type::getInt32Ty(TheContext)) {
        Value* tmp = Builder.CreateICmpUGT(l, r, "gttmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
    
}

Value* EqExprAST::codegen() const {
    Value *l = Vec[0]->codegen();
    Value *r = Vec[1]->codegen();
    if (!l || !r)
      return nullptr;
    Type* typel = l->getType();
    Type* typer = r->getType();
    if (typel != typer)
        yyerror("Types must match!");
    if (typel == Type::getDoubleTy(TheContext)){
        Value* tmp = Builder.CreateFCmpUEQ(l, r, "eqtmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else if (typel == Type::getInt32Ty(TheContext)) {
        Value* tmp = Builder.CreateICmpEQ(l, r, "eqtmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
    
}

Value* NeExprAST::codegen() const {
    Value *l = Vec[0]->codegen();
    Value *r = Vec[1]->codegen();
    if (!l || !r)
      return nullptr;
    Type* typel = l->getType();
    Type* typer = r->getType();
    if (typel != typer)
        yyerror("Types must match!");
    if (typel == Type::getDoubleTy(TheContext)){
        Value* tmp = Builder.CreateFCmpUNE(l, r, "netmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else if (typel == Type::getInt32Ty(TheContext)) {
        Value* tmp = Builder.CreateICmpNE(l, r, "netmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
    
}

Value* LeExprAST::codegen() const {
    Value *l = Vec[0]->codegen();
    Value *r = Vec[1]->codegen();
    if (!l || !r)
      return nullptr;
    Type* typel = l->getType();
    Type* typer = r->getType();
    if (typel != typer)
        yyerror("Types must match!");
    if (typel == Type::getDoubleTy(TheContext)){
        Value* tmp = Builder.CreateFCmpULE(l, r, "letmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else if (typel == Type::getInt32Ty(TheContext)) {
        Value* tmp = Builder.CreateICmpULE(l, r, "letmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
    
}

Value* GeExprAST::codegen() const {
    Value *l = Vec[0]->codegen();
    Value *r = Vec[1]->codegen();
    if (!l || !r)
      return nullptr;
    Type* typel = l->getType();
    Type* typer = r->getType();
    if (typel != typer)
        yyerror("Types must match!");
    if (typel == Type::getDoubleTy(TheContext)){
        Value* tmp = Builder.CreateFCmpUGE(l, r, "netmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else if (typel == Type::getInt32Ty(TheContext)) {
        Value* tmp = Builder.CreateICmpUGE(l, r, "getmp");
        return Builder.CreateUIToFP(tmp, Type::getDoubleTy(TheContext), "booltmp");
    }
    else{
        yyerror("Error matching types!");
        return nullptr;
    }
    
}


Value* AssignExprAST::codegen() const {
	Value *Val = Vec[0]->codegen();
	if (Val == nullptr)
		return nullptr;

	AllocaInst* alloca = NamedValues[VarName];
	if (alloca == nullptr)
		yyerror("Variable " + VarName + " does not exist");

    Type* ValType = Val->getType();
    Type* AllocaType = alloca->getAllocatedType();
    if(ValType != AllocaType)
        yyerror("Implicit conversion not allowed!");
    
	Builder.CreateStore(Val, alloca);
	
	return Val;
}

Value* CallExprAST::codegen() const {
  Function* CalleeF = TheModule->getFunction(Callee);
  if (CalleeF == nullptr)
    yyerror("Function " + Callee + " does not exist");

  unsigned arg_size = CalleeF->arg_size();
  if (arg_size != Vec.size())
    yyerror("Function " + Callee + " must be called with " + to_string(arg_size) + " arguments");

  vector<Value*> args;
  for (unsigned i = 0; i < arg_size; i++) {
    Value *tmp = Vec[i]->codegen();
    if (tmp == nullptr)
      return nullptr;
    args.push_back(tmp);
  }

  return Builder.CreateCall(CalleeF, args, "calltmp");
}

Value *DeclExprAST::codegen() const {
	Function *TheFunction = Builder.GetInsertBlock()->getParent();
    
	Value *tmp;
	for (unsigned i = 0; i < Vec.size(); i++) {
        AllocaInst *Alloca = NamedValues[Vec[i]];
        if(Alloca != nullptr)
            yyerror("Var " + Vec[i] + " already exist! Redefinition of variable not allowed");
        
        Alloca = CreateEntryBlockAlloca(Types, TheFunction, Vec[i]);

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
    
    Value* IfCondV;
    Type * CondType = CondV->getType();
    if(CondType == Type::getDoubleTy(TheContext))
        IfCondV = Builder.CreateFCmpONE(CondV, ConstantFP::get(TheContext, APFloat(0.0)), "ifcond");
    else if(CondType == Type::getInt32Ty(TheContext))
        IfCondV = Builder.CreateICmpEQ(CondV, ConstantInt::get(TheContext, APInt(32, 0)), "ifcond");

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
    
    //TODO phi node type 
//     PHINode *PHI = Builder.CreatePHI(ThenV->getType(), 2, "iftmp");
//     PHI->addIncoming(ThenV, ThenBB);
//     PHI->addIncoming(ElseV, ElseBB);
//     
//     return PHI;
        return ConstantInt::get(TheContext, APInt(32, 0));

}

Value* WhileExprAST::codegen() const {
    //TODO types in cond fix needed
    Function *F = Builder.GetInsertBlock()->getParent();
    BasicBlock *Loop1BB = BasicBlock::Create(TheContext, "loop1", F);
    BasicBlock *Loop2BB = BasicBlock::Create(TheContext, "loop2", F);
    BasicBlock *AfterLoopBB = BasicBlock::Create(TheContext, "afterloop", F);
    Builder.CreateBr(Loop1BB);
    Builder.SetInsertPoint(Loop1BB);
    
    Value* CondV = Vec[0]->codegen();
    if (CondV == nullptr)
      return nullptr;
    
    Value* WhileCondV;
    Type * CondType = CondV->getType();
    if(CondType == Type::getDoubleTy(TheContext))
        WhileCondV = Builder.CreateFCmpONE(CondV, ConstantFP::get(TheContext, APFloat(0.0)), "whilecond");
    else if(CondType == Type::getInt32Ty(TheContext))
        WhileCondV = Builder.CreateICmpEQ(CondV, ConstantInt::get(TheContext, APInt(32, 0)), "whilecond");
    
    Builder.CreateCondBr(WhileCondV, Loop2BB, AfterLoopBB);
    Loop1BB = Builder.GetInsertBlock();

    Builder.SetInsertPoint(Loop2BB);
    Value* Tmp = Vec[1]->codegen();
    if (Tmp == nullptr)
        return nullptr;
    Builder.CreateBr(Loop1BB);
    Loop2BB = Builder.GetInsertBlock();

    Builder.SetInsertPoint(AfterLoopBB);
    return ConstantInt::get(TheContext, APInt(32, 0));
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
	
	//TODO fix! not real return val
	
	if (Value *RetVal = Body->codegen()) {
        
        if (Proto->getType() == Type::getVoidTy(TheContext)){
            RetVal = nullptr;
        }
        
        Builder.CreateRet(RetVal);
		verifyFunction(*TheFunction);
		TheFPM->run(*TheFunction);
		return TheFunction;
	}
	
	TheFunction->eraseFromParent();

	return NULL;
}

Value* SwitchExprAST::codegen() const {
    
    //generating switch condition
    Value* SwitchCond = Condition->codegen();
    if (SwitchCond == nullptr)
        return nullptr;
    
    Function* TheFunction = Builder.GetInsertBlock()->getParent();
    
    int num_of_default_cases = 0;
    //check if theres default case on last position
    for(auto i: Cases)
    if(i.first.first == nullptr)
        num_of_default_cases++;
    if(num_of_default_cases > 1)
        yyerror("Too much default cases! Only one allowed");
    //calculate number of IF statemets
    int num_of_ifs = Cases.size() - num_of_default_cases;
    
    std::vector<BasicBlock*> ThenBBs(num_of_ifs);
    std::vector<BasicBlock*> ElseBBs(num_of_ifs);
    BasicBlock* MergeBB = BasicBlock::Create(TheContext, "ifcont");
    
    for(int i = 0; i < num_of_ifs; i++) {

        Value* CaseCond;
        if(Cases[i].first.first != nullptr)
        CaseCond = Cases[i].first.first->codegen();
        if(CaseCond == nullptr)
            return nullptr;
        
        Value *IfCondV = Builder.CreateICmpEQ(SwitchCond, CaseCond, "ifcond");
        
        ThenBBs[i] = BasicBlock::Create(TheContext, "then", TheFunction);
        ElseBBs[i] = BasicBlock::Create(TheContext, "else");
        
        Builder.CreateCondBr(IfCondV, ThenBBs[i], ElseBBs[i]);
        Builder.SetInsertPoint(ThenBBs[i]);

        Value* ThenV = Cases[i].first.second->codegen();
        if(ThenV == nullptr)
            return nullptr;

        //if case has break stmt jump to MergeBB which is the end
        if(Cases[i].second){
        }
        
        Builder.CreateBr(MergeBB);
        ThenBBs[i] = Builder.GetInsertBlock();
        
        TheFunction->getBasicBlockList().push_back(ElseBBs[i]);
        Builder.SetInsertPoint(ElseBBs[i]);        
        
    }
        
    Builder.CreateBr(MergeBB);
    ElseBBs[num_of_ifs-1] = Builder.GetInsertBlock();

    //if default case exists
    TheFunction->getBasicBlockList().push_back(MergeBB);
    Builder.SetInsertPoint(MergeBB);     
    
        
    
    return ConstantInt::get(TheContext, APInt(32, 0));
    
}

void TheFpmAndModuleInit(){
    
    TheModule = new Module("swi2else", TheContext);
    TheFPM = new legacy::FunctionPassManager(TheModule);
    
    //TheFPM->add(createInstructionCombiningPass());
    TheFPM->add(createReassociatePass());
    //TheFPM->add(createNewGVNPass());
    //TheFPM->add(createCFGSimplificationPass());
    //TheFPM->add(createPromoteMemoryToRegisterPass());
    
    TheFPM->doInitialization();
    
}


AllocaInst *CreateEntryBlockAlloca(Type *type, Function *TheFunction, const string &VarName) {
  	IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
 	return TmpB.CreateAlloca(type, 0, VarName);
}
