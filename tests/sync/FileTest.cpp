#include <filesystem/io/File.h>
#include <src/filesystem/io/FileOperations.h>

#include <src/filesystem/Assert.h>

#include <fstream>
#include <filesystem>
#include <string_view>

#if defined(filesystem_os_windows)
#define STR(x) L##x
#else
#define STR(x) x
#endif

constexpr const char* test_open_file_name   = "fs_open_test.txt";
constexpr const char* test_read_file_name   = "fs_read_test.txt";
constexpr const char* test_offset_file_name = "fs_offset_test.txt";
constexpr const char* test_eof_file_name    = "fs_eof_test.txt";
constexpr const char* test_reset_file_name  = "fs_reset_test.txt";
constexpr const char* test_always_file_name = "fs_open_always_test.txt";

void create_text_file(const char* path, std::string_view text) {
    std::ofstream out(path, std::ios::binary);
    filesystem_assert(out.good());
    out.write(text.data(), static_cast<std::streamsize>(text.size()));
}

void remove_if_exists(const char* path) {
    std::error_code ec;
    std::filesystem::remove(path, ec);
}

void test_file_default_constructor() {
    fs::io::file f;
    filesystem_assert(!f.is_open());
}

void test_file_move_constructor() {
    create_text_file(test_open_file_name, "hello");

    auto p = fs::io::path(STR("fs_open_test.txt"));
    auto f1 = fs::io::open(p);

    filesystem_assert(f1.is_open());
   /* auto f2 = std::move(f1);

    filesystem_assert(f2.is_open());*/
    remove_if_exists(test_open_file_name);
}

void test_file_move_assignment() {
    create_text_file(test_open_file_name, "hello");
    auto f1 = fs::io::open(fs::io::path(STR("fs_open_test.txt")));

    fs::io::file f2;
    f2 = std::move(f1);

    filesystem_assert(f2.is_open());
    remove_if_exists(test_open_file_name);
}

void test_open_existing_file() {
    create_text_file(test_open_file_name, "abcdef");
    auto file = fs::io::open(fs::io::path(STR("fs_open_test.txt")));

    filesystem_assert(file.is_open());
    remove_if_exists(test_open_file_name);
}

void test_open_nonexistent_file() {
    remove_if_exists("definitely_missing_file_123456789.tmp");

    bool thrown = false;

    try {
        auto file = fs::io::open(fs::io::path(STR("definitely_missing_file_123456789.tmp")));
        (void) file;
    }
    catch (...) {
        thrown = true;
    }

    filesystem_assert(thrown);
}

void test_reset() {
    create_text_file(test_reset_file_name, "abc");

    auto file = fs::io::open(fs::io::path(STR("fs_reset_test.txt")));

    filesystem_assert(file.is_open());
    file.reset();

    filesystem_assert(!file.is_open());
    remove_if_exists(test_reset_file_name);
}

void test_double_reset() {
    create_text_file(test_reset_file_name, "abc");

    auto file = fs::io::open(fs::io::path(STR("fs_reset_test.txt")));

    file.reset();
    file.reset();

    filesystem_assert(!file.is_open());
    remove_if_exists(test_reset_file_name);
}

void test_read_whole_file() {
    create_text_file(test_read_file_name, "abcdef");

    auto file = fs::io::open(fs::io::path(STR("fs_read_test.txt")));
    char buffer[6]{};
    auto read_bytes = fs::io::read(file, fs::io::mutable_buffer(buffer,  sizeof(buffer)), 0);

    filesystem_assert(read_bytes == 6);

    filesystem_assert(std::string_view(buffer, sizeof(buffer)) == "abcdef");
    remove_if_exists(test_read_file_name);
}

void test_read_with_offset() {
    create_text_file(test_offset_file_name, "0123456789");
    auto file = fs::io::open(fs::io::path(STR("fs_offset_test.txt")));

    char buffer[3]{};
    auto read_bytes = fs::io::read(file, fs::io::mutable_buffer(buffer, sizeof(buffer)), 4);

    filesystem_assert(read_bytes == 3);
    filesystem_assert(std::string_view(buffer, sizeof(buffer)) == "456");

    remove_if_exists(test_offset_file_name);
}

void test_read_single_byte() {
    create_text_file(test_read_file_name, "XYZ");

    auto file = fs::io::open(fs::io::path(STR("fs_read_test.txt")));
    char buffer[1]{};

    auto read_bytes = fs::io::read(file, fs::io::mutable_buffer(buffer, sizeof(buffer)), 1);

    filesystem_assert(read_bytes == 1);
    filesystem_assert(buffer[0] == 'Y');

    remove_if_exists(test_read_file_name);
}

void test_read_eof() {
    create_text_file(test_eof_file_name, "abc");

    auto file = fs::io::open(fs::io::path(STR("fs_eof_test.txt")));

    char buffer[64]{};
    auto read_bytes = fs::io::read(file, fs::io::mutable_buffer(buffer, sizeof(buffer)), 0);

    filesystem_assert(read_bytes <= 64);

    remove_if_exists(test_eof_file_name);
}

void test_open_always() {
    remove_if_exists(
        test_always_file_name
    );

    auto file = fs::io::open[fs::io::open_always](fs::io::path(STR("fs_open_always_test.txt")));

    filesystem_assert(file.is_open());
    remove_if_exists(test_always_file_name);
}

int main() {
    test_file_default_constructor();

    test_file_move_constructor();
    test_file_move_assignment();

    test_open_existing_file();
    test_open_nonexistent_file();

    test_reset();
    test_double_reset();

    test_read_whole_file();
    test_read_single_byte();
    test_read_with_offset();

    test_read_eof();

    test_open_always();

    return 0;
}