// G2k14s.cpp
// simple driver for 14 segment LED modules provided by Ghz2000
// some fonts are modified from the original.
// only 1 instance G2k14s is declared and supported.
//
#include <MsTimer2.h>
#include <SPI.h>
#include <G2k14s.h>

G2k14s_class G2k14s;

uint8_t G2k14s_class::col;  // 0:left .. 7:right
uint16_t G2k14s_class::buf[ G2K14S_BUF_SIZE ];
const uint16_t G2k14s_class::ptn[] = {
	0x0000, // SPACE
	0x0140, // !
	0x1100, // "
	0x1794, // #
	0x56B4, // $
	0x6C9A, // %
	0x2E29, // &
	0x0800, // '
	0x0808, // (
	0x2002, // )
	0x2C1A, // *
	0x1414, // +
	0x0002, // ,
	0x0410, // -
	0x0040, // .
	0x0802, // /

	0x4BA3, // 0
	0x0980, // 1
	0x0332, // 2
	0x03B0, // 3
	0x5414, // 4
	0x4628, // 5
	0x46B1, // 6
	0x4380, // 7
	0x47B1, // 8
	0x47B0, // 9
	0x0220, // :
	0x02A0, // ;
	0x0C08, // <
	0x0430, // =
	0x2012, // >
	0x4354, // ?

	0x07A5, // @
	0x4791, // A
	0x13B4, // B
	0x4221, // C
	0x13A4, // D
	0x4631, // E
	0x4611, // F
	0x42B1, // G
	0x4591, // H
	0x1224, // I
	0x01A1, // J
	0x4C09, // K
	0x4021, // L
	0x6981, // M
	0x6189, // N
	0x43A1, // O

	0x4711, // P
	0x43A9, // Q
	0x4719, // R
	0x46B0, // S
	0x1204, // T
	0x41A1, // U
	0x4803, // V
	0x418B, // W
	0x280A, // X
	0x2804, // Y
	0x0A22, // Z
	0x2222, // [
	0x2C14, // YEN
	0x0A28, // ]
	0x000A, // ^
	0x0020, // _

	0x2000, // `
	0x04A9, // a
	0x44B1, // b
	0x0431, // c
	0x05B1, // d
	0x0423, // e
	0x4601, // f
	0x23B0, // g
	0x4491, // h
	0x0880, // i
	0x08A0, // j
	0x4419, // k
	0x4001, // l
	0x0495, // m
	0x0491, // n
	0x04B1, // o

	0x4E01, // p
	0x2390, // q
	0x0411, // r
	0x0038, // s
	0x4421, // t
	0x00A1, // u
	0x0003, // v
	0x008B, // e
	0x041A, // x
	0x45B0, // y
	0x0422, // z
	0x2622, // {
	0x1004, // |
	0x0A38, // }
	0x0200, // ~
	0x7FFF, // DEL
};

G2k14s_class::G2k14s_class() {
	col = 0;
	for( int i = 0; i < G2K14S_BUF_SIZE; i++ ) {
		buf[ i ] = 0x0000;
	}
	pinMode( G2K14S_LE, OUTPUT );
	SPI.begin();
	MsTimer2::set( G2K14S_TIC, dyn );
	MsTimer2::start();
}

G2k14s_class::~G2k14s_class() {
	MsTimer2::stop();
	SPI.end();
}

void G2k14s_class::dyn() {
	SPI.beginTransaction( SPISettings() );

	digitalWrite( G2K14S_LE, LOW );

	uint8_t pos = ~( 0x80 >> col );
	SPI.transfer( pos );
	SPI.transfer16( buf[ col ] ); 
	SPI.transfer( pos );
	SPI.transfer16( buf[ col + 8 ] );

	digitalWrite( G2K14S_LE, HIGH );
	if( ++col > G2K14S_COL_SIZE ) { col = 0; }

	SPI.endTransaction();
}

uint16_t G2k14s_class::asc2G2k14s( const char c ) {
  	if( ( c >= 0x20 ) && ( c < 0x80 ) ) {
		return( ptn[ c - 0x20 ] );
  	}
  	else {
		return( 0x63E9 );  // special pattern
  	}
}

void G2k14s_class::flow( const int delay_time_ms, const char c[] ) {
	int flow_size = 0;
	while( ( flow_size < G2K14S_FLOW_MAX ) && ( c[ flow_size ] != '\0' ) ) {
		flow_size++;
	}
	for( int disp_p = - G2K14S_BUF_SIZE; disp_p < flow_size; disp_p++ ) {
		for( int buf_p = 0; buf_p < G2K14S_BUF_SIZE; buf_p++ ) {
			int flow_p = disp_p + buf_p;
			if( ( flow_p >= 0 ) && ( flow_p < flow_size ) ) {
				buf[ buf_p ] = asc2G2k14s( c[ flow_p ] );
			}
			else {
				buf[ buf_p ] = asc2G2k14s( ' ' );
			}
		}
	delay( delay_time_ms );
	}
}

void G2k14s_class::lamp_test( const int delay_time_ms ) {
	for( int j = 0; j < G2K14S_BUF_SIZE; j++ ) {
		for( int i = 0; i < G2K14S_BUF_SIZE; i++ ) {
			buf[ i ] = 0x8000 >> j;
		}
		delay( delay_time_ms );
	}
  	for( int j = 0; j < G2K14S_BUF_SIZE; j++ ) {
		for( int i = 0; i < G2K14S_BUF_SIZE; i++ ) {
			buf[ i ] = ~( 0x8000 >> j );
		}
		delay( delay_time_ms );
	}
}

void G2k14s_class::char_test( const int delay_time_ms ) {
	for( int i = 0; i < 16; i++ ) { buf[ i ] = asc2G2k14s( i + 0x00 ); }  delay( delay_time_ms );
	for( int i = 0; i < 16; i++ ) { buf[ i ] = asc2G2k14s( i + 0x20 ); }  delay( delay_time_ms );
	for( int i = 0; i < 16; i++ ) { buf[ i ] = asc2G2k14s( i + 0x30 ); }  delay( delay_time_ms );
	for( int i = 0; i < 16; i++ ) { buf[ i ] = asc2G2k14s( i + 0x40 ); }  delay( delay_time_ms );
	for( int i = 0; i < 16; i++ ) { buf[ i ] = asc2G2k14s( i + 0x50 ); }  delay( delay_time_ms );
	for( int i = 0; i < 16; i++ ) { buf[ i ] = asc2G2k14s( i + 0x60 ); }  delay( delay_time_ms );
	for( int i = 0; i < 16; i++ ) { buf[ i ] = asc2G2k14s( i + 0x70 ); }  delay( delay_time_ms );
	for( int i = 0; i < 16; i++ ) { buf[ i ] = asc2G2k14s( i + 0x80 ); }  delay( delay_time_ms );
}
