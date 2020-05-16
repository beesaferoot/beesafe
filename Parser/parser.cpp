#include "parser.h"
using namespace parser;


unordered_map<TokenType, PRECENDENCE> Parser::precendences = {
   {TokenType::PLUS, PRECENDENCE::ADD},
   {TokenType::MINUS, PRECENDENCE::SUB},
   {TokenType::UMINUS, PRECENDENCE::USUB},
   {TokenType::ASTERISK, PRECENDENCE::MUL},
   {TokenType::DIV, PRECENDENCE::DIV}
};

unordered_map<TokenType, bool> Parser::BinaryOperators = {
    {TokenType::PLUS, true},
    {TokenType::MINUS, true},
    {TokenType::ASTERISK, true},
    {TokenType::DIV, true},
    {TokenType::LT, true},
    {TokenType::LT_EQ, true},
    {TokenType::GT, true},
    {TokenType::GT_EQ, true},
    {TokenType::NOT_EQ, true},
    {TokenType::EQ, true},
    {TokenType::ASSIGN, true},
};

Parser::Parser(Lexer* l)
    :l{l}
{
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
       case TokenType::IF:
           return parseIfStatement();
       case TokenType::FOR:
           return parserForStatement();
       case TokenType::DECLARE:
           return parseDeclStatement();
       case TokenType::INIT:
           return parseInitStatement();
       case TokenType::DEFINE:
           return parseFunctionStatement();
       case TokenType::RETURN:
            return parseReturnStatement();
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
    std::ostringstream message;
    if(!expectPeek(TokenType::ID)){
        message << "expected an identifier"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }

    stmt->target = new Identifier(curToken, l->line);
    if(!expectPeek(TokenType::IN)){
        message << "expected 'in'"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    stmt->iter = parseRangeExpression();
    stmt->body = parseBlockStatement();
    nextToken();
    return stmt;
}

RangeExpr* Parser::parseRangeExpression()
{
    std::ostringstream message;
    if(!expectPeek(TokenType::LPAREN)){
        message << "expected '('"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    RangeExpr* expr = new RangeExpr(curToken,l->line);
    if(!expectPeek(TokenType::NUM)){
        message << "expected a number"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    Num* num_ptr = static_cast<Num*>(curToken);
    expr->init_point = num_ptr->value;
    if(!expectPeek(TokenType::RANGE)){
        message << "expected '..'"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    if(!expectPeek(TokenType::NUM)){
        message << "expected a number"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    num_ptr = static_cast<Num*>(curToken);
    expr->end_point = num_ptr->value;
    if(!expectPeek(TokenType::RPAREN)){
        message << "expected ')'"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    return expr;

}

BlockStmt* Parser::parseBlockStatement()
{
    std::ostringstream message;
    BlockStmt* stmt = new BlockStmt();
    if(!expectPeek(TokenType::LBRACE)){
        message << "expected '{'"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    stmt->tok = curToken;
    while(!peekTokenIs(TokenType::RBRACE) && !peekTokenIs(TokenType::EOB)){
        nextToken();
        auto sptr = parseStatement();
        if(sptr != nullptr)
            stmt->Stmts.push_back(sptr);
    }
    if(!expectPeek(TokenType::RBRACE)){
        message << "expected '}'"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    return stmt;
}

InitStmt* Parser::parseInitStatement()
{
    std::ostringstream message;
    InitStmt* stmt = new InitStmt(curToken, l->line);
    if(!expectPeek(TokenType::ID)){
        message << "expected an identifier"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    stmt->variable = new Identifier(curToken, l->line);;
    if(!expectPeek(TokenType::ASSIGN)){
        message << "expected '='"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    nextToken();
    stmt->value = parseExpression();
    return stmt;
}

DeclareStmt* Parser::parseDeclStatement()
{
    std::ostringstream message;
    DeclareStmt* stmt = new DeclareStmt(curToken, l->line);
    while(!peekTokenIs(TokenType::NEWLINE) && !peekTokenIs(TokenType::EOB))
    {
        if(peekTokenIs(TokenType::ID)){
            nextToken();
            stmt->varList.push_back( new Identifier(curToken, l->line));
            if(peekTokenIs(TokenType::COMMA)){
                nextToken();
            }
        }else{
            message << "expected an identifier, found " << peekToken->Literal;
            throw  SyntaxError(message.str());
        }
    }
    nextToken();
    return stmt;
}

ReturnStmt* Parser::parseReturnStatement()
{
    ReturnStmt* stmt = new ReturnStmt(curToken, l->line);
    nextToken();
    stmt->returnValue = parseExpression();
    return stmt;
}

IfStmt* Parser::parseIfStatement()
{
    std::ostringstream message;
    IfStmt* stmt = new IfStmt(curToken, l->line);
    if(!expectPeek(TokenType::LPAREN)){
        message << "expected '('"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    nextToken();
    stmt->condition = parseExpression();
    if(!expectPeek(TokenType::RPAREN)){
        message << "expected ')'"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    stmt->body = parseBlockStatement();
    if(peekTokenIs(TokenType::ELSE)){
        nextToken();
        stmt->alternative = parseBlockStatement();
    }
    nextToken();
    return  stmt;
}

Expr* Parser::parseBoolean()
{
    if(curTokenIs(TokenType::TRUE))
        return new BooleanExpr(curToken, true, l->line);
    return new BooleanExpr(curToken, false, l->line);
}

FunctionStmt* Parser::parseFunctionStatement()
{
    FunctionStmt* stmt = new FunctionStmt(curToken, l->line);
    stmt->Expression = parseFunctionLiteral();
    auto function = static_cast<FunctionLiteral*>(stmt->Expression);
    if(function->header == nullptr){
        throw SyntaxError("function statement requires a name");
    }
    return stmt;
}

vector<Identifier*> Parser::parseFunctionParameters()
{
        std::ostringstream message;
        vector<Identifier*> idents;
        while(!peekTokenIs(TokenType::RPAREN)){

            if(peekTokenIs(TokenType::ID)){
                nextToken();
                idents.push_back( new Identifier(curToken, l->line));

                if(peekTokenIs(TokenType::COMMA)){
                    nextToken();
                }

            }else{
                message << "expected an identifier, found " << peekToken->Literal;
                throw  SyntaxError(message.str());
            }
        }
        return idents;
}

Expr* Parser::parseFunctionLiteral()
{
    std::ostringstream message;
    FunctionLiteral* expr = new FunctionLiteral(curToken, l->line);
    if(!peekTokenIs(TokenType::ID) && !peekTokenIs(TokenType::LPAREN)){
        message << "expected an identifier"
                << "  or '('"
             << " found " << peekToken->Literal;
        throw SyntaxError(message.str());
    }
    nextToken();
    if(curTokenIs(TokenType::LPAREN)){
        expr->header = nullptr;
        expr->parameters = parseFunctionParameters();

    }else{
         expr->header = new Identifier(curToken, l->line);
         if(!expectPeek(TokenType::LPAREN)){
             message << "expected '('"
                  << " found " << peekToken->Literal;
             throw SyntaxError(message.str());
         }
         expr->parameters = parseFunctionParameters();
    }
    nextToken();
    expr->body = parseBlockStatement();
    return expr;
}

CallExpression* Parser::parseCallExpression()
{
    CallExpression* call = new CallExpression(curToken, l->line);

    while(!peekTokenIs(TokenType::RPAREN)){
            nextToken();
            if(curTokenIs(TokenType::COMMA)){
              nextToken();
            }
            call->arguments.push_back(parseExpression());

    }
    nextToken();
    return call;
}

ExpressionStmt* Parser::parseExpressionStatement()
{
    ExpressionStmt* stmt = new ExpressionStmt(curToken, l->line);
    stmt->Expression = parseExpression();
    return  stmt;
}

Expr* Parser::parseExpression()
{
    E();
    auto e = Operands.top();
    Operands.pop();
    return e;

}

void Parser::E()
{
    Operators.push({PRECENDENCE::BASE, nullptr});
    produce();
    auto e = binary(peekToken->Type);
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
    Operators.pop();


}

void Parser::produce()
{
    std::ostringstream message;
    switch (curToken->Type) {
          case TokenType::NUM:
               Operands.push(mkLeaf(curToken));
               break;
          case TokenType::ID:
              Operands.push(mkLeaf(curToken));
              if(peekTokenIs(TokenType::LPAREN)){
                  nextToken();
                  Operands.push(parseCallExpression());
                  Operators.push({PRECENDENCE::CALL, new Operator(TokenType::CALL, "()")});
              }
              break;
          case TokenType::DEFINE:
              Operands.push(parseFunctionLiteral());
              if(peekTokenIs(TokenType::LPAREN)){
                  nextToken();
                  Operands.push(parseCallExpression());
                  Operators.push({PRECENDENCE::CALL, new Operator(TokenType::CALL, "()")});
              }
              break;
          case TokenType::LITERAL:
              Operands.push(mkLeaf(curToken));
              break;
          case TokenType::TRUE:
              Operands.push(parseBoolean());
              break;
          case TokenType::FALSE:
              Operands.push(parseBoolean());
              break;
          case TokenType::LPAREN:
               nextToken();
               E();
               if(!expectPeek(TokenType::RPAREN))
               {
                   message << "expected ')'"
                        << " found " << peekToken->Literal;
                   throw SyntaxError(message.str());
               }
               break;
         default:
            auto e = uniary(curToken->Type);
            if(e != nullptr){
                pushOperator(e, false);
                nextToken();
                produce();
                popOperator(false);
                break;
            }else{
                message << "invalid expression"
                     << " found " << curToken->Literal;
                throw SyntaxError(message.str());
            }

    }
}

Expr* Parser::mkLeaf(Token* t)
{
    std::ostringstream message;
    switch (t->Type) {
        case TokenType::NUM:
         {
            auto n = static_cast<Num*>(t);
            return new Number(n, n->value, l->line);
         }
         case TokenType::ID:
          {
             auto id = static_cast<Word*>(t);
             return new Identifier(id, l->line);
          }
          case TokenType::LITERAL:
          {
            return new StringLiteral(curToken, l->line);
          }

         default :
            message << "couldn't parse the terminal type " << t->Literal
                 << endl;
            throw SyntaxError(message.str());
    }
}

Expr* Parser::mkNode(Operator* op, Expr* e1, Expr* e2)
{
    std::ostringstream message;
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
        case TokenType::BANG:
          {
             auto nt = new Not(op, l->line);
             nt->RightOp = e2;
             return nt;
          }
        case TokenType::DIV:
          {
            auto div = new Div(op, l->line);
            div->LeftOp = e1;
            div->RightOp = e2;
            return div;
          }
         case TokenType::LT:
         {
             auto lt = new LessThan(op, l->line);
             lt->LeftOp = e1;
             lt->RightOp = e2;
             return lt;
         }
         case TokenType::LT_EQ:
          {
             auto lt_eq = new LessThanOrEqual(op, l->line);
             lt_eq->LeftOp = e1;
             lt_eq->RightOp = e2;
             return lt_eq;
          }
          case TokenType::GT:
           {
              auto gt = new GreaterThan(op, l->line);
              gt->LeftOp = e1;
              gt->RightOp = e2;
              return gt;
           }
           case TokenType::GT_EQ:
            {
               auto gt_eq = new GreaterThanOrEqual(op, l->line);
               gt_eq->LeftOp = e1;
               gt_eq->RightOp = e2;
               return gt_eq;
            }
            case TokenType::NOT_EQ:
             {
                auto nt_eq = new NotEquals(op, l->line);
                nt_eq->LeftOp = e1;
                nt_eq->RightOp = e2;
                return nt_eq;
             }
            case TokenType::EQ:
            {
               auto eq = new Equals(op, l->line);
               eq->LeftOp = e1;
               eq->RightOp = e2;
               return eq;
            }
            case TokenType::ASSIGN:
            {
               auto assign = new Assign(op, l->line);
               assign->LeftOp = e1;
               assign->RightOp = e2;
               return assign;
            }
            case TokenType::CALL:
             {
               auto call = static_cast<CallExpression*>(e2);
               call->function = e1;
               return call;
             }
        default:
             message << "couldn't parse operator type " << op->symbol
                   << endl;
              throw SyntaxError(message.str());
    }
}

Operator* Parser::binary(TokenType t)
{

    if(BinaryOperators[t]){
        return static_cast<Operator*>(peekToken);
    }else{
        return nullptr;
    }
}

Operator* Parser::uniary(TokenType t)
{
    switch (t) {
    case TokenType::MINUS:
       return new Operator(TokenType::UMINUS, "-");
    case TokenType::BANG:
        return static_cast<Operator*>(curToken);
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
      auto e2 = Operands.top();
      Operands.pop();
      auto e1 = Operands.top();
      Operands.pop();
      Operands.push(mkNode(Operators.top().second, e1, e2));
    }else{
        auto e2 = Operands.top();
        Operands.pop();
        Operands.push(mkNode(Operators.top().second, nullptr, e2));
    }
    Operators.pop();
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
        return false;
    }
}

bool Parser::curTokenIs(TokenType t)
{
    return curToken->Type == t;
}
