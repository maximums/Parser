#include <iostream>
#include <memory>
#include "Basic.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

string readFIle()
{
  string myText;
  string line;
  ifstream myfile("text.txt");
    if(myfile.is_open())
    {
      while (getline(myfile, line))
      {
        if (line == "") continue;
        myText += line + " ";
      }
      myfile.close();
    }
  return myText;
}

vector<string> makeWords()
{
    int i = 0;
    vector<string> tmpVec;
    string tmp;
    while (i<readFIle().length())
    {
      if(isspace(readFIle()[i]))
      {
        tmpVec.push_back(tmp);
        tmp.clear();
        goto inc;
      }else if(ispunct(readFIle()[i]))
      {
        tmpVec.push_back(tmp);
        tmpVec.push_back(string(1, readFIle()[i]));
        tmp.clear();
        goto inc;
      }
      tmp+=readFIle()[i];
  inc:i++;      
    }
    return tmpVec;
}

vector<word> Tokire()
{
  int i = 0;
  vector<word> Tokens;
  vector<string> tmpVec = makeWords();
  while(i < tmpVec.size())
  {
    for(int kl = 0; kl < dataType.size(); kl++){
      if(tmpVec[i]==dataType[kl]){
        Tokens.push_back({tok_dataType, tmpVec[i]});
        goto out;
      }
    }
      //keywords
      for(int j = 0; j < keywords.size(); j++)
      {
        if(tmpVec[i]==keywords[j])
        {
          Tokens.push_back({tok_keywords,tmpVec[i]});
          goto out;
        }
      }
      //delimeters
      for(int k = 0; k < delimeters.size(); k++)
      {
        if(tmpVec[i]==delimeters[k])
        {
          Tokens.push_back({tok_delimeters,tmpVec[i]});
          goto out;
        }
      }
      //operators
      for(int y = 0; y < operators.size(); y++)
      {
        if(tmpVec[i]==operators[y])
        {
          Tokens.push_back({tok_operators,tmpVec[i]});
          goto out;
        }    
      }
      //identifire
      if(tmpVec[i][0] >= 60 && tmpVec[i][0] <=90 || tmpVec[i][0] >= 97 && tmpVec[i][0] <=122){
          Tokens.push_back({tok_identifire,tmpVec[i]});
        }
      //number
      for(int z = 0; z < tmpVec[i].length(); z++){
            if(!(tmpVec[i][z] >=48 && tmpVec[i][z] <=57 || tmpVec[i][z]==46)) break;
            else if(z == tmpVec[i].length()-1) 
            Tokens.push_back({tok_numbers,tmpVec[i]});
          }
       out: i++;
  }
  return Tokens;
}

///////////////////////////////////////////////////////Parsing\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

void binaryOperations(temp::DoubleExprAST left,int name);
void functionCall();

temp::VariableDecAST variableDeclaration(){
    string type = temp::currentToken.name;
    string name;

    temp::nextToken();

    if (temp::currentToken.token == tokens::tok_identifire)
        name = temp::currentToken.name;
    else
    {
        cout << "Identifier expected at variable declaration" << endl;
        exit(1);
    }
    temp::nextToken();
    if(temp::currentToken.name.compare(";") == 0){
        temp::VariableDecAST var(type, name);
        temp::newVars.push_back(var);
        temp::nextToken();
        return var;
    }else
    {
        cout<<"';' is missing "<<endl;
        exit(1);
    }  
}

void functionDeclaration(){
    string name;
    string type;
    vector<temp::VariableDecAST> args;

    temp::nextToken();
    if(temp::currentToken.token != tokens::tok_dataType){
        cout<<"Return type is missing "<<endl;
        return;
    }
    type = temp::currentToken.name;
    temp::nextToken();
    if(temp::currentToken.token != tokens::tok_identifire){
        cout<<"Invalid function name ' "<<temp::currentToken.name<<" '"<<endl;
        return;
    }
    name = temp::currentToken.name;
    temp::nextToken();
    if(temp::currentToken.name.compare("(") !=0){
        cout<<"'( )' are missing"<<endl;
        return;
    }
    temp::nextToken();
    while(temp::currentToken.name.compare(")") != 0){
        args.push_back(variableDeclaration());
    }
    temp::FunctionDefAST func(name, type, args);
    temp::newFunctions.push_back(func);
    temp::nextToken();
    if (temp::currentToken.name.compare("{") !=0 ){
        cout<<"' { ' is missing"<<endl;
        exit(1);
    }
    temp::nextToken();
    while (temp::currentToken.name.compare("}") != 0){
        if (temp::currentToken.token == tok_dataType)
            variableDeclaration();
        if (temp::currentToken.name.compare("fun") == 0)
            functionDeclaration();
        if (temp::currentToken.token == tok_identifire)
            functionCall();
    }
    temp::nextToken();
}

void variableAssign(string name){
    string type;
    int i;
    temp::DoubleExprAST* tmp;

    for (int j = 0; j < temp::newVars.size(); j++){
        if (name.compare(temp::newVars[j].name) == 0){
            i = j;
            type = temp::newVars[j].type;
            break;
        }
        if(j == temp::newVars.size()-1){
            cout<<"Undeclared varaiable "<<name<<endl;
            exit(1);
        }
    }
    // temp::nextToken(); becauze deja sa strecut la urmaorul simbol la '=' in cazul dat
    if (temp::currentToken.name.compare("=") != 0){
        cout<<"' = ' expected"<<endl;
        exit(1);
    }
    temp::nextToken();
    if(count(temp::currentToken.name.begin(), temp::currentToken.name.end(), '.') == 0 && type == "int"){
        temp::newVars[i].value = temp::currentToken.name;
        temp::DoubleExprAST left4bOP(stod(temp::currentToken.name));
        tmp = &left4bOP;
        temp::nextToken();
    }
    else if(count(temp::currentToken.name.begin(), temp::currentToken.name.end(), '.') == 1 && type == "float"){
        temp::newVars[i].value = temp::currentToken.name;
        temp::DoubleExprAST left4bOP(stod(temp::currentToken.name));
        tmp = &left4bOP;
        temp::nextToken();
    }
    else if(type == "string"){
        temp::newVars[i].value = temp::currentToken.name;
        temp::nextToken();
    }else {
        cout<<"Invalid data type for variable : "<<name<<endl;
        exit(1);
    }
    if(temp::currentToken.name.compare(";") == 0)
        temp::nextToken();
    else if (temp::currentToken.token == tok_operators) {
        binaryOperations(*tmp,i);
        return;
    }else{
        cout<<"' ; ' is missing "<<endl;
        exit(1);
    }
    
}

void parserMain(){
    temp::nextToken();
    while(temp::currentToken.token != -8){
        functionDeclaration();
    }    
}

void functionCall(){
    string name;
    int argsNr = 0;
    int i;

    name = temp::currentToken.name;
    temp::nextToken();
    if (temp::currentToken.name.compare("(") != 0){
        variableAssign(name);
        return;
        }
    for (int j = 0; j < temp::newFunctions.size(); j++){
        if (name.compare(temp::newFunctions[j].name) == 0){
            i = j;
            break;
        }
        if (j == temp::newFunctions.size()-1){
            cout<<"Unknown function ->"<<name<<endl;
            exit(1);
        }
    }
    temp::nextToken();
    while (temp::currentToken.name.compare(")") != 0){
        if (temp::currentToken.name.compare(",") == 0)
            temp::nextToken();
        if (temp::currentToken.token == tok_identifire){
            for (int j = 0; j < temp::newVars.size(); j++){
                if (temp::newVars[j].name.compare(temp::currentToken.name) == 0){
                    if (temp::newVars[j].type == temp::newFunctions[i].args[argsNr].type)
                        break;
                    cout<<"Parameter->"<<temp::currentToken.name<<" incompatible data type"<<endl;
                    exit(1);
                }
            }
            argsNr++;
            temp::nextToken();
        }else if(temp::currentToken.token == tok_numbers){
                if(count(temp::currentToken.name.begin(), temp::currentToken.name.end(), '.') == 0 && temp::newFunctions[i].args[argsNr].type == "int"){
                    argsNr++;
                    cout<<"int"<<endl;
                    temp::nextToken();
                }
                else if(count(temp::currentToken.name.begin(), temp::currentToken.name.end(), '.') == 1 && temp::newFunctions[i].args[argsNr].type == "float"){
                    argsNr++;
                    cout<<"flaot"<<endl;
                    temp::nextToken();
                }
                else {
                    cout<<"Invaid type of parameter->"<<temp::currentToken.name<<endl;
                    exit(1);
                }
        }else if(temp::currentToken.token == tok_string){
            if (temp::newFunctions[i].args[argsNr].type == "string"){
                argsNr++;
                temp::nextToken();
            }else {
                cout<<"Invaid type of parameter->"<<temp::currentToken.name<<endl;
                exit(1);
            }   
        }
    }
    temp::nextToken();
    if (temp::currentToken.name.compare(";") != 0){
                cout<<"' ; ' is missing"<<endl;
                exit(1);
    }
    cout<<"Function Call"<<endl;
    temp::nextToken();
}

void binaryOperations(temp::DoubleExprAST left,int name){
    int op = 0, opera = 1;
    double finalResult = 0;
    double resultM = 0;
    double resultD = 0;
    vector<string> right;
    right.push_back(to_string(left.value()));
    while(temp::currentToken.name.compare(";") != 0){
        if (temp::currentToken.token == tok_operators){
            right.push_back(temp::currentToken.name);
            op++;
            }
        else if (temp::currentToken.token == tok_numbers){
            right.push_back(temp::currentToken.name);
            opera++;
            }
        else
        {
            cout<<"Unknown symbol in exprection "<<temp::currentToken.name<<endl;
            exit(1);
        }      
        temp::nextToken();  
    }
    if (op != opera-1){
        cout<<"Invalid expresion"<<endl;
        exit(1);
    }
    while (right.size() != 1){
        auto itmult = find(right.begin(), right.end(), "*");
        auto itdiv = find(right.begin(), right.end(), "/");
        if (itmult != right.end()) {
            int i = distance(right.begin(), itmult);
            temp::DoubleExprAST node1(stod(right[i - 1]));
            temp::DoubleExprAST node2(stod(right[i + 1]));
            temp::BinaryOpASt binnode('*', &node1, &node2);
            resultM = binnode.value();
            right.erase(right.begin() + i - 1, right.begin() + i + 2);
            right.insert(right.begin() + i - 1, to_string(resultM));
        }else if (itdiv != right.end()) {
            int i = distance(right.begin(), itdiv);
            temp::DoubleExprAST node1(stod(right[i - 1]));
            temp::DoubleExprAST node2(stod(right[i + 1]));
            temp::BinaryOpASt binnode('/', &node1, &node2);
            resultD = binnode.value();
            right.erase(right.begin() + i - 1, right.begin() + i + 2);
            right.insert(right.begin() + i - 1, to_string(resultD));
        }else {
            temp::DoubleExprAST node1(stod(right[0]));
            temp::DoubleExprAST node2(stod(right[2]));
            temp::BinaryOpASt binnode(right[1][0], &node1, &node2);
            finalResult = binnode.value();
            right.erase(right.begin(), right.begin() + 3);
            right.insert(right.begin(), to_string(finalResult));
        }
    }
    temp::nextToken();
    temp::newVars[name].value = finalResult;
    cout<<temp::newVars[name].name<<" = "<<finalResult<<temp::newVars[name].value<<endl;
}


int main () {
  vector<word> Tokens = Tokire();
  word eof;
  eof.name = "EOF";
  eof.token = -8;
  Tokens.push_back(eof);
  temp::toookens = Tokens;
  parserMain();
    // for(int test = 0; test<Tokens.size(); test++)
    //   cout<<"Value\t"<<Tokens[test].name<<"\t\tToken\t"<<Tokens[test].token<<"\tIndex\t"<<test<<endl;
  return 0;
} 
