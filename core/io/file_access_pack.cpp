/*************************************************************************/
/*  file_access_pack.cpp                                                 */
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
#include "file_access_pack.h"

#include <stdio.h>

PackedData *PackedData::singleton = NULL;

Error PackedData::add_pack(const String& p_path, bool p_replace_files) {
#ifdef SINGLE_PACK_SOURCE_ENABLED
	if (source->try_open_pack(p_path, p_replace_files)){
		packs_loaded++;
		source->load_presedence = packs_loaded;
		return OK;
	}
	return ERR_FILE_UNRECOGNIZED;
#else
	for (int i=0; i<sources.size(); i++) {
		if (sources[i]->try_open_pack(p_path, p_replace_files)) {
			packs_loaded++;
			sources[i]->load_presedence = packs_loaded;

			return OK;
		};
	};
	return ERR_FILE_UNRECOGNIZED;
#endif
};

void PackedData::add_pack_source(PackSource *p_source) {
#ifdef SINGLE_PACK_SOURCE_ENABLED
	source = p_source;
#else
	sources.push_back(p_source);
#endif
};

PackSource *PackedData::get_source(int index){
#ifdef SINGLE_PACK_SOURCE_ENABLED
	return source;
#else
	return sources.get(index);
#endif
}

int PackedData::get_source_count(){
#ifdef SINGLE_PACK_SOURCE_ENABLED
	return 1;
#else
	return sources.size();
#endif
}

PackedData::PackedData() {
	singleton=this;
	disabled=false;
}

FileAccess *PackedData::try_open_path(const String& p_path) {
#ifdef SINGLE_PACK_SOURCE_ENABLED
	return source->get_file(p_path);
#else
	//This ensures that it only fetches the most recent file.
	PackSource *ret_source = NULL;
	uint16_t load_presedence = 0;

	for (int i = 0; i < sources.size(); i++) {
		PackSource::FileStatus status = sources[i]->has_file(p_path);
		if (status == PackSource::FileStatus::NOT_HAS_FILE){
			continue;
		} else {
			if (status == PackSource::FileStatus::HAS_FILE){
				if (sources[i]->load_presedence > load_presedence) {ret_source = sources[i];}
			} else { //file removed
				//The file could be re-introduced later, but if it's not, ret_source will be null
				ret_source = NULL;
			}
			load_presedence = sources[i]->load_presedence;
		}
	};

	return ret_source not_eq NULL ? ret_source->get_file(p_path) : NULL;
#endif
}

bool PackedData::has_path(const String& p_path) {
#ifdef SINGLE_PACK_SOURCE_ENABLED
	return source != NULL ? source->has_file(p_path) : false;
#else
	for (int i = 0; i < sources.size(); i++) {
		if (sources[i]->has_file(p_path)) {
			return true;
		};
	};
	return false;
#endif
}

