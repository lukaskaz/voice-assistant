#pragma once

#include "texttospeech.hpp"

#include <gmock/gmock.h>

class TextToVoiceMock : public tts::TextToVoiceIf
{
  public:
    MOCK_METHOD(void, speak, (const std::string&), (override));
    MOCK_METHOD(void, speak, (const std::string&, tts::language), (override));
};
