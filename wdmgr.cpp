#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string_view>
#include <system_error>
#include <utility>
#include <vector>

using namespace ::std::literals::string_literals;
using namespace ::std::literals::string_view_literals;

namespace fs = ::std::filesystem;

::fs::path GetWdmgrDir() {
#if defined(__unix)
  auto home = ::std::getenv("HOME");
  if (!home) {
    ::std::cerr << "Failed to resolve environment variable $HOME\n"sv;
    ::std::exit(EXIT_FAILURE);
  }
  return ::fs::path(home) / ".wdmgr"sv;
#elif defined(_WIN32)
  auto home = ::std::getenv("USERPROFILE");
  if (!home) {
    ::std::cerr << "Failed to resolve environment variable %USERPROFILE%\n"sv;
    ::std::exit(EXIT_FAILURE);
  }
  return ::fs::path(home) / "_wdmgr"sv;
#else
#error Unsupported Platform
#endif
}

const ::fs::path gWdmgrDir = GetWdmgrDir();

constexpr ::std::string_view gDefaultName = "unnamed"sv;

using WdmgrFunction = int (*)(::std::string_view);

::std::string GetStringFromFile(const ::fs::path& location, ::std::error_code& ec) {
  auto bExists = ::fs::exists(location, ec);
  if (ec)
    return {};
  if (!bExists) {
    ec = ::std::make_error_code(::std::errc::no_such_file_or_directory);
    return {};
  }
  ::std::ifstream file;
  try {
    file.exceptions(file.exceptions() | ::std::ios_base::badbit | ::std::ios_base::failbit);
    file.open(location);
    return ::std::string(::std::istreambuf_iterator<char>(file),
                         ::std::istreambuf_iterator<char>());
  }
  catch (::std::system_error& exn) {
    ec = exn.code();
  }
  return {};
}

//::fs::path GetPathFromFile(const ::fs::path& location, ::std::error_code& ec) {
//  try {
//    ::std::ifstream file;
//    file.exceptions(file.exceptions() | ::std::ios_base::badbit);
//    file.open(location);
//    auto content = ::fs::path(::std::istream_iterator<char>(file),
//                              ::std::istream_iterator<char>());
//    ec.clear();
//    return content;
//  }
//  catch (::std::system_error& exn) {
//    ec = exn.code();
//  }
//  return {};
//}

void WritePathToFile(const ::fs::path& location,
                     const ::fs::path& content, ::std::error_code& ec) {
  auto&& abspath = ::fs::absolute(content, ec);
  if (ec)
    return;
  try {
    ::std::ofstream file;
    file.exceptions(file.exceptions() | ::std::ios_base::badbit);
    file.open(location);
    file << abspath.string();
  }
  catch (::std::system_error& exn) {
    ec = exn.code();
  }
}

//int WdmgrChdir(::std::string_view name) {
//  ::std::error_code ec;
//  auto location = gWdmgrDir / name;
//  auto dest = GetPathFromFile(location, ec);
//  if (ec) {
//    if (ec == ::std::errc::no_such_file_or_directory)
//      ::std::cerr << "Could not find saved path with name \'"sv << name << "\': "sv;
//    else
//      ::std::cerr << "Failed to not read the saved path from store: "sv;
//    ::std::cerr << ec.message() << '\n';
//    return ec.value();
//  }
//  ::fs::current_path(dest, ec);
//  if (ec) {
//    ::std::cerr << "Failed to change working directory to \'"sv << dest << "\': "sv;
//    ::std::cerr << ec.message() << '\n';
//    return ec.value();
//  }
//  return EXIT_SUCCESS;
//}

int WdmgrSave(::std::string_view name) {
  ::std::error_code ec;
  ::fs::create_directories(gWdmgrDir, ec);
  if (ec) {
    ::std::cerr << "Failed to create the directory \'"sv << gWdmgrDir << "\': "sv;
    ::std::cerr << ec.message() << '\n';
    return ec.value();
  }
  auto cwd = ::fs::current_path(ec);
  if (ec) {
    ::std::cerr << "Failed to get the current working directory: "sv;
    ::std::cerr << ec.message() << '\n';
    return ec.value();
  }
  auto location = gWdmgrDir / name;
  WritePathToFile(location, cwd, ec);
  if (ec) {
    ::std::cerr << "Failed to store the current path: "sv;
    ::std::cerr << ec.message() << '\n';
    return ec.value();
  }
  return EXIT_SUCCESS;
}

int WdmgrShow(::std::string_view name) {
  ::std::error_code ec;
  auto location = gWdmgrDir / name;
  auto content = GetStringFromFile(location, ec);
  if (ec) {
    if (ec == ::std::errc::no_such_file_or_directory)
      ::std::cerr << "Could not find saved path with name \'"sv << name << "\': "sv;
    else
      ::std::cerr << "Failed to not read the saved path from store: "sv;
    ::std::cerr << ec.message() << '\n';
    return ec.value();
  }
  ::std::cout << content;
  return EXIT_SUCCESS;
}

int WdmgrList(::std::string_view) {
  ::std::error_code ec;
  auto it = ::fs::directory_iterator(gWdmgrDir, ec);
  if (ec) {
    if (ec == ::std::errc::no_such_file_or_directory) {
      ::std::cout << "(Empty)\n"sv;
      return EXIT_SUCCESS;
    }
    ::std::cerr << "Failed to list stored paths: "sv;
    ::std::cerr << ec.message() << '\n';
    return ec.value();
  }
  auto retv = EXIT_SUCCESS;
  ::std::vector<::std::pair<::std::string, ::std::string>> paths;
  auto sentinel = ::fs::directory_iterator();
  auto maxLen = ::std::size_t(0);
  while (it != sentinel) {
    auto& ent = *it;
    auto content = GetStringFromFile(ent.path(), ec);
    auto name = ent.path().filename().string();
    if (ec) {
      ::std::cerr << "Failed to retrieve stored path for \'" << name << "\': "sv;
      ::std::cerr << ec.message() << '\n';
      retv = ec.value();
    }
    it.increment(ec);
    if (ec) {
      ::std::cerr << "Failed to list all stored paths: "sv;
      ::std::cerr << ec.message() << '\n';
      retv = ec.value();
      break;
    }
    maxLen = ::std::max(maxLen, name.size());
    paths.emplace_back(::std::move(name), ::std::move(content));
  }
  for (auto& [name, content] : paths)
    ::std::cout << ::std::setw(::std::streamsize(maxLen)) << name << ' ' << content << '\n';
  if (paths.empty())
    ::std::cout << "(Empty)\n"sv;
  return retv;
}

int WdmgrRemove(::std::string_view name) {
  ::std::error_code ec;
  auto location = gWdmgrDir / name;
  auto bRes = ::fs::remove(location, ec);
  if (!bRes && ec == ::std::errc::no_such_file_or_directory) {
    ::std::cerr << "Could not find saved path with name \'"sv << name << "\': "sv;
    return int(::std::errc::no_such_file_or_directory);
  }
  if (ec) {
    ::std::cerr << "Failed to remove the saved path from store: "sv;
    ::std::cerr << ec.message() << '\n';
    return ec.value();
  }
  return EXIT_SUCCESS;
}

int WdmgrRemoveAll(::std::string_view) {
  ::std::error_code ec;
  ::fs::remove_all(gWdmgrDir, ec);
  if (ec == ::std::errc::no_such_file_or_directory) {}
  else if (ec) {
    ::std::cerr << "Failed to remove saved paths: "sv;
    ::std::cerr << ec.message() << '\n';
    return ec.value();
  }
  return EXIT_SUCCESS;
}

void ShowUsage(const char* program) {
  ::std::cout << "Usage:\n"sv;
  //::std::cout << program << " chdir|cd|c [name]\n"sv;
  //::std::cout << "  Change working directory to the saved path <name>.\n"sv;
  ::std::cout << program << " save|sv|s [name]\n"sv;
  ::std::cout << "  Save the current working directory as <name>.\n"sv;
  ::std::cout << program << " show|sh [name]\n"sv;
  ::std::cout << "  Show the path with <name>.\n"sv;
  ::std::cout << program << " list|ls|l\n"sv;
  ::std::cout << "  List all saved directories.\n"sv;
  ::std::cout << program << " remove|rm|r [name]\n"sv;
  ::std::cout << "  Remove the saved path <name>.\n"sv;
  ::std::cout << program << " removeall|rmall|ra\n"sv;
  ::std::cout << "  Remove all saved paths.\n"sv;
  ::std::cout << "Note: The default value of <name> is '"sv << gDefaultName << "' for all"
                 "operations.\n"sv;
}

WdmgrFunction CheckArgs(int argc, char* argv[]) {
  if (argc < 2)
    return nullptr;
  int maxArgc = 3;
  WdmgrFunction op = nullptr;
  ::std::string_view command = argv[1];
  //if (command == "chdir"sv || command == "cd"sv || command == "c"sv)
  //  op = WdmgrChdir;
  //else
  if (command == "save"sv || command == "sv"sv || command == "s"sv)
    op = WdmgrSave;
  else if (command == "show"sv || command == "sh"sv)
    op = WdmgrShow;
  else if (command == "list"sv || command == "ls"sv || command == "l"sv) {
    op = WdmgrList;
    maxArgc = 2;
  }
  else if (command == "remove"sv || command == "rm"sv || command == "r"sv)
    op = WdmgrRemove;
  else if (command == "removeall"sv || command == "rmall"sv || command == "ra"sv) {
    op = WdmgrRemoveAll;
    maxArgc = 2;
  }
  else
    return nullptr;
  if (argc > maxArgc)
    return nullptr;
  return op;
}

int main(int argc, char* argv[]) {
  WdmgrFunction fn = CheckArgs(argc, argv);
  if (!fn) {
    ::std::cerr << "Incorrect usage.\n"sv;
    ShowUsage(argv[0]);
    return EXIT_FAILURE;
  }
  ::std::string_view name = argc > 2 ? argv[2] : gDefaultName;
  return fn(name);
}
