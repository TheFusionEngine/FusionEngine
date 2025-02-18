/*************************************************************************/
/*  particles_editor_plugin.h                                            */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                 */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef PARTICLES_EDITOR_PLUGIN_H
#define PARTICLES_EDITOR_PLUGIN_H

#include "tools/editor/editor_plugin.h"
#include "tools/editor/editor_node.h"
#include "scene/3d/particles.h"
#include "scene/gui/spin_box.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class ParticlesEditor : public Control {

	OBJ_TYPE(ParticlesEditor, Control );

	Panel *panel;
	MenuButton *options;
	HBoxContainer *particles_editor_hb;
	Particles3D *node;


	FileDialog *emission_file_dialog;
	SceneTreeDialog *emission_tree_dialog;

	ConfirmationDialog *err_dialog;

	ConfirmationDialog *emission_dialog;
	SpinBox *emission_amount;
	OptionButton *emission_fill;




	enum Menu {

		MENU_OPTION_GENERATE_AABB,
		MENU_OPTION_CREATE_EMISSION_VOLUME_FROM_NODE,
		MENU_OPTION_CREATE_EMISSION_VOLUME_FROM_MESH,
		MENU_OPTION_CLEAR_EMISSION_VOLUME,

	};

	DVector<Face3> geometry;

	void _generate_emission_points();
	void _resource_seleted(const String& p_res);
	void _node_selected(const NodePath& p_path);

	void _menu_option(int);

	void _populate();

friend class ParticlesEditorPlugin;

protected:

	void _notification(int p_notification);
	void _node_removed(Node *p_node);
	static void _bind_methods();
public:

	void edit(Particles3D *p_particles);
	ParticlesEditor();
};

class ParticlesEditorPlugin : public EditorPlugin {

	OBJ_TYPE( ParticlesEditorPlugin, EditorPlugin );

	ParticlesEditor *particles_editor;
	EditorNode *editor;

public:

	virtual String get_name() const { return "Particles3D"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_node);
	virtual bool handles(Object *p_node) const;
	virtual void make_visible(bool p_visible);

	ParticlesEditorPlugin(EditorNode *p_node);
	~ParticlesEditorPlugin();

};

#endif // PARTICLES_EDITOR_PLUGIN_H
