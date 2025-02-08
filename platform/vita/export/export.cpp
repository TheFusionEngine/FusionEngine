#include "export.h"
#include "platform/vita/logo.h"
#include "tools/editor/editor_import_export.h"
#include "tools/editor/editor_settings.h"
#include "scene/resources/texture.h"

class EditorExportPlatformVita : public EditorExportPlatformConsole {
    OBJ_TYPE(EditorExportPlatformVita, EditorExportPlatformConsole);
private:

public:
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

EditorExportPlatform::ImageCompression EditorExportPlatformVita::get_image_compression() const{
    return ImageCompression::IMAGE_COMPRESSION_NONE;
}

bool EditorExportPlatformVita::can_export(String *r_error) const {
    return false;
};

Error EditorExportPlatformVita::export_project(const String& p_path,bool p_debug,bool p_dumb){
    return Error::ERR_DOES_NOT_EXIST;
};

void register_vita_exporter(){
    Ref<EditorExportPlatformVita> exporter = Ref<EditorExportPlatformVita>(memnew(EditorExportPlatformVita));

	exporter->set_binary_extension("PBP");
	exporter->set_name("Playstation Vita");

	Image img(_vita_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
	exporter->set_logo(logo);

	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
