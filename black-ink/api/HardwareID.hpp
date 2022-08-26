#pragma once

#include <windows.h>
#include <intrin.h>

#include <memory>

#include "md5/md5.hpp"

namespace HWID {
	__forceinline std::string CPUHardwareID( )
	{
		int CPUInfo[ 4 ] = { -1 };
		char CPUBrandString[ 0x40 ];
		__cpuid( CPUInfo, 0x80000000 );
		unsigned int nExIds = CPUInfo[ 0 ];

		memset( CPUBrandString, 0, sizeof( CPUBrandString ) );

		for ( size_t i = 0x80000000; i <= nExIds; ++i )
		{
			__cpuid( CPUInfo, i );
			if ( i == 0x80000002 )
				memcpy( CPUBrandString, CPUInfo, sizeof( CPUInfo ) );
			else if ( i == 0x80000003 )
				memcpy( CPUBrandString + 16, CPUInfo, sizeof( CPUInfo ) );
			else if ( i == 0x80000004 )
				memcpy( CPUBrandString + 32, CPUInfo, sizeof( CPUInfo ) );
		}

		return std::string( CPUBrandString );
	};

	__forceinline std::string GetVolumeID( )
	{
		HANDLE hFile = CreateFileA( "\\\\.\\PhysicalDrive0", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
		if ( hFile == INVALID_HANDLE_VALUE )
			return { };

		std::unique_ptr< std::remove_pointer <HANDLE >::type, void( * )( HANDLE ) > hDevice
		{
			hFile, [ ] ( HANDLE handle )
		{
			CloseHandle( handle );
		}
		};

		STORAGE_PROPERTY_QUERY PropertyQuery;
		PropertyQuery.PropertyId = StorageDeviceProperty;
		PropertyQuery.QueryType = PropertyStandardQuery;

		STORAGE_DESCRIPTOR_HEADER DescHeader;
		DWORD dwBytesReturned = 0;
		if ( !DeviceIoControl( hDevice.get( ), IOCTL_STORAGE_QUERY_PROPERTY, &PropertyQuery, sizeof( STORAGE_PROPERTY_QUERY ),
			&DescHeader, sizeof( STORAGE_DESCRIPTOR_HEADER ), &dwBytesReturned, NULL ) )
			return { };

		const DWORD dwOutBufferSize = DescHeader.Size;
		std::unique_ptr< BYTE[] > pOutBuffer{ new BYTE[ dwOutBufferSize ] { } };

		if ( !DeviceIoControl( hDevice.get( ), IOCTL_STORAGE_QUERY_PROPERTY, &PropertyQuery, sizeof( STORAGE_PROPERTY_QUERY ),
			pOutBuffer.get( ), dwOutBufferSize, &dwBytesReturned, NULL ) )
			return { };

		STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = reinterpret_cast< STORAGE_DEVICE_DESCRIPTOR* >( pOutBuffer.get( ) );
		if ( !pDeviceDescriptor )
			return { };

		const DWORD dwSerialNumberOffset = pDeviceDescriptor->SerialNumberOffset;
		if ( !dwSerialNumberOffset )
			return { };

		std::string sResult = reinterpret_cast< const char* >( pOutBuffer.get( ) + dwSerialNumberOffset );
		return sResult;
	};

	__forceinline std::string HardwareID( ) {
		return md5( md5( CPUHardwareID( ) ) + md5( GetVolumeID( ) ) );
	}
}