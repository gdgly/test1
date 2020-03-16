#include "stdafx.h"
#include "ddr_base_param.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

DDRBaseParam::DDRBaseParam()
{

}

DDRBaseParam::~DDRBaseParam()
{

}

void DDRBaseParam::set_error(const char *type,const char *report)
{
	TRACE("ddr param set error type:%s report:%s\n",type,report);
}

unsigned int DDRBaseParam::sdram_size(int cs, struct ddr_params *p)
{
	unsigned int dw;
	unsigned int banks;
	unsigned int size = 0;
	unsigned int row, col;

	switch (cs) {
	case 0:
		if (p->cs0 == 1) {
			row = p->row;
			col = p->col;
			break;
		} else
			return 0;
	case 1:
		if (p->cs1 == 1) {
			row = p->row1;
			col = p->col1;
			break;
		} else
			return 0;
	default:
		return 0;
	}

	banks = p->bank8 ? 8 : 4;
	dw = p->dw32 ? 4 : 2;
	size = (1 << (row + col)) * dw * banks;
	return size;
}

//uint64_t DDRBaseParam::get_ddr_param_ps(const char *s)
//{
//	string key = s;
//	ddr_cell *c = ddr_param_map[key];
//	if(c == NULL){
//		printf("ddr param name error! %s\n", s);
//		return 0;
//	}
//	if(strcasecmp("tck",c->get_unit()) == 0){
//		return c->get_data() * ps_per_tck;
//	}
//	if(strcasecmp("ps",c->get_unit()) == 0){
//		return c->get_data();
//	}
//	if(strcasecmp("ns",c->get_unit()) == 0){
//		return c->get_data() * 1000;
//	}
//	return c->get_data();
//}

void DDRBaseParam::ddr_fill_chip_param(void)
{
	struct ddr_params_common *params = &mDDRParams.private_params.ddr_base_params;
	memset(&mDDRParams.private_params, 0, sizeof(union private_params));

	params->tRAS = 40*1000;
	params->tRP = 15*1000;
	params->tRCD = 15*1000;
	params->tRC = 55*1000;
	params->tWR = 15*1000;
	params->tRRD = 10*1000;
	params->tWTR = 2*ps_per_tck;
	params->tRFC = 72*1000;
	params->tXP = 2*ps_per_tck;
	params->tCKE = 2*ps_per_tck;
	params->tREFI = 7800*1000;
	params->WL = 1;
	params->RL = 3*ps_per_tck;

	struct lpddr_params *lparams = &mDDRParams.private_params.lpddr_params;
	mDDRParams.cl = 3;
	lparams->RL = mDDRParams.cl;
	lparams->tXSRD = 200*1000;
	lparams->tMRD = 2*ps_per_tck;
	lparams->tDQSSMAX = 1*ps_per_tck;

	//	printf("&&&&&&&&&&&&& params->WL = %d\n",params->WL);
}

void DDRBaseParam::mem_remap(struct ddr_params *p)
{
	int address_bits;
	int swap_bits;
	int bank_bits;
	int startA, startB;
	int bit_width;
	unsigned int remap_array[5];
	unsigned char *s;
	int width;
	s = (unsigned char *)remap_array;
	for(unsigned int i = 0;i < sizeof(remap_array);i++)
		s[i] = i;

	if(p->size.chip1 && (p->size.chip0 != p->size.chip1))
		return;

	bank_bits = p->bank8 == 1 ? 3 : 2;
	bit_width = p->dw32 == 1 ? 2 : 1;

	/*
	 * count the chips's address space bits.
	 */
	address_bits =  bit_width + p->col + p->row + bank_bits + (p->cs0 + p->cs1 - 1);
	/*
	 * count address space bits for swap.
	 */
	swap_bits = bank_bits + (p->cs0 + p->cs1 - 1);

	startA = bit_width + p->col > 12 ? bit_width + p->col : 12;

	startB = address_bits - swap_bits - startA;
	startA = startA - 12;

#define swap_bytes(buf,b1,b2) do{		\
	unsigned char swap;		\
	swap = buf[b1];			\
	buf[b1] = buf[b2];		\
	buf[b2] = swap;			\
}while(0)

	/*
	 * bank and cs swap with row.
	 */
	for(int i = startA;i < swap_bits;i++)
		swap_bytes(s,startA + i,startB + i);
	/*
	 * random high address for securing.
	 */
	for(int i = 0;i < swap_bits / 2;i++){
		int sw = swap_bits - 1 - i;
		swap_bytes(s,startA + i,startA + sw);
	}

	width = startB + startA;
	startA = startA + swap_bits;
	for(int i = 0;i < width / 2;i++){
		int sw = width - 1 - i;
		swap_bytes(s,startA + i,startA + sw);
	}
	for(unsigned int i = 0;i <sizeof(remap_array) / sizeof(remap_array[0]);i++)
	{
		p->remap_array[i] = remap_array[i];
	}
}

void DDRBaseParam::ddr_fill_param()
{
	unsigned int ddrfreq;
	ddrfreq = 200000000;
	ps_per_tck = (1000000000 / (ddrfreq / 1000));
	//	printf("****************************\n\nddrfreq = %d ps_per_tck = %d\n",ddrfreq,ps_per_tck);

	mDDRParams.bank8 = 0;
	mDDRParams.type = 1;
	mDDRParams.dw32 = 0;
	mDDRParams.freq = ddrfreq;
	mDDRParams.cs0 = 1;
	mDDRParams.cs1 = 0;
	mDDRParams.bl = 4;
	mDDRParams.col = 9;
	mDDRParams.col1 = 9;
	mDDRParams.row = 13;
	mDDRParams.row1 = 13;

	mDDRParams.size.chip0 = sdram_size(0, &mDDRParams);
	mDDRParams.size.chip1 = sdram_size(1, &mDDRParams);
	ddr_fill_chip_param();
	mem_remap(&mDDRParams);
}

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
struct ddrc_mmap {
	unsigned int size_m;
	unsigned int cs;
	unsigned int mmap0;
	unsigned int mmap1;
};
static struct ddrc_mmap dmmap[] = {
	{8, 0, 0, 0},
	{8, 1, 0, 0},
	{16, 0, 0, 0},
	{16, 1, 0, 0},
};

int DDRBaseParam::ddrc_config_creator(struct ddrc_reg *ddrc, struct ddr_params *p)
{
	unsigned int  mem_base0 = 0, mem_base1 = 0, mem_mask0 = 0, mem_mask1 = 0;
	unsigned int memsize_cs0, memsize_cs1, memsize;
	int ret = 0;
	/* CFG */
	if(p->cs1) {
		ddrc->cfg.b.ROW1 = p->row1 - 12;
		ddrc->cfg.b.COL1 = p->col1 - 8;
	}else {
		ddrc->cfg.b.ROW1 = p->row - 12;
		ddrc->cfg.b.COL1 = p->col - 8;
	}
	ddrc->cfg.b.BA1 = p->bank8;
	ddrc->cfg.b.IMBA = 1;
	ddrc->cfg.b.BSL = (p->bl == 8) ? 1 : 0;
#ifdef CONFIG_DDR_CHIP_ODT
	ddrc->cfg.b.ODTEN = 1;
#else
	ddrc->cfg.b.ODTEN = 0;
#endif
	ddrc->cfg.b.MISPE = 1;
	if(p->cs0) {
		ddrc->cfg.b.ROW0 = p->row - 12;
		ddrc->cfg.b.COL0 = p->col - 8;
	}else {
		ddrc->cfg.b.ROW0 = 0;
		ddrc->cfg.b.COL0 = 0;
	}
	ddrc->cfg.b.CS1EN = p->cs1;
	ddrc->cfg.b.CS0EN = p->cs0;

	ddrc->cfg.b.CL = 0; /* defalt and NOT used in this version */

	ddrc->cfg.b.BA0 = p->bank8;
	ddrc->cfg.b.DW = p->dw32;

	switch (p->type) {
#define _CASE(D, P)				\
	case D:				\
	ddrc->cfg.b.TYPE = P;	\
	break
	_CASE(DDR3, 6);		/* DDR3:0b110 */
	_CASE(LPDDR, 3);	/* LPDDR:0b011 */
	_CASE(LPDDR2, 5);	/* LPDDR2:0b101 */
	_CASE(LPDDR3, 5);	/* LPDDR3:0b111 current ddr control support LPDDR2 */
	_CASE(DDR2, 4);	    /* DDR2:0b100 */
#undef _CASE
	default:
		set_error("type","don't support the ddr type.!");
		ret |= 1;
		break;
	}

	/* CTRL  */
	ddrc->ctrl = DDRC_CTRL_ACTPD | DDRC_CTRL_PDT_64
			| DDRC_CTRL_PRET_8 | 0 << 6 | DDRC_CTRL_UNALIGN
									  | DDRC_CTRL_ALH | DDRC_CTRL_RDC | DDRC_CTRL_CKE;

	/* MMAP0,1 */
	memsize_cs0 = p->size.chip0;
	memsize_cs1 = p->size.chip1;
	memsize = memsize_cs0 + memsize_cs1;
	{
		int i;
		unsigned int size;
		for(i = 0; i < ARRAY_SIZE(dmmap); i++) {
			size = dmmap[i].size_m * 1024*1024;
			if(size == memsize && dmmap[i].cs == memsize_cs1) {
				ddrc->mmap[0] = dmmap[i].mmap0;
				ddrc->mmap[1] = dmmap[i].mmap1;
				return ret;
			}
		}
	}

	if (memsize > 0x20000000) {
		if (memsize_cs1) {
			mem_base0 = 0x0;
			mem_mask0 = (~((memsize_cs0 >> 24) - 1) & ~(memsize >> 24))
					& DDRC_MMAP_MASK_MASK;
			mem_base1 = (memsize_cs1 >> 24) & 0xff;
			mem_mask1 = (~((memsize_cs1 >> 24) - 1) & ~(memsize >> 24))
					& DDRC_MMAP_MASK_MASK;
		} else {
			mem_base0 = 0x0;
			mem_mask0 = ~(((memsize_cs0 * 2) >> 24) - 1) & DDRC_MMAP_MASK_MASK;
			mem_mask1 = 0;
			mem_base1 = 0xff;
		}
	} else {
		mem_base0 = (DDR_MEM_PHY_BASE >> 24) & 0xff;
		mem_mask0 = ~((memsize_cs0 >> 24) - 1) & DDRC_MMAP_MASK_MASK;
		mem_base1 = ((DDR_MEM_PHY_BASE + memsize_cs0) >> 24) & 0xff;
		mem_mask1 = ~((memsize_cs1 >> 24) - 1) & DDRC_MMAP_MASK_MASK;
	}
	ddrc->mmap[0] = mem_base0 << DDRC_MMAP_BASE_BIT | mem_mask0;
	ddrc->mmap[1] = mem_base1 << DDRC_MMAP_BASE_BIT | mem_mask1;
	return ret;
}

void DDRBaseParam::ddrp_config_creator(struct ddrp_reg *ddrp, struct ddr_params *p)
{
	unsigned int i;
	unsigned char rzq[]={
		0x00,0x01,0x02,0x03,0x06,0x07,0x04,0x05,
		0x0C,0x0D,0x0E,0x0F,0x0A,0x0B,0x08,0x09,
		0x18,0x19,0x1A,0x1B,0x1E,0x1F,0x1C,0x1D,
		0x14,0x15,0x16,0x17,0x12,0x13,0x10,0x11};  //from ddr multiphy page 158.
	ddrp->odtcr.d32 = 0;
#ifdef CONFIG_DDR_CHIP_ODT
	ddrp->odtcr.d32 = 0x84210000;   //power on default.
#endif
	/* DDRC registers assign */
	for(i = 0;i < 4;i++)
		ddrp->dxngcrt[i].d32 = 0x00090e80;
	i = 0;
#ifdef CONFIG_DDR_PHY_ODT
	for(i = 0;i < (CONFIG_DDR_DW32 + 1) * 2;i++){
		ddrp->dxngcrt[i].b.dqsrtt = 1;
		ddrp->dxngcrt[i].b.dqrtt = 1;
		ddrp->dxngcrt[i].b.dqsodt = 1;
		ddrp->dxngcrt[i].b.dqodt = 1;
	}
#else
	for(i = 0;i < (p->dw32 + 1) * 2;i++){
		ddrp->dxngcrt[i].b.dqsrtt = 0;
		ddrp->dxngcrt[i].b.dqrtt = 0;
		ddrp->dxngcrt[i].b.dxen = 1;
	}
#endif
	for(i = 0;i<sizeof(rzq);i++)
		ddrp->rzq_table[i] = rzq[i];
}
int DDRBaseParam::ps2cycle_ceil(int ps,int div_tck)
{
	return (ps + div_tck * ps_per_tck - 1) / ps_per_tck;
}
int DDRBaseParam::ps2cycle_floor(int ps)
{
	return ps / ps_per_tck;
}
int DDRBaseParam::ddr_refi_div(int reftck,int *div)
{
	int tmp,factor = 0;
	do{
		tmp = reftck;
		tmp = tmp / (16 << factor);
		factor++;
	}while(tmp > 255);
	*div = (factor - 1);
	return tmp;
}

int DDRBaseParam::ddrc_generate_register_old(void)
{
	struct ddr_params_common *p = &mDDRParams.private_params.ddr_base_params;
	int ret = 0;
	int tmp;
	int div;
	/* tRP is differ in lpddr & lpddr2 & ddr2 & ddr3*/
	/* tWTR is differ in lpddr & lpddr2 & ddr2 & ddr3*/
	/* tWTR is differ in lpddr & lpddr2 & ddr2 & ddr3*/
	tmp = ps2cycle_ceil(p->tWR,1);
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tWR","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing1.b.tWR = tmp;


	tmp = ps2cycle_ceil(p->WL,1);
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("WL","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing1.b.tWL = tmp;

	/* tCCD is differ in lpddr & lpddr2 & ddr2 & ddr3*/
	tmp = ps2cycle_ceil(p->tRAS,1);
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tRAS","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing2.b.tRAS = tmp;


	tmp = ps2cycle_ceil(p->tRCD,1);
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tRCD","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing2.b.tRCD = tmp;


	tmp = ps2cycle_ceil(p->RL,1);

	//	printf("------------------------------------------ \n \n p->RL %d:%d\n",p->RL,tmp);
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tRL","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing2.b.tRL = tmp;


	mDDRController.timing3.b.ONUM = 4; //version info.

	/* tCKSRE is differ in lpddr & lpddr2 & ddr2 & ddr3*/

	tmp = ps2cycle_ceil(p->tRP,1);
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tRP","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing3.b.tRP = tmp;

	tmp = ps2cycle_ceil(p->tRRD,1);
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tRRD","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing3.b.tRRD = tmp;

	tmp = ps2cycle_ceil(p->tRC,1);
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tRC","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing3.b.tRC = tmp;


	tmp = ps2cycle_ceil(p->tRFC,2) / 2 - 1 ;
	if(tmp < 0)
		tmp = 0;
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tRFC","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing4.b.tRFC = tmp;


	mDDRController.timing4.b.tEXTRW = 1; /* default */
	mDDRController.timing4.b.tRWCOV = 0;  /* default */


	tmp = ps2cycle_ceil(p->tCKE,1) + 1;
	if((tmp >= (1 <<(3 + 1))) || (tmp < 0))
	{
		set_error("tCKE","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing4.b.tCKE = tmp;


	tmp = ps2cycle_ceil(p->tXP,1) + 1;
	if((tmp >= (1 <<(3 + 1))) || (tmp < 0))
	{
		set_error("tXP","it too large for controller.");
		ret |= 1;
	}
	mDDRController.timing4.b.tXP = tmp;

	mDDRController.timing5.b.tCTLUPD = 0xff; /* 0xff is the default value */

	/* tRTW is differ in lpddr & lpddr2 & ddr2 & ddr3 */
	/* tRDLAT is differ in lpddr & lpddr2 & ddr2 & ddr3 */
	/* tWDLAT is differ in lpddr & lpddr2 & ddr2 & ddr3 */

	/* tXSRD is diff in lpddr & lpddr2 & ddr2 & ddr3. */
	/* tXSRD is diff in lpddr & lpddr2 & ddr2 & ddr3. */

	mDDRController.timing6.b.tCFGW = 5;  // default and controller don't used.
	mDDRController.timing6.b.tCFGR = 5;  // default and controller don't used.

	tmp = ps2cycle_floor(p->tREFI);
	tmp -= 16; // controller is add 16 cycles.
	if(tmp < 0){
		set_error("tREFI","too small for Controller");
		ret |= 1;
	}
	tmp = ddr_refi_div(tmp,&div);
	if(div > 7){
		set_error("tREFI","div too large for Controller");
		ret |= 1;
	}
	if(tmp >= (1 << (8 + 1))) {
		set_error("tREFI","too large for Controller");
		ret |= 1;
	}
	mDDRController.refcnt = (tmp << DDRC_REFCNT_CON_BIT)
			| (div << DDRC_REFCNT_CLK_DIV_BIT)
			| DDRC_REFCNT_REF_EN;

	mDDRController.autosr_en = 0;

#ifdef CONFIG_DDR_AUTO_SELF_REFRESH
	mDDRController.autosr_en = 1;
#endif
	return ret;
}

int DDRBaseParam::ddrp_generate_register_old(void)
{
	int tmp = 0;
	int ret = 0;
	struct ddr_params *p = &mDDRParams;
	struct ddrp_reg *ddrp = &mDDRPhy;
	switch (p->type) {
#define _CASE(D, P)				\
	case D:				\
	tmp = P;		\
	break
	_CASE(DDR3, 3);		/* DDR3:0b011 */
	_CASE(LPDDR, 0);	/* LPDDR:0b000 */
	_CASE(LPDDR2, 4);	/* LPDDR2:0b100 */
	_CASE(LPDDR3, 4);	/* LPDDR3:0b010 */
	_CASE(DDR2, 2);	    /* DDR2:0b010 */
#undef _CASE
	default:
		set_error("Type","ddr type not support!");
		ret |= 1;
		break;
	}
	ddrp->dcr = tmp | (p->bank8 << 3);

	/* MR0'Register is differ in lpddr ddr2 lpddr2 ddr3 */

	/* DTPR0 registers */
	/* tMRD is differ in lpddr ddr2 lpddr2 ddr3 */
	/* tRTP is differ for ddr2 */
	tmp = ps2cycle_ceil(p->private_params.ddr_base_params.tWTR,1);
	if(tmp < 1) {
		set_error("tWTR","too small for ddr phy!");
		ret |= 1;
	}else if(tmp > 6) {
		set_error("tWTR","too large for ddr phy!");
		ret |= 1;
	}
	ddrp->dtpr0.b.tWTR = tmp;

	tmp = ps2cycle_ceil(p->private_params.ddr_base_params.tRP,1);
	if(tmp < 2) {
		set_error("tRP","too small for ddr phy!");
		ret |= 1;
	}else if(tmp > 11) {
		set_error("tRP","too large for ddr phy!");
		ret |= 1;
	}
	ddrp->dtpr0.b.tRP = tmp;

	tmp = ps2cycle_ceil(p->private_params.ddr_base_params.tRCD,1);
	if(tmp < 2) {
		set_error("tRCD","too small for ddr phy!");
		ret |= 1;
	}else if(tmp > 11) {
		set_error("tRCD","too large for ddr phy!");
		ret |= 1;
	}
	ddrp->dtpr0.b.tRCD = tmp;

	tmp = ps2cycle_ceil(p->private_params.ddr_base_params.tRAS,1);
	if(tmp < 2) {
		set_error("tRAS","too small for ddr phy!");
		ret |= 1;
	}else if(tmp > 31) {
		set_error("tRAS","too large for ddr phy!");
		ret |= 1;
	}
	ddrp->dtpr0.b.tRAS = tmp;

	tmp = ps2cycle_ceil(p->private_params.ddr_base_params.tRRD,1);
	if(tmp < 1) {
		set_error("tRRD","too small for ddr phy!");
		ret |= 1;
	}else if(tmp > 8) {
		set_error("tRRD","too large for ddr phy!");
		ret |= 1;
	}
	ddrp->dtpr0.b.tRRD = tmp;

	tmp = ps2cycle_ceil(p->private_params.ddr_base_params.tRC,1);
	if(tmp < 2) {
		set_error("tRC","too small for ddr phy!");
		ret |= 1;
	}else if(tmp > 42) {
		set_error("tRC","too large for ddr phy!");
		ret |= 1;
	}
	ddrp->dtpr0.b.tRC = tmp;

	/* tCCD is differ in lpddr ddr2 lpddr2 ddr3 */

	/* DTPR1 registers */
	/* tAOND is only used by DDR2. */
	/* tRTW is differ in lpddr ddr2 lpddr2 ddr3 */
	/* tFAW is differ in lpddr ddr2 lpddr2 ddr3 */

	//DDRP_TIMING_SET(1,ddr_base_params,tFAW,6,2,31);


	/* tMOD is used by ddr3 */
	/* tRTODT is used by ddr3 */

	tmp = ps2cycle_ceil(p->private_params.ddr_base_params.tRFC,1);
	if(tmp < 0) {
		set_error("tRFC","too small for ddr phy!");
		ret |= 1;
	}else if(tmp > 255) {
		set_error("tRFC","too large for ddr phy!");
		ret |= 1;
	}
	ddrp->dtpr1.b.tRFC = tmp;

	/* tDQSCKmin is used by lpddr2 */
	/* tDQSCKmax is used by lpddr2 */

	/* DTPR2 registers */
	/* tXS is differ in lpddr2 ddr3 */
	/* tXP is differ in lpddr2 ddr3 */
	/* tCKE is differ in ddr3 */

	/* PTRn registers */
	tmp = ps2cycle_ceil(50 * 1000,1);   /* default 50 ns and min clk is 8 cycle */
	if(tmp < 8) tmp = 8;
	ddrp->ptr0.b.tDLLSRST = tmp;

	tmp = ps2cycle_ceil(5120*1000, 1); /* default 5.12 us*/
	ddrp->ptr0.b.tDLLLOCK = tmp;

	ddrp->ptr0.b.tITMSRST = 8;    /* default 8 cycle & more than 8 */
	ddrp->dtpr2.b.tDLLK = 512;
//	printf("ddrp->ptr0.b.tDLLSRST = %d ddrp->ptr0.b.tDLLLOCK = %d\n",ddrp->ptr0.b.tDLLSRST,ddrp->ptr0.b.tDLLLOCK);

	/* PGCR'Register is differ in lpddr ddr2 lpddr2 ddr3 */
	return ret;
}
int DDRBaseParam::ddrc_generate_register(void)
{
	int tmp;
	int ret;
	struct ddrc_reg *ddrc = &mDDRController;
	struct lpddr_params *p = &mDDRParams.private_params.lpddr_params;

	ret = DDRBaseParam::ddrc_generate_register_old();

	ddrc->timing3.b.tCKSRE = 0;
	ddrc->timing1.b.tWL = 1;
	ddrc->timing2.b.tRL = mDDRParams.cl;

	tmp =  mDDRParams.bl / 2;
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tRTP","it too large for controller.");
		ret |= 1;
	}
	ddrc->timing1.b.tRTP = tmp;

	tmp = ps2cycle_ceil(p->tMRD,1) - 1;
	if((tmp >= (1 <<(2 + 1))) || (tmp < 0))
	{
		set_error("tMRD","it too large for controller.");
		ret |= 1;
	}
	ddrc->timing4.b.tMRD = tmp;

	tmp = mDDRParams.bl / 2;
	ddrc->timing2.b.tCCD = tmp;

	tmp = ps2cycle_ceil(p->tWTR + p->tDQSSMAX,1) +
			mDDRParams.bl / 2; // write to read for our controller
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tRTP","it too large for controller.");
		ret |= 1;
	}
	ddrc->timing1.b.tWTR = tmp;

	tmp = mDDRParams.cl + mDDRParams.bl / 2;
	if((tmp >= (1 <<(6 + 1))) || (tmp < 0))
	{
		set_error("tRTW","it too large for controller.");
		ret |= 1;
	}
	ddrc->timing5.b.tRTW = tmp;

	if(ddrc->timing1.b.tWL > 0)
		ddrc->timing5.b.tWDLAT = ddrc->timing1.b.tWL - 1;
	else{
		set_error("tWL","it too large for controller.");
		ret |= 1;
	}
	if(ddrc->timing2.b.tRL > 1)
		ddrc->timing5.b.tRDLAT = ddrc->timing2.b.tRL - 2;
	else{
		set_error("tRL","it too large for controller.");
		ret |= 1;
	}
	tmp = ps2cycle_ceil(p->tXSRD,4) / 4;
	if((tmp >= (1 <<(8 + 1))) || (tmp < 0))
	{
		set_error("tRTW","it too large for controller.");
		ret |= 1;
	}
	ddrc->timing6.b.tXSRD = tmp;
	tmp = ps2cycle_ceil(p->tRFC,8) / 8 - 1;
	if(tmp < 0)
		tmp = 0;
	if((tmp >= (1 <<(4 + 1))) || (tmp < 0))
	{
		set_error("tXSRD","it too large for controller.");
		ret |= 1;
	}
	ddrc->timing4.b.tMINSR = tmp;
	ddrc->timing6.b.tFAW = 0;
	return ret;
}

int DDRBaseParam::ddrp_generate_register(void)
{
	int tmp;
	int  count = 0;
	struct ddrp_reg *ddrp = &mDDRPhy;
	struct lpddr_params *p = &mDDRParams.private_params.lpddr_params;
	int ret = DDRBaseParam::ddrp_generate_register_old();

	/* MRn registers */
	tmp = mDDRParams.cl;
	if(tmp < 2 ||  tmp > 4){
		set_error("CL","too large for ddr phy.");
		ret |= 1;
	}
	ddrp->mr0.lpddr.CL = tmp;

	tmp = mDDRParams.bl;
	while (tmp >>= 1) count++;
	if(tmp > (1 << (3 + 1)))
	{
		set_error("BL","too large for ddr phy.");
		ret |= 1;
	}
	ddrp->mr0.lpddr.BL = count;

	ddrp->mr2.lpddr.PASR = 0;
	ddrp->mr2.lpddr.TCSR = 3;  //85 degree centigrade.

#ifdef DDR_DRIVER_STRENGTH
	ddrp->mr2.lpddr.DS = DDR_DRIVER_STRENGTH;
#endif

	ddrp->ptr1.b.tDINIT0 = ps2cycle_ceil(200000000, 1); /* LPDDR default 200us*/
	ddrp->ptr1.b.tDINIT1 = 192;  /* default 192,LPDDR don't used */

	ddrp->ptr2.b.tDINIT2 = 106610;  /* default 106610, LPDDR don't used */
	ddrp->ptr2.b.tDINIT3 = 534;     /* default 534,    LPDDR don't used */

	/* DTPR0 registers */
	tmp = ps2cycle_ceil(p->tMRD,1);
	if(tmp > 3 || tmp < 0)
	{
		set_error("tMRD","too large for ddr phy.");
		ret |= 1;
	}
	ddrp->dtpr0.b.tMRD = tmp;

	tmp = mDDRParams.bl / 2;
	if(tmp < 2 || tmp >= 6){
		set_error("tRTP","Out of range");
		ret |= 1;
	}
	ddrp->dtpr0.b.tRTP = tmp;

	ddrp->dtpr0.b.tCCD = 0; // default 0, lpddr don't used.

	ddrp->dtpr1.b.tFAW = 18;

	/* DTPR2 registers */
	tmp = ps2cycle_ceil(p->tXSRD, 1);  // the controller is same.
	if(tmp < 2 || tmp > 1023){
		set_error("tXSR","Out of range");
		ret |= 1;
	};
	ddrp->dtpr2.b.tXS = tmp;

	tmp = ps2cycle_ceil(p->tXP, 1);  // the controller is same.
	if(tmp < 2 || tmp > 31){
		set_error("tXP","Out of range");
		ret |= 1;
	};
	ddrp->dtpr2.b.tXP = tmp;


	tmp = ps2cycle_ceil(p->tCKE, 1);  // the controller is same.
	if(tmp < 2 || tmp > 15){
		set_error("tCKE","Out of range");
		ret |= 1;
	};
	ddrp->dtpr2.b.tCKE = tmp;

	/* PGCR registers */
	ddrp->pgcr = DDRP_PGCR_ITMDMD | DDRP_PGCR_DQSCFG | 7 << DDRP_PGCR_CKEN_BIT
															| 2 << DDRP_PGCR_CKDV_BIT
															| ( mDDRParams.cs0 | mDDRParams.cs1 << 1) << DDRP_PGCR_RANKEN_BIT
															| DDRP_PGCR_PDDISDX;
	ddrp->zqncr1 = 0x7b;   // default. lpddr do not used.
	return ret;
}

int DDRBaseParam::ddr_generate_register(void)
{
	int ret = 0;
	memset(&mDDRController,0,sizeof(struct ddrc_reg));
	memset(&mDDRPhy,0,sizeof(struct ddrp_reg));
	ret |= ddrc_config_creator(&mDDRController,&mDDRParams);
	ddrp_config_creator(&mDDRPhy,&mDDRParams);
	ret |= ddrc_generate_register();
	ret |= ddrp_generate_register();
	return ret;
}

void DDRBaseParam::ddr_convert_param(struct ddr_registers *ddr_registers_param)
{
	ddr_registers_param->ddrc_cfg = mDDRController.cfg.d32;
	ddr_registers_param->ddrc_ctrl = mDDRController.ctrl;
	ddr_registers_param->ddrc_mmap[0] = mDDRController.mmap[0];
	ddr_registers_param->ddrc_mmap[1] = mDDRController.mmap[1];
	ddr_registers_param->ddrc_refcnt = mDDRController.refcnt;
	ddr_registers_param->ddrc_timing1 = mDDRController.timing1.d32;
	ddr_registers_param->ddrc_timing2 = mDDRController.timing2.d32;
	ddr_registers_param->ddrc_timing3 = mDDRController.timing3.d32;
	ddr_registers_param->ddrc_timing4 = mDDRController.timing4.d32;
	ddr_registers_param->ddrc_timing5 = mDDRController.timing5.d32;
	ddr_registers_param->ddrc_timing6 = mDDRController.timing6.d32;
	ddr_registers_param->ddrc_autosr = mDDRController.autosr_en;
	ddr_registers_param->ddrp_dcr = mDDRPhy.dcr;
	ddr_registers_param->ddrp_mr0 = mDDRPhy.mr0.d32;
	ddr_registers_param->ddrp_mr1 = mDDRPhy.mr1.d32;
	ddr_registers_param->ddrp_mr2 = mDDRPhy.mr2.d32;
	ddr_registers_param->ddrp_mr3 = mDDRPhy.mr3.d32;
	ddr_registers_param->ddrp_ptr0 = mDDRPhy.ptr0.d32;
	ddr_registers_param->ddrp_ptr1 = mDDRPhy.ptr0.d32;
	ddr_registers_param->ddrp_ptr2 = mDDRPhy.ptr0.d32;
	ddr_registers_param->ddrp_dtpr0 = mDDRPhy.dtpr0.d32;
	ddr_registers_param->ddrp_dtpr1 = mDDRPhy.dtpr1.d32;
	ddr_registers_param->ddrp_dtpr2 = mDDRPhy.dtpr2.d32;

	ddr_registers_param->ddrp_pgcr = mDDRPhy.pgcr;
	ddr_registers_param->ddrp_odtcr = mDDRPhy.odtcr.d32;

	ddr_registers_param->ddrp_dxngct[0] = mDDRPhy.dxngcrt[0].d32;
	ddr_registers_param->ddrp_dxngct[1] = mDDRPhy.dxngcrt[1].d32;
	ddr_registers_param->ddrp_dxngct[2] = mDDRPhy.dxngcrt[2].d32;
	ddr_registers_param->ddrp_dxngct[3] = mDDRPhy.dxngcrt[3].d32;

	ddr_registers_param->ddrp_zqncr1 = mDDRPhy.zqncr1;
	ddr_registers_param->ddrp_impedance[0] = mDDRPhy.impedance[0];
	ddr_registers_param->ddrp_impedance[1] = mDDRPhy.impedance[1];
	ddr_registers_param->ddrp_odt_impedance[0] = mDDRPhy.odt_impedance[0];
	ddr_registers_param->ddrp_odt_impedance[1] = mDDRPhy.odt_impedance[1];
	for(unsigned int i = 0;i < sizeof(ddr_registers_param->ddrp_rzq);i++){
		ddr_registers_param->ddrp_rzq[i] = mDDRPhy.rzq_table[i];
	}
	for(unsigned int i = 0;i < sizeof(ddr_registers_param->remap_array) / sizeof(ddr_registers_param->remap_array[0]);i++){
		ddr_registers_param->remap_array[i] = mDDRParams.remap_array[i];
	}
	ddr_registers_param->ddr_chip0_size = mDDRParams.size.chip0;
	ddr_registers_param->ddr_chip1_size = mDDRParams.size.chip1;

}