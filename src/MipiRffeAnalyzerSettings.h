#ifndef MIPIRFFE_ANALYZER_SETTINGS
#define MIPIRFFE_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class MipiRffeAnalyzerSettings : public AnalyzerSettings
{
public:
	MipiRffeAnalyzerSettings();
	virtual ~MipiRffeAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	virtual void LoadSettings(const char *settings);
	virtual const char *SaveSettings();

	void UpdateInterfacesFromSettings();

	Channel mSclkChannel;
	Channel mSdatChannel;
	bool  mShowMarker;

protected:

	std::auto_ptr< AnalyzerSettingInterfaceChannel >    mSclkChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >    mSdatChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceBool >       mUseShowMarkerInterface;
};

#endif //MIPIRFFE_ANALYZER_SETTINGS