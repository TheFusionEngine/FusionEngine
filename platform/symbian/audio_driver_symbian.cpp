/*************************************************************************/
/*  audio_driver_psp.cpp                                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "audio_driver_symbian.h"
#include "core/os/thread.h"
#include "globals.h"
#include "os/os.h"
#include <string.h>

const char *AudioDriverMDA::get_name() const { return "Symbian MDA"; } 

Error AudioDriverMDA::init() {
	pcm_open = false;

	mix_rate = 44100;
	channels = 2;

	int latency = GLOBAL_DEF("audio/output_latency", 25);
 	buffer_size = nearest_power_of_2(latency * mix_rate / 1000);

	samples_in = memnew_arr(int32_t, buffer_size * channels);
	samples_out = memnew_arr(int16_t, buffer_size * channels);

  iSoundBufferPtr.Set(reinterpret_cast<TUint8 *>(samples_out),
                      buffer_size * channels * sizeof(int16_t));

  //iStreamSettings.Query();
  iStreamSettings.iChannels = channels == 1 ? TMdaAudioDataSettings::EChannelsMono : TMdaAudioDataSettings::EChannelsStereo;
  iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate44100Hz;
  iStreamSettings.iCaps = TMdaAudioDataSettings::ERealTime | TMdaAudioDataSettings::ESampleRateFixed;
	//iStreamSettings.iFlags = TMdaAudioDataSettings::ENoNetworkRouting;

  iOutputStream = CMdaAudioOutputStream::NewL(*this);
  iOutputStream->Open(&iStreamSettings);

	return OK;
}

void AudioDriverMDA::MaoscOpenComplete(TInt aError) {
  if (aError == KErrNone) {
    iVolume = iOutputStream->MaxVolume() / 2;
    iVolumeStep = iOutputStream->MaxVolume() / 8;
    if (iVolumeStep == 0)
      iVolumeStep = 1;

    iOutputStream->SetVolume(iVolume);
    iOutputStream->SetPriority(EPriorityNormal, EMdaPriorityPreferenceTime);
    iOutputStream->SetDataTypeL(KMMFFourCCCodePCM16);

    iOutputStatus = EOpen;

    RequestSound();
  } else {
    iOutputStatus = ENotReady;
  }    
}

void AudioDriverMDA::MaoscBufferCopied(TInt aError, const TDesC8& aBuffer) {
  if (aError == KErrAbort) {
    iOutputStatus = ENotReady;
  } else if (iOutputStatus != ENotReady) {
    RequestSound();
  }
}

void AudioDriverMDA::RequestSound() {
  lock();
  audio_server_process(buffer_size, samples_in);
  unlock();

	for (size_t i = 0; i < buffer_size * 2; ++i) {
		samples_out[i] = samples_in[i] >> 16;
	}

  if (iOutputStatus == ESetVolume) {
    iOutputStatus = EOpen;
    if (iVolume < 0)
      iVolume = 0;
    if (iVolume > iOutputStream->MaxVolume())
      iVolume = iOutputStream->MaxVolume();
  
    iOutputStream->SetVolume(iVolume);
  }

  iOutputStream->WriteL(iSoundBufferPtr);
}

void AudioDriverMDA::MaoscPlayComplete(TInt aError) {
  iOutputStatus = ENotReady;
}

void AudioDriverMDA::start() {
	active = true;
}

int AudioDriverMDA::get_mix_rate() const {
	return mix_rate;
}

AudioDriverSW::OutputFormat AudioDriverMDA::get_output_format() const {
	return channels == 1 ? OUTPUT_MONO : OUTPUT_STEREO;
}

void AudioDriverMDA::lock() {}

void AudioDriverMDA::unlock() {}

void AudioDriverMDA::finish() {
	if (samples_in) {
 		memdelete_arr(samples_in);
 	}
	if (samples_out) {
 		memdelete_arr(samples_out);
 	}
}

AudioDriverMDA::AudioDriverMDA() {
	active = false;
	id = NULL;
}

AudioDriverMDA::~AudioDriverMDA() {}

