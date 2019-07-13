#ifndef __AST_HPP__
#define __AST_HPP__ 1

#include <iostream>
#include <vector>
#include <string>
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

enum PrimType { T_INT, T_DOUBLE, T_CHAR, T_STRING };

class ExprAST {
public:
  	virtual Value* codegen() const = 0;
  	virtual ~ExprAST() {}
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
	BlockAST(vector<ExprAST *> e) : InnerExprAST(e) {}
	Value *codegen() const;

private:
	BlockAST(const BlockAST &);
	BlockAST &operator=(const BlockAST &);
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

class CallExprAST : public InnerExprAST {
public:
	CallExprAST(string c, const vector<ExprAST*> &v)
		:InnerExprAST(v), Callee(c)
	{ }
	Value* codegen() const;
private:
  	string Callee;
};

class IfExprAST : public InnerExprAST {
public:
	IfExprAST(ExprAST* cond, ExprAST *e1, ExprAST *e2)
		:InnerExprAST(cond, e1, e2)
	{}
	Value* codegen() const;
};

class AssignExprAST : public InnerExprAST {
public:
	AssignExprAST(string s, ExprAST* e)
		:InnerExprAST(e), VarName(s)
	{}
	Value* codegen() const;
private:
	string VarName;
	enum PrimType type;
};

class TypeAST {
public:
  TypeAST(Type *t, string v) : type(t), str(v) {}
  ~TypeAST();

  Type *type;
  std::string str;
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

AllocaInst *CreateEntryBlockAlloca(Type *type, Function *TheFunction, const string &VarName);

#endif

