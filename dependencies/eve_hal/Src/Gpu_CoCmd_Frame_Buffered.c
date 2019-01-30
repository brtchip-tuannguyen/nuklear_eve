/**
* This source code ("the Software") is provided by Bridgetek Pte Ltd
* ("Bridgetek") subject to the licence terms set out
*   http://brtchip.com/BRTSourceCodeLicenseAgreement/ ("the Licence Terms").
* You must read the Licence Terms before downloading or using the Software.
* By installing or using the Software you agree to the Licence Terms. If you
* do not agree to the Licence Terms then do not download or use the Software.
*
* Without prejudice to the Licence Terms, here is a summary of some of the key
* terms of the Licence Terms (and in the event of any conflict between this
* summary and the Licence Terms then the text of the Licence Terms will
* prevail).
*
* The Software is provided "as is".
* There are no warranties (or similar) in relation to the quality of the
* Software. You use it at your own risk.
* The Software should not be used in, or for, any medical device, system or
* appliance. There are exclusions of Bridgetek liability for certain types of loss
* such as: special loss or damage; incidental loss or damage; indirect or
* consequential loss or damage; loss of income; loss of business; loss of
* profits; loss of revenue; loss of contracts; business interruption; loss of
* the use of money or anticipated savings; loss of information; loss of
* opportunity; loss of goodwill or reputation; and/or loss of, damage to or
* corruption of data.
* There is a monetary cap on Bridgetek's liability.
* The Software may have subsequently been amended by another user and then
* distributed by that other user ("Adapted Software").  If so that user may
* have additional licence terms that apply to those amendments. However, Bridgetek
* has no liability in relation to those amendments.
*/

#include "FT_Platform.h"
#if defined(FT_GPU_COCMD_BUFFERED)

#define FT_BUFFER_CAPACITY (FT_CMD_FIFO_SIZE >> 1)
#define FT_BUFFER_MASK (FT_BUFFER_CAPACITY - 1)

static ft_uint8_t s_CmdBuffer[FT_BUFFER_CAPACITY];
static ft_uint16_t s_CmdBufferIndex = 0;

ft_void_t Ft_Gpu_CoCmd_SendCmdArr(Ft_Gpu_Hal_Context_t *phost, ft_uint32_t *cmd, ft_size_t nb)
{
	eve_assert(phost->cmd_frame);
	ft_uint16_t len = (sizeof(cmd[0]) * nb) & FT_CMD_FIFO_MASK;
	if ((s_CmdBufferIndex + len) > FT_BUFFER_CAPACITY)
	{
		Ft_Gpu_CoCmd_EndFrame(phost);
		Ft_Gpu_CoCmd_StartFrame(phost);
	}
	memcpy(&s_CmdBuffer[s_CmdBufferIndex], cmd, len);
	s_CmdBufferIndex += len;
}

ft_void_t Ft_Gpu_CoCmd_SendCmd(Ft_Gpu_Hal_Context_t *phost, ft_uint32_t cmd)
{
	eve_assert(phost->cmd_frame);
	if ((s_CmdBufferIndex + sizeof(cmd)) > FT_BUFFER_CAPACITY)
	{
		Ft_Gpu_CoCmd_EndFrame(phost);
		Ft_Gpu_CoCmd_StartFrame(phost);
	}
	memcpy(&s_CmdBuffer[s_CmdBufferIndex], &cmd, sizeof(cmd));
	s_CmdBufferIndex += sizeof(cmd);
}

ft_void_t Ft_Gpu_CoCmd_SendStr(Ft_Gpu_Hal_Context_t *phost, const ft_char8_t *s)
{
	eve_assert(phost->cmd_frame);
	ft_uint16_t length = (ft_uint16_t)strlen(s) + 1;
	if ((s_CmdBufferIndex + length) > FT_BUFFER_CAPACITY)
	{
		Ft_Gpu_CoCmd_EndFrame(phost);
		Ft_Gpu_CoCmd_StartFrame(phost);
	}
	memcpy(&s_CmdBuffer[s_CmdBufferIndex], s, length);
	s_CmdBufferIndex += length;
	while (s_CmdBufferIndex & 3)
	{
		s_CmdBuffer[s_CmdBufferIndex] = 0;
		++s_CmdBufferIndex;
	}
}

ft_void_t Ft_Gpu_CoCmd_StartFrame(Ft_Gpu_Hal_Context_t *phost)
{
	eve_assert(!phost->cmd_frame);
	phost->cmd_frame = FT_TRUE;
	s_CmdBufferIndex = 0;
	memset(s_CmdBuffer, 0, FT_BUFFER_CAPACITY);
}

ft_void_t Ft_Gpu_CoCmd_EndFrame(Ft_Gpu_Hal_Context_t *phost)
{
	eve_assert(phost->cmd_frame);
	phost->cmd_frame = FT_FALSE;
	Ft_Gpu_Hal_WrCmdBuf(phost, s_CmdBuffer, s_CmdBufferIndex);
}

#endif

/* end of file */