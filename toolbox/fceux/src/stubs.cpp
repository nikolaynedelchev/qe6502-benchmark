#include <types.h>
#include <fceu.h>
#include <sound.h>

#include <array>
#include <cstdarg>
#include <cstdio>

int fceuindbg = 0;
int newppu = 0;
bool overclock_enabled = false;
bool overclocking = false;
bool skip_7bit_overclocking = false;
int normalscanlines = 0;
int totalscanlines = 0;
int postrenderscanlines = 0;
int vblankscanlines = 0;
bool AutoResumePlay = false;
bool frameAdvanceLagSkip = false;
char romNameWhenClosingEmulator[1] = {};
uint64 timestampbase = 0;
int MMC5Hack = 0;
uint32 MMC5HackVROMMask = 0;
uint8* MMC5HackExNTARAMPtr = nullptr;
uint8* MMC5HackVROMPTR = nullptr;
uint8 MMC5HackCHRMode = 0;
uint8 MMC5HackSPMode = 0;
uint8 MMC50x5130 = 0;
uint8 MMC5HackSPScroll = 0;
uint8 MMC5HackSPPage = 0;
int PEC586Hack = 0;
int QTAIHack = 0;
uint8 QTAINTRAM[2048] = {};
uint8 qtaintramreg = 0;
uint8* RAM = nullptr;
int EmulationPaused = 0;
int frameAdvance_Delay = 0;
int RAMInitOption = 0;
void (*GameInterface)(GI) = nullptr;
void (*GameStateRestore)(int) = nullptr;
FCEUGI* GameInfo = nullptr;
int GameAttributes = 0;
uint8 PAL = 0;
int dendy = 0;
bool movieSubtitles = false;
FCEUS FSettings = {};
uint8 Exit = 0;
readfunc ARead[0x10000] = {};
writefunc BWrite[0x10000] = {};
uint32 soundtsoffs = 0;
int32 WaveFinalLen = 0;
int32 WaveHi[40000] = {};
int32 WaveFinal[2048 + 512] = {};
uint8 trimode = 0;
uint8 tricoop = 0;
uint8 PSG[0x18] = {};
int FSettings_SoundVolume = 0;
int FSettings_PCMVolume = 0;
int FSettings_TriangleVolume = 0;
int FSettings_Square1Volume = 0;
int FSettings_Square2Volume = 0;
int FSettings_NoiseVolume = 0;
int StackAddrBackup = -1;
int test = 0;

volatile int codecount = 0;
volatile int datacount = 0;
volatile int undefinedcount = 0;
unsigned char* cdloggerdata = nullptr;
unsigned int cdloggerdataSize = 0;
int debug_loggingCD = 0;
int iaPC = 0;
uint32 iapoffset = 0;
bool break_asap = false;
bool break_on_unlogged_code = false;
bool break_on_unlogged_data = false;
uint64 total_cycles_base = 0;
uint64 delta_cycles_base = 0;
bool break_on_cycles = false;
uint64 break_cycles_limit = 0;
uint64 total_instructions = 0;
uint64 delta_instructions = 0;
bool break_on_instructions = false;
uint64 break_instructions_limit = 0;

void ResetGameLoaded() {}
void FCEU_MemoryRand(uint8* ptr, uint32 size, bool default_zero) { for(uint32 i=0;i<size;i++) ptr[i] = default_zero ? 0 : 0xff; }
void SetReadHandler(int32 start, int32 end, readfunc func) { for(int32 i=start; i<=end; ++i) ARead[static_cast<uint16>(i)] = func; }
void SetWriteHandler(int32 start, int32 end, writefunc func) { for(int32 i=start; i<=end; ++i) BWrite[static_cast<uint16>(i)] = func; }
writefunc GetWriteHandler(int32 a) { return BWrite[static_cast<uint16>(a)]; }
readfunc GetReadHandler(int32 a) { return ARead[static_cast<uint16>(a)]; }
int AllocGenieRW() { return 0; }
void FlushGenieRW() {}
void FCEU_ResetVidSys() {}
void ResetMapping() {}
void ResetNES() {}
void PowerNES() {}
void SetAutoFireOffset(int) {}
void SetAutoFirePattern(int, int) {}
void GetAutoFirePattern(int* onframes, int* offframes) { if(onframes) *onframes=0; if(offframes) *offframes=0; }
bool GetAutoFireState(int) { return false; }
void AutoFire() {}
void FCEUI_RewindToLastAutosave() {}
const char* FCEUI_GetAboutString() { return "fceux cpu-only"; }
uint8 FCEU_ReadRomByte(uint32) { return 0xff; }
void FCEU_WriteRomByte(uint32, uint8) {}
int FCEU_TextScanlineOffset(int y) { return y; }
int FCEU_TextScanlineOffsetFromBottom(int y) { return y; }
bool CheckFileExists(const char*) { return false; }
void FCEU_PrintError(const char* format, ...) { (void)format; }
void FCEU_printf(const char* format, ...) { (void)format; }
void FCEU_DispMessage(const char* format, int disppos, ...) { (void)format; (void)disppos; }
void FCEU_DispMessageOnMovie(const char* format, ...) { (void)format; }
void FCEU_TogglePPU() {}
void SetNESDeemph_OldHacky(uint8, int) {}
void DrawTextTrans(uint8*, uint32, uint8*, uint8) {}
void FCEU_PutImage() {}

void FCEU_SoundCPUHook(int32) {}
void FCEUI_Sound(int) {}
void FCEUI_SetSoundVolume(uint32) {}
void FCEUI_SetSoundQuality(int) {}
void FCEUI_SetLowPass(int) {}
void FCEUI_SetSoundRate(uint32) {}
void FCEUI_SetTriangleVolume(uint32) {}
void FCEUI_SetSquare1Volume(uint32) {}
void FCEUI_SetSquare2Volume(uint32) {}
void FCEUI_SetNoiseVolume(uint32) {}
void FCEUI_SetPCMVolume(uint32) {}
void FCEUI_SetSoundPreGap(uint32) {}
int FlushEmulateSound() { return 0; }
void SetSoundVariables() {}
int FlushEmulateSound(int16*) { return 0; }

int getBank(int) { return 0; }
int GetNesFileAddress(int) { return 0; }
int GetPRGAddress(int) { return 0; }
int GetRomAddress(int) { return 0; }
uint8* GetNesPRGPointer(int) { return nullptr; }
uint8* GetNesCHRPointer(int) { return nullptr; }
void KillDebugger() {}
uint8 GetMem(uint16 A) { return ARead[A] ? ARead[A](A) : 0xff; }
uint8 GetPPUMem(uint8) { return 0; }
void LogCDVectors(int) {}
void LogCDData(uint8*, uint16, int) {}
void DebugCycle() {}
bool CondForbidTest(int) { return false; }
void BreakHit(int) {}
void ResetDebugStatisticsCounters() {}
void ResetCyclesCounter() {}
void ResetInstructionsCounter() {}
void ResetDebugStatisticsDeltaCounters() {}
void IncrementInstructionsCounters() { ++total_instructions; ++delta_instructions; }
