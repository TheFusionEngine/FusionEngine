#include "export.h"
#include "platform/psp/logo.h"
#include "tools/editor/editor_import_export.h"
#include "tools/editor/editor_settings.h"
#include "scene/resources/texture.h"
#include "core/os/os.h"
#include "core/os/file_access.h"
#include "core/os/dir_access.h"
#include "core/globals.h"


static const char *sdk_path_var = "psp/base_sdk_path";
static const char *pack_pbp_path = "/bin/pack-pbp";
static const char *mksfo_path = "/bin/mksfoex";

class EditorExportPlatformPSP : public EditorExportPlatformConsole {
    OBJ_TYPE(EditorExportPlatformPSP, EditorExportPlatformConsole);
private:
	bool embed_pck;
	String background_path;
	String icon_path;
	String sound_path;

	mutable bool background_valid;
	mutable bool icon_valid;
	mutable bool sound_valid;

public:

	void _get_property_list( List<PropertyInfo> *p_list) const;
	bool _set(const StringName& p_name, const Variant& p_value);
	bool _get(const StringName& p_name,Variant &r_ret) const;

	virtual ImageCompression get_image_compression() const;

	virtual bool poll_devices() { return false; }
	virtual int get_device_count() const { return 0; }
	virtual String get_device_name(int p_device) const { return ""; }
	virtual String get_device_info(int p_device) const { return ""; }
	virtual Error run(int p_device,bool p_dumb=false) { return OK; }

	virtual bool can_export(String *r_error=NULL) const;

	virtual bool requires_password(bool p_debug) const { return false; }
	virtual Error export_project(const String& p_path,bool p_debug,bool p_dumb=false);
};

void EditorExportPlatformPSP::_get_property_list( List<PropertyInfo> *p_list) const{
	p_list->push_back(PropertyInfo(Variant::BOOL, "data/embed_pck"));
	p_list->push_back(PropertyInfo(Variant::STRING, "launcher/background", PROPERTY_HINT_FILE, "png"));
	p_list->push_back(PropertyInfo(Variant::STRING, "launcher/icon", PROPERTY_HINT_FILE, "png"));
	p_list->push_back(PropertyInfo(Variant::STRING, "launcher/sound", PROPERTY_HINT_FILE, "at3"));
}

bool EditorExportPlatformPSP::_set(const StringName& p_name, const Variant& p_value){
	if (p_name == "data/embed_pck"){
		embed_pck = p_value;
	} else if (p_name == "launcher/background"){
		background_path = p_value;
	} else if (p_name == "launcher/icon"){
		icon_path = p_value;
	} else if (p_name == "launcher/sound"){
		sound_path = p_value;
	} else {
		return false;
	}

	return true;
}

bool EditorExportPlatformPSP::_get(const StringName& p_name,Variant &r_ret) const{
	if (p_name == "data/embed_pck"){
		r_ret = embed_pck;
	} else if (p_name == "launcher/background"){
		r_ret = background_path;
	} else if (p_name == "launcher/icon"){
		r_ret = icon_path;
	} else if (p_name == "launcher/sound"){
		r_ret = sound_path;
	} else {
		return false;
	}

	return true;
}

EditorExportPlatform::ImageCompression EditorExportPlatformPSP::get_image_compression() const{
    return IMAGE_COMPRESSION_NONE;
}

bool EditorExportPlatformPSP::can_export(String *r_error) const {
	String err = "";

	bool valid = true;

	String sdk_path = EditorSettings::get_singleton()->get(sdk_path_var);

	Error dir_err;
	DirAccess::open(sdk_path, &dir_err);
	if (dir_err != OK){
		valid = false;
		err += "Can't open the PSPSDK path, please set this in Editor Settings.\n";
	} else {
		if (not FileAccess::exists(sdk_path + mksfo_path)){
			valid = false;
			err += "Can't find mksfoex in the PSPSDK.\n";
		}
		if (not FileAccess::exists(sdk_path + pack_pbp_path)){
			valid = false;
			err += "Can't find pack-pbp in the PSPSDK.\n";
		}
	}

	String exe_path = EditorSettings::get_singleton()->get_settings_path()+"/templates/";

	bool debug_found = FileAccess::exists(exe_path + "psp_debug.32");
	bool release_found = FileAccess::exists(exe_path + "psp_release.32");

	if (not debug_found or not release_found) {
		valid=false;
		err +="No export templates found.\nDownload and install export templates.\n";
	} else if (not debug_found){
		err += "No debug export template found.\n";
	} else if(not release_found){
		err += "No release export template found.\n";
	}

	background_valid = false;
	if (background_path != ""){
		if (FileAccess::exists(Globals::get_singleton()->globalize_path(background_path))){
			Image background;
			background.load(background_path);

			background_valid = (background.get_height() == 272 and background.get_width() == 480);

			if (not background_valid){
				valid = false;
				err += "The size of the background is wrong! It should be 480x272.\n";
			}
		} else {
			err += "The background file cannot be found!\n";
		}
	}

	icon_valid = false;
	if (icon_path != ""){
		if (FileAccess::exists(Globals::get_singleton()->globalize_path(icon_path))){
			Image icon;
			icon.load(icon_path);

			icon_valid = (icon.get_height() == 82 and icon.get_width() == 144);

			if (not icon_valid){
				valid = false;
				err += "The size of the icon is wrong! It should be 144x82.\n";
			}
		} else {
			err += "The icon file cannot be found!\n";
		}
	}

	sound_valid = false;
	if (sound_path != ""){
		if (FileAccess::exists(Globals::get_singleton()->globalize_path(sound_path))){
			//Idk man lgtm until I can actually test the file here
			sound_valid = true;
		} else {
			err += "The sound file cannot be found!\n";
		}
	}

	if (r_error){
		*r_error=err;
	}

    return valid;
};

Error EditorExportPlatformPSP::export_project(const String& p_path,bool p_debug,bool p_dumb){
	//This is not a complete implementation, just a start
	List<String> args;
	int retcode;
	Error run_err;

	String sdk_path = EditorSettings::get_singleton()->get(sdk_path_var);

	//Make PARAM.SFO

	args.push_back("-d");
	args.push_back("MEMSIZE=1");
	args.push_back("-s");

	//TODO: Expose version variable
	args.push_back(Globals::get_singleton()->get("0"));

	String application_name = Globals::get_singleton()->get("application/name");

	if (application_name.empty()){ application_name = "Fusion Engine"; }

	args.push_back(application_name);

	String sfo_path = EditorSettings::get_singleton()->get_settings_path() + "/tmp/PARAM.SFO";

	args.push_back(sfo_path);

	run_err = OS::get_singleton()->execute(sdk_path + mksfo_path, args, true, NULL, NULL, &retcode);

	if (run_err or retcode != 0){
		print_line("Error in running mksfoex: " + String::num(retcode));
		return ERR_CANT_CREATE;
	}

	retcode = 0;

	args.clear();

	//Make EBOOT.PBP

	args.push_back(p_path); //output.pbp
	args.push_back(sfo_path); //param.sfo


	args.push_back(icon_valid ? Globals::get_singleton()->globalize_path(icon_path) : "NULL"); //icon0.png

	args.push_back("NULL"); //icon1.pmf
	args.push_back("NULL"); //pic0.png
	args.push_back(background_valid ? Globals::get_singleton()->globalize_path(background_path) : "NULL"); //pic1.png
	args.push_back(sound_valid ? Globals::get_singleton()->globalize_path(sound_path) : "NULL"); //snd0.at3

	String exe_path = EditorSettings::get_singleton()->get_settings_path()+"/templates/";

	if (p_debug){ //debug template
		if (embed_pck){
			//TODO: Copy template, embed pck, set path of embedded template here
			args.push_back(exe_path + "psp_debug.32");
		} else {
			args.push_back(exe_path + "psp_debug.32");
		}
	} else { //release template
		if (embed_pck){
			//TODO: Copy template, embed pck, set path of embedded template here
			args.push_back(exe_path + "psp_release.32");
		} else {
			args.push_back(exe_path + "psp_release.32");
		}
	}

	args.push_back("NULL"); //data.psar

	run_err = OS::get_singleton()->execute(sdk_path + pack_pbp_path, args, true, NULL, NULL, &retcode);

	if (run_err or retcode != 0){
		print_line("Error in running pack-pbp: " + String::num(retcode));
		return ERR_CANT_CREATE;
	}

	return OK;
};

void register_psp_exporter(){
	EDITOR_DEF(sdk_path_var, "");

    Ref<EditorExportPlatformPSP> exporter = memnew(EditorExportPlatformPSP);

	exporter->set_name("Playstation Portable");
	exporter->set_binary_extension("PBP");
	Image img(_psp_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
	exporter->set_logo(logo);

	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
