#include "export.h"
#include "platform/winrt/logo.h"
#include "tools/editor/editor_import_export.h"
#include "tools/editor/editor_settings.h"
#include "scene/resources/texture.h"

class EditorExportPlatformWinRT : public EditorExportPlatform {
    OBJ_TYPE(EditorExportPlatformWinRT, EditorExportPlatform);
private:
    Ref<Texture> logo;

public:
    EditorExportPlatformWinRT();

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

String EditorExportPlatformWinRT::get_name() const {
    return "Windows RT";
}

EditorExportPlatform::ImageCompression EditorExportPlatformWinRT::get_image_compression() const{
    return ImageCompression::IMAGE_COMPRESSION_NONE;
}

Ref<Texture> EditorExportPlatformWinRT::get_logo() const{
    return logo;
}

bool EditorExportPlatformWinRT::can_export(String *r_error) const {
    return false;
};

String EditorExportPlatformWinRT::get_binary_extension() const {
    return "exe";
};

Error EditorExportPlatformWinRT::export_project(const String& p_path,bool p_debug,bool p_dumb){
    return Error::ERR_DOES_NOT_EXIST;
};

EditorExportPlatformWinRT::EditorExportPlatformWinRT(){
	Image img(_winrt_logo);
	Ref<ImageTexture> logo = memnew(ImageTexture);
	logo->create_from_image(img);
};

void register_winrt_exporter(){
    Ref<EditorExportPlatformWinRT> exporter = Ref<EditorExportPlatformWinRT>(memnew(EditorExportPlatformWinRT));
	EditorImportExport::get_singleton()->add_export_platform(exporter);
}
