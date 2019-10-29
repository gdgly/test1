#ifndef APOLLO_H
#define APOLLO_H

void apollo_int_io_init(void);
void apollo_read_sw_version(void);
void apollo_upgrade(void);
void apollo_init(void);
void apollo_check_wakeup(void);

typedef struct
{
    TaskData               data;
    Source      data_source_sco;
    Source      data_source_mic;
#ifndef GAIA_TEST
    unsigned int msg_cnt_sco;
    unsigned int msg_cnt_mic;
#endif
}ApolloTaskData;

#endif // APOLLO_H
