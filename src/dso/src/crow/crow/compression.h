#pragma once

#include <string>
#include <zlib.h>

// http://zlib.net/manual.html

namespace crow
{
    namespace compression
    {
        // Values used in the 'windowBits' parameter for deflateInit2.
        enum algorithm
        {
            // 15 is the default value for deflate
            DEFLATE = 15,
            // windowBits can also be greater than 15 for optional gzip encoding.
            // Add 16 to windowBits to write a simple gzip header and trailer around the compressed data instead of a zlib wrapper.
            GZIP = 15|16,
        };

        std::string compress_string(std::string const & str, algorithm algo);
        std::string decompress_string(std::string const & deflated_string);
    }
}
