#include <iostream>

#include "arg_parser.hpp"

int main(int argc, char *argv[])
{
    using namespace std::string_literals;

    const char *arguments[] = {"add", "-l", "-n", "test", "-p"};

    ArgParser parser;

    parser.command("help", "h")
        .help("Shows help.");

    parser.command("new", "n", "create", "c")
        .help("Creates a new project.");

    parser.command("add", "a")
        .help("Adds a new dependency.");

    parser.command("remove", "rm", "delete", "del")
        .help("Removes a dependency.");

    parser.command("build", "b")
        .help("Builds the project.")
        .argument(Argument::Flag({"debug", "d"}, "Runs debug build."))
        .argument(Argument::Flag({"release", "r"}, "Runs release build."));

    parser.command("run", "r")
        .help("Executes the project and perform a build, if needed.")
        .argument(Argument::Flag({"debug", "d"}, "Runs debug build."))
        .argument(Argument::Flag({"release", "r"}, "Runs release build."));

    parser.parse(arguments, 5, 0);

    std::cout << parser << std::endl;

    return EXIT_SUCCESS;
}