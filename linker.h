#ifndef __LINKER_H
#define __LINKER_H

#include <stdlib.h>
#include "linkerapi_enum.h"

typedef struct LinkerCtx* LinkerHandle;

LinkerHandle LinkerOpen();
void LinkerClose(LinkerHandle h);

size_t LinkerRead(LinkerHandle h, void* mem, size_t NumBytes);
size_t LinkerWrite(LinkerHandle h, const void* mem, size_t NumBytes);

bool LinkerSetCommand(LinkerHandle h, LINKER_CMD cmd, const uint8_t* Params, size_t ParamSize);
bool LinkerSetCommand(LinkerHandle h, LINKER_CMD cmd, uint8_t Param);

bool LinkerGetFwVersion(LinkerHandle h, char* Str);
size_t LinkerGetStateVector(LinkerHandle h, uint8_t* pState, size_t MaxSize);

size_t LinkerGetAVI_IF(LinkerHandle h, uint8_t* pIF, size_t MaxSize);
size_t LinkerGetHDR_IF(LinkerHandle h, uint8_t* pIF, size_t MaxSize);
size_t LinkerGetVIC_MODE(LinkerHandle h, uint8_t* pVIC, size_t MaxSize);

#endif