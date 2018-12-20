//
// Created by ilya on 18.12.18.
//

#include <assert.h>
#include "operation.h"
#include "parser.h"

std::map<std::string, UnaryOperator> map_unary_operators = {
        {"-", UOP_NEG}, {"sin", UOP_SIN}, {"cos", UOP_COS}, {"log", UOP_LOG}
};

std::map<BinaryOperator, std::string> map_binary_codes = {
        {BOP_SUM, "+"}, {BOP_MUL, "*"}, {BOP_DIV, "/"},
        {BOP_DEG, "^"}, {BOP_SUB, "-"}
};

BinaryOperation::BinaryOperation(
        BinaryOperator bop, std::shared_ptr<Operation> left,
        std::shared_ptr<Operation> right)
        : bop_(bop), left_(left), right_(right) {}

std::shared_ptr<Operation> BinaryOperation::derivative(std::string var) const {
    auto left_derivative = left_->derivative(var);
    auto right_derivative = right_->derivative(var);
    switch (bop_) {
        case BOP_SUM: {
            return std::make_shared<BinaryOperation>(
                    BOP_SUM, left_derivative, right_derivative);
        }
        case BOP_SUB: {
            return std::make_shared<BinaryOperation>(
                    BOP_SUB, left_derivative, right_derivative);
        }
        case BOP_MUL: {
            auto left = std::make_shared<BinaryOperation>(
                    BOP_MUL, left_derivative, right_);
            auto right = std::make_shared<BinaryOperation>(
                    BOP_MUL, right_derivative, left_);
            return std::make_shared<BinaryOperation>(
                    BOP_SUM, left, right);
        }
        case BOP_DIV: {
            auto left = std::make_shared<BinaryOperation>(
                    BOP_MUL, left_derivative, right_);
            auto right = std::make_shared<BinaryOperation>(
                    BOP_MUL, right_derivative, left_);
            auto up = std::make_shared<BinaryOperation>(
                    BOP_SUB, left, right);
            auto down = std::make_shared<BinaryOperation>(
                    BOP_DEG, right_, std::make_shared<PrimaryExpression>("2"));
            return std::make_shared<BinaryOperation>(
                    BOP_DIV, up, down);
        }
        case BOP_DEG: {
            auto left = std::make_shared<BinaryOperation>(
                    BOP_DEG, left_, right_);
            auto right_left = std::make_shared<BinaryOperation>(
                    BOP_DIV, right_, left_);
            auto log_left = std::make_shared<UnaryOperation>(
                    UOP_LOG, left_);
            auto right_right = std::make_shared<BinaryOperation>(
                    BOP_MUL, right_derivative, log_left);
            auto right = std::make_shared<BinaryOperation>(
                    BOP_SUM, right_left, right_right);
            return std::make_shared<BinaryOperation>(
                    BOP_MUL, left, right);
        }
        default:
            assert(false);
    }
}

std::string BinaryOperation::to_string() const {
    auto left = left_->to_string();
    auto right = right_->to_string();
    // Некоторые вшитые по уменьшению размера эвристики
    // В будущем их можно вынести в отдельную функцию
    if (map_binary_codes[bop_] == "*") {
        if (left == "0" || right == "0") {
            return "0";
        }
    }
    if (map_binary_codes[bop_] == "+") {
        if (left == "0") {
            return right;
        } else if (right == "0") {
            return left;
        }
    }
    if (left.size() == 1 && right.size() == 1) {
        return left + map_binary_codes[bop_] + right;
    }
    return "(" + left + ")" + map_binary_codes[bop_] + "(" + right + ")";
}

//! UNARY_OPERATION
UnaryOperation::UnaryOperation(
        UnaryOperator uop, std::shared_ptr<Operation> inner)
        : uop_(uop), inner_(inner) {}

std::shared_ptr<Operation> UnaryOperation::derivative(std::string var) const {
    auto in_derivative = inner_->derivative(var);
    switch (uop_) {
        case UOP_SIN: {
            auto first = std::make_shared<UnaryOperation>(UOP_COS, inner_);
            auto second = in_derivative;
            return std::make_shared<BinaryOperation>(BOP_MUL, first, second);
        }
        case UOP_COS: {
            auto first = std::make_shared<UnaryOperation>(UOP_SIN, inner_);
            auto second = std::make_shared<UnaryOperation>(UOP_NEG, in_derivative);
            return std::make_shared<BinaryOperation>(BOP_MUL, first, second);
        }
        case UOP_NEG: {
            return std::make_shared<UnaryOperation>(UOP_NEG, in_derivative);
        }
        case UOP_LOG: {
            auto one = std::make_shared<PrimaryExpression>("1");
            auto first = std::make_shared<BinaryOperation>(
                    BOP_DIV, one, inner_);
            return std::make_shared<BinaryOperation>(BOP_MUL, first, in_derivative);
        }
        default: {
            assert(false);
        }
    }
}

std::string UnaryOperation::to_string() const {
    auto invert_unary = get_invert_map(map_unary_operators);
    std::string inner_string = inner_->to_string();
    return invert_unary[uop_] + "(" + inner_string + ")";
}

//! PRIMARY_EXPRESSION
PrimaryExpression::PrimaryExpression(std::string val)
        : val_(val) {}

std::shared_ptr<Operation> PrimaryExpression::derivative(std::string var) const {
    std::string result = var == val_ ? "1" : "0";
    auto shared = std::make_shared<PrimaryExpression>(result);
    return shared;
}

std::string PrimaryExpression::to_string() const {
    return val_;
}