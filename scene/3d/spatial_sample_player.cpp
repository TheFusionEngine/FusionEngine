/*************************************************************************/
/*  spatial_sample_player.cpp                                            */
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
#include "spatial_sample_player.h"

#include "servers/audio_server.h"
#include "camera.h"
#include "servers/spatial_sound_server.h"
#include "scene/scene_string_names.h"

bool SamplePlayer3D::_set(const StringName& p_name, const Variant& p_value) {

	String name=p_name;

	if (name==SceneStringNames::get_singleton()->play_play) {
		if (library.is_valid()) {

			String what=p_value;
			if (what=="")
				stop_all();
			else
				play(what);

			played_back=what;
		}
		return true;

	}

	return false;
}

bool SamplePlayer3D::_get(const StringName& p_name,Variant &r_ret) const {


	String name=p_name;

	if (name==SceneStringNames::get_singleton()->play_play) {
		r_ret=played_back;
		return true;
	}

	return false;
}

void SamplePlayer3D::_get_property_list(List<PropertyInfo> *p_list) const {

	String en="";
	if (library.is_valid()) {
		List<StringName> samples;
		Ref<SampleLibrary> ncl=library;
		ncl->get_sample_list(&samples);
		for (List<StringName>::Element *E=samples.front();E;E=E->next()) {

			en+=",";
			en+=E->get();
		}
	}

	p_list->push_back( PropertyInfo( Variant::STRING, "play/play", PROPERTY_HINT_ENUM, en,PROPERTY_USAGE_EDITOR));

}
void SamplePlayer3D::_notification(int p_what) {


	switch(p_what) {

		case NOTIFICATION_ENTER_WORLD: {

			SpatialSoundServer::get_singleton()->source_set_polyphony(get_source_rid(),polyphony);


		} break;
	}

}

void SamplePlayer3D::set_sample_library(const Ref<SampleLibrary>& p_library) {

	library=p_library;
}

Ref<SampleLibrary> SamplePlayer3D::get_sample_library() const {

	return library;
}

void SamplePlayer3D::set_polyphony(int p_voice_count) {

	ERR_FAIL_COND(p_voice_count<0 || p_voice_count>64);
	polyphony=p_voice_count;
	if (get_source_rid().is_valid())
		SpatialSoundServer::get_singleton()->source_set_polyphony(get_source_rid(),polyphony);

}

int SamplePlayer3D::get_polyphony() const {

	return polyphony;
}

SamplePlayer3D::VoiceID SamplePlayer3D::play(const String& p_sample,int p_voice) {

	if (!get_source_rid().is_valid())
		return INVALID_VOICE;
	if (library.is_null())
		return INVALID_VOICE;
	if (!library->has_sample(p_sample))
		return INVALID_VOICE;
	Ref<Sample> sample = library->get_sample(p_sample);
	float vol_change = library->sample_get_volume_db(p_sample);
	float pitch_change = library->sample_get_pitch_scale(p_sample);

	VoiceID vid = SpatialSoundServer::get_singleton()->source_play_sample(get_source_rid(),sample->get_rid(),sample->get_mix_rate()*pitch_change,p_voice);
	if (vol_change)
		SpatialSoundServer::get_singleton()->source_voice_set_volume_scale_db(get_source_rid(),vid,vol_change);

	return vid;


}
//voices
void SamplePlayer3D::voice_set_pitch_scale(VoiceID p_voice, float p_pitch_scale) {

	if (!get_source_rid().is_valid())
		return;

	SpatialSoundServer::get_singleton()->source_voice_set_pitch_scale(get_source_rid(),p_voice,p_pitch_scale);

}

void SamplePlayer3D::voice_set_volume_scale_db(VoiceID p_voice, float p_volume_db) {

	if (!get_source_rid().is_valid())
		return;
	SpatialSoundServer::get_singleton()->source_voice_set_volume_scale_db(get_source_rid(),p_voice,p_volume_db);

}

bool SamplePlayer3D::is_voice_active(VoiceID p_voice) const {

	if (!get_source_rid().is_valid())
		return false;
	return SpatialSoundServer::get_singleton()->source_is_voice_active(get_source_rid(),p_voice);

}

void SamplePlayer3D::stop_voice(VoiceID p_voice) {

	if (!get_source_rid().is_valid())
		return;
	SpatialSoundServer::get_singleton()->source_stop_voice(get_source_rid(),p_voice);

}

void SamplePlayer3D::stop_all() {

	if (!get_source_rid().is_valid())
		return;

	for(int i=0;i<polyphony;i++) {

		SpatialSoundServer::get_singleton()->source_stop_voice(get_source_rid(),i);
	}
}

void SamplePlayer3D::_bind_methods() {


	ObjectTypeDB::bind_method(_MD("set_sample_library","library:SampleLibrary"),&SamplePlayer3D::set_sample_library);
	ObjectTypeDB::bind_method(_MD("get_sample_library:SampleLibrary"),&SamplePlayer3D::get_sample_library);

	ObjectTypeDB::bind_method(_MD("set_polyphony","voices"),&SamplePlayer3D::set_polyphony);
	ObjectTypeDB::bind_method(_MD("get_polyphony"),&SamplePlayer3D::get_polyphony);

	ObjectTypeDB::bind_method(_MD("play","sample","voice"),&SamplePlayer3D::play,DEFVAL(NEXT_VOICE));
	//voices,DEV
	ObjectTypeDB::bind_method(_MD("voice_set_pitch_scale","voice","ratio"),&SamplePlayer3D::voice_set_pitch_scale);
	ObjectTypeDB::bind_method(_MD("voice_set_volume_scale_db","voice","db"),&SamplePlayer3D::voice_set_volume_scale_db);

	ObjectTypeDB::bind_method(_MD("is_voice_active","voice"),&SamplePlayer3D::is_voice_active);
	ObjectTypeDB::bind_method(_MD("stop_voice","voice"),&SamplePlayer3D::stop_voice);
	ObjectTypeDB::bind_method(_MD("stop_all"),&SamplePlayer3D::stop_all);

	BIND_CONSTANT( INVALID_VOICE );
	BIND_CONSTANT( NEXT_VOICE );

	ADD_PROPERTY( PropertyInfo( Variant::INT, "config/polyphony", PROPERTY_HINT_RANGE, "1,64,1"),_SCS("set_polyphony"),_SCS("get_polyphony"));
	ADD_PROPERTY( PropertyInfo( Variant::OBJECT, "config/samples", PROPERTY_HINT_RESOURCE_TYPE,"SampleLibrary"),_SCS("set_sample_library"),_SCS("get_sample_library"));


}


SamplePlayer3D::SamplePlayer3D() {

	polyphony=1;

}

SamplePlayer3D::~SamplePlayer3D() {


}
