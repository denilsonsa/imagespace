#ifndef _QEMBED_1804289383
#define _QEMBED_1804289383
static const unsigned int  alpha_png_len = 154;
static const unsigned char alpha_png_data[] = {
    0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,
    0x44,0x52,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x10,0x08,0x02,0x00,0x00,
    0x00,0x90,0x91,0x68,0x36,0x00,0x00,0x00,0x06,0x62,0x4b,0x47,0x44,0x00,
    0xff,0x00,0xff,0x00,0xff,0xa0,0xbd,0xa7,0x93,0x00,0x00,0x00,0x09,0x70,
    0x48,0x59,0x73,0x00,0x00,0x0b,0x12,0x00,0x00,0x0b,0x12,0x01,0xd2,0xdd,
    0x7e,0xfc,0x00,0x00,0x00,0x07,0x74,0x49,0x4d,0x45,0x07,0xd3,0x08,0x14,
    0x01,0x19,0x24,0x11,0xb4,0x68,0xd6,0x00,0x00,0x00,0x27,0x49,0x44,0x41,
    0x54,0x78,0x9c,0x63,0x64,0xc0,0x01,0xfe,0xff,0xff,0x8f,0x55,0x9c,0x09,
    0x97,0x06,0x5c,0x60,0x54,0x03,0x31,0x80,0x11,0x57,0x78,0x33,0x32,0x32,
    0x52,0xc7,0x86,0x51,0x0d,0xc4,0x00,0x00,0x9e,0x21,0x06,0x1c,0x3a,0x26,
    0xb2,0x31,0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82
};

/* Generated by qembed */
#include <qcstring.h>
#include <qdict.h>
static struct Embed {
    unsigned int size;
    const unsigned char *data;
    const char *name;
} embed_vec[] = {
    { 154, alpha_png_data, "alpha.png" },
    { 0, 0, 0 }
};

static const QByteArray& qembed_findData( const char* name )
{
    static QDict<QByteArray> dict;
    QByteArray* ba = dict.find( name );
    if ( !ba ) {
	for ( int i = 0; embed_vec[i].data; i++ ) {
	    if ( strcmp(embed_vec[i].name, name) == 0 ) {
		ba = new QByteArray;
		ba->setRawData( (char*)embed_vec[i].data,
				embed_vec[i].size );
		dict.insert( name, ba );
		break;
	    }
	}
	if ( !ba ) {
	    static QByteArray dummy;
	    return dummy;
	}
    }
    return *ba;
}

#endif
