#include "../util.hpp"

namespace sdk {
#if defined( _WIN32 ) || defined( _WIN64 )
    ALWAYS_INLINE std::string to_multi_byte( const std::wstring_view wstr ) {
        if ( wstr.empty( ) )
            return {};

        const auto len = WideCharToMultiByte( CP_UTF8, 0, wstr.data( ), wstr.size( ), 0, 0, 0, 0 );
        if ( len <= 0 )
            return {};

        std::string str{};

        str.resize( len );

        if ( WideCharToMultiByte( CP_UTF8, 0, wstr.data( ), wstr.size( ), str.data( ), len, 0, 0 ) <= 0 )
            return {};

        return str;
    }

    ALWAYS_INLINE std::wstring to_wide_char( const std::string_view str ) {
        if ( str.empty( ) )
            return {};

        const auto len = MultiByteToWideChar( CP_UTF8, 0, str.data( ), str.size( ), 0, 0 );
        if ( len <= 0 )
            return {};

        std::wstring wstr{};

        wstr.resize( len );

        if ( MultiByteToWideChar( CP_UTF8, 0, str.data( ), str.size( ), wstr.data( ), len ) <= 0 )
            return {};

        return wstr;
    }
#endif

    template < typename _char_t >
        requires is_char_v< _char_t >
    ALWAYS_INLINE constexpr hash_t hash( const _char_t* const str, const std::size_t len ) {
        constexpr auto k_64b_contstants = sizeof( hash_t ) >= sizeof( std::uint64_t );

        constexpr auto k_basis = k_64b_contstants ? 0xcbf29ce484222325u : 0x811c9dc5u;
        constexpr auto k_prime = k_64b_contstants ? 0x100000001b3u : 0x1000193u;

        auto hash = k_basis;

        for ( std::size_t i{}; i < len; ++i ) {
            hash ^= str[ i ];
            hash *= k_prime;
        }

        return hash;
    }

    template < typename _char_t >
        requires is_char_v< _char_t >
    ALWAYS_INLINE constexpr hash_t hash( const _char_t* const str ) {
        std::size_t len{};
        while ( str[ ++len ] != '\0' )
            ;

        return hash( str, len );
    }
}