#ifndef GOKU_GOKU_H
#define GOKU_GOKU_H

#include "define.h"
#include "i_loop.h"
#include "i_object_factory.h"
#include "i_tcp_client.h"
#include "i_tcp_server.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

GOKU_API goku::IObjectFactory* GetObjectFactory();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // GOKU_GOKU_H
