#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <cstring>
#include <fstream>

template <class Char>
class Poet {
    Char* text;
    long file_size;
    std::vector <int> pointers_to_strings;

    void fill_pointers() {
        pointers_to_strings.push_back(0);
        Char* cur_char = text;
        Char* end = text + file_size + 1;
        int i = 0;
        while (cur_char != end) {
            if (*cur_char == '\n') {
                pointers_to_strings.push_back(i + 1);
                *cur_char = 0;
            }
            ++cur_char;
            ++i;
        }
        std::cout << "vector size: " << pointers_to_strings.size() << std::endl;
    }

    std::vector<int> get_pointers_to_ends() const {
        std::vector<int> ends;
        for (size_t i = 1; i < pointers_to_strings.size(); ++i) {
            ends.push_back(pointers_to_strings[i] - 2);
        }
        ends.push_back(file_size - 1);
        return ends;
    }

    std::vector<int> get_pointers_to_starts(std::vector<int> ends) const {
        for (int i = 0; i < ends.size(); ++i) {
            while (ends[i] != 0 && text[ends[i]] != 0 && text[ends[i] - 1] != 0) {
                --ends[i];
            }
        }
        return ends;
    }

    bool is_punctuation(char c) {
        return c == ',' || c == '.' || c == ':'
                || c == ';' || c == '?' || c == '!'
                || c == '-' || c == '"' || c == ' ';
    }

    int skip_punctuation(int x) {
        while (x >= 0 && is_punctuation(text[x])) {
            --x;
        }
        return x;
    }

public:
    Poet(std::string filename) {
        FILE* fp = std::fopen(filename.c_str(), "r");
        std::fseek(fp, 0, SEEK_END); // seek to end
        file_size = std::ftell(fp);
        std::cout << "filesize: " << file_size << std::endl;

        std::fseek(fp, 0, SEEK_SET); // seek to start
        text = new Char[file_size + 1];
        std::fread(text, sizeof(text[0]),
                   static_cast<size_t>(file_size), fp);
        text[file_size] = 0;
        fill_pointers();
        std::fclose(fp);
    }

    template <class Comp>
    void sort_straight(Comp comp) {
        std::sort(pointers_to_strings.begin(), pointers_to_strings.end(),
                  [&](int x, int y) {
                      return comp(text + x, text + y);});
    }

    template <class Comp>
    void sort_reversed(Comp comp) {
        auto&& ends = get_pointers_to_ends();
        std::sort(ends.begin(), ends.end(),
                  [&](int x, int y) {
                      x = skip_punctuation(x);
                      y = skip_punctuation(y);
                      return comp(text + x, text + y);});
        pointers_to_strings = std::move(get_pointers_to_starts(ends));
    }

    void fprint(char* filename, size_t begin, size_t end) {
        FILE* fp = fopen(filename, "w");
        for (size_t i = begin;
             i < std::min(end, pointers_to_strings.size()); ++i) {
            fprintf(fp, "%s\n", text + pointers_to_strings[i]);
        }
        fclose(fp);
    }

    void create(std::string type) const {
        int x[2] = {0, 0};
        x[0] = rand() % 5000 + 1000;
        x[1] = rand() % 5000 + 1000;
        for (int i = 0; i < type.size(); ++i) {
            printf("%s\n", text + pointers_to_strings[x[type[i] - 'A']++]);
        }
    }

    ~Poet() {
        delete[] text;
    }
};

template <class T>
class TStringComp {
public:
    bool operator()(T* w1, T* w2) const {
        for (int i1 = 0, i2 = 0; ; ++i1, ++i2) {
            if (w2[i2] == 0) {
                return false;
            }
            if (w1[i1] == 0) {
                return true;
            }
            if (w1[i1] < w2[i2]) {
                return true;
            } else if (w2[i2] < w1[i1]) {
                return false;
            }
        }
    }
};

template <class T>
class TStringCompReverse {
public:
    bool operator()(T* w1, T* w2) const {
        for (int i1 = 0, i2 = 0; ; --i1, --i2) {
            if (w2[i2] == 0) {
                return false;
            }
            if (w1[i1] == 0) {
                return true;
            }
            if (w1[i1] < w2[i2]) {
                return true;
            } else if (w2[i2] < w1[i1]) {
                return false;
            }
        }
    }
};

int main() {
    Poet<char> poet("onegin.txt");
    poet.sort_reversed(TStringCompReverse<char>());
    poet.fprint("new_onegin.txt", 0, 10000);
    while (true) {
        std::string type;
        std::cin >> type;
        if (type[0] == '0') {
            break;
        }
        poet.create(type);
    }
    return 0;
}