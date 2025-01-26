#include "register_openssl.h"

#include "stream_peer_openssl.h"
#ifdef OPENSSL_ENABLED

void register_openssl() {

	REGISTER_OBJECT(StreamPeerOpenSSL);
	StreamPeerOpenSSL::initialize_ssl();

}

void unregister_openssl() {

	StreamPeerOpenSSL::finalize_ssl();

}
#endif

