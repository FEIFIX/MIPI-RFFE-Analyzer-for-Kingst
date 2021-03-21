
#include "MipiRffeAnalyzer.h"
#include "MipiRffeAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

MipiRffeAnalyzer::MipiRffeAnalyzer()
	: Analyzer(),
	mSettings(new MipiRffeAnalyzerSettings()),
	mSimulationInitilized(false),
	mSclk(NULL),
	mSdat(NULL)
{
	SetAnalyzerSettings(mSettings.get());
}

MipiRffeAnalyzer::~MipiRffeAnalyzer()
{
	KillThread();
}

void MipiRffeAnalyzer::SetupResults()
{
	mResults.reset(new MipiRffeAnalyzerResults(this, mSettings.get()));
	SetAnalyzerResults(mResults.get());

	mResults->AddChannelBubblesWillAppearOn(mSettings->mSdatChannel);

}

void MipiRffeAnalyzer::AdvanceToSscEnd()
{
	U8 nc = 0xFF;
	U8 nd = 0xFF;
	U8 n0, n1;
	U64 pos1, pos2;
	pos1 = pos2 = 0;
	// 调用之前要求对齐
	if (mSdat->GetBitState() != BIT_LOW)
	{
		BothAndvanceToNextEdge(BySdat);
	} 
	for (;;)
	{
		mSdat->Advance(1);
		mSclk->Advance(1);
		n0 = mSdat->GetBitState();
		n1 = mSclk->GetBitState();
		if ((n0 != nd % 2) || (n1 != nc % 2)) 
		{ 
			nd = nd * 2; 
			nd += n0;  
			nc = nc * 2; 
			nc += n1;
			pos1 = pos2;
			pos2 = mSdat->GetSampleNumber();
		}
		if ((nc % 8 == 0) && (nd % 8 == 2))
			break;
	}
	mResults->AddMarker((pos2+pos1)/2, AnalyzerResults::Square, mSettings->mSdatChannel);
	if (BothAndvanceToNextEdge(BySclk) >= 2)
	{
		ERRORs |= HAZARD_ERROR_FLAG;
		pos1 = mSdat->GetSampleNumber();
		mResults->AddMarker(pos1, AnalyzerResults::ErrorSquare, mSettings->mSdatChannel);
	}

}

bool MipiRffeAnalyzer::CheckParity(U32 rawdata) //奇偶校验Fail则返回true
{
	U32  data;
	bool n;
	data = rawdata;
	n = true;
	for (;;)
	{
		if (data == 0){return n;}
		if (data % 2) { n = !n; }
		data = data / 2;
	}

}

U8 MipiRffeAnalyzer::AnalyzeByCsType(U8 cmd)
{
	if (cmd >= 0x80) { Analyze80(cmd); return 0x80; }
	else if (cmd >= 0x60) { Analyze60(cmd); return 0x60; }
	else if (cmd >= 0x40) { Analyze40(cmd); return 0x40; }
	else if (cmd >= 0x38) { Analyze38(cmd); return 0x38; }
	else if (cmd >= 0x30) { Analyze30(cmd); return 0x30; }
	else if (cmd >= 0x20) { Analyze20(cmd); return 0x20; }
	else if (cmd == 0x1f) { return 0x1f; }
	else if (cmd == 0x1e) { Analyze1E(cmd); return 0x1e; }
	else if (cmd == 0x1d) { Analyze1D(cmd); return 0x1d; }
	else if (cmd == 0x1c) { Analyze1C(cmd); return 0x1c; }
	else if (cmd == 0x1b) { Analyze1B(cmd); return 0x1b; }
	else if (cmd == 0x1a) { Analyze1A(cmd); return 0x1a; }
	else if (cmd == 0x19) { Analyze19(cmd); return 0x19; }
	else if (cmd >= 0x10) { return 0x10; }
	else { Analyze00(cmd); return 0x0; }
}


U8 MipiRffeAnalyzer::AdvanceAndRead(U64 *last_pos)
{
	U64 pos;
	U8 ha, res1, res2;
	mSclk->AdvanceToNextEdge();
	pos = mSclk->GetSampleNumber() - 1;
	ha = mSdat->AdvanceToAbsPosition(pos);
	res1 = mSdat->GetBitState();
	mSdat->Advance(1);
	res2 = mSdat->GetBitState();
	ha += BothAndvanceToNextEdge(BySclk);
	if (ha >= 2) 
	{ 
		ERRORs |= HAZARD_ERROR_FLAG; 
		mResults->AddMarker(pos, AnalyzerResults::ErrorSquare, mSettings->mSdatChannel);
	}
	if (res1 != res2) 
	{ 
		ERRORs |= RACE_ERROR_FLAG; 
		mResults->AddMarker(pos, AnalyzerResults::ErrorX, mSettings->mSdatChannel);
	}
	mResults->AddMarker(pos, AnalyzerResults::DownArrow, mSettings->mSclkChannel);
	last_pos[0] = pos;
	return res1;
}

U64 MipiRffeAnalyzer::AdvanceAndReadBits(U32 n, U64 *last_pos)
{
	U64 i, res;
	res = 0;
	for (i = 0; i < n; i++)
	{
		res = res * 2;
		res += AdvanceAndRead(last_pos);
	}
	
	return res;
}

U8 MipiRffeAnalyzer::BothAndvanceToNextEdge(enum BaseOnChannel ch)
{
	U64 pos;
	U8 n;
	if (ch == BySclk)
	{
		mSclk->AdvanceToNextEdge();
		pos = mSclk->GetSampleNumber();
		n = mSdat->AdvanceToAbsPosition(pos);
	}
	else if (ch == BySdat)
	{
		mSdat->AdvanceToNextEdge();
		pos = mSdat->GetSampleNumber();
		n = mSclk->AdvanceToAbsPosition(pos);
	}
	return n;
}

void MipiRffeAnalyzer::SkipBpc()
{
	if (BothAndvanceToNextEdge(BySclk) >= 2)
	{
		ERRORs |= HAZARD_ERROR_FLAG;
	}
}

void MipiRffeAnalyzer::SkipBpc2() //用于单个序列中间的主从切换BPC,因为此类BPC后面的Frame没有SSC，需要帮助下个Frame找到上升沿
{
	if (BothAndvanceToNextEdge(BySclk) + BothAndvanceToNextEdge(BySclk) >= 2)
	{
		ERRORs |= HAZARD_ERROR_FLAG;
	}
}

void MipiRffeAnalyzer::SkipBhc()
{
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze80(U8 cmd)
{
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze60(U8 cmd)
{
	SkipBpc2();
	Analyze9bits(Data);
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze40(U8 cmd)
{
	Analyze9bits(Data);
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze38(U8 cmd)
{
	U8 i = 0;
	Analyze9bits(Address);
	Analyze9bits(Address);
	SkipBpc2();
	for (i = 0; i < (cmd % 8 + 1); i++)
	{
		Analyze9bits(Data);
	}
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze30(U8 cmd)
{
	U8 i = 0;
	Analyze9bits(Address);
	Analyze9bits(Address);
	for (i = 0; i < (cmd % 8 + 1); i++)
	{
		Analyze9bits(Data);
	}
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze20(U8 cmd)
{
	U8 i = 0;
	Analyze9bits(Address);
	SkipBpc2();
	for (i = 0; i < (cmd % 16 + 1); i++)
	{
		Analyze9bits(Data);
	}
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze00(U8 cmd)
{
	U8 i = 0;
	Analyze9bits(Address);
	for (i = 0; i < (cmd % 16 + 1); i++)
	{
		Analyze9bits(Data);
	}
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze19(U8 cmd)
{
	U8 i = 0;
	Analyze9bits(Address);
	Analyze9bits(Mask);
	Analyze9bits(Data);
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze1A(U8 cmd)
{
	U8 i = 0;
	U8 n = 0;
	n = Analyze9bits(ByteCount);
	Analyze9bits(Address);
	SkipBpc2();
	for (i = 0; i < (n + 1); i++)
	{
		Analyze9bits(Data);
	}
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze1B(U8 cmd)
{
	U8 i = 0;
	U8 n = 0;
	n = Analyze9bits(ByteCount);
	Analyze9bits(Address);
	for (i = 0; i < (n + 1); i++)
	{
		Analyze9bits(Data);
	}
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze1C(U8 cmd)
{
	Analyze9bits(Address);
	SkipBpc2();
	Analyze9bits(Data);
	Analyze9bits(Data);
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze1D(U8 cmd)
{
	Analyze9bits(Address);
	Analyze9bits(Data);
	Analyze9bits(Data);
	SkipBpc();
}

void MipiRffeAnalyzer::Analyze1E(U8 cmd)
{
	SkipBpc2();
	Analyze9bits(NewMasterResponse);
	SkipBhc();
}

U8 MipiRffeAnalyzer::Analyze9bits(enum FrameType ft)
{
	U32 n = 0;
	Frame dataFrame;
	ERRORs = 0;
	U64 last_edge_pos[1];
	dataFrame.mStartingSampleInclusive = mSclk->GetSampleNumber();
	n = AdvanceAndReadBits(9, last_edge_pos);
	dataFrame.mEndingSampleInclusive = mSclk->GetSampleNumber();
	dataFrame.mFlags = 0;
	if (CheckParity(n)) 
	{ 
		ERRORs |= PARITY_ERROR_FLAG;
		mResults->AddMarker(last_edge_pos[0], AnalyzerResults::ErrorDot, mSettings->mSdatChannel);
	}
	if (n == 0) { ERRORs |= NORESPONSES_ERROR_FLAG; }
	dataFrame.mFlags = ERRORs;
	n = n / 2;
	dataFrame.mData1 = n;
	dataFrame.mType = ft;
	mResults->AddFrame(dataFrame);
	mResults->CommitResults();
	ReportProgress(dataFrame.mEndingSampleInclusive);
	return n;
}

void MipiRffeAnalyzer::WorkerThread()
{
	U64 mCurrentSample = 0;
	U32 i = 0;
	U64 n = 0; 
	U64 mCommandFrameWithParity = 0;
	U64 last_edge[1];
	mSclk = GetAnalyzerChannelData(mSettings->mSclkChannel);
	mSdat = GetAnalyzerChannelData(mSettings->mSdatChannel);
	for (;;) 
	{
		mResults->CommitPacketAndStartNewPacket();
		Frame cmdFrame;
		ERRORs = 0;
		AdvanceToSscEnd();
		cmdFrame.mStartingSampleInclusive = mSclk->GetSampleNumber();
		mCommandFrameWithParity = AdvanceAndReadBits(13, last_edge);
		cmdFrame.mFlags = 0;
		
		if (CheckParity(mCommandFrameWithParity)) 
		{ 
			ERRORs |= PARITY_ERROR_FLAG; 
			mResults->AddMarker(last_edge[0], AnalyzerResults::ErrorDot, mSettings->mSdatChannel);
		}
		
		n = mCommandFrameWithParity;
		n = n / 2;
		cmdFrame.mData2 = n % 256; // command data
		n = n / 256;
		cmdFrame.mData1 = n % 16;  // USID 
		cmdFrame.mEndingSampleInclusive = mSclk->GetSampleNumber();
		if (((cmdFrame.mData2 >= 0x10) && (cmdFrame.mData2 <= 0x18)) || (cmdFrame.mData2 == 0x1f))
		{
			ERRORs |= TYPE_ERROR_FLAG;
		}
		cmdFrame.mFlags = ERRORs;
		cmdFrame.mType = Command;
		mResults->AddFrame(cmdFrame);
		mResults->CommitResults();
		ReportProgress(cmdFrame.mEndingSampleInclusive);
		AnalyzeByCsType(cmdFrame.mData2);
		CheckIfThreadShouldExit();
	}
}

bool MipiRffeAnalyzer::NeedsRerun()
{
	return false;
}

U32 MipiRffeAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor **simulation_channels)
{
	if (mSimulationInitilized == false) {
		mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
		mSimulationInitilized = true;
	}
	return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}


U32 MipiRffeAnalyzer::GetMinimumSampleRateHz()
{
	return 1000000; //1MHz.
}

const char *MipiRffeAnalyzer::GetAnalyzerName() const
{
	return "MIPI-RFFE";
}

const char *GetAnalyzerName()
{
	return "MIPI-RFFE";
}

Analyzer *CreateAnalyzer()
{
	return new MipiRffeAnalyzer();
}

void DestroyAnalyzer(Analyzer *analyzer)
{
	delete analyzer;
}
