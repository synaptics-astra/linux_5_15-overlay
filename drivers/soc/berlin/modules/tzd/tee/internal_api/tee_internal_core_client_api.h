/********************************************************************************
 * Marvell GPL License Option
 *
 * If you received this File from Marvell, you may opt to use, redistribute and/or
 * modify this File in accordance with the terms and conditions of the General
 * Public License Version 2, June 1991 (the "GPL License"), a copy of which is
 * available along with the File in the license.txt file or by writing to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
 * on the worldwide web at http://www.gnu.org/licenses/gpl.txt.
 *
 * THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
 * DISCLAIMED.  The GPL License provides additional details about this warranty
 * disclaimer.
 ******************************************************************************/

#ifndef _TEE_INTERNAL_CORE_CLIENT_API_H_
#define _TEE_INTERNAL_CORE_CLIENT_API_H_

#include "tee_internal_core_common.h"

TEE_Result TEE_OpenTASession(
	const TEE_UUID*		destination,
	uint32_t		cancellationRequestTimeout,
	uint32_t		paramTypes,
	TEE_Param		params[4],
	TEE_TASessionHandle*	session,
	uint32_t*		returnOrigin);

void TEE_CloseTASession(
	TEE_TASessionHandle	session);

TEE_Result TEE_InvokeTACommand(
	TEE_TASessionHandle	session,
	uint32_t		cancellationRequestTimeout,
	uint32_t		commandID,
	uint32_t		paramTypes,
	TEE_Param		params[4],
	uint32_t*		returnOrigin);

#endif /* _TEE_INTERNAL_CORE_CLIENT_API_H_ */
