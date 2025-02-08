#include "export.h"
#include "platform/dreamcast/logo.h"
#include "tools/editor/editor_import_export.h"
#include "scene/resources/texture.h"

class EditorExportPlatformDC : public EditorExportPlatformConsole {
    OBJ_TYPE(EditorExportPlatformDC, EditorExportPlatformConsole);

public:

	virtual ImageCompression get_image_compression() const;

	virtual bool poll_devices() {return false;}
	virtual int get_device_count() const {return 0;}
	virtual String get_device_name(int p_device) const {return "Dreamcast";}
	virtual String get_device_info(int p_device) const {return "";}
	virtual Error run(int p_device,bool p_dumb=false);

	virtual bool can_export(String *r_error=NULL) const;

	virtual bool requires_password(bool p_debug) const {return false;}
	virtual Error export_project(const String& p_path,bool p_debug,bool p_dumb=false);
};

EditorExportPlatform::ImageCompression EditorExportPlatformDC::get_image_compression() const{
    return ImageCompression::IMAGE_COMPRESSION_PVRTC;
};

Error EditorExportPlatformDC::run(int p_device,bool p_dumb){
    return Error::OK;
}

bool EditorExportPlatformDC::can_export(String *r_error) const {
    return false;
};

Error EditorExportPlatformDC::export_project(const String& p_path,bool p_debug,bool p_dumb){
    return OK;
};

void register_dreamcast_exporter() {
	Ref<EditorExportPlatformDC> exporter = Ref<EditorExportPlatformDC>(memnew(EditorExportPlatformDC));

	exporter->set_binary_extension("elf");
	exporter->set_name("Sega Dreamcast");

	Image img(_dreamcast_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
	exporter->set_logo(logo);

	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
