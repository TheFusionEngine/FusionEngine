/*************************************************************************/
/*  audio_driver_dummy.cpp                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                 */
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
#include "audio_driver_vita.h"

#include "globals.h"
#include "os/os.h"

Error AudioDriverVita::init() {

	active=false;
	thread_exited=false;
	exit_thread=false;
	pcm_open = false;
	samples_in = NULL;


	mix_rate = 44100;
	output_format = OUTPUT_STEREO;
	channels = 2;

	int latency = GLOBAL_DEF("audio/output_latency",25);
	buffer_size = nearest_power_of_2( latency * mix_rate / 1000 );

	samples_in = memnew_arr(int32_t, buffer_size*channels);
	samples_out = memnew_arr(int16_t, buffer_size*channels);

	port = sceAudioOutOpenPort(SCE_AUDIO_OUT_PORT_TYPE_BGM, buffer_size, mix_rate, SCE_AUDIO_OUT_MODE_STEREO);
	int vols[] = {SCE_AUDIO_VOLUME_0DB, SCE_AUDIO_VOLUME_0DB};
	sceAudioOutSetVolume(port, (SceAudioOutChannelFlag)(SCE_AUDIO_VOLUME_FLAG_L_CH | SCE_AUDIO_VOLUME_FLAG_R_CH), vols);
	mutex=Mutex::create();
	thread = Thread::create(AudioDriverVita::thread_func, this);

	return OK;
};

void AudioDriverVita::thread_func(void* p_udata) {

	AudioDriverVita* ad = (AudioDriverVita*)p_udata;

	uint64_t usdelay = (ad->buffer_size / float(ad->mix_rate))*1000000;


	while (!ad->exit_thread) {


		if (!ad->active) {
			for(int i = 0; i < ad->buffer_size*ad->channels; ++i) {
				ad->samples_out[i] = 0;
			}
		} else {

			ad->lock();

			ad->audio_server_process(ad->buffer_size, ad->samples_in);
			for(int i = 0; i < ad->buffer_size*ad->channels; ++i) {
				ad->samples_out[i] = ad->samples_in[i] >> 16;
			}
			
			sceAudioOutOutput(ad->port, ad->samples_out);
			ad->unlock();
			
		};

		OS::get_singleton()->delay_usec(1000);

	};

	ad->thread_exited=true;

};

void AudioDriverVita::start() {

	active = true;
};

int AudioDriverVita::get_mix_rate() const {

	return mix_rate;
};

AudioDriverSW::OutputFormat AudioDriverVita::get_output_format() const {

	return output_format;
};
void AudioDriverVita::lock() {

	if (!thread || !mutex)
		return;
	mutex->lock();
};
void AudioDriverVita::unlock() {

	if (!thread || !mutex)
		return;
	mutex->unlock();
};

void AudioDriverVita::finish() {

	if (!thread)
		return;

	exit_thread = true;
	Thread::wait_to_finish(thread);

	if (samples_in) {
		memdelete_arr(samples_in);
	};
	if (samples_out) {
		memdelete_arr(samples_out);
	};

	memdelete(thread);
	if (mutex)
		memdelete(mutex);
	thread = NULL;
	sceAudioOutOutput(port, NULL);
	sceAudioOutReleasePort(port);
};

AudioDriverVita::AudioDriverVita() {

	mutex = NULL;
	thread=NULL;

};

AudioDriverVita::~AudioDriverVita() {

};


