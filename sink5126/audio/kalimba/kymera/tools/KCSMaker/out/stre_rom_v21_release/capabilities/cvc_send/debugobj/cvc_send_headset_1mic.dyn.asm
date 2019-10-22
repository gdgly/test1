/****************************************************************
 *     Dynamic Memory Configuration                              
 *     Created by kdynmem : version  2905332 $
 ****************************************************************/ 

.CONST $DYN_SECTION_TYPE_ALLOC_INST      0x0000;
.CONST $DYN_SECTION_TYPE_ALLOC_SCRATCH   0x0010;
.CONST $DYN_SECTION_TYPE_ALLOC_SHARE     0x0020;
.CONST $DYN_SECTION_TYPE_DATA_INST       0x0001;
.CONST $DYN_SECTION_TYPE_DATA_SHARE      0x0021;
.CONST $DYN_SECTION_TYPE_RELOC_INST      0x0002;
.CONST $DYN_SECTION_TYPE_RELOC_SCRATCH   0x0012;
.CONST $DYN_SECTION_TYPE_RELOC_ROOT      0x0004;
.CONST $DYN_SECTION_TYPE_BOUNDARY        0x0040;
.CONST $DYN_EXPORT_CONSTANTS_CMD         0x0080;
.CONST $DYN_SECTION_TYPE_END             0x5432;


#if !defined(EXPORTED_CONSTANTS_ONCHIP_BUILD) || EXPORTED_CONSTANTS_ONCHIP_BUILD
.MODULE $M.CVC_SEND_CAP.headset_data_1mic.Downloadable; 
	.DATASEGMENT DM; 

	.VAR16/CONST16 DynTable_Main[] = 
	/* If constants are moved off-chip to a file, the code needs 
	  to reference this variable */
.set $_CVC_SEND_CAP_headset_data_1mic_DownloadableDynTable_Main, DynTable_Main
#if !defined(EXPORTED_CONSTANTS_ONCHIP_BUILD) || EXPORTED_CONSTANTS_ONCHIP_BUILD 
		 // Variant 0x0000: 4128 words allocated, 102 not used
		 //	Size 	DM0 		DM1 		DM2
		 //	  6 	 1 		 0 		 0
		 //	1032 	 0 		 3 		 1
		 //
		 //	Block[00] Bank=DM0 Size=6 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=22 
		 //		 [I] DM1?$M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj Size=22
		 //		 [I] DM1?$M.CVC_SEND_CAP.headset_data_1mic.fba_ref Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.aec_obj Size=127
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.dms200_obj Size=93
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable Size=58
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd Size=58
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp Size=41
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.snd_harm_obj Size=38
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd Size=25
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.root Size=22
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.StatusArray Size=20
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.ref_vad_peq Size=19
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.ref_vad410 Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.oms_wnr_obj Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout_va1 Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.stream_map_left_adc Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.stream_map_refin Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.aed100_obj Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va Size=10
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.vad_default_param Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.fft_obj Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.dms_obj Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.block?0 Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.fba_left Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.snd_streams Size=5
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.vad_peq_output Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.AEC_Et Size=3
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.AEC_Dt Size=3
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_1mic.ref_delay_buffer Size=240
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.vad_agc_obj Size=5
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.BExp_X_buf Size=3
		 //	Block[02] Bank=DM2 Size=1032  Available=21 
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_1mic.snd_peq_dm2 Size=23
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_1mic.in_l_dcblock_dm2 Size=15
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1 Size=7
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_1mic.fbs_snd Size=7
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_1mic.Ga_real Size=130
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_1mic.RcvBuf_imag Size=130
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_1mic.bufd_aec_inp Size=120
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_1mic.bufd_l_inp Size=120
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_1mic.L2absGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.dms_state Size=256
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.CurParams Size=139
		 //	Block[03] Bank=DM1 Size=1032  Available=31 
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_1mic.Ga_imag Size=130
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_1mic.RcvBuf_real Size=130
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.harm_history Size=180
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.bufd_outp_va1 Size=180
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.bufd_outp Size=180
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.sndLpX_queue Size=136
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.BExp_Ga Size=65
		 //	Block[04] Bank=DM1 Size=1032  Available=28 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.Cng_Nz_Shape_Tab Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.LPwrX1 Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.LpZ_nz Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.LPwrX0 Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.G_dmsZ Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.SqGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.LPwrD Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.Gr_real Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.Gr_imag Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.RatFE Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.cvc_send_internal_streams Size=35
		 //		*[U] SM1?$M.CVC_SEND_CAP.headset_data_1mic.FFT_DM1 Size=260
		 //		*[U] SM1?$M.CVC_SEND_CAP.headset_data_1mic.Et_real Size=64
		 $DYN_SECTION_TYPE_ALLOC_INST,0x0000,10,
		 0x0005,0x0000,0x0001,0x0006,0x03F2,0x0201,0x03F3,0x03E9,
		 0x0100,0x03EC,
		 // Variant 0x0000: 2064 words Scratch allocated, 417 not used
		 //	Size 	SM0 		SM1 		SM2
		 //	  4 	 1 		 0 		 0
		 //	1032 	 0 		 1 		 1
		 //
		 //	Block[00] Bank=DM0 Size=4 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=273 
		 //		 SM1?$dm1_scratch Size=630
		 //		 SM1?$M.CVC_SEND_CAP.headset_data_1mic.Dt_real Size=64
		 //		 SM?$M.CVC_SEND_CAP.headset_data_1mic.DTC_lin Size=65
		 //	Block[02] Bank=DM2 Size=1032  Available=144 
		 //		 SM2?$dm2_scratch Size=630
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_1mic.FFT_DM2 Size=130
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_1mic.Et_imag Size=64
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_1mic.Dt_imag Size=64
		 $DYN_SECTION_TYPE_ALLOC_SCRATCH,0x0000,7,
		 0x0003,0x0001,0x0001,0x0004,0x02F7,0x0200,0x0378,
		 // Variant 0x0001: 6192 words allocated, 283 not used
		 //	Size 	DM0 		DM1 		DM2
		 //	  8 	 1 		 0 		 0
		 //	1032 	 1 		 3 		 2
		 //
		 //	Block[00] Bank=DM0 Size=8 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=4 
		 //		 [I] DM1?$M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj Size=22
		 //		 [I] DM1?$M.CVC_SEND_CAP.headset_data_1mic.fba_ref Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.aec_obj Size=127
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.dms200_obj Size=93
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable Size=58
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd Size=58
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp Size=41
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.snd_harm_obj Size=38
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd Size=25
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.root Size=22
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.StatusArray Size=20
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.ref_vad_peq Size=19
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.ref_vad410 Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.oms_wnr_obj Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout_va1 Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.stream_map_left_adc Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.stream_map_refin Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.aed100_obj Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va Size=10
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.vad_default_param Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.fft_obj Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.dms_obj Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.block?0 Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.fba_left Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.snd_streams Size=5
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.vad_peq_output Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.AEC_Et Size=3
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_1mic.AEC_Dt Size=3
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_1mic.Ga_imag Size=258
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.vad_agc_obj Size=5
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.BExp_X_buf Size=3
		 //	Block[02] Bank=DM2 Size=1032  Available=21 
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_1mic.snd_peq_dm2 Size=23
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_1mic.in_l_dcblock_dm2 Size=15
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1 Size=7
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_1mic.fbs_snd Size=7
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_1mic.Ga_real Size=258
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_1mic.RcvBuf_imag Size=258
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_1mic.bufd_aec_inp Size=240
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_1mic.L2absGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.CurParams Size=139
		 //	Block[03] Bank=DM1 Size=1032  Available=3 
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_1mic.ref_delay_buffer Size=480
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_1mic.RcvBuf_real Size=258
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.dms_state Size=256
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.cvc_send_internal_streams Size=35
		 //	Block[04] Bank=DM2 Size=1032  Available=8 
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_1mic.bufd_l_inp Size=240
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.harm_history Size=360
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.bufd_outp_va1 Size=360
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.SqGr Size=64
		 //	Block[05] Bank=DM0 Size=1032  Available=51 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.bufd_outp Size=360
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.sndLpX_queue Size=170
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.BExp_Ga Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.Cng_Nz_Shape_Tab Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.LPwrX1 Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.LPwrD Size=64
		 //	Block[06] Bank=DM1 Size=1032  Available=196 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.LpZ_nz Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.LPwrX0 Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.G_dmsZ Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.Gr_real Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.Gr_imag Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_1mic.RatFE Size=64
		 //		*[U] SM1?$M.CVC_SEND_CAP.headset_data_1mic.Et_real Size=64
		 //		*[U] SM1?$M.CVC_SEND_CAP.headset_data_1mic.Dt_real Size=64
		 //		*[U] SM?$M.CVC_SEND_CAP.headset_data_1mic.DTC_lin Size=129
		 $DYN_SECTION_TYPE_ALLOC_INST,0x0001,13,
		 0x0007,0x0000,0x0001,0x0008,0x0404,0x0201,0x03F3,0x0405,
		 0x0200,0x0400,0x03D5,0x0100,0x0344,
		 // Variant 0x0001: 3096 words Scratch allocated, 934 not used
		 //	Size 	SM0 		SM1 		SM2
		 //	  5 	 1 		 0 		 0
		 //	1032 	 0 		 2 		 1
		 //
		 //	Block[00] Bank=DM0 Size=5 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=402 
		 //		 SM1?$dm1_scratch Size=630
		 //	Block[02] Bank=DM1 Size=1032  Available=516 
		 //		 SM1?$M.CVC_SEND_CAP.headset_data_1mic.FFT_DM1 Size=516
		 //	Block[03] Bank=DM2 Size=1032  Available=16 
		 //		 SM2?$dm2_scratch Size=630
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_1mic.FFT_DM2 Size=258
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_1mic.Et_imag Size=64
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_1mic.Dt_imag Size=64
		 $DYN_SECTION_TYPE_ALLOC_SCRATCH,0x0001,8,
		 0x0004,0x0001,0x0001,0x0005,0x0276,0x0102,0x0204,0x03F8,
		 // Variant 0xFFFF: Iniiatization Data 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][46] = 0x0 
		 // Word Offset 0x10000 , Length=2
		 // Word Offset 0x1001F , Length=1
		 // Word Offset 0x100A6 , Length=3
		 // Word Offset 0x100F9 , Length=1
		 // Word Offset 0x1020C , Length=1
		 // Word Offset 0x10211 , Length=1
		 // Word Offset 0x10238 , Length=1
		 // Word Offset 0x102BF , Length=9
		 // Word Offset 0x102D1 , Length=1
		 // Word Offset 0x102EE , Length=1
		 // Word Offset 0x20002 , Length=1
		 // Word Offset 0x20019 , Length=1
		 // Word Offset 0x2002B , Length=1
		 // Word Offset 0x20032 , Length=2
		 $DYN_SECTION_TYPE_DATA_INST,0xFFFF,80,
		 0x0102,0x0000,0x0000,0x0001,0x0000,0x0008,0x0101,0x001F,
		 0x01EB,0x851F,0x0103,0x00A6,0x3999,0x999A,0x0083,0x126F,
		 0x0000,0x0001,0x0101,0x00F9,0x0000,0x0001,0x0101,0x020C,
		 0x0000,0x5E0C,0x0101,0x0211,0x7FFF,0xFFFF,0x0101,0x0238,
		 0x0000,0x0003,0x0109,0x02BF,0x0600,0x0000,0x0600,0x0000,
		 0x0010,0x624E,0x0063,0xD70A,0x4000,0x0000,0x0080,0x0000,
		 0xFD40,0x0000,0x0050,0x0000,0x00E5,0x6042,0x0101,0x02D1,
		 0x0000,0x0001,0x0101,0x02EE,0x0000,0x0780,0x0201,0x0002,
		 0x0000,0x0005,0x0201,0x0019,0x0000,0x0001,0x0201,0x002B,
		 0x0000,0x0001,0x0202,0x0032,0x0000,0x0001,0xFFFF,0xFFFF,
		 // Variant 0x0000: Iniiatization Data 
		 //	[1,524: $M.CVC_SEND_CAP.headset_data_1mic.root][17] = 0xE829FB 
		 //	[1,748: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream][2] = 0x3C0 
		 // Word Offset 0x1021D , Length=1
		 // Word Offset 0x102EE , Length=1
		 $DYN_SECTION_TYPE_DATA_INST,0x0000,8,
		 0x0101,0x021D,0x00E8,0x29FB,0x0101,0x02EE,0x0000,0x03C0,
		 // Variant 0x0001: Iniiatization Data 
		 //	[1,524: $M.CVC_SEND_CAP.headset_data_1mic.root][17] = 0xE829FB 
		 // Word Offset 0x1021D , Length=1
		 $DYN_SECTION_TYPE_DATA_INST,0x0001,4,
		 0x0101,0x021D,0x00E8,0x29FB,
		 // Variant 0xFFFF: 119 Internal Links to Resolve
		 //	Root[2] = [1,645: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_refin] + 0 
		 //	Root[3] = [1,632: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_left_adc] + 0 
		 //	Root[7] = [1,658: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout] + 0 
		 //	Root[8] = [1,619: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout_va1] + 0 
		 //	Root[22] = [1,694: $M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd] + 0 
		 //	Root[23] = [1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable] + 0 
		 //	Root[24] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 0 
		 //	Root[25] = [1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray] + 0 
		 //	Root[45] = [1,734: $M.CVC_SEND_CAP.headset_data_1mic.D0] + 0 
		 //	[1,22: $M.CVC_SEND_CAP.headset_data_1mic.fba_ref][1] = [1,748: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream] + 0 
		 //	[1,22: $M.CVC_SEND_CAP.headset_data_1mic.fba_ref][3] = [1,712: $M.CVC_SEND_CAP.headset_data_1mic.fft_obj] + 0 
		 //	[1,22: $M.CVC_SEND_CAP.headset_data_1mic.fba_ref][4] = [1,730: $M.CVC_SEND_CAP.headset_data_1mic.X] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][9] = [1,748: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][10] = [1,730: $M.CVC_SEND_CAP.headset_data_1mic.X] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][15] = [1,734: $M.CVC_SEND_CAP.headset_data_1mic.D0] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][36] = [1,759: $M.CVC_SEND_CAP.headset_data_1mic.AEC_Dt] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][37] = [1,756: $M.CVC_SEND_CAP.headset_data_1mic.AEC_Et] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][0] = [1,734: $M.CVC_SEND_CAP.headset_data_1mic.D0] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][1] = [1,734: $M.CVC_SEND_CAP.headset_data_1mic.D0] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][15] = [1,721: $M.CVC_SEND_CAP.headset_data_1mic.dms_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][4] = [2,23: $M.CVC_SEND_CAP.headset_data_1mic.in_l_dcblock_dm2] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][14] = [2,45: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][17] = [2,38: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][20] = [1,737: $M.CVC_SEND_CAP.headset_data_1mic.fba_left] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][23] = [1,405: $M.CVC_SEND_CAP.headset_data_1mic.snd_harm_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][26] = [1,0: $M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][29] = [1,585: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad410] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][32] = [1,22: $M.CVC_SEND_CAP.headset_data_1mic.fba_ref] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][35] = [1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][38] = [1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][41] = [1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][43] = [1,602: $M.CVC_SEND_CAP.headset_data_1mic.oms_wnr_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][44] = [1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][46] = [1,730: $M.CVC_SEND_CAP.headset_data_1mic.X] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][47] = [1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][50] = [1,671: $M.CVC_SEND_CAP.headset_data_1mic.aed100_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][52] = [2,0: $M.CVC_SEND_CAP.headset_data_1mic.snd_peq_dm2] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable][56] = [1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][1] = [1,743: $M.CVC_SEND_CAP.headset_data_1mic.snd_streams] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][7] = [1,645: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_refin] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][10] = [1,632: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_left_adc] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][13] = [2,23: $M.CVC_SEND_CAP.headset_data_1mic.in_l_dcblock_dm2] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][23] = [1,22: $M.CVC_SEND_CAP.headset_data_1mic.fba_ref] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][26] = [1,585: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad410] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][28] = [1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][29] = [1,585: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad410] + 9 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][32] = [1,405: $M.CVC_SEND_CAP.headset_data_1mic.snd_harm_obj] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][35] = [1,737: $M.CVC_SEND_CAP.headset_data_1mic.fba_left] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][38] = [1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][41] = [1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][44] = [1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][47] = [1,0: $M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][50] = [1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][53] = [1,684: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][55] = [1,743: $M.CVC_SEND_CAP.headset_data_1mic.snd_streams] + 0 
		 //	[1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp][0] = [1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj] + 0 
		 //	[1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp][2] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 6 
		 //	[1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp][3] = [1,585: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad410] + 9 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][2] = [1,671: $M.CVC_SEND_CAP.headset_data_1mic.aed100_obj] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][5] = [1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][8] = [1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][11] = [2,45: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][13] = [2,0: $M.CVC_SEND_CAP.headset_data_1mic.snd_peq_dm2] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][20] = [1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][26] = [1,658: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][1] = [1,743: $M.CVC_SEND_CAP.headset_data_1mic.snd_streams] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][4] = [1,645: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_refin] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][7] = [1,632: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_left_adc] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][17] = [1,658: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][20] = [1,619: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout_va1] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][22] = [1,743: $M.CVC_SEND_CAP.headset_data_1mic.snd_streams] + 0 
		 //	[1,524: $M.CVC_SEND_CAP.headset_data_1mic.root][3] = [1,405: $M.CVC_SEND_CAP.headset_data_1mic.snd_harm_obj] + 0 
		 //	[1,524: $M.CVC_SEND_CAP.headset_data_1mic.root][4] = [1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj] + 0 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][0] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 0 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][4] = [1,0: $M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj] + 6 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][5] = [1,0: $M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj] + 5 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][6] = [1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm] + 7 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][7] = [1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm] + 15 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][8] = [1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj] + 84 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][9] = [1,602: $M.CVC_SEND_CAP.headset_data_1mic.oms_wnr_obj] + 8 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][10] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 6 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][11] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 12 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][12] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 6 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][13] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 6 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][17] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 6 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][18] = [1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp] + 31 
		 //	[1,566: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad_peq][0] = [1,748: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream] + 0 
		 //	[1,566: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad_peq][1] = [1,752: $M.CVC_SEND_CAP.headset_data_1mic.vad_peq_output] + 0 
		 //	[1,585: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad410][0] = [1,748: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream] + 0 
		 //	[1,585: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad410][1] = [1,703: $M.CVC_SEND_CAP.headset_data_1mic.vad_default_param] + 0 
		 //	[1,602: $M.CVC_SEND_CAP.headset_data_1mic.oms_wnr_obj][1] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 6 
		 //	[1,602: $M.CVC_SEND_CAP.headset_data_1mic.oms_wnr_obj][2] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 14 
		 //	[1,602: $M.CVC_SEND_CAP.headset_data_1mic.oms_wnr_obj][3] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 12 
		 //	[1,671: $M.CVC_SEND_CAP.headset_data_1mic.aed100_obj][0] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 14 
		 //	[1,684: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va][2] = [2,38: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1] + 0 
		 //	[1,684: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va][8] = [1,619: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout_va1] + 0 
		 //	[1,694: $M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd][0] = [1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,694: $M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd][1] = [1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,694: $M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd][2] = [1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd] + 0 
		 //	[1,694: $M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd][3] = [1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd] + 0 
		 //	[1,694: $M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd][4] = [1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,694: $M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd][5] = [1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,694: $M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd][6] = [1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,694: $M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd][7] = [1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,694: $M.CVC_SEND_CAP.headset_data_1mic.ModeProcTableSnd][8] = [1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,721: $M.CVC_SEND_CAP.headset_data_1mic.dms_obj][1] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 8 
		 //	[1,721: $M.CVC_SEND_CAP.headset_data_1mic.dms_obj][3] = [1,524: $M.CVC_SEND_CAP.headset_data_1mic.root] + 18 
		 //	[1,729: $M.CVC_SEND_CAP.headset_data_1mic.block?0][3] = [1,729: $M.CVC_SEND_CAP.headset_data_1mic.BExp_X] + 0 
		 //	[1,729: $M.CVC_SEND_CAP.headset_data_1mic.block?0][7] = [1,733: $M.CVC_SEND_CAP.headset_data_1mic.BExp_D0] + 0 
		 //	[1,737: $M.CVC_SEND_CAP.headset_data_1mic.fba_left][3] = [1,712: $M.CVC_SEND_CAP.headset_data_1mic.fft_obj] + 0 
		 //	[1,737: $M.CVC_SEND_CAP.headset_data_1mic.fba_left][4] = [1,734: $M.CVC_SEND_CAP.headset_data_1mic.D0] + 0 
		 //	[1,743: $M.CVC_SEND_CAP.headset_data_1mic.snd_streams][0] = [1,658: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout] + 0 
		 //	[1,743: $M.CVC_SEND_CAP.headset_data_1mic.snd_streams][1] = [1,645: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_refin] + 0 
		 //	[1,743: $M.CVC_SEND_CAP.headset_data_1mic.snd_streams][2] = [1,632: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_left_adc] + 0 
		 //	[1,743: $M.CVC_SEND_CAP.headset_data_1mic.snd_streams][3] = [1,619: $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout_va1] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1][3] = [1,712: $M.CVC_SEND_CAP.headset_data_1mic.fft_obj] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1][4] = [1,730: $M.CVC_SEND_CAP.headset_data_1mic.X] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd][3] = [1,712: $M.CVC_SEND_CAP.headset_data_1mic.fft_obj] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd][4] = [1,734: $M.CVC_SEND_CAP.headset_data_1mic.D0] + 0 
		 $DYN_SECTION_TYPE_RELOC_INST,0xFFFF,360,
		 0x0077,0x0000,0x0000,0x0001,0x0002,0x0285,0x0001,0x0003,
		 0x0278,0x0001,0x0007,0x0292,0x0001,0x0008,0x026B,0x0001,
		 0x0016,0x02B6,0x0001,0x0017,0x00F8,0x0001,0x0018,0x020C,
		 0x0001,0x0019,0x0222,0x0001,0x002D,0x02DE,0x0101,0x0017,
		 0x02EC,0x0101,0x0019,0x02C8,0x0101,0x001A,0x02DA,0x0101,
		 0x0025,0x02EC,0x0101,0x0026,0x02DA,0x0101,0x002B,0x02DE,
		 0x0101,0x0040,0x02F7,0x0101,0x0041,0x02F4,0x0101,0x009B,
		 0x02DE,0x0101,0x009C,0x02DE,0x0101,0x00AA,0x02D1,0x0102,
		 0x00FC,0x0017,0x0102,0x0106,0x002D,0x0102,0x0109,0x0026,
		 0x0101,0x010C,0x02E1,0x0101,0x010F,0x0195,0x0101,0x0112,
		 0x0000,0x0101,0x0115,0x0249,0x0101,0x0118,0x0016,0x0101,
		 0x011B,0x001C,0x0101,0x011E,0x016C,0x0101,0x0121,0x009B,
		 0x0101,0x0123,0x025A,0x0101,0x0124,0x009B,0x0101,0x0126,
		 0x02DA,0x0101,0x0127,0x009B,0x0101,0x012A,0x029F,0x0102,
		 0x012C,0x0000,0x0101,0x0130,0x01D7,0x0101,0x0133,0x02E7,
		 0x0101,0x0139,0x0285,0x0101,0x013C,0x0278,0x0102,0x013F,
		 0x0017,0x0101,0x0149,0x0016,0x0101,0x014C,0x0249,0x0101,
		 0x014E,0x016C,0x0101,0x014F,0x0252,0x0101,0x0152,0x0195,
		 0x0101,0x0155,0x02E1,0x0101,0x0158,0x001C,0x0101,0x015B,
		 0x009B,0x0101,0x015E,0x009B,0x0101,0x0161,0x0000,0x0101,
		 0x0164,0x01BB,0x0101,0x0167,0x02AC,0x0101,0x0169,0x02E7,
		 0x0101,0x016C,0x001C,0x0101,0x016E,0x0212,0x0101,0x016F,
		 0x0252,0x0101,0x01BD,0x029F,0x0101,0x01C0,0x016C,0x0101,
		 0x01C3,0x001C,0x0102,0x01C6,0x002D,0x0102,0x01C8,0x0000,
		 0x0101,0x01CF,0x01D7,0x0101,0x01D5,0x0292,0x0101,0x01F4,
		 0x02E7,0x0101,0x01F7,0x0285,0x0101,0x01FA,0x0278,0x0101,
		 0x0204,0x0292,0x0101,0x0207,0x026B,0x0101,0x0209,0x02E7,
		 0x0101,0x020F,0x0195,0x0101,0x0210,0x009B,0x0101,0x0222,
		 0x020C,0x0101,0x0226,0x0006,0x0101,0x0227,0x0005,0x0101,
		 0x0228,0x01DE,0x0101,0x0229,0x01E6,0x0101,0x022A,0x0070,
		 0x0101,0x022B,0x0262,0x0101,0x022C,0x0212,0x0101,0x022D,
		 0x0218,0x0101,0x022E,0x0212,0x0101,0x022F,0x0212,0x0101,
		 0x0233,0x0212,0x0101,0x0234,0x018B,0x0101,0x0236,0x02EC,
		 0x0101,0x0237,0x02F0,0x0101,0x0249,0x02EC,0x0101,0x024A,
		 0x02BF,0x0101,0x025B,0x0212,0x0101,0x025C,0x021A,0x0101,
		 0x025D,0x0218,0x0101,0x029F,0x021A,0x0102,0x02AE,0x0026,
		 0x0101,0x02B4,0x026B,0x0101,0x02B6,0x01F3,0x0101,0x02B7,
		 0x01F3,0x0101,0x02B8,0x0132,0x0101,0x02B9,0x0132,0x0101,
		 0x02BA,0x01F3,0x0101,0x02BB,0x01F3,0x0101,0x02BC,0x01F3,
		 0x0101,0x02BD,0x01F3,0x0101,0x02BE,0x01F3,0x0101,0x02D2,
		 0x0214,0x0101,0x02D4,0x021E,0x0101,0x02DC,0x02D9,0x0101,
		 0x02E0,0x02DD,0x0101,0x02E4,0x02C8,0x0101,0x02E5,0x02DE,
		 0x0101,0x02E7,0x0292,0x0101,0x02E8,0x0285,0x0101,0x02E9,
		 0x0278,0x0101,0x02EA,0x026B,0x0201,0x0029,0x02C8,0x0201,
		 0x002A,0x02DA,0x0201,0x0030,0x02C8,0x0201,0x0031,0x02DE,
		 // Variant 0xFFFF: 9 Links from Root to Resolve
		 //	Root[27] = &$M.CVC_SEND_CAP.headset_data_1mic.cvclib_table, 1 references to resolve
		 //		[1,155] $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj[5] = ROOT[27] 
		 //	Root[28] = &$M.CVC_SEND_CAP.headset_data_1mic.fft_split, 1 references to resolve
		 //		[1,712] $M.CVC_SEND_CAP.headset_data_1mic.fft_obj[4] = ROOT[28] 
		 //	Root[30] = &$M.CVC_SEND_CAP.headset_data_1mic.dms_mode_object, 1 references to resolve
		 //		[1,155] $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj[3] = ROOT[30] 
		 //	Root[32] = &$M.CVC_SEND_CAP.headset_data_1mic.vad_dc_coeffs, 2 references to resolve
		 //		[1,566] $M.CVC_SEND_CAP.headset_data_1mic.ref_vad_peq[3] = ROOT[32] + 10 
		 //		[2,23] $M.CVC_SEND_CAP.headset_data_1mic.in_l_dcblock_dm2[3] = ROOT[32] 
		 //	Root[33] = &$M.CVC_SEND_CAP.headset_data_1mic.aec_mode_object, 1 references to resolve
		 //		[1,28] $M.CVC_SEND_CAP.headset_data_1mic.aec_obj[0] = ROOT[33] 
		 //	Root[34] = &$M.CVC_SEND_CAP.headset_data_1mic.fb_configuration, 4 references to resolve
		 //		[1,22] $M.CVC_SEND_CAP.headset_data_1mic.fba_ref[0] = ROOT[34] 
		 //		[1,737] $M.CVC_SEND_CAP.headset_data_1mic.fba_left[0] = ROOT[34] 
		 //		[2,38] $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1[0] = ROOT[34] 
		 //		[2,45] $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd[0] = ROOT[34] 
		 //	Root[40] = &$M.CVC_SEND_CAP.headset_data_1mic.cap_root_ptr, 10 references to resolve
		 //		[1,0] $M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj[2] = ROOT[40] + 47 
		 //		[1,28] $M.CVC_SEND_CAP.headset_data_1mic.aec_obj[2] = ROOT[40] + 47 
		 //		[1,155] $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj[2] = ROOT[40] + 47 
		 //		[1,405] $M.CVC_SEND_CAP.headset_data_1mic.snd_harm_obj[1] = ROOT[40] + 47 
		 //		[1,471] $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm[4] = ROOT[40] + 47 
		 //		[1,499] $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd[10] = ROOT[40] + 12 
		 //		[1,499] $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd[13] = ROOT[40] + 12 
		 //		[1,524] $M.CVC_SEND_CAP.headset_data_1mic.root[1] = ROOT[40] 
		 //		[1,585] $M.CVC_SEND_CAP.headset_data_1mic.ref_vad410[2] = ROOT[40] + 47 
		 //		[1,671] $M.CVC_SEND_CAP.headset_data_1mic.aed100_obj[1] = ROOT[40] + 47 
		 //	Root[54] = &$M.CVC_SEND_CAP.headset_data_1mic.mute_control_ptr, 1 references to resolve
		 //		[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[22] = ROOT[54] 
		 //	Root[58] = &$M.CVC_SEND_CAP.headset_data_1mic.ext_frame_size, 4 references to resolve
		 //		[1,619] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout_va1[5] = ROOT[58] 
		 //		[1,632] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_left_adc[5] = ROOT[58] 
		 //		[1,645] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_refin[5] = ROOT[58] 
		 //		[1,658] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout[5] = ROOT[58] 
		 $DYN_SECTION_TYPE_RELOC_ROOT,0xFFFF,93,
		 0x001B,0x0001,0x0001,0x00A0,0x0000,0x001C,0x0001,0x0001,
		 0x02CC,0x0000,0x001E,0x0001,0x0001,0x009E,0x0000,0x0020,
		 0x0002,0x0001,0x0239,0x000A,0x0002,0x001A,0x0000,0x0021,
		 0x0001,0x0001,0x001C,0x0000,0x0022,0x0004,0x0001,0x0016,
		 0x0000,0x0001,0x02E1,0x0000,0x0002,0x0026,0x0000,0x0002,
		 0x002D,0x0000,0x0028,0x000A,0x0001,0x0002,0x002F,0x0001,
		 0x001E,0x002F,0x0001,0x009D,0x002F,0x0001,0x0196,0x002F,
		 0x0001,0x01DB,0x002F,0x0001,0x01FD,0x000C,0x0001,0x0200,
		 0x000C,0x0001,0x020D,0x0000,0x0001,0x024B,0x002F,0x0001,
		 0x02A0,0x002F,0x0036,0x0001,0x0001,0x01D1,0x0000,0x003A,
		 0x0004,0x0001,0x0270,0x0000,0x0001,0x027D,0x0000,0x0001,
		 0x028A,0x0000,0x0001,0x0297,0x0000,
		 // Variant 0x0000: 83 Internal Links to Resolve
		 //	Root[12] = [4,645: $M.CVC_SEND_CAP.headset_data_1mic.cvc_send_internal_streams] + 0 
		 //	Root[26] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj][3] = [4,875: $M.CVC_SEND_CAP.headset_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj][4] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 2 
		 //	[1,22: $M.CVC_SEND_CAP.headset_data_1mic.fba_ref][2] = [2,312: $M.CVC_SEND_CAP.headset_data_1mic.bufd_aec_inp] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][1] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 90 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][6] = [4,260: $M.CVC_SEND_CAP.headset_data_1mic.G_dmsZ] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][7] = [4,875: $M.CVC_SEND_CAP.headset_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][8] = [4,645: $M.CVC_SEND_CAP.headset_data_1mic.stream_x] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][11] = [3,130: $M.CVC_SEND_CAP.headset_data_1mic.RcvBuf_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][12] = [2,182: $M.CVC_SEND_CAP.headset_data_1mic.RcvBuf_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][13] = [1,1007: $M.CVC_SEND_CAP.headset_data_1mic.BExp_X_buf] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][16] = [2,52: $M.CVC_SEND_CAP.headset_data_1mic.Ga_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][17] = [3,0: $M.CVC_SEND_CAP.headset_data_1mic.Ga_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][18] = [3,936: $M.CVC_SEND_CAP.headset_data_1mic.BExp_Ga] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][20] = [4,195: $M.CVC_SEND_CAP.headset_data_1mic.LPwrX0] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][21] = [4,65: $M.CVC_SEND_CAP.headset_data_1mic.LPwrX1] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][22] = [4,581: $M.CVC_SEND_CAP.headset_data_1mic.RatFE] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][23] = [4,517: $M.CVC_SEND_CAP.headset_data_1mic.Gr_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][24] = [4,453: $M.CVC_SEND_CAP.headset_data_1mic.Gr_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][25] = [4,325: $M.CVC_SEND_CAP.headset_data_1mic.SqGr] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][26] = [2,552: $M.CVC_SEND_CAP.headset_data_1mic.L2absGr] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][27] = [4,389: $M.CVC_SEND_CAP.headset_data_1mic.LPwrD] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][28] = [4,130: $M.CVC_SEND_CAP.headset_data_1mic.LpZ_nz] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][29] = [4,0: $M.CVC_SEND_CAP.headset_data_1mic.Cng_Nz_Shape_Tab] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][32] = [4,810: $M.CVC_SEND_CAP.headset_data_1mic.AttenuationPersist] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][4] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 79 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][6] = [3,800: $M.CVC_SEND_CAP.headset_data_1mic.sndLpX_queue] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][7] = [4,260: $M.CVC_SEND_CAP.headset_data_1mic.G_dmsZ] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][8] = [4,875: $M.CVC_SEND_CAP.headset_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][9] = [2,616: $M.CVC_SEND_CAP.headset_data_1mic.dms_state] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][17] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 78 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][8] = [4,645: $M.CVC_SEND_CAP.headset_data_1mic.stream_x] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][11] = [4,650: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp][1] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 108 
		 //	[1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp][4] = [4,810: $M.CVC_SEND_CAP.headset_data_1mic.AttenuationPersist] + 0 
		 //	[1,405: $M.CVC_SEND_CAP.headset_data_1mic.snd_harm_obj][0] = [4,650: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[1,405: $M.CVC_SEND_CAP.headset_data_1mic.snd_harm_obj][2] = [3,260: $M.CVC_SEND_CAP.headset_data_1mic.harm_history] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][16] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][17] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][23] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][25] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm][0] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 49 
		 //	[1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm][1] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm][2] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm][3] = [1,1002: $M.CVC_SEND_CAP.headset_data_1mic.vad_agc_obj] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][5] = [4,645: $M.CVC_SEND_CAP.headset_data_1mic.stream_x] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][8] = [4,650: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][11] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][14] = [4,675: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][16] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][19] = [4,675: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[1,524: $M.CVC_SEND_CAP.headset_data_1mic.root][2] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 0 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][1] = [4,650: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][2] = [4,655: $M.CVC_SEND_CAP.headset_data_1mic.stream_d1] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][3] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][14] = [4,660: $M.CVC_SEND_CAP.headset_data_1mic.stream_d2] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][15] = [4,665: $M.CVC_SEND_CAP.headset_data_1mic.stream_d3] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][16] = [4,645: $M.CVC_SEND_CAP.headset_data_1mic.stream_x] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][19] = [4,675: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 4 
		 //	[1,585: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad410][3] = [1,566: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad_peq] + 0 
		 //	[1,602: $M.CVC_SEND_CAP.headset_data_1mic.oms_wnr_obj][0] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 71 
		 //	[1,684: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va][4] = [4,675: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[1,684: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va][5] = [4,675: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[1,684: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va][7] = [4,675: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[1,721: $M.CVC_SEND_CAP.headset_data_1mic.dms_obj][2] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 69 
		 //	[1,729: $M.CVC_SEND_CAP.headset_data_1mic.block?0][1] = [4,745: $M.CVC_SEND_CAP.headset_data_1mic.X_real] + 0 
		 //	[1,729: $M.CVC_SEND_CAP.headset_data_1mic.block?0][5] = [4,680: $M.CVC_SEND_CAP.headset_data_1mic.D_l_real] + 0 
		 //	[1,737: $M.CVC_SEND_CAP.headset_data_1mic.fba_left][1] = [4,650: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[1,737: $M.CVC_SEND_CAP.headset_data_1mic.fba_left][2] = [2,432: $M.CVC_SEND_CAP.headset_data_1mic.bufd_l_inp] + 0 
		 //	[1,737: $M.CVC_SEND_CAP.headset_data_1mic.fba_left][5] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 137 
		 //	[1,748: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream][0] = [1,762: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_buffer] + 0 
		 //	[1,748: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream][3] = [1,762: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_buffer] + 0 
		 //	[1,756: $M.CVC_SEND_CAP.headset_data_1mic.AEC_Et][0] = [4,940: $M.CVC_SEND_CAP.headset_data_1mic.Et_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_1mic.snd_peq_dm2][0] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_1mic.snd_peq_dm2][1] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_1mic.snd_peq_dm2][3] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 10 
		 //	[2,23: $M.CVC_SEND_CAP.headset_data_1mic.in_l_dcblock_dm2][0] = [4,650: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[2,23: $M.CVC_SEND_CAP.headset_data_1mic.in_l_dcblock_dm2][1] = [4,650: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1][1] = [4,675: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1][2] = [3,440: $M.CVC_SEND_CAP.headset_data_1mic.bufd_outp_va1] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd][1] = [4,670: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd][2] = [3,620: $M.CVC_SEND_CAP.headset_data_1mic.bufd_outp] + 0 
		 $DYN_SECTION_TYPE_RELOC_INST,0x0000,252,
		 0x0053,0x0000,0x0000,0x0004,0x000C,0x0285,0x0002,0x001A,
		 0x0368,0x0104,0x0003,0x036B,0x0102,0x0004,0x036A,0x0102,
		 0x0018,0x0138,0x0102,0x001D,0x03C2,0x0104,0x0022,0x0104,
		 0x0104,0x0023,0x036B,0x0104,0x0024,0x0285,0x0103,0x0027,
		 0x0082,0x0102,0x0028,0x00B6,0x0101,0x0029,0x03EF,0x0102,
		 0x002C,0x0034,0x0103,0x002D,0x0000,0x0103,0x002E,0x03A8,
		 0x0104,0x0030,0x00C3,0x0104,0x0031,0x0041,0x0104,0x0032,
		 0x0245,0x0104,0x0033,0x0205,0x0104,0x0034,0x01C5,0x0104,
		 0x0035,0x0145,0x0102,0x0036,0x0228,0x0104,0x0037,0x0185,
		 0x0104,0x0038,0x0082,0x0104,0x0039,0x0000,0x0104,0x003C,
		 0x032A,0x0102,0x009F,0x03B7,0x0103,0x00A1,0x0320,0x0104,
		 0x00A2,0x0104,0x0104,0x00A3,0x036B,0x0102,0x00A4,0x0268,
		 0x0102,0x00AC,0x03B6,0x0104,0x013A,0x0285,0x0104,0x013D,
		 0x028A,0x0102,0x016D,0x03D4,0x0104,0x0170,0x032A,0x0104,
		 0x0195,0x028A,0x0103,0x0197,0x0104,0x0104,0x01CB,0x029E,
		 0x0104,0x01CC,0x029E,0x0104,0x01D2,0x029E,0x0104,0x01D4,
		 0x029E,0x0102,0x01D7,0x0399,0x0104,0x01D8,0x029E,0x0104,
		 0x01D9,0x029E,0x0101,0x01DA,0x03EA,0x0104,0x01F8,0x0285,
		 0x0104,0x01FB,0x028A,0x0104,0x01FE,0x029E,0x0104,0x0201,
		 0x02A3,0x0104,0x0203,0x029E,0x0104,0x0206,0x02A3,0x0102,
		 0x020E,0x0368,0x0104,0x0223,0x028E,0x0104,0x0224,0x0293,
		 0x0104,0x0225,0x02A2,0x0104,0x0230,0x0298,0x0104,0x0231,
		 0x029D,0x0104,0x0232,0x0289,0x0104,0x0235,0x02A7,0x0101,
		 0x024C,0x0236,0x0102,0x025A,0x03AF,0x0104,0x02B0,0x02A3,
		 0x0104,0x02B1,0x02A3,0x0104,0x02B3,0x02A3,0x0102,0x02D3,
		 0x03AD,0x0104,0x02DA,0x02E9,0x0104,0x02DE,0x02A8,0x0104,
		 0x02E2,0x028A,0x0102,0x02E3,0x01B0,0x0102,0x02E6,0x03F1,
		 0x0101,0x02EC,0x02FA,0x0101,0x02EF,0x02FA,0x0104,0x02F4,
		 0x03AC,0x0204,0x0000,0x029E,0x0204,0x0001,0x029E,0x0202,
		 0x0003,0x0372,0x0204,0x0017,0x028A,0x0204,0x0018,0x028A,
		 0x0204,0x0027,0x02A3,0x0203,0x0028,0x01B8,0x0204,0x002E,
		 0x029E,0x0203,0x002F,0x026C,
		 // Variant 0x0000: 15 Internal Links to Resolve (Scratch)
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][30] = [1,131: $M.CVC_SEND_CAP.headset_data_1mic.L_adaptA] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][31] = [2,0: $M.CVC_SEND_CAP.headset_data_1mic.Exp_Mts_adapt] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][33] = [1,0: $M.CVC_SEND_CAP.headset_data_1mic.W_ri] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][34] = [1,196: $M.CVC_SEND_CAP.headset_data_1mic.L_adaptR] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][35] = [1,694: $M.CVC_SEND_CAP.headset_data_1mic.DTC_lin] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][10] = [1,0: $dm1_scratch] + 0 
		 //	[1,712: $M.CVC_SEND_CAP.headset_data_1mic.fft_obj][1] = [1,0: $dm1_scratch] + 0 
		 //	[1,712: $M.CVC_SEND_CAP.headset_data_1mic.fft_obj][2] = [2,0: $dm2_scratch] + 0 
		 //	[1,712: $M.CVC_SEND_CAP.headset_data_1mic.fft_obj][3] = [1,0: $dm1_scratch] + 128 
		 //	[1,729: $M.CVC_SEND_CAP.headset_data_1mic.block?0][2] = [2,695: $M.CVC_SEND_CAP.headset_data_1mic.X_imag] + 0 
		 //	[1,729: $M.CVC_SEND_CAP.headset_data_1mic.block?0][6] = [2,630: $M.CVC_SEND_CAP.headset_data_1mic.D_l_imag] + 0 
		 //	[1,752: $M.CVC_SEND_CAP.headset_data_1mic.vad_peq_output][0] = [1,0: $dm1_scratch] + 0 
		 //	[1,756: $M.CVC_SEND_CAP.headset_data_1mic.AEC_Et][1] = [2,760: $M.CVC_SEND_CAP.headset_data_1mic.Et_imag] + 0 
		 //	[1,759: $M.CVC_SEND_CAP.headset_data_1mic.AEC_Dt][0] = [1,630: $M.CVC_SEND_CAP.headset_data_1mic.Dt_real] + 0 
		 //	[1,759: $M.CVC_SEND_CAP.headset_data_1mic.AEC_Dt][1] = [2,824: $M.CVC_SEND_CAP.headset_data_1mic.Dt_imag] + 0 
		 $DYN_SECTION_TYPE_RELOC_SCRATCH,0x0000,48,
		 0x000F,0x0000,0x0001,0x0101,0x003A,0x0083,0x0102,0x003B,
		 0x0000,0x0101,0x003D,0x0000,0x0101,0x003E,0x00C4,0x0101,
		 0x003F,0x02B6,0x0101,0x00A5,0x0000,0x0101,0x02C9,0x0000,
		 0x0102,0x02CA,0x0000,0x0101,0x02CB,0x0080,0x0102,0x02DB,
		 0x02B7,0x0102,0x02DF,0x0276,0x0101,0x02F0,0x0000,0x0102,
		 0x02F5,0x02F8,0x0101,0x02F7,0x0276,0x0102,0x02F8,0x0338,
		 // Variant 0x0001: 78 Internal Links to Resolve
		 //	Root[12] = [3,994: $M.CVC_SEND_CAP.headset_data_1mic.cvc_send_internal_streams] + 0 
		 //	Root[26] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj][4] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 2 
		 //	[1,22: $M.CVC_SEND_CAP.headset_data_1mic.fba_ref][2] = [2,568: $M.CVC_SEND_CAP.headset_data_1mic.bufd_aec_inp] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][1] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 90 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][6] = [6,258: $M.CVC_SEND_CAP.headset_data_1mic.G_dmsZ] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][8] = [3,994: $M.CVC_SEND_CAP.headset_data_1mic.stream_x] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][11] = [3,480: $M.CVC_SEND_CAP.headset_data_1mic.RcvBuf_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][12] = [2,310: $M.CVC_SEND_CAP.headset_data_1mic.RcvBuf_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][13] = [1,1025: $M.CVC_SEND_CAP.headset_data_1mic.BExp_X_buf] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][16] = [2,52: $M.CVC_SEND_CAP.headset_data_1mic.Ga_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][17] = [1,762: $M.CVC_SEND_CAP.headset_data_1mic.Ga_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][18] = [5,530: $M.CVC_SEND_CAP.headset_data_1mic.BExp_Ga] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][20] = [6,129: $M.CVC_SEND_CAP.headset_data_1mic.LPwrX0] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][21] = [5,788: $M.CVC_SEND_CAP.headset_data_1mic.LPwrX1] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][22] = [6,515: $M.CVC_SEND_CAP.headset_data_1mic.RatFE] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][23] = [6,451: $M.CVC_SEND_CAP.headset_data_1mic.Gr_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][24] = [6,387: $M.CVC_SEND_CAP.headset_data_1mic.Gr_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][25] = [4,960: $M.CVC_SEND_CAP.headset_data_1mic.SqGr] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][26] = [2,808: $M.CVC_SEND_CAP.headset_data_1mic.L2absGr] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][27] = [5,917: $M.CVC_SEND_CAP.headset_data_1mic.LPwrD] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][28] = [6,0: $M.CVC_SEND_CAP.headset_data_1mic.LpZ_nz] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][29] = [5,659: $M.CVC_SEND_CAP.headset_data_1mic.Cng_Nz_Shape_Tab] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][35] = [6,707: $M.CVC_SEND_CAP.headset_data_1mic.DTC_lin] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][4] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 79 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][6] = [5,360: $M.CVC_SEND_CAP.headset_data_1mic.sndLpX_queue] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][7] = [6,258: $M.CVC_SEND_CAP.headset_data_1mic.G_dmsZ] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][9] = [3,738: $M.CVC_SEND_CAP.headset_data_1mic.dms_state] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][17] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 78 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][8] = [3,994: $M.CVC_SEND_CAP.headset_data_1mic.stream_x] + 0 
		 //	[1,306: $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd][11] = [3,999: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp][1] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 108 
		 //	[1,405: $M.CVC_SEND_CAP.headset_data_1mic.snd_harm_obj][0] = [3,999: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[1,405: $M.CVC_SEND_CAP.headset_data_1mic.snd_harm_obj][2] = [4,240: $M.CVC_SEND_CAP.headset_data_1mic.harm_history] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][16] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][17] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][23] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,443: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice][25] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm][0] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 49 
		 //	[1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm][1] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm][2] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,471: $M.CVC_SEND_CAP.headset_data_1mic.snd_agc400_dm][3] = [1,1020: $M.CVC_SEND_CAP.headset_data_1mic.vad_agc_obj] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][5] = [3,994: $M.CVC_SEND_CAP.headset_data_1mic.stream_x] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][8] = [3,999: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][11] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][14] = [3,1024: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][16] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[1,499: $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd][19] = [3,1024: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[1,524: $M.CVC_SEND_CAP.headset_data_1mic.root][2] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 0 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][1] = [3,999: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][2] = [3,1004: $M.CVC_SEND_CAP.headset_data_1mic.stream_d1] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][3] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][14] = [3,1009: $M.CVC_SEND_CAP.headset_data_1mic.stream_d2] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][15] = [3,1014: $M.CVC_SEND_CAP.headset_data_1mic.stream_d3] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][16] = [3,994: $M.CVC_SEND_CAP.headset_data_1mic.stream_x] + 4 
		 //	[1,546: $M.CVC_SEND_CAP.headset_data_1mic.StatusArray][19] = [3,1024: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 4 
		 //	[1,585: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad410][3] = [1,566: $M.CVC_SEND_CAP.headset_data_1mic.ref_vad_peq] + 0 
		 //	[1,602: $M.CVC_SEND_CAP.headset_data_1mic.oms_wnr_obj][0] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 71 
		 //	[1,684: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va][4] = [3,1024: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[1,684: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va][5] = [3,1024: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[1,684: $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va][7] = [3,1024: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[1,721: $M.CVC_SEND_CAP.headset_data_1mic.dms_obj][2] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 69 
		 //	[1,737: $M.CVC_SEND_CAP.headset_data_1mic.fba_left][1] = [3,999: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[1,737: $M.CVC_SEND_CAP.headset_data_1mic.fba_left][2] = [4,0: $M.CVC_SEND_CAP.headset_data_1mic.bufd_l_inp] + 0 
		 //	[1,737: $M.CVC_SEND_CAP.headset_data_1mic.fba_left][5] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 137 
		 //	[1,748: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream][0] = [3,0: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_buffer] + 0 
		 //	[1,748: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_stream][3] = [3,0: $M.CVC_SEND_CAP.headset_data_1mic.ref_delay_buffer] + 0 
		 //	[1,756: $M.CVC_SEND_CAP.headset_data_1mic.AEC_Et][0] = [6,579: $M.CVC_SEND_CAP.headset_data_1mic.Et_real] + 0 
		 //	[1,759: $M.CVC_SEND_CAP.headset_data_1mic.AEC_Dt][0] = [6,643: $M.CVC_SEND_CAP.headset_data_1mic.Dt_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_1mic.snd_peq_dm2][0] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_1mic.snd_peq_dm2][1] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_1mic.snd_peq_dm2][3] = [2,872: $M.CVC_SEND_CAP.headset_data_1mic.CurParams] + 10 
		 //	[2,23: $M.CVC_SEND_CAP.headset_data_1mic.in_l_dcblock_dm2][0] = [3,999: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[2,23: $M.CVC_SEND_CAP.headset_data_1mic.in_l_dcblock_dm2][1] = [3,999: $M.CVC_SEND_CAP.headset_data_1mic.stream_d0] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1][1] = [3,1024: $M.CVC_SEND_CAP.headset_data_1mic.stream_z1] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd_va1][2] = [4,600: $M.CVC_SEND_CAP.headset_data_1mic.bufd_outp_va1] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd][1] = [3,1019: $M.CVC_SEND_CAP.headset_data_1mic.stream_z0] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.headset_data_1mic.fbs_snd][2] = [5,0: $M.CVC_SEND_CAP.headset_data_1mic.bufd_outp] + 0 
		 $DYN_SECTION_TYPE_RELOC_INST,0x0001,237,
		 0x004E,0x0000,0x0000,0x0003,0x000C,0x03E2,0x0002,0x001A,
		 0x0368,0x0102,0x0004,0x036A,0x0102,0x0018,0x0238,0x0102,
		 0x001D,0x03C2,0x0106,0x0022,0x0102,0x0103,0x0024,0x03E2,
		 0x0103,0x0027,0x01E0,0x0102,0x0028,0x0136,0x0101,0x0029,
		 0x0401,0x0102,0x002C,0x0034,0x0101,0x002D,0x02FA,0x0105,
		 0x002E,0x0212,0x0106,0x0030,0x0081,0x0105,0x0031,0x0314,
		 0x0106,0x0032,0x0203,0x0106,0x0033,0x01C3,0x0106,0x0034,
		 0x0183,0x0104,0x0035,0x03C0,0x0102,0x0036,0x0328,0x0105,
		 0x0037,0x0395,0x0106,0x0038,0x0000,0x0105,0x0039,0x0293,
		 0x0106,0x003F,0x02C3,0x0102,0x009F,0x03B7,0x0105,0x00A1,
		 0x0168,0x0106,0x00A2,0x0102,0x0103,0x00A4,0x02E2,0x0102,
		 0x00AC,0x03B6,0x0103,0x013A,0x03E2,0x0103,0x013D,0x03E7,
		 0x0102,0x016D,0x03D4,0x0103,0x0195,0x03E7,0x0104,0x0197,
		 0x00F0,0x0103,0x01CB,0x03FB,0x0103,0x01CC,0x03FB,0x0103,
		 0x01D2,0x03FB,0x0103,0x01D4,0x03FB,0x0102,0x01D7,0x0399,
		 0x0103,0x01D8,0x03FB,0x0103,0x01D9,0x03FB,0x0101,0x01DA,
		 0x03FC,0x0103,0x01F8,0x03E2,0x0103,0x01FB,0x03E7,0x0103,
		 0x01FE,0x03FB,0x0103,0x0201,0x0400,0x0103,0x0203,0x03FB,
		 0x0103,0x0206,0x0400,0x0102,0x020E,0x0368,0x0103,0x0223,
		 0x03EB,0x0103,0x0224,0x03F0,0x0103,0x0225,0x03FF,0x0103,
		 0x0230,0x03F5,0x0103,0x0231,0x03FA,0x0103,0x0232,0x03E6,
		 0x0103,0x0235,0x0404,0x0101,0x024C,0x0236,0x0102,0x025A,
		 0x03AF,0x0103,0x02B0,0x0400,0x0103,0x02B1,0x0400,0x0103,
		 0x02B3,0x0400,0x0102,0x02D3,0x03AD,0x0103,0x02E2,0x03E7,
		 0x0104,0x02E3,0x0000,0x0102,0x02E6,0x03F1,0x0103,0x02EC,
		 0x0000,0x0103,0x02EF,0x0000,0x0106,0x02F4,0x0243,0x0106,
		 0x02F7,0x0283,0x0203,0x0000,0x03FB,0x0203,0x0001,0x03FB,
		 0x0202,0x0003,0x0372,0x0203,0x0017,0x03E7,0x0203,0x0018,
		 0x03E7,0x0203,0x0027,0x0400,0x0204,0x0028,0x0258,0x0203,
		 0x002E,0x03FB,0x0205,0x002F,0x0000,
		 // Variant 0x0001: 20 Internal Links to Resolve (Scratch)
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_1mic.ndvc_obj][3] = [2,387: $M.CVC_SEND_CAP.headset_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][7] = [2,387: $M.CVC_SEND_CAP.headset_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][30] = [1,259: $M.CVC_SEND_CAP.headset_data_1mic.L_adaptA] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][31] = [3,0: $M.CVC_SEND_CAP.headset_data_1mic.Exp_Mts_adapt] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][32] = [2,258: $M.CVC_SEND_CAP.headset_data_1mic.AttenuationPersist] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][33] = [1,0: $M.CVC_SEND_CAP.headset_data_1mic.W_ri] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.headset_data_1mic.aec_obj][34] = [1,388: $M.CVC_SEND_CAP.headset_data_1mic.L_adaptR] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][8] = [2,387: $M.CVC_SEND_CAP.headset_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.headset_data_1mic.dms200_obj][10] = [1,0: $dm1_scratch] + 0 
		 //	[1,364: $M.CVC_SEND_CAP.headset_data_1mic.vsm_fdnlp][4] = [2,258: $M.CVC_SEND_CAP.headset_data_1mic.AttenuationPersist] + 0 
		 //	[1,712: $M.CVC_SEND_CAP.headset_data_1mic.fft_obj][1] = [1,0: $dm1_scratch] + 0 
		 //	[1,712: $M.CVC_SEND_CAP.headset_data_1mic.fft_obj][2] = [3,0: $dm2_scratch] + 0 
		 //	[1,712: $M.CVC_SEND_CAP.headset_data_1mic.fft_obj][3] = [1,0: $dm1_scratch] + 128 
		 //	[1,729: $M.CVC_SEND_CAP.headset_data_1mic.block?0][1] = [2,129: $M.CVC_SEND_CAP.headset_data_1mic.X_real] + 0 
		 //	[1,729: $M.CVC_SEND_CAP.headset_data_1mic.block?0][2] = [3,759: $M.CVC_SEND_CAP.headset_data_1mic.X_imag] + 0 
		 //	[1,729: $M.CVC_SEND_CAP.headset_data_1mic.block?0][5] = [2,0: $M.CVC_SEND_CAP.headset_data_1mic.D_l_real] + 0 
		 //	[1,729: $M.CVC_SEND_CAP.headset_data_1mic.block?0][6] = [3,630: $M.CVC_SEND_CAP.headset_data_1mic.D_l_imag] + 0 
		 //	[1,752: $M.CVC_SEND_CAP.headset_data_1mic.vad_peq_output][0] = [1,0: $dm1_scratch] + 0 
		 //	[1,756: $M.CVC_SEND_CAP.headset_data_1mic.AEC_Et][1] = [3,888: $M.CVC_SEND_CAP.headset_data_1mic.Et_imag] + 0 
		 //	[1,759: $M.CVC_SEND_CAP.headset_data_1mic.AEC_Dt][1] = [3,952: $M.CVC_SEND_CAP.headset_data_1mic.Dt_imag] + 0 
		 $DYN_SECTION_TYPE_RELOC_SCRATCH,0x0001,63,
		 0x0014,0x0000,0x0001,0x0102,0x0003,0x0183,0x0102,0x0023,
		 0x0183,0x0101,0x003A,0x0103,0x0103,0x003B,0x0000,0x0102,
		 0x003C,0x0102,0x0101,0x003D,0x0000,0x0101,0x003E,0x0184,
		 0x0102,0x00A3,0x0183,0x0101,0x00A5,0x0000,0x0102,0x0170,
		 0x0102,0x0101,0x02C9,0x0000,0x0103,0x02CA,0x0000,0x0101,
		 0x02CB,0x0080,0x0102,0x02DA,0x0081,0x0103,0x02DB,0x02F7,
		 0x0102,0x02DE,0x0000,0x0103,0x02DF,0x0276,0x0101,0x02F0,
		 0x0000,0x0103,0x02F5,0x0378,0x0103,0x02F8,0x03B8,
#endif /* EXPORTED_CONSTANTS_ONCHIP_BUILD */
		 //End of Descriptor
		 $DYN_SECTION_TYPE_END;


#if !defined(EXPORTED_CONSTANTS_ONCHIP_BUILD) || EXPORTED_CONSTANTS_ONCHIP_BUILD
	.VAR/CONST $M.CVC_SEND_CAP.headset_data_1mic.Downloadable.DynTable_Linker[] = 
		 // Variant 0xFFFF: 83 External Links to resolve
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[0] = &$cvc.init.root 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[3] = &$cvc.peq.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[6] = &$cvc.peq.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[9] = &$cvc.peq.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[12] = &$filter_bank.synthesis.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[15] = &$filter_bank.synthesis.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[18] = &$filter_bank.analysis.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[21] = &$harm100.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[22] = &$cvc.init.harm 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[24] = &$ndvc200.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[25] = &$cvc.init.ndvc200 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[27] = &$vad410.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[30] = &$filter_bank.analysis.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[33] = &$aec520.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[34] = &$cvc.init.aec520 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[36] = &$aec520.nlp.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[37] = &$cvc.init.vsm_fdnlp 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[39] = &$dms200.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[40] = &$cvc.init.dms200 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[42] = &$cvc.user.dms200.wnr.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[45] = &$cvc.user.dms200.va_init 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[48] = &$aed100.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[49] = &$cvc.init.aed100 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[51] = &$cvc.peq.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[54] = &$agc400.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.headset_data_1mic.ReInitializeTable[55] = &$cvc.init.agc400 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[0] = &$frame_proc.distribute_streams 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[3] = &$cvc.pre_process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[6] = &$cvc.stream_transfer.peak 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[9] = &$cvc.stream_transfer.peak 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[12] = &$cvc.peq.process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[15] = &$cvc.peq.process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[18] = &$cvc.peq.process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[21] = &$filter_bank.analysis.process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[22] = &$cvc.mc.ref_delay 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[24] = &$vad410.process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[27] = &$cvc.event.echo_flag 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[30] = &$harm100.process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[33] = &$filter_bank.analysis.process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[36] = &$aec520.process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[37] = &$cvc.mc.aec520 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[39] = &$dms200.process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[40] = &$cvc.mc.dms200 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[42] = &$dms200.apply_gain 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[43] = &$cvc.mc.dms_out 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[45] = &$ndvc200.process 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[48] = &$cvc.run_function_table 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[49] = &$cvc.mc.voice 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[51] = &$cvc.run_function_table 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[52] = &$cvc.mc.va 
		 //	[1,306] $M.CVC_SEND_CAP.headset_data_1mic.hfk_proc_funcsSnd[54] = &$frame_proc.update_streams 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[0] = &$aed100.process 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[1] = &$cvc.mc.aed100 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[3] = &$aec520.nlp.process 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[4] = &$cvc.mc.aec520_nlp 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[6] = &$aec520.cng.process 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[7] = &$cvc.mc.aec520_cng 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[9] = &$filter_bank.synthesis.process 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[12] = &$cvc.peq.process 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[15] = &$cvc_send.stream_gain.process.voice 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[18] = &$agc400.process 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[19] = &$cvc.mc.agc400 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[21] = &$cvc_send.mute_control 
		 //	[1,443] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_voice[24] = &$cvc.stream_transfer.peak.output 
		 //	[1,499] $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd[0] = &$frame_proc.distribute_streams 
		 //	[1,499] $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd[3] = &$cvc.stream_transfer.peak 
		 //	[1,499] $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd[6] = &$cvc.stream_transfer.peak 
		 //	[1,499] $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd[9] = &$cvc_send.passthrough.voice 
		 //	[1,499] $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd[12] = &$cvc_send.passthrough.va 
		 //	[1,499] $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd[15] = &$cvc.stream_transfer.peak.output 
		 //	[1,499] $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd[18] = &$cvc.stream_transfer.peak.output 
		 //	[1,499] $M.CVC_SEND_CAP.headset_data_1mic.copy_proc_funcsSnd[21] = &$frame_proc.update_streams 
		 //	[1,619] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout_va1[6] = &$frame_proc.distribute_output_stream 
		 //	[1,619] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout_va1[7] = &$frame_proc.update_output_stream 
		 //	[1,632] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_left_adc[6] = &$frame_proc.distribute_input_stream 
		 //	[1,632] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_left_adc[7] = &$frame_proc.update_input_stream 
		 //	[1,645] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_refin[6] = &$frame_proc.distribute_input_stream 
		 //	[1,645] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_refin[7] = &$frame_proc.update_input_stream 
		 //	[1,658] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout[6] = &$frame_proc.distribute_output_stream 
		 //	[1,658] $M.CVC_SEND_CAP.headset_data_1mic.stream_map_sndout[7] = &$frame_proc.update_output_stream 
		 //	[1,684] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va[0] = &$filter_bank.synthesis.process 
		 //	[1,684] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va[3] = &$cvc_send.stream_gain.process.va 
		 //	[1,684] $M.CVC_SEND_CAP.headset_data_1mic.proc_stream_va[6] = &$cvc.stream_transfer.peak.output 
		 $DYN_SECTION_TYPE_DATA_INST,0xFFFF,166,
		 0x0100F8,$cvc.init.root,
		 0x0100FB,$cvc.peq.initialize,
		 0x0100FE,$cvc.peq.initialize,
		 0x010101,$cvc.peq.initialize,
		 0x010104,$filter_bank.synthesis.initialize,
		 0x010107,$filter_bank.synthesis.initialize,
		 0x01010A,$filter_bank.analysis.initialize,
		 0x01010D,$harm100.initialize,
		 0x01010E,$cvc.init.harm,
		 0x010110,$ndvc200.initialize,
		 0x010111,$cvc.init.ndvc200,
		 0x010113,$vad410.initialize,
		 0x010116,$filter_bank.analysis.initialize,
		 0x010119,$aec520.initialize,
		 0x01011A,$cvc.init.aec520,
		 0x01011C,$aec520.nlp.initialize,
		 0x01011D,$cvc.init.vsm_fdnlp,
		 0x01011F,$dms200.initialize,
		 0x010120,$cvc.init.dms200,
		 0x010122,$cvc.user.dms200.wnr.initialize,
		 0x010125,$cvc.user.dms200.va_init,
		 0x010128,$aed100.initialize,
		 0x010129,$cvc.init.aed100,
		 0x01012B,$cvc.peq.initialize,
		 0x01012E,$agc400.initialize,
		 0x01012F,$cvc.init.agc400,
		 0x010132,$frame_proc.distribute_streams,
		 0x010135,$cvc.pre_process,
		 0x010138,$cvc.stream_transfer.peak,
		 0x01013B,$cvc.stream_transfer.peak,
		 0x01013E,$cvc.peq.process,
		 0x010141,$cvc.peq.process,
		 0x010144,$cvc.peq.process,
		 0x010147,$filter_bank.analysis.process,
		 0x010148,$cvc.mc.ref_delay,
		 0x01014A,$vad410.process,
		 0x01014D,$cvc.event.echo_flag,
		 0x010150,$harm100.process,
		 0x010153,$filter_bank.analysis.process,
		 0x010156,$aec520.process,
		 0x010157,$cvc.mc.aec520,
		 0x010159,$dms200.process,
		 0x01015A,$cvc.mc.dms200,
		 0x01015C,$dms200.apply_gain,
		 0x01015D,$cvc.mc.dms_out,
		 0x01015F,$ndvc200.process,
		 0x010162,$cvc.run_function_table,
		 0x010163,$cvc.mc.voice,
		 0x010165,$cvc.run_function_table,
		 0x010166,$cvc.mc.va,
		 0x010168,$frame_proc.update_streams,
		 0x0101BB,$aed100.process,
		 0x0101BC,$cvc.mc.aed100,
		 0x0101BE,$aec520.nlp.process,
		 0x0101BF,$cvc.mc.aec520_nlp,
		 0x0101C1,$aec520.cng.process,
		 0x0101C2,$cvc.mc.aec520_cng,
		 0x0101C4,$filter_bank.synthesis.process,
		 0x0101C7,$cvc.peq.process,
		 0x0101CA,$cvc_send.stream_gain.process.voice,
		 0x0101CD,$agc400.process,
		 0x0101CE,$cvc.mc.agc400,
		 0x0101D0,$cvc_send.mute_control,
		 0x0101D3,$cvc.stream_transfer.peak.output,
		 0x0101F3,$frame_proc.distribute_streams,
		 0x0101F6,$cvc.stream_transfer.peak,
		 0x0101F9,$cvc.stream_transfer.peak,
		 0x0101FC,$cvc_send.passthrough.voice,
		 0x0101FF,$cvc_send.passthrough.va,
		 0x010202,$cvc.stream_transfer.peak.output,
		 0x010205,$cvc.stream_transfer.peak.output,
		 0x010208,$frame_proc.update_streams,
		 0x010271,$frame_proc.distribute_output_stream,
		 0x010272,$frame_proc.update_output_stream,
		 0x01027E,$frame_proc.distribute_input_stream,
		 0x01027F,$frame_proc.update_input_stream,
		 0x01028B,$frame_proc.distribute_input_stream,
		 0x01028C,$frame_proc.update_input_stream,
		 0x010298,$frame_proc.distribute_output_stream,
		 0x010299,$frame_proc.update_output_stream,
		 0x0102AC,$filter_bank.synthesis.process,
		 0x0102AF,$cvc_send.stream_gain.process.va,
		 0x0102B2,$cvc.stream_transfer.peak.output,
		 //End of Descriptor
		 $DYN_SECTION_TYPE_END;

#endif /* EXPORTED_CONSTANTS_ONCHIP_BUILD */


.ENDMODULE; 

#endif /* !defined(EXPORTED_CONSTANTS_ONCHIP_BUILD) || EXPORTED_CONSTANTS_ONCHIP_BUILD */
