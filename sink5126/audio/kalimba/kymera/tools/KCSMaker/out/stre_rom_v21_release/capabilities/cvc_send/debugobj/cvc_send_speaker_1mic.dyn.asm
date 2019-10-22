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
.MODULE $M.CVC_SEND_CAP.speaker_data_1mic.Downloadable; 
	.DATASEGMENT DM; 

	.VAR16/CONST16 DynTable_Main[] = 
	/* If constants are moved off-chip to a file, the code needs 
	  to reference this variable */
.set $_CVC_SEND_CAP_speaker_data_1mic_DownloadableDynTable_Main, DynTable_Main
#if !defined(EXPORTED_CONSTANTS_ONCHIP_BUILD) || EXPORTED_CONSTANTS_ONCHIP_BUILD 
		 // Variant 0x0000: 9320 words allocated, 29 not used
		 //	Size 	DM0 		DM1 		DM2
		 //	 11 	 1 		 0 		 0
		 //	1032 	 1 		 3 		 1
		 //	1040 	 0 		 2 		 2
		 //
		 //	Block[00] Bank=DM0 Size=11 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=8 
		 //		 [I] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj Size=22
		 //		 [I] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.fba_ref Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.aec_obj Size=127
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj Size=93
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable Size=61
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd Size=61
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj Size=54
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp Size=41
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.snd_harm_obj Size=38
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd Size=25
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.root Size=22
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.StatusArray Size=20
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.ref_vad_peq Size=19
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410 Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.oms_wnr_obj Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout_va1 Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.stream_map_left_adc Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.stream_map_refin Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.aed100_obj Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va Size=10
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.vad_default_param Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.fft_obj Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.dms_obj Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.block?0 Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.fba_left Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.snd_streams Size=5
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.vad_peq_output Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.AEC_Et Size=3
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.AEC_Dt Size=3
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.harm_history Size=180
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.BExp_X_buf Size=17
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.vad_agc_obj Size=5
		 //	Block[02] Bank=DM2 Size=1032  Available=17 
		 //		 [I] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.snd_peq_dm2 Size=23
		 //		 [I] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.in_l_dcblock_dm2 Size=15
		 //		 [I] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1 Size=7
		 //		 [I] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd Size=7
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.Gb_real Size=384
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.bufd_aec_inp Size=240
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.bufd_l_inp Size=240
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.L2absGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.cvc_send_internal_streams Size=35
		 //	Block[03] Bank=DM1 Size=1040  Available=0 
		 //		 [U] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.Ga_imag Size=1040
		 //	Block[04] Bank=DM1 Size=1040  Available=0 
		 //		 [U] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.RcvBuf_real Size=1040
		 //	Block[05] Bank=DM1 Size=1032  Available=4 
		 //		 [U] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.Gb_imag Size=384
		 //		 [U] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_buffer Size=240
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.bufd_outp_va1 Size=300
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.in0oms_state Size=104
		 //	Block[06] Bank=DM2 Size=1040  Available=0 
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.Ga_real Size=1040
		 //	Block[07] Bank=DM2 Size=1040  Available=0 
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.RcvBuf_imag Size=1040
		 //	Block[08] Bank=DM0 Size=1032  Available=0 
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.bufd_outp Size=300
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.dms_state Size=256
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.CurParams Size=139
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.sndLpX_queue Size=136
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.in0oms_LpX_queue Size=136
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.BExp_Ga Size=65
		 //	Block[09] Bank=DM1 Size=1032  Available=0 
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.Cng_Nz_Shape_Tab Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.LPwrX1 Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.LpZ_nz Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.LPwrX0 Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.L_RatSqG Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.BExp_Gb Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.SqGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.LPwrD Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.Gr_real Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.Gr_imag Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.RatFE Size=64
		 //		*[U] SM1?$M.CVC_SEND_CAP.speaker_data_1mic.FFT_DM1 Size=260
		 //		*[U] SM1?$M.CVC_SEND_CAP.speaker_data_1mic.Et_real Size=64
		 $DYN_SECTION_TYPE_ALLOC_INST,0x0000,17,
		 0x000A,0x0000,0x0001,0x000B,0x0400,0x0201,0x03F7,0x0410,
		 0x0101,0x0410,0x0404,0x0202,0x0410,0x0410,0x0001,0x0408,
		 0x0408,
		 // Variant 0x0000: 2064 words Scratch allocated, 287 not used
		 //	Size 	SM0 		SM1 		SM2
		 //	  4 	 1 		 0 		 0
		 //	1032 	 0 		 1 		 1
		 //
		 //	Block[00] Bank=DM0 Size=4 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=273 
		 //		 SM1?$dm1_scratch Size=630
		 //		 SM1?$M.CVC_SEND_CAP.speaker_data_1mic.Dt_real Size=64
		 //		 SM?$M.CVC_SEND_CAP.speaker_data_1mic.DTC_lin Size=65
		 //	Block[02] Bank=DM2 Size=1032  Available=14 
		 //		 SM2?$dm2_scratch Size=630
		 //		 SM2?$M.CVC_SEND_CAP.speaker_data_1mic.FFT_DM2 Size=260
		 //		 SM2?$M.CVC_SEND_CAP.speaker_data_1mic.Et_imag Size=64
		 //		 SM2?$M.CVC_SEND_CAP.speaker_data_1mic.Dt_imag Size=64
		 $DYN_SECTION_TYPE_ALLOC_SCRATCH,0x0000,7,
		 0x0003,0x0001,0x0001,0x0004,0x02F7,0x0200,0x03FA,
		 // Variant 0x0001: 15480 words allocated, 286 not used
		 //	Size 	DM0 		DM1 		DM2
		 //	 13 	 1 		 0 		 0
		 //	1032 	 2 		 3 		 2
		 //	2064 	 0 		 2 		 2
		 //
		 //	Block[00] Bank=DM0 Size=13 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=0 
		 //		 [I] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj Size=22
		 //		 [I] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.fba_ref Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.aec_obj Size=127
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj Size=93
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable Size=61
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd Size=61
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj Size=54
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp Size=41
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.snd_harm_obj Size=38
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd Size=25
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.root Size=22
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.StatusArray Size=20
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.ref_vad_peq Size=19
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410 Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.oms_wnr_obj Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout_va1 Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.stream_map_left_adc Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.stream_map_refin Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.aed100_obj Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va Size=10
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.vad_default_param Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.fft_obj Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.dms_obj Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.block?0 Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.fba_left Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.snd_streams Size=5
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.vad_peq_output Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.AEC_Et Size=3
		 //		 [I] DM?$M.CVC_SEND_CAP.speaker_data_1mic.AEC_Dt Size=3
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.sndLpX_queue Size=170
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.cvc_send_internal_streams Size=35
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.vad_agc_obj Size=5
		 //	Block[02] Bank=DM2 Size=1032  Available=3 
		 //		 [I] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.snd_peq_dm2 Size=23
		 //		 [I] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.in_l_dcblock_dm2 Size=15
		 //		 [I] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1 Size=7
		 //		 [I] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd Size=7
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.bufd_aec_inp Size=480
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.bufd_l_inp Size=480
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.BExp_X_buf Size=17
		 //	Block[03] Bank=DM1 Size=2064  Available=0 
		 //		 [U] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.Ga_imag Size=2064
		 //	Block[04] Bank=DM1 Size=2064  Available=0 
		 //		 [U] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.RcvBuf_real Size=2064
		 //	Block[05] Bank=DM1 Size=1032  Available=29 
		 //		 [U] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_buffer Size=480
		 //		 [U] DM1?$M.CVC_SEND_CAP.speaker_data_1mic.Gb_imag Size=384
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.CurParams Size=139
		 //	Block[06] Bank=DM2 Size=2064  Available=0 
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.Ga_real Size=2064
		 //	Block[07] Bank=DM2 Size=2064  Available=0 
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.RcvBuf_imag Size=2064
		 //	Block[08] Bank=DM2 Size=1032  Available=24 
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.Gb_real Size=384
		 //		 [U] DM2?$M.CVC_SEND_CAP.speaker_data_1mic.L2absGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.harm_history Size=360
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.in0oms_LpX_queue Size=136
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.L_RatSqG Size=64
		 //	Block[09] Bank=DM0 Size=1032  Available=47 
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.bufd_outp_va1 Size=600
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.dms_state Size=256
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.BExp_Ga Size=129
		 //	Block[10] Bank=DM0 Size=1032  Available=45 
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.bufd_outp Size=600
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.Cng_Nz_Shape_Tab Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.LPwrX1 Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.LpZ_nz Size=129
		 //	Block[11] Bank=DM1 Size=1032  Available=138 
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.LPwrX0 Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.in0oms_state Size=124
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.BExp_Gb Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.SqGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.LPwrD Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.Gr_real Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.Gr_imag Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.speaker_data_1mic.RatFE Size=64
		 //		*[U] SM1?$M.CVC_SEND_CAP.speaker_data_1mic.Et_real Size=64
		 //		*[U] SM1?$M.CVC_SEND_CAP.speaker_data_1mic.Dt_real Size=64
		 //		*[U] SM?$M.CVC_SEND_CAP.speaker_data_1mic.DTC_lin Size=129
		 $DYN_SECTION_TYPE_ALLOC_INST,0x0001,20,
		 0x000C,0x0000,0x0001,0x000D,0x0408,0x0201,0x0405,0x0810,
		 0x0101,0x0810,0x03EB,0x0202,0x0810,0x0810,0x0200,0x03F0,
		 0x03D9,0x0001,0x03DB,0x037E,
		 // Variant 0x0001: 4128 words Scratch allocated, 1708 not used
		 //	Size 	SM0 		SM1 		SM2
		 //	  6 	 1 		 0 		 0
		 //	1032 	 0 		 2 		 2
		 //
		 //	Block[00] Bank=DM0 Size=6 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=402 
		 //		 SM1?$dm1_scratch Size=630
		 //	Block[02] Bank=DM1 Size=1032  Available=516 
		 //		 SM1?$M.CVC_SEND_CAP.speaker_data_1mic.FFT_DM1 Size=516
		 //	Block[03] Bank=DM2 Size=1032  Available=274 
		 //		 SM2?$dm2_scratch Size=630
		 //		 SM2?$M.CVC_SEND_CAP.speaker_data_1mic.Et_imag Size=64
		 //		 SM2?$M.CVC_SEND_CAP.speaker_data_1mic.Dt_imag Size=64
		 //	Block[04] Bank=DM2 Size=1032  Available=516 
		 //		 SM2?$M.CVC_SEND_CAP.speaker_data_1mic.FFT_DM2 Size=516
		 $DYN_SECTION_TYPE_ALLOC_SCRATCH,0x0001,10,
		 0x0005,0x0001,0x0001,0x0006,0x0276,0x0102,0x0204,0x02F6,
		 0x0200,0x0204,
		 // Variant 0xFFFF: Iniiatization Data 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][46] = 0x1F40 
		 // Word Offset 0x10000 , Length=2
		 // Word Offset 0x1001F , Length=2
		 // Word Offset 0x1004A , Length=1
		 // Word Offset 0x100A6 , Length=3
		 // Word Offset 0x100F9 , Length=2
		 // Word Offset 0x1017D , Length=2
		 // Word Offset 0x10248 , Length=1
		 // Word Offset 0x1024D , Length=1
		 // Word Offset 0x10274 , Length=1
		 // Word Offset 0x102FB , Length=9
		 // Word Offset 0x1030D , Length=1
		 // Word Offset 0x1032A , Length=1
		 // Word Offset 0x20002 , Length=1
		 // Word Offset 0x20019 , Length=1
		 // Word Offset 0x2002B , Length=1
		 // Word Offset 0x20032 , Length=2
		 $DYN_SECTION_TYPE_DATA_INST,0xFFFF,94,
		 0x0102,0x0000,0x0000,0x0001,0x0000,0x0008,0x0102,0x001F,
		 0x0F5C,0x28F6,0x0000,0x0002,0x0101,0x004A,0x0000,0x1F40,
		 0x0103,0x00A6,0x3999,0x999A,0x0083,0x126F,0x0000,0x0001,
		 0x0102,0x00F9,0x0000,0x0001,0x0000,0x0001,0x0102,0x017D,
		 0x3999,0x999A,0x0083,0x126F,0x0101,0x0248,0x0000,0x5E0C,
		 0x0101,0x024D,0x7FFF,0xFFFF,0x0101,0x0274,0x0000,0x0003,
		 0x0109,0x02FB,0x0600,0x0000,0x0600,0x0000,0x0010,0x624E,
		 0x0063,0xD70A,0x4000,0x0000,0x0080,0x0000,0xFD40,0x0000,
		 0x0050,0x0000,0x00E5,0x6042,0x0101,0x030D,0x0000,0x0001,
		 0x0101,0x032A,0x0000,0x0780,0x0201,0x0002,0x0000,0x0005,
		 0x0201,0x0019,0x0000,0x0001,0x0201,0x002B,0x0000,0x0001,
		 0x0202,0x0032,0x0000,0x0001,0xFFFF,0xFFFF,
		 // Variant 0x0000: Iniiatization Data 
		 //	[1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root][17] = 0xE6DE74 
		 //	[1,808: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream][2] = 0x3C0 
		 // Word Offset 0x10259 , Length=1
		 // Word Offset 0x1032A , Length=1
		 $DYN_SECTION_TYPE_DATA_INST,0x0000,8,
		 0x0101,0x0259,0x00E6,0xDE74,0x0101,0x032A,0x0000,0x03C0,
		 // Variant 0x0001: Iniiatization Data 
		 //	[1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root][17] = 0xE6948D 
		 // Word Offset 0x10259 , Length=1
		 $DYN_SECTION_TYPE_DATA_INST,0x0001,4,
		 0x0101,0x0259,0x00E6,0x948D,
		 // Variant 0xFFFF: 123 Internal Links to Resolve
		 //	Root[2] = [1,705: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_refin] + 0 
		 //	Root[3] = [1,692: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_left_adc] + 0 
		 //	Root[7] = [1,718: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout] + 0 
		 //	Root[8] = [1,679: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout_va1] + 0 
		 //	Root[22] = [1,754: $M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd] + 0 
		 //	Root[23] = [1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable] + 0 
		 //	Root[24] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 0 
		 //	Root[25] = [1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray] + 0 
		 //	Root[45] = [1,794: $M.CVC_SEND_CAP.speaker_data_1mic.D0] + 0 
		 //	[1,22: $M.CVC_SEND_CAP.speaker_data_1mic.fba_ref][1] = [1,808: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream] + 0 
		 //	[1,22: $M.CVC_SEND_CAP.speaker_data_1mic.fba_ref][3] = [1,772: $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj] + 0 
		 //	[1,22: $M.CVC_SEND_CAP.speaker_data_1mic.fba_ref][4] = [1,790: $M.CVC_SEND_CAP.speaker_data_1mic.X] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][9] = [1,808: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][10] = [1,790: $M.CVC_SEND_CAP.speaker_data_1mic.X] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][15] = [1,794: $M.CVC_SEND_CAP.speaker_data_1mic.D0] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][36] = [1,819: $M.CVC_SEND_CAP.speaker_data_1mic.AEC_Dt] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][37] = [1,816: $M.CVC_SEND_CAP.speaker_data_1mic.AEC_Et] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][0] = [1,794: $M.CVC_SEND_CAP.speaker_data_1mic.D0] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][1] = [1,794: $M.CVC_SEND_CAP.speaker_data_1mic.D0] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][15] = [1,781: $M.CVC_SEND_CAP.speaker_data_1mic.dms_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][4] = [2,23: $M.CVC_SEND_CAP.speaker_data_1mic.in_l_dcblock_dm2] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][14] = [2,45: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][17] = [2,38: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][20] = [1,797: $M.CVC_SEND_CAP.speaker_data_1mic.fba_left] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][23] = [1,465: $M.CVC_SEND_CAP.speaker_data_1mic.snd_harm_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][26] = [1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][29] = [1,0: $M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][32] = [1,645: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][35] = [1,22: $M.CVC_SEND_CAP.speaker_data_1mic.fba_ref] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][38] = [1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][41] = [1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][44] = [1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][46] = [1,662: $M.CVC_SEND_CAP.speaker_data_1mic.oms_wnr_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][47] = [1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][49] = [1,790: $M.CVC_SEND_CAP.speaker_data_1mic.X] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][50] = [1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][53] = [1,731: $M.CVC_SEND_CAP.speaker_data_1mic.aed100_obj] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][55] = [2,0: $M.CVC_SEND_CAP.speaker_data_1mic.snd_peq_dm2] + 0 
		 //	[1,248: $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable][59] = [1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][1] = [1,803: $M.CVC_SEND_CAP.speaker_data_1mic.snd_streams] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][7] = [1,705: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_refin] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][10] = [1,692: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_left_adc] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][13] = [2,23: $M.CVC_SEND_CAP.speaker_data_1mic.in_l_dcblock_dm2] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][23] = [1,22: $M.CVC_SEND_CAP.speaker_data_1mic.fba_ref] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][26] = [1,645: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][28] = [1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][29] = [1,645: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410] + 9 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][32] = [1,465: $M.CVC_SEND_CAP.speaker_data_1mic.snd_harm_obj] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][35] = [1,797: $M.CVC_SEND_CAP.speaker_data_1mic.fba_left] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][38] = [1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][41] = [1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][44] = [1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][47] = [1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][50] = [1,0: $M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][53] = [1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][56] = [1,744: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][58] = [1,803: $M.CVC_SEND_CAP.speaker_data_1mic.snd_streams] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][0] = [1,794: $M.CVC_SEND_CAP.speaker_data_1mic.D0] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][1] = [1,794: $M.CVC_SEND_CAP.speaker_data_1mic.D0] + 0 
		 //	[1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp][0] = [1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj] + 0 
		 //	[1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp][2] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 6 
		 //	[1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp][3] = [1,645: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410] + 9 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][2] = [1,731: $M.CVC_SEND_CAP.speaker_data_1mic.aed100_obj] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][5] = [1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][8] = [1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][11] = [2,45: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][13] = [2,0: $M.CVC_SEND_CAP.speaker_data_1mic.snd_peq_dm2] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][20] = [1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][26] = [1,718: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][1] = [1,803: $M.CVC_SEND_CAP.speaker_data_1mic.snd_streams] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][4] = [1,705: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_refin] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][7] = [1,692: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_left_adc] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][17] = [1,718: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][20] = [1,679: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout_va1] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][22] = [1,803: $M.CVC_SEND_CAP.speaker_data_1mic.snd_streams] + 0 
		 //	[1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root][3] = [1,465: $M.CVC_SEND_CAP.speaker_data_1mic.snd_harm_obj] + 0 
		 //	[1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root][4] = [1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj] + 0 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][0] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 0 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][4] = [1,0: $M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj] + 6 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][5] = [1,0: $M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj] + 5 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][6] = [1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm] + 7 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][7] = [1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm] + 15 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][8] = [1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj] + 84 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][9] = [1,662: $M.CVC_SEND_CAP.speaker_data_1mic.oms_wnr_obj] + 8 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][10] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 6 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][11] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 12 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][12] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 6 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][13] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 6 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][17] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 6 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][18] = [1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp] + 31 
		 //	[1,626: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad_peq][0] = [1,808: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream] + 0 
		 //	[1,626: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad_peq][1] = [1,812: $M.CVC_SEND_CAP.speaker_data_1mic.vad_peq_output] + 0 
		 //	[1,645: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410][0] = [1,808: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream] + 0 
		 //	[1,645: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410][1] = [1,763: $M.CVC_SEND_CAP.speaker_data_1mic.vad_default_param] + 0 
		 //	[1,662: $M.CVC_SEND_CAP.speaker_data_1mic.oms_wnr_obj][1] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 6 
		 //	[1,662: $M.CVC_SEND_CAP.speaker_data_1mic.oms_wnr_obj][2] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 14 
		 //	[1,662: $M.CVC_SEND_CAP.speaker_data_1mic.oms_wnr_obj][3] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 12 
		 //	[1,731: $M.CVC_SEND_CAP.speaker_data_1mic.aed100_obj][0] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 14 
		 //	[1,744: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va][2] = [2,38: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1] + 0 
		 //	[1,744: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va][8] = [1,679: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout_va1] + 0 
		 //	[1,754: $M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd][0] = [1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,754: $M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd][1] = [1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,754: $M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd][2] = [1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd] + 0 
		 //	[1,754: $M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd][3] = [1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd] + 0 
		 //	[1,754: $M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd][4] = [1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,754: $M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd][5] = [1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,754: $M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd][6] = [1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,754: $M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd][7] = [1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,754: $M.CVC_SEND_CAP.speaker_data_1mic.ModeProcTableSnd][8] = [1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd] + 0 
		 //	[1,781: $M.CVC_SEND_CAP.speaker_data_1mic.dms_obj][1] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 8 
		 //	[1,781: $M.CVC_SEND_CAP.speaker_data_1mic.dms_obj][3] = [1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root] + 18 
		 //	[1,789: $M.CVC_SEND_CAP.speaker_data_1mic.block?0][3] = [1,789: $M.CVC_SEND_CAP.speaker_data_1mic.BExp_X] + 0 
		 //	[1,789: $M.CVC_SEND_CAP.speaker_data_1mic.block?0][7] = [1,793: $M.CVC_SEND_CAP.speaker_data_1mic.BExp_D0] + 0 
		 //	[1,797: $M.CVC_SEND_CAP.speaker_data_1mic.fba_left][3] = [1,772: $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj] + 0 
		 //	[1,797: $M.CVC_SEND_CAP.speaker_data_1mic.fba_left][4] = [1,794: $M.CVC_SEND_CAP.speaker_data_1mic.D0] + 0 
		 //	[1,803: $M.CVC_SEND_CAP.speaker_data_1mic.snd_streams][0] = [1,718: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout] + 0 
		 //	[1,803: $M.CVC_SEND_CAP.speaker_data_1mic.snd_streams][1] = [1,705: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_refin] + 0 
		 //	[1,803: $M.CVC_SEND_CAP.speaker_data_1mic.snd_streams][2] = [1,692: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_left_adc] + 0 
		 //	[1,803: $M.CVC_SEND_CAP.speaker_data_1mic.snd_streams][3] = [1,679: $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout_va1] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1][3] = [1,772: $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1][4] = [1,790: $M.CVC_SEND_CAP.speaker_data_1mic.X] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd][3] = [1,772: $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd][4] = [1,794: $M.CVC_SEND_CAP.speaker_data_1mic.D0] + 0 
		 $DYN_SECTION_TYPE_RELOC_INST,0xFFFF,372,
		 0x007B,0x0000,0x0000,0x0001,0x0002,0x02C1,0x0001,0x0003,
		 0x02B4,0x0001,0x0007,0x02CE,0x0001,0x0008,0x02A7,0x0001,
		 0x0016,0x02F2,0x0001,0x0017,0x00F8,0x0001,0x0018,0x0248,
		 0x0001,0x0019,0x025E,0x0001,0x002D,0x031A,0x0101,0x0017,
		 0x0328,0x0101,0x0019,0x0304,0x0101,0x001A,0x0316,0x0101,
		 0x0025,0x0328,0x0101,0x0026,0x0316,0x0101,0x002B,0x031A,
		 0x0101,0x0040,0x0333,0x0101,0x0041,0x0330,0x0101,0x009B,
		 0x031A,0x0101,0x009C,0x031A,0x0101,0x00AA,0x030D,0x0102,
		 0x00FC,0x0017,0x0102,0x0106,0x002D,0x0102,0x0109,0x0026,
		 0x0101,0x010C,0x031D,0x0101,0x010F,0x01D1,0x0101,0x0112,
		 0x0172,0x0101,0x0115,0x0000,0x0101,0x0118,0x0285,0x0101,
		 0x011B,0x0016,0x0101,0x011E,0x001C,0x0101,0x0121,0x01A8,
		 0x0101,0x0124,0x009B,0x0101,0x0126,0x0296,0x0101,0x0127,
		 0x009B,0x0101,0x0129,0x0316,0x0101,0x012A,0x009B,0x0101,
		 0x012D,0x02DB,0x0102,0x012F,0x0000,0x0101,0x0133,0x0213,
		 0x0101,0x0136,0x0323,0x0101,0x013C,0x02C1,0x0101,0x013F,
		 0x02B4,0x0102,0x0142,0x0017,0x0101,0x014C,0x0016,0x0101,
		 0x014F,0x0285,0x0101,0x0151,0x01A8,0x0101,0x0152,0x028E,
		 0x0101,0x0155,0x01D1,0x0101,0x0158,0x031D,0x0101,0x015B,
		 0x0172,0x0101,0x015E,0x001C,0x0101,0x0161,0x009B,0x0101,
		 0x0164,0x009B,0x0101,0x0167,0x0000,0x0101,0x016A,0x01F7,
		 0x0101,0x016D,0x02E8,0x0101,0x016F,0x0323,0x0101,0x0172,
		 0x031A,0x0101,0x0173,0x031A,0x0101,0x01A8,0x001C,0x0101,
		 0x01AA,0x024E,0x0101,0x01AB,0x028E,0x0101,0x01F9,0x02DB,
		 0x0101,0x01FC,0x01A8,0x0101,0x01FF,0x001C,0x0102,0x0202,
		 0x002D,0x0102,0x0204,0x0000,0x0101,0x020B,0x0213,0x0101,
		 0x0211,0x02CE,0x0101,0x0230,0x0323,0x0101,0x0233,0x02C1,
		 0x0101,0x0236,0x02B4,0x0101,0x0240,0x02CE,0x0101,0x0243,
		 0x02A7,0x0101,0x0245,0x0323,0x0101,0x024B,0x01D1,0x0101,
		 0x024C,0x009B,0x0101,0x025E,0x0248,0x0101,0x0262,0x0006,
		 0x0101,0x0263,0x0005,0x0101,0x0264,0x021A,0x0101,0x0265,
		 0x0222,0x0101,0x0266,0x0070,0x0101,0x0267,0x029E,0x0101,
		 0x0268,0x024E,0x0101,0x0269,0x0254,0x0101,0x026A,0x024E,
		 0x0101,0x026B,0x024E,0x0101,0x026F,0x024E,0x0101,0x0270,
		 0x01C7,0x0101,0x0272,0x0328,0x0101,0x0273,0x032C,0x0101,
		 0x0285,0x0328,0x0101,0x0286,0x02FB,0x0101,0x0297,0x024E,
		 0x0101,0x0298,0x0256,0x0101,0x0299,0x0254,0x0101,0x02DB,
		 0x0256,0x0102,0x02EA,0x0026,0x0101,0x02F0,0x02A7,0x0101,
		 0x02F2,0x022F,0x0101,0x02F3,0x022F,0x0101,0x02F4,0x0135,
		 0x0101,0x02F5,0x0135,0x0101,0x02F6,0x022F,0x0101,0x02F7,
		 0x022F,0x0101,0x02F8,0x022F,0x0101,0x02F9,0x022F,0x0101,
		 0x02FA,0x022F,0x0101,0x030E,0x0250,0x0101,0x0310,0x025A,
		 0x0101,0x0318,0x0315,0x0101,0x031C,0x0319,0x0101,0x0320,
		 0x0304,0x0101,0x0321,0x031A,0x0101,0x0323,0x02CE,0x0101,
		 0x0324,0x02C1,0x0101,0x0325,0x02B4,0x0101,0x0326,0x02A7,
		 0x0201,0x0029,0x0304,0x0201,0x002A,0x0316,0x0201,0x0030,
		 0x0304,0x0201,0x0031,0x031A,
		 // Variant 0xFFFF: 10 Links from Root to Resolve
		 //	Root[27] = &$M.CVC_SEND_CAP.speaker_data_1mic.cvclib_table, 2 references to resolve
		 //		[1,155] $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj[5] = ROOT[27] 
		 //		[1,370] $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj[5] = ROOT[27] 
		 //	Root[28] = &$M.CVC_SEND_CAP.speaker_data_1mic.fft_split, 1 references to resolve
		 //		[1,772] $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj[4] = ROOT[28] 
		 //	Root[29] = &$M.CVC_SEND_CAP.speaker_data_1mic.oms_mode_object, 1 references to resolve
		 //		[1,370] $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj[3] = ROOT[29] 
		 //	Root[30] = &$M.CVC_SEND_CAP.speaker_data_1mic.dms_mode_object, 1 references to resolve
		 //		[1,155] $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj[3] = ROOT[30] 
		 //	Root[32] = &$M.CVC_SEND_CAP.speaker_data_1mic.vad_dc_coeffs, 2 references to resolve
		 //		[1,626] $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad_peq[3] = ROOT[32] + 10 
		 //		[2,23] $M.CVC_SEND_CAP.speaker_data_1mic.in_l_dcblock_dm2[3] = ROOT[32] 
		 //	Root[33] = &$M.CVC_SEND_CAP.speaker_data_1mic.aec_mode_object, 1 references to resolve
		 //		[1,28] $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj[0] = ROOT[33] 
		 //	Root[34] = &$M.CVC_SEND_CAP.speaker_data_1mic.fb_configuration, 4 references to resolve
		 //		[1,22] $M.CVC_SEND_CAP.speaker_data_1mic.fba_ref[0] = ROOT[34] 
		 //		[1,797] $M.CVC_SEND_CAP.speaker_data_1mic.fba_left[0] = ROOT[34] 
		 //		[2,38] $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1[0] = ROOT[34] 
		 //		[2,45] $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd[0] = ROOT[34] 
		 //	Root[40] = &$M.CVC_SEND_CAP.speaker_data_1mic.cap_root_ptr, 11 references to resolve
		 //		[1,0] $M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj[2] = ROOT[40] + 47 
		 //		[1,28] $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj[2] = ROOT[40] + 47 
		 //		[1,155] $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj[2] = ROOT[40] + 47 
		 //		[1,370] $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj[2] = ROOT[40] + 47 
		 //		[1,465] $M.CVC_SEND_CAP.speaker_data_1mic.snd_harm_obj[1] = ROOT[40] + 47 
		 //		[1,531] $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm[4] = ROOT[40] + 47 
		 //		[1,559] $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd[10] = ROOT[40] + 12 
		 //		[1,559] $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd[13] = ROOT[40] + 12 
		 //		[1,584] $M.CVC_SEND_CAP.speaker_data_1mic.root[1] = ROOT[40] 
		 //		[1,645] $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410[2] = ROOT[40] + 47 
		 //		[1,731] $M.CVC_SEND_CAP.speaker_data_1mic.aed100_obj[1] = ROOT[40] + 47 
		 //	Root[54] = &$M.CVC_SEND_CAP.speaker_data_1mic.mute_control_ptr, 1 references to resolve
		 //		[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[22] = ROOT[54] 
		 //	Root[58] = &$M.CVC_SEND_CAP.speaker_data_1mic.ext_frame_size, 4 references to resolve
		 //		[1,679] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout_va1[5] = ROOT[58] 
		 //		[1,692] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_left_adc[5] = ROOT[58] 
		 //		[1,705] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_refin[5] = ROOT[58] 
		 //		[1,718] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout[5] = ROOT[58] 
		 $DYN_SECTION_TYPE_RELOC_ROOT,0xFFFF,104,
		 0x001B,0x0002,0x0001,0x00A0,0x0000,0x0001,0x0177,0x0000,
		 0x001C,0x0001,0x0001,0x0308,0x0000,0x001D,0x0001,0x0001,
		 0x0175,0x0000,0x001E,0x0001,0x0001,0x009E,0x0000,0x0020,
		 0x0002,0x0001,0x0275,0x000A,0x0002,0x001A,0x0000,0x0021,
		 0x0001,0x0001,0x001C,0x0000,0x0022,0x0004,0x0001,0x0016,
		 0x0000,0x0001,0x031D,0x0000,0x0002,0x0026,0x0000,0x0002,
		 0x002D,0x0000,0x0028,0x000B,0x0001,0x0002,0x002F,0x0001,
		 0x001E,0x002F,0x0001,0x009D,0x002F,0x0001,0x0174,0x002F,
		 0x0001,0x01D2,0x002F,0x0001,0x0217,0x002F,0x0001,0x0239,
		 0x000C,0x0001,0x023C,0x000C,0x0001,0x0249,0x0000,0x0001,
		 0x0287,0x002F,0x0001,0x02DC,0x002F,0x0036,0x0001,0x0001,
		 0x020D,0x0000,0x003A,0x0004,0x0001,0x02AC,0x0000,0x0001,
		 0x02B9,0x0000,0x0001,0x02C6,0x0000,0x0001,0x02D3,0x0000,
		 // Variant 0x0000: 87 Internal Links to Resolve
		 //	Root[12] = [2,980: $M.CVC_SEND_CAP.speaker_data_1mic.cvc_send_internal_streams] + 0 
		 //	Root[26] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj][3] = [9,903: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj][4] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 2 
		 //	[1,22: $M.CVC_SEND_CAP.speaker_data_1mic.fba_ref][2] = [2,436: $M.CVC_SEND_CAP.speaker_data_1mic.bufd_aec_inp] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][1] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 90 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][7] = [9,903: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][8] = [2,980: $M.CVC_SEND_CAP.speaker_data_1mic.stream_x] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][11] = [4,0: $M.CVC_SEND_CAP.speaker_data_1mic.RcvBuf_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][12] = [7,0: $M.CVC_SEND_CAP.speaker_data_1mic.RcvBuf_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][13] = [1,1002: $M.CVC_SEND_CAP.speaker_data_1mic.BExp_X_buf] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][16] = [6,0: $M.CVC_SEND_CAP.speaker_data_1mic.Ga_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][17] = [3,0: $M.CVC_SEND_CAP.speaker_data_1mic.Ga_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][18] = [8,967: $M.CVC_SEND_CAP.speaker_data_1mic.BExp_Ga] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][20] = [9,195: $M.CVC_SEND_CAP.speaker_data_1mic.LPwrX0] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][21] = [9,65: $M.CVC_SEND_CAP.speaker_data_1mic.LPwrX1] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][22] = [9,644: $M.CVC_SEND_CAP.speaker_data_1mic.RatFE] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][23] = [9,580: $M.CVC_SEND_CAP.speaker_data_1mic.Gr_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][24] = [9,516: $M.CVC_SEND_CAP.speaker_data_1mic.Gr_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][25] = [9,388: $M.CVC_SEND_CAP.speaker_data_1mic.SqGr] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][26] = [2,916: $M.CVC_SEND_CAP.speaker_data_1mic.L2absGr] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][27] = [9,452: $M.CVC_SEND_CAP.speaker_data_1mic.LPwrD] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][28] = [9,130: $M.CVC_SEND_CAP.speaker_data_1mic.LpZ_nz] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][29] = [9,0: $M.CVC_SEND_CAP.speaker_data_1mic.Cng_Nz_Shape_Tab] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][32] = [9,838: $M.CVC_SEND_CAP.speaker_data_1mic.AttenuationPersist] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][39] = [2,52: $M.CVC_SEND_CAP.speaker_data_1mic.Gb_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][40] = [5,0: $M.CVC_SEND_CAP.speaker_data_1mic.Gb_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][41] = [9,324: $M.CVC_SEND_CAP.speaker_data_1mic.BExp_Gb] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][42] = [9,260: $M.CVC_SEND_CAP.speaker_data_1mic.L_RatSqG] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][4] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 79 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][6] = [8,695: $M.CVC_SEND_CAP.speaker_data_1mic.sndLpX_queue] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][9] = [8,300: $M.CVC_SEND_CAP.speaker_data_1mic.dms_state] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][17] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 78 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][8] = [2,980: $M.CVC_SEND_CAP.speaker_data_1mic.stream_x] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][11] = [2,985: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][6] = [8,831: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_LpX_queue] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][8] = [9,903: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][9] = [5,924: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_state] + 0 
		 //	[1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp][1] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 108 
		 //	[1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp][4] = [9,838: $M.CVC_SEND_CAP.speaker_data_1mic.AttenuationPersist] + 0 
		 //	[1,465: $M.CVC_SEND_CAP.speaker_data_1mic.snd_harm_obj][0] = [2,985: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[1,465: $M.CVC_SEND_CAP.speaker_data_1mic.snd_harm_obj][2] = [1,822: $M.CVC_SEND_CAP.speaker_data_1mic.harm_history] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][16] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][17] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][23] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][25] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm][0] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 49 
		 //	[1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm][1] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm][2] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm][3] = [1,1019: $M.CVC_SEND_CAP.speaker_data_1mic.vad_agc_obj] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][5] = [2,980: $M.CVC_SEND_CAP.speaker_data_1mic.stream_x] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][8] = [2,985: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][11] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][14] = [2,1010: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][16] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][19] = [2,1010: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root][2] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 0 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][1] = [2,985: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][2] = [2,990: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d1] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][3] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][14] = [2,995: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d2] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][15] = [2,1000: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d3] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][16] = [2,980: $M.CVC_SEND_CAP.speaker_data_1mic.stream_x] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][19] = [2,1010: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 4 
		 //	[1,645: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410][3] = [1,626: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad_peq] + 0 
		 //	[1,662: $M.CVC_SEND_CAP.speaker_data_1mic.oms_wnr_obj][0] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 71 
		 //	[1,744: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va][4] = [2,1010: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[1,744: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va][5] = [2,1010: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[1,744: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va][7] = [2,1010: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[1,781: $M.CVC_SEND_CAP.speaker_data_1mic.dms_obj][2] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 69 
		 //	[1,789: $M.CVC_SEND_CAP.speaker_data_1mic.block?0][1] = [9,773: $M.CVC_SEND_CAP.speaker_data_1mic.X_real] + 0 
		 //	[1,789: $M.CVC_SEND_CAP.speaker_data_1mic.block?0][5] = [9,708: $M.CVC_SEND_CAP.speaker_data_1mic.D_l_real] + 0 
		 //	[1,797: $M.CVC_SEND_CAP.speaker_data_1mic.fba_left][1] = [2,985: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[1,797: $M.CVC_SEND_CAP.speaker_data_1mic.fba_left][2] = [2,676: $M.CVC_SEND_CAP.speaker_data_1mic.bufd_l_inp] + 0 
		 //	[1,797: $M.CVC_SEND_CAP.speaker_data_1mic.fba_left][5] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 137 
		 //	[1,808: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream][0] = [5,384: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_buffer] + 0 
		 //	[1,808: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream][3] = [5,384: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_buffer] + 0 
		 //	[1,816: $M.CVC_SEND_CAP.speaker_data_1mic.AEC_Et][0] = [9,968: $M.CVC_SEND_CAP.speaker_data_1mic.Et_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.speaker_data_1mic.snd_peq_dm2][0] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.speaker_data_1mic.snd_peq_dm2][1] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.speaker_data_1mic.snd_peq_dm2][3] = [8,556: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 10 
		 //	[2,23: $M.CVC_SEND_CAP.speaker_data_1mic.in_l_dcblock_dm2][0] = [2,985: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[2,23: $M.CVC_SEND_CAP.speaker_data_1mic.in_l_dcblock_dm2][1] = [2,985: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1][1] = [2,1010: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1][2] = [5,624: $M.CVC_SEND_CAP.speaker_data_1mic.bufd_outp_va1] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd][1] = [2,1005: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd][2] = [8,0: $M.CVC_SEND_CAP.speaker_data_1mic.bufd_outp] + 0 
		 $DYN_SECTION_TYPE_RELOC_INST,0x0000,264,
		 0x0057,0x0000,0x0000,0x0002,0x000C,0x03D4,0x0008,0x001A,
		 0x022C,0x0109,0x0003,0x0387,0x0108,0x0004,0x022E,0x0102,
		 0x0018,0x01B4,0x0108,0x001D,0x0286,0x0109,0x0023,0x0387,
		 0x0102,0x0024,0x03D4,0x0104,0x0027,0x0000,0x0107,0x0028,
		 0x0000,0x0101,0x0029,0x03EA,0x0106,0x002C,0x0000,0x0103,
		 0x002D,0x0000,0x0108,0x002E,0x03C7,0x0109,0x0030,0x00C3,
		 0x0109,0x0031,0x0041,0x0109,0x0032,0x0284,0x0109,0x0033,
		 0x0244,0x0109,0x0034,0x0204,0x0109,0x0035,0x0184,0x0102,
		 0x0036,0x0394,0x0109,0x0037,0x01C4,0x0109,0x0038,0x0082,
		 0x0109,0x0039,0x0000,0x0109,0x003C,0x0346,0x0102,0x0043,
		 0x0034,0x0105,0x0044,0x0000,0x0109,0x0045,0x0144,0x0109,
		 0x0046,0x0104,0x0108,0x009F,0x027B,0x0108,0x00A1,0x02B7,
		 0x0108,0x00A4,0x012C,0x0108,0x00AC,0x027A,0x0102,0x013D,
		 0x03D4,0x0102,0x0140,0x03D9,0x0108,0x0178,0x033F,0x0109,
		 0x017A,0x0387,0x0105,0x017B,0x039C,0x0108,0x01A9,0x0298,
		 0x0109,0x01AC,0x0346,0x0102,0x01D1,0x03D9,0x0101,0x01D3,
		 0x0336,0x0102,0x0207,0x03ED,0x0102,0x0208,0x03ED,0x0102,
		 0x020E,0x03ED,0x0102,0x0210,0x03ED,0x0108,0x0213,0x025D,
		 0x0102,0x0214,0x03ED,0x0102,0x0215,0x03ED,0x0101,0x0216,
		 0x03FB,0x0102,0x0234,0x03D4,0x0102,0x0237,0x03D9,0x0102,
		 0x023A,0x03ED,0x0102,0x023D,0x03F2,0x0102,0x023F,0x03ED,
		 0x0102,0x0242,0x03F2,0x0108,0x024A,0x022C,0x0102,0x025F,
		 0x03DD,0x0102,0x0260,0x03E2,0x0102,0x0261,0x03F1,0x0102,
		 0x026C,0x03E7,0x0102,0x026D,0x03EC,0x0102,0x026E,0x03D8,
		 0x0102,0x0271,0x03F6,0x0101,0x0288,0x0272,0x0108,0x0296,
		 0x0273,0x0102,0x02EC,0x03F2,0x0102,0x02ED,0x03F2,0x0102,
		 0x02EF,0x03F2,0x0108,0x030F,0x0271,0x0109,0x0316,0x0305,
		 0x0109,0x031A,0x02C4,0x0102,0x031E,0x03D9,0x0102,0x031F,
		 0x02A4,0x0108,0x0322,0x02B5,0x0105,0x0328,0x0180,0x0105,
		 0x032B,0x0180,0x0109,0x0330,0x03C8,0x0202,0x0000,0x03ED,
		 0x0202,0x0001,0x03ED,0x0208,0x0003,0x0236,0x0202,0x0017,
		 0x03D9,0x0202,0x0018,0x03D9,0x0202,0x0027,0x03F2,0x0205,
		 0x0028,0x0270,0x0202,0x002E,0x03ED,0x0208,0x002F,0x0000,
		 // Variant 0x0000: 21 Internal Links to Resolve (Scratch)
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][6] = [2,760: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_G] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][30] = [1,131: $M.CVC_SEND_CAP.speaker_data_1mic.L_adaptA] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][31] = [2,0: $M.CVC_SEND_CAP.speaker_data_1mic.Exp_Mts_adapt] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][33] = [1,0: $M.CVC_SEND_CAP.speaker_data_1mic.W_ri] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][34] = [1,196: $M.CVC_SEND_CAP.speaker_data_1mic.L_adaptR] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][35] = [1,694: $M.CVC_SEND_CAP.speaker_data_1mic.DTC_lin] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][38] = [2,130: $M.CVC_SEND_CAP.speaker_data_1mic.rerdt_dtc] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][7] = [2,825: $M.CVC_SEND_CAP.speaker_data_1mic.G_dmsZ] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][10] = [1,0: $dm1_scratch] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][7] = [2,760: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_G] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][10] = [1,0: $dm1_scratch] + 0 
		 //	[1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp][5] = [2,0: $dm2_scratch] + 0 
		 //	[1,772: $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj][1] = [1,0: $dm1_scratch] + 0 
		 //	[1,772: $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj][2] = [2,0: $dm2_scratch] + 0 
		 //	[1,772: $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj][3] = [1,0: $dm1_scratch] + 128 
		 //	[1,789: $M.CVC_SEND_CAP.speaker_data_1mic.block?0][2] = [2,695: $M.CVC_SEND_CAP.speaker_data_1mic.X_imag] + 0 
		 //	[1,789: $M.CVC_SEND_CAP.speaker_data_1mic.block?0][6] = [2,630: $M.CVC_SEND_CAP.speaker_data_1mic.D_l_imag] + 0 
		 //	[1,812: $M.CVC_SEND_CAP.speaker_data_1mic.vad_peq_output][0] = [1,0: $dm1_scratch] + 0 
		 //	[1,816: $M.CVC_SEND_CAP.speaker_data_1mic.AEC_Et][1] = [2,890: $M.CVC_SEND_CAP.speaker_data_1mic.Et_imag] + 0 
		 //	[1,819: $M.CVC_SEND_CAP.speaker_data_1mic.AEC_Dt][0] = [1,630: $M.CVC_SEND_CAP.speaker_data_1mic.Dt_real] + 0 
		 //	[1,819: $M.CVC_SEND_CAP.speaker_data_1mic.AEC_Dt][1] = [2,954: $M.CVC_SEND_CAP.speaker_data_1mic.Dt_imag] + 0 
		 $DYN_SECTION_TYPE_RELOC_SCRATCH,0x0000,66,
		 0x0015,0x0000,0x0001,0x0102,0x0022,0x02F8,0x0101,0x003A,
		 0x0083,0x0102,0x003B,0x0000,0x0101,0x003D,0x0000,0x0101,
		 0x003E,0x00C4,0x0101,0x003F,0x02B6,0x0102,0x0042,0x0082,
		 0x0102,0x00A2,0x0339,0x0101,0x00A5,0x0000,0x0102,0x0179,
		 0x02F8,0x0101,0x017C,0x0000,0x0102,0x01AD,0x0000,0x0101,
		 0x0305,0x0000,0x0102,0x0306,0x0000,0x0101,0x0307,0x0080,
		 0x0102,0x0317,0x02B7,0x0102,0x031B,0x0276,0x0101,0x032C,
		 0x0000,0x0102,0x0331,0x037A,0x0101,0x0333,0x0276,0x0102,
		 0x0334,0x03BA,
		 // Variant 0x0001: 82 Internal Links to Resolve
		 //	Root[12] = [1,992: $M.CVC_SEND_CAP.speaker_data_1mic.cvc_send_internal_streams] + 0 
		 //	Root[26] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj][4] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 2 
		 //	[1,22: $M.CVC_SEND_CAP.speaker_data_1mic.fba_ref][2] = [2,52: $M.CVC_SEND_CAP.speaker_data_1mic.bufd_aec_inp] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][1] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 90 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][8] = [1,992: $M.CVC_SEND_CAP.speaker_data_1mic.stream_x] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][11] = [4,0: $M.CVC_SEND_CAP.speaker_data_1mic.RcvBuf_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][12] = [7,0: $M.CVC_SEND_CAP.speaker_data_1mic.RcvBuf_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][13] = [2,1012: $M.CVC_SEND_CAP.speaker_data_1mic.BExp_X_buf] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][16] = [6,0: $M.CVC_SEND_CAP.speaker_data_1mic.Ga_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][17] = [3,0: $M.CVC_SEND_CAP.speaker_data_1mic.Ga_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][18] = [9,856: $M.CVC_SEND_CAP.speaker_data_1mic.BExp_Ga] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][20] = [11,0: $M.CVC_SEND_CAP.speaker_data_1mic.LPwrX0] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][21] = [10,729: $M.CVC_SEND_CAP.speaker_data_1mic.LPwrX1] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][22] = [11,573: $M.CVC_SEND_CAP.speaker_data_1mic.RatFE] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][23] = [11,509: $M.CVC_SEND_CAP.speaker_data_1mic.Gr_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][24] = [11,445: $M.CVC_SEND_CAP.speaker_data_1mic.Gr_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][25] = [11,317: $M.CVC_SEND_CAP.speaker_data_1mic.SqGr] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][26] = [8,384: $M.CVC_SEND_CAP.speaker_data_1mic.L2absGr] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][27] = [11,381: $M.CVC_SEND_CAP.speaker_data_1mic.LPwrD] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][28] = [10,858: $M.CVC_SEND_CAP.speaker_data_1mic.LpZ_nz] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][29] = [10,600: $M.CVC_SEND_CAP.speaker_data_1mic.Cng_Nz_Shape_Tab] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][35] = [11,765: $M.CVC_SEND_CAP.speaker_data_1mic.DTC_lin] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][39] = [8,0: $M.CVC_SEND_CAP.speaker_data_1mic.Gb_real] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][40] = [5,480: $M.CVC_SEND_CAP.speaker_data_1mic.Gb_imag] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][41] = [11,253: $M.CVC_SEND_CAP.speaker_data_1mic.BExp_Gb] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][42] = [8,944: $M.CVC_SEND_CAP.speaker_data_1mic.L_RatSqG] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][4] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 79 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][6] = [1,822: $M.CVC_SEND_CAP.speaker_data_1mic.sndLpX_queue] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][9] = [9,600: $M.CVC_SEND_CAP.speaker_data_1mic.dms_state] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][17] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 78 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][8] = [1,992: $M.CVC_SEND_CAP.speaker_data_1mic.stream_x] + 0 
		 //	[1,309: $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd][11] = [1,997: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][6] = [8,808: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_LpX_queue] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][9] = [11,129: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_state] + 0 
		 //	[1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp][1] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 108 
		 //	[1,465: $M.CVC_SEND_CAP.speaker_data_1mic.snd_harm_obj][0] = [1,997: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[1,465: $M.CVC_SEND_CAP.speaker_data_1mic.snd_harm_obj][2] = [8,448: $M.CVC_SEND_CAP.speaker_data_1mic.harm_history] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][16] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][17] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][23] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,503: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice][25] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm][0] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 49 
		 //	[1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm][1] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm][2] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,531: $M.CVC_SEND_CAP.speaker_data_1mic.snd_agc400_dm][3] = [1,1027: $M.CVC_SEND_CAP.speaker_data_1mic.vad_agc_obj] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][5] = [1,992: $M.CVC_SEND_CAP.speaker_data_1mic.stream_x] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][8] = [1,997: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][11] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][14] = [1,1022: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][16] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[1,559: $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd][19] = [1,1022: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[1,584: $M.CVC_SEND_CAP.speaker_data_1mic.root][2] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 0 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][1] = [1,997: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][2] = [1,1002: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d1] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][3] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][14] = [1,1007: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d2] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][15] = [1,1012: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d3] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][16] = [1,992: $M.CVC_SEND_CAP.speaker_data_1mic.stream_x] + 4 
		 //	[1,606: $M.CVC_SEND_CAP.speaker_data_1mic.StatusArray][19] = [1,1022: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 4 
		 //	[1,645: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad410][3] = [1,626: $M.CVC_SEND_CAP.speaker_data_1mic.ref_vad_peq] + 0 
		 //	[1,662: $M.CVC_SEND_CAP.speaker_data_1mic.oms_wnr_obj][0] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 71 
		 //	[1,744: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va][4] = [1,1022: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[1,744: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va][5] = [1,1022: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[1,744: $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va][7] = [1,1022: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[1,781: $M.CVC_SEND_CAP.speaker_data_1mic.dms_obj][2] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 69 
		 //	[1,797: $M.CVC_SEND_CAP.speaker_data_1mic.fba_left][1] = [1,997: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[1,797: $M.CVC_SEND_CAP.speaker_data_1mic.fba_left][2] = [2,532: $M.CVC_SEND_CAP.speaker_data_1mic.bufd_l_inp] + 0 
		 //	[1,797: $M.CVC_SEND_CAP.speaker_data_1mic.fba_left][5] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 137 
		 //	[1,808: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream][0] = [5,0: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_buffer] + 0 
		 //	[1,808: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_stream][3] = [5,0: $M.CVC_SEND_CAP.speaker_data_1mic.ref_delay_buffer] + 0 
		 //	[1,816: $M.CVC_SEND_CAP.speaker_data_1mic.AEC_Et][0] = [11,637: $M.CVC_SEND_CAP.speaker_data_1mic.Et_real] + 0 
		 //	[1,819: $M.CVC_SEND_CAP.speaker_data_1mic.AEC_Dt][0] = [11,701: $M.CVC_SEND_CAP.speaker_data_1mic.Dt_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.speaker_data_1mic.snd_peq_dm2][0] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.speaker_data_1mic.snd_peq_dm2][1] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.speaker_data_1mic.snd_peq_dm2][3] = [5,864: $M.CVC_SEND_CAP.speaker_data_1mic.CurParams] + 10 
		 //	[2,23: $M.CVC_SEND_CAP.speaker_data_1mic.in_l_dcblock_dm2][0] = [1,997: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[2,23: $M.CVC_SEND_CAP.speaker_data_1mic.in_l_dcblock_dm2][1] = [1,997: $M.CVC_SEND_CAP.speaker_data_1mic.stream_d0] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1][1] = [1,1022: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z1] + 0 
		 //	[2,38: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd_va1][2] = [9,0: $M.CVC_SEND_CAP.speaker_data_1mic.bufd_outp_va1] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd][1] = [1,1017: $M.CVC_SEND_CAP.speaker_data_1mic.stream_z0] + 0 
		 //	[2,45: $M.CVC_SEND_CAP.speaker_data_1mic.fbs_snd][2] = [10,0: $M.CVC_SEND_CAP.speaker_data_1mic.bufd_outp] + 0 
		 $DYN_SECTION_TYPE_RELOC_INST,0x0001,249,
		 0x0052,0x0000,0x0000,0x0001,0x000C,0x03E0,0x0005,0x001A,
		 0x0360,0x0105,0x0004,0x0362,0x0102,0x0018,0x0034,0x0105,
		 0x001D,0x03BA,0x0101,0x0024,0x03E0,0x0104,0x0027,0x0000,
		 0x0107,0x0028,0x0000,0x0102,0x0029,0x03F4,0x0106,0x002C,
		 0x0000,0x0103,0x002D,0x0000,0x0109,0x002E,0x0358,0x010B,
		 0x0030,0x0000,0x010A,0x0031,0x02D9,0x010B,0x0032,0x023D,
		 0x010B,0x0033,0x01FD,0x010B,0x0034,0x01BD,0x010B,0x0035,
		 0x013D,0x0108,0x0036,0x0180,0x010B,0x0037,0x017D,0x010A,
		 0x0038,0x035A,0x010A,0x0039,0x0258,0x010B,0x003F,0x02FD,
		 0x0108,0x0043,0x0000,0x0105,0x0044,0x01E0,0x010B,0x0045,
		 0x00FD,0x0108,0x0046,0x03B0,0x0105,0x009F,0x03AF,0x0101,
		 0x00A1,0x0336,0x0109,0x00A4,0x0258,0x0105,0x00AC,0x03AE,
		 0x0101,0x013D,0x03E0,0x0101,0x0140,0x03E5,0x0108,0x0178,
		 0x0328,0x010B,0x017B,0x0081,0x0105,0x01A9,0x03CC,0x0101,
		 0x01D1,0x03E5,0x0108,0x01D3,0x01C0,0x0101,0x0207,0x03F9,
		 0x0101,0x0208,0x03F9,0x0101,0x020E,0x03F9,0x0101,0x0210,
		 0x03F9,0x0105,0x0213,0x0391,0x0101,0x0214,0x03F9,0x0101,
		 0x0215,0x03F9,0x0101,0x0216,0x0403,0x0101,0x0234,0x03E0,
		 0x0101,0x0237,0x03E5,0x0101,0x023A,0x03F9,0x0101,0x023D,
		 0x03FE,0x0101,0x023F,0x03F9,0x0101,0x0242,0x03FE,0x0105,
		 0x024A,0x0360,0x0101,0x025F,0x03E9,0x0101,0x0260,0x03EE,
		 0x0101,0x0261,0x03FD,0x0101,0x026C,0x03F3,0x0101,0x026D,
		 0x03F8,0x0101,0x026E,0x03E4,0x0101,0x0271,0x0402,0x0101,
		 0x0288,0x0272,0x0105,0x0296,0x03A7,0x0101,0x02EC,0x03FE,
		 0x0101,0x02ED,0x03FE,0x0101,0x02EF,0x03FE,0x0105,0x030F,
		 0x03A5,0x0101,0x031E,0x03E5,0x0102,0x031F,0x0214,0x0105,
		 0x0322,0x03E9,0x0105,0x0328,0x0000,0x0105,0x032B,0x0000,
		 0x010B,0x0330,0x027D,0x010B,0x0333,0x02BD,0x0201,0x0000,
		 0x03F9,0x0201,0x0001,0x03F9,0x0205,0x0003,0x036A,0x0201,
		 0x0017,0x03E5,0x0201,0x0018,0x03E5,0x0201,0x0027,0x03FE,
		 0x0209,0x0028,0x0000,0x0201,0x002E,0x03F9,0x020A,0x002F,
		 0x0000,
		 // Variant 0x0001: 26 Internal Links to Resolve (Scratch)
		 //	[1,0: $M.CVC_SEND_CAP.speaker_data_1mic.ndvc_obj][3] = [2,387: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][6] = [4,258: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_G] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][7] = [2,387: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][30] = [1,259: $M.CVC_SEND_CAP.speaker_data_1mic.L_adaptA] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][31] = [3,0: $M.CVC_SEND_CAP.speaker_data_1mic.Exp_Mts_adapt] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][32] = [2,258: $M.CVC_SEND_CAP.speaker_data_1mic.AttenuationPersist] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][33] = [1,0: $M.CVC_SEND_CAP.speaker_data_1mic.W_ri] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][34] = [1,388: $M.CVC_SEND_CAP.speaker_data_1mic.L_adaptR] + 0 
		 //	[1,28: $M.CVC_SEND_CAP.speaker_data_1mic.aec_obj][38] = [3,258: $M.CVC_SEND_CAP.speaker_data_1mic.rerdt_dtc] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][7] = [4,387: $M.CVC_SEND_CAP.speaker_data_1mic.G_dmsZ] + 0 
		 //	[1,155: $M.CVC_SEND_CAP.speaker_data_1mic.dms200_obj][10] = [1,0: $dm1_scratch] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][7] = [4,258: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_G] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][8] = [2,387: $M.CVC_SEND_CAP.speaker_data_1mic.in0oms_LpXnz] + 0 
		 //	[1,370: $M.CVC_SEND_CAP.speaker_data_1mic.oms280in0_obj][10] = [1,0: $dm1_scratch] + 0 
		 //	[1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp][4] = [2,258: $M.CVC_SEND_CAP.speaker_data_1mic.AttenuationPersist] + 0 
		 //	[1,424: $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp][5] = [3,0: $dm2_scratch] + 0 
		 //	[1,772: $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj][1] = [1,0: $dm1_scratch] + 0 
		 //	[1,772: $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj][2] = [3,0: $dm2_scratch] + 0 
		 //	[1,772: $M.CVC_SEND_CAP.speaker_data_1mic.fft_obj][3] = [1,0: $dm1_scratch] + 128 
		 //	[1,789: $M.CVC_SEND_CAP.speaker_data_1mic.block?0][1] = [2,129: $M.CVC_SEND_CAP.speaker_data_1mic.X_real] + 0 
		 //	[1,789: $M.CVC_SEND_CAP.speaker_data_1mic.block?0][2] = [4,129: $M.CVC_SEND_CAP.speaker_data_1mic.X_imag] + 0 
		 //	[1,789: $M.CVC_SEND_CAP.speaker_data_1mic.block?0][5] = [2,0: $M.CVC_SEND_CAP.speaker_data_1mic.D_l_real] + 0 
		 //	[1,789: $M.CVC_SEND_CAP.speaker_data_1mic.block?0][6] = [4,0: $M.CVC_SEND_CAP.speaker_data_1mic.D_l_imag] + 0 
		 //	[1,812: $M.CVC_SEND_CAP.speaker_data_1mic.vad_peq_output][0] = [1,0: $dm1_scratch] + 0 
		 //	[1,816: $M.CVC_SEND_CAP.speaker_data_1mic.AEC_Et][1] = [3,630: $M.CVC_SEND_CAP.speaker_data_1mic.Et_imag] + 0 
		 //	[1,819: $M.CVC_SEND_CAP.speaker_data_1mic.AEC_Dt][1] = [3,694: $M.CVC_SEND_CAP.speaker_data_1mic.Dt_imag] + 0 
		 $DYN_SECTION_TYPE_RELOC_SCRATCH,0x0001,81,
		 0x001A,0x0000,0x0001,0x0102,0x0003,0x0183,0x0104,0x0022,
		 0x0102,0x0102,0x0023,0x0183,0x0101,0x003A,0x0103,0x0103,
		 0x003B,0x0000,0x0102,0x003C,0x0102,0x0101,0x003D,0x0000,
		 0x0101,0x003E,0x0184,0x0103,0x0042,0x0102,0x0104,0x00A2,
		 0x0183,0x0101,0x00A5,0x0000,0x0104,0x0179,0x0102,0x0102,
		 0x017A,0x0183,0x0101,0x017C,0x0000,0x0102,0x01AC,0x0102,
		 0x0103,0x01AD,0x0000,0x0101,0x0305,0x0000,0x0103,0x0306,
		 0x0000,0x0101,0x0307,0x0080,0x0102,0x0316,0x0081,0x0104,
		 0x0317,0x0081,0x0102,0x031A,0x0000,0x0104,0x031B,0x0000,
		 0x0101,0x032C,0x0000,0x0103,0x0331,0x0276,0x0103,0x0334,
		 0x02B6,
#endif /* EXPORTED_CONSTANTS_ONCHIP_BUILD */
		 //End of Descriptor
		 $DYN_SECTION_TYPE_END;


#if !defined(EXPORTED_CONSTANTS_ONCHIP_BUILD) || EXPORTED_CONSTANTS_ONCHIP_BUILD
	.VAR/CONST $M.CVC_SEND_CAP.speaker_data_1mic.Downloadable.DynTable_Linker[] = 
		 // Variant 0xFFFF: 89 External Links to resolve
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[0] = &$cvc.init.root 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[3] = &$cvc.peq.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[6] = &$cvc.peq.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[9] = &$cvc.peq.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[12] = &$filter_bank.synthesis.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[15] = &$filter_bank.synthesis.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[18] = &$filter_bank.analysis.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[21] = &$harm100.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[22] = &$cvc.init.harm 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[24] = &$oms280.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[25] = &$cvc.init.oms_in 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[27] = &$ndvc200.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[28] = &$cvc.init.ndvc200 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[30] = &$vad410.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[33] = &$filter_bank.analysis.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[36] = &$aec520.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[37] = &$cvc.init.aec520 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[39] = &$aec520.nlp.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[40] = &$cvc.init.vsm_fdnlp 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[42] = &$dms200.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[43] = &$cvc.init.dms200 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[45] = &$cvc.user.dms200.wnr.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[48] = &$cvc.user.dms200.va_init 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[51] = &$aed100.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[52] = &$cvc.init.aed100 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[54] = &$cvc.peq.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[57] = &$agc400.initialize 
		 //	[1,248] $M.CVC_SEND_CAP.speaker_data_1mic.ReInitializeTable[58] = &$cvc.init.agc400 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[0] = &$frame_proc.distribute_streams 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[3] = &$cvc.pre_process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[6] = &$cvc.stream_transfer.peak 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[9] = &$cvc.stream_transfer.peak 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[12] = &$cvc.peq.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[15] = &$cvc.peq.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[18] = &$cvc.peq.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[21] = &$filter_bank.analysis.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[22] = &$cvc.mc.ref_delay 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[24] = &$vad410.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[27] = &$cvc.event.echo_flag 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[30] = &$harm100.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[33] = &$filter_bank.analysis.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[36] = &$dms200.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[37] = &$cvc.mc.oms_in 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[39] = &$aec520.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[40] = &$cvc.mc.aec520 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[42] = &$dms200.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[43] = &$cvc.mc.dms200 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[45] = &$dms200.apply_gain 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[46] = &$cvc.mc.dms_out 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[48] = &$ndvc200.process 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[51] = &$cvc.run_function_table 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[52] = &$cvc.mc.voice 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[54] = &$cvc.run_function_table 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[55] = &$cvc.mc.va 
		 //	[1,309] $M.CVC_SEND_CAP.speaker_data_1mic.hfk_proc_funcsSnd[57] = &$frame_proc.update_streams 
		 //	[1,424] $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp[6] = &$aec520.FdnlpProcess 
		 //	[1,424] $M.CVC_SEND_CAP.speaker_data_1mic.vsm_fdnlp[7] = &$aec520.VsmProcess 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[0] = &$aed100.process 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[1] = &$cvc.mc.aed100 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[3] = &$aec520.nlp.process 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[4] = &$cvc.mc.aec520_nlp 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[6] = &$aec520.cng.process 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[7] = &$cvc.mc.aec520_cng 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[9] = &$filter_bank.synthesis.process 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[12] = &$cvc.peq.process 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[15] = &$cvc_send.stream_gain.process.voice 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[18] = &$agc400.process 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[19] = &$cvc.mc.agc400 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[21] = &$cvc_send.mute_control 
		 //	[1,503] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_voice[24] = &$cvc.stream_transfer.peak.output 
		 //	[1,559] $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd[0] = &$frame_proc.distribute_streams 
		 //	[1,559] $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd[3] = &$cvc.stream_transfer.peak 
		 //	[1,559] $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd[6] = &$cvc.stream_transfer.peak 
		 //	[1,559] $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd[9] = &$cvc_send.passthrough.voice 
		 //	[1,559] $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd[12] = &$cvc_send.passthrough.va 
		 //	[1,559] $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd[15] = &$cvc.stream_transfer.peak.output 
		 //	[1,559] $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd[18] = &$cvc.stream_transfer.peak.output 
		 //	[1,559] $M.CVC_SEND_CAP.speaker_data_1mic.copy_proc_funcsSnd[21] = &$frame_proc.update_streams 
		 //	[1,679] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout_va1[6] = &$frame_proc.distribute_output_stream 
		 //	[1,679] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout_va1[7] = &$frame_proc.update_output_stream 
		 //	[1,692] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_left_adc[6] = &$frame_proc.distribute_input_stream 
		 //	[1,692] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_left_adc[7] = &$frame_proc.update_input_stream 
		 //	[1,705] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_refin[6] = &$frame_proc.distribute_input_stream 
		 //	[1,705] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_refin[7] = &$frame_proc.update_input_stream 
		 //	[1,718] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout[6] = &$frame_proc.distribute_output_stream 
		 //	[1,718] $M.CVC_SEND_CAP.speaker_data_1mic.stream_map_sndout[7] = &$frame_proc.update_output_stream 
		 //	[1,744] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va[0] = &$filter_bank.synthesis.process 
		 //	[1,744] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va[3] = &$cvc_send.stream_gain.process.va 
		 //	[1,744] $M.CVC_SEND_CAP.speaker_data_1mic.proc_stream_va[6] = &$cvc.stream_transfer.peak.output 
		 $DYN_SECTION_TYPE_DATA_INST,0xFFFF,178,
		 0x0100F8,$cvc.init.root,
		 0x0100FB,$cvc.peq.initialize,
		 0x0100FE,$cvc.peq.initialize,
		 0x010101,$cvc.peq.initialize,
		 0x010104,$filter_bank.synthesis.initialize,
		 0x010107,$filter_bank.synthesis.initialize,
		 0x01010A,$filter_bank.analysis.initialize,
		 0x01010D,$harm100.initialize,
		 0x01010E,$cvc.init.harm,
		 0x010110,$oms280.initialize,
		 0x010111,$cvc.init.oms_in,
		 0x010113,$ndvc200.initialize,
		 0x010114,$cvc.init.ndvc200,
		 0x010116,$vad410.initialize,
		 0x010119,$filter_bank.analysis.initialize,
		 0x01011C,$aec520.initialize,
		 0x01011D,$cvc.init.aec520,
		 0x01011F,$aec520.nlp.initialize,
		 0x010120,$cvc.init.vsm_fdnlp,
		 0x010122,$dms200.initialize,
		 0x010123,$cvc.init.dms200,
		 0x010125,$cvc.user.dms200.wnr.initialize,
		 0x010128,$cvc.user.dms200.va_init,
		 0x01012B,$aed100.initialize,
		 0x01012C,$cvc.init.aed100,
		 0x01012E,$cvc.peq.initialize,
		 0x010131,$agc400.initialize,
		 0x010132,$cvc.init.agc400,
		 0x010135,$frame_proc.distribute_streams,
		 0x010138,$cvc.pre_process,
		 0x01013B,$cvc.stream_transfer.peak,
		 0x01013E,$cvc.stream_transfer.peak,
		 0x010141,$cvc.peq.process,
		 0x010144,$cvc.peq.process,
		 0x010147,$cvc.peq.process,
		 0x01014A,$filter_bank.analysis.process,
		 0x01014B,$cvc.mc.ref_delay,
		 0x01014D,$vad410.process,
		 0x010150,$cvc.event.echo_flag,
		 0x010153,$harm100.process,
		 0x010156,$filter_bank.analysis.process,
		 0x010159,$dms200.process,
		 0x01015A,$cvc.mc.oms_in,
		 0x01015C,$aec520.process,
		 0x01015D,$cvc.mc.aec520,
		 0x01015F,$dms200.process,
		 0x010160,$cvc.mc.dms200,
		 0x010162,$dms200.apply_gain,
		 0x010163,$cvc.mc.dms_out,
		 0x010165,$ndvc200.process,
		 0x010168,$cvc.run_function_table,
		 0x010169,$cvc.mc.voice,
		 0x01016B,$cvc.run_function_table,
		 0x01016C,$cvc.mc.va,
		 0x01016E,$frame_proc.update_streams,
		 0x0101AE,$aec520.FdnlpProcess,
		 0x0101AF,$aec520.VsmProcess,
		 0x0101F7,$aed100.process,
		 0x0101F8,$cvc.mc.aed100,
		 0x0101FA,$aec520.nlp.process,
		 0x0101FB,$cvc.mc.aec520_nlp,
		 0x0101FD,$aec520.cng.process,
		 0x0101FE,$cvc.mc.aec520_cng,
		 0x010200,$filter_bank.synthesis.process,
		 0x010203,$cvc.peq.process,
		 0x010206,$cvc_send.stream_gain.process.voice,
		 0x010209,$agc400.process,
		 0x01020A,$cvc.mc.agc400,
		 0x01020C,$cvc_send.mute_control,
		 0x01020F,$cvc.stream_transfer.peak.output,
		 0x01022F,$frame_proc.distribute_streams,
		 0x010232,$cvc.stream_transfer.peak,
		 0x010235,$cvc.stream_transfer.peak,
		 0x010238,$cvc_send.passthrough.voice,
		 0x01023B,$cvc_send.passthrough.va,
		 0x01023E,$cvc.stream_transfer.peak.output,
		 0x010241,$cvc.stream_transfer.peak.output,
		 0x010244,$frame_proc.update_streams,
		 0x0102AD,$frame_proc.distribute_output_stream,
		 0x0102AE,$frame_proc.update_output_stream,
		 0x0102BA,$frame_proc.distribute_input_stream,
		 0x0102BB,$frame_proc.update_input_stream,
		 0x0102C7,$frame_proc.distribute_input_stream,
		 0x0102C8,$frame_proc.update_input_stream,
		 0x0102D4,$frame_proc.distribute_output_stream,
		 0x0102D5,$frame_proc.update_output_stream,
		 0x0102E8,$filter_bank.synthesis.process,
		 0x0102EB,$cvc_send.stream_gain.process.va,
		 0x0102EE,$cvc.stream_transfer.peak.output,
		 //End of Descriptor
		 $DYN_SECTION_TYPE_END;

#endif /* EXPORTED_CONSTANTS_ONCHIP_BUILD */


.ENDMODULE; 

#endif /* !defined(EXPORTED_CONSTANTS_ONCHIP_BUILD) || EXPORTED_CONSTANTS_ONCHIP_BUILD */
