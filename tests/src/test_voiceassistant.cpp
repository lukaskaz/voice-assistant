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
    public TestWithParam<std::tuple<std::string, uint32_t, uint32_t>>
{
  protected:
    std::shared_ptr<TextToVoiceMock> ttsMock =
        std::make_shared<NiceMock<TextToVoiceMock>>();
    std::shared_ptr<TextFromVoiceMock> sttMock =
        std::make_shared<NiceMock<TextFromVoiceMock>>();
    std::shared_ptr<GptChatMock> chatMock =
        std::make_shared<NiceMock<GptChatMock>>();
    std::shared_ptr<ShellMock> shellMock =
        std::make_shared<NiceMock<ShellMock>>();

  protected:
    const uint32_t defSttQuality{90};
};

TEST_P(TestVoiceAssistant, testCheckAllCommandsCallbacksWorkAsExpected)
{
    const auto& [command, speakoneargnum, speaktwoargsnum] = GetParam();
    std::vector<std::string> translatedjson = {
        "[[[\"Es ist spät\",\"jest "
        "późno\",null,null,11]],null,\"pl\",null,null,null,null,[]]"};
    ON_CALL(*shellMock, run(_, _))
        .WillByDefault(DoAll(SetArgReferee<1>(translatedjson), Return(0)));
    EXPECT_CALL(*ttsMock, speak(_)).Times(speakoneargnum);
    EXPECT_CALL(*ttsMock, speak(_, _)).Times(speaktwoargsnum);
    EXPECT_CALL(*sttMock, listen())
        .Times(2)
        .WillOnce(Return(std::make_pair(command, defSttQuality)))
        .WillOnce(Return(std::make_pair("zakończ", defSttQuality)));
    vassist::VoiceAssistant(ttsMock, sttMock, chatMock, shellMock).run();
}

auto getTestingValues()
{
    std::vector<std::tuple<std::string, uint32_t, uint32_t>> values;

    values.emplace_back("powtórz", 4, 0);
    values.emplace_back("zaśpiewaj", 4, 0);
    values.emplace_back("przetłumacz", 4, 0);
    values.emplace_back("przetłumacz tekst", 3, 1);
    values.emplace_back("czat", 4, 0);
    values.emplace_back("zrzuć", 4, 0);
    values.emplace_back("pomoc", 12, 0);

    return values;
}

INSTANTIATE_TEST_SUITE_P(_, TestVoiceAssistant,
                         ::testing::ValuesIn(getTestingValues()));
