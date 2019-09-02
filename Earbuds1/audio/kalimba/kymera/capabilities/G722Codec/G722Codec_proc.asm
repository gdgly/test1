#include "G722Codec_struct_asm_defs.h"

// *****************************************************************************
// MODULE:
//    $M.G722Codec_proc_func
//
// DESCRIPTION:
//    Define here your processing function
//
// INPUTS:
//   - Your input registers
//
// OUTPUTS:
//   - Your output registers
//
// TRASHED REGISTERS:
//    C calling convention respected.
//
// NOTES:
//
// *****************************************************************************
.MODULE $M.G722Codec_proc_func;
   .CODESEGMENT PM;
   .MAXIM;

$_G722Codec_proc_func:

    /*
     * TODO Assembly processing code goes here ...
     */

    r0 = M[r0 + $G722Codec_struct.G722CODEC_OP_DATA_struct.MY_DATA_FIELD];
    rts;

.ENDMODULE;
