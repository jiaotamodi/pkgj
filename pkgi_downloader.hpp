#pragma once

#include <deque>
#include <mutex>
#include <vector>

#include "pkgi_thread.hpp"

struct DownloadItem
{
    std::string name;
    std::string content;
    std::string url;
    std::vector<uint8_t> rif;
    std::vector<uint8_t> digest;
};

class Downloader
{
public:
    Downloader(const Downloader&) = delete;
    Downloader(Downloader&&) = delete;
    Downloader& operator=(const Downloader&) = delete;
    Downloader& operator=(Downloader&&) = delete;

    Downloader();
    ~Downloader();

    void add(const DownloadItem& d);
    bool is_in_queue(const std::string& titleid);

    std::function<void(const std::string& content)> refresh;

private:
    using ScopeLock = std::lock_guard<Mutex>;

    Cond _cond;
    std::deque<DownloadItem> _queue;

    std::string _current_content_id;

    Thread _thread;
    bool _dying = false;

    void run();
    void do_download(const DownloadItem& item);
};