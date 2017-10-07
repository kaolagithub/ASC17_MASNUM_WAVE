#include <simd.h>
#include <slave.h>

__thread_local volatile unsigned long put_reply;
__thread_local volatile double data[4];

#define src_id 10

void slave_bcast(const int my_id, const int src, doublev4 *v4_bcast)
{
	int src_col = src % 8;
	int src_row = src / 8;
	int dst_col = 0x08;  // 8 = 本列进行广播
	int dst_row = 0x08;  // 8 = 本行进行广播
	
	if (my_id / 8 == src_row) // 将 src 的数据广播到 src 所在的列，其余列的广播其实是没有用的
	{
		simd_putc(*v4_bcast, dst_col);
	} else {
		*v4_bcast = simd_getc(*v4_bcast);
	}
	
	if (my_id % 8 == src_col) // 每一行的第 src_col 个从核已经拿到 src 的数据，广播到行内其他从核
	{
		simd_putr(*v4_bcast, dst_row);
	} else {
		*v4_bcast = simd_getr(*v4_bcast);
	}
}

void slave_p2p(const int my_id, const int src, const int dst, doublev4 *v4_p2p)
{
	if (src == dst) return;
	
	int src_col = src % 8;
	int dst_col = dst % 8;
	int dst_row = dst / 8;
	int my_col  = my_id % 8;
	int my_row  = my_id / 8;
	doublev4 tmp;  // 用于中转，避免污染中转从核的数据
	
	
	// 将数据发送到与 src 在同一列、与 dst 在同一行的中转从核上
	if (my_id == src)
		simd_putc(*v4_p2p, dst_row);
	if ((my_col == src_col) && (my_row == dst_row))
		tmp = simd_getc(tmp);
	
	if (dst_col == src_col) 
	{
		if (my_id == dst) *v4_p2p = tmp;
		return;
	}
	
	// 将数据从与 src 在同一列、与 dst 在同一行的中转从核上发送到 dst
	if ((my_col == src_col) && (my_row == dst_row))
		simd_putr(tmp, dst_col);
	if (my_id == dst)
		*v4_p2p = simd_getr(*v4_p2p);
}

void broadcast(void *_host_res)
{
	int my_id = athread_get_id(-1);
	double *host_res = (double*)(_host_res);
	
	data[0] = data[1] = data[2] = data[3] = my_id;
	volatile doublev4 *v4_bcast = (doublev4*) &data[0];
	
	//slave_bcast(my_id, src_id, v4_bcast);
	
	for (int i = 0; i < 64; i++)
	{
		slave_p2p(my_id, src_id, i, v4_bcast);
		athread_syn(ARRAY_SCOPE, 0xFFFF);
		if (my_id == 0) printf("P2P %d --> %d\n", src_id, i);
	}
	
	put_reply = 0;
	athread_put(PE_MODE, &data[0], &host_res[4 * my_id], sizeof(double) * 4, (void*)(&put_reply), 0, 0);
	while (put_reply != 1);
}