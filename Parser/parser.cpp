#include "parser.h"
#include <memory>
using namespace parser;


unordered_map<TokenType, PRECENDENCE> Parser::precendences = {
   {TokenType::PLUS, PRECENDENCE::ADD},
   {TokenType::MINUS, PRECENDENCE::SUB},
   {TokenType::UMINUS, PRECENDENCE::USUB},
   {TokenType::ASTERISK, PRECENDENCE::MUL},
   {TokenType::DIV, PRECENDENCE::DIV},
   {TokenType::EQ, PRECENDENCE::EQ},
    {TokenType::NOT_EQ, PRECENDENCE::NT_EQ},
    {TokenType::LT_EQ, PRECENDENCE::LT_EQ},
    {TokenType::GT_EQ, PRECENDENCE::GT_EQ},
    {TokenType::GT, PRECENDENCE::GT},
    {TokenType::LT, PRECENDENCE::LT},
    {TokenType::ASSIGN, PRECENDENCE::ASSIGN}
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
    peekToken = l->NextToken();

}

Stmt* Parser::parseStatement()
{
    switch (curToken.Type) {
       case TokenType::IF:
           return parseIfStatement();
       case TokenType::FOR:
           return parseForIntRangeStatement();
       case TokenType::DECLARE:
           return parseDeclStatement();
       case TokenType::INIT:
           return parseInitStatement();
       case TokenType::DEFINE:
           if(peekTokenIs(TokenType::LBRACE)){
               return parseExpressionStatement();
           }
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

    while(!curTokenIs(TokenType::EOB) && !hasErrors())
    {
        auto stmt = parseStatement();
        if (stmt != nullptr){
            program->Stmts.push_back(stmt);
        }
        nextToken();
    }
    return program;
}

ForStmt* Parser::parseForIntRangeStatement()
{
    ForStmt* stmt(new ForStmt(curToken, l->line));
    std::ostringstream message;
    if(!expectPeek(TokenType::ID)){
        message << "expected an identifier"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete stmt;
        return nullptr;
    }

    stmt->target = new Identifier(curToken, l->line);
    if(!expectPeek(TokenType::IN)){
        message << "expected 'in'"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete stmt;
        return nullptr;
    }
    stmt->iter = parseIntRangeExpression();
    stmt->body = parseBlockStatement();
    nextToken();
    return stmt;
}

RangeExpr* Parser::parseIntRangeExpression()
{
    std::ostringstream message;
    if(!expectPeek(TokenType::LPAREN)){
        message << "expected '('"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        return nullptr;
    }
    RangeExpr* expr = new RangeExpr(curToken,l->line);
    if(!expectPeek(TokenType::NUM)){
        message << "expected a number"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete expr;
        return nullptr;
    }
    expr->init_value = stoi(curToken.Literal);
    if(!expectPeek(TokenType::RANGE)){
        message << "expected '..'"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete expr;
        return nullptr;
    }
    if(!expectPeek(TokenType::NUM)){
        message << "expected a number"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete expr;
        return nullptr;
    }
    expr->end_value = stoi(curToken.Literal);
    if(!expectPeek(TokenType::RPAREN)){
        message << "expected ')'"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete expr;
        return nullptr;
    }
    return expr;

}

BlockStmt* Parser::parseBlockStatement()
{
    std::ostringstream message;
    BlockStmt* stmt = new BlockStmt(curToken);
    if(!expectPeek(TokenType::LBRACE)){
        message << "expected '{'"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete stmt;
        return nullptr;
    }
    while(!peekTokenIs(TokenType::RBRACE) && !peekTokenIs(TokenType::EOB)){
        nextToken();
        auto sptr = parseStatement();
        if(sptr != nullptr)
            stmt->Stmts.push_back(sptr);
    }
    if(!expectPeek(TokenType::RBRACE)){
        message << "expected '}'"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete stmt;
        return nullptr;
    }
    return stmt;
}

InitStmt* Parser::parseInitStatement()
{
    std::ostringstream message;
    InitStmt* stmt = new InitStmt(curToken, l->line);
    if(!expectPeek(TokenType::ID)){
        message << "expected an identifier"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete stmt;
        return nullptr;
    }
    stmt->variable = new Identifier(curToken, l->line);;
    if(!expectPeek(TokenType::ASSIGN)){
        message << "expected '='"
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete stmt;
        return nullptr;
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
            message << "expected an identifier, found " << peekToken.Literal;
            errors.push_back(message.str());
            delete stmt;
            return nullptr;
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
             << " found " << peekToken.Literal;
        errors.push_back(message.str());
        delete stmt;
        return nullptr;
    }
    nextToken();
    stmt->condition = parseExpression();
    if(!expectPeek(TokenType::RPAREN)){
        message << "expected ')'"
             << " found " <<  (peekToken.Literal.size() > 0 ? peekToken.Literal : "nothing");

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

Expr* Parser::parseNull()
{
    return new NullExpr(curToken, l->line);
}

FunctionStmt* Parser::parseFunctionStatement()
{
    FunctionStmt* stmt = new FunctionStmt(curToken, l->line);
    stmt->Expression = parseFunctionLiteral();
    auto function = dynamic_cast<FunctionLiteral*>(stmt->Expression);
    if(function->header == nullptr){
        errors.push_back("function statement requires a name");
        delete stmt;
        return nullptr;
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
                message << "expected an identifier, found " << peekToken.Literal;
                errors.push_back(message.str());
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
             << " found " << (peekToken.Literal.size() > 0 ? peekToken.Literal : "nothing");
        errors.push_back(message.str());
        delete expr;
        return nullptr;
    }
    nextToken();
    if(curTokenIs(TokenType::LPAREN)){
        expr->header = nullptr;
        expr->parameters = parseFunctionParameters();

    }else{
         expr->header = new Identifier(curToken, l->line);
         if(!expectPeek(TokenType::LPAREN)){
             message << "expected '('"
                  << " found " << (peekToken.Literal.size() > 0 ? peekToken.Literal : "nothing");
             errors.push_back(message.str());
             delete expr;
             return nullptr;
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
    if(stmt->Expression != nullptr)
        return  stmt;
    delete stmt;
    return nullptr;
}

Expr* Parser::parseExpression()
{
    expand();
    if(hasErrors())
        return nullptr;
    auto e = Operands.top();
    Operands.pop();
    return e;
}

void Parser::expand()
{
    Operators.push({PRECENDENCE::BASE, Operator(TokenType::NULLTOKEN, "")});
    produce();
    auto e = binary(peekToken.Type);
    while(e.Type != TokenType::ERROR)
    {
        nextToken();
        pushOperator(e, true);
        nextToken();
        produce();
        e = binary(peekToken.Type);
    }
    while(Operators.top().second.Type != TokenType::ERROR &&
          Operators.top().second.Type != TokenType::NULLTOKEN)
    {
        popOperator(true);
    }
    Operators.pop();

}

void Parser::produce()
{
    std::ostringstream message;
    switch (curToken.Type) {
          case TokenType::NUM:
               Operands.push(mkLeaf(curToken));
               break;
          case TokenType::ID:
              Operands.push(mkLeaf(curToken));
              if(peekTokenIs(TokenType::LPAREN)){
                  nextToken();
                  Operands.push(parseCallExpression());
                  Operators.push({PRECENDENCE::CALL, Operator(TokenType::CALL, "()")});
              }
              if(peekTokenIs(TokenType::ID)){
                  message << "invalid syntax"
                             << " found " << peekToken.Literal;
                  errors.push_back(message.str());
              }
              break;
          case TokenType::DEFINE:
              Operands.push(parseFunctionLiteral());
              if(peekTokenIs(TokenType::LPAREN)){
                  nextToken();
                  Operands.push(parseCallExpression());
                  Operators.push({PRECENDENCE::CALL, Operator(TokenType::CALL, "()")});
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
               expand();
               if(!expectPeek(TokenType::RPAREN))
               {
                   message << "expected ')'"
                        << " found " << (peekToken.Literal.size() > 0 ? peekToken.Literal : "nothing");
                   errors.push_back(message.str());
               }
               break;
         case TokenType::NULLTOKEN:
            Operands.push(parseNull());
            break;
         default:
            auto e = uniary(curToken.Type);
            if(e.Type != TokenType::ERROR){
                pushOperator(e, false);
                nextToken();
                produce();
                popOperator(false);
                break;
            }else{
                message << "invalid expression";
                errors.push_back(message.str());
            }

    }
}

Expr* Parser::mkLeaf(Token &t)
{
    std::ostringstream message;
    switch (t.Type) {
        case TokenType::NUM:
         {
            return new Number(t, stoi(t.Literal), l->line);
         }
         case TokenType::ID:
          {
             return new Identifier(t, l->line);
          }
          case TokenType::LITERAL:
          {
            return new StringLiteral(t, l->line);
          }

         default :
            message << "couldn't parse the terminal type " << t.Literal
                 << endl;
            errors.push_back(message.str());
            return nullptr;
    }
}

Expr* Parser::mkNode(Operator& op, Expr* e1, Expr* e2)
{
    std::ostringstream message;
    switch (op.Type) {
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
               auto call = dynamic_cast<CallExpression*>(e2);
               call->function = e1;
               return call;
             }
        default:
             message << "couldn't parse operator type " << op.symbol
                   << endl;
             errors.push_back(message.str());
             return nullptr;
    }
}

Operator Parser::binary(TokenType t)
{

    if(BinaryOperators[t]){
        return Operator(peekToken.Type, peekToken.Literal);
    }else{
        return Operator(TokenType::ERROR, curToken.Literal);
    }
}

Operator Parser::uniary(TokenType t)
{
    switch (t) {
    case TokenType::MINUS:
       return Operator(TokenType::UMINUS, "-");
    case TokenType::BANG:
        return Operator(TokenType::BANG, curToken.Literal);
    default:
        return Operator(TokenType::ERROR, curToken.Literal);
    }
}

void Parser::pushOperator(Operator& op, bool isBinary)
{
    int pLevel = precendences[op.Type];
    while(Operators.top().first >= pLevel)
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
    }else if(Operands.size() > 0){
        auto e2 = Operands.top();
        Operands.pop();
        Operands.push(mkNode(Operators.top().second, nullptr, e2));
    }
    Operators.pop();
}



bool Parser::peekTokenIs(TokenType t)
{
    return peekToken.Type == t;
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
    return curToken.Type == t;
}

std::vector<std::string> Parser::Errors() const {
    return this->errors;
}

bool Parser::hasErrors() const {
    if (errors.size() > 0)
        return true;
    return false;
}
