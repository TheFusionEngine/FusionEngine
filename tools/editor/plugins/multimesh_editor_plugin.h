/*************************************************************************/
/*  multimesh_editor_plugin.h                                            */
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
#ifndef MULTIMESH_EDITOR_PLUGIN_H
#define MULTIMESH_EDITOR_PLUGIN_H

#include "tools/editor/editor_plugin.h"
#include "tools/editor/editor_node.h"
#include "scene/3d/multimesh_instance.h"
#include "scene/gui/spin_box.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class MultiMeshEditor : public Control {

	OBJ_TYPE(MultiMeshEditor, Control );


	AcceptDialog *err_dialog;

	MultiMeshInstance3D *_last_pp_node;
	bool browsing_source;

	Panel *panel;
	MenuButton * options;
	MultiMeshInstance3D *node;

	LineEdit *surface_source;
	LineEdit *mesh_source;

	SceneTreeDialog *std;

	ConfirmationDialog *populate_dialog;
	OptionButton *populate_axis;
	HScrollBar *populate_rotate_random;
	HScrollBar *populate_tilt_random;
	SpinBox *populate_scale_random;
	SpinBox *populate_scale;
	SpinBox *populate_amount;

	enum Menu {

		MENU_OPTION_POPULATE
	};

	void _browsed(const NodePath& p_path);
	void _menu_option(int);
	void _populate();
	void _browse(bool p_source);

protected:
	void _node_removed(Node *p_node);
	static void _bind_methods();
public:

	void edit(MultiMeshInstance3D *p_multimesh);
	MultiMeshEditor();
};

class MultiMeshEditorPlugin : public EditorPlugin {

	OBJ_TYPE( MultiMeshEditorPlugin, EditorPlugin );

	MultiMeshEditor *multimesh_editor;
	EditorNode *editor;

public:

	virtual String get_name() const { return "MultiMesh"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_node);
	virtual bool handles(Object *p_node) const;
	virtual void make_visible(bool p_visible);

	MultiMeshEditorPlugin(EditorNode *p_node);
	~MultiMeshEditorPlugin();

};

#endif // MULTIMESH_EDITOR_PLUGIN_H
