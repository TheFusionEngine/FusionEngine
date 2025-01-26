#include "export.h"
#include "platform/nacl/logo.h"
#include "tools/editor/editor_import_export.h"
#include "tools/editor/editor_settings.h"
#include "scene/resources/texture.h"

class EditorExportPlatformNaCl : public EditorExportPlatform {
    OBJ_TYPE(EditorExportPlatformNaCl, EditorExportPlatform);
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

String EditorExportPlatformNaCl::get_name() const {
    return "Google Native Client";
}

EditorExportPlatform::ImageCompression EditorExportPlatformNaCl::get_image_compression() const{
    return ImageCompression::IMAGE_COMPRESSION_NONE;
}

Ref<Texture> EditorExportPlatformNaCl::get_logo() const {
	Image img(_nacl_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
    return logo;
}

bool EditorExportPlatformNaCl::can_export(String *r_error) const {
    return false;
};

String EditorExportPlatformNaCl::get_binary_extension() const {
    return "PBP"; //TODO: Whatever file extension nacl uses
};

Error EditorExportPlatformNaCl::export_project(const String& p_path,bool p_debug,bool p_dumb){
    return Error::ERR_DOES_NOT_EXIST;
};

void register_nacl_exporter(){
    Ref<EditorExportPlatformNaCl> exporter = Ref<EditorExportPlatformNaCl>(memnew(EditorExportPlatformNaCl));
	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
