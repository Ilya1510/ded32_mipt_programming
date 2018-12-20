//
// Created by ilya on 18.12.18.
//

#ifndef DIFFERENTIATOR_OPERATION_H
#define DIFFERENTIATOR_OPERATION_H

#include <string>
#include <memory>

enum BinaryOperator {
    BOP_SUM = 0, BOP_MUL = 2, BOP_DIV = 3, BOP_DEG = 4,
    BOP_SUB = 1, BOP_UNDEF = 10
};

enum UnaryOperator {
    UOP_SIN = 1, UOP_COS = 2, UOP_NEG = 0, UOP_UNDEF = 10,
    UOP_LOG = 3
};

//! Общий абстрактный класс произвольного выражения
class Operation {
public:
    virtual std::shared_ptr<Operation> derivative(std::string var) const = 0;
    virtual std::string to_string() const = 0;
    virtual ~Operation() = default;
};

//! Унаследованный класс бинарного выражение
//! (Где верхняя операция бинарная)
class BinaryOperation : public Operation {
    std::shared_ptr<Operation> left_;
    std::shared_ptr<Operation> right_;
    BinaryOperator bop_;
public:
    BinaryOperation(BinaryOperator bop, std::shared_ptr<Operation> left,
                    std::shared_ptr<Operation> right);
    virtual std::shared_ptr<Operation> derivative(std::string var) const override;
    virtual std::string to_string() const override;
};

//! Унаследованный класс унарного выражение
class UnaryOperation : public Operation {
    std::shared_ptr<Operation> inner_;
    UnaryOperator uop_;
public:
    UnaryOperation(UnaryOperator uop, std::shared_ptr<Operation> inner);
    virtual std::shared_ptr<Operation> derivative(std::string var) const override;
    virtual std::string to_string() const override;
};

//! Класс простого выражения, не содержащего операций
//! является именем переменной или числом
class PrimaryExpression : public Operation {
    std::string val_;
public:
    PrimaryExpression(std::string val);
    virtual std::shared_ptr<Operation> derivative(std::string var) const override;
    virtual std::string to_string() const override;
};

#endif //DIFFERENTIATOR_OPERATION_H
