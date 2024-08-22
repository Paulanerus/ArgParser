#include <iostream>

#include "commands.hpp"
#include "arg_parser.hpp"

int main(int argc, char *argv[])
{
    using namespace psap;

    ArgParser parser{"tram"};

    parser.option(Option::Flag({"--test", "-t"}, "Global test flag"));
    parser.option(Option::Value({"--value", "-v"}, "Global test value"));

    parser.command("help", "h")
        .help("Shows help.")
        .fallback()
        .action(HELP_ACTION);

    parser.command("new", "n", "create", "c")
        .help("Creates a new project.")
        .action(NEW_ACTION);

    parser.command("add", "a")
        .help("Adds a new dependency.")
        .option(Option::Value({"--branch", "-b"}, "Select a specific branch."))
        .option(Option::Flag({"--link", "-l"}, "Links the specified library."))
        .action(ADD_ACTION);

    parser.command("remove", "rm", "delete", "del")
        .help("Removes a dependency.")
        .action(REMOVE_ACTION);

    parser.command("build", "b")
        .help("Builds the project.")
        .option(Option::Flag({"--debug", "-d"}, "Runs debug build."))
        .option(Option::Flag({"--release", "-r"}, "Runs release build."))
        .action(BUILD_ACTION);

    parser.command("run", "r")
        .help("Executes the project and perform a build, if needed.")
        .option(Option::Flag({"--debug", "-d"}, "Runs debug build."))
        .option(Option::Flag({"--release", "-r"}, "Runs release build."))
        .action(RUN_ACTION);

    parser.parse(argv, argc);

    return EXIT_SUCCESS;
}