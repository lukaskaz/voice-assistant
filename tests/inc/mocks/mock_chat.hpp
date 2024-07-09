#pragma once

#include "gptchat.hpp"

#include <gmock/gmock.h>

class GptChatMock : public gpt::GptChatIf
{
  public:
    MOCK_METHOD((std::pair<std::string, std::string>), run,
                (const std::string&, gpt::Callback&&, gpt::Callback&&, int32_t),
                (override));
    MOCK_METHOD(std::string, history, (), (const, override));
};
