#include "voiceassistant.hpp"

#include "shellcommand.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <deque>
#include <iostream>
#include <ranges>

namespace voiceassistant
{

VoiceAssistant::VoiceAssistant(std::shared_ptr<tts::TextToVoiceIf> tts,
                               std::shared_ptr<stt::TextFromVoiceIf> stt,
                               std::shared_ptr<gpt::GptChatIf> chat) :
    tts{tts},
    stt{stt}, chat{chat}
{
    intialize();
}

void VoiceAssistant::run() const
{
    tts->speak("Jestem twoim asystentem, powiedz co zrobić");
    while (1)
    {
        std::cout << "You speak now...\n";
        auto [prompt, confid] = stt->listen();
        auto [key, req] = getkeyrequest(prompt, defkey);
        std::cout << "I heard: " << prompt << " [" << confid << "%]\n";
        if (commands.at(key)(req))
        {
            tts->speak("Pytaj");
        }
        else
        {
            break;
        }
    }
}

inline void VoiceAssistant::intialize()
{
    addtocallbacks("powtórz",
                   std::bind(
                       [](std::shared_ptr<tts::TextToVoiceIf> tts,
                          const std::string& text) {
                           std::cout << "Speaking your words: " << text << '\n';
                           tts->speak(text);
                           return true;
                       },
                       tts, std::placeholders::_1));

    addtocallbacks("zaśpiewaj",
                   std::bind(
                       [](std::shared_ptr<tts::TextToVoiceIf> tts) {
                           std::cout << ":))) " << '\n';
                           tts->speak("Przez te oczy zielone.... "
                                      "zieloneeee oooooszalałeeeeeem");
                           return true;
                       },
                       tts));

    addtocallbacks("przetłumacz",
                   std::bind(
                       [](std::shared_ptr<tts::TextToVoiceIf> tts,
                          const std::string& text) {
                           std::cout << "I translate: " << text << '\n';
                           std::string translate =
                               "wget -U Mozilla/5.0 -qO - "
                               "\"http://translate.googleapis.com/translate_a/"
                               "single?client=gtx&sl=pl&tl=de&dt=t&q=" +
                               text + "\"";

                           if (std::vector<std::string> output;
                               !BashCommand().run(std::move(translate), output))
                           {
                               using json = nlohmann::json;

                               json data = json::parse(output.at(0));
                               std::string translated = data[0][0][0];
                               std::cout << "Translated: " << translated
                                         << '\n';
                               tts::TextToVoiceFactory::create(
                                   translated, tts::language::german);
                           }
                           else
                           {
                               tts->speak("Nie moge przetłumaczyć");
                           }
                           return true;
                       },
                       tts, std::placeholders::_1));

    addtocallbacks(
        "czat",
        std::bind(
            [](std::shared_ptr<tts::TextToVoiceIf> tts,
               std::shared_ptr<gpt::GptChatIf> chat, const std::string& text) {
                auto [fullanswer, shortanswer] = chat->run(
                    text,
                    [tts, &text]() {
                        std::cout << "Checking question: " << text << '\n';
                        tts->speak("Już sprawdzam");
                    },
                    [tts]() {
                        std::cout << "Wait...\n";
                        tts->speak("Jeszcze chwila");
                    },
                    3);

                std::cout << "Full answer:\n" << fullanswer << '\n';
                std::cout << "Short answer:\n" << shortanswer << '\n';
                shortanswer.empty() ? tts->speak(fullanswer)
                                    : tts->speak(shortanswer);
                return true;
            },
            tts, chat, std::placeholders::_1));

    addtocallbacks("zrzuć",
                   std::bind(
                       [](std::shared_ptr<tts::TextToVoiceIf> tts,
                          std::shared_ptr<gpt::GptChatIf> chat) {
                           std::cout << chat->history() << "\n";
                           tts->speak("Zrzucam historie GPT czatu na konsole");
                           return true;
                       },
                       tts, chat));

    addtocallbacks(
        "pomoc",
        std::bind(
            [this](std::shared_ptr<tts::TextToVoiceIf> tts) {
                tts->speak("Dostępne opcje to");

                std::deque<std::string> optlist;
                std::ranges::for_each(
                    commands,
                    [&optlist](const auto& cmd) { optlist.push_front(cmd); },
                    &std::unordered_map<std::string,
                                        callback>::value_type::first);
                std::ranges::for_each(optlist,
                                      [tts](auto& opt) { tts->speak(opt); });
                tts->speak("I to wszystko");
                return true;
            },
            tts));

    addtocallbacks("zakończ", std::bind(
                                  [](std::shared_ptr<tts::TextToVoiceIf> tts) {
                                      std::cout << "Exitting program...\n";
                                      tts->speak("Na razie");
                                      return false;
                                  },
                                  tts));
}

inline void VoiceAssistant::addtocallbacks(const std::string& key,
                                           callback&& cb)
{
    commands.try_emplace(key, std::move(cb));
}

inline std::pair<std::string, std::string>
    VoiceAssistant::getkeyrequest(std::string_view prompt,
                                  std::string_view defaultkey) const
{
    std::string key, request;
    std::ranges::transform(prompt, std::back_inserter(request), ::tolower);

    if (auto pos = request.find_first_of(' '); pos != std::string::npos)
    {
        key = request.substr(0, pos);
        commands.contains(key) ? request.erase(0, pos + 1) : key = defaultkey;
    }
    else
    {
        key = commands.contains(request) ? request : defaultkey;
    }

    return std::make_pair(key, request);
}

std::shared_ptr<VoiceAssistant> VoiceAssistantFactory::create()
{
    auto tts = tts::TextToVoiceFactory::create(tts::language::polish);
    auto stt = stt::TextFromVoiceFactory::create(stt::language::polish);
    auto chat = gpt::GptChatFactory::create();

    return std::make_shared<VoiceAssistant>(tts, stt, chat);
}

} // namespace voiceassistant
