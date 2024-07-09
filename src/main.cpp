#include "voiceassistant.hpp"

#include <csignal>
#include <iostream>

void signalHandler(int signal)
{
    switch (signal)
    {
        case SIGINT:
            std::cout << "Process interrupted by the user, exitting...\n";
            BashCommand().run("killall -s KILL sox");
            exit(5);
        default:
            std::cout << "Received signal: " << signal << ", nothing to do\n";
            break;
    }
}

int main()
{
    std::signal(SIGINT, signalHandler);
    try
    {
        auto assitant = voiceassistant::VoiceAssistantFactory::create();
        assitant->run();
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}
