#include "../include/Application.h"

Application g_App;

int main()
{
	g_App.Initialize();

	while (true)
	{
	}

	return 0;
}

// #include "daisy_seed.h"

// using namespace daisy;

// DaisySeed hw;

// void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
// {
// 	for (size_t i = 0; i < size; i++)
// 	{
// 		out[0][i] = in[0][i];
// 		out[1][i] = in[1][i];
// 	}
// }

// int main(void)
// {
// 	hw.Init();
// 	hw.SetAudioBlockSize(4); // number of samples handled per callback
// 	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
// 	hw.StartAudio(AudioCallback);
// 	while (1)
// 	{
// 	}
// }
