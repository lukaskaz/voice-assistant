#include "voiceassistant.hpp"

#include <boost/program_options.hpp>

#include <csignal>
#include <iostream>

void signalHandler(int signal)
{
    switch (signal)
    {
        case SIGINT:
            std::cout << "Process interrupted by the user, exitting...\n";
            stt::TextFromVoiceIf::kill();
            exit(5);
        default:
            std::cout << "Received signal: " << signal << ", nothing to do\n";
            break;
    }
}

int main(int argc, char* argv[])
{
    std::signal(SIGINT, signalHandler);

    boost::program_options::options_description desc(
        "This program provides voice assistance functionality.\nAllowed "
        "options");
    desc.add_options()("help,h", "produce help message");

    boost::program_options::variables_map vm;
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.contains("help"))
    {
        std::cout << desc;
        return 0;
    }

    try
    {
        auto assitant = vassist::VoiceAssistantFactory::create();
        assitant->run();
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
