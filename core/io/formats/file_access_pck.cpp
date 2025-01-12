#include "file_access_pck.h"

#include "version.h"

#define PACK_VERSION 0

#define PCK_MAGIC 0x43504447

PackedSourcePCK *PackedSourcePCK::get_singleton(){
	if (singleton == NULL) {
		singleton = memnew(PackedSourcePCK);
	};
    return singleton;
}

bool PackedSourcePCK::try_open_pack(const String& p_path, bool p_replace_files) {

	FileAccess *f = FileAccess::open(p_path,FileAccess::READ);
	if (!f)
		return false;

	//printf("try open %ls!\n", p_path.c_str());

	uint32_t magic= f->get_32();

	if (magic != PCK_MAGIC) {
		//maybe at the end.... self contained exe
		f->seek_end();
		f->seek( f->get_pos() -4 );
		magic = f->get_32();

		if (magic != PCK_MAGIC) {
			memdelete(f);
			return false;
		}
		f->seek( f->get_pos() -12 );

		uint64_t ds = f->get_64();
		f->seek( f->get_pos() -ds-8 );

		magic = f->get_32();
		if (magic != PCK_MAGIC) {

			memdelete(f);
			return false;
		}
	}

	uint32_t version = f->get_32();
	uint32_t ver_major = f->get_32();
	uint32_t ver_minor = f->get_32();
	uint32_t ver_rev = f->get_32();

	ERR_EXPLAIN("Pack version newer than supported by engine: "+itos(version));
	ERR_FAIL_COND_V( version > PACK_VERSION, ERR_INVALID_DATA);
	ERR_EXPLAIN("Pack created with a newer version of the engine: "+itos(ver_major)+"."+itos(ver_minor)+"."+itos(ver_rev));
	ERR_FAIL_COND_V( ver_major > VERSION_MAJOR || (ver_major == VERSION_MAJOR && ver_minor > VERSION_MINOR), ERR_INVALID_DATA);

	for(int i = 0; i < 16; i++) {
		//reserved
		f->get_32();
	}

	int file_count = f->get_32();

	for(int i=0; i < file_count; i++) {

		uint32_t sl = f->get_32();
		CharString cs;
		cs.resize(sl+1);
		f->get_buffer((uint8_t*)cs.ptr(),sl);
		cs[sl]=0;

        PackedFile pf;

		String path;
		path.parse_utf8(cs.ptr());
        pf.pack=path;

		pf.offset = f->get_64();
		pf.size = f->get_64();
        f->get_buffer(pf.md5,16);

		//PackedData::get_singleton()->add_path(p_path, path, ofs, size, md5,this);

        PathMD5 pmd5(path.md5_buffer());

        bool exists = files.has(pmd5);

        files[pmd5]=pf;

        if (!exists) {
            //search for dir
            String p = path.replace_first("res://","");
            PackedDir *cd=root;

            if (p.find("/")!=-1) { //in a subdir

                Vector<String> ds=p.get_base_dir().split("/");

                for(int j=0;j<ds.size();j++) {

                    if (!cd->subdirs.has(ds[j])) {

                        PackedDir *pd = memnew( PackedDir );
                        pd->name=ds[j];
                        pd->parent=cd;
                        cd->subdirs[pd->name]=pd;
                        cd=pd;
                    } else {
                        cd=cd->subdirs[ds[j]];
                    }
                }
            }
            cd->files.insert(path.get_file());
        }
        pf.src = this;
    };

	return true;
};

// void PackedData::add_path(const String& pkg_path, const String& path, uint64_t ofs, uint64_t size,const uint8_t* p_md5, PackSource* p_src) {
//
// 	PathMD5 pmd5(path.md5_buffer());
// 	//printf("adding path %ls, %lli, %lli\n", path.c_str(), pmd5.a, pmd5.b);
//
//
//
// 	PackedFile pf;
// 	pf.pack=pkg_path;
// 	pf.offset=ofs;
// 	pf.size=size;
// 	for(int i=0;i<16;i++)
// 		pf.md5[i]=p_md5[i];
// 	pf.src = p_src;
//
// 	files[pmd5]=pf;
//
// 	if (!exists) {
// 		//search for dir
// 		String p = path.replace_first("res://","");
// 		PackedDir *cd=root;
//
// 		if (p.find("/")!=-1) { //in a subdir
//
// 			Vector<String> ds=p.get_base_dir().split("/");
//
// 			for(int j=0;j<ds.size();j++) {
//
// 				if (!cd->subdirs.has(ds[j])) {
//
// 					PackedDir *pd = memnew( PackedDir );
// 					pd->name=ds[j];
// 					pd->parent=cd;
// 					cd->subdirs[pd->name]=pd;
// 					cd=pd;
// 				} else {
// 					cd=cd->subdirs[ds[j]];
// 				}
// 			}
// 		}
// 		cd->files.insert(path.get_file());
// 	}
// }

FileAccess* PackedSourcePCK::get_file(const String &p_path) {
    PackedFile* p_file = &files.find(PathMD5(p_path.md5_buffer()))->value();

    if (!p_file){
        return NULL;
    }

	return memnew( FileAccessPCK(p_path, *p_file));
};

PackedSourcePCK::PackedSourcePCK(){
	root=memnew(PackedDir);
	root->parent=NULL;

    if (not singleton){
        singleton = this;
    }
}

///////////////////////////////////////////////////////////////////////

Error FileAccessPCK::_open(const String& p_path, int p_mode_flags) {
	ERR_FAIL_V(ERR_UNAVAILABLE);
	return ERR_UNAVAILABLE;
}

void FileAccessPCK::close() {
	f->close();
}

bool FileAccessPCK::is_open() const{
	return f->is_open();
}

void FileAccessPCK::seek(size_t p_position){
	if (p_position>pf.size) {
		eof=true;
	} else {
		eof=false;
	}

	f->seek(pf.offset+p_position);
	pos=p_position;
}

void FileAccessPCK::seek_end(int64_t p_position){
	seek(pf.size+p_position);
}

size_t FileAccessPCK::get_pos() const {
	return pos;
}

size_t FileAccessPCK::get_len() const{
	return pf.size;
}

bool FileAccessPCK::eof_reached() const{
	return eof;
}

uint8_t FileAccessPCK::get_8() const {
	if (pos>=pf.size) {
		eof=true;
		return 0;
	}

	pos++;
	return f->get_8();
}

int FileAccessPCK::get_buffer(uint8_t *p_dst,int p_length) const {
	if (eof)
		return 0;

	int64_t to_read=p_length;
	if (to_read+pos > pf.size) {
		eof=true;
		to_read=int64_t(pf.size)-int64_t(pos);
	}

	pos+=p_length;

	if (to_read<=0)
		return 0;
	f->get_buffer(p_dst,to_read);

	return to_read;
}

void FileAccessPCK::set_endian_swap(bool p_swap) {
	FileAccess::set_endian_swap(p_swap);
	f->set_endian_swap(p_swap);
}

Error FileAccessPCK::get_error() const {
	if (eof)
		return ERR_FILE_EOF;
	return OK;
}

void FileAccessPCK::store_8(uint8_t p_dest) {
	ERR_FAIL();
}

void FileAccessPCK::store_buffer(const uint8_t *p_src,int p_length) {
	ERR_FAIL();
}

bool FileAccessPCK::file_exists(const String& p_name) {
	return false;
}

FileAccessPCK::FileAccessPCK(const String& p_path, const PackedFile& p_file) {
	pf=p_file;
	f=FileAccess::open(pf.pack, FileAccess::READ);
	if (!f) {
		ERR_EXPLAIN("Can't open pack-referenced file: "+String(pf.pack));
		ERR_FAIL_COND(!f);
	}
	f->seek(pf.offset);
	pos=0;
	eof=false;
}

FileAccessPCK::~FileAccessPCK() {
	if (f)
		memdelete(f);
}

//////////////////////////////////////////////////////////////////////////////////
// DIR ACCESS
//////////////////////////////////////////////////////////////////////////////////

bool DirAccessPCK::list_dir_begin() {
	list_dirs.clear();
	list_files.clear();

	for (Map<String,PackedDir*>::Element *E=current->subdirs.front(); E; E=E->next()) {
		list_dirs.push_back(E->key());
	}

	for (Set<String>::Element *E=current->files.front();E;E=E->next()) {
		list_files.push_back(E->get());
	}

	return true;
}

String DirAccessPCK::get_next(){

	if (list_dirs.size()) {
		cdir=true;
		String d = list_dirs.front()->get();
		list_dirs.pop_front();
		return d;
	} else if (list_files.size()) {
		cdir=false;
		String f = list_files.front()->get();
		list_files.pop_front();
		return f;
	} else {
		return String();
	}
}

bool DirAccessPCK::current_is_dir() const{
	return cdir;
}

void DirAccessPCK::list_dir_end() {
	list_dirs.clear();
	list_files.clear();
}

int DirAccessPCK::get_drive_count() {
	return 0;
}

String DirAccessPCK::get_drive(int p_drive) {
	return "";
}

Error DirAccessPCK::change_dir(String p_dir) {
	String nd = p_dir.replace("\\","/");
	bool absolute=false;
	if (nd.begins_with("res://")) {
		nd=nd.replace_first("res://","");
		absolute=true;
	}

	nd=nd.simplify_path();

	if (nd.begins_with("/")) {
		nd=nd.replace_first("/","")	;
		absolute=true;
	}

	Vector<String> paths = nd.split("/");

	PackedDir *pd;

	if (absolute)
		pd = PackedSourcePCK::get_singleton()->root;
	else
		pd = current;

	for(int i=0;i<paths.size();i++) {

		String p = paths[i];
		if (p==".") {
			continue;
		} else if (p=="..") {
			if (pd->parent) {
				pd=pd->parent;
			}
		} else if (pd->subdirs.has(p)) {

			pd=pd->subdirs[p];

		} else {

			return ERR_INVALID_PARAMETER;
		}
	}

	current=pd;

	return OK;
}

String DirAccessPCK::get_current_dir() {
	String p;
	PackedDir *pd = current;
	while(pd->parent) {

		if (pd!=current)
			p="/"+p;
		p=p+pd->name;
	}

	return "res://"+p;

}

bool DirAccessPCK::file_exists(String p_file){
	return current->files.has(p_file);
}

bool DirAccessPCK::dir_exists(String p_dir) {
	return current->subdirs.has(p_dir);
}

Error DirAccessPCK::make_dir(String p_dir){
	return ERR_UNAVAILABLE;
}

Error DirAccessPCK::rename(String p_from, String p_to){
	return ERR_UNAVAILABLE;
}

Error DirAccessPCK::remove(String p_name){
	return ERR_UNAVAILABLE;
}

size_t DirAccessPCK::get_space_left(){
	return 0;
}

DirAccessPCK::DirAccessPCK() {
	current=PackedSourcePCK::get_singleton()->root;
	cdir=false;
}

DirAccessPCK::~DirAccessPCK() {
}
