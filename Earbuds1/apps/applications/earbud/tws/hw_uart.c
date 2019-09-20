#include <stdio.h>
#include <sink.h>
#include <source.h>
#include <stream.h>
#include <operator.h>
#include <panic.h>
#include <pio.h>
#include <ps.h>
#include <string.h>
#include <os.h>
#include <message.h>
#include <vmal.h>
#include <pmalloc.h>
#include <app/message/system_message.h>

#include "public.h"


#ifdef CONFIG_HW_UART
#define CONFIG_BAUDRATE_115200          // 115200用来测试命令使用,不定义则使用3M波特率


#define SEND_ONE_LEN     (500)      /* 单次最大发送长度 */
#define MAX_DATA_LEN     (2000)     /* 最大缓冲数据长度 */
#define UART_PIO_BANK    (0)
#define UART_RX_TX_BIT   ((1<<UART_RX_PIO) | (1<<UART_TX_PIO))

typedef struct tagUARTTASKINFO {
    TaskData     task;
    UartRxProc   RxProc;
    Sink         uart_sink;
    Source       uart_source;

    signed WriteNow;               // 1, write Now
    signed lenWr, lenSend;
    uint8 data[MAX_DATA_LEN];

}UartTaskInfo;
UartTaskInfo theUartTask = {{NULL,},};

inline UartTaskInfo *GetUartTask(void);
inline UartTaskInfo *GetUartTask(void)
{
    return &theUartTask;
}

// sendMessageMoreSpace
static void sendMessageMoreSpace(UartTaskInfo *theUartTask)
{
    MessageMoreSpace *message = PanicUnlessMalloc(sizeof(MessageMoreSpace));
    message->sink = theUartTask->uart_sink;
    MessageCancelAll(&theUartTask->task, MESSAGE_MORE_SPACE);
    MessageSendLater(&theUartTask->task, MESSAGE_MORE_SPACE, message, 0);
}

void UartTxData(const uint8 *data, uint16 length)
{
    signed firstLess = 0, space;
    UartTaskInfo *theTask = GetUartTask();

    if(theTask->lenWr >= theTask->lenSend)
        space = MAX_DATA_LEN - (theTask->lenWr - theTask->lenSend);
    else
        space = theTask->lenSend - theTask->lenWr;

    if(space < length) {
        printf("ESPACE\n");
        goto out;
    }

    firstLess = MAX_DATA_LEN - theTask->lenWr;
    if(firstLess >= length) {
        memcpy(&theTask->data[theTask->lenWr], data, length);
        theTask->lenWr += length;
    }
    else {
        memcpy(&theTask->data[theTask->lenWr], data, firstLess);
        memcpy(&theTask->data[0], &data[firstLess], length - firstLess);

        theTask->lenWr = length - firstLess;
    }

out:
    if(theTask->WriteNow) {
//        printf("more\n");
        sendMessageMoreSpace(theTask);
        theTask->WriteNow = 0;
    }
}

static void handleUartTxData(Task pTask, Sink sink)
{
    signed datlen = 0, length, offset, firstData;
    uint8 *dest;
    UartTaskInfo *theTask = (UartTaskInfo*)pTask;
    (void)sink;

    if(theTask->lenWr == theTask->lenSend) {
        theTask->WriteNow = 1;
//        printf("Empty\n");
        return;
    }

    if(theTask->lenWr >= theTask->lenSend) {
        datlen = (theTask->lenWr - theTask->lenSend);
        firstData = datlen;
    }
    else {
        datlen = MAX_DATA_LEN - (theTask->lenSend - theTask->lenWr);
        firstData = MAX_DATA_LEN - theTask->lenSend;
    }

    if(datlen > SEND_ONE_LEN)
        length = SEND_ONE_LEN;
    else
        length = datlen;

    /* Claim space in the sink, getting the offset to it */
    offset = SinkClaim(theTask->uart_sink, length);
    if(offset == 0xFFFF)  {
        theTask->WriteNow = 1;
        printf("EC=%d\n", length);
        return ;
    }

    /* Map the sink into memory space */
    dest = (uint8*)SinkMap(theTask->uart_sink);
    PanicNull(dest);
    /* Copy data into the claimed space */
    if(firstData >= length) {
        memcpy(dest+offset, &theTask->data[theTask->lenSend], length);
        theTask->lenSend += length;
    }
    else {
        memcpy(dest+offset, &theTask->data[theTask->lenSend], firstData);
        memcpy(dest+offset+firstData, &theTask->data[0], length - firstData);

        theTask->lenSend = length - firstData;
    }
    /* Flush the data out to the uart */
    PanicZero(SinkFlush(theTask->uart_sink, length));
}

static void handleUartRxData(Task pTask, Source src)
{
    uint16 length = 0;
    const uint8 *data = NULL;
    UartTaskInfo *theTask = (UartTaskInfo*)pTask;

    /* Get the number of bytes in the specified source before the next packet  boundary */
    if(!(length = SourceBoundary(src)))
        return;

    /* Maps the specified source into the address map */
    data = SourceMap(src);
    PanicNull((void*)data);
    /* Transmit the received data */
    if(theTask->RxProc)
        theTask->RxProc(data, length);

    SourceDrop(src, length);
}

static void UARTStreamMessageHandler (Task pTask, MessageId pId, Message pMessage)
{
    (void)pTask;

    switch (pId)
    {
    case MESSAGE_MORE_DATA:
        handleUartRxData(pTask, ((MessageMoreData *)pMessage)->source);
        break;
    case MESSAGE_MORE_SPACE:
        handleUartTxData(pTask, ((MessageMoreSpace *)pMessage)->sink);
        break;
    default:
        break;
    }
}


static void UartIoMap(void)
{
    bool ret;

    PioSetMapPins32Bank(UART_PIO_BANK, UART_RX_TX_BIT, 0);
    ret = PioSetFunction(UART_RX_PIO, UART_RX);
    if(FALSE == ret) {
        printf("Error UartRX %d\n", UART_RX_PIO);
    }

    ret = PioSetFunction(UART_TX_PIO, UART_TX);
    if(FALSE == ret) {
        printf("Error UartTX %d\n", UART_TX_PIO);
    }
}


void uartInit(void)
{
    UartTaskInfo *theTask = GetUartTask();

    UartIoMap();
    theTask->task.handler = UARTStreamMessageHandler;


    /* Configure uart settings */
    //    StreamUartConfigure(VM_UART_RATE_460K8, VM_UART_STOP_ONE, VM_UART_PARITY_NONE);
    //    StreamUartConfigure(VM_UART_RATE_921K6, VM_UART_STOP_ONE, VM_UART_PARITY_NONE);
    //    StreamUartConfigure(VM_UART_RATE_921K6*2+850, VM_UART_STOP_ONE, VM_UART_PARITY_NONE);    // 2M实际输出
#ifndef CONFIG_BAUDRATE_115200
    StreamUartConfigure(12168+400, VM_UART_STOP_ONE, VM_UART_PARITY_NONE);    // 3M实际输出 ((3000000/9600) *39 + X)
#else
    StreamUartConfigure(VM_UART_RATE_115K2, VM_UART_STOP_ONE, VM_UART_PARITY_NONE);
#endif

    /* Get the sink for the uart */
    theTask->uart_sink = StreamUartSink();
    PanicNull(theTask->uart_sink);

    /* Get the source for the uart */
    theTask->uart_source = StreamUartSource();
    PanicNull(theTask->uart_source);

    //    theUARTStreamTask.data  = (uint8*)PanicUnlessMalloc(MAX_DATA_LEN);
    theTask->lenWr = theTask->lenSend = 0;
    theTask->WriteNow = 1;

    /* Register uart source with task */
    VmalMessageSinkTask(StreamSinkFromSource(theTask->uart_source), &theTask->task);
}

void UartSetRecvHandle(UartRxProc func)
{
    if(GetUartTask()->task.handler == NULL)
        uartInit();
    GetUartTask()->RxProc = func;
 }

#endif



