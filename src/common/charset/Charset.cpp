/* Copyright (c) 2020 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#include "charset/Charset.h"

namespace nebula {

std::unordered_set<std::string> CharsetInfo::supportCharset = {"utf8"};

std::unordered_set<std::string> CharsetInfo::supportCollation = {"utf8_bin"};

std::unordered_map<std::string, CharsetInfo::CharsetToCollation> CharsetInfo::charsetToCollation = {
    {"utf8", {"utf8", "utf8_bin", {"utf8_bin"}, "UTF-8 Unicode", 4}}
};

std::unordered_map<std::string, std::string> CharsetInfo::collateToLocale = {
    {"utf8_bin", "en_US.UTF-8"}
};

Status CharsetInfo::isSupportCharset(const std::string& charsetName) {
    for (auto& sc : supportCharset) {
        if (!sc.compare(charsetName)) {
            return Status::OK();
        }
    }
    return Status::Error("Charset `%s' not support", charsetName.c_str());
}


Status CharsetInfo::isSupportCollate(const std::string& collateName) {
    for (auto& sc : supportCollation) {
        if (!sc.compare(collateName)) {
            return Status::OK();
        }
    }
    return Status::Error("Collation `%s' not support", collateName.c_str());
}


Status CharsetInfo::charsetAndCollateMatch(const std::string& charsetName,
                                           const std::string& collateName) {
    auto iter = charsetToCollation.find(charsetName);
    if (iter != charsetToCollation.end()) {
        for (auto& sc : iter->second.supportColl_) {
            if (!sc.compare(collateName)) {
                return Status::OK();
            }
        }
    }
    return  Status::Error("Charset `%s' and Collation `%s' not match",
                          charsetName.c_str(), collateName.c_str());
}


StatusOr<std::string> CharsetInfo::getDefaultCollationbyCharset(const std::string& charsetName) {
    auto iter = charsetToCollation.find(charsetName);
    if (iter != charsetToCollation.end()) {
        return iter->second.defaultColl_;
    }
    return Status::Error("Charset `%s' not support", charsetName.c_str());
}


StatusOr<std::string> CharsetInfo::getCharsetbyCollation(const std::string& collationName ) {
    for (auto& cset : charsetToCollation) {
        for (auto& coll : cset.second.supportColl_) {
            if (!coll.compare(collationName)) {
                return cset.first;
            }
        }
    }
    return Status::Error("Collation `%s' not support", collationName.c_str());
}


StatusOr<int>
CharsetInfo::nebulaStrCmp(const std::string& collateName, const char* p1, const char* p2) {
    auto iter = CharsetInfo::collateToLocale.find(collateName);
    if (iter == CharsetInfo::collateToLocale.end()) {
        return Status::Error("Collation `%s' not support", collateName.c_str());
    }

    std::locale loc(iter->second);
    if (loc.name().empty()) {
        return Status::Error("The locale variable is empty");
    }
    auto& f = std::use_facet<std::collate<char>>(loc);

    std::string str1(p1), str2(p2);
    return f.compare(&str1[0], &str1[0] + str1.size(),
                     &str2[0], &str2[0] + str2.size());
}


StatusOr<bool>
CharsetInfo::nebulaStrCmpLT(const std::string& collateName, const char* p1, const char* p2) {
    auto ret = nebulaStrCmp(collateName, p1, p2);
    if (!ret.ok()) {
        return ret.status();
    }
    auto val = ret.value();
    if (val < 0) {
        return true;
    } else {
        return false;
    }
}


StatusOr<bool>
CharsetInfo:: nebulaStrCmpLE(const std::string& collateName, const char* p1, const char* p2) {
    auto ret = nebulaStrCmp(collateName, p1, p2);
    if (!ret.ok()) {
        return ret.status();
    }
    auto val = ret.value();
    if (val <= 0) {
        return true;
    } else {
        return false;
    }
}


StatusOr<bool>
CharsetInfo::nebulaStrCmpGT(const std::string& collateName, const char* p1, const char* p2) {
    auto ret = nebulaStrCmpLE(collateName, p1, p2);
    if (!ret.ok()) {
        return ret.status();
    }
    return !ret.value();
}


StatusOr<bool>
CharsetInfo::nebulaStrCmpGE(const std::string& collateName, const char* p1, const char* p2) {
    auto ret = nebulaStrCmpLT(collateName, p1, p2);
    if (!ret.ok()) {
        return ret.status();
    }
    return !ret.value();
}


StatusOr<bool>
CharsetInfo::nebulaStrCmpEQ(const std::string& collateName, const char* p1, const char* p2) {
    auto ret = nebulaStrCmp(collateName, p1, p2);
    if (!ret.ok()) {
        return ret.status();
    }
    auto val = ret.value();
    if (val == 0) {
        return true;
    } else {
        return false;
    }
}


StatusOr<bool>
CharsetInfo::nebulaStrCmpNE(const std::string& collateName, const char* p1, const char* p2) {
    auto ret = nebulaStrCmpEQ(collateName, p1, p2);
    if (!ret.ok()) {
        return ret.status();
    }
    return !ret.value();
}

}   // namespace nebula
