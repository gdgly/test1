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
.MODULE $M.CVC_SEND_CAP.headset_data_2mic.Downloadable; 
	.DATASEGMENT DM; 

	.VAR16/CONST16 DynTable_Main[] = 
	/* If constants are moved off-chip to a file, the code needs 
	  to reference this variable */
.set $_CVC_SEND_CAP_headset_data_2mic_DownloadableDynTable_Main, DynTable_Main
#if !defined(EXPORTED_CONSTANTS_ONCHIP_BUILD) || EXPORTED_CONSTANTS_ONCHIP_BUILD 
		 // Variant 0x0000: 8256 words allocated, 155 not used
		 //	Size 	DM0 		DM1 		DM2
		 //	 10 	 1 		 0 		 0
		 //	1032 	 3 		 3 		 2
		 //
		 //	Block[00] Bank=DM0 Size=10 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=0 
		 //		 [I] DM1?$M.CVC_SEND_CAP.headset_data_2mic.asf_object Size=102
		 //		 [I] DM1?$M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj Size=22
		 //		 [I] DM1?$M.CVC_SEND_CAP.headset_data_2mic.fba_ref Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.aec_obj Size=127
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.dms200_obj Size=93
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd Size=91
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable Size=76
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.block?3 Size=74
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj Size=54
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp Size=41
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj Size=38
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice Size=34
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj Size=27
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.root Size=22
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.StatusArray Size=20
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.ref_vad_peq Size=19
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.ref_vad410 Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.oms_wnr_obj Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout_va1 Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.stream_map_right_adc Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.stream_map_left_adc Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.stream_map_refin Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.aed100_obj Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va Size=10
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj Size=8
		 //	Block[02] Bank=DM2 Size=1032  Available=14 
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent Size=24
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.snd_peq_dm2 Size=23
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.in_r_dcblock_dm2 Size=15
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.in_l_dcblock_dm2 Size=15
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1 Size=7
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.fbs_snd Size=7
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.block?1 Size=10
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.vad_default_param Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.fft_obj Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.dms_obj Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.block?0 Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.snd_streams Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.fba_left Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.ref_delay_stream Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.vad_peq_output Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.AEC_Et Size=3
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.AEC_Dt Size=3
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.harm_inputs Size=2
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.TP_data_dm2 Size=168
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.GaMic1_real Size=130
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.Ga_real Size=130
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.RcvBuf_imag Size=130
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.bufd_aec_inp Size=120
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.bufd_r_inp Size=120
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.dms_Ath Size=40
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.vad_agc_obj Size=5
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.BExp_X_buf Size=3
		 //	Block[03] Bank=DM1 Size=1032  Available=19 
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_2mic.ref_delay_buffer Size=240
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_2mic.TP_data_dm1 Size=168
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_2mic.GaMic1_imag Size=130
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_2mic.Ga_imag Size=130
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_2mic.RcvBuf_real Size=130
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.harm_history Size=180
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.cvc_send_internal_streams Size=35
		 //	Block[04] Bank=DM2 Size=1032  Available=38 
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.bufd_l_inp Size=120
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.L2absGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.dms_state Size=256
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.block?2 Size=235
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.bufd_outp_va1 Size=180
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.CurParams Size=139
		 //	Block[05] Bank=DM0 Size=1032  Available=11 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.bufd_outp Size=180
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.sndLpX_queue Size=136
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.in0oms_LpX_queue Size=136
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.wnr_g Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.coh Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.pXcI Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.pXcR Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.BExp_GaMic1 Size=65
		 //	Block[06] Bank=DM0 Size=1032  Available=30 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.pX1 Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.pX0 Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.in0oms_state Size=104
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.BExp_Ga Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.Cng_Nz_Shape_Tab Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.LPwrX1 Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.LpZ_nz Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.LPwrX0 Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.spp Size=65
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.SqGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.LPwrD Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.Gr_real Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.Gr_imag Size=64
		 //	Block[07] Bank=DM0 Size=1032  Available=23 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.RatFE Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.z1_aux_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.z0_aux_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.x1_saved_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.x0_saved_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.coh_sin Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_w1_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_w0_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam_w1_aux_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam_w0_aux_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_w1_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_phi_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_phi_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_cc_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_cc_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.z1_aux_real Size=63
		 //	Block[08] Bank=DM1 Size=1032  Available=20 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.z0_aux_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.x1_saved_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.x0_saved_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.coh_cos Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_w1_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_w0_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam_w1_aux_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam_w0_aux_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_w1_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_phi_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_phi_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_cc_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_cc_real Size=63
		 //		*[U] SM1?$M.CVC_SEND_CAP.headset_data_2mic.Et_real Size=64
		 //		*[U] SM1?$M.CVC_SEND_CAP.headset_data_2mic.Dt_real Size=64
		 //		*[U] SM?$M.CVC_SEND_CAP.headset_data_2mic.DTC_lin Size=65
		 $DYN_SECTION_TYPE_ALLOC_INST,0x0000,16,
		 0x0009,0x0000,0x0001,0x000A,0x0408,0x0201,0x03FA,0x03F5,
		 0x0200,0x03E2,0x03FD,0x0000,0x03EA,0x03F1,0x0100,0x03F4,
		 // Variant 0x0000: 4128 words Scratch allocated, 1570 not used
		 //	Size 	SM0 		SM1 		SM2
		 //	  6 	 1 		 0 		 0
		 //	1032 	 0 		 2 		 2
		 //
		 //	Block[00] Bank=DM0 Size=6 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=77 
		 //		 SM1?$dm1_scratch Size=630
		 //		 SM1?$M.CVC_SEND_CAP.headset_data_2mic.FFT_DM1 Size=325
		 //	Block[02] Bank=DM1 Size=1032  Available=772 
		 //		 SM1?$M.CVC_SEND_CAP.headset_data_2mic.dmss_sm1 Size=260
		 //	Block[03] Bank=DM2 Size=1032  Available=13 
		 //		 SM2?$dm2_scratch Size=630
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_2mic.FFT_DM2 Size=325
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_2mic.Et_imag Size=64
		 //	Block[04] Bank=DM2 Size=1032  Available=708 
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_2mic.dmss_sm2 Size=260
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_2mic.Dt_imag Size=64
		 $DYN_SECTION_TYPE_ALLOC_SCRATCH,0x0000,10,
		 0x0005,0x0001,0x0001,0x0006,0x03BB,0x0102,0x0104,0x03FB,
		 0x0200,0x0144,
		 // Variant 0x0001: 11352 words allocated, 337 not used
		 //	Size 	DM0 		DM1 		DM2
		 //	 13 	 1 		 0 		 0
		 //	1032 	 5 		 4 		 2
		 //
		 //	Block[00] Bank=DM0 Size=13 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=0 
		 //		 [I] DM1?$M.CVC_SEND_CAP.headset_data_2mic.asf_object Size=102
		 //		 [I] DM1?$M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj Size=22
		 //		 [I] DM1?$M.CVC_SEND_CAP.headset_data_2mic.fba_ref Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.aec_obj Size=127
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.dms200_obj Size=93
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd Size=91
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable Size=76
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.block?3 Size=74
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj Size=54
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp Size=41
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj Size=38
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice Size=34
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm Size=28
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj Size=27
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.root Size=22
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.StatusArray Size=20
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.ref_vad_peq Size=19
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.ref_vad410 Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.oms_wnr_obj Size=17
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout_va1 Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.stream_map_right_adc Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.stream_map_left_adc Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.stream_map_refin Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.aed100_obj Size=13
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va Size=10
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj Size=8
		 //	Block[02] Bank=DM2 Size=1032  Available=14 
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent Size=24
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.snd_peq_dm2 Size=23
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.in_r_dcblock_dm2 Size=15
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.in_l_dcblock_dm2 Size=15
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1 Size=7
		 //		 [I] DM2?$M.CVC_SEND_CAP.headset_data_2mic.fbs_snd Size=7
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.block?1 Size=10
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.vad_default_param Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.fft_obj Size=9
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.dms_obj Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.block?0 Size=8
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.snd_streams Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.fba_left Size=6
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.ref_delay_stream Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.vad_peq_output Size=4
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.AEC_Et Size=3
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.AEC_Dt Size=3
		 //		 [I] DM?$M.CVC_SEND_CAP.headset_data_2mic.harm_inputs Size=2
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.GaMic1_real Size=258
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.Ga_real Size=258
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.RcvBuf_imag Size=258
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.L2absGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.vad_agc_obj Size=5
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.BExp_X_buf Size=3
		 //	Block[03] Bank=DM1 Size=1032  Available=1 
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_2mic.ref_delay_buffer Size=480
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_2mic.GaMic1_imag Size=258
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_2mic.Ga_imag Size=258
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.cvc_send_internal_streams Size=35
		 //	Block[04] Bank=DM1 Size=1032  Available=11 
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_2mic.RcvBuf_real Size=258
		 //		 [U] DM1?$M.CVC_SEND_CAP.headset_data_2mic.TP_data_dm1 Size=168
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.harm_history Size=360
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.block?2 Size=235
		 //	Block[05] Bank=DM2 Size=1032  Available=5 
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.bufd_aec_inp Size=240
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.bufd_r_inp Size=240
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.bufd_l_inp Size=240
		 //		 [U] DM2?$M.CVC_SEND_CAP.headset_data_2mic.TP_data_dm2 Size=168
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.CurParams Size=139
		 //	Block[06] Bank=DM0 Size=1032  Available=16 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.bufd_outp_va1 Size=360
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.bufd_outp Size=360
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.dms_state Size=256
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.dms_Ath Size=40
		 //	Block[07] Bank=DM0 Size=1032  Available=16 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.sndLpX_queue Size=170
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.in0oms_LpX_queue Size=136
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.BExp_GaMic1 Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.BExp_Ga Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.Cng_Nz_Shape_Tab Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.LPwrX1 Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.LpZ_nz Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.spp Size=65
		 //	Block[08] Bank=DM0 Size=1032  Available=21 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.LPwrX0 Size=129
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.z1_aux_imag Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.z0_aux_imag Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.x1_saved_imag Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.x0_saved_imag Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.z1_aux_real Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.z0_aux_real Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.x1_saved_real Size=126
		 //	Block[09] Bank=DM0 Size=1032  Available=26 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.x0_saved_real Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.wnr_g Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.coh Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.pXcI Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.pXcR Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.pX1 Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.pX0 Size=126
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.in0oms_state Size=124
		 //	Block[10] Bank=DM0 Size=1032  Available=19 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.SqGr Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.LPwrD Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.Gr_real Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.Gr_imag Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.RatFE Size=64
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.coh_sin Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_w1_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_w0_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam_w1_aux_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam_w0_aux_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_w1_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_phi_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_phi_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_cc_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_cc_imag Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.coh_cos Size=63
		 //	Block[11] Bank=DM1 Size=1032  Available=208 
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_w1_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_w0_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam_w1_aux_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam_w0_aux_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_w1_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_phi_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_phi_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam1_cc_real Size=63
		 //		 [U] DM?$M.CVC_SEND_CAP.headset_data_2mic.beam0_cc_real Size=63
		 //		*[U] SM1?$M.CVC_SEND_CAP.headset_data_2mic.Et_real Size=64
		 //		*[U] SM1?$M.CVC_SEND_CAP.headset_data_2mic.Dt_real Size=64
		 //		*[U] SM?$M.CVC_SEND_CAP.headset_data_2mic.DTC_lin Size=129
		 $DYN_SECTION_TYPE_ALLOC_INST,0x0001,20,
		 0x000C,0x0000,0x0001,0x000D,0x0408,0x0201,0x03FA,0x0407,
		 0x0102,0x03FD,0x0403,0x0000,0x03F8,0x03F8,0x0000,0x03F3,
		 0x03EE,0x0001,0x03F5,0x0338,
		 // Variant 0x0001: 6192 words Scratch allocated, 2482 not used
		 //	Size 	SM0 		SM1 		SM2
		 //	  8 	 1 		 0 		 0
		 //	1032 	 0 		 3 		 3
		 //
		 //	Block[00] Bank=DM0 Size=8 (Allocation Block)
		 //	Block[01] Bank=DM1 Size=1032  Available=387 
		 //		 SM1?$M.CVC_SEND_CAP.headset_data_2mic.FFT_DM1 Size=645
		 //	Block[02] Bank=DM1 Size=1032  Available=402 
		 //		 SM1?$dm1_scratch Size=630
		 //	Block[03] Bank=DM1 Size=1032  Available=516 
		 //		 SM1?$M.CVC_SEND_CAP.headset_data_2mic.dmss_sm1 Size=516
		 //	Block[04] Bank=DM2 Size=1032  Available=259 
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_2mic.FFT_DM2 Size=645
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_2mic.Et_imag Size=64
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_2mic.Dt_imag Size=64
		 //	Block[05] Bank=DM2 Size=1032  Available=402 
		 //		 SM2?$dm2_scratch Size=630
		 //	Block[06] Bank=DM2 Size=1032  Available=516 
		 //		 SM2?$M.CVC_SEND_CAP.headset_data_2mic.dmss_sm2 Size=516
		 $DYN_SECTION_TYPE_ALLOC_SCRATCH,0x0001,13,
		 0x0007,0x0001,0x0001,0x0008,0x0285,0x0101,0x0276,0x0204,
		 0x0202,0x0305,0x0276,0x0200,0x0204,
		 // Variant 0xFFFF: Iniiatization Data 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][46] = 0x0 
		 // Word Offset 0x1000C , Length=1
		 // Word Offset 0x10066 , Length=2
		 // Word Offset 0x10085 , Length=1
		 // Word Offset 0x1010C , Length=3
		 // Word Offset 0x101BA , Length=1
		 // Word Offset 0x1025A , Length=2
		 // Word Offset 0x10349 , Length=1
		 // Word Offset 0x1034E , Length=1
		 // Word Offset 0x10375 , Length=1
		 // Word Offset 0x2001A , Length=1
		 // Word Offset 0x20031 , Length=1
		 // Word Offset 0x20040 , Length=1
		 // Word Offset 0x20052 , Length=1
		 // Word Offset 0x20059 , Length=1
		 // Word Offset 0x2006E , Length=9
		 // Word Offset 0x2009E , Length=1
		 $DYN_SECTION_TYPE_DATA_INST,0xFFFF,88,
		 0x0101,0x000C,0x0000,0x0001,0x0102,0x0066,0x0000,0x0001,
		 0x0000,0x0008,0x0101,0x0085,0x01EB,0x851F,0x0103,0x010C,
		 0x3999,0x999A,0x0083,0x126F,0x0000,0x0001,0x0101,0x01BA,
		 0x0000,0x0002,0x0102,0x025A,0x3999,0x999A,0x0083,0x126F,
		 0x0101,0x0349,0x0000,0x5E0F,0x0101,0x034E,0x7FFF,0xFFFF,
		 0x0101,0x0375,0x0000,0x0003,0x0201,0x001A,0x0000,0x0005,
		 0x0201,0x0031,0x0000,0x0001,0x0201,0x0040,0x0000,0x0001,
		 0x0201,0x0052,0x0000,0x0001,0x0201,0x0059,0x0000,0x0001,
		 0x0209,0x006E,0x0600,0x0000,0x0600,0x0000,0x0010,0x624E,
		 0x0063,0xD70A,0x4000,0x0000,0x0080,0x0000,0xFD40,0x0000,
		 0x0050,0x0000,0x00E5,0x6042,0x0201,0x009E,0x0000,0x0780,
		 // Variant 0x0000: Iniiatization Data 
		 //	[1,841: $M.CVC_SEND_CAP.headset_data_2mic.root][17] = 0xE829FB 
		 //	[2,156: $M.CVC_SEND_CAP.headset_data_2mic.ref_delay_stream][2] = 0x3C0 
		 // Word Offset 0x1035A , Length=1
		 // Word Offset 0x2009E , Length=1
		 $DYN_SECTION_TYPE_DATA_INST,0x0000,8,
		 0x0101,0x035A,0x00E8,0x29FB,0x0201,0x009E,0x0000,0x03C0,
		 // Variant 0x0001: Iniiatization Data 
		 //	[1,841: $M.CVC_SEND_CAP.headset_data_2mic.root][17] = 0xE829FB 
		 // Word Offset 0x1035A , Length=1
		 $DYN_SECTION_TYPE_DATA_INST,0x0001,4,
		 0x0101,0x035A,0x00E8,0x29FB,
		 // Variant 0xFFFF: 184 Internal Links to Resolve
		 //	Root[2] = [1,975: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_refin] + 0 
		 //	Root[3] = [1,962: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_left_adc] + 0 
		 //	Root[4] = [1,949: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_right_adc] + 0 
		 //	Root[7] = [1,988: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout] + 0 
		 //	Root[8] = [1,936: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout_va1] + 0 
		 //	Root[22] = [2,101: $M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd] + 0 
		 //	Root[23] = [1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable] + 0 
		 //	Root[24] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 0 
		 //	Root[25] = [1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray] + 0 
		 //	Root[45] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	Root[63] = [1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj] + 9 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][0] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][1] = [2,92: $M.CVC_SEND_CAP.headset_data_2mic.D1] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][8] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 18 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][14] = [2,0: $M.CVC_SEND_CAP.headset_data_2mic.ASF_TEMP0] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][15] = [2,3: $M.CVC_SEND_CAP.headset_data_2mic.ASF_TEMP1] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][16] = [2,6: $M.CVC_SEND_CAP.headset_data_2mic.ASF_TEMP2] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][17] = [2,9: $M.CVC_SEND_CAP.headset_data_2mic.ASF_TEMP3] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][18] = [2,12: $M.CVC_SEND_CAP.headset_data_2mic.X0_SAVED_FREQ_OBJ] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][19] = [2,15: $M.CVC_SEND_CAP.headset_data_2mic.X1_SAVED_FREQ_OBJ] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][20] = [2,18: $M.CVC_SEND_CAP.headset_data_2mic.Z0_AUX_FREQ_OBJ] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][21] = [2,21: $M.CVC_SEND_CAP.headset_data_2mic.Z1_AUX_FREQ_OBJ] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][22] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 12 
		 //	[1,124: $M.CVC_SEND_CAP.headset_data_2mic.fba_ref][1] = [2,156: $M.CVC_SEND_CAP.headset_data_2mic.ref_delay_stream] + 0 
		 //	[1,124: $M.CVC_SEND_CAP.headset_data_2mic.fba_ref][3] = [2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj] + 0 
		 //	[1,124: $M.CVC_SEND_CAP.headset_data_2mic.fba_ref][4] = [2,137: $M.CVC_SEND_CAP.headset_data_2mic.X] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][9] = [2,156: $M.CVC_SEND_CAP.headset_data_2mic.ref_delay_stream] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][10] = [2,137: $M.CVC_SEND_CAP.headset_data_2mic.X] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][15] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][19] = [1,1024: $M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][36] = [2,167: $M.CVC_SEND_CAP.headset_data_2mic.AEC_Dt] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][37] = [2,164: $M.CVC_SEND_CAP.headset_data_2mic.AEC_Et] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][0] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][1] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][15] = [2,128: $M.CVC_SEND_CAP.headset_data_2mic.dms_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][1] = [2,144: $M.CVC_SEND_CAP.headset_data_2mic.snd_streams] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][7] = [1,975: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_refin] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][10] = [1,962: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_left_adc] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][13] = [1,949: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_right_adc] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][16] = [2,62: $M.CVC_SEND_CAP.headset_data_2mic.in_l_dcblock_dm2] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][19] = [2,47: $M.CVC_SEND_CAP.headset_data_2mic.in_r_dcblock_dm2] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][26] = [1,124: $M.CVC_SEND_CAP.headset_data_2mic.fba_ref] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][29] = [1,902: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad410] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][31] = [1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][32] = [1,902: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad410] + 9 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][35] = [1,686: $M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][38] = [2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][41] = [2,95: $M.CVC_SEND_CAP.headset_data_2mic.fba_right] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][44] = [1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][47] = [1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][49] = [2,170: $M.CVC_SEND_CAP.headset_data_2mic.harm_inputs] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][50] = [1,686: $M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][53] = [1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][56] = [1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][59] = [1,529: $M.CVC_SEND_CAP.headset_data_2mic.dmss_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][62] = [1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][65] = [1,529: $M.CVC_SEND_CAP.headset_data_2mic.dmss_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][68] = [1,529: $M.CVC_SEND_CAP.headset_data_2mic.dmss_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][71] = [1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][74] = [1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][77] = [1,529: $M.CVC_SEND_CAP.headset_data_2mic.dmss_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][80] = [1,102: $M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][83] = [1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][86] = [1,1014: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][88] = [2,144: $M.CVC_SEND_CAP.headset_data_2mic.snd_streams] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][4] = [2,62: $M.CVC_SEND_CAP.headset_data_2mic.in_l_dcblock_dm2] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][7] = [2,47: $M.CVC_SEND_CAP.headset_data_2mic.in_r_dcblock_dm2] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][14] = [2,84: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][17] = [2,77: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][20] = [2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][23] = [2,95: $M.CVC_SEND_CAP.headset_data_2mic.fba_right] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][26] = [1,686: $M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][29] = [1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][32] = [1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][35] = [1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][38] = [1,102: $M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][41] = [1,902: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad410] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][44] = [1,124: $M.CVC_SEND_CAP.headset_data_2mic.fba_ref] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][47] = [1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][50] = [1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][53] = [1,529: $M.CVC_SEND_CAP.headset_data_2mic.dmss_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][56] = [1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][59] = [1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][61] = [1,919: $M.CVC_SEND_CAP.headset_data_2mic.oms_wnr_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][62] = [1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][64] = [2,137: $M.CVC_SEND_CAP.headset_data_2mic.X] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][65] = [1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][68] = [1,1001: $M.CVC_SEND_CAP.headset_data_2mic.aed100_obj] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][70] = [2,24: $M.CVC_SEND_CAP.headset_data_2mic.snd_peq_dm2] + 0 
		 //	[1,441: $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable][74] = [1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][12] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][13] = [2,92: $M.CVC_SEND_CAP.headset_data_2mic.D1] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][14] = [1,517: $M.CVC_SEND_CAP.headset_data_2mic.PIZ0] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][15] = [1,520: $M.CVC_SEND_CAP.headset_data_2mic.PIZ1] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][16] = [1,523: $M.CVC_SEND_CAP.headset_data_2mic.POZ0] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][17] = [1,526: $M.CVC_SEND_CAP.headset_data_2mic.POZ1] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][24] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 15 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][27] = [1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj] + 7 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][28] = [1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj] + 7 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][0] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][1] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	[1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp][0] = [1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj] + 0 
		 //	[1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp][2] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 6 
		 //	[1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp][3] = [1,902: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad410] + 9 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][2] = [1,529: $M.CVC_SEND_CAP.headset_data_2mic.dmss_obj] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][5] = [1,1001: $M.CVC_SEND_CAP.headset_data_2mic.aed100_obj] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][8] = [1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][11] = [1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][14] = [1,529: $M.CVC_SEND_CAP.headset_data_2mic.dmss_obj] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][17] = [2,84: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][19] = [2,24: $M.CVC_SEND_CAP.headset_data_2mic.snd_peq_dm2] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][26] = [1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][32] = [1,988: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][1] = [2,144: $M.CVC_SEND_CAP.headset_data_2mic.snd_streams] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][4] = [1,975: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_refin] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][7] = [1,962: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_left_adc] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][10] = [1,949: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_right_adc] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][20] = [1,988: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][23] = [1,936: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout_va1] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][25] = [2,144: $M.CVC_SEND_CAP.headset_data_2mic.snd_streams] + 0 
		 //	[1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj][0] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	[1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj][1] = [2,92: $M.CVC_SEND_CAP.headset_data_2mic.D1] + 0 
		 //	[1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj][4] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 8 
		 //	[1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj][6] = [1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj] + 31 
		 //	[1,841: $M.CVC_SEND_CAP.headset_data_2mic.root][3] = [1,686: $M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj] + 0 
		 //	[1,841: $M.CVC_SEND_CAP.headset_data_2mic.root][4] = [1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj] + 0 
		 //	[1,841: $M.CVC_SEND_CAP.headset_data_2mic.root][19] = [1,529: $M.CVC_SEND_CAP.headset_data_2mic.dmss_obj] + 0 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][0] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 0 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][4] = [1,102: $M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj] + 6 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][5] = [1,102: $M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj] + 5 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][6] = [1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm] + 7 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][7] = [1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm] + 15 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][8] = [1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj] + 84 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][9] = [1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object] + 70 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][10] = [1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object] + 71 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][11] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 12 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][12] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 6 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][13] = [1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj] + 9 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][17] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 6 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][18] = [1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp] + 31 
		 //	[1,883: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad_peq][0] = [2,156: $M.CVC_SEND_CAP.headset_data_2mic.ref_delay_stream] + 0 
		 //	[1,883: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad_peq][1] = [2,160: $M.CVC_SEND_CAP.headset_data_2mic.vad_peq_output] + 0 
		 //	[1,902: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad410][0] = [2,156: $M.CVC_SEND_CAP.headset_data_2mic.ref_delay_stream] + 0 
		 //	[1,902: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad410][1] = [2,110: $M.CVC_SEND_CAP.headset_data_2mic.vad_default_param] + 0 
		 //	[1,919: $M.CVC_SEND_CAP.headset_data_2mic.oms_wnr_obj][1] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 6 
		 //	[1,919: $M.CVC_SEND_CAP.headset_data_2mic.oms_wnr_obj][2] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 14 
		 //	[1,919: $M.CVC_SEND_CAP.headset_data_2mic.oms_wnr_obj][3] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 12 
		 //	[1,1001: $M.CVC_SEND_CAP.headset_data_2mic.aed100_obj][0] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 14 
		 //	[1,1014: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va][2] = [2,77: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1] + 0 
		 //	[1,1014: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va][8] = [1,936: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout_va1] + 0 
		 //	[1,1024: $M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj][0] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 8 
		 //	[1,1024: $M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj][2] = [2,92: $M.CVC_SEND_CAP.headset_data_2mic.D1] + 0 
		 //	[2,77: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1][3] = [2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj] + 0 
		 //	[2,77: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1][4] = [2,137: $M.CVC_SEND_CAP.headset_data_2mic.X] + 0 
		 //	[2,84: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd][3] = [2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj] + 0 
		 //	[2,84: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd][4] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][3] = [2,91: $M.CVC_SEND_CAP.headset_data_2mic.BExp_D1] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][7] = [2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][8] = [2,92: $M.CVC_SEND_CAP.headset_data_2mic.D1] + 0 
		 //	[2,101: $M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd][0] = [1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd] + 0 
		 //	[2,101: $M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd][1] = [1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd] + 0 
		 //	[2,101: $M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd][2] = [1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd] + 0 
		 //	[2,101: $M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd][3] = [1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd] + 0 
		 //	[2,101: $M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd][4] = [1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd] + 0 
		 //	[2,101: $M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd][5] = [1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd] + 0 
		 //	[2,101: $M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd][6] = [1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd] + 0 
		 //	[2,101: $M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd][7] = [1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd] + 0 
		 //	[2,101: $M.CVC_SEND_CAP.headset_data_2mic.ModeProcTableSnd][8] = [1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd] + 0 
		 //	[2,128: $M.CVC_SEND_CAP.headset_data_2mic.dms_obj][0] = [2,92: $M.CVC_SEND_CAP.headset_data_2mic.D1] + 0 
		 //	[2,128: $M.CVC_SEND_CAP.headset_data_2mic.dms_obj][1] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 8 
		 //	[2,128: $M.CVC_SEND_CAP.headset_data_2mic.dms_obj][3] = [1,841: $M.CVC_SEND_CAP.headset_data_2mic.root] + 18 
		 //	[2,136: $M.CVC_SEND_CAP.headset_data_2mic.block?0][3] = [2,136: $M.CVC_SEND_CAP.headset_data_2mic.BExp_X] + 0 
		 //	[2,136: $M.CVC_SEND_CAP.headset_data_2mic.block?0][7] = [2,140: $M.CVC_SEND_CAP.headset_data_2mic.BExp_D0] + 0 
		 //	[2,144: $M.CVC_SEND_CAP.headset_data_2mic.snd_streams][0] = [1,988: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout] + 0 
		 //	[2,144: $M.CVC_SEND_CAP.headset_data_2mic.snd_streams][1] = [1,975: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_refin] + 0 
		 //	[2,144: $M.CVC_SEND_CAP.headset_data_2mic.snd_streams][2] = [1,962: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_left_adc] + 0 
		 //	[2,144: $M.CVC_SEND_CAP.headset_data_2mic.snd_streams][3] = [1,949: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_right_adc] + 0 
		 //	[2,144: $M.CVC_SEND_CAP.headset_data_2mic.snd_streams][4] = [1,936: $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout_va1] + 0 
		 //	[2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left][3] = [2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj] + 0 
		 //	[2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left][4] = [2,141: $M.CVC_SEND_CAP.headset_data_2mic.D0] + 0 
		 //	[2,156: $M.CVC_SEND_CAP.headset_data_2mic.ref_delay_stream][0] = [3,0: $M.CVC_SEND_CAP.headset_data_2mic.ref_delay_buffer] + 0 
		 //	[2,156: $M.CVC_SEND_CAP.headset_data_2mic.ref_delay_stream][3] = [3,0: $M.CVC_SEND_CAP.headset_data_2mic.ref_delay_buffer] + 0 
		 //	[2,170: $M.CVC_SEND_CAP.headset_data_2mic.harm_inputs][0] = [2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left] + 0 
		 //	[2,170: $M.CVC_SEND_CAP.headset_data_2mic.harm_inputs][1] = [2,95: $M.CVC_SEND_CAP.headset_data_2mic.fba_right] + 0 
		 $DYN_SECTION_TYPE_RELOC_INST,0xFFFF,555,
		 0x00B8,0x0000,0x0000,0x0001,0x0002,0x03CF,0x0001,0x0003,
		 0x03C2,0x0001,0x0004,0x03B5,0x0001,0x0007,0x03DC,0x0001,
		 0x0008,0x03A8,0x0002,0x0016,0x0065,0x0001,0x0017,0x01B9,
		 0x0001,0x0018,0x0349,0x0001,0x0019,0x035F,0x0002,0x002D,
		 0x008D,0x0001,0x003F,0x0337,0x0102,0x0000,0x008D,0x0102,
		 0x0001,0x005C,0x0101,0x0008,0x035B,0x0102,0x000E,0x0000,
		 0x0102,0x000F,0x0003,0x0102,0x0010,0x0006,0x0102,0x0011,
		 0x0009,0x0102,0x0012,0x000C,0x0102,0x0013,0x000F,0x0102,
		 0x0014,0x0012,0x0102,0x0015,0x0015,0x0101,0x0016,0x0355,
		 0x0102,0x007D,0x009C,0x0102,0x007F,0x0077,0x0102,0x0080,
		 0x0089,0x0102,0x008B,0x009C,0x0102,0x008C,0x0089,0x0102,
		 0x0091,0x008D,0x0101,0x0095,0x0400,0x0102,0x00A6,0x00A7,
		 0x0102,0x00A7,0x00A4,0x0102,0x0101,0x008D,0x0102,0x0102,
		 0x008D,0x0102,0x0110,0x0080,0x0102,0x015F,0x0090,0x0101,
		 0x0165,0x03CF,0x0101,0x0168,0x03C2,0x0101,0x016B,0x03B5,
		 0x0102,0x016E,0x003E,0x0102,0x0171,0x002F,0x0101,0x0178,
		 0x007C,0x0101,0x017B,0x0386,0x0101,0x017D,0x0285,0x0101,
		 0x017E,0x038F,0x0101,0x0181,0x02AE,0x0102,0x0184,0x0096,
		 0x0102,0x0187,0x005F,0x0101,0x018A,0x032E,0x0101,0x018D,
		 0x032E,0x0102,0x018F,0x00AA,0x0101,0x0190,0x02AE,0x0101,
		 0x0193,0x024F,0x0101,0x0196,0x0082,0x0101,0x0199,0x0211,
		 0x0101,0x019C,0x0000,0x0101,0x019F,0x0211,0x0101,0x01A2,
		 0x0211,0x0101,0x01A5,0x0101,0x0101,0x01A8,0x0101,0x0101,
		 0x01AB,0x0211,0x0101,0x01AE,0x0066,0x0101,0x01B1,0x02D4,
		 0x0101,0x01B4,0x03F6,0x0102,0x01B6,0x0090,0x0102,0x01BD,
		 0x003E,0x0102,0x01C0,0x002F,0x0102,0x01C7,0x0054,0x0102,
		 0x01CA,0x004D,0x0102,0x01CD,0x0096,0x0102,0x01D0,0x005F,
		 0x0101,0x01D3,0x02AE,0x0101,0x01D6,0x032E,0x0101,0x01D9,
		 0x032E,0x0101,0x01DC,0x024F,0x0101,0x01DF,0x0066,0x0101,
		 0x01E2,0x0386,0x0101,0x01E5,0x007C,0x0101,0x01E8,0x0082,
		 0x0101,0x01EB,0x0285,0x0101,0x01EE,0x0211,0x0101,0x01F1,
		 0x0000,0x0101,0x01F4,0x0101,0x0101,0x01F6,0x0397,0x0101,
		 0x01F7,0x0101,0x0102,0x01F9,0x0089,0x0101,0x01FA,0x0101,
		 0x0101,0x01FD,0x03E9,0x0102,0x01FF,0x0018,0x0101,0x0203,
		 0x0312,0x0102,0x0211,0x008D,0x0102,0x0212,0x005C,0x0101,
		 0x0213,0x0205,0x0101,0x0214,0x0208,0x0101,0x0215,0x020B,
		 0x0101,0x0216,0x020E,0x0101,0x021D,0x0358,0x0101,0x0220,
		 0x0108,0x0101,0x0221,0x0256,0x0102,0x024F,0x008D,0x0102,
		 0x0250,0x008D,0x0101,0x0285,0x0082,0x0101,0x0287,0x034F,
		 0x0101,0x0288,0x038F,0x0101,0x02D6,0x0211,0x0101,0x02D9,
		 0x03E9,0x0101,0x02DC,0x0285,0x0101,0x02DF,0x0082,0x0101,
		 0x02E2,0x0211,0x0102,0x02E5,0x0054,0x0102,0x02E7,0x0018,
		 0x0101,0x02EE,0x0312,0x0101,0x02F4,0x03DC,0x0102,0x02F7,
		 0x0090,0x0101,0x02FA,0x03CF,0x0101,0x02FD,0x03C2,0x0101,
		 0x0300,0x03B5,0x0101,0x030A,0x03DC,0x0101,0x030D,0x03A8,
		 0x0102,0x030F,0x0090,0x0102,0x032E,0x008D,0x0102,0x032F,
		 0x005C,0x0101,0x0332,0x0351,0x0101,0x0334,0x026E,0x0101,
		 0x034C,0x02AE,0x0101,0x034D,0x0101,0x0101,0x035C,0x0211,
		 0x0101,0x035F,0x0349,0x0101,0x0363,0x006C,0x0101,0x0364,
		 0x006B,0x0101,0x0365,0x0319,0x0101,0x0366,0x0321,0x0101,
		 0x0367,0x00D6,0x0101,0x0368,0x0046,0x0101,0x0369,0x0047,
		 0x0101,0x036A,0x0355,0x0101,0x036B,0x034F,0x0101,0x036C,
		 0x0337,0x0101,0x0370,0x034F,0x0101,0x0371,0x02A4,0x0102,
		 0x0373,0x009C,0x0102,0x0374,0x00A0,0x0102,0x0386,0x009C,
		 0x0102,0x0387,0x006E,0x0101,0x0398,0x034F,0x0101,0x0399,
		 0x0357,0x0101,0x039A,0x0355,0x0101,0x03E9,0x0357,0x0102,
		 0x03F8,0x004D,0x0101,0x03FE,0x03A8,0x0101,0x0400,0x0351,
		 0x0102,0x0402,0x005C,0x0202,0x0050,0x0077,0x0202,0x0051,
		 0x0089,0x0202,0x0057,0x0077,0x0202,0x0058,0x008D,0x0202,
		 0x005E,0x005B,0x0202,0x0062,0x0077,0x0202,0x0063,0x005C,
		 0x0201,0x0065,0x02F6,0x0201,0x0066,0x02F6,0x0201,0x0067,
		 0x015E,0x0201,0x0068,0x015E,0x0201,0x0069,0x02F6,0x0201,
		 0x006A,0x02F6,0x0201,0x006B,0x02F6,0x0201,0x006C,0x02F6,
		 0x0201,0x006D,0x02F6,0x0202,0x0080,0x005C,0x0201,0x0081,
		 0x0351,0x0201,0x0083,0x035B,0x0202,0x008B,0x0088,0x0202,
		 0x008F,0x008C,0x0201,0x0090,0x03DC,0x0201,0x0091,0x03CF,
		 0x0201,0x0092,0x03C2,0x0201,0x0093,0x03B5,0x0201,0x0094,
		 0x03A8,0x0202,0x0099,0x0077,0x0202,0x009A,0x008D,0x0203,
		 0x009C,0x0000,0x0203,0x009F,0x0000,0x0202,0x00AA,0x0096,
		 0x0202,0x00AB,0x005F,
		 // Variant 0xFFFF: 12 Links from Root to Resolve
		 //	Root[27] = &$M.CVC_SEND_CAP.headset_data_2mic.cvclib_table, 3 references to resolve
		 //		[1,257] $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj[5] = ROOT[27] 
		 //		[1,517] $M.CVC_SEND_CAP.headset_data_2mic.block?3[21] = ROOT[27] 
		 //		[1,591] $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj[5] = ROOT[27] 
		 //	Root[28] = &$M.CVC_SEND_CAP.headset_data_2mic.fft_split, 1 references to resolve
		 //		[2,119] $M.CVC_SEND_CAP.headset_data_2mic.fft_obj[4] = ROOT[28] 
		 //	Root[29] = &$M.CVC_SEND_CAP.headset_data_2mic.oms_mode_object, 1 references to resolve
		 //		[1,591] $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj[3] = ROOT[29] 
		 //	Root[30] = &$M.CVC_SEND_CAP.headset_data_2mic.dms_mode_object, 1 references to resolve
		 //		[1,257] $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj[3] = ROOT[30] 
		 //	Root[31] = &$M.CVC_SEND_CAP.headset_data_2mic.asf_mode_table, 1 references to resolve
		 //		[1,0] $M.CVC_SEND_CAP.headset_data_2mic.asf_object[2] = ROOT[31] 
		 //	Root[32] = &$M.CVC_SEND_CAP.headset_data_2mic.vad_dc_coeffs, 3 references to resolve
		 //		[1,883] $M.CVC_SEND_CAP.headset_data_2mic.ref_vad_peq[3] = ROOT[32] + 10 
		 //		[2,47] $M.CVC_SEND_CAP.headset_data_2mic.in_r_dcblock_dm2[3] = ROOT[32] 
		 //		[2,62] $M.CVC_SEND_CAP.headset_data_2mic.in_l_dcblock_dm2[3] = ROOT[32] 
		 //	Root[33] = &$M.CVC_SEND_CAP.headset_data_2mic.aec_mode_object, 1 references to resolve
		 //		[1,130] $M.CVC_SEND_CAP.headset_data_2mic.aec_obj[0] = ROOT[33] 
		 //	Root[34] = &$M.CVC_SEND_CAP.headset_data_2mic.fb_configuration, 5 references to resolve
		 //		[1,124] $M.CVC_SEND_CAP.headset_data_2mic.fba_ref[0] = ROOT[34] 
		 //		[2,77] $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1[0] = ROOT[34] 
		 //		[2,84] $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd[0] = ROOT[34] 
		 //		[2,91] $M.CVC_SEND_CAP.headset_data_2mic.block?1[4] = ROOT[34] 
		 //		[2,150] $M.CVC_SEND_CAP.headset_data_2mic.fba_left[0] = ROOT[34] 
		 //	Root[37] = &$M.CVC_SEND_CAP.headset_data_2mic.mgdc_state_ptr, 2 references to resolve
		 //		[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[46] = ROOT[37] 
		 //		[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[31] = ROOT[37] 
		 //	Root[40] = &$M.CVC_SEND_CAP.headset_data_2mic.cap_root_ptr, 14 references to resolve
		 //		[1,0] $M.CVC_SEND_CAP.headset_data_2mic.asf_object[3] = ROOT[40] + 47 
		 //		[1,102] $M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj[2] = ROOT[40] + 47 
		 //		[1,130] $M.CVC_SEND_CAP.headset_data_2mic.aec_obj[2] = ROOT[40] + 47 
		 //		[1,257] $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj[2] = ROOT[40] + 47 
		 //		[1,517] $M.CVC_SEND_CAP.headset_data_2mic.block?3[31] = ROOT[40] + 47 
		 //		[1,591] $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj[2] = ROOT[40] + 47 
		 //		[1,686] $M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj[1] = ROOT[40] + 47 
		 //		[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[13] = ROOT[40] + 12 
		 //		[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[16] = ROOT[40] + 12 
		 //		[1,786] $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm[4] = ROOT[40] + 47 
		 //		[1,814] $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj[3] = ROOT[40] + 47 
		 //		[1,841] $M.CVC_SEND_CAP.headset_data_2mic.root[1] = ROOT[40] 
		 //		[1,902] $M.CVC_SEND_CAP.headset_data_2mic.ref_vad410[2] = ROOT[40] + 47 
		 //		[1,1001] $M.CVC_SEND_CAP.headset_data_2mic.aed100_obj[1] = ROOT[40] + 47 
		 //	Root[54] = &$M.CVC_SEND_CAP.headset_data_2mic.mute_control_ptr, 1 references to resolve
		 //		[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[28] = ROOT[54] 
		 //	Root[58] = &$M.CVC_SEND_CAP.headset_data_2mic.ext_frame_size, 5 references to resolve
		 //		[1,936] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout_va1[5] = ROOT[58] 
		 //		[1,949] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_right_adc[5] = ROOT[58] 
		 //		[1,962] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_left_adc[5] = ROOT[58] 
		 //		[1,975] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_refin[5] = ROOT[58] 
		 //		[1,988] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout[5] = ROOT[58] 
		 $DYN_SECTION_TYPE_RELOC_ROOT,0xFFFF,138,
		 0x001B,0x0003,0x0001,0x0106,0x0000,0x0001,0x021A,0x0000,
		 0x0001,0x0254,0x0000,0x001C,0x0001,0x0002,0x007B,0x0000,
		 0x001D,0x0001,0x0001,0x0252,0x0000,0x001E,0x0001,0x0001,
		 0x0104,0x0000,0x001F,0x0001,0x0001,0x0002,0x0000,0x0020,
		 0x0003,0x0001,0x0376,0x000A,0x0002,0x0032,0x0000,0x0002,
		 0x0041,0x0000,0x0021,0x0001,0x0001,0x0082,0x0000,0x0022,
		 0x0005,0x0001,0x007C,0x0000,0x0002,0x004D,0x0000,0x0002,
		 0x0054,0x0000,0x0002,0x005F,0x0000,0x0002,0x0096,0x0000,
		 0x0025,0x0002,0x0001,0x018C,0x0000,0x0001,0x01D8,0x0000,
		 0x0028,0x000E,0x0001,0x0003,0x002F,0x0001,0x0068,0x002F,
		 0x0001,0x0084,0x002F,0x0001,0x0103,0x002F,0x0001,0x0224,
		 0x002F,0x0001,0x0251,0x002F,0x0001,0x02AF,0x002F,0x0001,
		 0x0303,0x000C,0x0001,0x0306,0x000C,0x0001,0x0316,0x002F,
		 0x0001,0x0331,0x002F,0x0001,0x034A,0x0000,0x0001,0x0388,
		 0x002F,0x0001,0x03EA,0x002F,0x0036,0x0001,0x0001,0x02F0,
		 0x0000,0x003A,0x0005,0x0001,0x03AD,0x0000,0x0001,0x03BA,
		 0x0000,0x0001,0x03C7,0x0000,0x0001,0x03D4,0x0000,0x0001,
		 0x03E1,0x0000,
		 // Variant 0x0000: 134 Internal Links to Resolve
		 //	Root[12] = [3,978: $M.CVC_SEND_CAP.headset_data_2mic.cvc_send_internal_streams] + 0 
		 //	Root[26] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][4] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 67 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][5] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 131 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][23] = [6,126: $M.CVC_SEND_CAP.headset_data_2mic.pX0] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][24] = [6,0: $M.CVC_SEND_CAP.headset_data_2mic.pX1] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][25] = [5,830: $M.CVC_SEND_CAP.headset_data_2mic.pXcR] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][26] = [5,704: $M.CVC_SEND_CAP.headset_data_2mic.pXcI] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][27] = [5,578: $M.CVC_SEND_CAP.headset_data_2mic.coh] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][28] = [5,452: $M.CVC_SEND_CAP.headset_data_2mic.wnr_g] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][29] = [8,756: $M.CVC_SEND_CAP.headset_data_2mic.beam0_cc_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][30] = [7,883: $M.CVC_SEND_CAP.headset_data_2mic.beam0_cc_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][31] = [8,693: $M.CVC_SEND_CAP.headset_data_2mic.beam1_cc_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][32] = [7,820: $M.CVC_SEND_CAP.headset_data_2mic.beam1_cc_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][33] = [8,630: $M.CVC_SEND_CAP.headset_data_2mic.beam0_phi_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][34] = [7,757: $M.CVC_SEND_CAP.headset_data_2mic.beam0_phi_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][35] = [8,441: $M.CVC_SEND_CAP.headset_data_2mic.beam_w0_aux_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][36] = [7,568: $M.CVC_SEND_CAP.headset_data_2mic.beam_w0_aux_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][37] = [8,378: $M.CVC_SEND_CAP.headset_data_2mic.beam_w1_aux_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][38] = [7,505: $M.CVC_SEND_CAP.headset_data_2mic.beam_w1_aux_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][39] = [8,567: $M.CVC_SEND_CAP.headset_data_2mic.beam1_phi_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][40] = [7,694: $M.CVC_SEND_CAP.headset_data_2mic.beam1_phi_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][41] = [8,504: $M.CVC_SEND_CAP.headset_data_2mic.beam0_w1_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][42] = [7,631: $M.CVC_SEND_CAP.headset_data_2mic.beam0_w1_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][43] = [8,315: $M.CVC_SEND_CAP.headset_data_2mic.beam1_w0_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][44] = [7,442: $M.CVC_SEND_CAP.headset_data_2mic.beam1_w0_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][45] = [8,252: $M.CVC_SEND_CAP.headset_data_2mic.beam1_w1_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][46] = [7,379: $M.CVC_SEND_CAP.headset_data_2mic.beam1_w1_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][47] = [4,440: $M.CVC_SEND_CAP.headset_data_2mic.beam0_tr] + 1 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][48] = [4,505: $M.CVC_SEND_CAP.headset_data_2mic.beam1_tr] + 1 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][49] = [8,189: $M.CVC_SEND_CAP.headset_data_2mic.coh_cos] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][50] = [7,316: $M.CVC_SEND_CAP.headset_data_2mic.coh_sin] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][51] = [6,681: $M.CVC_SEND_CAP.headset_data_2mic.spp] + 1 
		 //	[1,102: $M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj][4] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 2 
		 //	[1,124: $M.CVC_SEND_CAP.headset_data_2mic.fba_ref][2] = [2,730: $M.CVC_SEND_CAP.headset_data_2mic.bufd_aec_inp] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][1] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 90 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][8] = [3,978: $M.CVC_SEND_CAP.headset_data_2mic.stream_x] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][11] = [3,668: $M.CVC_SEND_CAP.headset_data_2mic.RcvBuf_real] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][12] = [2,600: $M.CVC_SEND_CAP.headset_data_2mic.RcvBuf_imag] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][13] = [2,1015: $M.CVC_SEND_CAP.headset_data_2mic.BExp_X_buf] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][16] = [2,470: $M.CVC_SEND_CAP.headset_data_2mic.Ga_real] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][17] = [3,538: $M.CVC_SEND_CAP.headset_data_2mic.Ga_imag] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][18] = [6,356: $M.CVC_SEND_CAP.headset_data_2mic.BExp_Ga] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][20] = [6,616: $M.CVC_SEND_CAP.headset_data_2mic.LPwrX0] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][21] = [6,486: $M.CVC_SEND_CAP.headset_data_2mic.LPwrX1] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][22] = [7,0: $M.CVC_SEND_CAP.headset_data_2mic.RatFE] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][23] = [6,938: $M.CVC_SEND_CAP.headset_data_2mic.Gr_imag] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][24] = [6,874: $M.CVC_SEND_CAP.headset_data_2mic.Gr_real] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][25] = [6,746: $M.CVC_SEND_CAP.headset_data_2mic.SqGr] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][26] = [4,120: $M.CVC_SEND_CAP.headset_data_2mic.L2absGr] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][27] = [6,810: $M.CVC_SEND_CAP.headset_data_2mic.LPwrD] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][28] = [6,551: $M.CVC_SEND_CAP.headset_data_2mic.LpZ_nz] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][29] = [6,421: $M.CVC_SEND_CAP.headset_data_2mic.Cng_Nz_Shape_Tab] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][35] = [8,947: $M.CVC_SEND_CAP.headset_data_2mic.DTC_lin] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][4] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 79 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][6] = [5,180: $M.CVC_SEND_CAP.headset_data_2mic.sndLpX_queue] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][9] = [4,184: $M.CVC_SEND_CAP.headset_data_2mic.dms_state] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][17] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 78 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][8] = [3,978: $M.CVC_SEND_CAP.headset_data_2mic.stream_x] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][11] = [3,983: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][14] = [3,988: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][20] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 88 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][23] = [4,570: $M.CVC_SEND_CAP.headset_data_2mic.rnr_data] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][25] = [3,240: $M.CVC_SEND_CAP.headset_data_2mic.TP_data_dm1] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][26] = [2,172: $M.CVC_SEND_CAP.headset_data_2mic.TP_data_dm2] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][29] = [4,440: $M.CVC_SEND_CAP.headset_data_2mic.beam0_tr] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][30] = [4,505: $M.CVC_SEND_CAP.headset_data_2mic.beam1_tr] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][6] = [5,316: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_LpX_queue] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][9] = [6,252: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_state] + 0 
		 //	[1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp][1] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 108 
		 //	[1,686: $M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj][0] = [3,983: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[1,686: $M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj][2] = [3,798: $M.CVC_SEND_CAP.headset_data_2mic.harm_history] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][22] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][23] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][29] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][31] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][5] = [3,978: $M.CVC_SEND_CAP.headset_data_2mic.stream_x] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][8] = [3,983: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][11] = [3,988: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][14] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][17] = [3,1008: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][19] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][22] = [3,1008: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm][0] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 49 
		 //	[1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm][1] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm][2] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm][3] = [2,1010: $M.CVC_SEND_CAP.headset_data_2mic.vad_agc_obj] + 0 
		 //	[1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj][2] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 76 
		 //	[1,841: $M.CVC_SEND_CAP.headset_data_2mic.root][2] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 0 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][1] = [3,983: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][2] = [3,988: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][3] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][14] = [3,993: $M.CVC_SEND_CAP.headset_data_2mic.stream_d2] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][15] = [3,998: $M.CVC_SEND_CAP.headset_data_2mic.stream_d3] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][16] = [3,978: $M.CVC_SEND_CAP.headset_data_2mic.stream_x] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][19] = [3,1008: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 4 
		 //	[1,902: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad410][3] = [1,883: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad_peq] + 0 
		 //	[1,919: $M.CVC_SEND_CAP.headset_data_2mic.oms_wnr_obj][0] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 71 
		 //	[1,1014: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va][4] = [3,1008: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[1,1014: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va][5] = [3,1008: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[1,1014: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va][7] = [3,1008: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[1,1024: $M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj][3] = [2,340: $M.CVC_SEND_CAP.headset_data_2mic.GaMic1_real] + 0 
		 //	[1,1024: $M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj][4] = [3,408: $M.CVC_SEND_CAP.headset_data_2mic.GaMic1_imag] + 0 
		 //	[1,1024: $M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj][5] = [5,956: $M.CVC_SEND_CAP.headset_data_2mic.BExp_GaMic1] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][12] = [8,126: $M.CVC_SEND_CAP.headset_data_2mic.x0_saved_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][13] = [7,253: $M.CVC_SEND_CAP.headset_data_2mic.x0_saved_imag] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][15] = [8,63: $M.CVC_SEND_CAP.headset_data_2mic.x1_saved_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][16] = [7,190: $M.CVC_SEND_CAP.headset_data_2mic.x1_saved_imag] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][18] = [8,0: $M.CVC_SEND_CAP.headset_data_2mic.z0_aux_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][19] = [7,127: $M.CVC_SEND_CAP.headset_data_2mic.z0_aux_imag] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][21] = [7,946: $M.CVC_SEND_CAP.headset_data_2mic.z1_aux_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][22] = [7,64: $M.CVC_SEND_CAP.headset_data_2mic.z1_aux_imag] + 0 
		 //	[2,24: $M.CVC_SEND_CAP.headset_data_2mic.snd_peq_dm2][0] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[2,24: $M.CVC_SEND_CAP.headset_data_2mic.snd_peq_dm2][1] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[2,24: $M.CVC_SEND_CAP.headset_data_2mic.snd_peq_dm2][3] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 10 
		 //	[2,47: $M.CVC_SEND_CAP.headset_data_2mic.in_r_dcblock_dm2][0] = [3,988: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 0 
		 //	[2,47: $M.CVC_SEND_CAP.headset_data_2mic.in_r_dcblock_dm2][1] = [3,988: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 0 
		 //	[2,62: $M.CVC_SEND_CAP.headset_data_2mic.in_l_dcblock_dm2][0] = [3,983: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[2,62: $M.CVC_SEND_CAP.headset_data_2mic.in_l_dcblock_dm2][1] = [3,983: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[2,77: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1][1] = [3,1008: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[2,77: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1][2] = [4,675: $M.CVC_SEND_CAP.headset_data_2mic.bufd_outp_va1] + 0 
		 //	[2,84: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd][1] = [3,1003: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[2,84: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd][2] = [5,0: $M.CVC_SEND_CAP.headset_data_2mic.bufd_outp] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][5] = [3,988: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][6] = [2,850: $M.CVC_SEND_CAP.headset_data_2mic.bufd_r_inp] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][9] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 137 
		 //	[2,128: $M.CVC_SEND_CAP.headset_data_2mic.dms_obj][2] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 69 
		 //	[2,128: $M.CVC_SEND_CAP.headset_data_2mic.dms_obj][4] = [2,970: $M.CVC_SEND_CAP.headset_data_2mic.dms_Ath] + 0 
		 //	[2,128: $M.CVC_SEND_CAP.headset_data_2mic.dms_obj][5] = [6,681: $M.CVC_SEND_CAP.headset_data_2mic.spp] + 0 
		 //	[2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left][1] = [3,983: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left][2] = [4,0: $M.CVC_SEND_CAP.headset_data_2mic.bufd_l_inp] + 0 
		 //	[2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left][5] = [4,855: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 137 
		 //	[2,164: $M.CVC_SEND_CAP.headset_data_2mic.AEC_Et][0] = [8,819: $M.CVC_SEND_CAP.headset_data_2mic.Et_real] + 0 
		 //	[2,167: $M.CVC_SEND_CAP.headset_data_2mic.AEC_Dt][0] = [8,883: $M.CVC_SEND_CAP.headset_data_2mic.Dt_real] + 0 
		 $DYN_SECTION_TYPE_RELOC_INST,0x0000,405,
		 0x0086,0x0000,0x0000,0x0003,0x000C,0x03D2,0x0004,0x001A,
		 0x0357,0x0104,0x0004,0x039A,0x0104,0x0005,0x03DA,0x0106,
		 0x0017,0x007E,0x0106,0x0018,0x0000,0x0105,0x0019,0x033E,
		 0x0105,0x001A,0x02C0,0x0105,0x001B,0x0242,0x0105,0x001C,
		 0x01C4,0x0108,0x001D,0x02F4,0x0107,0x001E,0x0373,0x0108,
		 0x001F,0x02B5,0x0107,0x0020,0x0334,0x0108,0x0021,0x0276,
		 0x0107,0x0022,0x02F5,0x0108,0x0023,0x01B9,0x0107,0x0024,
		 0x0238,0x0108,0x0025,0x017A,0x0107,0x0026,0x01F9,0x0108,
		 0x0027,0x0237,0x0107,0x0028,0x02B6,0x0108,0x0029,0x01F8,
		 0x0107,0x002A,0x0277,0x0108,0x002B,0x013B,0x0107,0x002C,
		 0x01BA,0x0108,0x002D,0x00FC,0x0107,0x002E,0x017B,0x0104,
		 0x002F,0x01B9,0x0104,0x0030,0x01FA,0x0108,0x0031,0x00BD,
		 0x0107,0x0032,0x013C,0x0106,0x0033,0x02AA,0x0104,0x006A,
		 0x0359,0x0102,0x007E,0x02DA,0x0104,0x0083,0x03B1,0x0103,
		 0x008A,0x03D2,0x0103,0x008D,0x029C,0x0102,0x008E,0x0258,
		 0x0102,0x008F,0x03F7,0x0102,0x0092,0x01D6,0x0103,0x0093,
		 0x021A,0x0106,0x0094,0x0164,0x0106,0x0096,0x0268,0x0106,
		 0x0097,0x01E6,0x0107,0x0098,0x0000,0x0106,0x0099,0x03AA,
		 0x0106,0x009A,0x036A,0x0106,0x009B,0x02EA,0x0104,0x009C,
		 0x0078,0x0106,0x009D,0x032A,0x0106,0x009E,0x0227,0x0106,
		 0x009F,0x01A5,0x0108,0x00A5,0x03B3,0x0104,0x0105,0x03A6,
		 0x0105,0x0107,0x00B4,0x0104,0x010A,0x00B8,0x0104,0x0112,
		 0x03A5,0x0103,0x0166,0x03D2,0x0103,0x0169,0x03D7,0x0103,
		 0x016C,0x03DC,0x0104,0x0219,0x03AF,0x0104,0x021C,0x023A,
		 0x0103,0x021E,0x00F0,0x0102,0x021F,0x00AC,0x0104,0x0222,
		 0x01B8,0x0104,0x0223,0x01F9,0x0105,0x0255,0x013C,0x0106,
		 0x0258,0x00FC,0x0104,0x0286,0x03C3,0x0103,0x02AE,0x03D7,
		 0x0103,0x02B0,0x031E,0x0103,0x02EA,0x03EB,0x0103,0x02EB,
		 0x03EB,0x0103,0x02F1,0x03EB,0x0103,0x02F3,0x03EB,0x0103,
		 0x02FB,0x03D2,0x0103,0x02FE,0x03D7,0x0103,0x0301,0x03DC,
		 0x0103,0x0304,0x03EB,0x0103,0x0307,0x03F0,0x0103,0x0309,
		 0x03EB,0x0103,0x030C,0x03F0,0x0104,0x0312,0x0388,0x0103,
		 0x0313,0x03EB,0x0103,0x0314,0x03EB,0x0102,0x0315,0x03F2,
		 0x0104,0x0330,0x03A3,0x0104,0x034B,0x0357,0x0103,0x0360,
		 0x03DB,0x0103,0x0361,0x03E0,0x0103,0x0362,0x03EF,0x0103,
		 0x036D,0x03E5,0x0103,0x036E,0x03EA,0x0103,0x036F,0x03D6,
		 0x0103,0x0372,0x03F4,0x0101,0x0389,0x0373,0x0104,0x0397,
		 0x039E,0x0103,0x03FA,0x03F0,0x0103,0x03FB,0x03F0,0x0103,
		 0x03FD,0x03F0,0x0102,0x0403,0x0154,0x0103,0x0404,0x0198,
		 0x0105,0x0405,0x03BC,0x0208,0x000C,0x007E,0x0207,0x000D,
		 0x00FD,0x0208,0x000F,0x003F,0x0207,0x0010,0x00BE,0x0208,
		 0x0012,0x0000,0x0207,0x0013,0x007F,0x0207,0x0015,0x03B2,
		 0x0207,0x0016,0x0040,0x0203,0x0018,0x03EB,0x0203,0x0019,
		 0x03EB,0x0204,0x001B,0x0361,0x0203,0x002F,0x03DC,0x0203,
		 0x0030,0x03DC,0x0203,0x003E,0x03D7,0x0203,0x003F,0x03D7,
		 0x0203,0x004E,0x03F0,0x0204,0x004F,0x02A3,0x0203,0x0055,
		 0x03EB,0x0205,0x0056,0x0000,0x0203,0x0060,0x03DC,0x0202,
		 0x0061,0x0352,0x0204,0x0064,0x03E0,0x0204,0x0082,0x039C,
		 0x0202,0x0084,0x03CA,0x0206,0x0085,0x02A9,0x0203,0x0097,
		 0x03D7,0x0204,0x0098,0x0000,0x0204,0x009B,0x03E0,0x0208,
		 0x00A4,0x0333,0x0208,0x00A7,0x0373,
		 // Variant 0x0000: 40 Internal Links to Resolve (Scratch)
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][9] = [1,0: $dm1_scratch] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][10] = [3,0: $dm2_scratch] + 0 
		 //	[1,102: $M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj][3] = [1,890: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_LpXnz] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][6] = [3,825: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_G] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][7] = [1,890: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_LpXnz] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][30] = [1,131: $M.CVC_SEND_CAP.headset_data_2mic.L_adaptA] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][31] = [3,0: $M.CVC_SEND_CAP.headset_data_2mic.Exp_Mts_adapt] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][32] = [1,825: $M.CVC_SEND_CAP.headset_data_2mic.AttenuationPersist] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][33] = [1,0: $M.CVC_SEND_CAP.headset_data_2mic.W_ri] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][34] = [1,196: $M.CVC_SEND_CAP.headset_data_2mic.L_adaptR] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][7] = [3,890: $M.CVC_SEND_CAP.headset_data_2mic.G_dmsZ] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][10] = [1,0: $dm1_scratch] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][0] = [2,0: $M.CVC_SEND_CAP.headset_data_2mic.PIZ0r] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][1] = [4,0: $M.CVC_SEND_CAP.headset_data_2mic.PIZ0i] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][3] = [2,65: $M.CVC_SEND_CAP.headset_data_2mic.PIZ1r] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][4] = [4,65: $M.CVC_SEND_CAP.headset_data_2mic.PIZ1i] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][6] = [2,130: $M.CVC_SEND_CAP.headset_data_2mic.POZ0r] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][7] = [4,130: $M.CVC_SEND_CAP.headset_data_2mic.POZ0i] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][9] = [2,195: $M.CVC_SEND_CAP.headset_data_2mic.POZ1r] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][10] = [4,195: $M.CVC_SEND_CAP.headset_data_2mic.POZ1i] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][18] = [1,0: $dm1_scratch] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][19] = [3,0: $dm2_scratch] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][22] = [1,825: $M.CVC_SEND_CAP.headset_data_2mic.AttenuationPersist] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][7] = [3,825: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_G] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][8] = [1,890: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_LpXnz] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][10] = [1,0: $dm1_scratch] + 0 
		 //	[1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp][4] = [1,825: $M.CVC_SEND_CAP.headset_data_2mic.AttenuationPersist] + 0 
		 //	[1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj][5] = [3,825: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_G] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][1] = [1,695: $M.CVC_SEND_CAP.headset_data_2mic.D_r_real] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][2] = [3,695: $M.CVC_SEND_CAP.headset_data_2mic.D_r_imag] + 0 
		 //	[2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj][1] = [1,0: $dm1_scratch] + 0 
		 //	[2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj][2] = [3,0: $dm2_scratch] + 0 
		 //	[2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj][3] = [1,0: $dm1_scratch] + 128 
		 //	[2,136: $M.CVC_SEND_CAP.headset_data_2mic.block?0][1] = [1,760: $M.CVC_SEND_CAP.headset_data_2mic.X_real] + 0 
		 //	[2,136: $M.CVC_SEND_CAP.headset_data_2mic.block?0][2] = [3,760: $M.CVC_SEND_CAP.headset_data_2mic.X_imag] + 0 
		 //	[2,136: $M.CVC_SEND_CAP.headset_data_2mic.block?0][5] = [1,630: $M.CVC_SEND_CAP.headset_data_2mic.D_l_real] + 0 
		 //	[2,136: $M.CVC_SEND_CAP.headset_data_2mic.block?0][6] = [3,630: $M.CVC_SEND_CAP.headset_data_2mic.D_l_imag] + 0 
		 //	[2,160: $M.CVC_SEND_CAP.headset_data_2mic.vad_peq_output][0] = [1,0: $dm1_scratch] + 0 
		 //	[2,164: $M.CVC_SEND_CAP.headset_data_2mic.AEC_Et][1] = [3,955: $M.CVC_SEND_CAP.headset_data_2mic.Et_imag] + 0 
		 //	[2,167: $M.CVC_SEND_CAP.headset_data_2mic.AEC_Dt][1] = [4,260: $M.CVC_SEND_CAP.headset_data_2mic.Dt_imag] + 0 
		 $DYN_SECTION_TYPE_RELOC_SCRATCH,0x0000,123,
		 0x0028,0x0000,0x0001,0x0101,0x0009,0x0000,0x0103,0x000A,
		 0x0000,0x0101,0x0069,0x037A,0x0103,0x0088,0x0339,0x0101,
		 0x0089,0x037A,0x0101,0x00A0,0x0083,0x0103,0x00A1,0x0000,
		 0x0101,0x00A2,0x0339,0x0101,0x00A3,0x0000,0x0101,0x00A4,
		 0x00C4,0x0103,0x0108,0x037A,0x0101,0x010B,0x0000,0x0102,
		 0x0205,0x0000,0x0104,0x0206,0x0000,0x0102,0x0208,0x0041,
		 0x0104,0x0209,0x0041,0x0102,0x020B,0x0082,0x0104,0x020C,
		 0x0082,0x0102,0x020E,0x00C3,0x0104,0x020F,0x00C3,0x0101,
		 0x0217,0x0000,0x0103,0x0218,0x0000,0x0101,0x021B,0x0339,
		 0x0103,0x0256,0x0339,0x0101,0x0257,0x037A,0x0101,0x0259,
		 0x0000,0x0101,0x0289,0x0339,0x0103,0x0333,0x0339,0x0201,
		 0x005C,0x02B7,0x0203,0x005D,0x02B7,0x0201,0x0078,0x0000,
		 0x0203,0x0079,0x0000,0x0201,0x007A,0x0080,0x0201,0x0089,
		 0x02F8,0x0203,0x008A,0x02F8,0x0201,0x008D,0x0276,0x0203,
		 0x008E,0x0276,0x0201,0x00A0,0x0000,0x0203,0x00A5,0x03BB,
		 0x0204,0x00A8,0x0104,
		 // Variant 0x0001: 134 Internal Links to Resolve
		 //	Root[12] = [3,996: $M.CVC_SEND_CAP.headset_data_2mic.cvc_send_internal_streams] + 0 
		 //	Root[26] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][4] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 67 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][5] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 131 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][23] = [9,756: $M.CVC_SEND_CAP.headset_data_2mic.pX0] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][24] = [9,630: $M.CVC_SEND_CAP.headset_data_2mic.pX1] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][25] = [9,504: $M.CVC_SEND_CAP.headset_data_2mic.pXcR] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][26] = [9,378: $M.CVC_SEND_CAP.headset_data_2mic.pXcI] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][27] = [9,252: $M.CVC_SEND_CAP.headset_data_2mic.coh] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][28] = [9,126: $M.CVC_SEND_CAP.headset_data_2mic.wnr_g] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][29] = [11,504: $M.CVC_SEND_CAP.headset_data_2mic.beam0_cc_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][30] = [10,887: $M.CVC_SEND_CAP.headset_data_2mic.beam0_cc_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][31] = [11,441: $M.CVC_SEND_CAP.headset_data_2mic.beam1_cc_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][32] = [10,824: $M.CVC_SEND_CAP.headset_data_2mic.beam1_cc_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][33] = [11,378: $M.CVC_SEND_CAP.headset_data_2mic.beam0_phi_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][34] = [10,761: $M.CVC_SEND_CAP.headset_data_2mic.beam0_phi_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][35] = [11,189: $M.CVC_SEND_CAP.headset_data_2mic.beam_w0_aux_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][36] = [10,572: $M.CVC_SEND_CAP.headset_data_2mic.beam_w0_aux_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][37] = [11,126: $M.CVC_SEND_CAP.headset_data_2mic.beam_w1_aux_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][38] = [10,509: $M.CVC_SEND_CAP.headset_data_2mic.beam_w1_aux_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][39] = [11,315: $M.CVC_SEND_CAP.headset_data_2mic.beam1_phi_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][40] = [10,698: $M.CVC_SEND_CAP.headset_data_2mic.beam1_phi_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][41] = [11,252: $M.CVC_SEND_CAP.headset_data_2mic.beam0_w1_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][42] = [10,635: $M.CVC_SEND_CAP.headset_data_2mic.beam0_w1_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][43] = [11,63: $M.CVC_SEND_CAP.headset_data_2mic.beam1_w0_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][44] = [10,446: $M.CVC_SEND_CAP.headset_data_2mic.beam1_w0_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][45] = [11,0: $M.CVC_SEND_CAP.headset_data_2mic.beam1_w1_real] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][46] = [10,383: $M.CVC_SEND_CAP.headset_data_2mic.beam1_w1_imag] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][47] = [4,786: $M.CVC_SEND_CAP.headset_data_2mic.beam0_tr] + 1 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][48] = [4,851: $M.CVC_SEND_CAP.headset_data_2mic.beam1_tr] + 1 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][49] = [10,950: $M.CVC_SEND_CAP.headset_data_2mic.coh_cos] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][50] = [10,320: $M.CVC_SEND_CAP.headset_data_2mic.coh_sin] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][51] = [7,951: $M.CVC_SEND_CAP.headset_data_2mic.spp] + 1 
		 //	[1,102: $M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj][4] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 2 
		 //	[1,124: $M.CVC_SEND_CAP.headset_data_2mic.fba_ref][2] = [5,0: $M.CVC_SEND_CAP.headset_data_2mic.bufd_aec_inp] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][1] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 90 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][8] = [3,996: $M.CVC_SEND_CAP.headset_data_2mic.stream_x] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][11] = [4,0: $M.CVC_SEND_CAP.headset_data_2mic.RcvBuf_real] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][12] = [2,688: $M.CVC_SEND_CAP.headset_data_2mic.RcvBuf_imag] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][13] = [2,1015: $M.CVC_SEND_CAP.headset_data_2mic.BExp_X_buf] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][16] = [2,430: $M.CVC_SEND_CAP.headset_data_2mic.Ga_real] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][17] = [3,738: $M.CVC_SEND_CAP.headset_data_2mic.Ga_imag] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][18] = [7,435: $M.CVC_SEND_CAP.headset_data_2mic.BExp_Ga] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][20] = [8,0: $M.CVC_SEND_CAP.headset_data_2mic.LPwrX0] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][21] = [7,693: $M.CVC_SEND_CAP.headset_data_2mic.LPwrX1] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][22] = [10,256: $M.CVC_SEND_CAP.headset_data_2mic.RatFE] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][23] = [10,192: $M.CVC_SEND_CAP.headset_data_2mic.Gr_imag] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][24] = [10,128: $M.CVC_SEND_CAP.headset_data_2mic.Gr_real] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][25] = [10,0: $M.CVC_SEND_CAP.headset_data_2mic.SqGr] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][26] = [2,946: $M.CVC_SEND_CAP.headset_data_2mic.L2absGr] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][27] = [10,64: $M.CVC_SEND_CAP.headset_data_2mic.LPwrD] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][28] = [7,822: $M.CVC_SEND_CAP.headset_data_2mic.LpZ_nz] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][29] = [7,564: $M.CVC_SEND_CAP.headset_data_2mic.Cng_Nz_Shape_Tab] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][35] = [11,695: $M.CVC_SEND_CAP.headset_data_2mic.DTC_lin] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][4] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 79 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][6] = [7,0: $M.CVC_SEND_CAP.headset_data_2mic.sndLpX_queue] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][9] = [6,720: $M.CVC_SEND_CAP.headset_data_2mic.dms_state] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][17] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 78 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][8] = [3,996: $M.CVC_SEND_CAP.headset_data_2mic.stream_x] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][11] = [3,1001: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[1,350: $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd][14] = [3,1006: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][20] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 88 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][23] = [4,916: $M.CVC_SEND_CAP.headset_data_2mic.rnr_data] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][25] = [4,258: $M.CVC_SEND_CAP.headset_data_2mic.TP_data_dm1] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][26] = [5,720: $M.CVC_SEND_CAP.headset_data_2mic.TP_data_dm2] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][29] = [4,786: $M.CVC_SEND_CAP.headset_data_2mic.beam0_tr] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][30] = [4,851: $M.CVC_SEND_CAP.headset_data_2mic.beam1_tr] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][6] = [7,170: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_LpX_queue] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][9] = [9,882: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_state] + 0 
		 //	[1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp][1] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 108 
		 //	[1,686: $M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj][0] = [3,1001: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[1,686: $M.CVC_SEND_CAP.headset_data_2mic.snd_harm_obj][2] = [4,426: $M.CVC_SEND_CAP.headset_data_2mic.harm_history] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][22] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][23] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][29] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,724: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice][31] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][5] = [3,996: $M.CVC_SEND_CAP.headset_data_2mic.stream_x] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][8] = [3,1001: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][11] = [3,1006: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][14] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][17] = [3,1026: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][19] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,758: $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd][22] = [3,1026: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm][0] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 49 
		 //	[1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm][1] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm][2] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[1,786: $M.CVC_SEND_CAP.headset_data_2mic.snd_agc400_dm][3] = [2,1010: $M.CVC_SEND_CAP.headset_data_2mic.vad_agc_obj] + 0 
		 //	[1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj][2] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 76 
		 //	[1,841: $M.CVC_SEND_CAP.headset_data_2mic.root][2] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 0 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][1] = [3,1001: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][2] = [3,1006: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][3] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][14] = [3,1011: $M.CVC_SEND_CAP.headset_data_2mic.stream_d2] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][15] = [3,1016: $M.CVC_SEND_CAP.headset_data_2mic.stream_d3] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][16] = [3,996: $M.CVC_SEND_CAP.headset_data_2mic.stream_x] + 4 
		 //	[1,863: $M.CVC_SEND_CAP.headset_data_2mic.StatusArray][19] = [3,1026: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 4 
		 //	[1,902: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad410][3] = [1,883: $M.CVC_SEND_CAP.headset_data_2mic.ref_vad_peq] + 0 
		 //	[1,919: $M.CVC_SEND_CAP.headset_data_2mic.oms_wnr_obj][0] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 71 
		 //	[1,1014: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va][4] = [3,1026: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[1,1014: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va][5] = [3,1026: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[1,1014: $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va][7] = [3,1026: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[1,1024: $M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj][3] = [2,172: $M.CVC_SEND_CAP.headset_data_2mic.GaMic1_real] + 0 
		 //	[1,1024: $M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj][4] = [3,480: $M.CVC_SEND_CAP.headset_data_2mic.GaMic1_imag] + 0 
		 //	[1,1024: $M.CVC_SEND_CAP.headset_data_2mic.aec_dm_obj][5] = [7,306: $M.CVC_SEND_CAP.headset_data_2mic.BExp_GaMic1] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][12] = [9,0: $M.CVC_SEND_CAP.headset_data_2mic.x0_saved_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][13] = [8,507: $M.CVC_SEND_CAP.headset_data_2mic.x0_saved_imag] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][15] = [8,885: $M.CVC_SEND_CAP.headset_data_2mic.x1_saved_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][16] = [8,381: $M.CVC_SEND_CAP.headset_data_2mic.x1_saved_imag] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][18] = [8,759: $M.CVC_SEND_CAP.headset_data_2mic.z0_aux_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][19] = [8,255: $M.CVC_SEND_CAP.headset_data_2mic.z0_aux_imag] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][21] = [8,633: $M.CVC_SEND_CAP.headset_data_2mic.z1_aux_real] + 0 
		 //	[2,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_freqObj_persistent][22] = [8,129: $M.CVC_SEND_CAP.headset_data_2mic.z1_aux_imag] + 0 
		 //	[2,24: $M.CVC_SEND_CAP.headset_data_2mic.snd_peq_dm2][0] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[2,24: $M.CVC_SEND_CAP.headset_data_2mic.snd_peq_dm2][1] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[2,24: $M.CVC_SEND_CAP.headset_data_2mic.snd_peq_dm2][3] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 10 
		 //	[2,47: $M.CVC_SEND_CAP.headset_data_2mic.in_r_dcblock_dm2][0] = [3,1006: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 0 
		 //	[2,47: $M.CVC_SEND_CAP.headset_data_2mic.in_r_dcblock_dm2][1] = [3,1006: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 0 
		 //	[2,62: $M.CVC_SEND_CAP.headset_data_2mic.in_l_dcblock_dm2][0] = [3,1001: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[2,62: $M.CVC_SEND_CAP.headset_data_2mic.in_l_dcblock_dm2][1] = [3,1001: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[2,77: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1][1] = [3,1026: $M.CVC_SEND_CAP.headset_data_2mic.stream_z1] + 0 
		 //	[2,77: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd_va1][2] = [6,0: $M.CVC_SEND_CAP.headset_data_2mic.bufd_outp_va1] + 0 
		 //	[2,84: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd][1] = [3,1021: $M.CVC_SEND_CAP.headset_data_2mic.stream_z0] + 0 
		 //	[2,84: $M.CVC_SEND_CAP.headset_data_2mic.fbs_snd][2] = [6,360: $M.CVC_SEND_CAP.headset_data_2mic.bufd_outp] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][5] = [3,1006: $M.CVC_SEND_CAP.headset_data_2mic.stream_d1] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][6] = [5,240: $M.CVC_SEND_CAP.headset_data_2mic.bufd_r_inp] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][9] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 137 
		 //	[2,128: $M.CVC_SEND_CAP.headset_data_2mic.dms_obj][2] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 69 
		 //	[2,128: $M.CVC_SEND_CAP.headset_data_2mic.dms_obj][4] = [6,976: $M.CVC_SEND_CAP.headset_data_2mic.dms_Ath] + 0 
		 //	[2,128: $M.CVC_SEND_CAP.headset_data_2mic.dms_obj][5] = [7,951: $M.CVC_SEND_CAP.headset_data_2mic.spp] + 0 
		 //	[2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left][1] = [3,1001: $M.CVC_SEND_CAP.headset_data_2mic.stream_d0] + 0 
		 //	[2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left][2] = [5,480: $M.CVC_SEND_CAP.headset_data_2mic.bufd_l_inp] + 0 
		 //	[2,150: $M.CVC_SEND_CAP.headset_data_2mic.fba_left][5] = [5,888: $M.CVC_SEND_CAP.headset_data_2mic.CurParams] + 137 
		 //	[2,164: $M.CVC_SEND_CAP.headset_data_2mic.AEC_Et][0] = [11,567: $M.CVC_SEND_CAP.headset_data_2mic.Et_real] + 0 
		 //	[2,167: $M.CVC_SEND_CAP.headset_data_2mic.AEC_Dt][0] = [11,631: $M.CVC_SEND_CAP.headset_data_2mic.Dt_real] + 0 
		 $DYN_SECTION_TYPE_RELOC_INST,0x0001,405,
		 0x0086,0x0000,0x0000,0x0003,0x000C,0x03E4,0x0005,0x001A,
		 0x0378,0x0105,0x0004,0x03BB,0x0105,0x0005,0x03FB,0x0109,
		 0x0017,0x02F4,0x0109,0x0018,0x0276,0x0109,0x0019,0x01F8,
		 0x0109,0x001A,0x017A,0x0109,0x001B,0x00FC,0x0109,0x001C,
		 0x007E,0x010B,0x001D,0x01F8,0x010A,0x001E,0x0377,0x010B,
		 0x001F,0x01B9,0x010A,0x0020,0x0338,0x010B,0x0021,0x017A,
		 0x010A,0x0022,0x02F9,0x010B,0x0023,0x00BD,0x010A,0x0024,
		 0x023C,0x010B,0x0025,0x007E,0x010A,0x0026,0x01FD,0x010B,
		 0x0027,0x013B,0x010A,0x0028,0x02BA,0x010B,0x0029,0x00FC,
		 0x010A,0x002A,0x027B,0x010B,0x002B,0x003F,0x010A,0x002C,
		 0x01BE,0x010B,0x002D,0x0000,0x010A,0x002E,0x017F,0x0104,
		 0x002F,0x0313,0x0104,0x0030,0x0354,0x010A,0x0031,0x03B6,
		 0x010A,0x0032,0x0140,0x0107,0x0033,0x03B8,0x0105,0x006A,
		 0x037A,0x0105,0x007E,0x0000,0x0105,0x0083,0x03D2,0x0103,
		 0x008A,0x03E4,0x0104,0x008D,0x0000,0x0102,0x008E,0x02B0,
		 0x0102,0x008F,0x03F7,0x0102,0x0092,0x01AE,0x0103,0x0093,
		 0x02E2,0x0107,0x0094,0x01B3,0x0108,0x0096,0x0000,0x0107,
		 0x0097,0x02B5,0x010A,0x0098,0x0100,0x010A,0x0099,0x00C0,
		 0x010A,0x009A,0x0080,0x010A,0x009B,0x0000,0x0102,0x009C,
		 0x03B2,0x010A,0x009D,0x0040,0x0107,0x009E,0x0336,0x0107,
		 0x009F,0x0234,0x010B,0x00A5,0x02B7,0x0105,0x0105,0x03C7,
		 0x0107,0x0107,0x0000,0x0106,0x010A,0x02D0,0x0105,0x0112,
		 0x03C6,0x0103,0x0166,0x03E4,0x0103,0x0169,0x03E9,0x0103,
		 0x016C,0x03EE,0x0105,0x0219,0x03D0,0x0104,0x021C,0x0394,
		 0x0104,0x021E,0x0102,0x0105,0x021F,0x02D0,0x0104,0x0222,
		 0x0312,0x0104,0x0223,0x0353,0x0107,0x0255,0x00AA,0x0109,
		 0x0258,0x0372,0x0105,0x0286,0x03E4,0x0103,0x02AE,0x03E9,
		 0x0104,0x02B0,0x01AA,0x0103,0x02EA,0x03FD,0x0103,0x02EB,
		 0x03FD,0x0103,0x02F1,0x03FD,0x0103,0x02F3,0x03FD,0x0103,
		 0x02FB,0x03E4,0x0103,0x02FE,0x03E9,0x0103,0x0301,0x03EE,
		 0x0103,0x0304,0x03FD,0x0103,0x0307,0x0402,0x0103,0x0309,
		 0x03FD,0x0103,0x030C,0x0402,0x0105,0x0312,0x03A9,0x0103,
		 0x0313,0x03FD,0x0103,0x0314,0x03FD,0x0102,0x0315,0x03F2,
		 0x0105,0x0330,0x03C4,0x0105,0x034B,0x0378,0x0103,0x0360,
		 0x03ED,0x0103,0x0361,0x03F2,0x0103,0x0362,0x0401,0x0103,
		 0x036D,0x03F7,0x0103,0x036E,0x03FC,0x0103,0x036F,0x03E8,
		 0x0103,0x0372,0x0406,0x0101,0x0389,0x0373,0x0105,0x0397,
		 0x03BF,0x0103,0x03FA,0x0402,0x0103,0x03FB,0x0402,0x0103,
		 0x03FD,0x0402,0x0102,0x0403,0x00AC,0x0103,0x0404,0x01E0,
		 0x0107,0x0405,0x0132,0x0209,0x000C,0x0000,0x0208,0x000D,
		 0x01FB,0x0208,0x000F,0x0375,0x0208,0x0010,0x017D,0x0208,
		 0x0012,0x02F7,0x0208,0x0013,0x00FF,0x0208,0x0015,0x0279,
		 0x0208,0x0016,0x0081,0x0203,0x0018,0x03FD,0x0203,0x0019,
		 0x03FD,0x0205,0x001B,0x0382,0x0203,0x002F,0x03EE,0x0203,
		 0x0030,0x03EE,0x0203,0x003E,0x03E9,0x0203,0x003F,0x03E9,
		 0x0203,0x004E,0x0402,0x0206,0x004F,0x0000,0x0203,0x0055,
		 0x03FD,0x0206,0x0056,0x0168,0x0203,0x0060,0x03EE,0x0205,
		 0x0061,0x00F0,0x0205,0x0064,0x0401,0x0205,0x0082,0x03BD,
		 0x0206,0x0084,0x03D0,0x0207,0x0085,0x03B7,0x0203,0x0097,
		 0x03E9,0x0205,0x0098,0x01E0,0x0205,0x009B,0x0401,0x020B,
		 0x00A4,0x0237,0x020B,0x00A7,0x0277,
		 // Variant 0x0001: 40 Internal Links to Resolve (Scratch)
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][9] = [2,0: $dm1_scratch] + 0 
		 //	[1,0: $M.CVC_SEND_CAP.headset_data_2mic.asf_object][10] = [5,0: $dm2_scratch] + 0 
		 //	[1,102: $M.CVC_SEND_CAP.headset_data_2mic.ndvc_obj][3] = [1,516: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_LpXnz] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][6] = [4,387: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_G] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][7] = [1,516: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_LpXnz] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][30] = [2,259: $M.CVC_SEND_CAP.headset_data_2mic.L_adaptA] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][31] = [5,0: $M.CVC_SEND_CAP.headset_data_2mic.Exp_Mts_adapt] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][32] = [1,387: $M.CVC_SEND_CAP.headset_data_2mic.AttenuationPersist] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][33] = [2,0: $M.CVC_SEND_CAP.headset_data_2mic.W_ri] + 0 
		 //	[1,130: $M.CVC_SEND_CAP.headset_data_2mic.aec_obj][34] = [2,388: $M.CVC_SEND_CAP.headset_data_2mic.L_adaptR] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][7] = [4,516: $M.CVC_SEND_CAP.headset_data_2mic.G_dmsZ] + 0 
		 //	[1,257: $M.CVC_SEND_CAP.headset_data_2mic.dms200_obj][10] = [2,0: $dm1_scratch] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][0] = [3,0: $M.CVC_SEND_CAP.headset_data_2mic.PIZ0r] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][1] = [6,0: $M.CVC_SEND_CAP.headset_data_2mic.PIZ0i] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][3] = [3,129: $M.CVC_SEND_CAP.headset_data_2mic.PIZ1r] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][4] = [6,129: $M.CVC_SEND_CAP.headset_data_2mic.PIZ1i] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][6] = [3,258: $M.CVC_SEND_CAP.headset_data_2mic.POZ0r] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][7] = [6,258: $M.CVC_SEND_CAP.headset_data_2mic.POZ0i] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][9] = [3,387: $M.CVC_SEND_CAP.headset_data_2mic.POZ1r] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][10] = [6,387: $M.CVC_SEND_CAP.headset_data_2mic.POZ1i] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][18] = [2,0: $dm1_scratch] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][19] = [5,0: $dm2_scratch] + 0 
		 //	[1,517: $M.CVC_SEND_CAP.headset_data_2mic.block?3][22] = [1,387: $M.CVC_SEND_CAP.headset_data_2mic.AttenuationPersist] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][7] = [4,387: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_G] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][8] = [1,516: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_LpXnz] + 0 
		 //	[1,591: $M.CVC_SEND_CAP.headset_data_2mic.oms280in0_obj][10] = [2,0: $dm1_scratch] + 0 
		 //	[1,645: $M.CVC_SEND_CAP.headset_data_2mic.vsm_fdnlp][4] = [1,387: $M.CVC_SEND_CAP.headset_data_2mic.AttenuationPersist] + 0 
		 //	[1,814: $M.CVC_SEND_CAP.headset_data_2mic.mgdc100_obj][5] = [4,387: $M.CVC_SEND_CAP.headset_data_2mic.in0oms_G] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][1] = [1,129: $M.CVC_SEND_CAP.headset_data_2mic.D_r_real] + 0 
		 //	[2,91: $M.CVC_SEND_CAP.headset_data_2mic.block?1][2] = [4,129: $M.CVC_SEND_CAP.headset_data_2mic.D_r_imag] + 0 
		 //	[2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj][1] = [2,0: $dm1_scratch] + 0 
		 //	[2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj][2] = [5,0: $dm2_scratch] + 0 
		 //	[2,119: $M.CVC_SEND_CAP.headset_data_2mic.fft_obj][3] = [2,0: $dm1_scratch] + 128 
		 //	[2,136: $M.CVC_SEND_CAP.headset_data_2mic.block?0][1] = [1,258: $M.CVC_SEND_CAP.headset_data_2mic.X_real] + 0 
		 //	[2,136: $M.CVC_SEND_CAP.headset_data_2mic.block?0][2] = [4,258: $M.CVC_SEND_CAP.headset_data_2mic.X_imag] + 0 
		 //	[2,136: $M.CVC_SEND_CAP.headset_data_2mic.block?0][5] = [1,0: $M.CVC_SEND_CAP.headset_data_2mic.D_l_real] + 0 
		 //	[2,136: $M.CVC_SEND_CAP.headset_data_2mic.block?0][6] = [4,0: $M.CVC_SEND_CAP.headset_data_2mic.D_l_imag] + 0 
		 //	[2,160: $M.CVC_SEND_CAP.headset_data_2mic.vad_peq_output][0] = [2,0: $dm1_scratch] + 0 
		 //	[2,164: $M.CVC_SEND_CAP.headset_data_2mic.AEC_Et][1] = [4,645: $M.CVC_SEND_CAP.headset_data_2mic.Et_imag] + 0 
		 //	[2,167: $M.CVC_SEND_CAP.headset_data_2mic.AEC_Dt][1] = [4,709: $M.CVC_SEND_CAP.headset_data_2mic.Dt_imag] + 0 
		 $DYN_SECTION_TYPE_RELOC_SCRATCH,0x0001,123,
		 0x0028,0x0000,0x0001,0x0102,0x0009,0x0000,0x0105,0x000A,
		 0x0000,0x0101,0x0069,0x0204,0x0104,0x0088,0x0183,0x0101,
		 0x0089,0x0204,0x0102,0x00A0,0x0103,0x0105,0x00A1,0x0000,
		 0x0101,0x00A2,0x0183,0x0102,0x00A3,0x0000,0x0102,0x00A4,
		 0x0184,0x0104,0x0108,0x0204,0x0102,0x010B,0x0000,0x0103,
		 0x0205,0x0000,0x0106,0x0206,0x0000,0x0103,0x0208,0x0081,
		 0x0106,0x0209,0x0081,0x0103,0x020B,0x0102,0x0106,0x020C,
		 0x0102,0x0103,0x020E,0x0183,0x0106,0x020F,0x0183,0x0102,
		 0x0217,0x0000,0x0105,0x0218,0x0000,0x0101,0x021B,0x0183,
		 0x0104,0x0256,0x0183,0x0101,0x0257,0x0204,0x0102,0x0259,
		 0x0000,0x0101,0x0289,0x0183,0x0104,0x0333,0x0183,0x0201,
		 0x005C,0x0081,0x0204,0x005D,0x0081,0x0202,0x0078,0x0000,
		 0x0205,0x0079,0x0000,0x0202,0x007A,0x0080,0x0201,0x0089,
		 0x0102,0x0204,0x008A,0x0102,0x0201,0x008D,0x0000,0x0204,
		 0x008E,0x0000,0x0202,0x00A0,0x0000,0x0204,0x00A5,0x0285,
		 0x0204,0x00A8,0x02C5,
#endif /* EXPORTED_CONSTANTS_ONCHIP_BUILD */
		 //End of Descriptor
		 $DYN_SECTION_TYPE_END;


#if !defined(EXPORTED_CONSTANTS_ONCHIP_BUILD) || EXPORTED_CONSTANTS_ONCHIP_BUILD
	.VAR/CONST $M.CVC_SEND_CAP.headset_data_2mic.Downloadable.DynTable_Linker[] = 
		 // Variant 0xFFFF: 113 External Links to resolve
		 //	[1,0] $M.CVC_SEND_CAP.headset_data_2mic.asf_object[11] = &$asf100.bf.func_adaptive_2mic_mvdr_beam 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[0] = &$frame_proc.distribute_streams 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[3] = &$cvc.pre_process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[6] = &$cvc.stream_transfer.peak 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[9] = &$cvc.stream_transfer.peak 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[12] = &$cvc.stream_transfer.peak 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[15] = &$cvc.peq.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[18] = &$cvc.peq.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[21] = &$cvc.peq.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[24] = &$filter_bank.analysis.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[25] = &$cvc.mc.ref_delay 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[27] = &$vad410.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[30] = &$cvc.event.echo_flag 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[33] = &$harm100.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[36] = &$filter_bank.analysis.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[39] = &$filter_bank.analysis.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[42] = &$mgdc100.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[43] = &$cvc.mc.mgdc100 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[45] = &$cvc.mgdc_persist.state_upload 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[48] = &$cvc.mgdc.harm_dynamic 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[51] = &$dms200.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[52] = &$cvc.mc.oms_in 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[54] = &$aec520.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[55] = &$cvc.mc.aec520 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[57] = &$dmss.input_power_monitor 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[60] = &$asf100.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[61] = &$cvc.mc.asf100 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[63] = &$dmss.output_power_monitor 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[66] = &$dmss.rnr.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[67] = &$cvc.mc.dmss_rnr 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[69] = &$dms200.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[70] = &$cvc.mc.dms200 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[72] = &$dms200.apply_gain 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[73] = &$cvc.mc.dms_out 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[75] = &$dmss.calc_TR 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[78] = &$ndvc200.process 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[81] = &$cvc.run_function_table 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[82] = &$cvc.mc.voice 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[84] = &$cvc.run_function_table 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[85] = &$cvc.mc.va 
		 //	[1,350] $M.CVC_SEND_CAP.headset_data_2mic.hfk_proc_funcsSnd[87] = &$frame_proc.update_streams 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[0] = &$cvc.init.root 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[3] = &$cvc.peq.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[6] = &$cvc.peq.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[9] = &$cvc.peq.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[12] = &$filter_bank.synthesis.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[15] = &$filter_bank.synthesis.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[18] = &$filter_bank.analysis.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[21] = &$filter_bank.analysis.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[24] = &$harm100.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[25] = &$cvc.init.harm 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[27] = &$mgdc100.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[30] = &$cvc.mgdc_persist.init 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[33] = &$oms280.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[34] = &$cvc.init.oms_in 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[36] = &$ndvc200.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[37] = &$cvc.init.ndvc200 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[39] = &$vad410.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[42] = &$filter_bank.analysis.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[45] = &$aec520.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[46] = &$cvc.init.aec520 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[48] = &$aec520.nlp.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[49] = &$cvc.init.vsm_fdnlp 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[51] = &$dmss.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[54] = &$asf100.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[55] = &$cvc.init.asf100 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[57] = &$dms200.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[58] = &$cvc.init.dms200 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[60] = &$cvc.user.dms200.wnr.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[63] = &$cvc.user.dms200.va_init 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[66] = &$aed100.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[67] = &$cvc.init.aed100 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[69] = &$cvc.peq.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[72] = &$agc400.initialize 
		 //	[1,441] $M.CVC_SEND_CAP.headset_data_2mic.ReInitializeTable[73] = &$cvc.init.agc400 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[0] = &$dmss.tp.process 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[1] = &$cvc.mc.dmss_tp 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[3] = &$aed100.process 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[4] = &$cvc.mc.aed100 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[6] = &$aec520.nlp.process 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[7] = &$cvc.mc.aec520_nlp 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[9] = &$aec520.cng.process 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[10] = &$cvc.mc.aec520_cng 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[12] = &$dmss.rnr.gain_apply 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[15] = &$filter_bank.synthesis.process 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[18] = &$cvc.peq.process 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[21] = &$cvc_send.stream_gain.process.voice 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[24] = &$agc400.process 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[25] = &$cvc.mc.agc400 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[27] = &$cvc_send.mute_control 
		 //	[1,724] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_voice[30] = &$cvc.stream_transfer.peak.output 
		 //	[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[0] = &$frame_proc.distribute_streams 
		 //	[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[3] = &$cvc.stream_transfer.peak 
		 //	[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[6] = &$cvc.stream_transfer.peak 
		 //	[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[9] = &$cvc.stream_transfer.peak 
		 //	[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[12] = &$cvc_send.passthrough.voice 
		 //	[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[15] = &$cvc_send.passthrough.va 
		 //	[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[18] = &$cvc.stream_transfer.peak.output 
		 //	[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[21] = &$cvc.stream_transfer.peak.output 
		 //	[1,758] $M.CVC_SEND_CAP.headset_data_2mic.copy_proc_funcsSnd[24] = &$frame_proc.update_streams 
		 //	[1,936] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout_va1[6] = &$frame_proc.distribute_output_stream 
		 //	[1,936] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout_va1[7] = &$frame_proc.update_output_stream 
		 //	[1,949] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_right_adc[6] = &$frame_proc.distribute_input_stream 
		 //	[1,949] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_right_adc[7] = &$frame_proc.update_input_stream 
		 //	[1,962] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_left_adc[6] = &$frame_proc.distribute_input_stream 
		 //	[1,962] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_left_adc[7] = &$frame_proc.update_input_stream 
		 //	[1,975] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_refin[6] = &$frame_proc.distribute_input_stream 
		 //	[1,975] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_refin[7] = &$frame_proc.update_input_stream 
		 //	[1,988] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout[6] = &$frame_proc.distribute_output_stream 
		 //	[1,988] $M.CVC_SEND_CAP.headset_data_2mic.stream_map_sndout[7] = &$frame_proc.update_output_stream 
		 //	[1,1014] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va[0] = &$filter_bank.synthesis.process 
		 //	[1,1014] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va[3] = &$cvc_send.stream_gain.process.va 
		 //	[1,1014] $M.CVC_SEND_CAP.headset_data_2mic.proc_stream_va[6] = &$cvc.stream_transfer.peak.output 
		 $DYN_SECTION_TYPE_DATA_INST,0xFFFF,226,
		 0x01000B,$asf100.bf.func_adaptive_2mic_mvdr_beam,
		 0x01015E,$frame_proc.distribute_streams,
		 0x010161,$cvc.pre_process,
		 0x010164,$cvc.stream_transfer.peak,
		 0x010167,$cvc.stream_transfer.peak,
		 0x01016A,$cvc.stream_transfer.peak,
		 0x01016D,$cvc.peq.process,
		 0x010170,$cvc.peq.process,
		 0x010173,$cvc.peq.process,
		 0x010176,$filter_bank.analysis.process,
		 0x010177,$cvc.mc.ref_delay,
		 0x010179,$vad410.process,
		 0x01017C,$cvc.event.echo_flag,
		 0x01017F,$harm100.process,
		 0x010182,$filter_bank.analysis.process,
		 0x010185,$filter_bank.analysis.process,
		 0x010188,$mgdc100.process,
		 0x010189,$cvc.mc.mgdc100,
		 0x01018B,$cvc.mgdc_persist.state_upload,
		 0x01018E,$cvc.mgdc.harm_dynamic,
		 0x010191,$dms200.process,
		 0x010192,$cvc.mc.oms_in,
		 0x010194,$aec520.process,
		 0x010195,$cvc.mc.aec520,
		 0x010197,$dmss.input_power_monitor,
		 0x01019A,$asf100.process,
		 0x01019B,$cvc.mc.asf100,
		 0x01019D,$dmss.output_power_monitor,
		 0x0101A0,$dmss.rnr.process,
		 0x0101A1,$cvc.mc.dmss_rnr,
		 0x0101A3,$dms200.process,
		 0x0101A4,$cvc.mc.dms200,
		 0x0101A6,$dms200.apply_gain,
		 0x0101A7,$cvc.mc.dms_out,
		 0x0101A9,$dmss.calc_TR,
		 0x0101AC,$ndvc200.process,
		 0x0101AF,$cvc.run_function_table,
		 0x0101B0,$cvc.mc.voice,
		 0x0101B2,$cvc.run_function_table,
		 0x0101B3,$cvc.mc.va,
		 0x0101B5,$frame_proc.update_streams,
		 0x0101B9,$cvc.init.root,
		 0x0101BC,$cvc.peq.initialize,
		 0x0101BF,$cvc.peq.initialize,
		 0x0101C2,$cvc.peq.initialize,
		 0x0101C5,$filter_bank.synthesis.initialize,
		 0x0101C8,$filter_bank.synthesis.initialize,
		 0x0101CB,$filter_bank.analysis.initialize,
		 0x0101CE,$filter_bank.analysis.initialize,
		 0x0101D1,$harm100.initialize,
		 0x0101D2,$cvc.init.harm,
		 0x0101D4,$mgdc100.initialize,
		 0x0101D7,$cvc.mgdc_persist.init,
		 0x0101DA,$oms280.initialize,
		 0x0101DB,$cvc.init.oms_in,
		 0x0101DD,$ndvc200.initialize,
		 0x0101DE,$cvc.init.ndvc200,
		 0x0101E0,$vad410.initialize,
		 0x0101E3,$filter_bank.analysis.initialize,
		 0x0101E6,$aec520.initialize,
		 0x0101E7,$cvc.init.aec520,
		 0x0101E9,$aec520.nlp.initialize,
		 0x0101EA,$cvc.init.vsm_fdnlp,
		 0x0101EC,$dmss.initialize,
		 0x0101EF,$asf100.initialize,
		 0x0101F0,$cvc.init.asf100,
		 0x0101F2,$dms200.initialize,
		 0x0101F3,$cvc.init.dms200,
		 0x0101F5,$cvc.user.dms200.wnr.initialize,
		 0x0101F8,$cvc.user.dms200.va_init,
		 0x0101FB,$aed100.initialize,
		 0x0101FC,$cvc.init.aed100,
		 0x0101FE,$cvc.peq.initialize,
		 0x010201,$agc400.initialize,
		 0x010202,$cvc.init.agc400,
		 0x0102D4,$dmss.tp.process,
		 0x0102D5,$cvc.mc.dmss_tp,
		 0x0102D7,$aed100.process,
		 0x0102D8,$cvc.mc.aed100,
		 0x0102DA,$aec520.nlp.process,
		 0x0102DB,$cvc.mc.aec520_nlp,
		 0x0102DD,$aec520.cng.process,
		 0x0102DE,$cvc.mc.aec520_cng,
		 0x0102E0,$dmss.rnr.gain_apply,
		 0x0102E3,$filter_bank.synthesis.process,
		 0x0102E6,$cvc.peq.process,
		 0x0102E9,$cvc_send.stream_gain.process.voice,
		 0x0102EC,$agc400.process,
		 0x0102ED,$cvc.mc.agc400,
		 0x0102EF,$cvc_send.mute_control,
		 0x0102F2,$cvc.stream_transfer.peak.output,
		 0x0102F6,$frame_proc.distribute_streams,
		 0x0102F9,$cvc.stream_transfer.peak,
		 0x0102FC,$cvc.stream_transfer.peak,
		 0x0102FF,$cvc.stream_transfer.peak,
		 0x010302,$cvc_send.passthrough.voice,
		 0x010305,$cvc_send.passthrough.va,
		 0x010308,$cvc.stream_transfer.peak.output,
		 0x01030B,$cvc.stream_transfer.peak.output,
		 0x01030E,$frame_proc.update_streams,
		 0x0103AE,$frame_proc.distribute_output_stream,
		 0x0103AF,$frame_proc.update_output_stream,
		 0x0103BB,$frame_proc.distribute_input_stream,
		 0x0103BC,$frame_proc.update_input_stream,
		 0x0103C8,$frame_proc.distribute_input_stream,
		 0x0103C9,$frame_proc.update_input_stream,
		 0x0103D5,$frame_proc.distribute_input_stream,
		 0x0103D6,$frame_proc.update_input_stream,
		 0x0103E2,$frame_proc.distribute_output_stream,
		 0x0103E3,$frame_proc.update_output_stream,
		 0x0103F6,$filter_bank.synthesis.process,
		 0x0103F9,$cvc_send.stream_gain.process.va,
		 0x0103FC,$cvc.stream_transfer.peak.output,
		 //End of Descriptor
		 $DYN_SECTION_TYPE_END;

#endif /* EXPORTED_CONSTANTS_ONCHIP_BUILD */


.ENDMODULE; 

#endif /* !defined(EXPORTED_CONSTANTS_ONCHIP_BUILD) || EXPORTED_CONSTANTS_ONCHIP_BUILD */
