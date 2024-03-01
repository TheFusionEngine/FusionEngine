/*************************************************************************/
/*  rasterizer_3ds.cpp                                                 */
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
#include "rasterizer_3ds.h"
#include "shaders/simple_3d.h"
#include "shaders/simple_2d.h"

static const vertex3ds vertex_list[] =
{
	// First face (PZ)
	// First triangle
	{ {-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
	{ {+0.5f, -0.5f, +0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
	{ {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f} },
	// Second triangle
	{ {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f} },
	{ {-0.5f, +0.5f, +0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, +1.0f} },
	{ {-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },

	// Second face (MZ)
	// First triangle
	{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
	{ {-0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
	{ {+0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
	// Second triangle
	{ {+0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
	{ {+0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
	{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },

	// Third face (PX)
	// First triangle
	{ {+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },
	{ {+0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },
	{ {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} },
	// Second triangle
	{ {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} },
	{ {+0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} },
	{ {+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },

	// Fourth face (MX)
	// First triangle
	{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
	{ {-0.5f, -0.5f, +0.5f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
	{ {-0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
	// Second triangle
	{ {-0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
	{ {-0.5f, +0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
	{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },

	// Fifth face (PY)
	// First triangle
	{ {-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },
	{ {-0.5f, +0.5f, +0.5f}, {1.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },
	{ {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f} },
	// Second triangle
	{ {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f} },
	{ {+0.5f, +0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, +1.0f, 0.0f} },
	{ {-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },

	// Sixth face (MY)
	// First triangle
	{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
	{ {+0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
	{ {+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
	// Second triangle
	{ {+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
	{ {-0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
	{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
};

#define vertex_list_count (sizeof(vertex_list)/sizeof(vertex_list[0]))

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))


#define TEXTURE_TRANSFER_FLAGS (GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) |  GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
		GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) |  GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))
	

void _set_uniform(int uniform_location, const Matrix32& p_transform)
{
	const Matrix32& tr = p_transform;
	
	C3D_Mtx mtx;
	
	float matrix[16] = { /* build a 16x16 matrix */
		tr.elements[2][0], 0, tr.elements[1][0], tr.elements[0][0],
		tr.elements[2][1], 0, tr.elements[1][1], tr.elements[0][1],
		0, 1, 0, 0,
		1, 0, 0, 0,
	};
	
	memcpy(mtx.m, matrix, sizeof(matrix));
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uniform_location, &mtx);
}

void _set_uniform(int uniform_location, const CameraMatrix& p_matrix)
{
	// Swap X/Y axis due to 3DS screens being sideways
	// and adjust clipping range from [-1, 1] to [-1, 0]
	float m[16] = {
		0, -1, 0, 0,
		1, 0, 0, 0,
		0, 0, 0.5, 0,
		0, 0, -0.5, 1,
	};
	
	CameraMatrix p = *reinterpret_cast<CameraMatrix*>(m) * p_matrix;
	
	C3D_Mtx mtx;
	float *to = mtx.m;
	const float *from = reinterpret_cast<const float*>(p.matrix);

	
	to[0] = from[12];
	to[1] = from[8];
	to[2] = from[4];
	to[3] = from[0];
	to[4] = from[13];
	to[5] = from[9];
	to[6] = from[5];
	to[7] = from[1];
	to[8] = from[14];
	to[9] = from[10];
	to[10] = from[6];
	to[11] = from[2];
	to[12] = from[15];
	to[13] = from[11];
	to[14] = from[7];
	to[15] = from[3];
	

	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uniform_location, &mtx);
}

/* TEXTURE API */


RID Rasterizer3DS::texture_create() {

	Texture *texture = memnew(Texture);
	ERR_FAIL_COND_V(!texture,RID());
	return texture_owner.make_rid( texture );

}

void Rasterizer3DS::texture_allocate(RID p_texture,int p_width, int p_height,Image::Format p_format,uint32_t p_flags) {


	Texture *texture = texture_owner.get( p_texture );
	ERR_FAIL_COND(!texture);
	C3D_TexInit(&texture->texture, p_width, p_height, GPU_RGBA8);
	texture->width=p_width;
	texture->height=p_height;
	texture->format=p_format;
	texture->flags=p_flags;
}

void Rasterizer3DS::texture_set_data(RID p_texture,const Image& p_image,VS::CubeMapSide p_cube_side) {

	Texture * texture = texture_owner.get(p_texture);

	ERR_FAIL_COND(!texture);
	ERR_FAIL_COND(texture->format != p_image.get_format() );

	texture->image[p_cube_side]=p_image;

	DVector<uint8_t> d = p_image.get_data();
	const u32* buffer = reinterpret_cast<const u32*>(d.read().ptr());
	u8 * gpuBuffer = (u8 *)linearAlloc(texture->width * texture->height * 4);
	
	u32 * src = (u32 *)buffer;
	u32 * dst = (u32 *)gpuBuffer;

	for (unsigned y = 0; y < texture->height; y++)
	{
		for (unsigned x = 0; x < texture->width; x++)
		{
			u32 clr = *src;
			*dst = __builtin_bswap32(clr);
		}
		dst += 4;
	}
	
	GSPGPU_FlushDataCache((u32 *)gpuBuffer, texture->width * texture->height * 4);

	C3D_SyncDisplayTransfer((u32 *)gpuBuffer, GX_BUFFER_DIM(texture->width, texture->height), (u32 *)texture->texture.data, GX_BUFFER_DIM(texture->width, texture->height), TEXTURE_TRANSFER_FLAGS);

	// gspWaitForPPF();
	C3D_TexSetFilter(&texture->texture, GPU_LINEAR, GPU_NEAREST);
	C3D_TexBind(0, &texture->texture);
	
	linearFree(gpuBuffer);
}

Image Rasterizer3DS::texture_get_data(RID p_texture,VS::CubeMapSide p_cube_side) const {

	Texture * texture = texture_owner.get(p_texture);

	ERR_FAIL_COND_V(!texture,Image());

	return texture->image[p_cube_side];
}

void Rasterizer3DS::texture_set_flags(RID p_texture,uint32_t p_flags) {

	Texture *texture = texture_owner.get( p_texture );
	ERR_FAIL_COND(!texture);
	uint32_t cube = texture->flags & VS::TEXTURE_FLAG_CUBEMAP;
	texture->flags=p_flags|cube; // can't remove a cube from being a cube

}
uint32_t Rasterizer3DS::texture_get_flags(RID p_texture) const {

	Texture * texture = texture_owner.get(p_texture);

	ERR_FAIL_COND_V(!texture,0);

	return texture->flags;

}
Image::Format Rasterizer3DS::texture_get_format(RID p_texture) const {

	Texture * texture = texture_owner.get(p_texture);

	ERR_FAIL_COND_V(!texture,Image::FORMAT_GRAYSCALE);

	return texture->format;
}
uint32_t Rasterizer3DS::texture_get_width(RID p_texture) const {

	Texture * texture = texture_owner.get(p_texture);

	ERR_FAIL_COND_V(!texture,0);

	return texture->width;
}
uint32_t Rasterizer3DS::texture_get_height(RID p_texture) const {

	Texture * texture = texture_owner.get(p_texture);

	ERR_FAIL_COND_V(!texture,0);

	return texture->height;
}

bool Rasterizer3DS::texture_has_alpha(RID p_texture) const {

	Texture * texture = texture_owner.get(p_texture);

	ERR_FAIL_COND_V(!texture,0);

	return false;

}

void Rasterizer3DS::texture_set_size_override(RID p_texture,int p_width, int p_height) {

	Texture * texture = texture_owner.get(p_texture);

	ERR_FAIL_COND(!texture);

	ERR_FAIL_COND(p_width<=0 || p_width>4096);
	ERR_FAIL_COND(p_height<=0 || p_height>4096);
	//real texture size is in alloc width and height
//	texture->width=p_width;
//	texture->height=p_height;

}

void Rasterizer3DS::texture_set_reload_hook(RID p_texture,ObjectID p_owner,const StringName& p_function) const {


}

static void _draw_primitive(int p_points, const Vector3 *p_vertices, const Vector3 *p_normals, const Color* p_colors, const Vector3 *p_uvs,const Plane *p_tangents=NULL,int p_instanced=1) {
	ERR_FAIL_COND(!p_vertices);
	ERR_FAIL_COND(p_points <1 || p_points>4);

	// int type = prim_type[p_points - 1];

	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, p_vertices, sizeof(Vector3), 2, 0x10);
	
	C3D_DrawArrays(GPU_TRIANGLE_FAN, 0, 4);
};

void Rasterizer3DS::_draw_textured_quad(const Rect2& p_rect, const Rect2& p_src_region, const Size2& p_tex_size,bool p_flip_h=false,bool p_flip_v=false ) {
	VertexArray dscoords[4]= {
		{Vector3( p_src_region.pos.x/p_tex_size.width,
		p_src_region.pos.y/p_tex_size.height, 0),
		Vector2( p_rect.pos.x, p_rect.pos.y )},
		
		
		{Vector3((p_src_region.pos.x+p_src_region.size.width)/p_tex_size.width,
		p_src_region.pos.y/p_tex_size.height, 0),
		Vector2( p_rect.pos.x+p_rect.size.width, p_rect.pos.y )},

		{Vector3( (p_src_region.pos.x+p_src_region.size.width)/p_tex_size.width,
		(p_src_region.pos.y+p_src_region.size.height)/p_tex_size.height, 0),
		Vector2( p_rect.pos.x+p_rect.size.width, p_rect.pos.y+p_rect.size.height )},

		{Vector3( p_src_region.pos.x/p_tex_size.width,
		(p_src_region.pos.y+p_src_region.size.height)/p_tex_size.height, 0),
		Vector2( p_rect.pos.x,p_rect.pos.y+p_rect.size.height )}
	};


	if (p_flip_h) {
		SWAP( dscoords[0], dscoords[1] );
		SWAP( dscoords[2], dscoords[3] );
	}
	if (p_flip_v) {
		SWAP( dscoords[1], dscoords[2] );
		SWAP( dscoords[0], dscoords[3] );
	}

	Vector3 coords[4]= {

	};
	/*
	void* vbo_data = linearAlloc(sizeof(vertex_list));
	memcpy(vbo_data, vertex_list, sizeof(vertex_list));
	// memcpy(vbo_data+sizeof(coords), texcoords, sizeof(texcoords));
	
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vbo_data, sizeof(vertex3ds), 3, 0x210);
	
	C3D_DrawArrays(GPU_TRIANGLES, 0, vertex_list_count);;*/

	
	// Mtx_Identity(&modelView);
	
	// Mtx_Translate(&modelView, 0.0, 0.0, -2.5, true);
	
	// C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView,  &modelView);

	// Draw the triangle directly
	
	C3D_ImmDrawBegin(GPU_TRIANGLE_STRIP);
		C3D_ImmSendAttrib(p_src_region.pos.x/p_tex_size.width, p_src_region.pos.y/p_tex_size.height, 0.5f, 0.0f); // v0=position
		C3D_ImmSendAttrib(p_rect.pos.x, p_rect.pos.y, 0.0f, 0.0f);     // v1=color

		C3D_ImmSendAttrib((p_src_region.pos.x+p_src_region.size.width)/p_tex_size.width, p_src_region.pos.y/p_tex_size.height, 0.5f, 0.0f);
		C3D_ImmSendAttrib(p_rect.pos.x+p_rect.size.width, p_rect.pos.y, 0.0f, 0.0f);

		C3D_ImmSendAttrib((p_src_region.pos.x+p_src_region.size.width)/p_tex_size.width, (p_src_region.pos.y+p_src_region.size.height)/p_tex_size.height, 0.5f, 0.0f);
		C3D_ImmSendAttrib(p_rect.pos.x+p_rect.size.width, p_rect.pos.y+p_rect.size.height, 0.0f, 0.0f);
		
		C3D_ImmSendAttrib(p_src_region.pos.x/p_tex_size.width, (p_src_region.pos.y+p_src_region.size.height)/p_tex_size.height, 0.5f, 0.0f);
		C3D_ImmSendAttrib(p_rect.pos.x, p_rect.pos.y+p_rect.size.height, 0.0f, 0.0f);
		
	C3D_ImmDrawEnd();
	// _draw_primitive(4,coords,0,0,texcoords);
}

/* SHADER API */

/* SHADER API */

RID Rasterizer3DS::shader_create(VS::ShaderMode p_mode) {

	Shader *shader = memnew( Shader );
	shader->mode=p_mode;
	shader->fragment_line=0;
	shader->vertex_line=0;
	shader->light_line=0;
	RID rid = shader_owner.make_rid(shader);

	return rid;

}



void Rasterizer3DS::shader_set_mode(RID p_shader,VS::ShaderMode p_mode) {

	ERR_FAIL_INDEX(p_mode,3);
	Shader *shader=shader_owner.get(p_shader);
	ERR_FAIL_COND(!shader);
	shader->mode=p_mode;

}
VS::ShaderMode Rasterizer3DS::shader_get_mode(RID p_shader) const {

	Shader *shader=shader_owner.get(p_shader);
	ERR_FAIL_COND_V(!shader,VS::SHADER_MATERIAL);
	return shader->mode;
}

void Rasterizer3DS::shader_set_code(RID p_shader, const String& p_vertex, const String& p_fragment,const String& p_light,int p_vertex_ofs,int p_fragment_ofs,int p_light_ofs) {


	Shader *shader=shader_owner.get(p_shader);
	ERR_FAIL_COND(!shader);
	shader->fragment_code=p_fragment;
	shader->vertex_code=p_vertex;
	shader->light_code=p_light;
	shader->fragment_line=p_fragment_ofs;
	shader->vertex_line=p_vertex_ofs;
	shader->light_line=p_vertex_ofs;

}


String Rasterizer3DS::shader_get_vertex_code(RID p_shader) const {

	Shader *shader=shader_owner.get(p_shader);
	ERR_FAIL_COND_V(!shader,String());
	return shader->vertex_code;

}

String Rasterizer3DS::shader_get_fragment_code(RID p_shader) const {

	Shader *shader=shader_owner.get(p_shader);
	ERR_FAIL_COND_V(!shader,String());
	return shader->fragment_code;

}

String Rasterizer3DS::shader_get_light_code(RID p_shader) const {

	Shader *shader=shader_owner.get(p_shader);
	ERR_FAIL_COND_V(!shader,String());
	return shader->light_code;

}

void Rasterizer3DS::shader_get_param_list(RID p_shader, List<PropertyInfo> *p_param_list) const {

	Shader *shader=shader_owner.get(p_shader);
	ERR_FAIL_COND(!shader);

}

/* COMMON MATERIAL API */


RID Rasterizer3DS::material_create() {

	return material_owner.make_rid( memnew( Material ) );
}

void Rasterizer3DS::material_set_shader(RID p_material, RID p_shader) {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND(!material);
	material->shader=p_shader;

}

RID Rasterizer3DS::material_get_shader(RID p_material) const {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND_V(!material,RID());
	return material->shader;
}

void Rasterizer3DS::material_set_param(RID p_material, const StringName& p_param, const Variant& p_value) {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND(!material);

	if (p_value.get_type()==Variant::NIL)
		material->shader_params.erase(p_param);
	else
		material->shader_params[p_param]=p_value;
}
Variant Rasterizer3DS::material_get_param(RID p_material, const StringName& p_param) const {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND_V(!material,Variant());

	if (material->shader_params.has(p_param))
		return material->shader_params[p_param];
	else
		return Variant();
}


void Rasterizer3DS::material_set_flag(RID p_material, VS::MaterialFlag p_flag,bool p_enabled) {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND(!material);
	ERR_FAIL_INDEX(p_flag,VS::MATERIAL_FLAG_MAX);
	material->flags[p_flag]=p_enabled;

}
bool Rasterizer3DS::material_get_flag(RID p_material,VS::MaterialFlag p_flag) const {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND_V(!material,false);
	ERR_FAIL_INDEX_V(p_flag,VS::MATERIAL_FLAG_MAX,false);
	return material->flags[p_flag];


}

void Rasterizer3DS::material_set_depth_draw_mode(RID p_material, VS::MaterialDepthDrawMode p_mode) {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND(!material);
	material->depth_draw_mode=p_mode;
}

VS::MaterialDepthDrawMode Rasterizer3DS::material_get_depth_draw_mode(RID p_material) const{

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND_V(!material,VS::MATERIAL_DEPTH_DRAW_ALWAYS);
	return material->depth_draw_mode;

}


void Rasterizer3DS::material_set_blend_mode(RID p_material,VS::MaterialBlendMode p_mode) {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND(!material);
	material->blend_mode=p_mode;

}
VS::MaterialBlendMode Rasterizer3DS::material_get_blend_mode(RID p_material) const {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND_V(!material,VS::MATERIAL_BLEND_MODE_ADD);
	return material->blend_mode;
}

void Rasterizer3DS::material_set_line_width(RID p_material,float p_line_width) {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND(!material);
	material->line_width=p_line_width;

}
float Rasterizer3DS::material_get_line_width(RID p_material) const {

	Material *material = material_owner.get(p_material);
	ERR_FAIL_COND_V(!material,0);

	return material->line_width;
}

/* MESH API */


RID Rasterizer3DS::mesh_create() {


	return mesh_owner.make_rid( memnew( Mesh ) );
}


void Rasterizer3DS::mesh_add_surface(RID p_mesh,VS::PrimitiveType p_primitive,const Array& p_arrays,const Array& p_blend_shapes,bool p_alpha_sort) {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND(!mesh);

	ERR_FAIL_INDEX( p_primitive, VS::PRIMITIVE_MAX );
	ERR_FAIL_COND(p_arrays.size()!=VS::ARRAY_MAX);

	Surface s;


	s.format=0;

	for(int i=0;i<p_arrays.size();i++) {

		if (p_arrays[i].get_type()==Variant::NIL)
			continue;

		s.format|=(1<<i);

		if (i==VS::ARRAY_VERTEX) {

			Vector3Array v = p_arrays[i];
			int len = v.size();
			ERR_FAIL_COND(len==0);
			Vector3Array::Read r = v.read();


			for(int i=0;i<len;i++) {

				if (i==0)
					s.aabb.pos=r[0];
				else
					s.aabb.expand_to(r[i]);
			}

		}
	}

	ERR_FAIL_COND((s.format&VS::ARRAY_FORMAT_VERTEX)==0); // mandatory

	s.data=p_arrays;
	s.morph_data=p_blend_shapes;
	s.primitive=p_primitive;
	s.alpha_sort=p_alpha_sort;
	s.morph_target_count=mesh->morph_target_count;
	s.morph_format=s.format;


	Surface *surface = memnew( Surface );
	*surface=s;

	mesh->surfaces.push_back(surface);


}



void Rasterizer3DS::mesh_add_custom_surface(RID p_mesh,const Variant& p_dat) {

	ERR_EXPLAIN("Dummy Rasterizer does not support custom surfaces. Running on wrong platform?");
	ERR_FAIL_V();
}

Array Rasterizer3DS::mesh_get_surface_arrays(RID p_mesh,int p_surface) const {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,Array());
	ERR_FAIL_INDEX_V(p_surface, mesh->surfaces.size(), Array() );
	Surface *surface = mesh->surfaces[p_surface];
	ERR_FAIL_COND_V( !surface, Array() );

	return surface->data;


}
Array Rasterizer3DS::mesh_get_surface_morph_arrays(RID p_mesh,int p_surface) const{

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,Array());
	ERR_FAIL_INDEX_V(p_surface, mesh->surfaces.size(), Array() );
	Surface *surface = mesh->surfaces[p_surface];
	ERR_FAIL_COND_V( !surface, Array() );

	return surface->morph_data;

}


void Rasterizer3DS::mesh_set_morph_target_count(RID p_mesh,int p_amount) {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND(!mesh);
	ERR_FAIL_COND( mesh->surfaces.size()!=0 );

	mesh->morph_target_count=p_amount;

}

int Rasterizer3DS::mesh_get_morph_target_count(RID p_mesh) const{

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,-1);

	return mesh->morph_target_count;

}

void Rasterizer3DS::mesh_set_morph_target_mode(RID p_mesh,VS::MorphTargetMode p_mode) {

	ERR_FAIL_INDEX(p_mode,2);
	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND(!mesh);

	mesh->morph_target_mode=p_mode;

}

VS::MorphTargetMode Rasterizer3DS::mesh_get_morph_target_mode(RID p_mesh) const {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,VS::MORPH_MODE_NORMALIZED);

	return mesh->morph_target_mode;

}



void Rasterizer3DS::mesh_surface_set_material(RID p_mesh, int p_surface, RID p_material,bool p_owned) {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND(!mesh);
	ERR_FAIL_INDEX(p_surface, mesh->surfaces.size() );
	Surface *surface = mesh->surfaces[p_surface];
	ERR_FAIL_COND( !surface);

	if (surface->material_owned && surface->material.is_valid())
		free(surface->material);

	surface->material_owned=p_owned;
	surface->material=p_material;
}

RID Rasterizer3DS::mesh_surface_get_material(RID p_mesh, int p_surface) const {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,RID());
	ERR_FAIL_INDEX_V(p_surface, mesh->surfaces.size(), RID() );
	Surface *surface = mesh->surfaces[p_surface];
	ERR_FAIL_COND_V( !surface, RID() );

	return surface->material;
}

int Rasterizer3DS::mesh_surface_get_array_len(RID p_mesh, int p_surface) const {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,-1);
	ERR_FAIL_INDEX_V(p_surface, mesh->surfaces.size(), -1 );
	Surface *surface = mesh->surfaces[p_surface];
	ERR_FAIL_COND_V( !surface, -1 );

	Vector3Array arr = surface->data[VS::ARRAY_VERTEX];
	return arr.size();

}

int Rasterizer3DS::mesh_surface_get_array_index_len(RID p_mesh, int p_surface) const {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,-1);
	ERR_FAIL_INDEX_V(p_surface, mesh->surfaces.size(), -1 );
	Surface *surface = mesh->surfaces[p_surface];
	ERR_FAIL_COND_V( !surface, -1 );

	IntArray arr = surface->data[VS::ARRAY_INDEX];
	return arr.size();

}
uint32_t Rasterizer3DS::mesh_surface_get_format(RID p_mesh, int p_surface) const {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,0);
	ERR_FAIL_INDEX_V(p_surface, mesh->surfaces.size(), 0 );
	Surface *surface = mesh->surfaces[p_surface];
	ERR_FAIL_COND_V( !surface, 0 );

	return surface->format;
}
VS::PrimitiveType Rasterizer3DS::mesh_surface_get_primitive_type(RID p_mesh, int p_surface) const {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,VS::PRIMITIVE_POINTS);
	ERR_FAIL_INDEX_V(p_surface, mesh->surfaces.size(), VS::PRIMITIVE_POINTS );
	Surface *surface = mesh->surfaces[p_surface];
	ERR_FAIL_COND_V( !surface, VS::PRIMITIVE_POINTS );

	return surface->primitive;
}

void Rasterizer3DS::mesh_remove_surface(RID p_mesh,int p_index) {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND(!mesh);
	ERR_FAIL_INDEX(p_index, mesh->surfaces.size() );
	Surface *surface = mesh->surfaces[p_index];
	ERR_FAIL_COND( !surface);

	memdelete( mesh->surfaces[p_index] );
	mesh->surfaces.remove(p_index);

}
int Rasterizer3DS::mesh_get_surface_count(RID p_mesh) const {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,-1);

	return mesh->surfaces.size();
}

AABB Rasterizer3DS::mesh_get_aabb(RID p_mesh,RID p_skeleton) const {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,AABB());

	AABB aabb;

	for (int i=0;i<mesh->surfaces.size();i++) {

		if (i==0)
			aabb=mesh->surfaces[i]->aabb;
		else
			aabb.merge_with(mesh->surfaces[i]->aabb);
	}

	return aabb;
}

void Rasterizer3DS::mesh_set_custom_aabb(RID p_mesh,const AABB& p_aabb) {

	Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND(!mesh);

	mesh->custom_aabb=p_aabb;
}

AABB Rasterizer3DS::mesh_get_custom_aabb(RID p_mesh) const {

	const Mesh *mesh = mesh_owner.get( p_mesh );
	ERR_FAIL_COND_V(!mesh,AABB());

	return mesh->custom_aabb;

}

/* MULTIMESH API */

RID Rasterizer3DS::multimesh_create() {

	return multimesh_owner.make_rid( memnew( MultiMesh ));
}

void Rasterizer3DS::multimesh_set_instance_count(RID p_multimesh,int p_count) {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND(!multimesh);

	multimesh->elements.clear(); // make sure to delete everything, so it "fails" in all implementations
	multimesh->elements.resize(p_count);

}
int Rasterizer3DS::multimesh_get_instance_count(RID p_multimesh) const {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND_V(!multimesh,-1);

	return multimesh->elements.size();
}

void Rasterizer3DS::multimesh_set_mesh(RID p_multimesh,RID p_mesh) {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND(!multimesh);

	multimesh->mesh=p_mesh;

}
void Rasterizer3DS::multimesh_set_aabb(RID p_multimesh,const AABB& p_aabb) {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND(!multimesh);
	multimesh->aabb=p_aabb;
}
void Rasterizer3DS::multimesh_instance_set_transform(RID p_multimesh,int p_index,const Transform& p_transform) {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND(!multimesh);
	ERR_FAIL_INDEX(p_index,multimesh->elements.size());
	multimesh->elements[p_index].xform=p_transform;

}
void Rasterizer3DS::multimesh_instance_set_color(RID p_multimesh,int p_index,const Color& p_color) {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND(!multimesh)
	ERR_FAIL_INDEX(p_index,multimesh->elements.size());
	multimesh->elements[p_index].color=p_color;

}

RID Rasterizer3DS::multimesh_get_mesh(RID p_multimesh) const {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND_V(!multimesh,RID());

	return multimesh->mesh;
}
AABB Rasterizer3DS::multimesh_get_aabb(RID p_multimesh) const {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND_V(!multimesh,AABB());

	return multimesh->aabb;
}

Transform Rasterizer3DS::multimesh_instance_get_transform(RID p_multimesh,int p_index) const {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND_V(!multimesh,Transform());

	ERR_FAIL_INDEX_V(p_index,multimesh->elements.size(),Transform());

	return multimesh->elements[p_index].xform;

}
Color Rasterizer3DS::multimesh_instance_get_color(RID p_multimesh,int p_index) const {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND_V(!multimesh,Color());
	ERR_FAIL_INDEX_V(p_index,multimesh->elements.size(),Color());

	return multimesh->elements[p_index].color;
}

void Rasterizer3DS::multimesh_set_visible_instances(RID p_multimesh,int p_visible) {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND(!multimesh);
	multimesh->visible=p_visible;

}

int Rasterizer3DS::multimesh_get_visible_instances(RID p_multimesh) const {

	MultiMesh *multimesh = multimesh_owner.get(p_multimesh);
	ERR_FAIL_COND_V(!multimesh,-1);
	return multimesh->visible;

}

/* IMMEDIATE API */


RID Rasterizer3DS::immediate_create() {

	Immediate *im = memnew( Immediate );
	return immediate_owner.make_rid(im);

}

void Rasterizer3DS::immediate_begin(RID p_immediate,VS::PrimitiveType p_rimitive,RID p_texture){


}
void Rasterizer3DS::immediate_vertex(RID p_immediate,const Vector3& p_vertex){


}
void Rasterizer3DS::immediate_normal(RID p_immediate,const Vector3& p_normal){


}
void Rasterizer3DS::immediate_tangent(RID p_immediate,const Plane& p_tangent){


}
void Rasterizer3DS::immediate_color(RID p_immediate,const Color& p_color){


}
void Rasterizer3DS::immediate_uv(RID p_immediate,const Vector2& tex_uv){


}
void Rasterizer3DS::immediate_uv2(RID p_immediate,const Vector2& tex_uv){


}

void Rasterizer3DS::immediate_end(RID p_immediate){


}
void Rasterizer3DS::immediate_clear(RID p_immediate) {


}

AABB Rasterizer3DS::immediate_get_aabb(RID p_immediate) const {

	return AABB(Vector3(-1,-1,-1),Vector3(2,2,2));
}

void Rasterizer3DS::immediate_set_material(RID p_immediate,RID p_material) {

	Immediate *im = immediate_owner.get(p_immediate);
	ERR_FAIL_COND(!im);
	im->material=p_material;

}

RID Rasterizer3DS::immediate_get_material(RID p_immediate) const {

	const Immediate *im = immediate_owner.get(p_immediate);
	ERR_FAIL_COND_V(!im,RID());
	return im->material;

}

/* PARTICLES API */

RID Rasterizer3DS::particles_create() {

	Particles *particles = memnew( Particles );
	ERR_FAIL_COND_V(!particles,RID());
	return particles_owner.make_rid(particles);
}

void Rasterizer3DS::particles_set_amount(RID p_particles, int p_amount) {

	ERR_FAIL_COND(p_amount<1);
	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	particles->data.amount=p_amount;

}

int Rasterizer3DS::particles_get_amount(RID p_particles) const {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,-1);
	return particles->data.amount;

}

void Rasterizer3DS::particles_set_emitting(RID p_particles, bool p_emitting) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	particles->data.emitting=p_emitting;;

}
bool Rasterizer3DS::particles_is_emitting(RID p_particles) const {

	const Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,false);
	return particles->data.emitting;

}

void Rasterizer3DS::particles_set_visibility_aabb(RID p_particles, const AABB& p_visibility) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	particles->data.visibility_aabb=p_visibility;

}

void Rasterizer3DS::particles_set_emission_half_extents(RID p_particles, const Vector3& p_half_extents) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);

	particles->data.emission_half_extents=p_half_extents;
}
Vector3 Rasterizer3DS::particles_get_emission_half_extents(RID p_particles) const {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,Vector3());

	return particles->data.emission_half_extents;
}

void Rasterizer3DS::particles_set_emission_base_velocity(RID p_particles, const Vector3& p_base_velocity) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);

	particles->data.emission_base_velocity=p_base_velocity;
}

Vector3 Rasterizer3DS::particles_get_emission_base_velocity(RID p_particles) const {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,Vector3());

	return particles->data.emission_base_velocity;
}


void Rasterizer3DS::particles_set_emission_points(RID p_particles, const DVector<Vector3>& p_points) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);

	particles->data.emission_points=p_points;
}

DVector<Vector3> Rasterizer3DS::particles_get_emission_points(RID p_particles) const {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,DVector<Vector3>());

	return particles->data.emission_points;

}

void Rasterizer3DS::particles_set_gravity_normal(RID p_particles, const Vector3& p_normal) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);

	particles->data.gravity_normal=p_normal;

}
Vector3 Rasterizer3DS::particles_get_gravity_normal(RID p_particles) const {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,Vector3());

	return particles->data.gravity_normal;
}


AABB Rasterizer3DS::particles_get_visibility_aabb(RID p_particles) const {

	const Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,AABB());
	return particles->data.visibility_aabb;

}

void Rasterizer3DS::particles_set_variable(RID p_particles, VS::ParticleVariable p_variable,float p_value) {

	ERR_FAIL_INDEX(p_variable,VS::PARTICLE_VAR_MAX);

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	particles->data.particle_vars[p_variable]=p_value;

}
float Rasterizer3DS::particles_get_variable(RID p_particles, VS::ParticleVariable p_variable) const {

	const Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,-1);
	return particles->data.particle_vars[p_variable];
}

void Rasterizer3DS::particles_set_randomness(RID p_particles, VS::ParticleVariable p_variable,float p_randomness) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	particles->data.particle_randomness[p_variable]=p_randomness;

}
float Rasterizer3DS::particles_get_randomness(RID p_particles, VS::ParticleVariable p_variable) const {

	const Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,-1);
	return particles->data.particle_randomness[p_variable];

}

void Rasterizer3DS::particles_set_color_phases(RID p_particles, int p_phases) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	ERR_FAIL_COND( p_phases<0 || p_phases>VS::MAX_PARTICLE_COLOR_PHASES );
	particles->data.color_phase_count=p_phases;

}
int Rasterizer3DS::particles_get_color_phases(RID p_particles) const {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,-1);
	return particles->data.color_phase_count;
}


void Rasterizer3DS::particles_set_color_phase_pos(RID p_particles, int p_phase, float p_pos) {

	ERR_FAIL_INDEX(p_phase, VS::MAX_PARTICLE_COLOR_PHASES);
	if (p_pos<0.0)
		p_pos=0.0;
	if (p_pos>1.0)
		p_pos=1.0;

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	particles->data.color_phases[p_phase].pos=p_pos;

}
float Rasterizer3DS::particles_get_color_phase_pos(RID p_particles, int p_phase) const {

	ERR_FAIL_INDEX_V(p_phase, VS::MAX_PARTICLE_COLOR_PHASES, -1.0);

	const Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,-1);
	return particles->data.color_phases[p_phase].pos;

}

void Rasterizer3DS::particles_set_color_phase_color(RID p_particles, int p_phase, const Color& p_color) {

	ERR_FAIL_INDEX(p_phase, VS::MAX_PARTICLE_COLOR_PHASES);
	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	particles->data.color_phases[p_phase].color=p_color;

	//update alpha
	particles->has_alpha=false;
	for(int i=0;i<VS::MAX_PARTICLE_COLOR_PHASES;i++) {
		if (particles->data.color_phases[i].color.a<0.99)
			particles->has_alpha=true;
	}

}

Color Rasterizer3DS::particles_get_color_phase_color(RID p_particles, int p_phase) const {

	ERR_FAIL_INDEX_V(p_phase, VS::MAX_PARTICLE_COLOR_PHASES, Color());

	const Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,Color());
	return particles->data.color_phases[p_phase].color;

}

void Rasterizer3DS::particles_set_attractors(RID p_particles, int p_attractors) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	ERR_FAIL_COND( p_attractors<0 || p_attractors>VisualServer::MAX_PARTICLE_ATTRACTORS );
	particles->data.attractor_count=p_attractors;

}
int Rasterizer3DS::particles_get_attractors(RID p_particles) const {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,-1);
	return particles->data.attractor_count;
}

void Rasterizer3DS::particles_set_attractor_pos(RID p_particles, int p_attractor, const Vector3& p_pos) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	ERR_FAIL_INDEX(p_attractor,particles->data.attractor_count);
	particles->data.attractors[p_attractor].pos=p_pos;;
}
Vector3 Rasterizer3DS::particles_get_attractor_pos(RID p_particles,int p_attractor) const {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,Vector3());
	ERR_FAIL_INDEX_V(p_attractor,particles->data.attractor_count,Vector3());
	return particles->data.attractors[p_attractor].pos;
}

void Rasterizer3DS::particles_set_attractor_strength(RID p_particles, int p_attractor, float p_force) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	ERR_FAIL_INDEX(p_attractor,particles->data.attractor_count);
	particles->data.attractors[p_attractor].force=p_force;
}

float Rasterizer3DS::particles_get_attractor_strength(RID p_particles,int p_attractor) const {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,0);
	ERR_FAIL_INDEX_V(p_attractor,particles->data.attractor_count,0);
	return particles->data.attractors[p_attractor].force;
}

void Rasterizer3DS::particles_set_material(RID p_particles, RID p_material,bool p_owned) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	if (particles->material_owned && particles->material.is_valid())
		free(particles->material);

	particles->material_owned=p_owned;

	particles->material=p_material;

}
RID Rasterizer3DS::particles_get_material(RID p_particles) const {

	const Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,RID());
	return particles->material;

}

void Rasterizer3DS::particles_set_use_local_coordinates(RID p_particles, bool p_enable) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	particles->data.local_coordinates=p_enable;

}

bool Rasterizer3DS::particles_is_using_local_coordinates(RID p_particles) const {

	const Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,false);
	return particles->data.local_coordinates;
}
bool Rasterizer3DS::particles_has_height_from_velocity(RID p_particles) const {

	const Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,false);
	return particles->data.height_from_velocity;
}

void Rasterizer3DS::particles_set_height_from_velocity(RID p_particles, bool p_enable) {

	Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND(!particles);
	particles->data.height_from_velocity=p_enable;

}

AABB Rasterizer3DS::particles_get_aabb(RID p_particles) const {

	const Particles* particles = particles_owner.get( p_particles );
	ERR_FAIL_COND_V(!particles,AABB());
	return particles->data.visibility_aabb;
}

/* SKELETON API */

RID Rasterizer3DS::skeleton_create() {

	Skeleton *skeleton = memnew( Skeleton );
	ERR_FAIL_COND_V(!skeleton,RID());
	return skeleton_owner.make_rid( skeleton );
}
void Rasterizer3DS::skeleton_resize(RID p_skeleton,int p_bones) {

	Skeleton *skeleton = skeleton_owner.get( p_skeleton );
	ERR_FAIL_COND(!skeleton);
	if (p_bones == skeleton->bones.size()) {
		return;
	};

	skeleton->bones.resize(p_bones);

}
int Rasterizer3DS::skeleton_get_bone_count(RID p_skeleton) const {

	Skeleton *skeleton = skeleton_owner.get( p_skeleton );
	ERR_FAIL_COND_V(!skeleton, -1);
	return skeleton->bones.size();
}
void Rasterizer3DS::skeleton_bone_set_transform(RID p_skeleton,int p_bone, const Transform& p_transform) {

	Skeleton *skeleton = skeleton_owner.get( p_skeleton );
	ERR_FAIL_COND(!skeleton);
	ERR_FAIL_INDEX( p_bone, skeleton->bones.size() );

	skeleton->bones[p_bone] = p_transform;
}

Transform Rasterizer3DS::skeleton_bone_get_transform(RID p_skeleton,int p_bone) {

	Skeleton *skeleton = skeleton_owner.get( p_skeleton );
	ERR_FAIL_COND_V(!skeleton, Transform());
	ERR_FAIL_INDEX_V( p_bone, skeleton->bones.size(), Transform() );

	// something
	return skeleton->bones[p_bone];
}


/* LIGHT API */

RID Rasterizer3DS::light_create(VS::LightType p_type) {

	Light *light = memnew( Light );
	light->type=p_type;
	return light_owner.make_rid(light);
}

VS::LightType Rasterizer3DS::light_get_type(RID p_light) const {

	Light *light = light_owner.get(p_light);
	ERR_FAIL_COND_V(!light,VS::LIGHT_OMNI);
	return light->type;
}

void Rasterizer3DS::light_set_color(RID p_light,VS::LightColor p_type, const Color& p_color) {

	Light *light = light_owner.get(p_light);
	ERR_FAIL_COND(!light);
	ERR_FAIL_INDEX( p_type, 3 );
	light->colors[p_type]=p_color;
}
Color Rasterizer3DS::light_get_color(RID p_light,VS::LightColor p_type) const {

	Light *light = light_owner.get(p_light);
	ERR_FAIL_COND_V(!light, Color());
	ERR_FAIL_INDEX_V( p_type, 3, Color() );
	return light->colors[p_type];
}

void Rasterizer3DS::light_set_shadow(RID p_light,bool p_enabled) {

	Light *light = light_owner.get(p_light);
	ERR_FAIL_COND(!light);
	light->shadow_enabled=p_enabled;
}

bool Rasterizer3DS::light_has_shadow(RID p_light) const {

	Light *light = light_owner.get(p_light);
	ERR_FAIL_COND_V(!light,false);
	return light->shadow_enabled;
}

void Rasterizer3DS::light_set_volumetric(RID p_light,bool p_enabled) {

	Light *light = light_owner.get(p_light);
	ERR_FAIL_COND(!light);
	light->volumetric_enabled=p_enabled;

}
bool Rasterizer3DS::light_is_volumetric(RID p_light) const {

	Light *light = light_owner.get(p_light);
	ERR_FAIL_COND_V(!light,false);
	return light->volumetric_enabled;
}

void Rasterizer3DS::light_set_projector(RID p_light,RID p_texture) {

	Light *light = light_owner.get(p_light);
	ERR_FAIL_COND(!light);
	light->projector=p_texture;
}
RID Rasterizer3DS::light_get_projector(RID p_light) const {

	Light *light = light_owner.get(p_light);
	ERR_FAIL_COND_V(!light,RID());
	return light->projector;
}

void Rasterizer3DS::light_set_var(RID p_light, VS::LightParam p_var, float p_value) {

	Light * light = light_owner.get( p_light );
	ERR_FAIL_COND(!light);
	ERR_FAIL_INDEX( p_var, VS::LIGHT_PARAM_MAX );

	light->vars[p_var]=p_value;
}
float Rasterizer3DS::light_get_var(RID p_light, VS::LightParam p_var) const {

	Light * light = light_owner.get( p_light );
	ERR_FAIL_COND_V(!light,0);

	ERR_FAIL_INDEX_V( p_var, VS::LIGHT_PARAM_MAX,0 );

	return light->vars[p_var];
}

void Rasterizer3DS::light_set_operator(RID p_light,VS::LightOp p_op) {

	Light * light = light_owner.get( p_light );
	ERR_FAIL_COND(!light);


};

VS::LightOp Rasterizer3DS::light_get_operator(RID p_light) const {

	return VS::LightOp(0);
};

void Rasterizer3DS::light_omni_set_shadow_mode(RID p_light,VS::LightOmniShadowMode p_mode) {


}

VS::LightOmniShadowMode Rasterizer3DS::light_omni_get_shadow_mode(RID p_light) const{

	return VS::LightOmniShadowMode(0);
}

void Rasterizer3DS::light_directional_set_shadow_mode(RID p_light,VS::LightDirectionalShadowMode p_mode) {


}

VS::LightDirectionalShadowMode Rasterizer3DS::light_directional_get_shadow_mode(RID p_light) const {

	return VS::LIGHT_DIRECTIONAL_SHADOW_ORTHOGONAL;
}

void Rasterizer3DS::light_directional_set_shadow_param(RID p_light,VS::LightDirectionalShadowParam p_param, float p_value) {


}

float Rasterizer3DS::light_directional_get_shadow_param(RID p_light,VS::LightDirectionalShadowParam p_param) const {

	return 0;
}


AABB Rasterizer3DS::light_get_aabb(RID p_light) const {

	Light *light = light_owner.get( p_light );
	ERR_FAIL_COND_V(!light,AABB());

	switch( light->type ) {

		case VS::LIGHT_SPOT: {

			float len=light->vars[VS::LIGHT_PARAM_RADIUS];
			float size=Math::tan(Math::deg2rad(light->vars[VS::LIGHT_PARAM_SPOT_ANGLE]))*len;
			return AABB( Vector3( -size,-size,-len ), Vector3( size*2, size*2, len ) );
		} break;
		case VS::LIGHT_OMNI: {

			float r = light->vars[VS::LIGHT_PARAM_RADIUS];
			return AABB( -Vector3(r,r,r), Vector3(r,r,r)*2 );
		} break;
		case VS::LIGHT_DIRECTIONAL: {

			return AABB();
		} break;
		default: {}
	}

	ERR_FAIL_V( AABB() );
}


RID Rasterizer3DS::light_instance_create(RID p_light) {

	Light *light = light_owner.get( p_light );
	ERR_FAIL_COND_V(!light, RID());

	LightInstance *light_instance = memnew( LightInstance );

	light_instance->light=p_light;
	light_instance->base=light;


	return light_instance_owner.make_rid( light_instance );
}
void Rasterizer3DS::light_instance_set_transform(RID p_light_instance,const Transform& p_transform) {

	LightInstance *lighti = light_instance_owner.get( p_light_instance );
	ERR_FAIL_COND(!lighti);
	lighti->transform=p_transform;

}

bool Rasterizer3DS::light_instance_has_shadow(RID p_light_instance) const {

	return false;

}


bool Rasterizer3DS::light_instance_assign_shadow(RID p_light_instance) {

	return false;

}


Rasterizer::ShadowType Rasterizer3DS::light_instance_get_shadow_type(RID p_light_instance) const {

	LightInstance *lighti = light_instance_owner.get( p_light_instance );
	ERR_FAIL_COND_V(!lighti,Rasterizer::SHADOW_NONE);

	switch(lighti->base->type) {

		case VS::LIGHT_DIRECTIONAL: return SHADOW_PSM; break;
		case VS::LIGHT_OMNI: return SHADOW_DUAL_PARABOLOID; break;
		case VS::LIGHT_SPOT: return SHADOW_SIMPLE; break;
	}

	return Rasterizer::SHADOW_NONE;
}

Rasterizer::ShadowType Rasterizer3DS::light_instance_get_shadow_type(RID p_light_instance,bool p_far) const {

	return SHADOW_NONE;
}
void Rasterizer3DS::light_instance_set_shadow_transform(RID p_light_instance, int p_index, const CameraMatrix& p_camera, const Transform& p_transform, float p_split_near,float p_split_far) {


}

int Rasterizer3DS::light_instance_get_shadow_passes(RID p_light_instance) const {

	return 0;
}

bool Rasterizer3DS::light_instance_get_pssm_shadow_overlap(RID p_light_instance) const {

	return false;
}


void Rasterizer3DS::light_instance_set_custom_transform(RID p_light_instance, int p_index, const CameraMatrix& p_camera, const Transform& p_transform, float p_split_near,float p_split_far) {

	LightInstance *lighti = light_instance_owner.get( p_light_instance );
	ERR_FAIL_COND(!lighti);

	ERR_FAIL_COND(lighti->base->type!=VS::LIGHT_DIRECTIONAL);
	ERR_FAIL_INDEX(p_index,1);

	lighti->custom_projection=p_camera;
	lighti->custom_transform=p_transform;

}
void Rasterizer3DS::shadow_clear_near() {


}

bool Rasterizer3DS::shadow_allocate_near(RID p_light) {

	return false;
}

bool Rasterizer3DS::shadow_allocate_far(RID p_light) {

	return false;
}

/* PARTICLES INSTANCE */

RID Rasterizer3DS::particles_instance_create(RID p_particles) {

	ERR_FAIL_COND_V(!particles_owner.owns(p_particles),RID());
	ParticlesInstance *particles_instance = memnew( ParticlesInstance );
	ERR_FAIL_COND_V(!particles_instance, RID() );
	particles_instance->particles=p_particles;
	return particles_instance_owner.make_rid(particles_instance);
}

void Rasterizer3DS::particles_instance_set_transform(RID p_particles_instance,const Transform& p_transform) {

	ParticlesInstance *particles_instance=particles_instance_owner.get(p_particles_instance);
	ERR_FAIL_COND(!particles_instance);
	particles_instance->transform=p_transform;
}


/* RENDER API */
/* all calls (inside begin/end shadow) are always warranted to be in the following order: */


RID Rasterizer3DS::viewport_data_create() {

	return RID();
}

RID Rasterizer3DS::render_target_create(){

	return RID();

}
void Rasterizer3DS::render_target_set_size(RID p_render_target, int p_width, int p_height){


}
RID Rasterizer3DS::render_target_get_texture(RID p_render_target) const{

	return RID();

}
bool Rasterizer3DS::render_target_renedered_in_frame(RID p_render_target){

	return false;
}


void Rasterizer3DS::begin_frame() {
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C3D_RenderTargetClear(top_rt->target, C3D_CLEAR_ALL, clear_color_top, 0);
	C3D_FrameDrawOn(top_rt->target);
}

void Rasterizer3DS::capture_viewport(Image* r_capture) {


}


void Rasterizer3DS::clear_viewport(const Color& p_color) {

	clear_color_top = convert_color(&p_color);
};

void Rasterizer3DS::set_viewport(const VS::ViewportRect& p_viewport) {



}

void Rasterizer3DS::set_render_target(RID p_render_target, bool p_transparent_bg, bool p_vflip) {


}


void Rasterizer3DS::begin_scene(RID p_viewport_data,RID p_env,VS::ScenarioDebugMode p_debug) {
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_FRAGMENT_PRIMARY_COLOR, GPU_FRAGMENT_SECONDARY_COLOR, (GPU_TEVSRC)0);
	// C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_ADD);
	
	C3D_TexBind(0, NULL);

	C3D_BindProgram(&program);
	_set_uniform(uLoc_modelView, Matrix32());
};

void Rasterizer3DS::begin_shadow_map( RID p_light_instance, int p_shadow_pass ) {

}

void Rasterizer3DS::set_camera(const Transform& p_world,const CameraMatrix& p_projection) {

	_set_uniform(uLoc_projection, p_projection);
}

void Rasterizer3DS::add_light( RID p_light_instance ) {



}




void Rasterizer3DS::add_mesh( const RID& p_mesh, const InstanceData *p_data) {


}

void Rasterizer3DS::add_multimesh( const RID& p_multimesh, const InstanceData *p_data){




}

void Rasterizer3DS::add_particles( const RID& p_particle_instance, const InstanceData *p_data){



}



void Rasterizer3DS::end_scene() {
	// C3D_Flush();

}
void Rasterizer3DS::end_shadow_map() {

}


void Rasterizer3DS::end_frame() {

	C3D_FrameEnd(0);
	C3D_FrameBufTransfer(&top_rt->target->frameBuf, top_rt->target->screen, top_rt->target->side, top_rt->target->transferFlags);
	gfxSwapBuffersGpu();
}

/* CANVAS API */


void Rasterizer3DS::canvas_begin() {
	// C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 3); // v1=color
	
	Mtx_OrthoTilt(&projection, 0.0, 800.0, 480.0, 0.0, 0.0, 1.0, true);
	
	C3D_BindProgram(&program);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
}
void Rasterizer3DS::canvas_disable_blending() {



}

void Rasterizer3DS::canvas_set_opacity(float p_opacity) {


}

void Rasterizer3DS::canvas_set_blend_mode(VS::MaterialBlendMode p_mode) {


}


void Rasterizer3DS::canvas_begin_rect(const Matrix32& p_transform) {
	_set_uniform(uLoc_modelView, p_transform);
}

void Rasterizer3DS::canvas_set_clip(bool p_clip, const Rect2& p_rect) {




}

void Rasterizer3DS::canvas_end_rect() {


}

void Rasterizer3DS::canvas_draw_line(const Point2& p_from, const Point2& p_to,const Color& p_color,float p_width) {



}

void Rasterizer3DS::canvas_draw_rect(const Rect2& p_rect, int p_flags, const Rect2& p_source,RID p_texture,const Color& p_modulate) {


	// _set_glcoloro( p_modulate,canvas_opacity );

	if ( p_texture.is_valid() ) {

		// sceGuEnable(GU_TEXTURE_2D);
		Texture *texture = texture_owner.get( p_texture );
		ERR_FAIL_COND(!texture);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture( GL_TEXTURE_2D,texture->tex_id );

		C3D_TexEnv* env = C3D_GetTexEnv(0);
		C3D_TexEnvSrc(env, C3D_Both, GPU_FRAGMENT_PRIMARY_COLOR, GPU_FRAGMENT_SECONDARY_COLOR);
		C3D_TexEnvOpRgb(env, GPU_TEVOP_RGB_SRC_COLOR);
		C3D_TexEnvOpAlpha(env,GPU_TEVOP_A_SRC_ALPHA);
		C3D_TexEnvFunc(env, C3D_Both, GPU_ADD);

		C3D_TexBind(0, &texture->texture);

		if (!(p_flags&CANVAS_RECT_REGION)) {

			Rect2 region = Rect2(0,0,texture->width,texture->height);
			_draw_textured_quad(p_rect,region,region.size,p_flags&CANVAS_RECT_FLIP_H,p_flags&CANVAS_RECT_FLIP_V);

		} else {


			_draw_textured_quad(p_rect, p_source, Size2(texture->width,texture->height),p_flags&CANVAS_RECT_FLIP_H,p_flags&CANVAS_RECT_FLIP_V );

		}
		
	} else {

		WARN_PRINT("p_texture is not valid");

		// sceGuDisable(GU_TEXTURE_2D);
		// _draw_quad( p_rect );

	}

}
void Rasterizer3DS::canvas_draw_style_box(const Rect2& p_rect, RID p_texture,const float *p_margin, bool p_draw_center,const Color& p_modulate) {


}
void Rasterizer3DS::canvas_draw_primitive(const Vector<Point2>& p_points, const Vector<Color>& p_colors,const Vector<Point2>& p_uvs, RID p_texture,float p_width) {



}


void Rasterizer3DS::canvas_draw_polygon(int p_vertex_count, const int* p_indices, const Vector2* p_vertices, const Vector2* p_uvs, const Color* p_colors,const RID& p_texture,bool p_singlecolor) {



}

void Rasterizer3DS::canvas_set_transform(const Matrix32& p_transform) {


}

/* ENVIRONMENT */

RID Rasterizer3DS::environment_create() {

	Environment * env = memnew( Environment );
	return environment_owner.make_rid(env);
}

void Rasterizer3DS::environment_set_background(RID p_env,VS::EnvironmentBG p_bg) {

	ERR_FAIL_INDEX(p_bg,VS::ENV_BG_MAX);
	Environment * env = environment_owner.get(p_env);
	ERR_FAIL_COND(!env);
	env->bg_mode=p_bg;
}

VS::EnvironmentBG Rasterizer3DS::environment_get_background(RID p_env) const{

	const Environment * env = environment_owner.get(p_env);
	ERR_FAIL_COND_V(!env,VS::ENV_BG_MAX);
	return env->bg_mode;
}

void Rasterizer3DS::environment_set_background_param(RID p_env,VS::EnvironmentBGParam p_param, const Variant& p_value){

	ERR_FAIL_INDEX(p_param,VS::ENV_BG_PARAM_MAX);
	Environment * env = environment_owner.get(p_env);
	ERR_FAIL_COND(!env);
	env->bg_param[p_param]=p_value;

}
Variant Rasterizer3DS::environment_get_background_param(RID p_env,VS::EnvironmentBGParam p_param) const{

	ERR_FAIL_INDEX_V(p_param,VS::ENV_BG_PARAM_MAX,Variant());
	const Environment * env = environment_owner.get(p_env);
	ERR_FAIL_COND_V(!env,Variant());
	return env->bg_param[p_param];

}

void Rasterizer3DS::environment_set_enable_fx(RID p_env,VS::EnvironmentFx p_effect,bool p_enabled){

	ERR_FAIL_INDEX(p_effect,VS::ENV_FX_MAX);
	Environment * env = environment_owner.get(p_env);
	ERR_FAIL_COND(!env);
	env->fx_enabled[p_effect]=p_enabled;
}
bool Rasterizer3DS::environment_is_fx_enabled(RID p_env,VS::EnvironmentFx p_effect) const{

	ERR_FAIL_INDEX_V(p_effect,VS::ENV_FX_MAX,false);
	const Environment * env = environment_owner.get(p_env);
	ERR_FAIL_COND_V(!env,false);
	return env->fx_enabled[p_effect];

}

void Rasterizer3DS::environment_fx_set_param(RID p_env,VS::EnvironmentFxParam p_param,const Variant& p_value){

	ERR_FAIL_INDEX(p_param,VS::ENV_FX_PARAM_MAX);
	Environment * env = environment_owner.get(p_env);
	ERR_FAIL_COND(!env);
	env->fx_param[p_param]=p_value;
}
Variant Rasterizer3DS::environment_fx_get_param(RID p_env,VS::EnvironmentFxParam p_param) const{

	ERR_FAIL_INDEX_V(p_param,VS::ENV_FX_PARAM_MAX,Variant());
	const Environment * env = environment_owner.get(p_env);
	ERR_FAIL_COND_V(!env,Variant());
	return env->fx_param[p_param];

}


RID Rasterizer3DS::sampled_light_dp_create(int p_width,int p_height) {

	return sampled_light_owner.make_rid(memnew(SampledLight));
}

void Rasterizer3DS::sampled_light_dp_update(RID p_sampled_light, const Color *p_data, float p_multiplier) {


}


/*MISC*/

bool Rasterizer3DS::is_texture(const RID& p_rid) const {

	return texture_owner.owns(p_rid);
}
bool Rasterizer3DS::is_material(const RID& p_rid) const {

	return material_owner.owns(p_rid);
}
bool Rasterizer3DS::is_mesh(const RID& p_rid) const {

	return mesh_owner.owns(p_rid);
}

bool Rasterizer3DS::is_immediate(const RID& p_rid) const {

	return immediate_owner.owns(p_rid);
}

bool Rasterizer3DS::is_multimesh(const RID& p_rid) const {

	return multimesh_owner.owns(p_rid);
}
bool Rasterizer3DS::is_particles(const RID &p_beam) const {

	return particles_owner.owns(p_beam);
}

bool Rasterizer3DS::is_light(const RID& p_rid) const {

	return light_owner.owns(p_rid);
}
bool Rasterizer3DS::is_light_instance(const RID& p_rid) const {

	return light_instance_owner.owns(p_rid);
}
bool Rasterizer3DS::is_particles_instance(const RID& p_rid) const {

	return particles_instance_owner.owns(p_rid);
}
bool Rasterizer3DS::is_skeleton(const RID& p_rid) const {

	return skeleton_owner.owns(p_rid);
}
bool Rasterizer3DS::is_environment(const RID& p_rid) const {

	return environment_owner.owns(p_rid);
}

bool Rasterizer3DS::is_shader(const RID& p_rid) const {

	return false;
}

void Rasterizer3DS::free(const RID& p_rid) {

	if (texture_owner.owns(p_rid)) {

		// delete the texture
		Texture *texture = texture_owner.get(p_rid);
		texture_owner.free(p_rid);
		memdelete(texture);

	} else if (shader_owner.owns(p_rid)) {

		// delete the texture
		Shader *shader = shader_owner.get(p_rid);
		shader_owner.free(p_rid);
		memdelete(shader);

	} else if (material_owner.owns(p_rid)) {

		Material *material = material_owner.get( p_rid );
		material_owner.free(p_rid);
		memdelete(material);

	} else if (mesh_owner.owns(p_rid)) {

		Mesh *mesh = mesh_owner.get(p_rid);

		for (int i=0;i<mesh->surfaces.size();i++) {

			memdelete( mesh->surfaces[i] );
		};

		mesh->surfaces.clear();
		mesh_owner.free(p_rid);
		memdelete(mesh);

	} else if (multimesh_owner.owns(p_rid)) {

	       MultiMesh *multimesh = multimesh_owner.get(p_rid);
	       multimesh_owner.free(p_rid);
	       memdelete(multimesh);

	} else if (immediate_owner.owns(p_rid)) {

		Immediate *immediate = immediate_owner.get(p_rid);
		immediate_owner.free(p_rid);
		memdelete(immediate);

	} else if (particles_owner.owns(p_rid)) {

		Particles *particles = particles_owner.get(p_rid);
		particles_owner.free(p_rid);
		memdelete(particles);
	} else if (particles_instance_owner.owns(p_rid)) {

		ParticlesInstance *particles_isntance = particles_instance_owner.get(p_rid);
		particles_instance_owner.free(p_rid);
		memdelete(particles_isntance);

	} else if (skeleton_owner.owns(p_rid)) {

		Skeleton *skeleton = skeleton_owner.get( p_rid );
		skeleton_owner.free(p_rid);
		memdelete(skeleton);

	} else if (light_owner.owns(p_rid)) {

		Light *light = light_owner.get( p_rid );
		light_owner.free(p_rid);
		memdelete(light);

	} else if (light_instance_owner.owns(p_rid)) {

		LightInstance *light_instance = light_instance_owner.get( p_rid );
		light_instance_owner.free(p_rid);
		memdelete( light_instance );


	} else if (environment_owner.owns(p_rid)) {

		Environment *env = environment_owner.get( p_rid );
		environment_owner.free(p_rid);
		memdelete( env );
	} else if (sampled_light_owner.owns(p_rid)) {

		SampledLight *sampled_light = sampled_light_owner.get( p_rid );
		ERR_FAIL_COND(!sampled_light);

		sampled_light_owner.free(p_rid);
		memdelete( sampled_light );

	};
}


void Rasterizer3DS::custom_shade_model_set_shader(int p_model, RID p_shader) {


};

RID Rasterizer3DS::custom_shade_model_get_shader(int p_model) const {

	return RID();
};

void Rasterizer3DS::custom_shade_model_set_name(int p_model, const String& p_name) {

};

String Rasterizer3DS::custom_shade_model_get_name(int p_model) const {

	return String();
};

void Rasterizer3DS::custom_shade_model_set_param_info(int p_model, const List<PropertyInfo>& p_info) {

};

void Rasterizer3DS::custom_shade_model_get_param_info(int p_model, List<PropertyInfo>* p_info) const {

};



void Rasterizer3DS::init() {
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	
	top_rt = memnew(RenderTarget);
	
	top_rt->target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(top_rt->target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
	
	
	simple_2ds = DVLB_ParseFile((u32*)shader_builtin_simple_2d, sizeof(shader_builtin_simple_2d));
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &simple_2ds->DVLE[0]);
	C3D_BindProgram(&program);
	
	uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");
	uLoc_modelView = shaderInstanceGetUniformLocation(program.vertexShader, "modelView");
	
	// Configure attributes for use with the vertex shader
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 3); // v1=color
	
	// Mtx_PerspTilt(&projection, C3D_AngleFromDegrees(80.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, false);
	Mtx_OrthoTilt(&projection, 0.0, 800.0, 0.0, 440.0, 0.0, 1.0, true);
	
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0, (GPU_TEVSRC)0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
}

void Rasterizer3DS::finish() {


}

int Rasterizer3DS::get_render_info(VS::RenderInfo p_info) {

	return 0;
}

bool Rasterizer3DS::needs_to_draw_next_frame() const {

	return false;
}


bool Rasterizer3DS::has_feature(VS::Features p_feature) const {

	return false;

}


Rasterizer3DS::Rasterizer3DS() {

};

Rasterizer3DS::~Rasterizer3DS() {
	shaderProgramFree(&program);
	DVLB_Free(simple_2ds);
	C3D_Fini();
};

