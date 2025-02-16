#ifndef E9127E28_9157_441B_BB45_13D4C8A85127
#define E9127E28_9157_441B_BB45_13D4C8A85127
#ifndef __LDOIN_COMMU__
#define __LDOIN_COMMU__

#define USER_CHARGESTORE_COMM_EN    1

#define BOX_LOW_GOTO_POWER_OFF      0

void user_comm_init(void);

void ldoin_isr_user_comm_deal(u8 index, u8 gpio, u8 edge);



#endif


#endif /* E9127E28_9157_441B_BB45_13D4C8A85127 */
