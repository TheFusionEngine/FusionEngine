#include "export.h"
#include "platform/iphone/logo.h"
#include "tools/editor/editor_import_export.h"
#include "tools/editor/editor_settings.h"
#include "scene/resources/texture.h"

class EditorExportPlatformIPhone : public EditorExportPlatform {
    OBJ_TYPE(EditorExportPlatformIPhone, EditorExportPlatform);
private:

public:

	virtual String get_name() const;
	virtual ImageCompression get_image_compression() const;
	virtual Ref<Texture> get_logo() const;

	virtual bool poll_devices() { return false; }
	virtual int get_device_count() const { return 0; }
	virtual String get_device_name(int p_device) const { return ""; }
	virtual String get_device_info(int p_device) const { return ""; }
	virtual Error run(int p_device,bool p_dumb=false) { return OK; }

	virtual bool can_export(String *r_error=NULL) const;

	virtual bool requires_password(bool p_debug) const { return false; }
	virtual String get_binary_extension() const;
	virtual Error export_project(const String& p_path,bool p_debug,bool p_dumb=false);
};

String EditorExportPlatformIPhone::get_name() const {
    return "Apple iPhone";
}

EditorExportPlatform::ImageCompression EditorExportPlatformIPhone::get_image_compression() const{
    return IMAGE_COMPRESSION_NONE;
}

Ref<Texture> EditorExportPlatformIPhone::get_logo() const{
	Image img(_iphone_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
    return logo;
}

bool EditorExportPlatformIPhone::can_export(String *r_error) const {
    return false;
};

String EditorExportPlatformIPhone::get_binary_extension() const {
    return "ipa";
};

Error EditorExportPlatformIPhone::export_project(const String& p_path,bool p_debug,bool p_dumb){
    return ERR_DOES_NOT_EXIST;
};

void register_iphone_exporter(){
    Ref<EditorExportPlatformIPhone> exporter = Ref<EditorExportPlatformIPhone>(memnew(EditorExportPlatformIPhone));
	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
