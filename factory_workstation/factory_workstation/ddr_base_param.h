#ifndef DDR_BASE_PARAM_H
#define DDR_BASE_PARAM_H
#include <stdint.h>
#include <string>
#include <map>
#include "ddr_params.h"
#include "ddrc.h"
#include "ddrp_dwc.h"
#include "ddr_dwc.h"
using namespace std;

struct ddr_registers
{
	uint32_t ddrc_cfg;
	uint32_t ddrc_ctrl;
	uint32_t ddrc_mmap[2];
	uint32_t ddrc_refcnt;
	uint32_t ddrc_timing1;
	uint32_t ddrc_timing2;
	uint32_t ddrc_timing3;
	uint32_t ddrc_timing4;
	uint32_t ddrc_timing5;
	uint32_t ddrc_timing6;
	uint32_t ddrc_autosr;
	uint32_t ddrp_dcr;
	uint32_t ddrp_mr0;
	uint32_t ddrp_mr1;
	uint32_t ddrp_mr2;
	uint32_t ddrp_mr3;
	uint32_t ddrp_ptr0;
	uint32_t ddrp_ptr1;
	uint32_t ddrp_ptr2;
	uint32_t ddrp_dtpr0;
	uint32_t ddrp_dtpr1;
	uint32_t ddrp_dtpr2;
	uint32_t ddrp_pgcr;
	uint32_t ddrp_odtcr;
	uint32_t ddrp_dxngct[4];
	uint32_t ddrp_zqncr1;
	uint32_t ddrp_impedance[2];
	uint32_t ddrp_odt_impedance[2];
	unsigned char ddrp_rzq[32];
	uint32_t ddr_chip0_size;
	uint32_t ddr_chip1_size;
	uint32_t remap_array[5];
};
typedef map<string,string> ddr_error_map;

class DDRBaseParam
{
	static const char *ddr_base_param_name[];

private:
	ddr_error_map error_map;
	unsigned int sdram_size(int cs, struct ddr_params *p);
	int ddrc_config_creator(struct ddrc_reg *ddrc, struct ddr_params *p);
	void ddrp_config_creator(struct ddrp_reg *ddrp, struct ddr_params *p);
	void save_param_to_file();
	int ddr_load_param();

protected:
	struct ddrc_reg mDDRController;
	struct ddrp_reg mDDRPhy;
	ddr_params mDDRParams;

	enum ddr_type ddr_type;
	uint32_t ps_per_tck;
	uint64_t get_ddr_param_ps(const char *s);
	int ps2cycle_ceil(int ps,int div_tck);
	int ps2cycle_floor(int ps);
	struct ddr_out_impedance*
			find_nearby_impedance(struct ddr_out_impedance *table,int table_size,int r_ohm);

	int ddr_refi_div(int reftck,int *div);
	void set_error(const char *type,const char *report);
	void mem_remap(struct ddr_params *p);
	virtual int ddrc_generate_register(void);
	virtual int ddrp_generate_register(void);
	virtual int ddrc_generate_register_old(void);
	virtual int ddrp_generate_register_old(void);
	virtual void ddr_fill_chip_param(void);


public:
	DDRBaseParam();
	virtual ~DDRBaseParam();

	void ddr_fill_param();
	void generate_ddr_param(void);
	int ddr_generate_register(void);
	int init_ddr_inifile(const char *path);
	void ddr_convert_param(struct ddr_registers *ddr_registers_param);
	void dump_ddr_register_data(struct ddr_registers *ddr_registers_param);
	void* get_param_nextline(void* handle,char *strline,int size);
	void* get_error_nextline(void* handle,char *strline,int size);
	int set_param(char* strLine,int size);
	//int get_ddr_param_changed(INIArgument *boardcfg);
	int get_ddr_type()
	{
		return ddr_type;
	}


};

#endif // DDR_BASE_PARAM_H
