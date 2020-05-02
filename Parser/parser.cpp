#include "parser.h"
using namespace parser;

unordered_map<TokenType, PRECENDENCE> Parser::precendences = {
   {TokenType::PLUS, PRECENDENCE::ADD},
   {TokenType::MINUS, PRECENDENCE::SUB},
   {TokenType::UMINUS, PRECENDENCE::USUB},
   {TokenType::ASTERISK, PRECENDENCE::MUL},
   {TokenType::DIV, PRECENDENCE::DIV}
};

Parser::Parser(Lexer* l)
    :l{l}
{
    cout << "INIT PARSER\n";
}

Parser::~Parser()
{
    delete curToken;
    delete peekToken;
    delete l;
}

Parser* Parser::New(Lexer *l)
{
    Parser* p = new Parser(l);
    p->nextToken();
    p->nextToken();
    return p;
}

void Parser::nextToken()
{

    curToken = peekToken;
    peekToken = nullptr;
    peekToken = l->NextToken();

}

Stmt* Parser::parseStatement()
{
    switch (curToken->Type) {
       case TokenType::FOR:
           return parserForStatement();
       case TokenType::DECLARE:
           return parseDeclStatement();
       case TokenType::INIT:
           return parseInitStatement();
       case TokenType::NEWLINE:
             return nullptr;
       default:
           return parseExpressionStatement();
    }
}

Program* Parser::parseProgram()
{
    Program* program = new Program();
    program->Stmts = vector<Stmt*>{};

    while(!curTokenIs(TokenType::EOB))
    {
        auto stmt = parseStatement();
        if (stmt != nullptr){
            program->Stmts.push_back(stmt);
        }
        nextToken();
    }
    return program;
}

ForStmt* Parser::parserForStatement()
{
    ForStmt* stmt = new ForStmt(curToken, l->line);
    if(!expectPeek(TokenType::ID)){
        throw  "syntax error";
    }

    stmt->target = new Identifier(curToken, l->line);
    if(!expectPeek(TokenType::IN)){
        throw  "syntax error";
    }
    stmt->iter = parseRangeExpression();
    stmt->body = parseBlockStatement();
    nextToken();
    return stmt;
}

BlockStmt* Parser::parseBlockStatement()
{
    BlockStmt* stmt = new BlockStmt();
    if(!expectPeek(TokenType::LBRACE)){
        throw "syntax error";
    }
    stmt->tok = curToken;
    while(!peekTokenIs(TokenType::RBRACE)){
        nextToken();
        auto sptr = parseStatement();
        if(sptr != nullptr)
            stmt->Stmts.push_back(sptr);
    }
    nextToken();
    return stmt;
}

InitStmt* Parser::parseInitStatement()
{
    InitStmt* stmt = new InitStmt(curToken, l->line);
    if(!expectPeek(TokenType::ID)){
        throw "syntax error";
    }
    stmt->variable = new Identifier(curToken, l->line);;
    if(!expectPeek(TokenType::ASSIGN)){
        throw "syntax error";
    }
    nextToken();
    stmt->value = parseExpression();
    return stmt;
}

DeclareStmt* Parser::parseDeclStatement()
{
    DeclareStmt* stmt = new DeclareStmt(curToken, l->line);

    while(!peekTokenIs(TokenType::NEWLINE))
    {
        if(peekTokenIs(TokenType::ID)){
            nextToken();
            stmt->varList.push_back( new Identifier(curToken, l->line));

            if(!peekTokenIs(TokenType::NEWLINE) && !peekTokenIs(TokenType::COMMA)){
                cerr << "expected a new line, found " << peekToken->Literal << endl;
                throw  "syntax error";
            }else{
                if(peekTokenIs(TokenType::COMMA)){
                    nextToken();
                }
            }
        }else{
            cerr << "expected an identifier, found " << peekToken->Literal << endl;
            throw  "syntax error";
        }
    }
    nextToken();
    return stmt;
}

ExpressionStmt* Parser::parseExpressionStatement()
{
    ExpressionStmt* stmt = new ExpressionStmt(curToken, l->line);
    stmt->Expression = parseExpression();
    nextToken();
    return  stmt;
}

Expr* Parser::parseExpression()
{
    Operators.push({PRECENDENCE::BASE, nullptr});
    E();
    if(!peekTokenIs(TokenType::NEWLINE) && !peekTokenIs(TokenType::EOB))
    {
        throw "Syntax Error";
    }
    auto e = Operands.top();
    Operands.pop();
    Operators.pop();
    return e;

}

void Parser::E()
{
    produce();
    Operator* e=nullptr;
    e = binary(peekToken->Type);
    while(e != nullptr)
    {

        nextToken();
        pushOperator(e, true);
        nextToken();
        produce();
        e = binary(peekToken->Type);
    }


    while(Operators.top().second != nullptr)
    {
        popOperator(true);
    }


}

void Parser::produce()
{
    switch (curToken->Type) {
          case TokenType::NUM:
                Operands.push(mkLeaf(curToken));
                break;
          case TokenType::ID:
              Operands.push(mkLeaf(curToken));
              break;
          case TokenType::LPAREN:
               nextToken();
               E();
               if(!expectPeek(TokenType::RPAREN))
               {
                   throw  "Syntax Error";
               }
               break;
         default:
            Operator* e=nullptr;
            e = uniary(curToken->Type);
            if(e != nullptr){
                pushOperator(e, false);
                produce();
                break;
            }

    }
}

Expr* Parser::mkLeaf(Token* t)
{
    switch (t->Type) {
        case TokenType::NUM:
         {
            auto n = (Num*)t;
            return new Number(n, n->value, l->line);
         }
         case TokenType::ID:
          {
             auto id = (Word*)t;
             return new Identifier(id, l->line);
          }
         default :
            cerr << "couldn't parse the terminal type " << t->Literal
                 << endl;
            throw "Syntax error";
    }
}

Expr* Parser::mkNode(Operator* op, Expr* e1, Expr* e2)
{

    switch (op->Type) {
         case TokenType::PLUS:
             {
              auto add = new Add(op, l->line);
              add->LeftOp = e1;
              add->RightOp = e2;
              return add;
             }
        case TokenType::ASTERISK:
             {
              auto mul = new Mult(op, l->line);
              mul->LeftOp = e1;
              mul->RightOp = e2;
              return mul;
             }
        case TokenType::MINUS:
           {
             auto sub = new Sub(op, l->line);
             sub->LeftOp = e1;
             sub->RightOp = e2;
             return sub;
           }
        case TokenType::UMINUS:
           {
             auto usub = new UniarySub(op, l->line);
             usub->RightOp = e2;
             return usub;
           }
        case TokenType::DIV:
          {
            auto div = new Div(op, l->line);
            div->RightOp = e2;
            return div;
          }
        default:
             cerr << "couldn't parse operator type " << op->symbol
                   << endl;
              throw  "Syntax error";
    }
}

Operator* Parser::binary(TokenType t)
{
    switch (t) {
         case TokenType::PLUS:
             return new Operator(TokenType::PLUS, "+");
        case TokenType::ASTERISK:
             return new Operator(TokenType::ASTERISK, "*");
        case TokenType::MINUS:
             return new Operator(TokenType::MINUS, "-");
        case TokenType::DIV:
             return new Operator(TokenType::DIV, "/");
        default:
           return nullptr;
    }
}

Operator* Parser::uniary(TokenType t)
{
    switch (t) {
    case TokenType::UMINUS:
       return new Operator(TokenType::UMINUS, "-");
    default:
        return nullptr;
    }
}

void Parser::pushOperator(Operator* op, bool isBinary)
{
    int pLevel = precendences[op->Type];
    while(Operators.top().first > pLevel)
    {
        popOperator(isBinary);
    }
    Operators.push({pLevel, op});
}

void Parser::popOperator(bool isBinary)
{

    if(Operands.size() >= 2 && isBinary)
    {
      auto e1 = Operands.top();
      Operands.pop();
      auto e2 = Operands.top();
      Operands.pop();
      Operands.push(mkNode(Operators.top().second, e1, e2));
    }else{
        auto e1 = Operands.top();
        Operands.pop();
        Operands.push(mkNode(Operators.top().second, nullptr, e1));
    }
    Operators.pop();
}

RangeExpr* Parser::parseRangeExpression()
{

    if(!expectPeek(TokenType::LPAREN)){
        throw  "syntax error";
    }
    RangeExpr* expr = new RangeExpr(curToken,l->line);
    if(!expectPeek(TokenType::NUM)){
        throw "syntax error";
    }
    Num* num_ptr = (Num*)curToken;
    expr->init_point = num_ptr->value;
    if(!expectPeek(TokenType::RANGE)){
        throw "syntax error";
    }
    if(!expectPeek(TokenType::NUM)){
        throw "syntax error";
    }
    num_ptr = (Num*)curToken;
    expr->end_point = num_ptr->value;
    if(!expectPeek(TokenType::RPAREN)){
        throw "syntax error";
    }
    expr->type = new RangeGen(expr->init_point, expr->end_point);
    return expr;

}

bool Parser::peekTokenIs(TokenType t)
{
    return peekToken->Type == t;
}

bool Parser::expectPeek(TokenType t)
{
    if(peekTokenIs(t)){
        nextToken();
        return true;
    }else{
        cerr << "expected TokenType " << t
             << " found " << peekToken->Type << endl;
        return false;
    }
}

bool Parser::curTokenIs(TokenType t)
{
    return curToken->Type == t;
}
