/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p10/procedures/ppe/qme/qme_img_edit.c $          */
/*                                                                        */
/* OpenPOWER EKB Project                                                  */
/*                                                                        */
/* COPYRIGHT 2015,2020                                                    */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */

#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <stddef.h>   /* offsetof  */
#include <time.h>

#include <iota_debug_ptrs.h>
#include <p10_hcode_image_defines.H>

enum
{
    QME_IMAGE   =   1,
    CPMR_IMAGE  =   2,
};

uint32_t updateQmeImage( FILE* i_fpQmeImg, uint32_t i_imgSize, uint8_t i_quiet );
uint32_t updateCpmrImage( FILE* i_fpCpmrHdrImg, uint32_t i_imgSize, uint8_t i_quiet );

int main(int narg, char* argv[])
{
    if (narg < 2)
    {
        printf("QME Img Edit Usage: %s <full path to image> <quiet>\n",
               argv[0]);
        return -1;
    }

    int imageType           =   QME_IMAGE;
    FILE* fpImage           =   NULL;
    uint32_t size           =   0;
    uint8_t   arg           =   0;
    uint8_t quiet           =   0;

    fpImage     =   fopen( argv[1], "r+" );

    if( !fpImage )
    {
        printf("\n QME Img Edit: Could not open %s", argv[arg] );
        return -1;
    }

    if ( narg > 2 )
    {
        quiet = 1;
    }

    fseek ( fpImage, 0, SEEK_END );
    size = ftell ( fpImage );
    rewind ( fpImage );

    if( CPMR_HEADER_SIZE == size )
    {
        imageType      =    CPMR_IMAGE;

        if ( !quiet )
        {
            printf("\nCPMR edit" );
        }

        updateCpmrImage( fpImage, size, quiet );
    }

    if( QME_IMAGE  ==  imageType )
    {
        if ( !quiet )
        {
            printf("\nQME edit" );
        }

        updateQmeImage( fpImage, size, quiet );
    }

    fclose( fpImage );
    fpImage =   NULL;

    return 0;
}

//---------------------------------------------------------------------------------------------

uint32_t getTime( uint8_t i_quiet )
{
    time_t buildTime        =   time(NULL);
    struct tm* headerTime   =   localtime(&buildTime);
    uint32_t temp           =   (((headerTime->tm_year + 1900) << 16) |
                                 ((headerTime->tm_mon + 1) << 8) |
                                 (headerTime->tm_mday));

    if ( !i_quiet )
    {
        printf( "                    Build date              : %X -> %04d/%02d/%02d (YYYY/MM/DD)\n",
                temp, headerTime->tm_year + 1900, headerTime->tm_mon + 1, headerTime->tm_mday );
    }

    return temp;
}

//---------------------------------------------------------------------------------------------

uint32_t updateQmeImage( FILE* i_fpQmeImg, uint32_t i_imgSize, uint8_t i_quiet )
{
    uint32_t l_rc       =   0;
    uint32_t l_tempVal  =   0;
    uint32_t headerFieldPos =
        QME_HEADER_IMAGE_OFFSET + offsetof(QmeHeader_t, g_qme_hcode_length);
    fseek( i_fpQmeImg, headerFieldPos, SEEK_SET);
    l_tempVal       =   htonl(i_imgSize);
    fwrite( &l_tempVal, sizeof(uint32_t), 1, i_fpQmeImg );

    if ( !i_quiet )
    {
        printf( "                    QME Hcode Size              : %X -> %04d\n",
                i_imgSize, i_imgSize );
    }

    return l_rc;
}

//---------------------------------------------------------------------------------------------

uint32_t updateCpmrImage( FILE* i_fpCpmrHdrImg, uint32_t i_imgSize, uint8_t i_quiet )
{
    uint32_t l_rc       =   0;
    uint32_t l_tempVal  =   0;
    uint32_t headerFieldPos =  0;

    headerFieldPos  =   offsetof( CpmrHeader_t, iv_buildDate );
    l_tempVal       =   htonl(getTime( i_quiet ));
    fseek( i_fpCpmrHdrImg, headerFieldPos, SEEK_SET);
    fwrite( &l_tempVal, sizeof(uint32_t), 1, i_fpCpmrHdrImg );

    headerFieldPos  =   offsetof( CpmrHeader_t, iv_version );
    l_tempVal       =   htonl(CPMR_BUILD_VER);
    fseek( i_fpCpmrHdrImg, headerFieldPos, SEEK_SET);
    fwrite( &l_tempVal, sizeof(uint32_t), 1, i_fpCpmrHdrImg );

    headerFieldPos  =    offsetof( CpmrHeader_t, iv_qmeImgOffset );
    fseek( i_fpCpmrHdrImg, headerFieldPos, SEEK_SET);
    l_tempVal   =  QME_HCODE_OFFSET;
    fwrite( &l_tempVal, sizeof(uint32_t), 1, i_fpCpmrHdrImg );

    return l_rc;
}

//---------------------------------------------------------------------------------------------