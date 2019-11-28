#include "max20340.h"
#ifdef HAVE_MAX20340

/*! \brief Read a register from the proximity sensor */
bool max20340ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value)
{
    bitserial_result result;
    /* First write the register address to be read */
    result = BitserialWrite(handle,
                            BITSERIAL_NO_MSG,
                            &reg, 1,
                            BITSERIAL_FLAG_BLOCK);
    if (result == BITSERIAL_RESULT_SUCCESS){
        /* Now read the actual data in the register */
        result = BitserialRead(handle,
                                BITSERIAL_NO_MSG,
                                value, 1,
                                BITSERIAL_FLAG_BLOCK);
    }
    if(result != BITSERIAL_RESULT_SUCCESS){
        //DEBUG_LOG("%s faild,result = %d\n",__func__, result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

bool max20340ReadRegister_withlen(bitserial_handle handle, uint8 reg,  uint8 *value, uint8 len)
{
    bitserial_result result;
    /* First write the register address to be read */
    result = BitserialWrite(handle,
                            BITSERIAL_NO_MSG,
                            &reg, 1,
                            BITSERIAL_FLAG_BLOCK);
    if (result == BITSERIAL_RESULT_SUCCESS){
        /* Now read the actual data in the register */
        result = BitserialRead(handle,
                                BITSERIAL_NO_MSG,
                                value, len,
                                BITSERIAL_FLAG_BLOCK);
    }
    if(result != BITSERIAL_RESULT_SUCCESS){
        //DEBUG_LOG("%s faild,result = %d\n",__func__, result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

/*! \brief Write to a proximity sensor register */
bool max20340WriteRegister(bitserial_handle handle, uint8 reg, uint8 value)
{
    bitserial_result result;
    uint8 command[2] = {reg, value};

    /* Write the write command and register */
    result = BitserialWrite(handle,
                            BITSERIAL_NO_MSG,
                            command, 2,
                            BITSERIAL_FLAG_BLOCK);
    if(result != BITSERIAL_RESULT_SUCCESS){
        DEBUG_LOG("%s faild,result = %d\n",__func__, result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

bool max20340WriteRegister_withlen(bitserial_handle handle, uint8 reg,  uint8 *value, uint8 len)//len<20
{
    bitserial_result result;
    uint8 command[21];
    command[0] = reg;
    memcpy(command+1, value, len);

    /* Write the write command and register */
    result = BitserialWrite(handle,
                            BITSERIAL_NO_MSG,
                            command, len+1,
                            BITSERIAL_FLAG_BLOCK);
    if(result != BITSERIAL_RESULT_SUCCESS){
        DEBUG_LOG("%s faild,result = %d\n",__func__, result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

unsigned int used_i2c_addr=0;
bitserial_handle max20340Enable(void)
{
    uint16 bank;
    uint32 mask;

    bank = PIO2BANK(MAX20340_ITR_PIN);
    mask = PIO2MASK(MAX20340_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));

#ifdef MAX20340_TEST
    bank = PIO2BANK(MAX20340_TEST_PIN);
    mask = PIO2MASK(MAX20340_TEST_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));
#endif

    return hwi2cOpen(used_i2c_addr, MAX20340_I2C_FREQ);
}

void max20340Disable(bitserial_handle handle)
{
    //DEBUG_LOG("EM20168Disable");
    hwi2cClose(handle);
}

#define REAL_ITR 1
#define TEST_ITR 0
#if REAL_ITR
static void box_get_data_process(uint8 *get_buf, uint8 *send_buf, uint8 *cache_buf, uint8 need_times)
{
    uint8 type,cmd,req_data_num;
    cmd = (get_buf[0] & 0x3c) >> 2;
    type = (get_buf[0] & 0x3);
    switch(type){
    case 0://开始包
        send_buf[1]=0;
        send_buf[2]=0;
        break;
    case 1://数据包
        req_data_num = get_buf[1];
        if(req_data_num > (need_times-1)){
            req_data_num = 0;
            printf("max20340 request err cmd=%d,type=%d\n", cmd, type);
        }
        send_buf[1]=cache_buf[0+req_data_num*2];
        send_buf[2]=cache_buf[1+req_data_num*2];
        break;
    case 2://结束包
        break;
    case 3://重发包
        break;
    }
    send_buf[0] = get_buf[0];
}

static void box_send_data_process(uint8 *get_buf, uint8 *send_buf, uint8 *cache_buf, uint8 need_times, uint8 *offset)
{
    uint8 type,cmd;
    cmd = (get_buf[0] & 0x3c) >> 2;
    type = (get_buf[0] & 0x3);
    switch(type){
    case 0://开始包
        *offset = 0;
        send_buf[1] = 0;
        send_buf[2] = 0;
        break;
    case 1://数据包
        if( *offset > (need_times - 1) ){
            *offset = 0;
            DEBUG_LOG("max20340 request err cmd=%d,type=%d\n", cmd, type);
        }
        cache_buf[0+*offset*2] = get_buf[1];
        cache_buf[1+*offset*2] = get_buf[2];
        send_buf[1] = *offset;
        send_buf[2] = 0;
        (*offset) ++;
        break;
    case 2://结束包
        *offset = 0;
        send_buf[1] = 0;
        send_buf[2] = 0;
        break;
    case 3://重发包
        break;
    }
    send_buf[0] = get_buf[0];
}

static void box_get_btaddr(uint8 *get_buf, uint8 *send_buf)
{
    static uint8 btaddr[6]={0x13,0x14,0x15,0,0,1};
    uint8 type;
    type = (get_buf[0] & 0x3);
    if(type == 0){//开始包，获取一下蓝牙地址
        ;
    }
    box_get_data_process(get_buf, send_buf, btaddr, 6/2);
}


static void box_send_btaddr(uint8 *get_buf, uint8 *send_buf)
{
    static uint8 btaddr[6]={0x13,0x14,0x15,0,0,1};
    static uint8 offset = 0;
    uint8 type;
    type = (get_buf[0] & 0x3);
    if(type == 2){//结束包，蓝牙包接收完整，可以通知出去了
        DEBUG_LOG("btaddr = %x:%x:%x:%x:%x:%x\n",
               btaddr[0], btaddr[1], btaddr[2], btaddr[3], btaddr[4], btaddr[5]);
    }
    box_send_data_process(get_buf, send_buf, btaddr, 6/2, &offset);
}

static void box_get_earsoft_version(uint8 *get_buf, uint8 *send_buf)
{
    static uint8 version[8]={0x1,0x0,0x1,0,0,0,2,3};
    uint8 type;
    type = (get_buf[0] & 0x3);
    if(type == 0){//开始包，获取版本号
        SystemGetCurrentVersion(version);
    }
    box_get_data_process(get_buf, send_buf, version, 8/2);
}

static void box_send_earsoft_version(uint8 *get_buf, uint8 *send_buf)
{
    static uint8 version[8];
    static uint8 offset = 0;
    uint8 type;
    type = (get_buf[0] & 0x3);
    if(type == 2){//版本号接收完整，可以通知出去了
        SystemSetVersion(DEV_RIGHT,version);
    }
    box_send_data_process(get_buf, send_buf, version, 8/2, &offset);
}

static void box_send_boxhwsoft_version(uint8 *get_buf, uint8 *send_buf)
{
    static uint8 version[8];
    static uint8 offset = 0;
    uint8 type;
    type = (get_buf[0] & 0x3);
    if(type == 2){//版本号接收完整，可以通知出去了
        SystemSetVersion(DEV_CASE,version);
    }
    box_send_data_process(get_buf, send_buf, version, 8/2, &offset);
}

static void box_send_boxpower(uint8 *get_buf, uint8 *send_buf)
{
    appUiCaseStatus(0, 0, 0, get_buf[1]);
    send_buf[0] = get_buf[0];
    send_buf[1] = 0;
    send_buf[2] = 0;
}

static void box_get_earpower(uint8 *get_buf, uint8 *send_buf)
{
    send_buf[0] = get_buf[0];
    send_buf[1] = appUiGetPower();
    send_buf[2] = 0;
    DEBUG_LOG("Case GetPower=%d", send_buf[1]);
}

static void box_send_boxevent(uint8 *get_buf, uint8 *send_buf)
{
    uint8 if_cap, if_usb, if_key, key_time;
    if_cap = (get_buf[1] & 0xf0) >> 4;
    if_usb = (get_buf[1] & 0x0f);
    if_key = (get_buf[2] & 0x80) >> 7;
    key_time = (get_buf[2] & 0x7f);
    if(if_cap == 0x1){//盒盖
        appUiCaseStatus(0,-1,-1,-1);
    }
    if(if_cap == 0x2){//开盖
        appUiCaseStatus(1,-1,-1,-1);
    }
    if(if_usb == 0x1){//插入

    }
    if(if_usb == 0x2){//拔出

    }
    if(if_key == 0x1){//按键事件
        if(key_time<4){
            appUiCaseStatus(-1,1,-1,-1);
        }else{
            appUiCaseStatus(-1,-1,1,-1);
        }
    }
    send_buf[0] = get_buf[0];
    send_buf[1] = 0;
    send_buf[2] = 0;
}

static void box_update(uint8 *get_buf, uint8 *send_buf)
{
    static uint8 buffer_1k[1024];
    uint8 type,start_flag;
    uint16 checksum=0,i,data_num;
    type = (get_buf[0] & 0x3);
    switch(type){
    case 0://开始包
        start_flag = get_buf[1];
        if(start_flag == 1){//开始包1 回复总共需要多少个1k字节
            send_buf[1] = 10;//先假设是10个之后计算出确认值
            send_buf[2] = 0;
        }else if(start_flag == 2){//开始包2 表示接下来要开始发送第几个1k
            //u8 num_k = get_buf[2];
            //memcpy(buffer_1k, buf, 1024);
            for(i=0; i<1024; i++){
                checksum += buffer_1k[i];
            }
        }else if(start_flag == 3){//开始包3 耳机需回复1k字节的校验码
            send_buf[1] = ((checksum>>8) & 0xff);
            send_buf[2] = (checksum & 0xff);
        }else{
            DEBUG_LOG("update request start_flag error! start_flag = %d\n", start_flag);
        }
        break;
    case 1://数据包
        data_num = (get_buf[1]<<8 + get_buf[2]);//0-511
        if(data_num>511){
            DEBUG_LOG("update request data_num=%d error\n", data_num);
        }
        send_buf[1]=buffer_1k[0+(data_num)*2];
        send_buf[2]=buffer_1k[1+(data_num)*2];
        break;
    case 2://结束包
        //表示升级完成
        break;
    case 3://重发包
        break;
    }
    send_buf[0] = get_buf[0];
}

static void recv_data_process_cmd(bitserial_handle handle, uint8 *buf)
{
    uint8 send_buf[3];
    uint8 cmd;
    cmd = (buf[MX20340_REG_RX_DATA0] & 0x3c) >> 2;
    switch(cmd){
    case 1://盒子获取蓝牙地址
        box_get_btaddr(&buf[MX20340_REG_RX_DATA0], send_buf);
        break;
    case 2://盒子发送蓝牙地址
        box_send_btaddr(&buf[MX20340_REG_RX_DATA0], send_buf);
        break;
    case 3://盒子获取耳机软件版本
        box_get_earsoft_version(&buf[MX20340_REG_RX_DATA0], send_buf);
        break;
    case 4://盒子发送耳机软件版本
        box_send_earsoft_version(&buf[MX20340_REG_RX_DATA0], send_buf);
        break;
    case 5://盒子发送自己软硬件版本
        box_send_boxhwsoft_version(&buf[MX20340_REG_RX_DATA0], send_buf);
        break;
    case 6://盒子发送自己电量
        box_send_boxpower(&buf[MX20340_REG_RX_DATA0], send_buf);
        break;
    case 7://盒子获取耳机电量
        box_get_earpower(&buf[MX20340_REG_RX_DATA0], send_buf);
        break;
    case 8://按键，盖子，usb插拔事件
        box_send_boxevent(&buf[MX20340_REG_RX_DATA0], send_buf);
        break;
    case 9://升级
        box_update(&buf[MX20340_REG_RX_DATA0], send_buf);
        break;
    }
    max20340WriteRegister_withlen(handle, MX20340_REG_TX_DATA0, send_buf, 3);
    max20340WriteRegister(handle, MX20340_REG_PLC_CTL, buf[MX20340_REG_PLC_CTL] | 3);
/*    while(1){
        max20340ReadRegister(handle, MX20340_REG_PLC_STA, value_a);
        if (value_a[0] & 0x10){
            continue;
        }else{
            break;
        }
    }*/
}

static void recv_data_process_ear(bitserial_handle handle, uint8 *buf)
{
    uint8 ear;
    ear = (buf[MX20340_REG_RX_DATA0] & 0xc0) >> 6;
    switch(ear){
    case 1:
        if(used_i2c_addr == MAX20340_LEFTEAR_I2C_ADDR){
            recv_data_process_cmd(handle, buf);
        }
        break;
    case 2:
        if(used_i2c_addr == MAX20340_RIGHTEAR_I2C_ADDR){
            recv_data_process_cmd(handle, buf);
        }
        break;
    case 3:
        break;
    }
}
#endif

void singlebus_itr_process(void)
{
#if TEST_ITR
    uint8 i;
#endif
    uint8 value_a[0x13];
    bitserial_handle handle;
    handle = max20340Enable();

    max20340ReadRegister_withlen(handle, 0x0, value_a, 0x13);
#if TEST_ITR
    for(i=0; i<0x13; i++){
        DEBUG_LOG("reg 0x%x = 0x%x\n", i, value_a[i]);
    }
    DEBUG_LOG("over\n");
    max20340WriteRegister(handle, MX20340_REG_STA_MASK, 0x7f);
    max20340WriteRegister(handle, MX20340_REG_PLC_MASK, 0xff);
#endif

#if REAL_ITR
    if( (value_a[MX20340_REG_STA_IRQ]&0x1) ){
        if( ((value_a[MX20340_REG_STA1]&0x1c) == (5<<2)) ){
            //说明是插入动作,可能是芯片bug需要重写mask寄存器
            DEBUG_LOG("plc in\n");
        }else if( ((value_a[MX20340_REG_STA1]&0x1c) == (3<<2)) ){
            //说明是拔出动作,可能是芯片bug需要重写mask寄存器
            DEBUG_LOG("plc out\n");
        }
        //max20340WriteRegister(handle, MX20340_REG_STA_MASK, 0x2);
        max20340WriteRegister(handle, MX20340_REG_STA_MASK, 0x7f);
        max20340WriteRegister(handle, MX20340_REG_PLC_MASK, 0x0e);
        //max20340WriteRegister(handle, MX20340_REG_PLC_MASK, 0xff);
    }else if(value_a[MX20340_REG_PLC_IRQ] & 0x08){//总线接收数据出错，不做回应，master会重发
        ;
    }else if(value_a[MX20340_REG_PLC_IRQ] & 0x06){//总线接收到数据
        recv_data_process_ear(handle, value_a);
    }
#endif
    max20340Disable(handle);
}

void singlebus_itr_handler(Task task, MessageId id, Message msg)
{
    (void)task;
    unsigned long pin, pio_state;
    MessagePioChanged *pioMsg = (MessagePioChanged*)msg;
    pin = PIO2MASK(MAX20340_ITR_PIN);
    pio_state = pioMsg->state16to31 << 16 | pioMsg->state;

    switch(id) {
        case MESSAGE_PIO_CHANGED:
            if( !(pin&pio_state) ){
                singlebus_itr_process();
            }
            break;
        default:
            DEBUG_LOG("id=%d(0x%x\n", id, id);
            break;
    }
}

#ifdef MAX20340_TEST
void singlebus_key_itr_handler(Task task, MessageId id, Message msg)
{
    (void)task;
    uint8 i;
    uint8 value_a[0x13];
    bitserial_handle handle;
    unsigned long pin, pio_state;
    MessagePioChanged *pioMsg = (MessagePioChanged*)msg;
    pin = PIO2MASK(MAX20340_TEST_PIN);
    pio_state = pioMsg->state16to31 << 16 | pioMsg->state;
    DEBUG_LOG("max20340 itr happened pio_state = 0x%x\n", pio_state);
    switch(id) {
        case MESSAGE_PIO_CHANGED:
            if( !(pin&pio_state) ){
                handle = max20340Enable();
                max20340ReadRegister_withlen(handle, 0x0, value_a, 0x13);
                for(i=0; i<0x13; i++){
                    DEBUG_LOG("reg 0x%x = 0x%x\n", i, value_a[i]);
                }
                DEBUG_LOG("over\n");
                max20340Disable(handle);
            }
            break;
        default:
            DEBUG_LOG("id=%d(0x%x\n", id, id);
            break;
    }
}
#endif

#define TIME_READ_MAX20340_REG

typedef struct tagSHELLCMDINFO {
    TaskData       task;
    bool status;
}singlebus_funcInfoTask;
static singlebus_funcInfoTask *psbfuncTask = NULL;
#ifdef MAX20340_TEST
static singlebus_funcInfoTask *psbtest_funcTask = NULL;
#endif
#ifdef TIME_READ_MAX20340_REG
static singlebus_funcInfoTask *time_funcTask = NULL;
#endif

#ifdef TIME_READ_MAX20340_REG
#define MESSAGE_MAX20340_TIME_TRIGGER 1
static void max20340_time_handle_msg(Task task, MessageId id, Message message)
{
    (void)message;(void)task;
    switch (id)
    {
        case MESSAGE_MAX20340_TIME_TRIGGER:
            singlebus_itr_process();
            //DEBUG_LOG("lalalala\n");
            MessageSendLater(&time_funcTask->task,
                             MESSAGE_MAX20340_TIME_TRIGGER, NULL,
                             1000);
        break;
    }
}
#endif

int max20340_get_left_or_right(void)
{
    bitserial_handle handle;
    uint8 value;
    static uint8 ret_value = 0;
    if( (ret_value!=1) && (ret_value!=2) ){
        value = 0;
        used_i2c_addr = MAX20340_LEFTEAR_I2C_ADDR;
        handle = max20340Enable();
        max20340ReadRegister(handle, 0x00, &value);
        max20340ReadRegister(handle, 0x00, &value);
        max20340Disable(handle);
        if(value == 0x10){
            DEBUG_LOG("max20340 i2c addr = 0x%x\n",MAX20340_LEFTEAR_I2C_ADDR);
            ret_value = 1;
            return 1;
        }

        value = 0;
        used_i2c_addr = MAX20340_RIGHTEAR_I2C_ADDR;
        handle = max20340Enable();
        max20340ReadRegister(handle, 0x00, &value);
        max20340ReadRegister(handle, 0x00, &value);
        max20340Disable(handle);
        if(value == 0x10){
            DEBUG_LOG("max20340 i2c addr = 0x%x\n",MAX20340_RIGHTEAR_I2C_ADDR);
            ret_value = 2;
            return 2;
        }
        DEBUG_LOG("max20340 i2c addr error\n");
        return 0;
    }else{
        return ret_value;
    }
}

typedef struct{
    uint8 reg;
    uint8 value;
}max20340_str;
max20340_str max20340_init_array[] = {
    //{MX20340_REG_CTRL1, 0xe0},
    {MX20340_REG_CTRL1, 0xe1},
    //{MX20340_REG_CTRL2, 0xe1},
    {MX20340_REG_CTRL2, 0xe2},
    {MX20340_REG_CTRL3, 0xa4},
    {MX20340_REG_CTRL4, 0x0},
    //{MX20340_REG_STA_MASK, 0x2},
    {MX20340_REG_STA_MASK, 0x7f},
    {MX20340_REG_PLC_CTL, 0x94},
    //{MX20340_REG_PLC_CTL, 0x14},
    //{MX20340_REG_PLC_CTL, 0xa4},
    {MX20340_REG_PLC_MASK, 0x0e},
    //{MX20340_REG_PLC_MASK, 0xff},
};

int max20340_GetStatus(void)
{
    if(psbfuncTask && psbfuncTask->status == TRUE){
        return 0;
    }else{
        return -1;
    }
}

void max20340_init(void)
{
    bitserial_handle handle;
    uint8 value;uint8 value_a[13];
    uint8 i;
    if( max20340_get_left_or_right()==0 ){
        return;
    }
    value = 0;
    handle = max20340Enable();
    if(BITSERIAL_HANDLE_ERROR == handle) {
        return;
    }
    max20340ReadRegister(handle, 0x00, &value);
    max20340ReadRegister(handle, 0x00, &value);
    DEBUG_LOG("max20340 id = 0x%x\n", value);

    for(i=0; i<ARRAY_DIM(max20340_init_array); i++){
        max20340WriteRegister(handle, max20340_init_array[i].reg, max20340_init_array[i].value);
    }
    for(i=0; i<ARRAY_DIM(max20340_init_array); i++){
        max20340ReadRegister(handle, max20340_init_array[i].reg, &value);
        DEBUG_LOG("max20340 reg 0x%x = 0x%x\n", max20340_init_array[i].reg, value);
    }
    max20340ReadRegister_withlen(handle, 0x0, value_a, 0x13);//read clear itr

    psbfuncTask = PanicUnlessNew(singlebus_funcInfoTask);
    memset(psbfuncTask, 0, sizeof(singlebus_funcInfoTask));
    psbfuncTask->task.handler = singlebus_itr_handler;
    InputEventManagerRegisterTask(&psbfuncTask->task, MAX20340_ITR_PIN);
    psbfuncTask->status = TRUE;

#ifdef MAX20340_TEST
    psbtest_funcTask = PanicUnlessNew(singlebus_funcInfoTask);
    memset(psbtest_funcTask, 0, sizeof(singlebus_funcInfoTask));
    psbtest_funcTask->task.handler = singlebus_key_itr_handler;
    InputEventManagerRegisterTask(&psbtest_funcTask->task, MAX20340_TEST_PIN);
#endif

#ifdef TIME_READ_MAX20340_REG
    time_funcTask = PanicUnlessNew(singlebus_funcInfoTask);
    memset(time_funcTask, 0, sizeof(singlebus_funcInfoTask));
    time_funcTask->task.handler = max20340_time_handle_msg;
    MessageSendLater(&time_funcTask->task,
                     MESSAGE_MAX20340_TIME_TRIGGER, NULL,
                     6000);
#endif

    max20340Disable(handle);
    return;
}

#endif
