#ifndef FILE_ACCESS_PCK_H
#define FILE_ACCESS_PCK_H

#include "core/io/file_access_pack.h"

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

class FileAccessPCK : public FileAccess {

	PackedFile pf;

	mutable size_t pos;
	mutable bool eof;

	FileAccess *f;
	virtual Error _open(const String& p_path, int p_mode_flags);
	virtual uint64_t _get_modified_time(const String& p_file) { return 0; }

public:

	virtual void close();
	virtual bool is_open() const;

	virtual void seek(size_t p_position);
	virtual void seek_end(int64_t p_position=0);
	virtual size_t get_pos() const;
	virtual size_t get_len() const;

	virtual bool eof_reached() const;

	virtual uint8_t get_8() const;

	virtual int get_buffer(uint8_t *p_dst,int p_length) const;

	virtual void set_endian_swap(bool p_swap);

	virtual Error get_error() const;

	virtual void store_8(uint8_t p_dest);

	virtual void store_buffer(const uint8_t *p_src,int p_length);

	virtual bool file_exists(const String& p_name);


	FileAccessPCK(const String& p_path, const PackedFile& p_file);
	~FileAccessPCK();
};

class DirAccessPCK : public DirAccess {
	PackedDir *current;

	List<String> list_dirs;
	List<String> list_files;
	bool cdir;

public:

	virtual bool list_dir_begin();
	virtual String get_next();
	virtual bool current_is_dir() const;
	virtual void list_dir_end();

	virtual int get_drive_count();
	virtual String get_drive(int p_drive);

	virtual Error change_dir(String p_dir);
	virtual String get_current_dir();


	virtual bool file_exists(String p_file);
	virtual bool dir_exists(String p_dir);

	virtual Error make_dir(String p_dir);

	virtual Error rename(String p_from, String p_to);
	virtual Error remove(String p_name);

	size_t get_space_left();

	DirAccessPCK();
	~DirAccessPCK();
};

class PackedSourcePCK : public PackSource {
private:
	static PackedSourcePCK *singleton;

	Map<PathMD5, PackedFile> files;

public:
	PackedDir *root;

	static PackedSourcePCK *get_singleton();

	virtual bool try_open_pack(const String& p_path, bool p_replace_files = true);
	virtual FileAccess* get_file(const String& p_path);
	virtual FileStatus has_file(const String& p_path);

	PackedSourcePCK();

};

#endif
