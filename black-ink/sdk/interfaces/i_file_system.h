#pragma once
class i_file_system
{
public:
    VFUNC( read( void* pOutput, int nFileSize, void* pFile ), 0, int( __thiscall* )( void*, void*, int, void* ), pOutput, nFileSize, pFile );
    VFUNC( open( const char* szFileName, const char* szOptions, const char* szPathID ), 2, void* ( __thiscall* )( void*, const char*, const char*, const char* ), szFileName, szOptions, szPathID );
    VFUNC( close( void* pFile ), 3, void( __thiscall* )( void*, void* ), pFile );
    VFUNC( size( void* pFile ), 7, int( __thiscall* )( void*, void* ), pFile );
};