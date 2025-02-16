#ifndef TONE_PLAYER_H
#define TONE_PLAYER_H

#include "app_config.h"
#include "tone_player_api.h"

#define TONE_NUM_0      		SDFILE_RES_ROOT_PATH"tone/0.*"
#define TONE_NUM_1      		SDFILE_RES_ROOT_PATH"tone/1.*"
#define TONE_NUM_2				SDFILE_RES_ROOT_PATH"tone/2.*"
#define TONE_NUM_3				SDFILE_RES_ROOT_PATH"tone/3.*"
#define TONE_NUM_4				SDFILE_RES_ROOT_PATH"tone/4.*"
#define TONE_NUM_5				SDFILE_RES_ROOT_PATH"tone/5.*"
#define TONE_NUM_6				SDFILE_RES_ROOT_PATH"tone/6.*"
#define TONE_NUM_7				SDFILE_RES_ROOT_PATH"tone/7.*"
#define TONE_NUM_8				SDFILE_RES_ROOT_PATH"tone/8.*"
#define TONE_NUM_9				SDFILE_RES_ROOT_PATH"tone/9.*"
#define TONE_BT_MODE			SDFILE_RES_ROOT_PATH"tone/bt.*"
#define TONE_BT_CONN       		SDFILE_RES_ROOT_PATH"tone/connet.*"
#define TONE_BT_DISCONN    		SDFILE_RES_ROOT_PATH"tone/bt_dconn.*"
#define TONE_TWS_CONN			SDFILE_RES_ROOT_PATH"tone/tws_conn.*"
#define TONE_TWS_DISCONN		SDFILE_RES_ROOT_PATH"tone/tws_dconn.*"
#define TONE_LOW_POWER			SDFILE_RES_ROOT_PATH"tone/power_low.*"
#define TONE_POWER_OFF			SDFILE_RES_ROOT_PATH"tone/power_off.*"
#define TONE_POWER_ON			SDFILE_RES_ROOT_PATH"tone/power_on.*"
#define TONE_RING				SDFILE_RES_ROOT_PATH"tone/phone_in.*"
#define TONE_MAX_VOL			SDFILE_RES_ROOT_PATH"tone/max_vo.*"
#define TONE_ANC_OFF			SDFILE_RES_ROOT_PATH"tone/anc_off.*"
#define TONE_ANC_ON				SDFILE_RES_ROOT_PATH"tone/anc_on.*"
#define TONE_TRANSPARENCY		SDFILE_RES_ROOT_PATH"tone/anc_trans.*"
#define TONE_ANC_ADAPTIVE		SDFILE_RES_ROOT_PATH"tone/adaptive.*"
#define TONE_ANC_ADAPTIVE_COEFF	SDFILE_RES_ROOT_PATH"tone/adap_coeff.*"
#define TONE_ANC_NORMAL_COEFF	SDFILE_RES_ROOT_PATH"tone/nor_coeff.*"
#define TONE_EAR_CHECK			SDFILE_RES_ROOT_PATH"tone/ear_check.*"
#define TONE_MUSIC_MODE  		SDFILE_RES_ROOT_PATH"tone/music.*"
#define TONE_PC_MODE  	    	SDFILE_RES_ROOT_PATH"tone/pc.*"
#define TONE_SPKCHAT_OFF  		SDFILE_RES_ROOT_PATH"tone/spkchat_off.*"
#define TONE_SPKCHAT_ON  		SDFILE_RES_ROOT_PATH"tone/spkchat_on.*"
#define TONE_WCLICK_OFF  		SDFILE_RES_ROOT_PATH"tone/wclick_off.*"
#define TONE_WCLICK_ON  		SDFILE_RES_ROOT_PATH"tone/wclick_on.*"
#define TONE_WINDDET_OFF  		SDFILE_RES_ROOT_PATH"tone/winddet_off.*"
#define TONE_WINDDET_ON  		SDFILE_RES_ROOT_PATH"tone/winddet_on.*"

#define TONE_LOW_NS_ANC         SDFILE_RES_ROOT_PATH"tone/l_ns.*"
#define TONE_MID_NS_ANC         SDFILE_RES_ROOT_PATH"tone/m_ns.*"
#define TONE_H_NS_ANC           SDFILE_RES_ROOT_PATH"tone/h_ns.*"

#define TONE_PAIRING            SDFILE_RES_ROOT_PATH"tone/pairing.*"
#define TONE_ONE_DU             SDFILE_RES_ROOT_PATH"tone/du.*"
#define TONE_BIG_SOURCE         SDFILE_RES_ROOT_PATH"tone/max_pp.*"
#define TONE_NEW_LOW_POWER      SDFILE_RES_ROOT_PATH"tone/pp_low.*"
#define TONE_NEW_CONMING        SDFILE_RES_ROOT_PATH"tone/comming.*"
#define TONE_NEW_VOL_UP         SDFILE_RES_ROOT_PATH"tone/vol_up.*"
#define TONE_NEW_VOL_DOWN       SDFILE_RES_ROOT_PATH"tone/vol_dow.*"
#define TONE_NEW_DUT_MODE       SDFILE_RES_ROOT_PATH"tone/dut_m.*"


#define TONE_NEW_VOL_LEVEL_1    SDFILE_RES_ROOT_PATH"tone/nvol1.*"
#define TONE_NEW_VOL_LEVEL_2    SDFILE_RES_ROOT_PATH"tone/nvol2.*"
#define TONE_NEW_VOL_LEVEL_3    SDFILE_RES_ROOT_PATH"tone/nvol3.*"
#define TONE_NEW_VOL_LEVEL_4    SDFILE_RES_ROOT_PATH"tone/nvol4.*"

/*

#define TONE_LOW_POWER			SDFILE_RES_ROOT_PATH"tone/c_power_low.*"
#define TONE_POWER_OFF			SDFILE_RES_ROOT_PATH"tone/c_power_off.*"
#define TONE_POWER_ON			SDFILE_RES_ROOT_PATH"tone/c_power_on.*"
#define TONE_RING				SDFILE_RES_ROOT_PATH"tone/c_phone_in.*"
#define TONE_MAX_VOL			SDFILE_RES_ROOT_PATH"tone/c_max_vo.*"
c_connet        // 已连接
c_disconnet     // 断开连接
c_h_ns          // 强噪模式
c_l_ns          //  室内模式
c_m_ns          //  室外模式
c_max_vo        // 最大音量
c_pairing       //  配对中
c_phone_in      //  你有新的来电
c_power_low     //  电量低
c_power_off     //  关机
c_power_on      // 开机
*/

#define SINE_WTONE_NORAML           0
#define SINE_WTONE_TWS_CONNECT      1
#define SINE_WTONE_TWS_DISCONNECT   2
#define SINE_WTONE_LOW_POWER        4
#define SINE_WTONE_RING             5
#define SINE_WTONE_MAX_VOLUME       6
#define SINE_WTONE_ADSP				7
#define SINE_WTONE_LOW_LATENRY_IN   8
#define SINE_WTONE_LOW_LATENRY_OUT  9
#define SINE_WTONE_ANC_ADAPTIVE_1   10
#define SINE_WTONE_ANC_ADAPTIVE_2   11

#if CONFIG_USE_DEFAULT_SINE
#undef TONE_TWS_CONN
#define TONE_TWS_CONN           DEFAULT_SINE_TONE(SINE_WTONE_TWS_CONNECT)
#undef TONE_TWS_DISCONN
#define TONE_TWS_DISCONN        DEFAULT_SINE_TONE(SINE_WTONE_TWS_DISCONNECT)

#undef TONE_MAX_VOL
#define TONE_MAX_VOL            DEFAULT_SINE_TONE(SINE_WTONE_MAX_VOLUME)

#undef TONE_NORMAL
#define TONE_NORMAL            DEFAULT_SINE_TONE(SINE_WTONE_NORAML)
#if BT_PHONE_NUMBER

#else
#undef TONE_RING
#define TONE_RING               DEFAULT_SINE_TONE(SINE_WTONE_RING)
#endif

#undef TONE_LOW_POWER
#define TONE_LOW_POWER          DEFAULT_SINE_TONE(SINE_WTONE_LOW_POWER)
#endif

#define TONE_SIN_NORMAL			DEFAULT_SINE_TONE(SINE_WTONE_NORAML)

#define TONE_LOW_LATENCY_IN     DEFAULT_SINE_TONE(SINE_WTONE_LOW_LATENRY_IN)
#define TONE_LOW_LATENCY_OUT    DEFAULT_SINE_TONE(SINE_WTONE_LOW_LATENRY_OUT)

#define TONE_ANC_ADAPTIVE_LVL1    DEFAULT_SINE_TONE(SINE_WTONE_ANC_ADAPTIVE_1)
#define TONE_ANC_ADAPTIVE_LVL2    DEFAULT_SINE_TONE(SINE_WTONE_ANC_ADAPTIVE_2)


enum {
    IDEX_TONE_NUM_0,
    IDEX_TONE_NUM_1,
    IDEX_TONE_NUM_2,
    IDEX_TONE_NUM_3,
    IDEX_TONE_NUM_4,
    IDEX_TONE_NUM_5,
    IDEX_TONE_NUM_6,
    IDEX_TONE_NUM_7,
    IDEX_TONE_NUM_8,
    IDEX_TONE_NUM_9,
    IDEX_TONE_BT_MODE,
    IDEX_TONE_BT_CONN,
    IDEX_TONE_BT_DISCONN,
    IDEX_TONE_TWS_CONN,
    IDEX_TONE_TWS_DISCONN,
    IDEX_TONE_LOW_POWER,
    IDEX_TONE_POWER_OFF,
    IDEX_TONE_POWER_ON,
    IDEX_TONE_RING,
    IDEX_TONE_MAX_VOL,
    IDEX_TONE_NORMAL,
    IDEX_TONE_ANC_OFF,
    IDEX_TONE_ANC_ON,
    IDEX_TONE_TRANSPARCNCY,
    IDEX_TONE_ANC_ADAPTIVE,
    IDEX_TONE_ANC_ADAPTIVE_COEFF,
    IDEX_TONE_ANC_NORMAL_COEFF,
    IDEX_TONE_HEARING_AID_LOW_NS,
    IDEX_TONE_HEARING_AID_MID_NS,
    IDEX_TONE_HEARING_AID_HIGH_NS,
    IDEX_TONE_PAIRING,
    IDEX_TONE_DU,               // 嘟一声
    IDEX_BIG_SOUR,              // 新最大音量
    IDEX_NEW_LOW_POWER,         // 新低电
    IDEX_NEW_COMMING,           // 新来电
    IDEX_NEW_VOL_UP,            // 新音量+
    IDEX_NEW_VOL_DOWN,          // 新音量-
    IDEX_NEW_DUT_MODE,          // DUT

    IDEX_NEW_VOL_LEVEL_1,       // 音量等级1
    IDEX_NEW_VOL_LEVEL_2,       // 音量等级2
    IDEX_NEW_VOL_LEVEL_3,       // 音量等级3
    IDEX_NEW_VOL_LEVEL_4,       // 音量等级4


    IDEX_TONE_NONE = 0xFF,
};



int tone_play_init(void);
int tone_play_index(u8 index, u8 preemption);
int tone_play_index_with_callback(u8 index, u8 preemption, void (*user_evt_handler)(void *priv), void *priv);
int tone_play_index_no_tws(u8 index, u8 preemption);
/*
 *@brief:提示音比较，确认目标提示音和正在播放的提示音是否一致
 *@return: 0 	匹配
 *		   非0	不匹配或者当前没有提示音播放
 *@note:通过提示音索引比较
 */
int tone_name_cmp_by_index(u8 index);


// 按键提示音初始化
int audio_key_tone_init(void);
// 注销按键提示音播放
void audio_key_tone_destroy(void);

// 播放正弦波数组
int audio_key_tone_play_sin(struct audio_sin_param *sin, u8 sin_num, u8 preemption);
// 播放文件
int audio_key_tone_play_name(const char *name, u8 preemption);
// 按序号播放文件 // 使用key_tone_index[]数组
int audio_key_tone_play_index(u8 index, u8 preemption);
// 按键提示音播放 // 默认播放数组中第一个
void audio_key_tone_play(void);

// 检测按键提示音是否在播放
int audio_key_tone_is_play(void);

// 设置按键提示音的音量
void audio_key_tone_digvol_set(u8 volume);

#endif


