#ifndef MIPIRFFE_ANALYZER_H
#define MIPIRFFE_ANALYZER_H

#include <Analyzer.h>
#include "MipiRffeAnalyzerResults.h"
#include "MipiRffeSimulationDataGenerator.h"

class MipiRffeAnalyzerSettings;
enum FrameType { Command, Address, Data, Mask, ByteCount, NewMasterResponse };
enum BaseOnChannel {BySclk, BySdat};


class ANALYZER_EXPORT MipiRffeAnalyzer : public Analyzer
{
public:
	MipiRffeAnalyzer();
	virtual ~MipiRffeAnalyzer();
	virtual void SetupResults();
	virtual void WorkerThread();


	virtual U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels);
	virtual U32 GetMinimumSampleRateHz();

	virtual const char *GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //functions
	void AdvanceToSscEnd();
	void SkipBpc();
	void SkipBpc2();
	void SkipBhc();
	bool CheckParity(U32 rawdata);
	U8 AnalyzeByCsType(U8 cmd);
	void Analyze80(U8 cmd);
	void Analyze60(U8 cmd);
	void Analyze40(U8 cmd);
	void Analyze38(U8 cmd);
	void Analyze30(U8 cmd);
	void Analyze20(U8 cmd);
	void Analyze00(U8 cmd);
	void Analyze19(U8 cmd);
	void Analyze1A(U8 cmd);
	void Analyze1B(U8 cmd);
	void Analyze1C(U8 cmd);
	void Analyze1D(U8 cmd);
	void Analyze1E(U8 cmd);
	U8 BothAndvanceToNextEdge(enum BaseOnChannel ch);
	U8 Analyze9bits(enum FrameType ft);
	U8 AdvanceAndRead(U64 *last_pos);
	U64 AdvanceAndReadBits(U32 n, U64 *last_pos);

#pragma warning( push )
#pragma warning( disable : 4251 ) //warning C4251: 'SerialAnalyzer::<...>' : class <...> needs to have dll-interface to be used by clients of class
protected:  //vars
	std::auto_ptr< MipiRffeAnalyzerSettings > mSettings;
	std::auto_ptr< MipiRffeAnalyzerResults > mResults;
	bool mSimulationInitilized;
	MipiRffeSimulationDataGenerator mSimulationDataGenerator;


	AnalyzerChannelData *mSclk;
	AnalyzerChannelData *mSdat;

	U64 mCurrentSample;
	U8 ERRORs;
	AnalyzerResults::MarkerType mArrowMarker;
	std::vector<U64> mArrowLocations;

#pragma warning( pop )
};

extern "C" ANALYZER_EXPORT const char *__cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer *__cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer *analyzer);

#endif //MIPIRFFE_ANALYZER_H
