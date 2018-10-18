#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <usb.h>
#include <assert.h>

#include "linker.h"

#define LINKER_USB_VID                      0x10c4
#define LINKER_USB_PID                      0xEA61

#define TX_TIMEOUT		4000
#define RX_TIMEOUT    4000
#define BUFFER_SIZE		4096

#ifdef DEBUG
#define LOGD(args...) printf (args)
#else
#define LOGD(format, ...)
#endif
#define LOGE(args...) printf (args)

struct LinkerCtx
{
	usb_dev_handle* udev;
	int interface;
	int ep_out;
	int ep_in;
	size_t buffer_offset;
	char buffer[BUFFER_SIZE];
};

static int linker_init = 0;

static void init()
{
	if (linker_init == 0)
	{
		usb_init();
		linker_init = 1;
	}
}

LinkerHandle LinkerOpen()
{
	init();
	usb_find_busses();
	usb_find_devices();

	struct usb_bus* busses = usb_get_busses();
	struct usb_device* pdev = NULL;

	for (struct usb_bus* bus = busses; bus; bus = bus->next)
	{
		for (struct usb_device* dev = bus->devices; dev; dev = dev->next) 
		{
			if (dev->descriptor.idVendor == LINKER_USB_VID && dev->descriptor.idProduct == LINKER_USB_PID)
			{
				pdev = dev;
				break;
			}
		}
	}

	if (pdev == NULL)
	{
		return NULL;
	}

	LinkerCtx* ph = (LinkerCtx*)malloc(sizeof(LinkerCtx));
	if (ph == NULL)
		return NULL;

	memset(ph, 0, sizeof(LinkerCtx));

	ph->ep_out = -1;
	ph->ep_in = -1;

	// get bulk in/out endpoints
	for (int i = 0; i < pdev->config[0].interface[0].altsetting[0].bNumEndpoints; i++)
	{
		if (pdev->config[0].interface[0].altsetting[0].endpoint[i].bmAttributes == USB_ENDPOINT_TYPE_BULK)
		{
			if ((pdev->config[0].interface[0].altsetting[0].endpoint[i].bEndpointAddress&USB_ENDPOINT_DIR_MASK) != 0)
				ph->ep_in = pdev->config[0].interface[0].altsetting[0].endpoint[i].bEndpointAddress;
			else
				ph->ep_out = pdev->config[0].interface[0].altsetting[0].endpoint[i].bEndpointAddress;
		}
	}

	if (ph->ep_out==-1 || ph->ep_in==-1)
	{
		LOGE("Unable to identify BULK IN/OUT endpoints\n");
		free(ph);
		return NULL;
	}

	ph->udev = usb_open(pdev);
	if (ph->udev == NULL)
	{
		free(ph);
		return NULL;
	}

	// claim the interface
	ph->interface = pdev->config[0].interface[0].altsetting[0].bInterfaceNumber;
	if (usb_claim_interface(ph->udev, ph->interface))
	{
		LOGE("Unable to claim interface. Ensure device is not claimed by any kernel modules. Check permissions of /dev/bus/usb/...\n");
		usb_close(ph->udev);
		free(ph);
		return NULL;
	}

	usb_control_msg(ph->udev, 0x40, 0x00, 0xFFFF, 0, NULL, 0, TX_TIMEOUT);
	usb_resetep(ph->udev, ph->ep_in);
	usb_resetep(ph->udev, ph->ep_out);
	usb_clear_halt(ph->udev, ph->ep_in);
	usb_clear_halt(ph->udev, ph->ep_out);
	usb_control_msg(ph->udev, 0x40, 0x02, 0x0002, 0, NULL, 0, TX_TIMEOUT);
	return ph;
}

void LinkerClose(LinkerHandle h)
{
	init();

	LinkerCtx* ctx = (LinkerCtx*)h;
	if (ctx)
	{
		usb_release_interface(ctx->udev, ctx->interface);
	  usb_close(ctx->udev);
	}
	free(ctx);
}

static void _LinkerRead(LinkerCtx* ctx, int timeout)
{
	assert(ctx != NULL);

	size_t toread = BUFFER_SIZE - (int)ctx->buffer_offset;
	int r = usb_bulk_read(ctx->udev, ctx->ep_in, ctx->buffer + ctx->buffer_offset, toread, timeout);
	if (r > 0)
	{
		ctx->buffer_offset += (size_t)r;
	}
}

static void _LinkerFlush(LinkerCtx* ctx)
{
	ctx->buffer_offset = 0;
}

static size_t _LinkerCopyBuffer(LinkerCtx* ctx, void* mem, size_t NumBytes)
{
	assert(ctx != NULL);
	assert(mem != NULL);

	if (ctx->buffer_offset < NumBytes)
		NumBytes = ctx->buffer_offset;
	else if (NumBytes > BUFFER_SIZE)
		NumBytes = BUFFER_SIZE;

	if (NumBytes > 0)
	{
		memcpy(mem, ctx->buffer, NumBytes);
		ctx->buffer_offset -= NumBytes;
		if (ctx->buffer_offset > 0)
			memmove(ctx->buffer, ctx->buffer + NumBytes, ctx->buffer_offset);
	}

	return NumBytes;
}

size_t LinkerRead(LinkerHandle h, void* mem, size_t NumBytes)
{
	LinkerCtx* ctx = (LinkerCtx*)h;
	if (ctx->buffer_offset < NumBytes)
		_LinkerRead(ctx, RX_TIMEOUT);

	return _LinkerCopyBuffer(ctx, mem, NumBytes);
}

size_t LinkerWrite(LinkerHandle h, const void* mem, size_t NumBytes)
{
	LinkerCtx* ctx = (LinkerCtx*)h;

	_LinkerRead(ctx, 100);

	int written = usb_bulk_write(ctx->udev, ctx->ep_out, (const char*)mem, NumBytes, TX_TIMEOUT);

	_LinkerRead(ctx, 100);

	if (written < 0) written = 0;

	return (size_t)written;
}

struct LinkerHeader_s
{
	uint8_t Flags;
	uint8_t Unknown;
	uint8_t Type;
};

struct LinkerResponse_s
{
	uint8_t Type;
	uint8_t Unknown; // size?
};

struct LinkerCommand_s : LinkerHeader_s
{
	uint8_t Command;

	LinkerCommand_s(uint8_t cmd = 0)
	{
		Flags = 0x11;
		Unknown = 0;
		Type = 0xE0;
		Command = cmd;
	}
};

struct LinkerSetCommand_s : LinkerCommand_s
{
	LinkerSetCommand_s(uint8_t cmd = 0) : LinkerCommand_s(cmd)
	{
		Flags = 0x10;
	}
};

static size_t _LinkerQuery(LinkerHandle h, uint8_t cmd, uint8_t* Buffer, size_t BufferSize)
{
	LinkerCommand_s get_cmd(cmd);

	LinkerWrite(h, &get_cmd, sizeof(get_cmd));

	size_t read = LinkerRead(h, Buffer, BufferSize);

	if (read < sizeof(LinkerResponse_s) || Buffer[0] != get_cmd.Type)
		return 0;

	read -= sizeof(LinkerResponse_s);
	memmove(Buffer, Buffer + 2, read);
	return read;
}

bool LinkerGetFwVersion(LinkerHandle h, char* Str)
{
	LinkerCommand_s get_fw(FW_VERSION);

	LinkerWrite(h, &get_fw, sizeof(get_fw));

	uint8_t data[32];
	size_t r = LinkerRead(h, data, sizeof(data));

	if (r < sizeof(LinkerResponse_s) || data[0] != get_fw.Type)
		return false;

	sprintf(Str, "%d.%d", data[sizeof(LinkerResponse_s)], data[sizeof(LinkerResponse_s) + 1]);
	return true;
}

size_t LinkerGetAVI_IF(LinkerHandle h, uint8_t* pIF, size_t MaxSize)
{
	// note that infoframe also contains the shared 3 byte IF header which denotes what kind of IF we're dealing with.
	return _LinkerQuery(h, AVI_IF, pIF, MaxSize);
}

size_t LinkerGetHDR_IF(LinkerHandle h, uint8_t* pIF, size_t MaxSize)
{
	return _LinkerQuery(h, HDR_IF, pIF, MaxSize);
}

size_t LinkerGetVIC_MODE(LinkerHandle h, uint8_t* pVIC, size_t MaxSize)
{
	return _LinkerQuery(h, VIC_MODE, pVIC, MaxSize);
}

size_t LinkerGetStateVector(LinkerHandle h, uint8_t* pState, size_t MaxSize)
{
	LinkerHeader_s get_linkerstate;
	get_linkerstate.Flags = 0x11;
	get_linkerstate.Unknown = 0;
	get_linkerstate.Type = 0xE1;

	LinkerWrite(h, &get_linkerstate, sizeof(get_linkerstate));

	size_t read = LinkerRead(h, pState, MaxSize);

	if (read < sizeof(LinkerResponse_s) || pState[0] != get_linkerstate.Type)
		return 0;

	read -= sizeof(LinkerResponse_s);

	memmove(pState, pState + sizeof(LinkerResponse_s), read);
	return read;
}

bool LinkerSetCommand(LinkerHandle h, LINKER_CMD cmd, const uint8_t* Params, size_t ParamSize)
{
	uint8_t tmp[256];
	assert((ParamSize + 4) < sizeof(tmp));

	LinkerSetCommand_s* pCmd = (LinkerSetCommand_s*)tmp;
	*pCmd = LinkerSetCommand_s(cmd);

	memcpy(&pCmd[1], Params, ParamSize);
	LinkerWrite(h, tmp, sizeof(LinkerSetCommand_s) + ParamSize);

	size_t read = LinkerRead(h, tmp, sizeof(tmp));
	// todo: parse response to determine success
	return !!(read != 0);
}

bool LinkerSetCommand(LinkerHandle h, LINKER_CMD cmd, uint8_t Param)
{
	return LinkerSetCommand(h, cmd, &Param, 1);
}
