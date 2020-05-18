#include <vector>
#include <string>
using namespace std;

struct word
{
  int token;
  string name;
};

enum tokens
{
  tok_keywords = -1,
  tok_operators = -2,
  tok_delimeters = -3,
  tok_identifire = -4,
  tok_numbers = -5,
  tok_dataType = -6,
  tok_string = -7
};
vector<string> keywords = {"fun", "return", "for",  "while", "if", "else", "start", "end"};
vector<string> dataType = {"int", "string", "float", "void"};
vector<string> operators = {"+","-","/","*","<",">","$"};
vector<string> delimeters = {"(",")","{","}",",",";","\"","="};
struct Edges
{
    int src;
    int dest;

};
struct Node
{
    int state;
    vector<Edges> edges;
};

///////////////////////////////////////////////Abstract Syntax Tree\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

namespace temp {
vector<word> toookens;

static word currentToken;
int indexx = -1;
static word nextToken(){
    indexx++;
    return currentToken = toookens[indexx];
}

class BaseASTExpr{
    public:
    virtual ~BaseASTExpr(){}
};

class VariableDecAST : public BaseASTExpr{
    public:
        string type;
        string name;
        string value;

        VariableDecAST(const string &type, const string &name) : type(type), name(name) {cout<<"Variable declared"<<endl;}

        string getName(){ return name; }
        string getType(){ return type; }
};

vector<VariableDecAST> newVars;

class FunctionDefAST : public BaseASTExpr{
    public:
        string name;
        string type;
        vector<BaseASTExpr> body;
        vector<VariableDecAST> args;

        FunctionDefAST(const string &name, const string &type, vector<VariableDecAST> args) : name(name), type(type), args(move(args)){cout<<"Function Declared"<<endl;} 

        string getName(){ return name; }
        vector<VariableDecAST> getArgs(){ return args; }
};

vector<FunctionDefAST> newFunctions;

class NumberExprAST : public BaseASTExpr{
    public:
    
        virtual double value() = 0;
};

class DoubleExprAST : public NumberExprAST{
        double nrValue;

        public:
            DoubleExprAST(double nrValue) : nrValue(nrValue){}

            double value(){ return nrValue; }
};

class BinaryOpASt : public NumberExprAST{
    char op;
    NumberExprAST *left;
    NumberExprAST *right;

    public:
        BinaryOpASt(char op, NumberExprAST *left, NumberExprAST *right) : op(op), left(move(left)), right(move(right)){}

        double value(){
            switch (op)
            {
            case '+': return left->value() + right->value();
            case '-': return left->value() - right->value();
            case '*': return left->value() * right->value();
            case '/': return left->value() / right->value();
            case '>': return left->value() > right->value();
            case '<': return left->value() < right->value();
            case '$':return left->value() == right->value();
            default :
                cout<<"InvalidOperatorException"<<endl;
                exit(1);
                return 1;
            }
        }
    };
}

