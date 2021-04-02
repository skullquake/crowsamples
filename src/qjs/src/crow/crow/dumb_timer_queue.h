#pragma once

#include <boost/asio.hpp>
#include <deque>
#include <functional>
#include <chrono>
#include <thread>

#include "crow/logging.h"

namespace crow
{
    namespace detail 
    {
        /// Fast timer queue for fixed tick value.
        class dumb_timer_queue
        {
        public:
            static int tick;
            //int tick;
            using key = std::pair<dumb_timer_queue*, int>;

            void cancel(key& k);

            /// Add a function to the queue.
            key add(std::function<void()> f);

            /// Process the queue: take functions out in time intervals and execute them.
            void process();

            void set_io_service(boost::asio::io_service& io_service);

            dumb_timer_queue() noexcept;
        private:
            boost::asio::io_service* io_service_{};
            std::deque<std::pair<decltype(std::chrono::steady_clock::now()), std::function<void()>>> dq_;
            int step_{};
        };
	//int dumb_timer_queue::tick=0;
    }
}
