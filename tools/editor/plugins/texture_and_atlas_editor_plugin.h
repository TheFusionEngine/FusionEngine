#ifndef TEXTURE_AND_ATLASTEX_EDITOR_PLUGIN_H
#define TEXTURE_AND_ATLASTEX_EDITOR_PLUGIN_H

#include "tools/editor/editor_plugin.h"
#include "tools/editor/editor_node.h"
#include "scene/gui/box_container.h"
#include "scene/gui/split_container.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/button_array.h"
#include "scene/gui/dialogs.h"

class TextureViewerEditor;

class TextureViewerEditor : public VBoxContainer {
    OBJ_TYPE(TextureViewerEditor, VBoxContainer);
public:
    enum ViewMode {
        VIEW_IMAGE,
        VIEW_ATLASTEX_CREATOR,
        VIEW_PALETTE,
    };
private:
    TextureFrame *texture_preview;
    ScrollContainer *texture_scroll;

    PanelContainer *info_base;
    OptionButton *view_mode;

    UndoRedo *undo_redo;

    LineEdit *atlas_name;
    VBoxContainer *atlastex_list;
    CheckButton *show_atlas_outlines;

    Vector<AtlasTexture *> atlas_textures;

    Vector2 corner_a;
    Vector2 corner_b;

    ViewMode mode;
    bool block_opening;
    bool creating_atlas;
    bool panning;

    void load_atlas_menu();
    //void load_palette_menu();

    //for use with atlas texture coordinates
    _FORCE_INLINE_ Vector2 localize_pos(Vector2 p_pos){return p_pos - get_global_transform().get_origin();}
    _FORCE_INLINE_ Vector2 globalize_pos(Vector2 p_pos){return p_pos + get_global_transform().get_origin();}

    void load_atlas_textures(EditorFileSystemDirectory *efsd);
    void add_atlas_button(AtlasTexture *p_atlas);

    void draw_outline(Point2 p_corner_1, Point2 p_corner_2);

protected:
    void _notification(int p_what);
    static void _bind_methods();

public:
    void _input_event(const InputEvent p_event);

    Texture *get_texture();
    void load_texture(Texture *p_texture);

    void delete_atlas(ObjectID p_to_delete);
    void set_editing_atlas(ObjectID p_obj);
    uint32_t append_preview();

    void set_undo_redo(UndoRedo *p_undo_redo) {undo_redo=p_undo_redo; }

    void switch_mode(ViewMode mode);

    TextureViewerEditor();
};

VARIANT_ENUM_CAST(TextureViewerEditor::ViewMode);

class TextureViewEditorPlugin : public EditorPlugin {
    OBJ_TYPE(TextureViewEditorPlugin, EditorPlugin);

    EditorNode *editor;
    TextureViewerEditor *tex_view;

public:

	virtual String get_name() const { return "Texture"; }
	bool has_main_screen() const { return true; }
	virtual void edit(Object *p_node);
	virtual bool handles(Object *p_node) const;
	virtual void make_visible(bool p_visible);

    TextureViewEditorPlugin(EditorNode *p_editor_node);
};

#endif
