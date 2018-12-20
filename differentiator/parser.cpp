//
// Created by ilya on 19.12.18.
//

#include <assert.h>
#include "parser.h"

std::map<char, ul> binary_priorities = {
        {'+', 0}, {'-', 0}, {'*', 1}, {'/', 2}, {'^', 3}
};

std::map<char, ul> map_binary_operators = {
        {'+', BOP_SUM}, {'*', BOP_MUL}, {'/', BOP_DIV},
        {'^', BOP_DEG}, {'-', BOP_SUB}
};

OperatorInExpression::OperatorInExpression(size_t pos, OperatorType type, ul code)
        : pos_(pos), type_(type), code_(code) {}

//! PARSER
std::string Parser::remove_outer_brackets(std::string expr) const {
    size_t i = 0, j = expr.size() - 1;
    while (i < j && expr[i] == '(' && expr[j] == ')') {
        ++i; --j;
    }
    return expr.substr(i, j - i + 1);
}

std::string Parser::remove_spaces(std::string expr) const {
    std::string res;
    for (auto it = expr.begin(); it != expr.end(); ++it) {
        if (*it != ' ') {
            res.push_back(*it);
        }
    }
    return res;
}

OperatorInExpression Parser::get_binary_upper(std::string expr) const {
    int balance = 0;
    size_t upper_position = expr.size();
    ul code = BOP_UNDEF;
    for (size_t i = 0; i < expr.size(); ++i) {
        assert(balance >= 0);
        if (expr[i] == '(') {
            balance += 1;
        } else if (expr[i] == ')') {
            balance -= 1;
        }
        if (balance == 0 && map_binary_operators.find(expr[i]) !=
                                    map_binary_operators.end()) {
            if (upper_position == expr.size() ||
                    binary_priorities[expr[i]] <
                            binary_priorities[expr[upper_position]]) {
                upper_position = i;
                code = map_binary_operators[expr[i]];
            }
        }
    }
    assert(balance == 0);
    return OperatorInExpression(upper_position, OP_BINARY, code);
}

OperatorInExpression Parser::get_unary_upper(std::string expr) const {
    if (expr[0] == '-') {
        return OperatorInExpression(0, OP_UNARY, UOP_NEG);
    } else if (expr.substr(0, 3) == "sin") {
        return OperatorInExpression(0, OP_UNARY, UOP_SIN);
    } else if (expr.substr(0, 3) == "cos") {
        return OperatorInExpression(0, OP_UNARY, UOP_COS);
    } else if (expr.substr(0, 3) == "log") {
        return OperatorInExpression(0, OP_UNARY, UOP_LOG);
    }
    return OperatorInExpression(0, OP_UNARY, UOP_UNDEF);
}

OperatorInExpression Parser::get_upper(std::string expr) const {
    auto upper = get_binary_upper(expr);
    if (upper.code_ != BOP_UNDEF) {
        return upper;
    } else {
        upper = get_unary_upper(expr);
        if (upper.code_ != UOP_UNDEF) {
            return upper;
        } else {
            return OperatorInExpression(0, OP_PRIMARY, 0);
        }
    }
}

std::shared_ptr<Operation> Parser::parse(std::string expr) const {
    expr = remove_outer_brackets(remove_spaces(expr));
    auto upper = get_upper(expr);
    if (upper.type_ == OP_UNARY) {
        auto code = static_cast<UnaryOperator>(upper.code_);
        switch (upper.code_) {
            case UOP_NEG: {
                auto inner = parse(expr.substr(1));
                return std::make_shared<UnaryOperation>(UOP_NEG, inner);
            }
            case UOP_COS:
            case UOP_SIN: {
                auto inner = parse(expr.substr(3));
                return std::make_shared<UnaryOperation>(code, inner);
            }
            default:
                assert(false);
        }
    } else if (upper.type_ == OP_BINARY) {
        auto code = static_cast<BinaryOperator>(upper.code_);
        auto left = parse(expr.substr(0, upper.pos_));
        auto right = parse(expr.substr(upper.pos_ + 1));
        return std::make_shared<BinaryOperation>(code, left, right);
    } else {
        return std::make_shared<PrimaryExpression>(expr);
    }
}

