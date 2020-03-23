/****************************************************************************
 * Copyright (c) 2014 - 2018 Qualcomm Technologies International, Ltd 
****************************************************************************/
/**
 * \file  G722Codec.c
 * \ingroup  capabilities
 *
 *  A Stub implementation of a Capability that can be built and communicated
 *  with. This is provided to accelerate the development of new capabilities.
 *
 */

#include "capabilities.h"
#include "G722Codec.h"
#include "codec\acodec.h"

#if 0
#define G722_DEBUG       L2_DBG_MSG
#define G722_DEBUG1      L2_DBG_MSG1
#define G722_DEBUG2      L2_DBG_MSG2
#define G722_DEBUG3      L2_DBG_MSG3
#else
#define G722_DEBUG(...)
#define G722_DEBUG1(...)
#define G722_DEBUG2(...)
#define G722_DEBUG3(...)
#endif

//#define TEST_NO_ENC       // 测试,类似于 PASS through，可以正常多次拨打电话
//#define TEST_DAT_2BYTE    // 上面定义会每个sample传输4个字节

/****************************************************************************
Private Function Definitions
*/
static void G722Codec_process_data(OPERATOR_DATA*, TOUCHED_TERMINALS*);

#define G722_ENCODE_IN_SAMPLES          (320)         // 压缩输入数据
#define G722_ENCODE_OUT_SAMPLES         (G722_ENCODE_IN_SAMPLES/16)
#define G722_DECODE_IN_SAMPLES          (20)          // 解压缩输入数据

/****************************************************************************
Private Constant Declarations
*/
#define G722CODEC_ID  0xC010 /* CHANGE THIS VALUE TO THAT SELECTED */

/** The stub capability function handler table */
const handler_lookup_struct G722Codec_handler_table =
{
    G722Codec_create,           /* OPCMD_CREATE */
    G722Codec_destroy,          /* OPCMD_DESTROY */
    base_op_start,            /* OPCMD_START */
    base_op_stop,             /* OPCMD_STOP */
    G722Codec_reset,            /* OPCMD_RESET */
    G722Codec_connect,          /* OPCMD_CONNECT */
    G722Codec_disconnect,       /* OPCMD_DISCONNECT */
    G722Codec_buffer_details,   /* OPCMD_BUFFER_DETAILS */
    G722Codec_get_data_format,  /* OPCMD_DATA_FORMAT */
    G722Codec_get_sched_info    /* OPCMD_GET_SCHED_INFO */
};

/* Null terminated operator message handler table - this is the set of operator
 * messages that the capability understands and will attempt to service. */
const opmsg_handler_lookup_table_entry G722Codec_opmsg_handler_table[] =
{
    {OPMSG_COMMON_ID_GET_CAPABILITY_VERSION, base_op_opmsg_get_capability_version},
    {OPMSG_PASSTHROUGH_ID_CHANGE_INPUT_DATA_TYPE,     G722Codec_opmsg_input_data_type},
    {OPMSG_PASSTHROUGH_ID_CHANGE_OUTPUT_DATA_TYPE,    G722Codec_opmsg_output_data_type},
    {OPMSG_COMMON_SET_SAMPLE_RATE,                 G722Codec_opmsg_set_sample_rate},
    {OPMSG_COMMON_ID_SET_BUFFER_SIZE,            G722Codec_opmsg_set_buffer_size},
    {OPMSG_COMMON_ID_GET_PARAMS,                   G722Codec_opmsg_get_params},
    {OPMSG_COMMON_ID_SET_PARAMS,                   G722Codec_opmsg_set_params},
    {OPMSG_PASSTHROUGH_ID_DISABLE_AUDIO_FORWARD,   G722Codec_opmsg_disable_audio_forward},
    {0, NULL}
};


/* Capability data - This is the definition of the capability that Opmgr uses to
 * create the capability from. */
const CAPABILITY_DATA G722Codec_cap_data =
{
    G722CODEC_ID,             /* Capability ID */
    0, 1,                           /* Version information - hi and lo parts */
    1, 1,                           /* Max number of sinks/inputs and sources/outputs */
    &G722Codec_handler_table, /* Pointer to message handler function table */
    G722Codec_opmsg_handler_table,    /* Pointer to operator message handler function table */
    G722Codec_process_data,           /* Pointer to data processing function */
    0,                              /* TODO: this would hold processing time information */
    sizeof(G722CODEC_OP_DATA)       /* Size of capability-specific per-instance data */
};
MAP_INSTANCE_DATA(G722CODEC_ID, G722CODEC_OP_DATA)

/* Accessing the capability-specific per-instance data function */
static inline G722CODEC_OP_DATA *get_instance_data(OPERATOR_DATA *op_data)
{
    return (G722CODEC_OP_DATA *) base_op_get_instance_data(op_data);
}


static signed G722Codec_metadata_read(G722CODEC_OP_DATA *opx_data, tCbuffer *src, unsigned octets)
{
    metadata_tag *ret_mtag = NULL;
    unsigned b4idx, afteridx;

    patch_fn_shared(buff_metadata);

    if (octets == 0 || src == NULL)  {
        G722_DEBUG("metadata_strict_transport: ignoring zero transfer");
        return -1;
    }

    ret_mtag = buff_metadata_remove(src, octets*OCTETS_PER_SAMPLE, &b4idx, &afteridx);
//    G722_DEBUG3("get ret_mtag=%p %d %d", ret_mtag, b4idx, afteridx);
    buff_metadata_tag_list_delete(ret_mtag);

    cbuffer_read(opx_data->ip_buffer, (int*)&opx_data->in_buffer[opx_data->rd_samples], octets);
    opx_data->rd_samples += (unsigned short)octets;
    return 0;
}


static signed G722Codec_metadata_write(G722CODEC_OP_DATA *opx_data, tCbuffer *dst)
{
    metadata_tag *new_mtag = NULL;

    new_mtag = buff_metadata_new_tag();
    if(!new_mtag) {
        G722_DEBUG("Fail No Enough memory for newtag");
    }
    else {
        METADATA_PACKET_START_SET(new_mtag);
        METADATA_PACKET_END_SET(new_mtag);
#ifdef TEST_NO_ENC
        new_mtag->length = opx_data->in_samples * OCTETS_PER_SAMPLE;
  #ifdef TEST_DAT_2BYTE
        new_mtag->length = opx_data->in_samples * 2;
  #endif
#else
        new_mtag->length = opx_data->out_samples * G722_BYTE_PER_SAMPLE;        // bytePERsample = 2
#endif

//        new_mtag->timestamp = ret_mtag->timestamp;
//        new_mtag->sp_adjust = ret_mtag->sp_adjust;
        buff_metadata_append(dst, new_mtag, 0, new_mtag->length);
    }

    return 0;
}

static void G722Codec_processing_encode(G722CODEC_OP_DATA *opx_data, unsigned octets)
{
    signed short i, *ptr, outsize = G722_ENCODE_OUT_SAMPLES * G722_BYTE_PER_SAMPLE;

#ifdef TEST_NO_ENC
    (void)i; (void)ptr; (void)outsize;
  #ifdef TEST_DAT_2BYTE
    ptr = (signed short *)opx_data->in_buffer;
    for(i = 0; i < octets; i++) {
           ptr[i*2] = ptr[i*2+1]; // 在前端为SPLITER 时我们是取 ptr[i*2+1]
    }
    cbuffer_write(opx_data->op_buffer, (int*)opx_data->in_buffer,  octets);
  #else
    cbuffer_write(opx_data->op_buffer, (int*)opx_data->in_buffer, octets);

    acodec_encoder(opx_data->g722Handle, (unsigned char*)opx_data->in_buffer, (short)(opx_data->in_samples*G722_BYTE_PER_SAMPLE),
                   (unsigned char*)opx_data->out_buffer, (unsigned short*)&outsize);
  #endif

#else
    // 将每个SAMPLE为4个字节转换为2个字节
    ptr = (signed short *)opx_data->in_buffer;
    for(i = 0; i < octets; i++) {
     //  ptr[i] = ptr[i*2];   // 在前端为模拟音频时我们是取 ptr[i*2]
           ptr[i] = ptr[i*2+1]; // 在前端为SPLITER 时我们是取 ptr[i*2+1]
    }
    G722_DEBUG1("encode intime=%d", time_get_time());

    acodec_encoder(opx_data->g722Handle, (unsigned char*)opx_data->in_buffer, (short)(opx_data->in_samples*G722_BYTE_PER_SAMPLE),
                   (unsigned char*)opx_data->out_buffer, (unsigned short*)&outsize);

    // 将每个SAMPLE为2个字节转换为4个字节
    ptr = (signed short *)opx_data->out_buffer;
    for(i = (signed short)(opx_data->out_samples-1); i >= 0; i--)
        ptr[i*2] = ptr[i];

    cbuffer_write(opx_data->op_buffer, (int*)opx_data->out_buffer, opx_data->out_samples);
    G722_DEBUG1("encode outtime=%d", time_get_time());
#endif
}


/* Data processing function */
// 使用有多少数据先读取出来，然后再进行编码
static void G722Codec_process_data(OPERATOR_DATA *op_data, TOUCHED_TERMINALS *touched)
{
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);
    unsigned in_data, output_space, max_read;

    // 本次最多读取数据量
    max_read = opx_data->in_samples - opx_data->rd_samples;

    in_data = cbuffer_calc_amount_data_in_words(opx_data->ip_buffer);
    if(in_data < 1) return;

    if(max_read > in_data)
        max_read = in_data;

    G722Codec_metadata_read(opx_data, opx_data->ip_buffer, max_read);

    if(opx_data->disable_audforward) {
        opx_data->rd_samples = 0;
        touched->sources = TOUCHED_SOURCE_0;
        return;
    }

    output_space = cbuffer_calc_amount_space_in_words(opx_data->op_buffer);
    G722_DEBUG3("G722Codec_process in=%d/%d read=%d", in_data, max_read, opx_data->rd_samples);
    if (output_space < opx_data->out_samples) {
        touched->sinks   = TOUCHED_SINK_0;
        return;
    }

    if(opx_data->rd_samples >= opx_data->in_samples) {    // only support ==
        G722Codec_processing_encode(opx_data, opx_data->in_samples);
        G722Codec_metadata_write(opx_data, opx_data->op_buffer);
        opx_data->rd_samples = 0;
    }

    /* We have got this far, so we have something to do on every output */
    touched->sources = TOUCHED_SOURCE_0;
    touched->sinks   = TOUCHED_SINK_0;
}

static void G722Codec_setmode(G722CODEC_OP_DATA *opx_data, bool is_encoder)
{
    opx_data->is_encoder           = (unsigned char)is_encoder;
    if(is_encoder) {
        opx_data->in_samples       = G722_ENCODE_IN_SAMPLES;
        opx_data->out_samples      = G722_ENCODE_IN_SAMPLES / 16;
    }
    else {       // decoder
        opx_data->in_samples       = G722_DECODE_IN_SAMPLES;
        opx_data->out_samples      = G722_DECODE_IN_SAMPLES * 16;
    }

#ifdef TEST_NO_ENC
    // 不编码时，每次不能超过256 Sample
    opx_data->in_samples           = 180;
    opx_data->out_samples          = 180;
#endif
}

static void G722Codec_Init(G722CODEC_OP_DATA *opx_data)
{
    opx_data->sample_rate      = 16000;
    opx_data->ip_format        = AUDIO_DATA_FORMAT_FIXP;
    opx_data->op_format        = AUDIO_DATA_FORMAT_16_BIT;

    opx_data->num_active_chans = 0;
    opx_data->active_chans     = 0;
    opx_data->disable_audforward=TRUE;

    opx_data->sample_cnt       = 0;
    opx_data->timestamp        = 0; // time_get_time();
    G722Codec_setmode(opx_data, 1);
    opx_data->in_buffer        = (unsigned *)xzpmalloc(opx_data->in_samples * 4);
    opx_data->out_buffer       = (unsigned *)xzpmalloc(opx_data->out_samples *4);
    opx_data->rd_samples       = 0;
}

bool G722Codec_create(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);

    L2_DBG_MSG1("G722Codec_create %d", __LINE__);
    /* call base_op create, which also allocates and fills response message */
    if (!base_op_create(op_data, message_data, response_id, response_data))   {
        return FALSE;
    }

    G722Codec_Init(opx_data);
#ifdef INSTALL_TTP
    {
        /* Allocate and initialise the TTP data */
        opx_data->time_to_play = ttp_init();
        if (opx_data->time_to_play != NULL) {
            ttp_params params;
            ttp_get_default_params(&params, TTP_TYPE_PCM);
            ttp_configure_latency(opx_data->time_to_play, 4000);   // 4000us = 64 samples
            ttp_configure_params(opx_data->time_to_play, &params);
            ttp_configure_rate(opx_data->time_to_play, opx_data->sample_rate);
        }
        else    {
            /* TTP init failed, so fail the operator creation */
            base_op_change_response_status(response_data, STATUS_CMD_FAILED);
            return TRUE;
        }
    }
#endif

    if(10) {
        int memsize = acodec_memsize();
        L2_DBG_MSG1("G722Codec_create memsize=%d", memsize);

        memsize = (memsize / 16 + 1) * 16;
        opx_data->g722Handle = (void*)xzpmalloc(memsize);
        opx_data->g722Handle = (void*)acodec_init((void*)opx_data->g722Handle, memsize);
    }

    return TRUE;
}

bool G722Codec_destroy(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);

    pdelete(opx_data->in_buffer), opx_data->in_buffer = NULL;
    pdelete(opx_data->out_buffer), opx_data->out_buffer = NULL;
    pdelete(opx_data->g722Handle), opx_data->g722Handle = NULL;
    L2_DBG_MSG("G722Codec_destroy");

#ifdef INSTALL_TTP
    /* Time to play pointer will only be non-NULL if this is the TTP_PASS capability
     * It's safe to call ttp_free whether it's NULL or not.
     */
    ttp_free(opx_data->time_to_play);
#endif
    /* call base_op destroy that creates and fills response message, too */
    return base_op_destroy(op_data, message_data, response_id, response_data);
}

bool G722Codec_buffer_details(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    unsigned terminal_id = OPMGR_GET_OP_BUF_DETAILS_TERMINAL_ID(message_data);
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);

    if (!base_op_buffer_details(op_data, message_data, response_id, response_data))
    {
        return FALSE;
    }

    ((OP_BUF_DETAILS_RSP*)*response_data)->supports_metadata = TRUE;
    if((terminal_id & TERMINAL_SINK_MASK) == 0)
        ((OP_BUF_DETAILS_RSP*)*response_data)->b.buffer_size = opx_data->out_samples * 6;   // op
    else
       ((OP_BUF_DETAILS_RSP*)*response_data)->b.buffer_size = opx_data->in_samples * 6;    // ip

    if(((OP_BUF_DETAILS_RSP*)*response_data)->b.buffer_size < 240)
        ((OP_BUF_DETAILS_RSP*)*response_data)->b.buffer_size = 240;

    L2_DBG_MSG2("G722Codec_buffer_details format=%d id=%x",
                ((OP_BUF_DETAILS_RSP*)*response_data)->b.buffer_size, terminal_id);

    return TRUE;
}

bool G722Codec_get_data_format(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);

    if (!base_op_get_data_format(op_data, message_data, response_id, response_data))
        return FALSE;

    /* return the terminal's data format - purely based on terminal data direction flag */
    if((OPMGR_GET_OP_DATA_FORMAT_TERMINAL_ID(message_data) & TERMINAL_SINK_MASK) == 0)
         ((OP_STD_RSP*)*response_data)->resp_data.data = opx_data->op_format;
    else
         ((OP_STD_RSP*)*response_data)->resp_data.data = opx_data->ip_format;

    L2_DBG_MSG1("G722Codec_get_data_format format=%d", ((OP_STD_RSP*)*response_data)->resp_data.data);
    return TRUE;
}

bool G722Codec_get_sched_info(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    OP_SCHED_INFO_RSP* resp;
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);

    resp = base_op_get_sched_info_ex(op_data, message_data, response_id);
    if (resp == NULL)  {
        return base_op_build_std_response_ex(op_data, STATUS_CMD_FAILED, response_data);
    }
    *response_data = resp;

    resp->run_period = 10;
    resp->block_size = opx_data->in_samples;
    L2_DBG_MSG1("G722Codec_get_sched_info %d", resp->block_size);

    return TRUE;
}

bool G722Codec_connect(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);
    unsigned terminal_id = OPMGR_GET_OP_CONNECT_TERMINAL_ID(message_data);
    unsigned channel;

    if (!base_op_build_std_response_ex(op_data, STATUS_OK, response_data))  {
        /* Shouldn't change anything if there is not enough memory for the response.*/
        return FALSE;
    }

    channel = terminal_id & (~TERMINAL_SINK_MASK);
    if (channel >= 1)  {
        base_op_change_response_status(response_data, STATUS_INVALID_CMD_PARAMS);
        return TRUE;
    }

    /* There is already a structure representing this channel so populate
     * it with this connection. If it's already populated this is an error
     */
    if (terminal_id & TERMINAL_SINK_MASK)    {
        if (opx_data->ip_buffer == NULL)   {
            opx_data->ip_buffer = OPMGR_GET_OP_CONNECT_BUFFER(message_data);
            if (opx_data->metadata_ip_buffer == NULL)   {
                if (buff_has_metadata(opx_data->ip_buffer))     {
                    opx_data->metadata_ip_buffer = opx_data->ip_buffer;
                    L2_DBG_MSG2("G722Codec_connect ipbuf size=%d ptr=%p", opx_data->metadata_ip_buffer->size,
                        opx_data->metadata_ip_buffer->base_addr);
                }
            }
        }
        else   {   // 已经获取分配过内存
            base_op_change_response_status(response_data, STATUS_CMD_FAILED);
            return TRUE;
        }
    }
    else  {       // Source
        if (opx_data->op_buffer == NULL)   {
            opx_data->op_buffer = OPMGR_GET_OP_CONNECT_BUFFER(message_data);
            if (opx_data->metadata_op_buffer == NULL) {
                if (buff_has_metadata(opx_data->op_buffer)) {
                    opx_data->metadata_op_buffer = opx_data->op_buffer;
                    /* Set the usable octets for the metadata. */
                    cbuffer_set_usable_octets(opx_data->metadata_op_buffer, 2);
#ifdef TEST_NO_ENC
   #ifndef TEST_DAT_2BYTE
                    cbuffer_set_usable_octets(opx_data->metadata_op_buffer, 4);
   #endif
#endif
                    L2_DBG_MSG2("G722Codec_connect opbuf size=%d ptr=%p", opx_data->metadata_op_buffer->size,
                        opx_data->metadata_op_buffer->base_addr);
                }
            }
        }
        else {   // 已经获取分配过内存
            base_op_change_response_status(response_data, STATUS_CMD_FAILED);
            return TRUE;
        }
    }

    L2_DBG_MSG1("G722Codec_connect id=%d", terminal_id);

    return TRUE;
}


bool G722Codec_disconnect(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);
    unsigned terminal_id = OPMGR_GET_OP_DISCONNECT_TERMINAL_ID(message_data);

    if (!base_op_build_std_response_ex(op_data, STATUS_OK, response_data))  {
        /* Shouldn't change anything if there is not enough memory for the response.*/
        return FALSE;
    }

    /* Channel structure found */
    if (terminal_id & TERMINAL_SINK_MASK)          {
        if (opx_data->ip_buffer != NULL)      {
#ifdef INSTALL_METADATA
            opx_data->metadata_ip_buffer = NULL;
#endif /* INSTALL_METADATA */
            opx_data->ip_buffer = NULL;
        }
    }
    else {
        if (opx_data->op_buffer != NULL)      {
#ifdef INSTALL_METADATA
            opx_data->metadata_op_buffer = NULL;
#endif /* INSTALL_METADATA */
            opx_data->op_buffer = NULL;
        }
    }

    L2_DBG_MSG("G722Codec_disconnect %d");

    return TRUE;
}

bool G722Codec_reset(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);

    G722Codec_Init(opx_data);
    acodec_reset(opx_data->g722Handle);
    if (!base_op_reset(op_data, message_data, response_id, response_data)) {
        return FALSE;
    }

    L2_DBG_MSG("G722Codec_reset");
    return TRUE;
}

/*///////////////////////////////////////////////////////////////////////////////////////////////
    Capability 参数设置部分
/////////////////////////////////////////////////////////////////////////////////////////////////*/
bool G722Codec_opmsg_set_params(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
//    bool retval;
//    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);
//    retval = cpsSetParameterMsgHandler(&op_extra_data->parms_def ,message_data, resp_length,resp_data);

    return TRUE;
}

bool G722Codec_opmsg_get_params(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
//    bool retval;
//    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);
//    retval = cpsSetParameterMsgHandler(&op_extra_data->parms_def ,message_data, resp_length,resp_data);

    return TRUE;
}

bool G722Codec_opmsg_set_sample_rate(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    return TRUE;
}


bool G722Codec_opmsg_set_buffer_size(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    return TRUE;
}


bool G722Codec_opmsg_input_data_type(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);

     if (opmgr_op_is_running(op_data)) {
         return FALSE;
     }

     opx_data->ip_format = OPMSG_FIELD_GET(message_data, OPMSG_PASSTHROUGH_CHANGE_INPUT_DATA_TYPE, DATA_TYPE);

     return TRUE;
}

bool G722Codec_opmsg_output_data_type(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);

     if (opmgr_op_is_running(op_data)) {
         return FALSE;
     }

     opx_data->op_format = OPMSG_FIELD_GET(message_data, OPMSG_PASSTHROUGH_CHANGE_INPUT_DATA_TYPE, DATA_TYPE);

     return TRUE;
}

/*
 * basic_passthrough_disable_audio_forward
 */
bool G722Codec_opmsg_disable_audio_forward(OPERATOR_DATA *op_data, void *message_data,
                                             unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    G722CODEC_OP_DATA *opx_data = get_instance_data(op_data);

    opx_data->disable_audforward = (bool)OPMSG_FIELD_GET(message_data, OPMSG_PASSTHROUGH_DISABLE_AUDIO_FORWARD, DISABLE);

   return TRUE;
}


