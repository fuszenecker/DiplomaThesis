#include <can.h>
#include <stm32f10x_can.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

CanRxMsg RxMessage;
CanTxMsg TxMessage;

#ifndef CAN_RETRIES
#define CAN_RETRIES 16
#endif

int CAN_init(unsigned int prescaler) {
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef  CAN_InitStructure;

    /* CAN register init */
    CAN_DeInit();
    CAN_StructInit(&CAN_InitStructure);

    /* CAN Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    /* Configure CAN pin: TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
#ifdef CAN_NO_AUTORETRANSMIT
    CAN_InitStructure.CAN_NART = ENABLE;
#else
#warning Use CAN_NO_AUTORETRANSMIT
    CAN_InitStructure.CAN_NART = DISABLE;
#endif
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
#ifdef CAN_LOOPBACK
    CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
#else
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
#endif
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
    CAN_InitStructure.CAN_Prescaler = prescaler & 0x3ff;
    CAN_Init(&CAN_InitStructure);

    return 0;
}

int CAN_set_prescaler(unsigned int prescaler) {
    CAN_InitTypeDef  CAN_InitStructure;

    /* CAN register init */
    CAN_StructInit(&CAN_InitStructure);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
#ifdef CAN_NO_AUTORETRANSMIT
    CAN_InitStructure.CAN_NART = ENABLE;
#else
#warning Use CAN_NO_AUTORETRANSMIT
    CAN_InitStructure.CAN_NART = DISABLE;
#endif
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
#ifdef CAN_LOOPBACK
    CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
#else
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
#endif
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
    CAN_InitStructure.CAN_Prescaler = prescaler & 0x3ff;
    CAN_Init(&CAN_InitStructure);

    return 0;
}

void CAN_set_filter(unsigned int filter, unsigned int mask, unsigned int id) {
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    /* ssss ssss sssE EEEE EEEE EEEE EEEE E___ */
    /*    E EEEE EEEE EEEE EEEE E */
    /*                           ssss ssss sss */

    // unsigned int new_id = ((id & 0x1ffff800) >> 8) | ((id & 0x7ff) << 21);
    // unsigned int new_mask = ((mask & 0x1ffff800) >> 8) | ((mask & 0x7ff) << 21);

    unsigned int new_id = id << 3;
    unsigned int new_mask = mask << 3;
    
    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = filter;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = new_id >> 16;
    CAN_FilterInitStructure.CAN_FilterIdLow = new_id & 0xffff;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = new_mask >> 16;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = new_mask & 0xffff;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}

int CAN_send(unsigned int id, unsigned int length, unsigned int data_h, unsigned int data_l) {
    unsigned char TransmitMailbox;
    unsigned int retval = -1, i = 0;

    /* transmit */
    TxMessage.ExtId = id;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_EXT;
    TxMessage.DLC = length;
    TxMessage.Data[0] = (data_h >> 24) & 0xff;
    TxMessage.Data[1] = (data_h >> 16) & 0xff;
    TxMessage.Data[2] = (data_h >> 8) & 0xff;
    TxMessage.Data[3] = (data_h >> 0) & 0xff;
    TxMessage.Data[4] = (data_l >> 24) & 0xff;
    TxMessage.Data[5] = (data_l >> 16) & 0xff;
    TxMessage.Data[6] = (data_l >> 8) & 0xff;
    TxMessage.Data[7] = (data_l >> 0) & 0xff;

    do {
        TransmitMailbox = CAN_Transmit(&TxMessage);
        
        if (TransmitMailbox != CAN_NO_MB)
            while ((retval = CAN_TransmitStatus(TransmitMailbox)) == CANTXPENDING);

        i++;
    } while ((retval != CANTXOK) && (i <= CAN_RETRIES));

    return retval;
}

CanRxMsg *CAN_recv() {
    /* receive */
    RxMessage.StdId = 0x00;
    RxMessage.IDE = CAN_ID_EXT;
    RxMessage.DLC = 0;
    RxMessage.Data[0] = 0x00;
    RxMessage.Data[1] = 0x00;
    RxMessage.Data[2] = 0x00;
    RxMessage.Data[3] = 0x00;
    RxMessage.Data[4] = 0x00;
    RxMessage.Data[5] = 0x00;
    RxMessage.Data[6] = 0x00;
    RxMessage.Data[7] = 0x00;

    CAN_Receive(CAN_FIFO0, &RxMessage);

    return &RxMessage;
}
 
