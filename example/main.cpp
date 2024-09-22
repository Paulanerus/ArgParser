#include <iostream>

#include "arg_parser.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
    using namespace psap;

    ArgParser parser {
        ParserConf { "tram", 4, true, true, ValueStyle::Both, UnknownOptionPolicy::ReportRemove }
    };

    parser.option(make_flag({ "--test", "-t" }, "Global test flag"));
    parser.option(make_value({ "--value", "-v" }, "Global test value"));

    parser.command("help", "h")
        .help("Shows help.")
        .fallback()
        .action(HELP_ACTION);

    parser.command("new", "n", "create", "c")
        .help("Creates a new project.")
        .action(NEW_ACTION);

    parser.command("add", "a")
        .help("Adds a new dependency.")
        .option(make_value({ "--branch", "-b" }, "Select a specific branch."))
        .option(make_flag({ "--link", "-l" }, "Links the specified library."))
        .action(ADD_ACTION);

    parser.command("remove", "rm", "delete", "del")
        .help("Removes a dependency.")
        .action(REMOVE_ACTION);

    parser.command("build", "b")
        .help("Builds the project.")
        .option(make_flag({ "--debug", "-d" }, "Runs debug build."))
        .option(make_flag({ "--release", "-r" }, "Runs release build."))
        .action(BUILD_ACTION);

    parser.command("run", "r")
        .help("Executes the project and perform a build, if needed.")
        .option(make_flag({ "--debug", "-d" }, "Runs debug build."))
        .option(make_flag({ "--release", "-r" }, "Runs release build."))
        .action(RUN_ACTION);

    parser.parse(argv, argc);

    return EXIT_SUCCESS;
}
