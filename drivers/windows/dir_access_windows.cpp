/*************************************************************************/
/*  dir_access_windows.cpp                                               */
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
#if defined(WINDOWS_ENABLED)

#include "dir_access_windows.h"

#include "os/memory.h"

#include <windows.h>
#include <tchar.h>
#include <direct.h>
#include <stdio.h>
#include "print_string.h"

#ifdef WINRT_ENABLED
#include <Synchapi.h>
#include <collection.h>
#include <ppltasks.h>
#endif

/*

[03:57] <reduz> yessopie, so i dont havemak to rely on unicows
[03:58] <yessopie> reduz- yeah, all of the functions fail, and then you can call GetLastError () which will return 120
[03:58] <drumstick> CategoryApl, hehe, what? :)
[03:59] <CategoryApl> didn't Verona lead to some trouble
[03:59] <yessopie> 120 = ERROR_CALL_NOT_IMPLEMENTED
[03:59] <yessopie> (you can use that constant if you include winerr.h)
[03:59] <CategoryApl> well answer with winning a compo

[04:02] <yessopie> if ( SetCurrentDirectoryW ( L"." ) == FALSE && GetLastError () == ERROR_CALL_NOT_IMPLEMENTED ) { use ANSI }
*/

struct DirAccessWindowsPrivate {

	HANDLE h; //handle for findfirstfile
	WIN32_FIND_DATA f;
};

// CreateFolderAsync

bool DirAccessWindows::list_dir_begin() {

	_cisdir=false;
	
	list_dir_end();
	String f = current_dir.replace("/", "\\")+"\\*";
	p->h = FindFirstFile(f.t_str(), &p->f);

	return (p->h==INVALID_HANDLE_VALUE);
}


String DirAccessWindows::get_next() {

	if (p->h==INVALID_HANDLE_VALUE)
		return "";

#ifndef WINRT_ENABLED
	_cisdir=(p->f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	String name=p->f.cFileName;

	if (FindNextFile(p->h, &p->f) == 0) {

		FindClose(p->h);
		p->h=INVALID_HANDLE_VALUE;
	}

	return name;
#else
	return ""
#endif
}

bool DirAccessWindows::current_is_dir() const {

	return _cisdir;
}

void DirAccessWindows::list_dir_end() {

	if (p->h!=INVALID_HANDLE_VALUE) {

		FindClose(p->h);
		p->h=INVALID_HANDLE_VALUE;
	}

}
int DirAccessWindows::get_drive_count() {

	return drive_count;

}
String DirAccessWindows::get_drive(int p_drive) {

	if (p_drive<0 || p_drive>=drive_count)
		return "";

	return String::chr(drives[p_drive])+":";
}

Error DirAccessWindows::change_dir(String p_dir) {

	GLOBAL_LOCK_FUNCTION

#ifdef WINRT_ENABLED

	p_dir = fix_path(p_dir);
	current_dir = normalize_path(p_dir);

	return OK;
#else


	p_dir=fix_path(p_dir);
	String tdir = p_dir.replace("/", "\\");

	TCHAR real_current_dir_name[2048];
	GetCurrentDirectory(2048,real_current_dir_name);
	String prev_dir=real_current_dir_name;

	String tcurdir = current_dir.replace("/", "\\");
	SetCurrentDirectory(tcurdir.t_str());
	bool worked=(SetCurrentDirectory(tdir.t_str())!=0);

	String base = _get_root_path();
	if (base!="") {

		GetCurrentDirectory(2048,real_current_dir_name);
		String new_dir;
		new_dir = String(real_current_dir_name).replace("\\", "/");
		if (!new_dir.to_lower().begins_with(base.replace("\\", "/").to_lower())) {
			puts("BAD BASE!");
			worked=false;
		}
	}

	if (worked) {

		GetCurrentDirectory(2048,real_current_dir_name);
		current_dir=String(real_current_dir_name); // TODO, utf8 parser
		current_dir=current_dir.replace("\\", "/");

	}

	SetCurrentDirectory(prev_dir.t_str());

	return worked?OK:ERR_INVALID_PARAMETER;
#endif
}

Error DirAccessWindows::make_dir(String p_dir) {

	GLOBAL_LOCK_FUNCTION

#ifdef WINRT_ENABLED

	return ERR_CANT_CREATE;

#else

	p_dir=fix_path(p_dir);
	
	p_dir=p_dir.replace("/","\\");

	bool success;
	int err;

	TCHAR real_current_dir_name[2048];
	GetCurrentDirectory(2048,real_current_dir_name);

	String tcurdir = current_dir.replace("/", "\\");
	SetCurrentDirectory(tcurdir.t_str());

	success=CreateDirectory(p_dir.t_str(), NULL);
	err = GetLastError();

	SetCurrentDirectory(real_current_dir_name);

	if (success) {
		return OK;
	};

	if (err == ERROR_ALREADY_EXISTS) {
		return ERR_ALREADY_EXISTS;
	};

	return ERR_CANT_CREATE;

#endif
}


String DirAccessWindows::get_current_dir() {

	String base = _get_root_path();
	if (base!="") {


		String bd = current_dir.replace("\\","/").replace_first(base,"");
		if (bd.begins_with("/"))
			return _get_root_string()+bd.substr(1,bd.length());
		else
			return _get_root_string()+bd;

	} else {

	}

	return current_dir;
}

bool DirAccessWindows::file_exists(String p_file) {

	GLOBAL_LOCK_FUNCTION

	if (!p_file.is_abs_path())
		p_file=get_current_dir()+"/"+p_file;

	p_file=fix_path(p_file).replace("/","\\");

	WIN32_FILE_ATTRIBUTE_DATA    fileInfo;

	DWORD fileAttr;

	fileAttr = GetFileAttributes(p_file.t_str());
	if (0 == fileAttr)
		return false;

	return !(fileAttr&FILE_ATTRIBUTE_DIRECTORY);
}

bool DirAccessWindows::dir_exists(String p_dir) {

	GLOBAL_LOCK_FUNCTION

	if (!p_dir.is_abs_path())
		p_dir=get_current_dir()+"/"+p_dir;

	p_dir=fix_path(p_dir).replace("/","\\");

	WIN32_FILE_ATTRIBUTE_DATA    fileInfo;

#ifndef WINRT_ENABLED
	DWORD fileAttr;

	fileAttr = GetFileAttributes(p_dir.t_str());
	if (0 == fileAttr)
		return false;

	return (fileAttr&FILE_ATTRIBUTE_DIRECTORY);
#else
	return false;
#endif
}

Error DirAccessWindows::rename(String p_path,String p_new_path) {

	p_path=fix_path(p_path).replace("/", "\\");
	p_new_path=fix_path(p_new_path).replace("/", "\\");
	
	if (file_exists(p_new_path)) {
		if (remove(p_new_path) != OK) {
			return FAILED;
		};
	};

	return ::rename(p_path.ascii().get_data(),p_new_path.ascii().get_data())==0?OK:FAILED;
}

Error DirAccessWindows::remove(String p_path)  {

	p_path=fix_path(p_path).replace("/", "\\");
	
	WIN32_FILE_ATTRIBUTE_DATA    fileInfo;
	DWORD fileAttr = GetFileAttributes(p_path.t_str());
	if (fileAttr == INVALID_FILE_ATTRIBUTES)
		return FAILED;

	if (fileAttr & FILE_ATTRIBUTE_DIRECTORY)
		return ::rmdir(p_path.ascii().get_data())==0?OK:FAILED;
	else
		return ::unlink(p_path.ascii().get_data())==0?OK:FAILED;
}
/*

FileType DirAccessWindows::get_file_type(const String& p_file) const {


	wchar_t real_current_dir_name[2048];
	GetCurrentDirectoryW(2048,real_current_dir_name);
	String prev_dir=real_current_dir_name;

	bool worked SetCurrentDirectoryW(current_dir.c_str());

	DWORD attr;
	if (worked) {

		WIN32_FILE_ATTRIBUTE_DATA    fileInfo;
		attr = GetFileAttributesExW(p_file.c_str(), GetFileExInfoStandard, &fileInfo);

	}

	SetCurrentDirectoryW(prev_dir.c_str());

	if (!worked)
		return FILE_TYPE_NONE;


	return (attr&FILE_ATTRIBUTE_DIRECTORY)?FILE_TYPE_
}
*/
size_t  DirAccessWindows::get_space_left() {

	return -1;
};

DirAccessWindows::DirAccessWindows() {

	p = memnew( DirAccessWindowsPrivate );
	p->h=INVALID_HANDLE_VALUE;
	current_dir=".";

	drive_count=0;

#ifdef WINRT_ENABLED
	Windows::Storage::StorageFolder ^install_folder = Windows::ApplicationModel::Package::Current->InstalledLocation;
	change_dir(install_folder->Path->Data());

#else


	DWORD mask=GetLogicalDrives();

	for (int i=0;i<MAX_DRIVES;i++) {

		if (mask&(1<<i)) { //DRIVE EXISTS

			drives[drive_count]='a'+i;
			drive_count++;
		}
	}

	change_dir(".");
#endif
}


DirAccessWindows::~DirAccessWindows() {

	memdelete( p );
}

#endif //windows DirAccess support
