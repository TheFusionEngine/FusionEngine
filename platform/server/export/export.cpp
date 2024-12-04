#include "export.h"
#include "platform/server/logo.h"
#include "tools/editor/editor_import_export.h"
#include "scene/resources/texture.h"

void register_server_exporter(){
	Image img(_server_logo);
	Ref<ImageTexture> logo = memnew( ImageTexture );
	logo->create_from_image(img);

	{
		Ref<EditorExportPlatformPC> exporter = Ref<EditorExportPlatformPC>( memnew(EditorExportPlatformPC) );
		exporter->set_binary_extension("x86_64");
		exporter->set_release_binary32("server_32_release");
		exporter->set_debug_binary32("server_32_debug");
		exporter->set_release_binary64("server_64_release");
		exporter->set_debug_binary64("server_64_debug");
		exporter->set_name("Server");
		exporter->set_logo(logo);
		EditorImportExport::get_singleton()->add_export_platform(exporter);
	}
}
