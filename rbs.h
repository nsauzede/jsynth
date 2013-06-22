#ifndef __RBS_H__
#define __RBS_H__

/* 
 * loosely based on :
 *
 * http://www.sonicspot.com/guide/rbsfiles.html
 *
 * should be based on RIP, so called :
 *
 * ftp://ftp.propellerheads.se/pub/devzone/RBS42.txt
 *
 */

#include <inttypes.h>

#pragma pack(1)

typedef struct {
/*0x00 	4*/ 	char chunk_id[4];			// Chunk ID 	"HEAD" (0x48454144)
/*0x04 	4*/ 	uint32_t chunk_data_size;	// Chunk Data Size 	256
/*0x08 	9*/ 	char version[9];			// Version 	0x5b545b54bc04020000
/*0x11 	129*/ 	char copyright[129];		// Copyright 	"(c)1997 Propellerhead Software, all rights reserved"
/*0x92 	118*/ 	uint8_t reserved[118];		// Reserved 	all zeros
} head_t;

typedef struct {
							/*Offset 	Size 	Description 	Value*/
char chunk_id[4];			/*0x00 	4 	Chunk ID 	"GLOB" (0x474C4F42)*/
uint32_t chunk_data_size;	/*0x04 	4 	Chunk Data Size 	512*/
uint8_t play_mode;			/*0x08 	1 	Play Mode 	0x00 = pattern, 0x01 = song*/
uint8_t loop;				/*0x09 	1 	Loop 	0x00 = Off, 0x01 = On (only in song mode)*/
uint32_t tempo;				/*0x0A 	4 	Tempo 	BPM * 1000*/
uint32_t loop_start;		/*0x0E 	4 	Loop Start 	Bar * 768 (only in song mode)*/
uint32_t loop_end;			/*0x12 	4 	Loop End 	Bar * 768 (only in song mode)*/
uint8_t shuffle_amount;		/*0x16 	1 	Shuffle Amount 	0x00 - 0x7F*/
char mod_name[65];			/*0x17 	65 	Name of mod 	standard mod = "Standard ReBirth"*/
char mod_ftp[201];			/*0x58 	201 	FTP for mod 	standard mod = "ftp.propellerheads.se"*/
char mod_www[201];			/*0x121 	201 	WWW for mod 	standard mod = "www.propellerheads.se"*/
uint8_t vintage_mode;		/*0x1EA 	1 	Vintage Mode 	0x00 = ReBirth 2.0 sound, 0x01 = Vintage ReBirth sound*/
uint8_t reserved[29];		/*0x1EB 	29 	Reserved 	all zeros*/
} glob_t;

typedef struct {
							/*Offset 	Size 	Description 	Value*/
char chunk_id[4];			/*0x00 	4 	Chunk ID 	"USRI" (0x55535249)*/
uint32_t chunk_data_size;	/*0x04 	4 	Chunk Data Size 	512*/
char window_title[41];		/*0x08 	41 	Window Title 	text added to window title*/
char song_information[401];	/*0x31 	201 	Song Information 	text in song information dialog (0x0D is newline)*/
char web_page[101];			/*0xFA 	101 	Web Page 	url-address in song information dialog*/
uint8_t show_song_info;		/*0x15F 	1 	Show Song Info On Open 	0x00 = no, 0x01 = yes*/
uint8_t reserved[168];		/*0x160 	168 	Reserved 	all zeros*/
} usri_t;

typedef struct mixr {
							/*Offset 	Size 	Description 	Value*/
char chunk_id[4];			/*0x00 	4 	Chunk ID 	"MIXR" (0x4D495852)*/
uint32_t chunk_data_size;	/*0x04 	4 	Chunk Data Size 	64*/
uint8_t master_fader_level;	/*0x08 	1 	Master fader level 	0x00 - 0x7F*/
uint8_t compressor_id;		/*0x09 	1 	Compressor Device ID 	0x00 = none*/
								/*0x01 = master*/
								/*0x02 = tb303 1*/
								/*0x03 = tb303 2*/
								/*0x04 = tr808*/
								/*0x05 = tr909*/
uint8_t pcf_id;				/*0x0A 	1 	PCF Device ID 	0x00 = off*/
								/*0x02 = tb303 1*/
								/*0x03 = tb303 2*/
								/*0x04 = tr808*/
								/*0x05 = tr909*/
uint8_t reserved0[13];		/*0x0B 	13 	Reserved 	all zeros*/
uint8_t mix_enabled_3a;		/*0x18 	1 	TB303 1 Mix Enabled 	0x00 = off*/
								/*0x01 = on*/
uint8_t mix_level_3a;		/*0x19 	1 	TB303 1 Mix Level 	0x00 - 0x7F*/
uint8_t mix_pan_3a;			/*0x1A 	1 	TB303 1 Mix Pan (left - center - right) 	0x00 - 0x40 - 0x7F*/
uint8_t delay_send_amt_3a;	/*0x1B 	1 	TB303 1 Delay Send Amount 	0x00 - 0x7F*/
uint8_t disto_enabled_3a;	/*0x1C 	1 	TB303 1 Distortion Enabled 	0x00 = off*/
								/*0x01 = on*/
uint8_t reserved1[7];		/*0x1D 	7 	Reserved 	all zeros*/
uint8_t mix_enabled_3b;		/*0x24 	1 	TB303 2 Mix Enabled 	0x00 = off*/
								/*0x01 = on*/
uint8_t mix_level_3b;		/*0x25 	1 	TB303 2 Mix Level 	0x00 - 0x7F*/
uint8_t mix_pan_3b;			/*0x26 	1 	TB303 2 Mix Pan (left - center - right) 	0x00 - 0x40 - 0x7F*/
uint8_t delay_send_amt_3b;	/*0x27 	1 	TB303 2 Delay Send Amount 	0x00 - 0x7F*/
uint8_t disto_enabled_3b;	/*0x28 	1 	TB303 2 Distortion Enabled 	0x00 = off*/
								/*0x01 = on*/
uint8_t reserved2[7];		/*0x29 	7 	Reserved 	all zeros*/
uint8_t mix_enabled_8;		/*0x30 	1 	TB808 Mix Enabled 	0x00 = off*/
								/*0x01 = on*/
uint8_t mix_level_8;		/*0x31 	1 	TB808 Mix Level 	0x00 - 0x7F*/
uint8_t mix_pan_8;			/*0x32 	1 	TB808 Mix Pan (left - center - right) 	0x00 - 0x40 - 0x7F*/
uint8_t delay_send_amt_8;	/*0x33 	1 	TB808 Delay Send Amount 	0x00 - 0x7F*/
uint8_t disto_enabled_8;		/*0x34 	1 	TB808 Distortion Enabled 	0x00 = off*/
								/*0x01 = on*/
uint8_t reserved3[7];		/*0x35 	7 	Reserved 	all zeros*/
uint8_t mix_enabled_9;	/*0x3C 	1 	TB909 Mix Enabled 	0x00 = off*/
								/*0x01 = on*/
uint8_t mix_level_9;	/*0x3D 	1 	TB909 Mix Level 	0x00 - 0x7F*/
uint8_t mix_pan_9;	/*0x3E 	1 	TB909 Mix Pan (left - center - right) 	0x00 - 0x40 - 0x7F*/
uint8_t delay_send_amt_9;	/*0x3F 	1 	TB909 Delay Send Amount 	0x00 - 0x7F*/
uint8_t disto_enabled_9;	/*0x40 	1 	TB909 Distortion Enabled 	0x00 = off*/
								/*0x01 = on*/
uint8_t reserved4[7];		/*0x41 	7 	Reserved 	all zeros*/
} mixr_t;

typedef struct {
							/*Offset 	Size 	Description 	Value*/
char chunk_id[4];			/*0x00 	4 	Chunk ID 	"CAT " (0x43415420)*/
uint32_t chunk_data_size;	/*0x04 	4 	Chunk Data Size 	(file size) - 8*/
} chunk_t;

typedef struct {
#if 1
							/*Offset 	Size 	Description 	Value*/
char chunk_id[4];			/*0x00 	4 	Chunk ID 	"CAT " (0x43415420)*/
uint32_t chunk_data_size;	/*0x04 	4 	Chunk Data Size 	(file size) - 8*/
char iff_type[4];			/*0x08 	4 	IFF Type 	"RB40" (0x52423430)*/
/*0x10 	data chunks*/
#else
// CAT 
	chunk_t chunk;			// CAT , size ??
//  RB40
	char iff_type[4];		// RB40
//   HEAD, 256
//   GLOB, 512
//   USRI, 712
//   CAT , 
//    DEVL
//     MIXR, 64
//     DELY, 8
//     PCF, 
//     DIST
//     COMP
//     303 
//     808 
//     909 
//   CAT
//    TRKL
//     TRAK
#endif
} rbs_t;

#pragma pack()

#endif/*__RBS_H__*/

