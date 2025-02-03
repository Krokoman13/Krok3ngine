#include <functional>
#include <map>
#include <utility>
#include <string>
#include <memory>

namespace Event {
    template <typename... T>
    class Event;

    class EventDispatcher;

    template <typename... T>
    class SubscribedFunction {
        friend class Event<T...>;
    private:
        explicit SubscribedFunction(const std::function<void(T...)>& a_callback)
            : m_id(s_nextId++)
            , m_callback(a_callback)
        {}

        SubscribedFunction(const SubscribedFunction&) = delete;
        SubscribedFunction& operator=(const SubscribedFunction&) = delete;

        bool operator==(const SubscribedFunction& a_other) const {
            return m_id == a_other.m_id;
        }

    private:
        static unsigned int s_nextId;
        const unsigned int m_id;
        const std::function<void(T...)> m_callback;
    };

    template <typename... T>
    unsigned int SubscribedFunction<T...>::s_nextId = 0;

    class EventBase {
    public:
        virtual void Post(void*) = 0;
    };

    template <typename... Args>
    class Event : public EventBase{        
        friend class EventDispatcher;

    private:
        Event() {};
        Event(const Event&) = delete;
        Event& operator=(const Event&) = delete;

    public:
        SubscribedFunction<Args...>* Subscribe(const std::function<void(Args...)>& a_callback) {
            unsigned int nextId = SubscribedFunction<Args...>::s_nextId;
            m_subscribedFunctions[nextId] = SubscribedFunction<Args...>(a_callback);
            return &m_subscribedFunctions[nextId];
        }

        void Unsubscribe(SubscribedFunction<Args...>* a_toUnsubscribe) {
            if (!a_toUnsubscribe) return;

            auto it = m_subscribedFunctions.find(a_toUnsubscribe->m_id);
            if (it != m_subscribedFunctions.end()) {
                it->second.~SubscribedFunction();
                m_subscribedFunctions.erase(it);
            }
        }

        void Post(Args... args) override {
            for (auto& pair : m_subscribedFunctions) {
                pair.second.m_callback(args...);
            }
        }

    private:
        std::map<unsigned int, SubscribedFunction<Args...>> m_subscribedFunctions;
    };

    class EventDispatcher {
    public:
        EventDispatcher() {};

    protected:
        template<typename... Args>
        Event<Args...>* createEvent() {
            auto it = m_events.find();
            
            if (it != m_events.end()) {
                return;
            }

            return (m_events[eventId] = std::make_unique<Event<Args...>>()).get();
        }

    protected:
        std::vector<std::unique_ptr<EventBase>> m_events;
    };
}