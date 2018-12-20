//
// Created by ilya on 14.12.18.
//

#include <stdlib.h>
#include <functional>
#include <iostream>

#pragma once

enum Level {
    L_ALL, L_DEBUG, L_INFO, L_ERR, L_NO,
};

/// \tparam T type of elements
/// \tparam LogStream type of log strim
/// works for std::cout and for std::ofstream
/// \tparam Hash hush function for type T
template <class T, class LogStream, class Hash = std::hash<T>>
class Stack {
public:
    static const size_t PRIME_BASE = 37;
    static constexpr const T DUMMY_ELEMENT = 7;
    size_t size_;
    size_t capacity_;
    size_t check_sum_;
    Hash hash_;
    LogStream& log_stream_;
    Level level_;
    T* arr_;

    void init() {
        arr_ = new T[4];
        size_ = 1;
        capacity_ = 4;
        arr_[0] = T();
        hash_ = Hash();
        check_sum_ = hash_(arr_[0]);
    }

    size_t get_check_sum() const {
        size_t check_sum = hash_(arr_[0]);
        for (size_t i = 1; i < size_; ++i) {
            check_sum *= PRIME_BASE;
            check_sum += hash_(arr_[i]);
        }
        return check_sum;
    }

    void assert_check_sum() const {
        if (check_sum_ != get_check_sum() || level_ <= L_DEBUG) {
            if (typeid(log_stream_) == typeid(std::cout)) {
                if (check_sum_ != get_check_sum()) {
                    log_stream_ << "\033[1;31mcheck_sum is incorrect\033[0m\n";
                }
                log_stream_ << "\033[1;33mold_check_sum = " << check_sum_ << "\033[0m\n";
                log_stream_ << "\033[1;33mnow_check_sum = " << get_check_sum() << "\033[0m\n";
            } else {
                if (check_sum_ != get_check_sum()) {
                    log_stream_ << "check_sum is incorrect\n";
                }
                log_stream_ << "old_check_sum = " << check_sum_ << "\n";
                log_stream_ << "now_check_sum = " << get_check_sum() << "\n";
            }
        }
    }

    void refresh_check_sum() {
        check_sum_ = get_check_sum();
    }

    void expand() {
        T* arr_new_ = new T[2 * capacity_];
        std::copy(arr_, arr_ + capacity_, arr_new_);
        delete[] arr_;
        arr_ = arr_new_;
        capacity_ *= 2;
    }

    template <class U>
    void print_push_back_start(U&& elem) {
        if (level_ <= L_INFO) {
            log_stream_ << "\033[1;34mpush_back(" << elem << ") begin\033[0m\n";
        }
    }

    template <class U>
    void print_push_back_end(U&& elem) {
        if (level_ <= L_INFO) {
            log_stream_ << "\033[1;34mpush_back(" << elem << ") end\033[0m\n";
        }
    }

    void print_pop_back_start() {
        if (level_ <= L_INFO) {
            log_stream_ << "\033[1;35mpop_back() begin\033[0m\n";
        }
    }

    void print_pop_back_end() {
        if (level_ <= L_INFO) {
            log_stream_ << "\033[1;35mpop_back() end\033[0m\n";
        }
    }

    void check_dummy_element() {
        if (level_ <= L_DEBUG) {
            if (arr_[size_ - 1] != DUMMY_ELEMENT) {
                log_stream_ << "\033[1;31mdummy element bad\033[0m\n";
            } else {
                log_stream_ << "\033[1;28mdummy element right\033[0m\n";
            }
        }
    }

    void print_extract_element() {
        if (level_ <= L_INFO) {
            log_stream_ << "\033[1;35mpop_back() elem = " << arr_[size_ - 1] << "\033[0m\n";
        }
    }

    void print_stack() {
        if (level_ <= L_ALL) {
            print();
        }
    }

public:
    template <class U>
    Stack(U&& log_stream, Level level) :
            log_stream_(std::forward<U>(log_stream)),
            level_(level) {
        init();
    };

    template <class U>
    void push_back(U&& elem) {
        print_push_back_start(std::forward<U>(elem));
        assert_check_sum();

        if (size_ == capacity_) {
            expand();
        }
        arr_[size_++] = std::forward<U>(elem);

        if (size_ == capacity_) {
            expand();
        }
        arr_[size_++] = DUMMY_ELEMENT;

        refresh_check_sum();
        print_push_back_end(elem);
    }

    T pop_back() {
        print_pop_back_start();
        assert_check_sum();
        if (size_ == 1) {
            log_stream_ << "\033[1;45mno elements in stack!\033[0m\n";
            return DUMMY_ELEMENT;
        }

        check_dummy_element();

        arr_[size_ - 1].~T();
        --size_;

        print_extract_element();

        T&& elem = std::move(arr_[size_ - 1]);
        --size_;
        refresh_check_sum();
        print_pop_back_end();
        return elem;
    }

    const size_t get_size() const {
        return size_ - 1;
    }

    void print() {
        for (int i = 1; i < size_; ++i) {
            log_stream_ << arr_[i] << " ";
        }
        log_stream_ << std::endl;
    }

    ~Stack() {
        for (size_t i = 0; i < size_; ++i) {
            arr_[i].~T();
        }
        delete[] arr_;
    }
};
