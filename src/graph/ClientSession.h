/* Copyright (c) 2018 vesoft inc. All rights reserved.
 *
 * This source code is licensed under Apache 2.0 License,
 * attached with Common Clause Condition 1.0, found in the LICENSES directory.
 */

#ifndef GRAPH_CLIENTSESSION_H_
#define GRAPH_CLIENTSESSION_H_

#include "base/Base.h"
#include "time/Duration.h"
#include "graph/GraphFlags.h"

/**
 * A ClientSession holds the context informations of a session opened by a client.
 */

namespace nebula {
namespace graph {

class ClientSession final {
public:
    int64_t id() const {
        return id_;
    }

    void setId(int64_t id) {
        id_ = id;
    }

    GraphSpaceID space() const {
        return space_;
    }

    void setSpace(const std::string &name,
                  GraphSpaceID space,
                  const std::string &charsetName,
                  const std::string &collateName) {
        spaceName_ = name;
        space_ = space;
        spaceCharset_ = charsetName;
        spaceCollate_ = collateName;
    }

    const std::string& spaceName() const {
        return spaceName_;
    }

    const std::string& spaceCharset() const {
        return spaceCharset_;
    }

    const std::string& spaceCollate() const {
        return spaceCollate_;
    }

    uint64_t idleSeconds() const;

    const std::string& user() const {
        return user_;
    }

    void setUser(std::string user) {
        user_ = std::move(user);
    }

    void charge();

private:
    // ClientSession could only be created via SessionManager
    friend class SessionManager;
    ClientSession() = default;
    explicit ClientSession(int64_t id);

    static std::shared_ptr<ClientSession> create(int64_t id);


private:
    int64_t             id_{0};
    GraphSpaceID        space_{-1};
    time::Duration      idleDuration_;
    std::string         spaceName_;
    std::string         spaceCharset_ = FLAGS_default_charset;
    std::string         spaceCollate_ = FLAGS_default_collate;
    std::string         user_;
};

}   // namespace graph
}   // namespace nebula

#endif  // GRAPH_CLIENTSESSION_H_
