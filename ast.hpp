#ifndef __AST_HPP__
#define __AST_HPP__ 1

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include "llvm/IR/Value.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;
using namespace llvm::legacy;
using namespace std;


class ExprAST {
public:
  	virtual Value* codegen() const = 0;
  	virtual ~ExprAST() {}
};

class VariableExprAST : public ExprAST {
public:
	VariableExprAST(const string &n)
		:Name(n)
	{}
	Value* codegen() const;
private:
  	string Name;
};

class IntNumberExprAST : public ExprAST {
public:
	IntNumberExprAST(int v)
		:Val(v)
	{}
	Value* codegen() const;
private:
	int Val;
};

class DoubleNumberExprAST : public ExprAST {
public:
	DoubleNumberExprAST(double v)
		:Val(v)
	{}
	Value* codegen() const;
private:
	double Val;
};

class InnerExprAST : public ExprAST {
public:
	InnerExprAST(const vector<ExprAST*> &v)
		:Vec(v)
	{}
	InnerExprAST(ExprAST* e1);
	InnerExprAST(ExprAST* e1, ExprAST* e2);
	InnerExprAST(ExprAST* e1, ExprAST* e2, ExprAST* e3);
	InnerExprAST(ExprAST* e1, ExprAST* e2, ExprAST* e3, ExprAST* e4);
	~InnerExprAST();
private:
	InnerExprAST(const InnerExprAST&);
	InnerExprAST& operator=(const InnerExprAST&);
protected:
  	vector<ExprAST*> Vec;
};

class BlockAST : public InnerExprAST {
public:
	BlockAST(vector<ExprAST *> e) 
        : InnerExprAST(e) 
    {}
	Value *codegen() const;
};

class AddExprAST : public InnerExprAST {
public:
	AddExprAST(ExprAST* l, ExprAST *r)
		:InnerExprAST(l, r)
	{}
	Value* codegen() const;
};

class SubExprAST : public InnerExprAST {
public:
	SubExprAST(ExprAST* l, ExprAST *r)
		:InnerExprAST(l, r)
	{}
	Value* codegen() const;
};

class MulExprAST : public InnerExprAST {
public:
	MulExprAST(ExprAST* l, ExprAST *r)
		:InnerExprAST(l, r)
	{}
	Value* codegen() const;
};

class DivExprAST : public InnerExprAST {
public:
	DivExprAST(ExprAST* l, ExprAST *r)
		:InnerExprAST(l, r)
	{}
	Value* codegen() const;
};

class LtExprAST : public InnerExprAST {
public:
	LtExprAST(ExprAST* l, ExprAST *r)
		:InnerExprAST(l, r)
	{}
	Value* codegen() const;
};

class GtExprAST : public InnerExprAST {
public:
	GtExprAST(ExprAST* l, ExprAST *r)
		:InnerExprAST(l, r)
	{}
	Value* codegen() const;
};

class EqExprAST : public InnerExprAST {
public:
	EqExprAST(ExprAST* l, ExprAST *r)
		:InnerExprAST(l, r)
	{}
	Value* codegen() const;
};

class NeExprAST : public InnerExprAST {
public:
	NeExprAST(ExprAST* l, ExprAST *r)
		:InnerExprAST(l, r)
	{}
	Value* codegen() const;
};

class LeExprAST : public InnerExprAST {
public:
	LeExprAST(ExprAST* l, ExprAST *r)
		:InnerExprAST(l, r)
	{}
	Value* codegen() const;
};

class GeExprAST : public InnerExprAST {
public:
	GeExprAST(ExprAST* l, ExprAST *r)
		:InnerExprAST(l, r)
	{}
	Value* codegen() const;
};

class CallExprAST : public InnerExprAST {
public:
	CallExprAST(std::string c, const vector<ExprAST*> &v)
		:InnerExprAST(v), Callee(c)
	{ }
	Value* codegen() const;
private:
  	string Callee;
};

class WhileExprAST : public InnerExprAST {
public:
  WhileExprAST(ExprAST *e1, ExprAST *e2)
    :InnerExprAST(e1, e2)
  {}
  Value* codegen() const;
};

class IfExprAST : public InnerExprAST {
public:
	IfExprAST(ExprAST* cond, ExprAST *e1, ExprAST *e2)
		:InnerExprAST(cond, e1, e2)
	{}
	Value* codegen() const;
};

class SwitchExprAST : public ExprAST {
public:
    SwitchExprAST(ExprAST* condition, std::vector<std::pair<std::pair<ExprAST*, ExprAST*>, bool>> &cases)
        : Condition(condition), Cases(cases)
    {}
    Value* codegen() const;
private:
    ExprAST* Condition;
    std::vector<std::pair<std::pair<ExprAST*, ExprAST*>, bool>> &Cases;
    
};

class AssignExprAST : public InnerExprAST {
public:
	AssignExprAST(std::string s, ExprAST* e)
		:InnerExprAST(e), VarName(s)
	{}
	Value* codegen() const;
private:
 	std::string VarName;
};

class DeclAndAssignExprAST : public ExprAST {
public:
    DeclAndAssignExprAST(Type* t, std::string n, ExprAST* e)
        : Expr(e), VarType(t), VarName(n)
    {}
    Value *codegen() const;
private:
    Type* VarType;
    std::string VarName;
    ExprAST* Expr;
};

class TypeAST {
public:
	TypeAST(Type *t, string v) 
        : type(t), VarName(v)
    {}
	~TypeAST();
private:
	Type *type;
	std::string VarName;
    friend class PrototypeAST;
};

class DeclExprAST : public ExprAST {
public:
	DeclExprAST(Type *t, std::vector<std::string> v) : Types(t), Vec(v) {}
	Value *codegen() const;

private:
	Type *Types;
	vector<string> Vec;
};

class PrototypeAST {
public:
	PrototypeAST(Type *t, std::string n, std::vector<TypeAST *> a)
		: Type(t), Name(n), Args(a) {}
	Function *codegen() const;
    Type* getType(){
        return Type;
    }
	string getName() const { return Name; }

private:
	Type *Type;
	std::string Name;
	std::vector<TypeAST*> Args;
};

class FunctionAST {
public:
	FunctionAST(PrototypeAST *p, ExprAST *b) : Proto(p), Body(b) {}
	~FunctionAST();
	Function *codegen() const;

private:
	FunctionAST(const FunctionAST &f);
	FunctionAST &operator=(const FunctionAST &f);
	PrototypeAST *Proto;
	ExprAST *Body;
};

void TheFpmAndModuleInit();

AllocaInst *CreateEntryBlockAlloca(Type *type, Function *TheFunction, const string &VarName);

AllocaInst *FindVarInTable(std::string Name);

#endif

