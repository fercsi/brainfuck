/// # Brainfuck by FERcsI
//    ══════════════════════════════════════════════════════
//       Brainfuck interpreter
//       Copyright (c) 2023 Ferenc Vajda
//     ────────────────────────────────────────────────────
//       brainfuck.cpp
//       Source code of brainfuck interpreter
//     ────────────────────────────────────────────────────
//       This code is licensed under the MIT license. See
//       the LICENSE file in the project root for license
//       terms.
//    ══════════════════════════════════════════════════════    */
//-----------------------------------------------------------------File info-//[
//@ > - **File**: brainfuck.cpp
//@ > - **Author**: VAJDA, Ferenc
//@ > - **Last modified**: 2023-03-02T18:09:39+0000
//]

#include <array>
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

static std::string VERSION = "1.0-beta1";

static constexpr int CELL_ARRAY_SIZE = 30'000;
static constexpr int START_CELL_POS = 0;
static constexpr int DEFAULT_DUMP_SIZE = 32;
using CellType = uint8_t;

// ======== EXCEPTIONS ========
class BrainfuckSyntaxError: public std::logic_error {
public:
    explicit BrainfuckSyntaxError(const char* what_arg): std::logic_error(what_arg) {}
};

class BrainfuckRuntimeError: public std::runtime_error {
public:
    explicit BrainfuckRuntimeError(const char* what_arg): std::runtime_error(what_arg) {}
};


// ======== BRAINFUCK INTERPRETER CLASS ========
class Brainfuck {
    std::array<CellType, CELL_ARRAY_SIZE> cell_array;
    size_t cell_ptr;
    size_t code_ptr;
    const std::vector<char>& code;
    std::vector<size_t> jump_addresses;
    size_t dump_range_start{0};
    size_t dump_range_length{DEFAULT_DUMP_SIZE};

public:
    Brainfuck(const std::vector<char>& code) : code(code) {
        std::stack<size_t> bracket_stack;
        jump_addresses = std::vector<size_t>(code.size(), 0);
        for (auto& ch : code) {
            size_t cur = &ch - &code[0];
            if (ch == '[') {
                bracket_stack.push(cur);
            } else if (ch == ']') {
                if (bracket_stack.size() == 0) {
                    throw BrainfuckSyntaxError("Unmatched ']' found");
                }
                size_t prev = bracket_stack.top();
                bracket_stack.pop();
                jump_addresses[prev] = cur;
                jump_addresses[cur] = prev;
            }
        }
        if (bracket_stack.size() != 0) {
            throw BrainfuckSyntaxError("Unmatched '[' found");
        }
        this->reset();
    }

    void reset() {
        cell_array.fill(0);
        cell_ptr = START_CELL_POS;
        code_ptr = 0;
    }

    void set_dump_range(size_t start, size_t length) {
        this->dump_range_start = start;
        this->dump_range_length = length;
    }

    std::string get_code() const {
        std::string code_str = std::string(this->code.begin(), this->code.end());
        return code_str;
    }

    void run(std::ostream& out_stream, std::istream& in_stream) {
        while (code_ptr < code.size()) {
            switch (code[code_ptr]) {
                case '<':
                    if (cell_ptr == 0) {
                        throw BrainfuckRuntimeError("Negative cell array index");
                    }
                    --cell_ptr;
                    break;
                case '>':
                    ++cell_ptr;
                    if (cell_ptr == cell_array.size()) {
                        throw BrainfuckRuntimeError("Cell array index out of range");
                    }
                    break;
                case '+':
                    cell_array[cell_ptr]++;
                    break;
                case '-':
                    cell_array[cell_ptr]--;
                    break;
                case '[':
                    if (cell_array[cell_ptr] == 0) {
                        code_ptr = jump_addresses[code_ptr];
                    }
                    break;
                case ']':
                    if (cell_array[cell_ptr] != 0) {
                        code_ptr = jump_addresses[code_ptr];
                    }
                    break;
                case '.':
                    out_stream << cell_array[cell_ptr] << std::flush;
                    break;
                case ',':
                    in_stream.read((char*)&cell_array[cell_ptr], 1);
                    break;
                case '@':
                    this->dump(this->dump_range_start, this->dump_range_start + this->dump_range_length);
                    break;
                case '$':
                    std::cerr << "[exit]\n";
                    return;
            }
            ++code_ptr;
        }
    }

    void dump(size_t end=DEFAULT_DUMP_SIZE) {
        this->dump(START_CELL_POS, START_CELL_POS + end);
    }
    void dump(size_t start, size_t end) {
        std::cerr << "[";
        start += START_CELL_POS;
        end += START_CELL_POS;
        for (int i = start; i < end; ++i) {
            if (i == cell_ptr) {
                std::cerr << "\x1b[1;31m";
            }
            std::fprintf(stderr, " %.2x", this->cell_array[i]);
            if (i == cell_ptr) {
                std::cerr << "\x1b[0m";
            }
        }
        std::cerr << "... ]" << std::endl;
    }
};


// ======== MAIN PROGRAM ========
bool opt_debug = false;
bool opt_minify = false;
int opt_dump_range_length = DEFAULT_DUMP_SIZE;
std::vector<std::string> files;

bool read_program(std::vector<char>& program, const std::string& filename) {
    std::string instructions = "<>+-[].,";
    if (opt_debug) {
        instructions += "@$";
    }
    std::fstream bf_source(filename, std::fstream::in);
    if (bf_source.fail()) {
        std::fprintf(stderr, "Could not open file '%s'\n", filename.c_str());
        return true;
    }
    bool comment = false;
    char ch;
    // noskipws is necessary for checking comment ending
    while (bf_source >> std::noskipws >> ch) {
        if (opt_debug && (ch == '#' || comment && ch != '\n')) {
            comment = true;
            continue;
        }
        comment = false;
        if (instructions.find(ch) != std::string::npos) {
            program.push_back(ch);
        }
    }
    return false;
}

std::pair<bool, int> parse_options(int argc, char* argv[]) {
    const std::vector<std::string_view> args(argv + 1, argv + argc);
    int param_cnt = 0;
    for (const auto &arg : args) {
        if (param_cnt) {
            --param_cnt;
            // Currently, there is only a single option with parameter
            opt_dump_range_length = std::stoi(arg.data());
        } else if (arg[0] == '-') {
            if (arg == "-d" || arg == "--debug") {
                opt_debug = true;
            } else if (arg == "-s" || arg == "--dump-size") {
                param_cnt = 1;
            } else if (arg == "--minify") {
                opt_minify = true;
            } else if (arg == "-h" || arg == "--help") {
                std::cout << "Usage: brainfuck [-d] SCRIPTFILE [INPUTFILE]\n";
                std::cout << "Options:\n";
                std::cout << "  -d, --debug\n";
                std::cout << "          Debug mode enabled. Additional instructions:\n";
                std::cout << "          @ Memory dump of first cells (default: 32), highlighting the\n";
                std::cout << "            current one\n";
                std::cout << "          $ Exit program immediately\n";
                std::cout << "  -s, --dump-size LENGTH\n";
                std::cout << "          In debug mode, this value determins, how many cells are\n";
                std::cout << "          shown in cell array dump.\n";
                std::cout << "  --minify\n";
                std::cout << "          Does not run brainfuck code, but prints minified code of\n";
                std::cout << "          input. If using with -d, comments and debug isntructions\n";
                std::cout << "          are also considered\n";
                std::cout << "  --version\n";
                std::cout << "          Version information\n";
                std::cout << "  -h, --help\n";
                std::cout << "          This help\n";
                std::cout << "Input: INPUTFILE or stdin\n";
                std::cout << "Output: stdout\n\n";
                std::cout << "Classic Brainfuck implementation with 30000 byte (i.e. 8 bit unsigned\n";
                std::cout << "integer) cell array and one-direction addressing.\n\n";
                std::cout << "Additional feature is commenting (from `#` until the end of line) and\n";
                std::cout << "additional debug instructions if debug mode is enabled.\n";
                return std::make_pair(true, 0);
            } else if (arg == "--version") {
                std::cout << "Brainfuck " << VERSION << " by FERcsI" << std::endl;
                return std::make_pair(true, 0);
            } else {
                std::fprintf(stderr, "Invalid argument %s\n", arg.data());
                return std::make_pair(true, 1);
            }
        } else {
            files.push_back(std::string(arg.data()));
        }
    }
    if (param_cnt) {
        std::fprintf(stderr, "Option parameter is missing\n");
        return std::make_pair(true, 1);
    }
    if (files.size() == 0) {
        std::fprintf(stderr, "Brainfuck source file is missing\n");
        return std::make_pair(true, 1);
    } else if (files.size() > 2) {
        std::fprintf(stderr, "Too many arguments\n");
        return std::make_pair(true, 1);
    }
    return std::make_pair(false, 0);
}

int main(int argc, char* argv[]) {
    auto [exit, retval] = parse_options(argc, argv);
    if (exit) {
        return retval;
    }
    std::vector<char> program{};
    if (read_program(program, files[0])) {
        return 2;
    }
    Brainfuck bf(program);
    if (opt_minify) {
        std::cout << bf.get_code() << std::endl;
    } else {
        bf.set_dump_range(0, opt_dump_range_length);
        if (files.size() == 2) {
            std::fstream input(files[1], std::fstream::in);
            bf.run(std::cout, input);
        } else {
            bf.run(std::cout, std::cin);
        }
    }
    return 0;
}
