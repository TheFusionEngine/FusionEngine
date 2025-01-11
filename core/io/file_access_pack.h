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

class PackSource;

class PackedData {
friend class FileAccessPack;
friend class DirAccessPack;
friend class PackSource;

public:
	enum FileStatus {
		NOT_HAS_FILE = 0,
		HAS_FILE = 1,
		REMOVED_FILE = 2
	};

	struct PackedFile {

		String pack;
		uint64_t offset; //if offset is ZERO, the file was ERASED
		uint64_t size;
		uint8_t md5[16];
		PackSource* src;
	};

	struct PackedDir {
		PackedDir *parent;
		String name;
		Map<String,PackedDir*> subdirs;
		Set<String> files;
	};

	struct PathMD5 {
		uint64_t a;
		uint64_t b;
		bool operator < (const PathMD5& p_md5) const {

			if (p_md5.a == a) {
				return b < p_md5.b;
			} else {
				return a < p_md5.a;
			}
		}

		bool operator == (const PathMD5& p_md5) const {
			return a == p_md5.a && b == p_md5.b;
		};

		PathMD5() {
			a = b = 0;
		};

		PathMD5(const Vector<uint8_t> p_buf) {
			a = *((uint64_t*)&p_buf[0]);
			b = *((uint64_t*)&p_buf[8]);
		};
	};

private:
	Map<PathMD5,PackedFile> files;

	//We need to rearrange the order of these pointers so that the most recently loaded source takes prescedence
	Vector<PackSource*> sources;

	PackedDir *root;
	//Map<String,PackedDir*> dirs;

	static PackedData *singleton;
	uint16_t packs_loaded;
	bool disabled;

public:

	void add_pack_source(PackSource* p_source);
	void add_path(const String& pkg_path, const String& path, uint64_t ofs, uint64_t size,const uint8_t* p_md5, PackSource* p_src); // for PackSource

	void set_disabled(bool p_disabled) { disabled=p_disabled; }
	_FORCE_INLINE_ bool is_disabled() const { return disabled; }

	static PackedData *get_singleton() { return singleton; }
	Error add_pack(const String& p_path);
	Error add_pack(const String& p_path, bool p_replace_files);

	_FORCE_INLINE_ FileAccess *try_open_path(const String& p_path);
	_FORCE_INLINE_ bool has_path(const String& p_path);

	PackedData();
};

class PackSource {
public:
	uint16_t load_presedence;

	virtual bool try_open_pack(const String& p_path, bool p_replace_files = true)=0;
	virtual FileAccess* get_file(const String& p_path, PackedData::PackedFile* p_file)=0;
	virtual FileAccess* get_file(const String& p_path)=0;
	virtual PackedData::FileStatus has_file(const String& p_path)=0;
};

#endif // FILE_ACCESS_PACK_H
