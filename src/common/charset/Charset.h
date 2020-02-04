/* Copyright (c) 2020 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef COMMON_CHARSET_CHARSET_H_
#define COMMON_CHARSET_CHARSET_H_

#include "base/Base.h"
#include "base/StatusOr.h"

namespace nebula {

class CharsetInfo final {
public:
    CharsetInfo() = delete;

    struct CharsetToCollation {
        std::string               charsetName_;
        std::string               defaultColl_;
        std::vector<std::string>  supportColl_;
        std::string               desc_;
        int32_t                   maxLen_;
    };

    /**
     * Check if charset is supported
     */
    static Status isSupportCharset(const std::string& charsetName);
    /**
     * Check if collation is supported
     */
    static Status isSupportCollate(const std::string& collateName);
    /**
     * check if charset and collation match
     */
    static Status charsetAndCollateMatch(const std::string& charsetName,
                                         const std::string& collateName);
    /**
     * Get the corresponding collation according to charset
     */
    static StatusOr<std::string> getDefaultCollationbyCharset(const std::string& charsetName);
    /**
     * Get the corresponding charset according to collation
     */
    static StatusOr<std::string> getCharsetbyCollation(const std::string& collationName);
    /**
     * Compare strings according to the collate of the specified locale
     */
    static StatusOr<int>
    nebulaStrCmp(const std::string& collateName, const char* p1, const char* p2);

    static StatusOr<bool>
    nebulaStrCmpLT(const std::string& collateName, const char* p1, const char* p2);

    static StatusOr<bool>
    nebulaStrCmpLE(const std::string& collateName, const char* p1, const char* p2);

    static StatusOr<bool>
    nebulaStrCmpGT(const std::string& collateName, const char* p1, const char* p2);

    static StatusOr<bool>
    nebulaStrCmpGE(const std::string& collateName, const char* p1, const char* p2);

    static StatusOr<bool>
    nebulaStrCmpEQ(const std::string& collateName, const char* p1, const char* p2);

    static StatusOr<bool>
    nebulaStrCmpNE(const std::string& collateName, const char* p1, const char* p2);


    static std::unordered_set<std::string> supportCharset;

    static std::unordered_set<std::string> supportCollation;

    static std::unordered_map<std::string, CharsetToCollation> charsetToCollation;

    static std::unordered_map<std::string, std::string> collateToLocale;
};

}   // namespace nebula

#endif  // COMMON_CHARSET_CHARSET_H_
