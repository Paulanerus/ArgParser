# ArgParser

ArgParser is a simple, single header-only C++20 library to parse command like input. Similar in functionality to tools like [cargo](https://doc.rust-lang.org/stable/cargo/), the Rust packacke manager. It started as a testing project and after some develoment i thought that someone could use this as well.

This library was tested on Windows (MSCV) and GNU/Linux (GCC), with additional checking on GNU/Linux with [Valgrind](https://valgrind.org/)

## Example

Example 1:
```c++
#include "arg_parser.hpp"

int main(int argc, char* argv[])
{
    psap::ArgParser parser{"project_name"};

    auto HELP_ACTION = 
        [](const psap::ArgParser &parser, const psap::Command &cmd)
        {
            parser(parser[0]); // Print info text of remaining args
        }

    parser.command("help", "h")
        .help("Shows help.")
        .action(HELP_ACTION);

    //More commands to come

    parser.parse(argv, argc);
}
```

Output:
```
Usage: project_name [Command] [Options]

Commands:
    help, h    Shows help.

See 'project_name help <command>' for more information on a specific command.
```

Example 2:
```c++
    psap::ArgParser parser{"project_name"};

    auto RUN_ACTION = 
        [](const psap::ArgParser &parser, const psap::Command &cmd)
        {
            //Run action...
        }

    parser.command("run", "r")
        .help("Runs something.")
        .option(Option::Value({"--target", "-t"}, "Select a Target."))
        .option(Option::Flag({"--debug", "-d"}, "Runs in debug."))
        .action(RUN_ACTION);

    //More commands to come

    parser.parse(argv, argc);
```

## Documentation

TODO

## Dependencies

ArgParser has no other dependencies.

## Todos
+ Global options
+ Remove '--' and '-' specifier
+ Debug logging
+ Support custom types with get

## Support
If you encounter any issues or have any questions about this project, don't hesitate to reach out to me. You can [open an issue](https://github.com/Paulanerus/ArgParser/issues) on GitHub.