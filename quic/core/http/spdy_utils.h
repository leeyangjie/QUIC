// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICHE_QUIC_CORE_HTTP_SPDY_UTILS_H_
#define QUICHE_QUIC_CORE_HTTP_SPDY_UTILS_H_

#include <cstddef>
#include <cstdint>
#include <string>

#include "net/third_party/quiche/src/quic/core/http/quic_header_list.h"
#include "net/third_party/quiche/src/quic/core/quic_packets.h"
#include "net/third_party/quiche/src/quic/platform/api/quic_export.h"
#include "net/third_party/quiche/src/spdy/core/spdy_framer.h"

namespace quic {

class QUIC_EXPORT_PRIVATE SpdyUtils {
 public:
  SpdyUtils() = delete;

  // Populate |content length| with the value of the content-length header.
  // Returns true on success, false if parsing fails or content-length header is
  // missing.
  static bool ExtractContentLengthFromHeaders(int64_t* content_length,
                                              spdy::SpdyHeaderBlock* headers);

  // Copies a list of headers to a SpdyHeaderBlock.
  static bool CopyAndValidateHeaders(const QuicHeaderList& header_list,
                                     int64_t* content_length,
                                     spdy::SpdyHeaderBlock* headers);

  // Copies a list of headers to a SpdyHeaderBlock.
  // If |expect_final_byte_offset| is true, requires exactly one header field
  // with key kFinalOffsetHeaderKey and an integer value.
  // If |expect_final_byte_offset| is false, no kFinalOffsetHeaderKey may be
  // present.
  // Returns true if parsing is successful.  Returns false if the presence of
  // kFinalOffsetHeaderKey does not match the value of
  // |expect_final_byte_offset|, the kFinalOffsetHeaderKey value cannot be
  // parsed, any other pseudo-header is present, an empty header key is present,
  // or a header key contains an uppercase character.
  static bool CopyAndValidateTrailers(const QuicHeaderList& header_list,
                                      bool expect_final_byte_offset,
                                      size_t* final_byte_offset,
                                      spdy::SpdyHeaderBlock* trailers);

  // Returns a canonicalized URL composed from the :scheme, :authority, and
  // :path headers of a PUSH_PROMISE. Returns empty string if the headers do not
  // conform to HTTP/2 spec or if the ":method" header contains a forbidden
  // method for PUSH_PROMISE.
  static std::string GetPromisedUrlFromHeaders(
      const spdy::SpdyHeaderBlock& headers);

  // Returns hostname, or empty string if missing.
  static std::string GetPromisedHostNameFromHeaders(
      const spdy::SpdyHeaderBlock& headers);

  // Returns true if result of |GetPromisedUrlFromHeaders()| is non-empty
  // and is a well-formed URL.
  static bool PromisedUrlIsValid(const spdy::SpdyHeaderBlock& headers);

  // Populates the fields of |headers| to make a GET request of |url|,
  // which must be fully-qualified.
  static bool PopulateHeaderBlockFromUrl(const std::string url,
                                         spdy::SpdyHeaderBlock* headers);

  // Returns a canonical, valid URL for a PUSH_PROMISE with the specified
  // ":scheme", ":authority", and ":path" header fields, or an empty
  // string if the resulting URL is not valid or supported.
  static std::string GetPushPromiseUrl(QuicStringPiece scheme,
                                       QuicStringPiece authority,
                                       QuicStringPiece path);
};

}  // namespace quic

#endif  // QUICHE_QUIC_CORE_HTTP_SPDY_UTILS_H_
