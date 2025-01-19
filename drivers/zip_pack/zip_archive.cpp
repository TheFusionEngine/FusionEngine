/*************************************************************************/
/*  file_access_zip.cpp                                                  */
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
#ifdef MINIZIP_ENABLED

#include "zip_archive.h"

#include "core/os/file_access.h"

ZipArchive* ZipArchive::instance = NULL;

void ZipArchive::close_handle(unzFile p_file) const {

	ERR_FAIL_COND(!p_file);
	FileAccess* f = (FileAccess*)unzGetOpaque(p_file);
	unzCloseCurrentFile(p_file);
	unzClose(p_file);
	memdelete(f);
};

unzFile ZipArchive::get_file_handle(String p_file) const {

	ERR_FAIL_COND_V(!has_file(p_file), NULL);
	File file = files[p_file];

	FileAccess* f = FileAccess::open(packages[file.package].filename, FileAccess::READ);
	ERR_FAIL_COND_V(!f, NULL);

	zlib_filefunc_def io;

	io.opaque = f;
	io.zopen_file = zipio_open;
	io.zread_file = zipio_read;
	io.zwrite_file = zipio_write;

	io.ztell_file = zipio_tell;
	io.zseek_file = zipio_seek;
	io.zclose_file = zipio_close;
	io.zerror_file = zipio_testerror;

	unzFile pkg = unzOpen2(packages[file.package].filename.utf8().get_data(), &io);
	ERR_FAIL_COND_V(!pkg, NULL);
	unzGoToFilePos(pkg, &file.file_pos);
	if (unzOpenCurrentFile(pkg) != UNZ_OK) {

		unzClose(pkg);
		ERR_FAIL_V(NULL);
	};

	return pkg;
};

String ZipArchive::get_pack_extension() const{
	return "zip";
}

#ifdef TOOLS_ENABLED

String ZipArchive::get_pack_name() const{
	return "Zip Archive";
}

Error ZipArchive::export_pack(FileAccess *p_destination, Vector<FileExportData> p_files, PackingProgressCallback p_progress){

	return ERR_CANT_OPEN;
}

#endif

bool ZipArchive::try_open_pack(const String& p_name, bool p_replace_files) {

	printf("opening pack %ls, %i, %i\n", p_name.c_str(), p_name.extension().nocasecmp_to("zip"), p_name.extension().nocasecmp_to("pcz"));
	if (p_name.extension().nocasecmp_to("zip") != 0 and p_name.extension().nocasecmp_to("pcz") != 0)
		return false;

	zlib_filefunc_def io;

	FileAccess* f = FileAccess::open(p_name, FileAccess::READ);
	if (!f)
		return false;
	io.opaque = f;
	io.zopen_file = zipio_open;
	io.zread_file = zipio_read;
 	io.zwrite_file = zipio_write;

	io.ztell_file = zipio_tell;
	io.zseek_file = zipio_seek;
	io.zclose_file = zipio_close;
	io.zerror_file = zipio_testerror;

	unzFile zfile = unzOpen2(p_name.utf8().get_data(), &io);
	ERR_FAIL_COND_V(!zfile, false);

	unz_global_info64 gi;
	int err = unzGetGlobalInfo64(zfile, &gi);
	ERR_FAIL_COND_V(err!=UNZ_OK, false);

	Package pkg;
	pkg.filename = p_name;
	pkg.zfile = zfile;
	packages.push_back(pkg);
	int pkg_num = packages.size()-1;

	for (unsigned int i=0;i<gi.number_entry;i++) {

		char filename_inzip[256];

		unz_file_info64 file_info;
		err = unzGetCurrentFileInfo64(zfile,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);
		ERR_CONTINUE(err != UNZ_OK);

		File f;
		f.package = pkg_num;
		unzGetFilePos(zfile, &f.file_pos);

		String fname = String("res://") + filename_inzip;
		files[fname] = f;

		//uint8_t md5[16]={0,0,0,0,0,0,0,0 , 0,0,0,0,0,0,0,0};
		//PackedData::get_singleton()->add_path(p_name, fname, 0, 0, md5, this);

		if ((i+1)<gi.number_entry) {
			unzGoToNextFile(zfile);
		};
	};

	return true;
};

PackSource::FileStatus ZipArchive::has_file(const String& p_path) const {
	return files.has(p_path) ? FileStatus::HAS_FILE : FileStatus::NOT_HAS_FILE;
}

FileAccess* ZipArchive::get_file(const String& p_path) const {
	return memnew(FileAccessZip(p_path));
};

ZipArchive* ZipArchive::get_singleton() {
	if (instance == NULL) {
		instance = memnew(ZipArchive);
	};

	return instance;
};

ZipArchive::ZipArchive() {
	instance = this;
	//fa_create_func = FileAccess::get_create_func();
};

ZipArchive::~ZipArchive() {
	for (int i=0; i<packages.size(); i++) {

		FileAccess* f = (FileAccess*)unzGetOpaque(packages[i].zfile);
		unzClose(packages[i].zfile);
		memdelete(f);
	};

	packages.clear();
};

Error FileAccessZip::_open(const String& p_path, int p_mode_flags) {
	close();

	ERR_FAIL_COND_V(p_mode_flags & FileAccess::WRITE, FAILED);
	ZipArchive* arch = ZipArchive::get_singleton();
	ERR_FAIL_COND_V(!arch, FAILED);
	zfile = arch->get_file_handle(p_path);
	ERR_FAIL_COND_V(!zfile, FAILED);

	int err = unzGetCurrentFileInfo64(zfile,&file_info,NULL,0,NULL,0,NULL,0);
	ERR_FAIL_COND_V(err != UNZ_OK, FAILED);

	return OK;
};

void FileAccessZip::close() {
	if (!zfile)
		return;

	ZipArchive* arch = ZipArchive::get_singleton();
	ERR_FAIL_COND(!arch);
	arch->close_handle(zfile);
	zfile = NULL;
};

bool FileAccessZip::is_open() const {
	return zfile != NULL;
};

void FileAccessZip::seek(size_t p_position) {
	ERR_FAIL_COND(!zfile);
	unzSeekCurrentFile(zfile, p_position);
};

void FileAccessZip::seek_end(int64_t p_position) {
	ERR_FAIL_COND(!zfile);
	unzSeekCurrentFile(zfile, get_len() + p_position);
};

size_t FileAccessZip::get_pos() const {
	ERR_FAIL_COND_V(!zfile, 0);
	return unztell(zfile);
};

size_t FileAccessZip::get_len() const {
	ERR_FAIL_COND_V(!zfile, 0);
	return file_info.uncompressed_size;
};

bool FileAccessZip::eof_reached() const {
	ERR_FAIL_COND_V(!zfile, true);

	return at_eof;
};

uint8_t FileAccessZip::get_8() const {
	uint8_t ret = 0;
	get_buffer(&ret, 1);
	return ret;
};

int FileAccessZip::get_buffer(uint8_t *p_dst,int p_length) const {
	ERR_FAIL_COND_V(!zfile, -1);
	at_eof = unzeof(zfile);
	if (at_eof)
		return 0;
	int read = unzReadCurrentFile(zfile, p_dst, p_length);
	ERR_FAIL_COND_V(read < 0, read);
	if (read < p_length)
		at_eof = true;
	return read;
};

Error FileAccessZip::get_error() const {
	if (!zfile) {

		return ERR_UNCONFIGURED;
	};
	if (eof_reached()) {
		return ERR_FILE_EOF;
	};

	return OK;
};

void FileAccessZip::store_8(uint8_t p_dest) {
	ERR_FAIL();
};

bool FileAccessZip::file_exists(const String& p_name) {
	return false;
};

FileAccessZip::FileAccessZip(const String& p_path) {
	zfile = NULL;
	_open(p_path, FileAccess::READ);
};

FileAccessZip::~FileAccessZip() {
	close();
};

#endif
