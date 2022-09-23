#pragma once
#include <stack>

template <class S, class T>
class SessionList {
public:
    SessionList(const S& s) : data(s) {}

    void begin() {
        sessions.push(cursor);
    }

    void abort() {
        if (!sessions.empty()) {
            cursor = sessions.top();
            sessions.pop();
        }
    }

    void commit() {
        if (!sessions.empty()) { sessions.pop(); }
    }

    void consume(int count) {
        cursor += count;
    }

    int tell() {
        return cursor;
    }

    void seek(int id) {
        cursor = id;
    }

    int size() { return data.size(); }

    int available() { return size() - cursor; }

    const T& operator[](int id) {
        return data[cursor + id];
    }

private:
    std::stack<int> sessions;
    const S& data;
    int cursor = 0;

};