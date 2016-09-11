// G2k14s.h
// simple driver for 14 segment LED modules provided by Ghz2000
// only 1 instance G2k14s is declared and supported.
//
#ifndef __G2k14s_h__
#define __G2k14s_h__

#define G2K14S_COL_SIZE   8  // columns of an LED module
#define G2K14S_BUF_SIZE  16  // COL_SIZE * modules/system
#define G2K14S_LE		  9  // pin for latch-enable(RCK) of the LED modules
#define G2K14S_TIC		  2  // timer interval(ms) for dynamic lighting of LED
#define G2K14S_FLOW_MAX 128  // max characters of a text to flow

class G2k14s_class {

public:
	G2k14s_class();
	~G2k14s_class();
	void lamp_test( const int delay_time_ms );
	void char_test( const int delay_time_ms );
	void flow( const int delay_time_ms, const char c[] );

private:
	static uint8_t col;
	static uint16_t buf[ G2K14S_BUF_SIZE ];
	static void dyn();

	static const uint16_t ptn[];
	uint16_t asc2G2k14s( const char c );
};

extern G2k14s_class G2k14s;

#endif // __G2k14s_h__
