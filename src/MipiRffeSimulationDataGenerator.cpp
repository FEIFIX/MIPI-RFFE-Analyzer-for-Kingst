#include "MipiRffeSimulationDataGenerator.h"
#include "MipiRffeAnalyzerSettings.h"

MipiRffeSimulationDataGenerator::MipiRffeSimulationDataGenerator()
{
}

MipiRffeSimulationDataGenerator::~MipiRffeSimulationDataGenerator()
{
}

void MipiRffeSimulationDataGenerator::Initialize(U32 simulation_sample_rate, MipiRffeAnalyzerSettings *settings)
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mSclkGenerator.Init(simulation_sample_rate / 20, simulation_sample_rate);

	mSdat = mMipiRffeSimulationChannels.Add(settings->mSdatChannel, mSimulationSampleRateHz, BIT_LOW);
	mSclk = mMipiRffeSimulationChannels.Add(settings->mSclkChannel, mSimulationSampleRateHz, BIT_LOW);
	mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(10.0));     //insert 10 bit-periods of idle
}


U32 MipiRffeSimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor **simulation_channels)
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample(largest_sample_requested, sample_rate, mSimulationSampleRateHz);
	U32 num = 94;
	U64 seqs[94] =
	{
		0x3C4A, 0x2F92, 0xA63800, 0x3E50, 0xD69398, 0x279FA, 0x2CE2BFE2AF0, 0x3C09067BBF776, 0x860FE7E7DC, 0x2112665ADF7FE, 0xC32D961CCE, 0x719A79F76, 
		0x43A7A17914, 0x21E200,	0x10D404EA64D60, 0x46C020CCC206, 0x1B64, 0xC6830E, 0x6A5A74, 0x1CE36505AFA, 0x781DBE29A17B96C, 0x34C70CA0B38F8, 
		0x11110CC9E725C, 0xA32FA821AA, 0x71720D114, 0x43A4F3E092, 0x1EA00, 0x18D005A55A4F4, 0x46C029B1D87C, 0x32E8, 0x27CA6C, 0x1AA3D0, 0x14E0CE5FA0A, 
		0xA00D8310, 0x61C7543E0, 0x4845419F34, 0x325B0EF76, 0x70D31BB52, 0x3BAFEBE6C, 0x21E200, 0x10D404F736702, 0x46C0230D081C, 0x1BA4, 0x26905E, 0x324A06, 
		0xCE16CF0B7A, 0x381B0E34BF066AC, 0x46062557F8, 0x6406125C, 0x2336153780, 0x872982C038, 0x3B88D8398, 0x31EA42, 0x8D4044370354, 0x6E02704DBF2, 0xE22, 
		0x27B02C, 0x125622, 0x28E1F0F0956, 0xA02AD1E98E, 0x30C4837DDCBDC, 0x1886EC2B432, 0x23322D582A, 0x71DD3A648, 0x23AE4CCC46, 0x21E242, 0x10D40452035A0,
		0x6E0255D7F46, 0x1E9E, 0x76D8EC, 0x529E24, 0x19CF350C462D2, 0x1E02C27E922, 0x20C6D554680A4, 0x117329101CE6, 0xC321919310, 0x47202509D2,
		0x63B3B5117E, 0x11E222, 0xD0056538E28, 0x86C02BA214A4, 0x1EBC, 0xB6F2C6, 0x522446, 0x14E07354128, 0x601D1FBEDC7EA6C, 0x8605B37DEE, 0x24111BE6,
		0x1432C866950, 0xC712450840, 0x63B4D949DE, 0x11E242, 0x18D0055C46340, 0x86C02CE367D0, 
	};
	U32 seq_len[94] =
	{
		14, 14, 24, 14, 24, 23, 42, 50, 41, 51, 41, 42, 41, 24, 51, 50, 14, 24, 23, 42, 59, 50, 51, 41, 42, 41, 24, 51, 50, 14, 24, 23, 42, 32, 41, 42, 
		41, 42, 41, 24, 51, 50, 14, 24, 23, 42, 59, 41, 33, 41, 42, 41, 24, 51, 50, 14, 24, 23, 42, 41, 50, 42, 41, 42, 41, 24, 51, 50, 14, 24, 23, 51, 
		41, 50, 51, 41, 42, 41, 24, 51, 50, 14, 24, 23, 42, 59, 41, 33, 41, 42, 41, 24, 51, 50, 

	};

	while (mSclk->GetCurrentSampleNumber() < adjusted_largest_sample_requested) {
		U32 i, j;
		for (i = 0; i < num; i++)
		{
			BitExtractor data_bits(seqs[i], AnalyzerEnums::MsbFirst, seq_len[i]);
			mSclk->TransitionIfNeeded(BIT_LOW);
			mSdat->TransitionIfNeeded(BIT_LOW);
			mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(200.0));
			mSdat->Transition();
			mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(10));  // SSC
			mSdat->Transition();
			mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(10));
			for (j = 0; j < seq_len[i]; j++)
			{
				
				if ((i == 3) && (j == 5)) // Race Error
				{
					mSclk->TransitionIfNeeded(BIT_HIGH);
					mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(10));
					mSdat->TransitionIfNeeded(data_bits.GetNextBit());
					mSclk->TransitionIfNeeded(BIT_LOW);
					mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(10));
				}
				else if ((i == 4) && (j == seq_len[i] - 3)) // Hazard Error
				{
					mSclk->TransitionIfNeeded(BIT_HIGH);
					mSdat->TransitionIfNeeded(data_bits.GetNextBit());
					mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(8));
					mSdat->Transition();
					mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(1));
					mSdat->Transition();
					mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(1));
					mSclk->TransitionIfNeeded(BIT_LOW);
					mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(10));
				}
				else
				{
					mSclk->TransitionIfNeeded(BIT_HIGH);
					mSdat->TransitionIfNeeded(data_bits.GetNextBit());
					mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(10));
					mSclk->TransitionIfNeeded(BIT_LOW);
					mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(10));
				}
				
			}
			mMipiRffeSimulationChannels.AdvanceAll(mSclkGenerator.AdvanceByHalfPeriod(200));

		}


	}

	*simulation_channels = mMipiRffeSimulationChannels.GetArray();
	return mMipiRffeSimulationChannels.GetCount();

}