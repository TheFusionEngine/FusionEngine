#ifndef EDITOR_IMPORT_ASSIMP_H
#define EDITOR_IMPORT_ASSIMP_H

#include "tools/editor/io_plugins/editor_scene_import_plugin.h"
#include "tools/editor/editor_import_export.h"
#include "scene/resources/font.h"

class EditorNode;
class EditorMeshImportDialog;

class EditorMeshImportPlugin : public EditorImportPlugin {

	OBJ_TYPE(EditorMeshImportPlugin,EditorImportPlugin);

	EditorMeshImportDialog *dialog;

	bool generate_normals;
	bool generate_tangents;
	bool flip_faces;
	bool force_smooth;
	bool weld_vertices;
	float weld_tolerance;

public:

	virtual String get_name() const;
	virtual String get_visible_name() const;
	virtual void import_dialog(const String& p_from="");
	virtual Error import(const String& p_path, const Ref<ResourceImportMetadata>& p_from);

	Error old_obj_import(const String& p_path, const Ref<ResourceImportMetadata>& p_from);
	Error import_assimp(const String& p_path, const Ref<ResourceImportMetadata>& p_from);

	EditorMeshImportPlugin(EditorNode* p_editor);
};


class EditorAssimpSceneImporter : public EditorSceneImporter {
	OBJ_TYPE(EditorAssimpSceneImporter,EditorSceneImporter );

public:
	virtual uint32_t get_import_flags() const;
	virtual void get_extensions(List<String> *r_extensions) const;
	virtual Node* import_scene(const String& p_path,uint32_t p_flags,int p_bake_fps,List<String> *r_missing_deps=NULL,Error* r_err=NULL);
	virtual Ref<Animation> import_animation(const String& p_path,uint32_t p_flags);

	EditorAssimpSceneImporter();
};


#endif // EDITOR_IMPORT_ASSIMP_H
