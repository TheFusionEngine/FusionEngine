#include "texture_and_atlas_editor_plugin.h"
#include "tools/editor/editor_settings.h"

#define INRANGE(p_num, p_min, p_max)(p_num > p_min ? (p_num < p_max ? true : false) : false)

Rect2i create_rect_from_points(Point2i p_point_a, Point2i p_point_b){
    Rect2i rect;

    Point2i top_left;
    Point2i bottom_right;

    bool a_is_leftward = p_point_a.x < p_point_b.x;
    bool a_is_upward = p_point_a.y < p_point_b.y;

    if (a_is_leftward){
            top_left.x = p_point_a.x;
            bottom_right.x = p_point_b.x;
    } else {
            top_left.x = p_point_b.x;
            bottom_right.x = p_point_a.x;
    }
    if (a_is_upward){
            top_left.y = p_point_a.y;
            bottom_right.y = p_point_b.y;
    } else {
            top_left.y = p_point_b.y;
            bottom_right.y = p_point_a.y;
    }

    rect.pos = top_left;
    rect.size = bottom_right - top_left;

    return rect;
}

void TextureViewerEditor::draw_outline(Point2 p_corner_1, Point2 p_corner_3){
    Color outline_color = EDITOR_DEF("texture_viewer/atlas_outline_color", Color::hex(0xFFFFFFFF));

    Point2 corner_2 = Point2(p_corner_3.x, p_corner_1.y);
    Point2 corner_4 = Point2(p_corner_1.x, p_corner_3.y);

    draw_line(p_corner_1, corner_2, outline_color);
    draw_line(corner_2, p_corner_3, outline_color);
    draw_line(p_corner_3, corner_4, outline_color);
    draw_line(corner_4, p_corner_1, outline_color);
}

void TextureViewerEditor::_notification(int p_what){
    if (p_what == NOTIFICATION_DRAW){
        if (creating_atlas and corner_b != Vector2(0,0)){
            Color rect_color = EDITOR_DEF("texture_viewer/atlas_rect_color", Color::hex(0x83'd3'FF'FF));

            VisualServer::get_singleton()->canvas_item_set_on_top(get_canvas_item(), true);

            draw_rect(create_rect_from_points(corner_a, corner_b), rect_color);
            draw_outline(corner_a, corner_b);
        }

        if (show_atlas_outlines->is_pressed()){
            for (int i = 0; i < atlas_textures.size(); i++){
                //TODO: Render box outlines of all AtlasTextures of the viewed atlas,
                //Select an AtlasTexture to view by clicking its box outline
            }
        }
    }
}

void TextureViewerEditor::_input_event(const InputEvent p_event){
    if (p_event.type == InputEvent::MOUSE_BUTTON){
        int button = p_event.mouse_button.button_index;

        const int pan_speed = 10;

        Transform2D texture_transform = texture_preview->get_transform();

        if (p_event.is_pressed()){
            switch (button){
                case BUTTON_LEFT: //left click
                    if (mode == VIEW_ATLASTEX_CREATOR){
                        Point2i mouse_pos = Point2i(p_event.mouse_button.global_x, p_event.mouse_button.global_y);

                        Vector2 tex_top_left = texture_preview->get_global_transform().get_origin();
                        tex_top_left += Vector2(texture_scroll->get_h_scroll(), texture_scroll->get_v_scroll());

                        Rect2 tex_rect = Rect2(tex_top_left, texture_preview->get_size());

                        if (not tex_rect.has_point(mouse_pos)){
                            print_line("Failed to create atlas because mouse started out-of-bounds!" + String::num(mouse_pos.x) + " " + String::num(mouse_pos.y));
                            //out of bounds
                            break;
                        }

                        corner_a = localize_pos(mouse_pos);

                        if (INRANGE(corner_a.x, 0, get_texture()->get_width()) and INRANGE(corner_a.y, 0, get_texture()->get_height())){
                            creating_atlas = true;
                        } else {
                            creating_atlas = false;
                            corner_a = Vector2(0,0);
                            break;
                        }
                    }
                    break;
                case BUTTON_RIGHT: //right click
                    //TODO: Have this create an atlas texture by scanning outwards from the click point until clear space is hit on all sides
                    break;
                case BUTTON_WHEEL_UP:
                    texture_transform.scale_basis(Vector2(1.1, 1.1));

                    break;
                case BUTTON_WHEEL_DOWN:
                    texture_transform.scale_basis(Vector2(0.9, 0.9));
                    break;
                case 6: //pan left
                    texture_scroll->set_h_scroll(texture_scroll->get_h_scroll() - pan_speed);
                    break;
                case 7: //pan right
                    texture_scroll->set_h_scroll(texture_scroll->get_h_scroll() + pan_speed);
                    break;

                default: break;
            }
        } else {
            creating_atlas = false;
        }

        if (not creating_atlas and corner_a != Point2i(0,0) and corner_b != Point2i(0,0)){
            //generate_atlas
            block_opening = true;

            Point2 preview_offset = texture_preview->get_global_transform().get_origin() - get_global_transform().get_origin();

            Rect2i tex_region = create_rect_from_points(corner_a - preview_offset, corner_b - preview_offset);

            tex_region.set_size(
                Point2i(
                    MIN(tex_region.get_size().x, get_texture()->get_width()),
                    MIN(tex_region.get_size().y, get_texture()->get_height())
                )
            );

            AtlasTexture *new_tex = memnew(AtlasTexture);

            new_tex->set_atlas(get_texture());

            new_tex->set_region(tex_region);

            if (atlas_name->get_text() != ""){
                new_tex->set_name(atlas_name->get_text() + "_" + String::num(new_tex->get_instance_ID()));
            }

            add_atlas_button(new_tex);

            corner_a = Vector2(0,0);
            corner_b = Vector2(0,0);

        }
    } else if (p_event.type == InputEvent::MOUSE_MOTION and creating_atlas){
        Point2i mouse_pos = Point2i(p_event.mouse_motion.global_x, p_event.mouse_motion.global_y);
        corner_b = mouse_pos - get_global_transform().get_origin();
    }
    update();
}

void TextureViewerEditor::_bind_methods(){
    ObjectTypeDB::bind_method(_MD("_input_event"),&TextureViewerEditor::_input_event);
    ObjectTypeDB::bind_method(_MD("switch_mode", "mode"),&TextureViewerEditor::switch_mode);
    ObjectTypeDB::bind_method(_MD("delete_atlas", "atlas"),&TextureViewerEditor::delete_atlas);
    ObjectTypeDB::bind_method(_MD("set_editing_atlas", "atlas"),&TextureViewerEditor::set_editing_atlas);

}

Texture *TextureViewerEditor::get_texture(){
    return texture_preview->get_texture().ptr();
}

void TextureViewerEditor::load_texture(Texture *p_texture){

    if (not block_opening and p_texture->is_type("AtlasTexture")){
        AtlasTexture *p_atlas = p_texture->cast_to<AtlasTexture>();
        if (p_atlas->get_atlas()->get_instance_ID() == get_texture()->get_instance_ID()){
            block_opening = false;
            return;
        }
    }
    print_line("Loading Texture..." + p_texture->get_path() + " " + p_texture->get_name());
    atlas_textures.clear();

    texture_preview->set_texture(p_texture);

    view_mode->select(VIEW_IMAGE);
}

void TextureViewerEditor::delete_atlas(ObjectID p_obj){

    HBoxContainer *hbox = ObjectDB::get_instance(p_obj)->cast_to<HBoxContainer>();
    if (hbox){
        memdelete(hbox);
    }
}

void TextureViewerEditor::set_editing_atlas(ObjectID p_obj){
    Texture *atlas = ObjectDB::get_instance(p_obj)->cast_to<Texture>();

    //block_opening = false;

    EditorNode::get_singleton()->edit_resource(atlas);
}


//Never call this directly, because it's hooked up to view_mode by signal. Instead, call view_mode->select
void TextureViewerEditor::switch_mode(ViewMode p_mode){

    bool mode_switched = p_mode != mode;

    if (mode_switched){ //clear out info panel
        for (int i = 0; i < info_base->get_child_count();){
            Node * child =info_base->get_child(i);
            memdelete(child);
        }
        mode = p_mode;

        //Hide specific GUI elements by default
        info_base->hide();
        show_atlas_outlines->hide();
    }

    if (p_mode == VIEW_ATLASTEX_CREATOR){
        if (mode_switched){
            load_atlas_menu();
        }

        load_atlas_textures(EditorFileSystem::get_singleton()->get_filesystem());
    }

    else if (p_mode == VIEW_PALETTE){
        ImageTexture *img_text = get_texture()->cast_to<ImageTexture>();

        if (img_text and (img_text->get_format() == Image::FORMAT_INDEXED or img_text->get_format() == Image::FORMAT_INDEXED_ALPHA)){
            //we can actually load the palette info
        }
        else {
            //smth smth tell the user this is not an indexed image
        }
    }
}

void TextureViewerEditor::load_atlas_menu(){
    show_atlas_outlines->show();
    info_base->show();
    info_base->set_tooltip("All the AtlasTextures using this texture as an atlas.");

    VBoxContainer *vbox = memnew(VBoxContainer);
    vbox->set_h_size_flags(SIZE_FILL);
    vbox->set_v_size_flags(SIZE_FILL);

    info_base->add_child(vbox);

    Label *label = memnew(Label); label->set_text("AtlasTextures\n\nDefault name:");

    vbox->add_child(label);

    atlas_name = memnew(LineEdit);

    vbox->add_child(atlas_name);

    ScrollContainer *scroll = memnew(ScrollContainer);
    scroll->set_h_size_flags(SIZE_EXPAND_FILL);
    scroll->set_v_size_flags(SIZE_EXPAND_FILL);
    scroll->set_enable_h_scroll(false);

    vbox->add_child(scroll);

    atlastex_list = memnew(VBoxContainer);
    atlastex_list->set_h_size_flags(SIZE_EXPAND_FILL);
    atlastex_list->set_v_size_flags(SIZE_EXPAND_FILL);

    scroll->add_child(atlastex_list);
}

//based on QuickOpen
void TextureViewerEditor::load_atlas_textures(EditorFileSystemDirectory *efsd){
    block_opening = true;

	for(int i = 0; i < efsd->get_subdir_count(); i++) {
		load_atlas_textures(efsd->get_subdir(i));
	}

	for(int i = 0; i < efsd->get_file_count(); i++) {
		if (ObjectTypeDB::is_type(efsd->get_file_type(i),"AtlasTexture")) {

            String atlas_path = efsd->get_file_path(i);

            List<String> deps;

            ResourceLoader::get_dependencies(atlas_path, &deps);

            if (deps.find(get_texture()->get_path())){
                Ref<AtlasTexture> tex = ResourceLoader::load(atlas_path);
                //This kinda just assumes the file loads successfully, might be an issue but idk
                if (tex.ptr()){
                    atlas_textures.push_back(tex.ptr());
                    add_atlas_button(tex.ptr());
                }
            }
		}
	}
	block_opening = false;
}

void TextureViewerEditor::add_atlas_button(AtlasTexture *p_atlas){
    HBoxContainer *hbox = memnew(HBoxContainer);
    hbox->set_h_size_flags(SIZE_EXPAND_FILL);
    hbox->set_v_size_flags(SIZE_FILL);

    Vector<Variant> bind_args;
    bind_args.push_back(p_atlas->get_instance_ID());

    Button *select = memnew(Button);
    select->set_icon(p_atlas);
    if (p_atlas->get_path() != ""){
        select->set_text(p_atlas->get_path().get_file());
    }
    select->set_h_size_flags(SIZE_EXPAND_FILL);
    select->set_v_size_flags(SIZE_EXPAND_FILL);
    hbox->add_child(select);

    select->connect("pressed", this, "set_editing_atlas", bind_args);

    bind_args.clear();
    bind_args.push_back(hbox->get_instance_ID());

    Button *del_button = memnew(Button);
    del_button->set_text("Hide");
    del_button->set_v_size_flags(SIZE_EXPAND_FILL);
    del_button->set_anchor(MARGIN_RIGHT, ANCHOR_END);
    del_button->set_anchor(MARGIN_BOTTOM, ANCHOR_END);

    hbox->add_child(del_button);


    hbox->set_custom_minimum_size(hbox->get_size());

    atlastex_list->add_child(hbox);

    Ref<Resource> res_atlas = p_atlas->cast_to<Resource>();

    del_button->connect("pressed", this, "delete_atlas", bind_args);
}

TextureViewerEditor::TextureViewerEditor(){
    set_h_size_flags(SIZE_EXPAND_FILL);
    set_v_size_flags(SIZE_EXPAND_FILL);

    //Header bar with options

    HBoxContainer *hbox = memnew(HBoxContainer);

    view_mode = memnew(OptionButton);

    view_mode->set_tooltip("Change the Texture mode");
    view_mode->add_item("Texture Viewer", VIEW_IMAGE);
    view_mode->add_item("AtlasTexture Creator", VIEW_ATLASTEX_CREATOR);
    //view_mode->add_item("PaletteTexture Viewer", VIEW_PALETTE);

    view_mode->connect("item_selected", this, "switch_mode");

    hbox->add_child(view_mode);

    show_atlas_outlines = memnew(CheckButton);
    show_atlas_outlines->set_text("Show AtlasTexture Outlines");
    show_atlas_outlines->hide();

    hbox->add_child(show_atlas_outlines);

    add_child(hbox);

    //Base GUI

    HSplitContainer *hsplit = memnew(HSplitContainer);

    hsplit->set_anchor(MARGIN_RIGHT, ANCHOR_END);
    hsplit->set_anchor(MARGIN_BOTTOM, ANCHOR_END);
    //This causes it to not block when moving the seperator, but it's necessary
    //to make the atlas rectangle drawing work
    hsplit->set_stop_mouse(false);
    hsplit->set_h_size_flags(SIZE_FILL);
    hsplit->set_v_size_flags(SIZE_EXPAND_FILL);

    add_child(hsplit);

    //Info base

    info_base = memnew(PanelContainer);
    info_base->set_h_size_flags(SIZE_FILL);
    info_base->set_v_size_flags(SIZE_FILL);
    info_base->hide();

    hsplit->add_child(info_base);

    //Texture view

    texture_preview = memnew(TextureFrame);
    texture_preview->set_expand(false);
    texture_preview->set_h_size_flags(SIZE_FILL);
    texture_preview->set_v_size_flags(SIZE_FILL);
    texture_preview->set_stop_mouse(false);
    texture_preview->set_ignore_mouse(false);
    texture_preview->set_draw_behind_parent(false);

    texture_scroll = memnew(ScrollContainer);
    texture_scroll->set_h_size_flags(SIZE_EXPAND_FILL);
    texture_scroll->set_v_size_flags(SIZE_EXPAND_FILL);
    texture_scroll->set_stop_mouse(false);

    texture_scroll->add_child(texture_preview);

    hsplit->add_child(texture_scroll);

    view_mode->select(VIEW_IMAGE);
}



void TextureViewEditorPlugin::edit(Object *p_node){

    tex_view->set_undo_redo(&get_undo_redo());

    Texture *tex = p_node->cast_to<Texture>();

    if (!tex){ return;}

    tex_view->load_texture(tex);
}

bool TextureViewEditorPlugin::handles(Object *p_node) const {
    return p_node->is_type("Texture");
}

void TextureViewEditorPlugin::make_visible(bool p_visible){
    if (p_visible){
        tex_view->show();
        tex_view->set_process(true);

    } else {
        tex_view->hide();
        tex_view->set_process(false);
    }
}

TextureViewEditorPlugin::TextureViewEditorPlugin(EditorNode *p_node){
	editor=p_node;
	tex_view = memnew( TextureViewerEditor );
	editor->get_viewport()->add_child(tex_view);
	tex_view->set_area_as_parent_rect();
    tex_view->hide();
}
