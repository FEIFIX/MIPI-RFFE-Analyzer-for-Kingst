#ifndef MIPIRFFE_SIMULATION_DATA_GENERATOR
#define MIPIRFFE_SIMULATION_DATA_GENERATOR

#include <AnalyzerHelpers.h>

class MipiRffeAnalyzerSettings;

class MipiRffeSimulationDataGenerator
{
public:
	MipiRffeSimulationDataGenerator();
	~MipiRffeSimulationDataGenerator();

	void Initialize(U32 simulation_sample_rate, MipiRffeAnalyzerSettings *settings);
	U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels);

protected:
	MipiRffeAnalyzerSettings *mSettings;
	U32 mSimulationSampleRateHz;
	U64 mValue;

protected: //MIPIRFFE specific
	ClockGenerator mSclkGenerator;


	SimulationChannelDescriptorGroup mMipiRffeSimulationChannels;
	SimulationChannelDescriptor *mSclk;
	SimulationChannelDescriptor *mSdat;
};
#endif //MIPIRFFE_SIMULATION_DATA_GENERATOR
