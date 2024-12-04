#include "export.h"
#include "platform/dreamcast/logo.h"
#include "tools/editor/editor_import_export.h"
#include "scene/resources/texture.h"

class EditorExportPlatformDC : public EditorExportPlatform {
    OBJ_TYPE(EditorExportPlatformDC, EditorExportPlatform);
    Ref<Texture> logo;

public:
    EditorExportPlatformDC();

	virtual String get_name() const;
	virtual ImageCompression get_image_compression() const;
	virtual Ref<Texture> get_logo() const;

	virtual bool poll_devices() {return false;}
	virtual int get_device_count() const {return 0;}
	virtual String get_device_name(int p_device) const {return "Dreamcast";}
	virtual String get_device_info(int p_device) const {return "";}
	virtual Error run(int p_device,bool p_dumb=false);

	virtual bool can_export(String *r_error=NULL) const;

        //someday we gotta fix this typo lol
	virtual bool requieres_password(bool p_debug) const {return false;}
	virtual String get_binary_extension() const;
	virtual Error export_project(const String& p_path,bool p_debug,bool p_dumb=false);
};

String EditorExportPlatformDC::get_name() const {
	return "Dreamcast";
};

EditorExportPlatform::ImageCompression EditorExportPlatformDC::get_image_compression() const{
    return ImageCompression::IMAGE_COMPRESSION_PVRTC;
};

Ref<Texture> EditorExportPlatformDC::get_logo() const{
    return logo;
}

Error EditorExportPlatformDC::run(int p_device,bool p_dumb){
    return Error::OK;
}

bool EditorExportPlatformDC::can_export(String *r_error) const {
    return false;
};

String EditorExportPlatformDC::get_binary_extension() const {
    return "elf";
};

Error EditorExportPlatformDC::export_project(const String& p_path,bool p_debug,bool p_dumb){
    return OK;
};

EditorExportPlatformDC::EditorExportPlatformDC(){
	Image img(_dreamcast_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
};

void register_dreamcast_exporter() {
     Ref<EditorExportPlatformDC> exporter = Ref<EditorExportPlatformDC>(memnew(EditorExportPlatformDC));
	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
