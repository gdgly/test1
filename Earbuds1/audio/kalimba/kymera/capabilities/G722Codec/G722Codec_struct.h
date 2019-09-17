/**
 * \file  G722Codec_struct.h
 * \ingroup capabilities
 *
 *
 */

#ifndef G722CODEC_STRUCT_H
#define G722CODEC_STRUCT_H

#include <buffer/cbuffer_c.h>
#include "ttp/ttp.h"
#include "common/ttp_utilities.h"
#include "op_msg_utilities.h"
#include "capabilities.h"


#define CHANNEL_MASK(ID)            (1<<(ID))
#define PROCESS_DATA_SAMPLES        (320)
#define G722_BYTE_PER_SAMPLE        (2)
/****************************************************************************
Public Type Definitions
*/

typedef struct G722CODEC_OP_DATA{
    /** The audio data format configurations of the input terminal */
    AUDIO_DATA_FORMAT ip_format;

    /** The audio data format configurations of the output terminal */
    AUDIO_DATA_FORMAT op_format;

    /** 1 is Encode, 0 is Decoder */
    unsigned char  is_encoder;
    unsigned char  rev;
    unsigned short rd_samples;                   // 需要分多次读取数据
    unsigned short in_samples, out_samples;

    /** The input buffer with metadata to transport from */
    tCbuffer *ip_buffer;
    tCbuffer *metadata_ip_buffer;
    /** The output buffer with metadata to transport to */
    tCbuffer *op_buffer;
    tCbuffer *metadata_op_buffer;

    /** The number of channels currently active */
    unsigned num_active_chans;

    /** Bitfield indicates which channels are currently active. */
    unsigned active_chans;

    unsigned sample_cnt;        // data count
    unsigned timestamp;       // ttp test
    unsigned sample_rate;
    ttp_context *time_to_play;
    unsigned disable_audforward;   // 0:enable, 1:disable

    unsigned *in_buffer;
    unsigned *out_buffer;

    CPS_PARAM_DEF parms_def;
    void     *g722Handle;
} G722CODEC_OP_DATA;



bool G722Codec_create(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
bool G722Codec_destroy(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
bool G722Codec_buffer_details(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
bool G722Codec_get_sched_info(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
bool G722Codec_get_data_format(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
bool G722Codec_connect(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
bool G722Codec_disconnect(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
bool G722Codec_reset(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);

bool G722Codec_opmsg_get_params(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data);
bool G722Codec_opmsg_set_params(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data);
bool G722Codec_opmsg_set_sample_rate(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data);
bool G722Codec_opmsg_input_data_type(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data);
bool G722Codec_opmsg_output_data_type(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data);
bool G722Codec_opmsg_set_buffer_size(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data);
bool G722Codec_opmsg_disable_audio_forward(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data);

#endif /* G722CODEC_H */
