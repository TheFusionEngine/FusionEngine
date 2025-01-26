#pragma once

#include <network.h>

namespace gc {
namespace net {

inline u32 gethostip(void) { return net_gethostip(); }
inline s32 socket(u32 domain,u32 type,u32 protocol) { return net_socket(domain, type, protocol); }
inline s32 bind(s32 s,struct sockaddr *name,socklen_t namelen) { return net_bind(s, name, namelen); }
inline s32 listen(s32 s,u32 backlog) { return net_listen(s, backlog); }
inline s32 accept(s32 s,struct sockaddr *addr,socklen_t *addrlen) { return net_accept(s, addr, addrlen); }
inline s32 connect(s32 s,struct sockaddr *addr,socklen_t addrlen) { return net_connect(s, addr, addrlen); }
inline s32 write(s32 s,const void *data,s32 size) { return net_write(s, data, size); }
inline s32 send(s32 s,const void *data,s32 size,u32 flags) { return net_send(s, data, size, flags); }
inline s32 sendto(s32 s,const void *data,s32 len,u32 flags,struct sockaddr *to,socklen_t tolen) { return net_sendto(s, data, len, flags, to, tolen); }
inline s32 recv(s32 s,void *mem,s32 len,u32 flags) { return net_recv(s, mem, len, flags); }
inline s32 recvfrom(s32 s,void *mem,s32 len,u32 flags,struct sockaddr *from,socklen_t *fromlen) { return net_recvfrom(s, mem, len, flags, from, fromlen); }
inline s32 read(s32 s,void *mem,s32 len) { return net_read(s, mem, len); }
inline s32 close(s32 s) { return net_close(s); }
inline s32 select(s32 maxfdp1,fd_set *readset,fd_set *writeset,fd_set *exceptset,struct timeval *timeout) { return net_select(maxfdp1, readset, writeset, exceptset, timeout); }
inline s32 getsockopt(s32 s,u32 level,u32 optname,const void *optval,socklen_t optlen) { return net_getsockopt(s, level, optname, optval, optlen); }
inline s32 setsockopt(s32 s,u32 level,u32 optname,const void *optval,socklen_t optlen) { return net_setsockopt(s, level, optname, optval, optlen); }
inline s32 getsockname(s32 s, struct sockaddr *addr, socklen_t *addrlen) { return net_getsockname(s, addr, addrlen); }
inline s32 ioctl(s32 s, u32 cmd, void *argp) { return net_ioctl(s, cmd, argp); }
inline s32 fcntl(s32 s, u32 cmd, u32 flags) { return net_fcntl(s, cmd, flags); }
inline s32 poll(struct pollsd *sds,s32 nsds,s32 timeout) { return net_poll(sds, nsds, timeout); }
inline s32 shutdown(s32 s, u32 how) { return net_shutdown(s, how); }

inline struct hostent * gethostbyname(const char *addrString) { return net_gethostbyname(addrString); }
}
}
