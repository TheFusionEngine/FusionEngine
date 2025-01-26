#include "export.h"
#include "platform/symbian/logo.h"
#include "tools/editor/editor_import_export.h"
#include "tools/editor/editor_settings.h"
#include "scene/resources/texture.h"


class EditorExportPlatformSymbian : public EditorExportPlatform {
    OBJ_TYPE(EditorExportPlatformSymbian, EditorExportPlatform);

public:

	virtual String get_name() const;
	virtual ImageCompression get_image_compression() const;
	virtual Ref<Texture> get_logo() const;

	virtual bool poll_devices() {return false;}
	virtual int get_device_count() const {return 0;}
	virtual String get_device_name(int p_device) const {return "";}
	virtual String get_device_info(int p_device) const {return "";}
	virtual Error run(int p_device,bool p_dumb=false);

	virtual bool can_export(String *r_error=NULL) const;

	virtual bool requires_password(bool p_debug) const {return false;}
	virtual String get_binary_extension() const;
	virtual Error export_project(const String& p_path,bool p_debug,bool p_dumb=false);
};

String EditorExportPlatformSymbian::get_name() const {
	return "Symbian";
};

EditorExportPlatform::ImageCompression EditorExportPlatformSymbian::get_image_compression() const{
    return ImageCompression::IMAGE_COMPRESSION_NONE;
};

Ref<Texture> EditorExportPlatformSymbian::get_logo() const {
	Image img(_symbian_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
	return logo;
}

Error EditorExportPlatformSymbian::run(int p_device,bool p_dumb){
    return Error::OK;
}

bool EditorExportPlatformSymbian::can_export(String *r_error) const {
    return false;
};

String EditorExportPlatformSymbian::get_binary_extension() const {
    return "elf"; //prolly wrong
};

Error EditorExportPlatformSymbian::export_project(const String& p_path,bool p_debug,bool p_dumb){
    return OK;
};

void register_symbian_exporter(){
    Ref<EditorExportPlatformSymbian> exporter = Ref<EditorExportPlatformSymbian>(memnew(EditorExportPlatformSymbian));
	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
