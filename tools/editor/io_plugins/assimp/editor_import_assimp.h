#ifndef EDITOR_IMPORT_ASSIMP_H
#define EDITOR_IMPORT_ASSIMP_H

#include "tools/editor/io_plugins/editor_scene_import_plugin.h"
#include "tools/editor/editor_import_export.h"
#include "scene/resources/font.h"

class EditorNode;
class EditorMeshImportDialog;
class AssimpImportDialog;

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

	EditorMeshImportPlugin(EditorNode* p_editor);
};

class AssimpImportPlugin : public EditorImportPlugin {

	OBJ_TYPE(AssimpImportPlugin, EditorImportPlugin);

	AssimpImportDialog *dialog;

public:

	virtual String get_name() const {
		return "assimp_3d";
	};
	virtual String get_visible_name() const {
		return "3D Scene/Mesh (Assimp)";
	}
	virtual void import_dialog(const String& p_from="");
	virtual Error import(const String& p_path, const Ref<ResourceImportMetadata>& p_from);

};

#endif // EDITOR_IMPORT_ASSIMP_H
