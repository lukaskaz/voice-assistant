#include "gptchat.hpp"
#include "mocks/mock_chat.hpp"
#include "mocks/mock_shell.hpp"
#include "mocks/mock_stt.hpp"
#include "mocks/mock_tts.hpp"
#include "speechtotext.hpp"
#include "texttospeech.hpp"
#include "voiceassistant.hpp"

using namespace testing;

class TestVoiceAssistant :
    public TestWithParam<std::pair<std::string, uint32_t>>
{
  protected:
    std::shared_ptr<CommandsMock> commandMock =
        std::make_shared<NiceMock<CommandsMock>>();
    std::shared_ptr<TextToVoiceMock> ttsMock =
        std::make_shared<NiceMock<TextToVoiceMock>>();
    std::shared_ptr<TextFromVoiceMock> sttMock =
        std::make_shared<NiceMock<TextFromVoiceMock>>();
    std::shared_ptr<GptChatMock> chatMock =
        std::make_shared<NiceMock<GptChatMock>>();

  protected:
    const uint32_t defSttQuality{90};
};

TEST_P(TestVoiceAssistant, initialTest1)
{
    const auto& [command, speaknum] = GetParam();
    EXPECT_CALL(*ttsMock, speak(_)).Times(speaknum);
    EXPECT_CALL(*sttMock, listen())
        .Times(2)
        .WillOnce(Return(std::make_pair(command, defSttQuality)))
        .WillOnce(Return(std::make_pair("zakończ", defSttQuality)));
    voiceassistant::VoiceAssistant(ttsMock, sttMock, chatMock).run();
}

auto getTestingValues()
{
    std::vector<std::pair<std::string, uint32_t>> values;

    values.emplace_back("powtórz", 4);
    values.emplace_back("zaśpiewaj", 4);
    values.emplace_back("czat", 4);
    values.emplace_back("zrzuć", 4);
    values.emplace_back("pomoc", 12);

    return values;
}

INSTANTIATE_TEST_CASE_P(_, TestVoiceAssistant,
                        ::testing::ValuesIn(getTestingValues()));
