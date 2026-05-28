#include <filesystem/io/File.h>
#include <src/filesystem/io/FsOperations.h>

#include <src/filesystem/Assert.h>

#include <fstream>
#include <filesystem>
#include <string_view>

constexpr const char* test_open_file_name   = "fs_open_test.txt";
constexpr const char* test_read_file_name   = "fs_read_test.txt";
constexpr const char* test_offset_file_name = "fs_offset_test.txt";
constexpr const char* test_eof_file_name    = "fs_eof_test.txt";
constexpr const char* test_reset_file_name  = "fs_reset_test.txt";
constexpr const char* test_always_file_name = "fs_open_always_test.txt";
constexpr const char* test_create_file_name = "fs_create_test.txt";
constexpr const char* test_write_file_name  = "fs_write_test.txt";
constexpr const char* test_exists_file_name = "fs_exists_test.txt";
constexpr const char* test_rename_old_name  = "fs_rename_old.txt";
constexpr const char* test_rename_new_name  = "fs_rename_new.txt";

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

    auto p = fs::io::path(filesystem_native_text("fs_open_test.txt"));
    auto [f1, err] = fs::io::open(p);

    filesystem_assert(f1.is_open());
    auto f2 = std::move(f1);

    filesystem_assert(f2.is_open());
    remove_if_exists(test_open_file_name);
}

void test_file_move_assignment() {
    create_text_file(test_open_file_name, "hello");
    auto [f1, err] = fs::io::open(fs::io::path(filesystem_native_text("fs_open_test.txt")));

    fs::io::file f2;
    f2 = std::move(f1);

    filesystem_assert(f2.is_open());
    remove_if_exists(test_open_file_name);
}

void test_open_existing_file() {
    create_text_file(test_open_file_name, "abcdef");

    auto [f, ec] = fs::io::open(fs::io::path(filesystem_native_text("fs_open_test.txt")));
    filesystem_assert(ec); filesystem_assert(f.is_open());
    remove_if_exists(test_open_file_name);
}

void test_open_nonexistent_file() {
    remove_if_exists("definitely_missing_file_123456789.tmp");

    auto [file, err] = fs::io::open(fs::io::path(filesystem_native_text("definitely_missing_file_123456789.tmp")));
    filesystem_assert(err == fs::system::io_error::file_not_found);
    filesystem_assert(!file.is_open());
}

void test_reset() {
    create_text_file(test_reset_file_name, "abc");

    auto [file, err] = fs::io::open(fs::io::path(filesystem_native_text("fs_reset_test.txt")));

    filesystem_assert(file.is_open());
    file.reset();

    filesystem_assert(!file.is_open());
    remove_if_exists(test_reset_file_name);
}

void test_double_reset() {
    create_text_file(test_reset_file_name, "abc");

    auto [file, err] = fs::io::open(fs::io::path(filesystem_native_text("fs_reset_test.txt")));

    file.reset();
    file.reset();

    filesystem_assert(!file.is_open());
    remove_if_exists(test_reset_file_name);
}

void test_read_whole_file() {
    create_text_file(test_read_file_name, "abcdef");

    auto [file, err1] = fs::io::open(fs::io::path(filesystem_native_text("fs_read_test.txt")));

    char buffer[6] {};

    {
        auto [read_bytes, err2] = fs::io::read(file, fs::io::buffer(buffer), 0);
        filesystem_assert(read_bytes == 6);
        filesystem_assert(std::string_view(buffer, sizeof(buffer)) == "abcdef");
    }

    {
        fs::io::read[fs::options::on_completion([&buffer](fs::system::io_error err, fs::sizetype read_bytes) {
            filesystem_assert(read_bytes == 6); filesystem_assert(std::string_view(buffer, sizeof(buffer)) == "abcdef");
        })](file, fs::io::buffer(buffer), 0);
    }

    remove_if_exists(test_read_file_name);
}

void test_read_with_offset() {
    create_text_file(test_offset_file_name, "0123456789");
    auto [file, err1] = fs::io::open(fs::io::path(filesystem_native_text("fs_offset_test.txt")));

    char buffer[3]{};

    {
        auto [read_bytes, err2] = fs::io::read(file, fs::io::buffer(buffer), 4);
        filesystem_assert(read_bytes == 3); filesystem_assert(std::string_view(buffer, sizeof(buffer)) == "456");
    }

    {
        fs::io::read[fs::options::on_completion([&buffer](fs::system::io_error err, fs::sizetype read_bytes) {
            filesystem_assert(read_bytes == 3); filesystem_assert(std::string_view(buffer, sizeof(buffer)) == "456");
        })](file, fs::io::buffer(buffer), 4);
    }

    remove_if_exists(test_offset_file_name);
}

void test_read_single_byte() {
    create_text_file(test_read_file_name, "XYZ");

    auto [file, err1] = fs::io::open(fs::io::path(filesystem_native_text("fs_read_test.txt")));
    char buffer[1]{};

    {
        auto [read_bytes, err2] = fs::io::read(file, fs::io::buffer(buffer), 1);
        filesystem_assert(read_bytes == 1);
        filesystem_assert(buffer[0] == 'Y');
    }

    {
        fs::io::read[fs::options::on_completion([&buffer](fs::system::io_error err, fs::sizetype read_bytes) {
            filesystem_assert(read_bytes == 1); filesystem_assert(buffer[0] == 'Y');
        })](file, fs::io::buffer(buffer), 1);
    }

    remove_if_exists(test_read_file_name);
}

void test_read_eof() {
    create_text_file(test_eof_file_name, "abc");

    auto [file, err1] = fs::io::open(fs::io::path(filesystem_native_text("fs_eof_test.txt")));
    
    char buffer[64]{};

    {
        auto [read_bytes, err2] = fs::io::read(file, fs::io::buffer(buffer), 0);
        filesystem_assert(read_bytes <= 64);
    }

    {
        fs::io::read[fs::options::on_completion([&buffer](fs::system::io_error err, fs::sizetype read_bytes) {
            filesystem_assert(read_bytes <= 64);
        })](file, fs::io::buffer(buffer), 0);
    }

    remove_if_exists(test_eof_file_name);
}

void test_open_always() {
    remove_if_exists(test_always_file_name);

    auto [file, err] = fs::io::open[fs::io::always](fs::io::path(filesystem_native_text("fs_open_always_test.txt")));

    filesystem_assert(file.is_open());
    filesystem_assert(err);

    remove_if_exists(test_always_file_name);
}

void test_exists_true() {
    create_text_file(test_exists_file_name, "hello");
    filesystem_assert(fs::io::exists(fs::io::path(filesystem_native_text("fs_exists_test.txt"))));
    remove_if_exists(test_exists_file_name);
}

void test_exists_false() {
    remove_if_exists(test_exists_file_name);
    filesystem_assert(!fs::io::exists(fs::io::path(filesystem_native_text("fs_exists_test.txt"))));
}

void test_create_file() {
    create_text_file(test_create_file_name, "abc");

    const auto path = fs::io::path(filesystem_native_text("fs_create_test.txt"));

    {
        auto [file, err] = fs::io::create[fs::io::if_not_exists](path);
        filesystem_assert(file.is_open());
        filesystem_assert(fs::io::exists(fs::io::path(filesystem_native_text("fs_create_test.txt"))));
    }

    {
        auto [file, err] = fs::io::create(path);
        filesystem_assert(!file.is_open());
        filesystem_assert(fs::io::exists(fs::io::path(filesystem_native_text("fs_create_test.txt"))));
        filesystem_assert(!err); 
        filesystem_assert(err == fs::system::io_error::file_exists);
    }

    remove_if_exists(test_create_file_name);

    {
        auto [file, err] = fs::io::create(path);
        filesystem_assert(file.is_open());
        filesystem_assert(fs::io::exists(fs::io::path(filesystem_native_text("fs_create_test.txt"))));
        filesystem_assert(err); 
    }

    {
        auto [file, err] = fs::io::create[fs::io::always](path);
        filesystem_assert(file.is_open());
        filesystem_assert(fs::io::exists(fs::io::path(filesystem_native_text("fs_create_test.txt"))));
        filesystem_assert(err);
    }
 
    remove_if_exists(test_create_file_name);

    {
        auto [file, err] = fs::io::create[fs::io::always](path);
        filesystem_assert(file.is_open());
        filesystem_assert(fs::io::exists(fs::io::path(filesystem_native_text("fs_create_test.txt"))));
        filesystem_assert(err);
    }
}

void test_write_whole_buffer() {
    remove_if_exists(test_write_file_name);

    auto [file, err1] = fs::io::open[fs::io::always][fs::io::for_write](fs::io::path(filesystem_native_text("fs_write_test.txt")));
    const char data[] = "abcdef";

    auto [written, err2] = fs::io::write(file, fs::io::buffer(data), 0);
    filesystem_assert(written == sizeof(data));

    file.reset();
    std::ifstream in(test_write_file_name, std::ios::binary);

    std::string text;
    text.resize(sizeof(data) - 1);
    in.read(text.data(), static_cast<std::streamsize>(text.size()));

    filesystem_assert(text == "abcdef");
    remove_if_exists(test_write_file_name);
}

void test_write_with_offset() {
    create_text_file(test_write_file_name, "0123456789");

    auto [file, err1] = fs::io::open[fs::io::for_write](fs::io::path(filesystem_native_text("fs_write_test.txt")));
    const char data[] = "ABC";

    auto [written, err2] = fs::io::write(file, fs::io::buffer(data), 4);
    filesystem_assert(written == sizeof(data));

    file.reset();
    std::ifstream in(test_write_file_name, std::ios::binary);

    std::string text;
    text.resize(7);
    in.read(text.data(), 7);

    filesystem_assert(text == "0123ABC");
    remove_if_exists(test_write_file_name);
}

void test_write_empty_buffer() {
    remove_if_exists(test_write_file_name);

    auto [file, err1] = fs::io::open[fs::io::always][fs::io::for_write](fs::io::path(filesystem_native_text("fs_write_test.txt")));
    const char dummy = 0;
    auto [written, err2] = fs::io::write(file, fs::io::buffer(&dummy, &dummy), 0);

    filesystem_assert(written == 0);
    remove_if_exists(test_write_file_name);
}

void test_remove() {
    auto fs_remove = []() {
        auto err = fs::io::remove(fs::io::path(filesystem_native_text("fs_write_test.txt")));
        filesystem_assert(err.translate_to_success(fs::system::io_error::file_not_found));
        filesystem_assert(!fs::io::exists(fs::io::path(filesystem_native_text("fs_write_test.txt"))));
    };

    create_text_file(test_write_file_name, "0123456789");
    fs_remove();
    fs_remove();
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

    test_exists_true();
    test_exists_false();

    test_create_file();

    test_write_whole_buffer();
    test_write_with_offset();
    test_write_empty_buffer();

    test_remove();

    return 0;
}