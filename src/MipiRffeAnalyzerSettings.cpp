#include "MipiRffeAnalyzerSettings.h"

#include <AnalyzerHelpers.h>
#include <sstream>
#include <cstring>

MipiRffeAnalyzerSettings::MipiRffeAnalyzerSettings():
	mSclkChannel(UNDEFINED_CHANNEL),
	mSdatChannel(UNDEFINED_CHANNEL),
	mShowMarker(BIT_HIGH)

{
	

	mSdatChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
	mSdatChannelInterface->SetTitleAndTooltip("SDATA", "SDATA");
	mSdatChannelInterface->SetChannel(mSdatChannel);
	mSdatChannelInterface->SetSelectionOfNoneIsAllowed(false);

	mSclkChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
	mSclkChannelInterface->SetTitleAndTooltip("SCLK", "SCLK");
	mSclkChannelInterface->SetChannel(mSclkChannel);

	mUseShowMarkerInterface.reset(new AnalyzerSettingInterfaceBool());
	mUseShowMarkerInterface->SetTitleAndTooltip("", "Show decode marker or not");
	mUseShowMarkerInterface->SetCheckBoxText("Show Decode Marker");
	mUseShowMarkerInterface->SetValue(mShowMarker);
	
	AddInterface(mSdatChannelInterface.get());
	AddInterface(mSclkChannelInterface.get());
	AddInterface(mUseShowMarkerInterface.get());


	AddExportOption(0, "Export as text/csv file");
	AddExportExtension(0, "Text file", "txt");
	AddExportExtension(0, "CSV file", "csv");

	ClearChannels();

	AddChannel(mSclkChannel, "SCLK", false);
	AddChannel(mSdatChannel, "SDAT", false);
}

MipiRffeAnalyzerSettings::~MipiRffeAnalyzerSettings()
{
}

bool MipiRffeAnalyzerSettings::SetSettingsFromInterfaces()
{
	Channel sclk = mSclkChannelInterface->GetChannel();
	Channel sdat = mSdatChannelInterface->GetChannel();

	std::vector<Channel> channels;
	channels.push_back(sclk);
	channels.push_back(sdat);

	if (AnalyzerHelpers::DoChannelsOverlap(&channels[0], channels.size()) == true) {
		SetErrorText("Please select different channels for each input.");
		return false;
	}


	mSclkChannel = mSclkChannelInterface->GetChannel();
	mSdatChannel = mSdatChannelInterface->GetChannel();

	mShowMarker = mUseShowMarkerInterface->GetValue();

	ClearChannels();
	AddChannel(mSclkChannel, "SCLK", mSclkChannel != UNDEFINED_CHANNEL);
	AddChannel(mSdatChannel, "SDAT", mSdatChannel != UNDEFINED_CHANNEL);

	return true;
}

void MipiRffeAnalyzerSettings::LoadSettings(const char *settings)
{
	SimpleArchive text_archive;
	text_archive.SetString(settings);

	const char *name_string;  //the first thing in the archive is the name of the protocol analyzer that the data belongs to.
	text_archive >> &name_string;
	if (strcmp(name_string, "QyMipiRffeAnalyzer") != 0) {
		AnalyzerHelpers::Assert("Kingst: Provided with a settings string that doesn't belong to us;");
	}


	text_archive >> mSclkChannel;
	text_archive >> mSdatChannel;

	bool show_marker;
	if (text_archive >> show_marker) {
		mShowMarker = show_marker;
	}

	ClearChannels();

	AddChannel(mSclkChannel, "SCLK", mSclkChannel != UNDEFINED_CHANNEL);
	AddChannel(mSdatChannel, "SDAT", mSdatChannel != UNDEFINED_CHANNEL);

	UpdateInterfacesFromSettings();
}

const char *MipiRffeAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << "QyMipiRffeAnalyzer";
	text_archive << mSclkChannel;
	text_archive << mSdatChannel;
	text_archive << mShowMarker;

	return SetReturnString(text_archive.GetString());
}

void MipiRffeAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mSclkChannelInterface->SetChannel(mSclkChannel);
	mSdatChannelInterface->SetChannel(mSdatChannel);
	mUseShowMarkerInterface->SetValue(mShowMarker);
}