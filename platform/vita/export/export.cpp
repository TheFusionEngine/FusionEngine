#include "export.h"
#include "platform/vita/logo.h"
#include "tools/editor/editor_import_export.h"
#include "tools/editor/editor_settings.h"
#include "scene/resources/texture.h"

class EditorExportPlatformVita : public EditorExportPlatform {
    OBJ_TYPE(EditorExportPlatformVita, EditorExportPlatform);
private:
    Ref<Texture> logo;

public:
    EditorExportPlatformVita();

	virtual String get_name() const;
	virtual ImageCompression get_image_compression() const;
	virtual Ref<Texture> get_logo() const;

	virtual bool poll_devices() { return false; }
	virtual int get_device_count() const { return 0; }
	virtual String get_device_name(int p_device) const { return ""; }
	virtual String get_device_info(int p_device) const { return ""; }
	virtual Error run(int p_device,bool p_dumb=false) { return OK; }

	virtual bool can_export(String *r_error=NULL) const;

	virtual bool requieres_password(bool p_debug) const { return false; }
	virtual String get_binary_extension() const;
	virtual Error export_project(const String& p_path,bool p_debug,bool p_dumb=false);
};

String EditorExportPlatformVita::get_name() const {
    return "Playstation Vita";
}

EditorExportPlatform::ImageCompression EditorExportPlatformVita::get_image_compression() const{
    return ImageCompression::IMAGE_COMPRESSION_NONE;
}

Ref<Texture> EditorExportPlatformVita::get_logo() const{
    return logo;
}

bool EditorExportPlatformVita::can_export(String *r_error) const {
    return false;
};

String EditorExportPlatformVita::get_binary_extension() const {
    return "PBP";
};

Error EditorExportPlatformVita::export_project(const String& p_path,bool p_debug,bool p_dumb){
    return Error::ERR_DOES_NOT_EXIST;
};

EditorExportPlatformVita::EditorExportPlatformVita(){
	Image img(_vita_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
};

void register_vita_exporter(){
	EDITOR_DEF("vita/embed_pck", "");
    Ref<EditorExportPlatformVita> exporter = Ref<EditorExportPlatformVita>(memnew(EditorExportPlatformVita));
	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
