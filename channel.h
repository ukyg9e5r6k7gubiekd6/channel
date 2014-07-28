#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "channel_types.h"

static inline void net_channel_queue(net_channel_t *chan, uint32_t item) {
	uint16_t tail = chan->p.tail;
	uint16_t nxt = (tail + 1) % NET_CHANNEL_Q_ENTRIES;
	if (nxt != chan->c.head) {
		chan->q[tail] = item;
		STORE_BARRIER;
		chan->p.tail = nxt;
		if (chan->p.wakecnt != chan->c.wakecnt) {
			++chan->p.wakecnt;
			net_chan_wakeup(chan);
		}
	}
}

#endif /* CHANNEL_H_ */
