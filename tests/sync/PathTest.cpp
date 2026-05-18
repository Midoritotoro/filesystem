#include <iostream>
#include <filesystem/io/Path.h>
#include <src/filesystem/Assert.h>
#include <string_view>

#if defined(filesystem_os_windows)
#define STR(x) L##x
#else
#define STR(x) x
#endif

void test_constructors() {
    fs::io::path p1;
    filesystem_assert(p1.empty());
    filesystem_assert(p1.native() == fs::io::path::string_type{});

    fs::io::path p2(fs::io::path::string_type(STR("foo/bar")));
    filesystem_assert(!p2.empty());
    filesystem_assert(p2.native() == STR("foo/bar"));

    fs::io::path p3(p2);
    filesystem_assert(p3.native() == p2.native());

    fs::io::path p4(std::move(p3));
    filesystem_assert(p4.native() == STR("foo/bar"));

    fs::io::path p5;
    p5 = fs::io::path::string_type(STR("/new/path"));
    filesystem_assert(p5.native() == STR("/new/path"));
}

void test_native_access() {
    fs::io::path p(fs::io::path::string_type(STR("test.txt")));

    fs::io::path::string_type implicit = static_cast<fs::io::path::string_type>(p);
    filesystem_assert(implicit == STR("test.txt"));

    const fs::io::path::string_type& ref = p.native();
    filesystem_assert(ref == STR("test.txt"));

    filesystem_assert(std::basic_string_view<fs::io::path::value_type>(p.c_str()) == STR("test.txt"));
}

void test_modifiers() {
    if constexpr (fs::windows) {
        fs::io::path win(fs::io::path::string_type(STR("C:/foo/bar")));
        win.make_preferred();
        filesystem_assert(win.native() == STR("C:\\foo\\bar"));
    }
    else {
        fs::io::path unix(fs::io::path::string_type(STR("/foo/bar")));
        unix.make_preferred();
        filesystem_assert(unix.native() == STR("/foo/bar"));
    }

    fs::io::path p1(fs::io::path::string_type(STR("dir/file.txt")));
    p1.remove_filename();
    filesystem_assert(p1.native() == STR("dir/"));

    fs::io::path p2(fs::io::path::string_type(STR("src/main.cpp")));
    p2.replace_filename(fs::io::path(fs::io::path::string_type(STR("test.cpp"))));
    
    if constexpr (fs::windows) filesystem_assert(p2.native() == STR("src/test.cpp"));
    else filesystem_assert(p2.native() == STR("src/test.cpp"));

    fs::io::path p3(fs::io::path::string_type(STR("archive.tar.gz")));
    p3.replace_extension(fs::io::path(fs::io::path::string_type(STR("zip"))));
    filesystem_assert(p3.native() == STR("archive.zip"));

    fs::io::path p4(fs::io::path::string_type(STR("noext")));
    p4.replace_extension(fs::io::path(fs::io::path::string_type(STR(".txt"))));
    filesystem_assert(p4.native() == STR("noext.txt"));

    fs::io::path p5(fs::io::path::string_type(STR("/some/path")));
    p5.clear();
    filesystem_assert(p5.empty());
}

void test_decomposition() {
#if defined(filesystem_os_windows)
    fs::io::path win_abs(fs::io::path::string_type(STR("C:\\Users\\Alice\\doc.txt")));
    filesystem_assert(win_abs.root_name().native() == STR("C:"));
    filesystem_assert(win_abs.root_directory().native() == STR("\\"));
    filesystem_assert(win_abs.root_path().native() == STR("C:\\"));
    filesystem_assert(win_abs.relative_path().native() == STR("Users\\Alice\\doc.txt"));
    filesystem_assert(win_abs.parent_path().native() == STR("C:\\Users\\Alice\\"));
    filesystem_assert(win_abs.filename().native() == STR("doc.txt"));
    filesystem_assert(win_abs.stem().native() == STR("doc"));
    filesystem_assert(win_abs.extension().native() == STR(".txt"));

    fs::io::path win_rel(fs::io::path::string_type(STR("projects\\eve\\src\\main.cpp")));
    filesystem_assert(!win_rel.has_root_name());
    filesystem_assert(!win_rel.has_root_directory());
    filesystem_assert(win_rel.filename().native() == STR("main.cpp"));
    filesystem_assert(win_rel.stem().native() == STR("main"));
    filesystem_assert(win_rel.extension().native() == STR(".cpp"));
#else
    fs::io::path posix_abs(fs::io::path::string_type(STR("/home/alice/doc.txt")));
    filesystem_assert(!posix_abs.has_root_name());
    filesystem_assert(posix_abs.root_directory().native() == STR("/"));
    filesystem_assert(posix_abs.root_path().native() == STR("/"));
    filesystem_assert(posix_abs.relative_path().native() == STR("home/alice/doc.txt"));
    filesystem_assert(posix_abs.parent_path().native() == STR("/home/alice"));
    filesystem_assert(posix_abs.filename().native() == STR("doc.txt"));
    filesystem_assert(posix_abs.stem().native() == STR("doc"));
    filesystem_assert(posix_abs.extension().native() == STR(".txt"));

    fs::io::path posix_rel(fs::io::path::string_type(STR("projects/eve/src/main.cpp")));
    filesystem_assert(!posix_rel.has_root_path());
    filesystem_assert(posix_rel.filename().native() == STR("main.cpp"));
#endif

    fs::io::path only_ext(fs::io::path::string_type(STR(".gitignore")));
    filesystem_assert(only_ext.filename().native() == STR(".gitignore"));
    filesystem_assert(only_ext.has_stem());
    filesystem_assert(only_ext.has_extension());

    fs::io::path dir_with_dot(fs::io::path::string_type(STR("foo/.")));
    filesystem_assert(dir_with_dot.filename().native() == STR("."));
}
//
//
//void test_queries() {
//    filesystem_assert(fs::io::path().empty());
//    filesystem_assert(!fs::io::path(fs::io::path::string_type(STR("x"))).empty());
//
//    fs::io::path p(fs::io::path::string_type(STR("dir/file.txt")));
//    filesystem_assert(!p.has_root_name());
//    filesystem_assert(!p.has_root_directory());
//    filesystem_assert(p.has_relative_path());
//    filesystem_assert(p.has_parent_path());
//    filesystem_assert(p.has_filename());
//    filesystem_assert(p.has_stem());
//    filesystem_assert(p.has_extension());
//
//#if defined(filesystem_os_windows)
//    filesystem_assert(fs::io::path(fs::io::path::string_type(STR("C:\\"))).is_absolute());
//    filesystem_assert(!fs::io::path(fs::io::path::string_type(STR("C:\\"))).is_relative());
//    filesystem_assert(!fs::io::path(fs::io::path::string_type(STR("foo\\bar"))).is_absolute());
//    filesystem_assert(fs::io::path(fs::io::path::string_type(STR("foo\\bar"))).is_relative());
//#else
//    filesystem_assert(fs::io::path(fs::io::path::string_type(STR("/"))).is_absolute());
//    filesystem_assert(!fs::io::path(fs::io::path::string_type(STR("/"))).is_relative());
//    filesystem_assert(!fs::io::path(fs::io::path::string_type(STR("foo/bar"))).is_absolute());
//    filesystem_assert(fs::io::path(fs::io::path::string_type(STR("foo/bar"))).is_relative());
//#endif
//}
//
//void test_lexical_ops() {
//    fs::io::path p1(fs::io::path::string_type(STR("foo/./bar/../baz")));
//    auto normalized = p1.lexically_normal();
//#if defined(filesystem_os_windows)
//    filesystem_assert(normalized.native() == STR("foo\\baz"));
//#else
//    filesystem_assert(normalized.native() == STR("foo/baz"));
//#endif
//
//#if !defined(filesystem_os_windows)
//    fs::io::path base(fs::io::path::string_type(STR("/home/user/projects")));
//    fs::io::path target(fs::io::path::string_type(STR("/home/user/docs/readme.txt")));
//    auto rel = target.lexically_relative(base);
//    filesystem_assert(rel.native() == STR("../docs/readme.txt"));
//#endif
//
//#if defined(filesystem_os_windows)
//    fs::io::path base_win(fs::io::path::string_type(STR("C:\\home\\user\\projects")));
//    fs::io::path target_win(fs::io::path::string_type(STR("C:\\home\\user\\docs\\readme.txt")));
//    auto rel_win = target_win.lexically_relative(base_win);
//    filesystem_assert(rel_win.native() == STR("..\\docs\\readme.txt"));
//
//    fs::io::path disk_d(fs::io::path::string_type(STR("D:\\data")));
//    auto prox = disk_d.lexically_proximate(base_win);
//    filesystem_assert(prox.native() == STR("D:\\data"));
//#endif
//}
//
//void test_separator_handling() {
//    fs::io::path p1(fs::io::path::string_type(STR("a/b\\c")));
//    filesystem_assert(!p1.empty());
//}

int main() {
    test_constructors();
    test_native_access();
    test_modifiers();
    test_decomposition();
    //test_queries();
    //test_lexical_ops();
    //test_separator_handling();
    return 0;
}