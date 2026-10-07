#include "VolumeTextSet.h"
#include "GameConstants.h"
#include "Master.h"
#include "Utility.h"

VolumeTextSet::VolumeTextSet()
	: mnVolumeHandle(-1)
	, mnVolumeBackground(-1)
	, mnTextFontSize(VolumeController::TextFontSize)
	, mnVolumeFontSize(VolumeController::VolumeTextSize)
{
	// âÊëúì«Ç›çûÇ›
	mnVolumeHandle = LoadGraph(TitleResourcePath::MusicNote.c_str());
	if (mnVolumeHandle == -1) { printfDx("âπïÑÇÃâÊëúÇ™Ç»Ç¢"); }

	mnVolumeBackground = LoadGraph(TitleResourcePath::VolumeSettingsBg.c_str());
	if (mnVolumeBackground == -1) { printfDx("âπó ê›íËÇÃîwåiâÊëúÇ™Ç»Ç¢"); }

}

VolumeTextSet::~VolumeTextSet()
{
	if (mnVolumeHandle != -1)
	{
		DeleteGraph(mnVolumeHandle);
		mnVolumeHandle = -1;
	}

	if (mnVolumeBackground != -1)
	{
		DeleteGraph(mnVolumeBackground);
		mnVolumeBackground = -1;
	}
}

// BGMÇÃâπó ÉeÉLÉXÉgÇï`âÊÇ∑ÇÈä÷êî
void VolumeTextSet::DrawBgmText(VolumeBar& bgm) const
{
	// BGMä÷åW
	{
		// BGMï∂éö
		Master::mpGameManager->GetFontManager()->FontManagerDrawString(
			FontManager::FontType::Nikumaru,
			TitlePosition::BgmTextX,
			TitlePosition::BgmTextY,
			mnTextFontSize,
			ColorOption::White,
			"BGM"
		);

		int bgmVolume = Master::mpGameManager->GetSoundManager()->GetBGMVolume();
		// BGMÉoÅ[
		bgm.Draw(bgmVolume);

		// BGMâπó ÇÃêîéö
		Master::mpGameManager->GetFontManager()->FontManagerDrawString(
			FontManager::FontType::Nikumaru,
			TitlePosition::VolumeBarX
			+ (bgmVolume * TitlePosition::VolumeBarWidth
				/ SoundSetting::VolumeMax)
			+ TitlePosition::VolumeNumberXOffset,

			TitlePosition::BgmBarY
			- TitlePosition::VolumeNumberYOffset,
			mnVolumeFontSize,
			ColorOption::White,
			"%d",
			bgmVolume
		);

		// BGMÇ¬Ç‹Ç›
		DrawRotaGraph(
			TitlePosition::VolumeBarX
			+ (bgmVolume * TitlePosition::VolumeBarWidth
				/ SoundSetting::VolumeMax),

			TitlePosition::BgmBarY
			+ TitlePosition::MusicNoteOffset,

			TitleScale::MusicNote,
			0.0f,
			mnVolumeHandle,
			TRUE
		);
	}
}



// âπó ÇÃÉeÉLÉXÉgÇï`âÊÇ∑ÇÈä÷êî
void VolumeTextSet::DrawSeText(VolumeBar& se) const
{
	// SEä÷åW
	{
		// SEï∂éö
		Master::mpGameManager->GetFontManager()->FontManagerDrawString(
			FontManager::FontType::Nikumaru,
			TitlePosition::SeTextX,
			TitlePosition::SeTextY,
			mnTextFontSize,
			ColorOption::White,
			"SE"
		);

		int seVolume = Master::mpGameManager->GetSoundManager()->GetSEVolume();
		// SEÉoÅ[
		se.Draw(seVolume);

		// SEâπó ÇÃêîéö
		Master::mpGameManager->GetFontManager()->FontManagerDrawString(
			FontManager::FontType::Nikumaru,
			TitlePosition::VolumeBarX
			+ (seVolume * TitlePosition::VolumeBarWidth
				/ SoundSetting::VolumeMax)
			+ TitlePosition::VolumeNumberXOffset,

			TitlePosition::SeBarY
			- TitlePosition::VolumeNumberYOffset,
			mnVolumeFontSize,
			ColorOption::White,
			"%d",
			seVolume
		);

		// SEÇ¬Ç‹Ç›
		DrawRotaGraph(
			TitlePosition::VolumeBarX
			+ (seVolume * TitlePosition::VolumeBarWidth
				/ SoundSetting::VolumeMax),

			TitlePosition::SeBarY
			+ TitlePosition::MusicNoteOffset,
			TitleScale::MusicNote,
			0.0f,
			mnVolumeHandle,
			TRUE
		);
	}
}

// ÉQÅ[ÉÄâÊñ Çà√Ç≠ÇµÇƒâπó âÊñ ÇÃîwåiÇï`âÊÇ∑ÇÈä÷êî
void VolumeTextSet::DrawVolumeSettingPanel()
{
	// îwåià√Ç≠Ç∑ÇÈ
	SetDrawBlendMode(
		DX_BLENDMODE_ALPHA,
		TitleSetting::VolumeBackgroundAlpha
	);
	//çïÇ¢BOX
	DrawBox(
		0,
		0,
		Utility::SCREEN_WIDTH,
		Utility::SCREEN_HEIGHT,
		ColorOption::Black,
		TRUE
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // ñﬂÇ∑


	// âπó ê›íËÇÃîwåiÇÃçïî¬
	DrawRotaGraph(
		Utility::SCREEN_WIDTH / 2,
		Utility::SCREEN_HEIGHT / 2,
		TitleScale::VolumeSettingsBackground,
		0.0f,
		mnVolumeBackground,
		TRUE
	);

	Master::mpGameManager->GetFontManager()->FontManagerDrawString(
		FontManager::FontType::Nikumaru,
		TitlePosition::VolumeTitleX,
		TitlePosition::VolumeTitleY,
		80,
		ColorOption::White,
		"âπó ê›íË"
	);
}