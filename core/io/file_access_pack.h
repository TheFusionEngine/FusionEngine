/*************************************************************************/
/*  file_access_pack.h                                                   */
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
#ifndef FILE_ACCESS_PACK_H
#define FILE_ACCESS_PACK_H

#include "os/file_access.h"
#include "os/dir_access.h"
#include "map.h"
#include "list.h"
#include "print_string.h"

#ifdef TOOLS_ENABLED
typedef void (*PackingProgressCallback)(const String& p_state, int p_step);

struct FileExportData {
	String export_path;
	String internal_path;

	explicit FileExportData(String p_export, String p_internal){
		export_path = p_export;
		internal_path = p_internal;
	}
	explicit FileExportData(){
		export_path = "";
		internal_path = "";
	}
};
#endif


class PackSource {
public:
	enum FileStatus {
		NOT_HAS_FILE = 0,
		HAS_FILE = 1,
		REMOVED_FILE = 2
	};

	uint16_t load_presedence;

	virtual bool try_open_pack(const String& p_path, bool p_replace_files = true)=0;
	virtual FileAccess* get_file(const String& p_path) const=0;
	virtual FileStatus has_file(const String& p_path) const=0;

	virtual String get_pack_extension() const=0;
#ifdef TOOLS_ENABLED
	virtual String get_pack_name() const=0;
	virtual Error export_pack(FileAccess *p_destination, Vector<FileExportData> p_files, PackingProgressCallback p_progress)=0;
#endif
};


class PackedData {
	friend class FileAccessPack;
	friend class DirAccessPack;
	friend class PackSource;
private:
	static PackedData *singleton;

#ifdef SINGLE_PACK_SOURCE_ENABLED
	PackSource *source;
#else
	Vector<PackSource*> sources;
#endif
	uint16_t packs_loaded;
	bool disabled;

public:
	void add_pack_source(PackSource* p_source);

	PackSource *get_source(int index);
	int get_source_count();

	void set_disabled(bool p_disabled) { disabled=p_disabled; }
	_FORCE_INLINE_ bool is_disabled() const { return disabled; }

	static PackedData *get_singleton() { return singleton; }
	Error add_pack(const String& p_path, bool p_replace_files);

	//_FORCE_INLINE_ FileAccess *try_open_path(const String& p_path);
	FileAccess *try_open_path(const String& p_path);
	//_FORCE_INLINE_ bool has_path(const String& p_path);
	bool has_path(const String& p_path);

	PackedData();
};

#endif // FILE_ACCESS_PACK_H
