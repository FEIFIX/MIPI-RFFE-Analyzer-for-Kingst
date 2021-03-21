#include "MipiRffeAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "MipiRffeAnalyzer.h"
#include "MipiRffeAnalyzerSettings.h"
#include <iostream>
#include <sstream>

#pragma warning(disable: 4996) //warning C4996: 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.

MipiRffeAnalyzerResults::MipiRffeAnalyzerResults(MipiRffeAnalyzer *analyzer, MipiRffeAnalyzerSettings *settings)
	: AnalyzerResults(),
	mSettings(settings),
	mAnalyzer(analyzer)
{
}

MipiRffeAnalyzerResults::~MipiRffeAnalyzerResults()
{
}

void MipiRffeAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base)    //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();
	Frame frame = GetFrame(frame_index);
	char number_str1[128];
	char number_str2[128];
	char warning[128];
	char star[128];
	strcpy(warning, "");
	strcpy(star, "");
	if (frame.mFlags & PARITY_ERROR_FLAG) { strcpy(warning, " (Parity Error)"); strcpy(star, " *");}
	if (frame.mFlags & RACE_ERROR_FLAG) { strcpy(warning, " (Race Warning)"); strcpy(star, " *");}
	if (frame.mFlags & HAZARD_ERROR_FLAG) { strcpy(warning, " (Hazard Warning)"); strcpy(star, " *");}
	if (frame.mFlags & NORESPONSES_ERROR_FLAG) { strcpy(warning, " (Slave No Response)"); strcpy(star, " *");}
	if (frame.mFlags & TYPE_ERROR_FLAG) { strcpy(warning, " (UnDefined CMD Type)"); strcpy(star, " *");}
	
	if (frame.mType == Command)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 4, number_str1, 128);
		

		U8 cmd = frame.mData2;
		if (cmd >= 0x80) 
		{ 
			AnalyzerHelpers::GetNumberString(frame.mData2 % 128, display_base, 7, number_str2, 128);
			AddResultString("Register 0 Write: USID=", number_str1," RegAddr=0 Data=", number_str2, warning);
			AddResultString("Register 0 Write: ", number_str1, " 0 ", number_str2, warning);
			AddResultString("R0W: ", number_str1, " 0 ", number_str2, star);
			AddResultString("R0W", star);
		}
		else if (cmd >= 0x60) 
		{  
			AnalyzerHelpers::GetNumberString(frame.mData2 % 32, display_base, 5, number_str2, 128);
			AddResultString("Register Read: USID=", number_str1, " RegAddr=", number_str2, warning);
			AddResultString("Register Read: ", number_str1, " ", number_str2, warning);
			AddResultString("RR: ", number_str1, " ", number_str2, star);
			AddResultString("RR", star);
		}
		else if (cmd >= 0x40)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 32, display_base, 5, number_str2, 128);
			AddResultString("Register Write: USID=", number_str1, " RegAddr=", number_str2, warning);
			AddResultString("Register Write: ", number_str1, " ", number_str2, warning);
			AddResultString("RW: ", number_str1, " ", number_str2, star);
			AddResultString("RW", star);
		}
		else if (cmd >= 0x38)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 8, display_base, 3, number_str2, 128);
			AddResultString("Extended Register Read Long: USID=", number_str1, " ByteCount=", number_str2, warning);
			AddResultString("Extended Register Read Long: ", number_str1, " ", number_str2, warning);
			AddResultString("ERRL: ", number_str1, " ", number_str2, star);
			AddResultString("ERRL", star);
		}
		else if (cmd >= 0x30)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 8, display_base, 3, number_str2, 128);
			AddResultString("Extended Register Write Long: USID=", number_str1, " ByteCount=", number_str2, warning);
			AddResultString("Extended Register Write Long: ", number_str1, " ", number_str2, warning);
			AddResultString("ERWL: ", number_str1, " ", number_str2, star);
			AddResultString("ERWL", star);
		}
		else if (cmd >= 0x20)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 16, display_base, 4, number_str2, 128);
			AddResultString("Extended Register Read: USID=", number_str1, " ByteCount=", number_str2, warning);
			AddResultString("Extended Register Read: ", number_str1, " ", number_str2, warning);
			AddResultString("ERR: ", number_str1, " ", number_str2, star);
			AddResultString("ERR", star);
		}
		else if (cmd == 0x1f)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			AddResultString("Undefined: ", number_str1, " Cmd=", number_str2, warning);
			AddResultString("Undef", star);
		}
		else if (cmd == 0x1e)
		{
			AnalyzerHelpers::GetNumberString(frame.mData1 % 4, display_base, 2, number_str1, 128);
			AnalyzerHelpers::GetNumberString(frame.mData2 , display_base, 8, number_str2, 128);
			AddResultString("Mater Ownership Handover: MID=", number_str1, " Cmd=", number_str2, warning);
			AddResultString("Mater Ownership Handover: ", number_str1, " ", number_str2, warning);
			AddResultString("MOH: ", number_str1, " ", number_str2, star);
			AddResultString("MOH", star);
		}
		else if (cmd == 0x1d)
		{
			AnalyzerHelpers::GetNumberString(frame.mData1 % 4, display_base, 2, number_str1, 128);
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			AddResultString("Mater Write: MID=", number_str1, " Cmd=", number_str2, warning);
			AddResultString("Mater Write: ", number_str1, " ", number_str2, warning);
			AddResultString("MW: ", number_str1, " ", number_str2, star);
			AddResultString("MW", star);
		}
		else if (cmd == 0x1c)
		{
			AnalyzerHelpers::GetNumberString(frame.mData1 % 4, display_base, 2, number_str1, 128);
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			AddResultString("Mater Read: MID=", number_str1, " Cmd=", number_str2, warning);
			AddResultString("Mater Read: ", number_str1, " ", number_str2, warning);
			AddResultString("MR: ", number_str1, " ", number_str2, star);
			AddResultString("MR", star);
		}
		else if (cmd == 0x1b)
		{
			AnalyzerHelpers::GetNumberString(frame.mData1 % 4, display_base, 2, number_str1, 128);
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			AddResultString("Mater Context Transer - Write: MID=", number_str1, " Cmd=", number_str2, warning);
			AddResultString("Mater Context Transer - W: ", number_str1, " ", number_str2, warning);
			AddResultString("MCT-W: ", number_str1, " ", number_str2, star);
			AddResultString("MCT-W", star);
		}
		else if (cmd == 0x1a)
		{
			AnalyzerHelpers::GetNumberString(frame.mData1 % 4, display_base, 2, number_str1, 128);
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			AddResultString("Mater Context Transer - Read: MID=", number_str1, " Cmd=", number_str2, warning);
			AddResultString("Mater Context Transer - R: ", number_str1, " ", number_str2, warning);
			AddResultString("MCT-R: ", number_str1, " ", number_str2, star);
			AddResultString("MCT-R", star);
		}
		else if (cmd == 0x19)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			AddResultString("Masked Write: USID=", number_str1, " Cmd=", number_str2, warning);
			AddResultString("Masked Write: ", number_str1, " ", number_str2, warning);
			AddResultString("MkW: ", number_str1, " ", number_str2, star);
			AddResultString("MkW", star);
		}
		else if (cmd >= 0x10)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			AddResultString("Undefined: ", number_str1, " Cmd=", number_str2, warning);
			AddResultString("Undef", star);
		}
		else
		{
		AnalyzerHelpers::GetNumberString(frame.mData2 % 16, display_base, 4, number_str2, 128);
		AddResultString("Extended Register Write: USID=", number_str1, " ByteCount=", number_str2, warning);
		AddResultString("Extended Register Write: ", number_str1, " ", number_str2, warning);
		AddResultString("ERW: ", number_str1, " ", number_str2, star);
		AddResultString("ERW", star);
		}

		
	}
	else if (frame.mType == Data)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str1, 128);
		AddResultString("Data: ", number_str1, warning);
		AddResultString("Data", star);
	}
	else if (frame.mType == Address)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str1, 128);
		AddResultString("Address: ", number_str1, warning);
		AddResultString("Addr", star);
	}
	else if (frame.mType == Mask)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str1, 128);
		AddResultString("DataMask: ", number_str1, warning);
		AddResultString("Mask", number_str1, star);
	}
	else if (frame.mType == ByteCount)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str1, 128);
		AddResultString("ByteCount: ", number_str1, warning);
		AddResultString("BC", star);
	}
	else if (frame.mType == NewMasterResponse)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str1, 128);
		AddResultString("NewMasterResponse: ", number_str1, warning);
		AddResultString("Resp: ", star);
	}
}

void MipiRffeAnalyzerResults::hexstr(U32 numsize, char *str, U8 *num)
{
	U32 i, s;
	char buf[3];
	s = 0;
	for (i = 0; i < numsize; i++)
	{
		snprintf(buf, 3, "%X", num[i]);
		str[s] = buf[0];
		s++;
		str[s] = buf[1];
		s++;
	}
	snprintf(buf, 3, "\0");
	str[s] = buf[0];
}

void MipiRffeAnalyzerResults::GenerateExportFile(const char *file, DisplayBase display_base, U32 /*export_type_user_id*/)
{
	//export_type_user_id is only important if we have more than one export type.

	std::stringstream ss;
	void *f = AnalyzerHelpers::StartFile(file);

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	ss << "Time[s],Command Type,USID,Register Address,Byte Count,Data" << std::endl;

	char time_str[128] = "";
	char command_type[128] = "";
	char reg_num_str[128] = "";
	char usid_num_str[128] = "";
	char byte_count_str[128] = "";
	char data_num_str[1024] = "";
	U32 data_count = 0, reg = 0, byte_count;
	U8 cmd, usid;
	U8 data[512];


	U64 num_frames = GetNumFrames();
	for (U32 i = 0; i <= num_frames; i++) {
		Frame frame = GetFrame(i);

		if ((frame.mType == Command) || (i == num_frames))
		{
			if (i > 0)
			{
				snprintf(reg_num_str, 128, "%X", reg);
				snprintf(usid_num_str, 128, "%X", usid);
				snprintf(byte_count_str, 128, "%d", byte_count);
				hexstr(data_count, data_num_str, data);
				ss << time_str << "," << command_type << ",0x" << usid_num_str << ",0x" << reg_num_str << "," << byte_count_str << ",0x" << data_num_str << std::endl;

			}
			strcpy(time_str, "");
			strcpy(command_type, "");
			strcpy(reg_num_str, "");
			strcpy(usid_num_str, "");
			strcpy(data_num_str, "");
			strcpy(byte_count_str, "");
			data_count = 0;
			byte_count = 0;
			reg = 0;
			usid = 0;
			cmd = frame.mData2;
			AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);
			if (cmd >= 0x80)
			{
				strcpy(command_type, "Register 0 Write");
				usid = frame.mData1;
				reg = 0;
				data[data_count] = cmd % 128;
				data_count += 1;
				byte_count = 1;

			}
			else if (cmd >= 0x60)
			{
				strcpy(command_type, "Register Read");
				usid = frame.mData1;
				reg = frame.mData2 % 32;
				byte_count = 1;
			}
			else if (cmd >= 0x40)
			{
				strcpy(command_type, "Register Write");
				usid = frame.mData1;
				reg = frame.mData2 % 32;
				byte_count = 1;
			}
			else if (cmd >= 0x38)
			{
				strcpy(command_type, "Extended Rigister Read Long");
				usid = frame.mData1;
				byte_count = frame.mData2 % 8;
				byte_count++;

			}
			else if (cmd >= 0x30)
			{
				strcpy(command_type, "Extended Rigister Write Long");
				usid = frame.mData1;
				byte_count = frame.mData2 % 8;
				byte_count++;
			}
			else if (cmd >= 0x20)
			{
				strcpy(command_type, "Extended Rigister Read");
				usid = frame.mData1;
				byte_count = frame.mData2 % 16;
				byte_count++;
			}
			else if (cmd == 0x1f)
			{
				strcpy(command_type, "Undefined");
				usid = frame.mData1;
			}
			else if (cmd == 0x1e)
			{
				strcpy(command_type, "Master Ownership Handover");
				usid = frame.mData1;
				byte_count = 0;
				byte_count++;
			}
			else if (cmd == 0x1d)
			{
				strcpy(command_type, "Master Write");
				usid = frame.mData1;
				byte_count = 1;
				byte_count++;
			}
			else if (cmd == 0x1c)
			{
				strcpy(command_type, "Master Read");
				usid = frame.mData1;
				byte_count = 1;
				byte_count++;
			}
			else if (cmd == 0x1b)
			{
				strcpy(command_type, "Master Context Transfer - Write");
				usid = frame.mData1;
			}
			else if (cmd == 0x1a)
			{
				strcpy(command_type, "Master Context Transfer - Read");
				usid = frame.mData1;
			}
			else if (cmd == 0x19)
			{
				strcpy(command_type, "Masked Write");
				usid = frame.mData1;
			}
			else if (cmd >= 0x10)
			{
				strcpy(command_type, "Undefined");
				usid = frame.mData1;
			}
			else
			{
				strcpy(command_type, "Extended Rigister Write");
				usid = frame.mData1;
				byte_count = frame.mData2 % 16;
				byte_count++;
			}
		}
		else if (frame.mType == Address)
		{
			reg = reg << 8;
			reg += frame.mData1;
		}
		else if (frame.mType == ByteCount)
		{
			byte_count = frame.mData1;
			byte_count++;
		}
		else
		{
			data[data_count] = frame.mData1;
			data_count++;
		}

		AnalyzerHelpers::AppendToFile((U8 *)ss.str().c_str(), ss.str().length(), f);
		ss.str(std::string());
	}
	

	UpdateExportProgressAndCheckForCancel(num_frames, num_frames);
	AnalyzerHelpers::EndFile(f);
	
}

void MipiRffeAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
	ClearTabularText();
	Frame frame = GetFrame(frame_index);
	char number_str1[128];
	char number_str2[128];
	
	std::stringstream ss;
	if (frame.mType == Command)
	{

		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 4, number_str1, 128);

		U8 cmd = frame.mData2;
		if (cmd >= 0x80)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 128, display_base, 7, number_str2, 128);
			ss << "R0W: " << number_str1 << " " << number_str2;

		}
		else if (cmd >= 0x60)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 32, display_base, 5, number_str2, 128);
			ss << "RR: " << number_str1 << " " << number_str2;
			//AddResultString("Register Read: ", number_str1, " ", number_str2);
			//AddResultString("RR: ", number_str1, " ", number_str2);
			//AddResultString("RR");
		}
		else if (cmd >= 0x40)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 32, display_base, 5, number_str2, 128);
			ss << "RW: " << number_str1 << " " << number_str2;
			//AddResultString("Register Write: ", number_str1, " ", number_str2);
			//AddResultString("RW: ", number_str1, " ", number_str2);
			//AddResultString("RW");
		}
		else if (cmd >= 0x38)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 8, display_base, 3, number_str2, 128);
			ss << "ERRL: " << number_str1 << " " << number_str2;
			//AddResultString("Extended Register Read Long: ", number_str1, " ", number_str2);
			//AddResultString("ERRL: ", number_str1, " ", number_str2);
			//AddResultString("ERRL");
		}
		else if (cmd >= 0x30)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 8, display_base, 3, number_str2, 128);
			ss << "ERWL: " << number_str1 << " " << number_str2;
			//AddResultString("Extended Register Write Long: ", number_str1, " ", number_str2);
			//AddResultString("ERWL: ", number_str1, " ", number_str2);
			//AddResultString("ERWL");
		}
		else if (cmd >= 0x20)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 16, display_base, 4, number_str2, 128);
			ss << "ERR: " << number_str1 << " " << number_str2;
			//AddResultString("Extended Register Read: ", number_str1, " ", number_str2);
			//AddResultString("ERR: ", number_str1, " ", number_str2);
			//AddResultString("ERR");
		}
		else if (cmd == 0x1f)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			ss << "Undef: " << number_str1 << " " << number_str2;
			//AddResultString("Undef");
		}
		else if (cmd == 0x1e)
		{
			AnalyzerHelpers::GetNumberString(frame.mData1 % 4, display_base, 2, number_str1, 128);
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			ss << "MOH: " << number_str1 << " " << number_str2;
			//AddResultString("Mater Ownership Handover: ", number_str1, " ", number_str2);
			//AddResultString("MOH: ", number_str1, " ", number_str2);
			//AddResultString("MOH");
		}
		else if (cmd == 0x1d)
		{
			AnalyzerHelpers::GetNumberString(frame.mData1 % 4, display_base, 2, number_str1, 128);
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			ss << "MW: " << number_str1 << " " << number_str2;
			//AddResultString("Mater Write: ", number_str1, " ", number_str2);
			//AddResultString("MW: ", number_str1, " ", number_str2);
			//AddResultString("MW");
		}
		else if (cmd == 0x1c)
		{
			AnalyzerHelpers::GetNumberString(frame.mData1 % 4, display_base, 2, number_str1, 128);
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			ss << "MR: " << number_str1 << " " << number_str2;
			//AddResultString("Mater Read: ", number_str1, " ", number_str2);
			//AddResultString("MR: ", number_str1, " ", number_str2);
			//AddResultString("MR");
		}
		else if (cmd == 0x1b)
		{
			AnalyzerHelpers::GetNumberString(frame.mData1 % 4, display_base, 2, number_str1, 128);
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			ss << "MCT-W: " << number_str1 << " " << number_str2;
			//AddResultString("Mater Context Transer - W: ", number_str1, " ", number_str2);
			//AddResultString("MCT-W: ", number_str1, " ", number_str2);
			//AddResultString("MCT-W");
		}
		else if (cmd == 0x1a)
		{
			AnalyzerHelpers::GetNumberString(frame.mData1 % 4, display_base, 2, number_str1, 128);
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			ss << "MCT-R: " << number_str1 << " ", number_str2;
			//AddResultString("Mater Context Transer - R: ", number_str1, " ", number_str2);
			//AddResultString("MCT-R: ", number_str1, " ", number_str2);
			//AddResultString("MCT-R");
		}
		else if (cmd == 0x19)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			ss << "MkW: " << number_str1 << " ", number_str2;
			//AddResultString("Masked Write: ", number_str1, " ", number_str2);
			//AddResultString("MkW: ", number_str1, " ", number_str2);
			//AddResultString("MkW");
		}
		else if (cmd >= 0x10)
		{
			AnalyzerHelpers::GetNumberString(frame.mData2, display_base, 8, number_str2, 128);
			ss << "Undef: " << number_str1 << " " << number_str2;
			//AddResultString("Undef");
		}
		else
		{
			AnalyzerHelpers::GetNumberString(frame.mData2 % 16, display_base, 4, number_str2, 128);
			ss << "ERW: " << number_str1 << " " << number_str2;
			//AddResultString("Extended Register Write: ", number_str1, " ", number_str2);
			//AddResultString("ERW: ", number_str1, " ", number_str2);
			//AddResultString("ERW");
		}


	}
	else if (frame.mType == Data)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str1, 128);
		ss << "Data: " << number_str1;
	}
	else if (frame.mType == Address)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str1, 128);
		ss << "Address: " << number_str1;
		//ss << "Addr");
	}
	else if (frame.mType == Mask)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str1, 128);
		ss << "DataMask: " << number_str1;
		//ss << "Mask" << number_str1;
	}
	else if (frame.mType == ByteCount)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str1, 128);
		ss << "ByteCount: " << number_str1;
		//ss << "BC");
	}
	else if (frame.mType == NewMasterResponse)
	{
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str1, 128);
		ss << "Response: " << number_str1;
		//ss << "NMR" << number_str1;
	}

	if (frame.mFlags & PARITY_ERROR_FLAG) { ss << " (Parity Error)"; }
	if (frame.mFlags & RACE_ERROR_FLAG) { ss << " (Race Warning)"; }
	if (frame.mFlags & HAZARD_ERROR_FLAG) { ss << " (Hazard Warning)"; }
	if (frame.mFlags & NORESPONSES_ERROR_FLAG) { ss << " (Slave No Response)"; }
	if (frame.mFlags & TYPE_ERROR_FLAG) { ss << " (UnDefined CMD Type)"; }
	AddTabularText(ss.str().c_str());
}

void MipiRffeAnalyzerResults::GeneratePacketTabularText(U64 /*packet_id*/, DisplayBase /*display_base*/)    //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();
	AddResultString("not supported");
}

void MipiRffeAnalyzerResults::GenerateTransactionTabularText(U64 /*transaction_id*/, DisplayBase /*display_base*/)    //unrefereced vars commented out to remove warnings.
{
	ClearResultStrings();
	AddResultString("not supported");
}
