#ifndef MIPIRFFE_ANALYZER_RESULTS
#define MIPIRFFE_ANALYZER_RESULTS

#include <AnalyzerResults.h>

#define PARITY_ERROR_FLAG ( 1 << 0 )
#define RACE_ERROR_FLAG ( 1 << 1 )
#define HAZARD_ERROR_FLAG ( 1 << 2 )
#define NORESPONSES_ERROR_FLAG ( 1 << 3 )
#define TYPE_ERROR_FLAG ( 1 << 4 )

class MipiRffeAnalyzer;
class MipiRffeAnalyzerSettings;

class MipiRffeAnalyzerResults : public AnalyzerResults
{
public:
	MipiRffeAnalyzerResults(MipiRffeAnalyzer *analyzer, MipiRffeAnalyzerSettings *settings);
	virtual ~MipiRffeAnalyzerResults();

	virtual void GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base);
	virtual void GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id);

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base);
	virtual void GeneratePacketTabularText(U64 packet_id, DisplayBase display_base);
	virtual void GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base);

protected: //functions
	void hexstr( U32 numsize, char *str, U8 *num);
protected: //vars
	MipiRffeAnalyzerSettings *mSettings;
	MipiRffeAnalyzer *mAnalyzer;
};

#endif //MIPIRFFE_ANALYZER_RESULTS
