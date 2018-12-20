//
// Created by ilya on 19.12.18.
//

#ifndef DIFFERENTIATOR_PARSER_H
#define DIFFERENTIATOR_PARSER_H

#include <memory>
#include <map>
#include "operation.h"

typedef unsigned long ul;

enum OperatorType {
    OP_BINARY, OP_UNARY, OP_PRIMARY
};

struct OperatorInExpression {
    size_t pos_;
    OperatorType type_;
    ul code_;
public:
    OperatorInExpression(size_t pos, OperatorType type, ul code);
};

template <class T, class U>
std::map<T, U> get_invert_map(const std::map<U, T>& m) {
    std::map<T, U> res;
    for (auto elem: m) {
        res[elem.second] = elem.first;
    }
    return res;
};

class Parser {
    std::string remove_outer_brackets(std::string expr) const;
    std::string remove_spaces(std::string expr) const;
    OperatorInExpression get_upper(std::string expr) const;
    OperatorInExpression get_binary_upper(std::string expr) const;
    OperatorInExpression get_unary_upper(std::string expr) const;
public:
    std::shared_ptr<Operation> parse(std::string expr) const;
};


#endif //DIFFERENTIATOR_PARSER_H
