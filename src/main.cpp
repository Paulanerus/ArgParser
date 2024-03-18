#include <iostream>

#include "arg_parser.hpp"

int main(int argc, char *argv[])
{
    using namespace std::string_literals;

    const char *arguments[] = {"add", "-l", "-n", "test", "-p"};

    ArgParser parser;

    parser.command("add"s, "a"s);

    /*
    parser.command("Help", "me");

    parser.command("add", "a")
        .help("Adds a library to the project")

        .argument("link", "l")
            .flag()
            .help("Marks the added library as a link library")
            .end()

        .argument("name", "n")
            .required()
            .help("The name of the library to add")
            .end();

        .argument("path", "p");
            .default(".")
            .help("The path to the library to add")
            .end();
    */

    parser.parse(arguments, 5, 0);

    return EXIT_SUCCESS;
}