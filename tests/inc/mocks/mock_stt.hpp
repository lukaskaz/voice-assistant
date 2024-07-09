#pragma once

#include "speechtotext.hpp"

#include <gmock/gmock.h>

class TextFromVoiceMock : public stt::TextFromVoiceIf
{
  public:
    MOCK_METHOD((std::pair<std::string, uint32_t>), listen, (), (override));
};
