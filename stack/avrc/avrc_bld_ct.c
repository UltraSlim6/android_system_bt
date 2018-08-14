/******************************************************************************
 *
 *  Copyright (c) 2015, The Linux Foundation. All rights reserved.
 *  Not a Contribution
 *  Copyright (C) 2006-2013 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
#include <string.h>

#include "gki.h"
#include "avrc_api.h"
#include "avrc_defs.h"
#include "avrc_int.h"

/*****************************************************************************
**  Global data
*****************************************************************************/


#if (AVRC_METADATA_INCLUDED == TRUE)
/*******************************************************************************
**
** Function         avrc_bld_next_cmd
**
** Description      This function builds the Request Continue or Abort command.
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
static tAVRC_STS avrc_bld_next_cmd (tAVRC_NEXT_CMD *p_cmd, BT_HDR *p_pkt)
{
    UINT8   *p_data, *p_start;

    AVRC_TRACE_API("avrc_bld_next_cmd");

    /* get the existing length, if any, and also the num attributes */
    p_start = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
    p_data = p_start + 2; /* pdu + rsvd */

    /* add fixed lenth 1 - pdu_id (1) */
    UINT16_TO_BE_STREAM(p_data, 1);
    UINT8_TO_BE_STREAM(p_data, p_cmd->target_pdu);
    p_pkt->len = (p_data - p_start);

    return AVRC_STS_NO_ERROR;
}

/*****************************************************************************
**  the following commands are introduced in AVRCP 1.4
*****************************************************************************/

#if (AVRC_ADV_CTRL_INCLUDED == TRUE)
/*******************************************************************************
**
** Function         avrc_bld_set_abs_volume_cmd
**
** Description      This function builds the Set Absolute Volume command.
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
static tAVRC_STS avrc_bld_set_abs_volume_cmd (tAVRC_SET_VOLUME_CMD *p_cmd, BT_HDR *p_pkt)
{
    UINT8   *p_data, *p_start;

    AVRC_TRACE_API("avrc_bld_set_abs_volume_cmd");
    /* get the existing length, if any, and also the num attributes */
    p_start = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
    p_data = p_start + 2; /* pdu + rsvd */
    /* add fixed lenth 1 - volume (1) */
    UINT16_TO_BE_STREAM(p_data, 1);
    UINT8_TO_BE_STREAM(p_data, (AVRC_MAX_VOLUME & p_cmd->volume));
    p_pkt->len = (p_data - p_start);
    return AVRC_STS_NO_ERROR;
}

/*******************************************************************************
**
** Function         avrc_bld_register_notifn
**
** Description      This function builds the register notification.
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
static tAVRC_STS avrc_bld_register_notifn(BT_HDR * p_pkt, UINT8 event_id, UINT32 event_param)
{
    UINT8   *p_data, *p_start;

    AVRC_TRACE_API("avrc_bld_register_notifn");
    /* get the existing length, if any, and also the num attributes */
    // Set the notify value
    p_start = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
    p_data = p_start + 2; /* pdu + rsvd */
    /* add fixed length 5 -*/
    UINT16_TO_BE_STREAM(p_data, 5);
    UINT8_TO_BE_STREAM(p_data,event_id);
    UINT32_TO_BE_STREAM(p_data, event_param);
    p_pkt->len = (p_data - p_start);
    return AVRC_STS_NO_ERROR;
}
/*******************************************************************************
**
** Function         avrc_bld_get_capability_cmd
**
** Description      This function builds the get capability command.
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
static tAVRC_STS avrc_bld_get_capability_cmd(BT_HDR * p_pkt, UINT8 cap_id)
{
    UINT8   *p_data, *p_start;

    AVRC_TRACE_API("avrc_bld_get_capability_cmd");
    p_start = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
    p_data = p_start + 2; /* pdu + rsvd */
    /* add fixed length 1 -*/
    UINT16_TO_BE_STREAM(p_data, 1);
    UINT8_TO_BE_STREAM(p_data,cap_id);
    p_pkt->len = (p_data - p_start);
    return AVRC_STS_NO_ERROR;
}
/*******************************************************************************
**
** Function         avrc_bld_list_player_app_attr_cmd
**
** Description      This function builds the list player app attrib command.
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
static tAVRC_STS avrc_bld_list_player_app_attr_cmd(BT_HDR * p_pkt)
{
    UINT8   *p_data, *p_start;

    AVRC_TRACE_API("avrc_bld_list_player_app_attr_cmd");
    p_start = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
    p_data = p_start + 2; /* pdu + rsvd */
    /* add fixed length 1 -*/
    UINT16_TO_BE_STREAM(p_data, 0);
    p_pkt->len = (p_data - p_start);
    return AVRC_STS_NO_ERROR;
}
/*******************************************************************************
**
** Function         avrc_bld_list_player_app_values_cmd
**
** Description      This function builds the list player app values command.
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
static tAVRC_STS avrc_bld_list_player_app_values_cmd(BT_HDR * p_pkt, UINT8 attrib_id)
{
    UINT8   *p_data, *p_start;

    AVRC_TRACE_API("avrc_bld_list_player_app_values_cmd");
    p_start = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
    p_data = p_start + 2; /* pdu + rsvd */
    /* add fixed length 1 -*/
    UINT16_TO_BE_STREAM(p_data, 1);
    UINT8_TO_BE_STREAM(p_data,attrib_id);
    p_pkt->len = (p_data - p_start);
    return AVRC_STS_NO_ERROR;
}
/*******************************************************************************
**
** Function         avrc_bld_get_current_player_app_values_cmd
**
** Description      This function builds the get current player app setting values command.
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
static tAVRC_STS avrc_bld_get_current_player_app_values_cmd(BT_HDR * p_pkt, UINT8 num_attrib_id, UINT8* attrib_ids)
{
    UINT8   *p_data, *p_start;
    UINT8 param_len = 0, count = 0;

    AVRC_TRACE_API("avrc_bld_get_current_player_app_values_cmd");
    p_start = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
    p_data = p_start + 2; /* pdu + rsvd */
    param_len = num_attrib_id + 1; // 1 additional to hold num attributes feild
    /* add length -*/
    UINT16_TO_BE_STREAM(p_data, param_len);
    UINT8_TO_BE_STREAM(p_data,num_attrib_id);
    for(count = 0; count < num_attrib_id && count < AVRC_MAX_APP_ATTR_SIZE; count ++)
    {
        UINT8_TO_BE_STREAM(p_data,attrib_ids[count]);
    }
    p_pkt->len = (p_data - p_start);
    return AVRC_STS_NO_ERROR;
}
/*******************************************************************************
**
** Function         avrc_bld_set_current_player_app_values_cmd
**
** Description      This function builds the set current player app setting values command.
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
static tAVRC_STS avrc_bld_set_current_player_app_values_cmd(BT_HDR * p_pkt, UINT8 num_attrib_id, tAVRC_APP_SETTING* p_val)
{
    UINT8   *p_data, *p_start;
    UINT8 param_len = 0, count = 0;

    AVRC_TRACE_API("avrc_bld_set_current_player_app_values_cmd");
    p_start = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
    p_data = p_start + 2; /* pdu + rsvd */
    /* we have to store attrib- value pair
     * 1 additional to store num elements
     */
    param_len = (2*num_attrib_id) + 1;
    /* add length */
    UINT16_TO_BE_STREAM(p_data, param_len);
    UINT8_TO_BE_STREAM(p_data,num_attrib_id);
    for(count = 0; count < num_attrib_id; count ++)
    {
        UINT8_TO_BE_STREAM(p_data,p_val[count].attr_id);
        UINT8_TO_BE_STREAM(p_data,p_val[count].attr_val);
    }
    p_pkt->len = (p_data - p_start);
    return AVRC_STS_NO_ERROR;
}
/*******************************************************************************
**
** Function         avrc_bld_get_element_attr_cmd
**
** Description      This function builds the get element attribute command.
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
static tAVRC_STS avrc_bld_get_element_attr_cmd(BT_HDR * p_pkt, UINT8 num_attrib, UINT32* attrib_ids)
{
    UINT8   *p_data, *p_start;
    UINT8 param_len = 0, count = 0;

    AVRC_TRACE_API("avrc_bld_get_element_attr_cmd");
    p_start = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
    p_data = p_start + 2; /* pdu + rsvd */
    /* we have to store attrib- value pair
     * 1 additional to store num elements
     */
    param_len = (4*num_attrib) + 9;
    /* add length */
    UINT16_TO_BE_STREAM(p_data, param_len);
    /* 8 bytes of identifier as 0 (playing)*/
    UINT32_TO_BE_STREAM(p_data,0);
    UINT32_TO_BE_STREAM(p_data,0);
    UINT8_TO_BE_STREAM(p_data,num_attrib);
    for(count = 0; count < num_attrib && count < AVRC_MAX_ELEM_ATTR_SIZE; count ++)
    {
        UINT32_TO_BE_STREAM(p_data,attrib_ids[count]);
    }
    p_pkt->len = (p_data - p_start);
    return AVRC_STS_NO_ERROR;
}
/*******************************************************************************
**
** Function         avrc_bld_get_play_status_cmd
**
** Description      This function builds the get play status command.
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
static tAVRC_STS avrc_bld_get_play_status_cmd(BT_HDR * p_pkt)
{
    UINT8   *p_data, *p_start;

    AVRC_TRACE_API("avrc_bld_list_player_app_attr_cmd");
    p_start = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
    p_data = p_start + 2; /* pdu + rsvd */
    /* add fixed length 1 -*/
    UINT16_TO_BE_STREAM(p_data, 0);
    p_pkt->len = (p_data - p_start);
    return AVRC_STS_NO_ERROR;
}
#endif

/*******************************************************************************
**
** Function         avrc_bld_init_cmd_buffer
**
** Description      This function initializes the command buffer based on PDU
**
** Returns          NULL, if no GKI buffer or failure to build the message.
**                  Otherwise, the GKI buffer that contains the initialized message.
**
*******************************************************************************/
static BT_HDR *avrc_bld_init_cmd_buffer(tAVRC_COMMAND *p_cmd)
{
    UINT16 offset = 0, chnl = AVCT_DATA_CTRL, len=AVRC_META_CMD_POOL_SIZE;
    BT_HDR *p_pkt=NULL;
    UINT8  opcode;

    opcode = avrc_opcode_from_pdu(p_cmd->pdu);
    AVRC_TRACE_API("avrc_bld_init_cmd_buffer: pdu=%x, opcode=%x", p_cmd->pdu, opcode);

    switch (opcode)
    {
    case AVRC_OP_PASS_THRU:
        offset  = AVRC_MSG_PASS_THRU_OFFSET;
        break;

    case AVRC_OP_VENDOR:
        offset  = AVRC_MSG_VENDOR_OFFSET;
        break;
    }

    /* allocate and initialize the buffer */
    p_pkt = (BT_HDR *)GKI_getbuf(len);
    if (p_pkt)
    {
        UINT8 *p_data, *p_start;

        p_pkt->layer_specific = chnl;
        p_pkt->event    = opcode;
        p_pkt->offset   = offset;
        p_data = (UINT8 *)(p_pkt + 1) + p_pkt->offset;
        p_start = p_data;

        /* pass thru - group navigation - has a two byte op_id, so dont do it here */
        if (opcode != AVRC_OP_PASS_THRU)
            *p_data++ = p_cmd->pdu;

        switch (opcode)
        {
        case AVRC_OP_VENDOR:
            /* reserved 0, packet_type 0 */
            UINT8_TO_BE_STREAM(p_data, 0);
            /* continue to the next "case to add length */
            /* add fixed lenth - 0 */
            UINT16_TO_BE_STREAM(p_data, 0);
            break;
        }

        p_pkt->len = (p_data - p_start);
    }
    p_cmd->cmd.opcode = opcode;
    return p_pkt;
}

/*******************************************************************************
**
** Function         AVRC_BldCommand
**
** Description      This function builds the given AVRCP command to the given
**                  GKI buffer
**
** Returns          AVRC_STS_NO_ERROR, if the command is built successfully
**                  Otherwise, the error code.
**
*******************************************************************************/
tAVRC_STS AVRC_BldCommand( tAVRC_COMMAND *p_cmd, BT_HDR **pp_pkt)
{
    tAVRC_STS status = AVRC_STS_BAD_PARAM;
    BT_HDR  *p_pkt;
    BOOLEAN alloc = FALSE;

    AVRC_TRACE_API("AVRC_BldCommand: pdu=%x status=%x", p_cmd->cmd.pdu, p_cmd->cmd.status);
    if (!p_cmd || !pp_pkt)
    {
        AVRC_TRACE_API("AVRC_BldCommand. Invalid parameters passed. p_cmd=%p, pp_pkt=%p",
            p_cmd, pp_pkt);
        return AVRC_STS_BAD_PARAM;
    }

    if (*pp_pkt == NULL)
    {
        if ((*pp_pkt = avrc_bld_init_cmd_buffer(p_cmd)) == NULL)
        {
            AVRC_TRACE_API("AVRC_BldCommand: Failed to initialize command buffer");
            return AVRC_STS_INTERNAL_ERR;
        }
        alloc = TRUE;
    }
    status = AVRC_STS_NO_ERROR;
    p_pkt = *pp_pkt;

    switch (p_cmd->pdu)
    {
    case AVRC_PDU_REQUEST_CONTINUATION_RSP:     /*        0x40 */
        status = avrc_bld_next_cmd(&p_cmd->continu, p_pkt);
        break;

    case AVRC_PDU_ABORT_CONTINUATION_RSP:       /*          0x41 */
        status = avrc_bld_next_cmd(&p_cmd->abort, p_pkt);
        break;
#if (AVRC_ADV_CTRL_INCLUDED == TRUE)
    case AVRC_PDU_SET_ABSOLUTE_VOLUME:         /* 0x50 */
        status = avrc_bld_set_abs_volume_cmd(&p_cmd->volume, p_pkt);
        break;
#endif
#if (AVRC_ADV_CTRL_INCLUDED == TRUE)
    case AVRC_PDU_REGISTER_NOTIFICATION:      /* 0x31 */
        status=avrc_bld_register_notifn(p_pkt,p_cmd->reg_notif.event_id,p_cmd->reg_notif.param);
        break;
    case AVRC_PDU_GET_CAPABILITIES:
        status = avrc_bld_get_capability_cmd(p_pkt, p_cmd->get_caps.capability_id);
        break;
    case AVRC_PDU_LIST_PLAYER_APP_ATTR:
        status = avrc_bld_list_player_app_attr_cmd(p_pkt);
        break;
    case AVRC_PDU_LIST_PLAYER_APP_VALUES:
        status = avrc_bld_list_player_app_values_cmd(p_pkt,p_cmd->list_app_values.attr_id);
        break;
    case AVRC_PDU_GET_CUR_PLAYER_APP_VALUE:
        status = avrc_bld_get_current_player_app_values_cmd(p_pkt,
             p_cmd->get_cur_app_val.num_attr,p_cmd->get_cur_app_val.attrs);
        break;
    case AVRC_PDU_SET_PLAYER_APP_VALUE:
        status = avrc_bld_set_current_player_app_values_cmd(p_pkt,
                     p_cmd->set_app_val.num_val,p_cmd->set_app_val.p_vals);
        break;
    case AVRC_PDU_GET_ELEMENT_ATTR:
        status = avrc_bld_get_element_attr_cmd(p_pkt,
              p_cmd->get_elem_attrs.num_attr,p_cmd->get_elem_attrs.attrs);
        break;
    case AVRC_PDU_GET_PLAY_STATUS:
        status = avrc_bld_get_play_status_cmd(p_pkt);
        break;
#endif
    }

    if (alloc && (status != AVRC_STS_NO_ERROR) )
    {
        GKI_freebuf(p_pkt);
        *pp_pkt = NULL;
    }
    AVRC_TRACE_API("AVRC_BldCommand: returning %d", status);
    return status;
}
#endif /* (AVRC_METADATA_INCLUDED == TRUE) */

