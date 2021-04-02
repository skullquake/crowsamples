#include"./dumb_timer_queue.h"
namespace crow
{
    namespace detail 
    {
            void dumb_timer_queue::cancel(dumb_timer_queue::key& k)
            {
                auto self = k.first;
                k.first = nullptr;
                if (!self)
                    return;

                unsigned int index = (unsigned int)(k.second - self->step_);
                if (index < self->dq_.size())
                    self->dq_[index].second = nullptr;
            }

            /// Add a function to the queue.
            dumb_timer_queue::key dumb_timer_queue::add(std::function<void()> f)
            {
                dq_.emplace_back(std::chrono::steady_clock::now(), std::move(f));
                int ret = step_+dq_.size()-1;

                CROW_LOG_DEBUG << "timer add inside: " << this << ' ' << ret ;
                return {this, ret};
            }

            /// Process the queue: take functions out in time intervals and execute them.
            void dumb_timer_queue::process()
            {
                if (!io_service_)
                    return;

                auto now = std::chrono::steady_clock::now();
                while(!dq_.empty())
                {
                    auto& x = dq_.front();
                    if (now - x.first < std::chrono::seconds(tick))
                        break;
                    if (x.second)
                    {
                        CROW_LOG_DEBUG << "timer call: " << this << ' ' << step_;
                        // we know that timer handlers are very simple currenty; call here
                        x.second();
                    }
                    dq_.pop_front();
                    step_++;
                }
            }

            void dumb_timer_queue::set_io_service(boost::asio::io_service& io_service)
            {
                io_service_ = &io_service;
            }

	    dumb_timer_queue::dumb_timer_queue() noexcept
            {
            }
    }
}
