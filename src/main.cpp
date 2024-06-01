#include <iostream>

#include "arg_parser.hpp"

int main(int argc, char *argv[])
{
    // std::cout << calculateDistance("adfda", "add") << std::endl;

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
        .option(Option::Flag({"--debug", "-d"}, "Runs debug build."))
        .option(Option::Flag({"--release", "-r"}, "Runs release build."));

    parser.command("run", "r")
        .help("Executes the project and perform a build, if needed.")
        .option(Option::Flag({"--debug", "-d"}, "Runs debug build."))
        .option(Option::Flag({"--release", "-r"}, "Runs release build."));

    parser.parse(argv, argc);

    return EXIT_SUCCESS;
}