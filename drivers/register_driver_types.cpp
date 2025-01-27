/*************************************************/
/*  register_driver_types.cpp                    */
/*************************************************/
/*            This file is part of:              */
/*                GODOT ENGINE                   */
/*************************************************/
/*       Source code within this file is:        */
/*  (c) 2007-2010 Juan Linietsky, Ariel Manzur   */
/*             All Rights Reserved.              */
/*************************************************/

#include "register_driver_types.h"

#include "png/image_loader_png.h"
#include "webp/image_loader_webp.h"
#include "png/resource_saver_png.h"
#include "jpg/image_loader_jpg.h"
#include "dds/texture_loader_dds.h"
#include "pvr/texture_loader_pvr.h"
#include "etc1/image_etc.h"
#include "chibi/event_stream_chibi.h"


#ifdef TOOLS_ENABLED
#include "squish/image_compress_squish.h"
#endif

#ifdef TOOLS_ENABLED
#include "convex_decomp/b2d_decompose.h"
#endif

#ifdef TREMOR_ENABLED
#include "teora/audio_stream_ogg.h"
#endif

#ifdef VORBIS_ENABLED
#include "vorbis/audio_stream_ogg_vorbis.h"
#endif


#ifdef SPEEX_ENABLED
#include "speex/audio_stream_speex.h"
#endif

#ifdef THEORA_ENABLED
//#include "theora/video_stream_theora.h"
#include "theoraplayer/video_stream_theoraplayer.h"
#endif

#include "drivers/trex/regex.h"

#ifdef MUSEPACK_ENABLED
#include "mpc/audio_stream_mpc.h"
#endif

#ifdef PNG_ENABLED
static ImageLoaderPNG *image_loader_png=NULL;
static ResourceSaverPNG *resource_saver_png=NULL;
#endif

#ifdef WEBP_ENABLED
static ImageLoaderWEBP *image_loader_webp=NULL;
//static ResourceSaverPNG *resource_saver_png=NULL;
#endif

#ifdef JPG_ENABLED
static ImageLoaderJPG *image_loader_jpg=NULL;
#endif

#ifdef DDS_ENABLED
static ResourceFormatDDS *resource_loader_dds=NULL;
#endif


#ifdef PVR_ENABLED
static ResourceFormatPVR *resource_loader_pvr=NULL;
#endif

#ifdef TREMOR_ENABLED
static ResourceFormatLoaderAudioStreamOGG *vorbis_stream_loader=NULL;
#endif

#ifdef VORBIS_ENABLED
static ResourceFormatLoaderAudioStreamOGGVorbis *vorbis_stream_loader=NULL;
#endif

#ifdef SPEEX_ENABLED
static ResourceFormatLoaderAudioStreamSpeex *speex_stream_loader=NULL;
#endif

#ifdef THEORA_ENABLED
//static ResourceFormatLoaderVideoStreamTheora* theora_stream_loader = NULL;
static ResourceFormatLoaderVideoStreamTheoraplayer* theoraplayer_stream_loader = NULL;
#endif

#ifdef MUSEPACK_ENABLED
static ResourceFormatLoaderAudioStreamMPC * mpc_stream_loader=NULL;
#endif

#ifdef OPENSSL_ENABLED
#include "openssl/register_openssl.h"
#endif

#ifdef PCK_ENABLED
#include "pck/pack_pck.h"
#endif

#ifdef MINIZIP_ENABLED
#include "zip_pack/zip_archive.h"
#endif

void register_core_driver_types() {

#ifdef PCK_ENABLED
	PackedData::get_singleton()->add_pack_source(PackedSourcePCK::get_singleton());
#endif

#if MINIZIP_ENABLED
	PackedData::get_singleton()->add_pack_source(ZipArchive::get_singleton());
#endif

#ifdef PNG_ENABLED
	image_loader_png = memnew( ImageLoaderPNG );
	ImageLoader::add_image_format_loader( image_loader_png );

	resource_saver_png = memnew( ResourceSaverPNG );
	ResourceSaver::add_resource_format_saver(resource_saver_png);

#endif

#ifdef WEBP_ENABLED
	image_loader_webp = memnew( ImageLoaderWEBP );
	ImageLoader::add_image_format_loader( image_loader_webp );

//	resource_saver_png = memnew( ResourceSaverPNG );
//	ResourceSaver::add_resource_format_saver(resource_saver_png);

#endif

#ifdef JPG_ENABLED

	image_loader_jpg = memnew( ImageLoaderJPG );
	ImageLoader::add_image_format_loader( image_loader_jpg );
#endif

	REGISTER_OBJECT(RegEx);
}

void unregister_core_driver_types() {

#ifdef PNG_ENABLED
	if (image_loader_png)
		memdelete( image_loader_png );
	if (resource_saver_png)
		memdelete( resource_saver_png );
#endif

#ifdef WEBP_ENABLED
	if (image_loader_webp)
		memdelete( image_loader_webp );
//	if (resource_saver_png)
//		memdelete( resource_saver_png );
#endif

#ifdef JPG_ENABLED
	if (image_loader_jpg)
		memdelete( image_loader_jpg );
#endif

}


void register_driver_types() {

#ifdef TREMOR_ENABLED
	vorbis_stream_loader=memnew( ResourceFormatLoaderAudioStreamOGG );
	ResourceLoader::add_resource_format_loader(vorbis_stream_loader );
	REGISTER_OBJECT(AudioStreamOGG);
#endif

#ifdef VORBIS_ENABLED
	vorbis_stream_loader=memnew( ResourceFormatLoaderAudioStreamOGGVorbis );
	ResourceLoader::add_resource_format_loader(vorbis_stream_loader );
	REGISTER_OBJECT(AudioStreamOGGVorbis);
#endif


#ifdef DDS_ENABLED
	resource_loader_dds = memnew( ResourceFormatDDS );
	ResourceLoader::add_resource_format_loader(resource_loader_dds );
#endif

#ifdef PVR_ENABLED
	resource_loader_pvr = memnew( ResourceFormatPVR );
	ResourceLoader::add_resource_format_loader(resource_loader_pvr );
#endif

#ifdef TOOLS_ENABLED

	Geometry::_decompose_func=b2d_decompose;
#endif

#ifdef SPEEX_ENABLED
	speex_stream_loader=memnew( ResourceFormatLoaderAudioStreamSpeex );
	ResourceLoader::add_resource_format_loader(speex_stream_loader);
	REGISTER_OBJECT(AudioStreamSpeex);
#endif

#ifdef MUSEPACK_ENABLED

	mpc_stream_loader=memnew( ResourceFormatLoaderAudioStreamMPC );
	ResourceLoader::add_resource_format_loader(mpc_stream_loader);
	REGISTER_OBJECT(AudioStreamMPC);

#endif

#ifdef OPENSSL_ENABLED

	register_openssl();
#endif

#ifdef THEORA_ENABLED
	//theora_stream_loader = memnew( ResourceFormatLoaderVideoStreamTheora );
	//ResourceLoader::add_resource_format_loader(theora_stream_loader);
	//REGISTER_OBJECT(VideoStreamTheora);
	theoraplayer_stream_loader = memnew( ResourceFormatLoaderVideoStreamTheoraplayer );
	ResourceLoader::add_resource_format_loader(theoraplayer_stream_loader);
	REGISTER_OBJECT(VideoStreamTheoraplayer);
#endif


#ifdef TOOLS_ENABLED
#ifdef SQUISH_ENABLED

	Image::set_compress_bc_func(image_compress_squish);

#endif
#endif

	_register_etc1_compress_func();
	initialize_chibi();
}

void unregister_driver_types() {

#ifdef TREMOR_ENABLED
	memdelete( vorbis_stream_loader );
#endif

#ifdef VORBIS_ENABLED
	memdelete( vorbis_stream_loader );
#endif

#ifdef SPEEX_ENABLED
	memdelete( speex_stream_loader );
#endif

#ifdef THEORA_ENABLED
	//memdelete (theora_stream_loader);
	memdelete (theoraplayer_stream_loader);
#endif

#ifdef MUSEPACK_ENABLED

	memdelete (mpc_stream_loader);
#endif

#ifdef DDS_ENABLED
	memdelete(resource_loader_dds);
#endif

#ifdef PVR_ENABLED
	memdelete(resource_loader_pvr);
#endif

#ifdef OPENSSL_ENABLED

	unregister_openssl();
#endif

	finalize_chibi();
}
