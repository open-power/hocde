/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: import/chips/p10/procedures/ppe/xgpe/xgpe_img_edit.c $        */
/*                                                                        */
/* OpenPOWER EKB Project                                                  */
/*                                                                        */
/* COPYRIGHT 2015,2019                                                    */
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
    XGPE_IMAGE  =   1,
    XPMR_IMAGE  =   2,
};

uint32_t updateXgpeImage( FILE* i_fpXgpeImg, uint32_t i_imgSize );
uint32_t updateXpmrImage( FILE* i_fpXpmrHdrImg, uint32_t i_imgSize );

int main(int narg, char* argv[])
{
    if (narg < 2)
    {
        printf("XGPE Img Edit Usage: %s <full path to image>\n",
               argv[0]);
        return -1;
    }

    int imageType           =   XGPE_IMAGE;
    FILE* fpImage           =   NULL;
    uint32_t size           =   0;
    uint8_t   arg           =   0;

    fpImage     =   fopen( argv[1], "r+" );

    if( !fpImage )
    {
        printf("\n XGPE Img Edit: Could not open %s", argv[arg] );
        return -1;
    }

    fseek ( fpImage, 0, SEEK_END );
    size = ftell ( fpImage );
    rewind ( fpImage );

    if( XPMR_HEADER_SIZE == size )
    {
        imageType      =    XPMR_IMAGE;
        printf("\nXPMR edit" );
        updateXpmrImage( fpImage, size );
    }

    if( XGPE_IMAGE  ==  imageType )
    {
        printf("\nXGPE edit" );
        updateXgpeImage( fpImage, size );
    }

    fclose( fpImage );
    fpImage =   NULL;

    return 0;
}

//---------------------------------------------------------------------------------------------

uint32_t getTime()
{
    time_t buildTime        =   time(NULL);
    struct tm* headerTime   =   localtime(&buildTime);
    uint32_t temp           =   (((headerTime->tm_year + 1900) << 16) |
                                 ((headerTime->tm_mon + 1) << 8) |
                                 (headerTime->tm_mday));
    printf( "                    Build date              : %X -> %04d/%02d/%02d (YYYY/MM/DD)\n",
            temp, headerTime->tm_year + 1900, headerTime->tm_mon + 1, headerTime->tm_mday );

    return temp;
}

//---------------------------------------------------------------------------------------------

uint32_t updateXgpeImage( FILE* i_fpXgpeImg, uint32_t i_imgSize )
{
    uint32_t l_rc       =   0;
    uint32_t l_tempVal  =   0;
    uint32_t headerFieldPos =
        XGPE_HEADER_IMAGE_OFFSET + offsetof(XgpeHeader_t, g_xgpe_hcodeLength);
    fseek( i_fpXgpeImg, headerFieldPos, SEEK_SET);
    l_tempVal       =   htonl(i_imgSize);
    fwrite( &l_tempVal, sizeof(uint32_t), 1, i_fpXgpeImg );

    headerFieldPos  =   XGPE_HEADER_IMAGE_OFFSET + offsetof(XgpeHeader_t, g_xgpe_buildDate );
    fseek( i_fpXgpeImg, headerFieldPos, SEEK_SET);
    l_tempVal       =   htonl(getTime());
    fwrite( &l_tempVal, sizeof(uint32_t), 1, i_fpXgpeImg );

    headerFieldPos  =   XGPE_HEADER_IMAGE_OFFSET + offsetof(XgpeHeader_t, g_xgpe_buildVer );
    fseek( i_fpXgpeImg, headerFieldPos, SEEK_SET);
    l_tempVal       =   htonl(XGPE_BUILD_VER);
    fwrite( &l_tempVal, sizeof(uint32_t), 1, i_fpXgpeImg );

    return l_rc;
}

//---------------------------------------------------------------------------------------------

uint32_t updateXpmrImage( FILE* i_fpXpmrHdrImg, uint32_t i_imgSize )
{
    uint32_t l_rc       =   0;
    uint32_t l_tempVal  =   0;
    uint32_t headerFieldPos =  0;

    headerFieldPos  =   offsetof( XpmrHeader_t, iv_buildDate );
    l_tempVal       =   htonl(getTime());
    fseek( i_fpXpmrHdrImg, headerFieldPos, SEEK_SET);
    fwrite( &l_tempVal, sizeof(uint32_t), 1, i_fpXpmrHdrImg );

    headerFieldPos  =   offsetof( XpmrHeader_t, iv_version );
    l_tempVal       =   htonl(XPMR_BUILD_VER);
    fseek( i_fpXpmrHdrImg, headerFieldPos, SEEK_SET);
    fwrite( &l_tempVal, sizeof(uint32_t), 1, i_fpXpmrHdrImg );

    headerFieldPos  =    offsetof( XpmrHeader_t, iv_xgpeHcodeOffset );
    fseek( i_fpXpmrHdrImg, headerFieldPos, SEEK_SET);
    l_tempVal   =  XGPE_HCODE_OFFSET;
    fwrite( &l_tempVal, sizeof(uint32_t), 1, i_fpXpmrHdrImg );

    return l_rc;
}

//---------------------------------------------------------------------------------------------
