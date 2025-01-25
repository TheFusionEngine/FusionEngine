#ifndef PACK_SMALL_PCK_H
#define PACK_SMALL_PCK_H

#include "core/io/file_access_pack.h"
#include "core/io/md5.h"
#include "core/io/compression.h"

//This is an implementation of pck that only uses hashes for files, and compresses
//all bundled files, which reduces file size of the end archive

struct ArchiveFile {
    uint64_t compressed_size;
    uint64_t full_size;
    uint64_t pck_offset;
};

class FileAccessSmallPCK : public FileAccess {

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
};

class PackSourceSmallPCK : public PackSource {
private:

    //key is a checksum
    Map<uint32_t, ArchiveFile> files;

public:
	virtual bool try_open_pack(const String& p_path, bool p_replace_files = true);
	virtual FileAccess* get_file(const String& p_path) const;
	virtual FileStatus has_file(const String& p_path) const;

	virtual String get_pack_extension() const;
#ifdef TOOLS_ENABLED
	virtual String get_pack_name() const;
	virtual Error export_pack(FileAccess *p_destination, Vector<FileExportData> p_files, PackingProgressCallback p_progress);
#endif
};

#endif
