#include "small_pck.h"

void FileAccessSmallPCK::close(){

};

bool FileAccessSmallPCK::is_open() const{
    return false;
};

void FileAccessSmallPCK::seek(size_t p_position){

};

void FileAccessSmallPCK::seek_end(int64_t p_position){

};

size_t FileAccessSmallPCK::get_pos() const {
    return 0;
};
size_t FileAccessSmallPCK::get_len() const {
    return 0;
};

bool FileAccessSmallPCK::eof_reached() const {
    return true;
};

uint8_t FileAccessSmallPCK::get_8() const{
    return 0;
};

int FileAccessSmallPCK::get_buffer(uint8_t *p_dst,int p_length) const {
    return 0;
};

void FileAccessSmallPCK::set_endian_swap(bool p_swap){

};

Error FileAccessSmallPCK::get_error() const {
    return ERR_CANT_OPEN;
};

void FileAccessSmallPCK::store_8(uint8_t p_dest){

};

void FileAccessSmallPCK::store_buffer(const uint8_t *p_src,int p_length){

};

bool FileAccessSmallPCK::file_exists(const String& p_name){
    return false;
};

bool PackSourceSmallPCK::try_open_pack(const String& p_path, bool p_replace_files){
    return false;
}

FileAccess* PackSourceSmallPCK::get_file(const String& p_path) const {
    return NULL;
};

PackSource::FileStatus PackSourceSmallPCK::has_file(const String& p_path) const{
    return NOT_HAS_FILE;
};

String PackSourceSmallPCK::get_pack_extension() const{
        return "spck";
};

#ifdef TOOLS_ENABLED
String PackSourceSmallPCK::get_pack_name() const{
    return "Small PCK";
};

Error PackSourceSmallPCK::export_pack(FileAccess *p_destination, Vector<FileExportData> p_files, PackingProgressCallback p_progress){
    return ERR_METHOD_NOT_FOUND;
};
#endif
