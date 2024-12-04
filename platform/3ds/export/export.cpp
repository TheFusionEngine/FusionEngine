#include "export.h"
#include "platform/3ds/logo.h"
#include "tools/editor/editor_import_export.h"
#include "tools/editor/editor_settings.h"
#include "scene/resources/texture.h"

class EditorExportPlatform3DS : public EditorExportPlatform {
    OBJ_TYPE(EditorExportPlatform3DS, EditorExportPlatform);
private:
    Ref<Texture> logo;

public:
    EditorExportPlatform3DS();

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

String EditorExportPlatform3DS::get_name() const {
    return "3DS";
}

EditorExportPlatform::ImageCompression EditorExportPlatform3DS::get_image_compression() const{
    return ImageCompression::IMAGE_COMPRESSION_NONE;
}

Ref<Texture> EditorExportPlatform3DS::get_logo() const{
    return logo;
}

bool EditorExportPlatform3DS::can_export(String *r_error) const {
    return false;
};

String EditorExportPlatform3DS::get_binary_extension() const {
    return "cia";
};

Error EditorExportPlatform3DS::export_project(const String& p_path,bool p_debug,bool p_dumb){
    return Error::ERR_DOES_NOT_EXIST;
};

EditorExportPlatform3DS::EditorExportPlatform3DS(){
	Image img(_3ds_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
}


void register_3ds_exporter(){
    EDITOR_DEF("3ds/embed_pck", ""); //embed pck in the cia
    Ref<EditorExportPlatform3DS> exporter = Ref<EditorExportPlatform3DS>(memnew(EditorExportPlatform3DS));
	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
