#pragma once

#include "gptchat.hpp"
#include "shellcommand.hpp"
#include "speechtotext.hpp"
#include "texttospeech.hpp"

#include <functional>
#include <memory>
#include <unordered_map>

namespace vassist
{

class VoiceAssistant
{
  public:
    VoiceAssistant(std::shared_ptr<tts::TextToVoiceIf>,
                   std::shared_ptr<stt::TextFromVoiceIf>,
                   std::shared_ptr<gpt::GptChatIf>,
                   std::shared_ptr<shell::ShellCommand>);

    void run() const;

  private:
    using callback = std::function<bool(const std::string&)>;

    const std::string defkey{"czat"};
    std::unordered_map<std::string, callback> commands;
    std::shared_ptr<tts::TextToVoiceIf> tts;
    std::shared_ptr<stt::TextFromVoiceIf> stt;
    std::shared_ptr<gpt::GptChatIf> chat;
    std::shared_ptr<shell::ShellCommand> shell;

    void intialize();
    void addtocallbacks(const std::string&, callback&&);
    std::pair<std::string, std::string> getkeyrequest(std::string_view,
                                                      std::string_view) const;
};

class VoiceAssistantFactory
{
  public:
    VoiceAssistantFactory() = delete;
    VoiceAssistantFactory(const VoiceAssistantFactory&) = delete;
    VoiceAssistantFactory(VoiceAssistantFactory&&) = delete;
    VoiceAssistantFactory& operator=(const VoiceAssistantFactory&) = delete;
    VoiceAssistantFactory& operator=(VoiceAssistantFactory&&) = delete;

    static std::shared_ptr<VoiceAssistant> create();
};

} // namespace vassist
