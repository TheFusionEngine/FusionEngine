#include "export.h"
#include "platform/3ds/logo.h"
#include "tools/editor/editor_import_export.h"
#include "tools/editor/editor_settings.h"
#include "scene/resources/texture.h"

class EditorExportPlatform3DS : public EditorExportPlatformConsole {
    OBJ_TYPE(EditorExportPlatform3DS, EditorExportPlatformConsole);
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

	void _get_property_list( List<PropertyInfo> *p_list) const;
};

void EditorExportPlatform3DS::_get_property_list( List<PropertyInfo> *p_list) const{

}

EditorExportPlatform::ImageCompression EditorExportPlatform3DS::get_image_compression() const{
    return ImageCompression::IMAGE_COMPRESSION_NONE;
}

bool EditorExportPlatform3DS::can_export(String *r_error) const {
    return false;
};

Error EditorExportPlatform3DS::export_project(const String& p_path,bool p_debug,bool p_dumb){
    return Error::ERR_DOES_NOT_EXIST;
};

void register_3ds_exporter(){
    Ref<EditorExportPlatform3DS> exporter = memnew(EditorExportPlatform3DS);

	exporter->set_binary_extension("cia");
	exporter->set_name("Nintendo 3DS");

	Image img(_3ds_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
	exporter->set_logo(logo);

	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
