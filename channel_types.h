#include <inttypes.h>

#define CACHE_LINE_SIZE 64 /* FIXME: Use cache_line_size.h */
#define NET_CHANNEL_ENTRIES 512 /* approx number of entries in channel q */
#define NET_CHANNEL_Q_ENTRIES \
((ROUND_UP(NET_CHANNEL_ENTRIES * sizeof(uint32_t), CACHE_LINE_SIZE) \
- sizeof(net_channel_producer_t) - sizeof(net_channel_consumer_t)) \
/ sizeof(uint32_t))
#define CACHE_ALIGN __attribute__((aligned(CACHE_LINE_SIZE)))

typedef struct {
	uint16_t tail; /* next element to add */
	uint8_t wakecnt; /* do wakeup if != consumer wakecnt */
	uint8_t pad;
} net_channel_producer_t;

typedef struct {
	uint16_t head; /* next element to remove */
	uint8_t wakecnt; /* increment to request wakeup */
	uint8_t wake_type; /* how to wakeup consumer */
	void * wake_arg; /* opaque argument to wakeup routine */
} net_channel_consumer_t;

typedef struct {
	net_channel_producer_t p CACHE_ALIGN; /* producer's header */
	uint32_t q[NET_CHANNEL_Q_ENTRIES];
	net_channel_consumer_t c; /* consumer's header */
} net_channel_t;
