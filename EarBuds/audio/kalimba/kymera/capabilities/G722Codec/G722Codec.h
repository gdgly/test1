/****************************************************************************
 * Copyright (c) 2014 - 2016 Qualcomm Technologies International, Ltd 
****************************************************************************/
/**
 * \file  G722Codec.h
 * \ingroup capabilities
 *
 * Stub Capability public header file. <br>
 *
 */

#ifndef G722CODEC_H
#define G722CODEC_H

#include "G722Codec_struct.h"

/** The capability data structure for stub capability */
extern const CAPABILITY_DATA G722Codec_cap_data;

/** Assembly processing function */
extern void G722Codec_proc_func(G722CODEC_OP_DATA *op_data);

#endif /* G722CODEC_H */
