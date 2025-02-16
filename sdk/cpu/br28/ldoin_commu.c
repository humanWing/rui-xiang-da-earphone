#include "app_config.h"
#include "system/includes.h"
#include "ldoin_commu.h"
#include "app_main.h"

#include "init.h"
#include "asm/charge.h"
#include "user_cfg.h"
#include "app_chargestore.h"
#include "device/vm.h"
#include "btstack/avctp_user.h"
#include "app_power_manage.h"
#include "app_action.h"
#include "app_charge.h"
#include "app_testbox.h"
#include "classic/tws_api.h"
#include "update.h"
#include "bt_ble.h"
#include "bt_tws.h"
#include "app_action.h"
#include "bt_common.h"
#include "le_rcsp_adv_module.h"
#include "tone_player.h"
#include "app_msg.h"
#include "key_event_deal.h"

#ifdef CONFIG_DEBUG_LITE_ENABLE
#include "debug_lite.h"
#define ldo_comm_printf printf_lite
#else
#define ldo_comm_printf printf
#endif

/**
 * 起始帧{a=100ms,b=100ms}，1是{d=20ms,f=20ms}，0是{c=20ms,e=20ms,g=20ms}，结束帧h=300ms
 * <-a-><b><c d e f g><-h-> 有效数据是cdef，g固定为0
 *      ___   _   _   ____
 *     |   | | | | | |    |
 * ____|   |_| |_| |_|    |
 */

/* 
    weak A（80ms L）+ guard B（>= 25ms H） +  s （5ms L） +数据  
    
    如果5ms后为上升沿：0   
    如果10ms后为上升沿：1

 */
#if USER_CHARGESTORE_COMM_EN
#define TIMER_CNT_UNIT      85  //us
#define TIMER_OUT_MS        500 //ms

#define MS_TO_CNT(ms, offset)  (((ms*1000) + (offset*1000))/TIMER_CNT_UNIT)

static volatile s32 low_level = 0;                   //低电平时间
static volatile s32 high_level = 0;                  //高电平时间
static volatile u8 get_leader = 0;                   //获取到起始帧标志
static volatile u8 comm_timeout = 1;                 //通信超时标志
static volatile u8 data_byte = 0x00;                 //通信数据暂存变量
static volatile u8 bit_cnt = 0;                      //当前获取到的bit数
static volatile u8 user_comm_data_buff[16] = {0};    //当前获取到的数据缓存
static volatile u8 user_comm_data_len = 0;           //当前获取到的数据缓存长度
static volatile u8 prev_edge = 0xff;                    

/**
 * @brief 定时器中断函数
 *  
 */
___interrupt
void user_comm_timeout(void)
{
    JL_TIMER3->CON |= BIT(14);
    comm_timeout = 1;
    get_leader = 0;
    JL_TIMER3->CON &= ~BIT(0);
    ldo_comm_printf("user_comm_timeout\n");
}

/**
 * @brief 通信定时器初始化
 * 
 */
void user_comm_init(void)
{
    ldo_comm_printf("user_comm_init\n");
    low_level = 0;
    high_level = 0;
    get_leader = 0;
    comm_timeout = 1;
    bit_cnt = 0;
    JL_TIMER3->CON = 0;
    bit_clr_ie(IRQ_TIME3_IDX);
    request_irq(IRQ_TIME3_IDX, 5, user_comm_timeout, 0);
    JL_TIMER3->CNT = 0;
    JL_TIMER3->PRD = TIMER_OUT_MS*1000/TIMER_CNT_UNIT + 1;
    JL_TIMER3->CON |= BIT(14);
    JL_TIMER3->CON = ((6 << 10) | (13 << 4));
}

/**
 * @brief 通信定时器重启
 * 
 */
void user_comm_restart(void)
{
    JL_TIMER3->CON = 0;
    JL_TIMER3->CNT = 0;
    JL_TIMER3->PRD = TIMER_OUT_MS*1000/TIMER_CNT_UNIT + 1;
    JL_TIMER3->CON |= BIT(14);
    JL_TIMER3->CON = ((6 << 10) | (13 << 4) | BIT(0));
}

/**
 * @brief 通信状态复位
 * 
 */
void user_comm_reset(void)
{
    low_level = 0;
    high_level = 0;
    get_leader = 0;
    comm_timeout = 0;
    bit_cnt = 0;
    JL_TIMER3->CON = 0;
    JL_TIMER3->CNT = 0;
    JL_TIMER3->PRD = TIMER_OUT_MS*1000/TIMER_CNT_UNIT + 1;
    JL_TIMER3->CON |= BIT(14);
    JL_TIMER3->CON = ((6 << 10) | (13 << 4) | BIT(0));
}

/**
 * @brief 通信完成
 * 
 */
void ldoin_comm_end_do(void)
{
    ldo_comm_printf("V\n");
    if(user_comm_data_len){
        put_buf(user_comm_data_buff, user_comm_data_len);
    }
}

void extern_send_cover_status(u8 state);
/*
 * 收数处理demo
 */

#define BOX_CMD_RESTORE_SYS     0x6f
int shutdown_timer = 0;
static u8 box_local_packet[36];

u8 factory_pro_flag = 0;

static void user_chargestore_shutdown_do(void *priv)
{
    ldo_comm_printf("chargestore shutdown!\n");
    power_set_soft_poweroff();
}

int user_chargestore_event_handler(struct chargestore_event *chargestore_dev)
{
    int ret = false;
    struct application *app = get_current_app();
#if defined(TCFG_CHARGE_KEEP_UPDATA) && TCFG_CHARGE_KEEP_UPDATA
    //在升级过程中,不响应智能充电舱app层消息
    if (dual_bank_update_exist_flag_get() || classic_update_task_exist_flag_get()) {
        return ret;
    }
#endif

    switch (chargestore_dev->event) {
    case 1:
        ldo_comm_printf("clear all info\n");
        if (app && strcmp(app->name, APP_NAME_BT)/*  && (app_var.goto_poweroff_flag == 0) */)
        {
            factory_pro_flag = 1;
            power_set_mode(TCFG_LOWPOWER_POWER_SEL);
            task_switch_to_bt();
        }
// #if TCFG_USER_TWS_ENABLE
//         bt_tws_remove_pairs();
// #endif
//         user_send_cmd_prepare(USER_CTRL_DEL_ALL_REMOTE_INFO, 0, NULL);
//         cpu_reset();
        break;
//     case 2:
// #if TCFG_USER_TWS_ENABLE
//         chargestore_set_tws_channel_info(__this->channel);
// #endif
//         break;

    case 3:
        if (!shutdown_timer)
        {
            shutdown_timer = sys_timer_add(NULL, user_chargestore_shutdown_do, 1000);
        }
        else
        {
            sys_timer_modify(shutdown_timer, 1000);
        }
        break;
    default:
        break;
    }

    return ret;
}

static void box_chargestore_event_to_user(u8 *packet, u32 type, u8 event, u8 size)
{
    struct sys_event e;
    e.type = SYS_DEVICE_EVENT;
    if (packet != NULL) {
        if (size > sizeof(box_local_packet)) {
            return;
        }
        e.u.chargestore.packet = box_local_packet;
        memcpy(e.u.chargestore.packet, packet, size);
    }
    e.arg  = (void *)type;
    e.u.chargestore.event = event;
    e.u.chargestore.size = size;
    sys_event_notify(&e);
}

void extern_send_cover_status(u8 state)
{
    switch (state)
    {
    case BOX_CMD_RESTORE_SYS:
        ldo_comm_printf("clear info\n");
        box_chargestore_event_to_user(NULL, DEVICE_EVENT_CHARGE_STORE, 1, 0);
        break;
    case 0x62:
        ldo_comm_printf("tws pair\n");
        // box_chargestore_event_to_user(NULL, DEVICE_EVENT_CHARGE_STORE, 2, 0);
        break;
    case 0xB0:
        ldo_comm_printf("close box 5v\n");
        box_chargestore_event_to_user(NULL, DEVICE_EVENT_CHARGE_STORE, 3, 0);
        break;
    
    default:
        break;
    }
}

volatile u8 level_table_cnt[20] = {0};

void ldoin_data_cap(u8 edge)
{
    if (get_leader == 0
        && high_level > MS_TO_CNT(25, -5)
        && high_level < MS_TO_CNT(25, 25)
        && low_level > MS_TO_CNT(80, -10)
        && low_level < MS_TO_CNT(80, 20))
    {
        get_leader = 1; //comm begin;
        user_comm_data_len = 0;
        bit_cnt = 0;
        data_byte = 0;
        high_level = 0;
        low_level = 0;
        // ldo_comm_printf("comm get start\n");
    }
    else if (get_leader)
    {
        if (edge == FALLING_EDGE)
        {
            if (high_level > MS_TO_CNT(10, -2))
            {   // 
                level_table_cnt[bit_cnt] = 1;
                level_table_cnt[bit_cnt + 1] = 1;
                bit_cnt += 2;
            }
            else
            {
                level_table_cnt[bit_cnt] = 1;
                bit_cnt += 1;
            }

            if (bit_cnt >= 17)
            {
                for (u8 i = 1; i < 17; )
                {
                    data_byte >>= 1;
                    if (level_table_cnt[i] == 0)
                    {
                        if (level_table_cnt[i + 1] == 0)
                        {   // 0 0
                            goto __err;
                        }
                        else
                        {   // 0 1
                            data_byte |= 0x80;
                        }
                    }
                    else
                    {
                        if (level_table_cnt[i + 1] == 0)
                        {   // 1 0
                            data_byte |= 0x00;
                        }
                        else
                        {   // 1 1
                            goto __err;
                        }
                    }

                    i += 2;
                }
                get_leader = 0;

                // for (u8 i = 1; i < 17; i++)
                // {
                //     ldo_comm_printf("[%d]%d\n", i, level_table_cnt[i]);
                // }
                ldo_comm_printf("box cmd 0x%x\n", data_byte);
                extern_send_cover_status(data_byte);
            }
        }
        else if (edge == RISING_EDGE)
        {
            if (low_level > MS_TO_CNT(10, -2))
            {   // 
                level_table_cnt[bit_cnt] = 0;
                level_table_cnt[bit_cnt + 1] = 0;
                bit_cnt += 2;
            }
            else
            {
                level_table_cnt[bit_cnt] = 0;
                bit_cnt += 1;
            }

            if (bit_cnt >= 17)
            {
                for (u8 i = 1; i < 17; )
                {
                    data_byte >>= 1;

                    if (level_table_cnt[i] == 0)
                    {
                        if (level_table_cnt[i + 1] == 0)
                        {   // 0 0
                            goto __err;
                        }
                        else
                        {   // 0 1
                            data_byte |= 0x80;
                        }
                    }
                    else
                    {
                        if (level_table_cnt[i + 1] == 0)
                        {   // 1 0
                            data_byte |= 0x00;
                        }
                        else
                        {   // 1 1
                            goto __err;
                        }
                    }


                    // ldo_comm_printf("[%d] %x", i, data_byte);

                    i += 2;
                }
                get_leader = 0;
                // for (u8 i = 1; i < 17; i++)
                // {
                //     ldo_comm_printf("[%d]%d\n", i, level_table_cnt[i]);
                // }
                ldo_comm_printf("box cmd 0x%x\n", data_byte);
                extern_send_cover_status(data_byte);
            }
        }
        return;
__err:
        ldo_comm_printf("comm maybe err\n");
        get_leader = 0;
        return;
    }
}

/**
 * @brief 电平跳变中断处理
 * 
 * @param index 唤醒源序号
 * @param gpio 唤醒源所在IO
 * @param edge 唤醒沿
 */
void ldoin_isr_user_comm_deal(u8 index, u8 gpio, u8 edge)
{
    if(gpio == IO_LDOIN_DET){
        if(comm_timeout || prev_edge == 0xff){
            prev_edge = edge;
            user_comm_reset();
            return;
        }
        else if(edge == FALLING_EDGE){
            if(prev_edge == RISING_EDGE){
                high_level = JL_TIMER3->CNT;
                ldo_comm_printf("h:%d\n", high_level);
            }
            user_comm_restart();
        }
        else if(edge == RISING_EDGE){
            if(prev_edge == FALLING_EDGE){
                low_level = JL_TIMER3->CNT;
                ldo_comm_printf("l:%d\n", low_level);
            }
            user_comm_restart();
        }
        prev_edge = edge;
        ldoin_data_cap(edge);
    }
    else{
        // ldo_comm_printf("gpio %d edge %d\n", gpio, edge);
    }
}

/**
 * @brief 获取通信忙状态
 * 
 * @return u8 
 */
u8 chargestore_get_comm_busy_flag(void)
{
    return get_leader;
}

#else
u8 chargestore_get_comm_busy_flag(void)
{
    return 0;
}

void user_comm_init(void)
{

}

void ldoin_isr_user_comm_deal(u8 index, u8 gpio, u8 edge)
{

}

#endif

