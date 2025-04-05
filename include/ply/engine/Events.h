#pragma once

#include <ply/core/Handle.h>
#include <ply/core/HandleArray.h>
#include <ply/core/Mutex.h>
#include <ply/core/Types.h>

#include <functional>
#include <mutex>
#include <typeindex>
#include <vector>

namespace ply {

#ifndef DOXYGEN_SKIP

namespace priv {

    template <typename E> class EventDispatcherImpl {
    protected:
        Handle addListener(std::function<void(const E&)>&& func);

        void removeListener(Handle handle);

        void sendEvent(const E& event);

    private:
        HandleArray<std::function<void(const E&)>> m_listeners;
    };

    ///////////////////////////////////////////////////////////
    /// \brief Stores events and listeners
    ///////////////////////////////////////////////////////////
    template <typename E> struct EventQueue {
        std::vector<E> m_events;                                //!< List of events
        HandleArray<std::function<void(const E&)>> m_listeners; //!< List of listeners
        std::mutex m_mutex;                                     //!< Mutex for thread safety
    };

    ///////////////////////////////////////////////////////////
    /// \brief Event queue lifecycle handlers
    ///////////////////////////////////////////////////////////
    struct EventQueueLifecycleHandler {
        std::function<void()> m_processEvents; //!< Type erased function to process events
        std::function<void()> m_cleanup;       //!< Type erased function to clean up the queue
    };

}

#endif

///////////////////////////////////////////////////////////
/// \brief Dispatches events for a set of event types
///
///////////////////////////////////////////////////////////
template <typename... Es> class EventDispatcher : public priv::EventDispatcherImpl<Es>... {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Add an event listener function
    ///
    /// The given function will be called every time the specified
    /// event is sent, and it must be casted into a std::function.
    /// This means the function can be a function pointer, a member
    /// function pointer, lambda, or any other function created
    /// with std::bind.
    ///
    /// A handle to the event listener is returned in case the
    /// listener needs to be removed later.
    ///
    /// \param func The listener function
    ///
    /// \return A handle to the listener function, used to remove the listener
    ///
    /// \see removeListener
    ///
    ///////////////////////////////////////////////////////////
    template <typename E> Handle addListener(std::function<void(const E&)>&& func);

    ///////////////////////////////////////////////////////////
    /// \brief Remove an event listener function
    ///
    /// The handle must be acquired from addListener().
    ///
    /// \param handle The handle of the listener to remove
    ///
    /// \see addListener
    ///
    ///////////////////////////////////////////////////////////
    template <typename E> void removeListener(Handle handle);

protected:
    ///////////////////////////////////////////////////////////
    /// \brief Send an event to all registered event listener functions
    ///
    /// All listener functions added under the specified event type
    /// will be invoked, but not necessarily in the order the
    /// listeners were added.
    ///
    /// \param event The event data to send
    ///
    ///////////////////////////////////////////////////////////
    template <typename E> void sendEvent(const E& event);

private:
    std::shared_mutex m_mutex;
};

///////////////////////////////////////////////////////////
/// \brief A deferred, thread safe event system where anyone can send and listen to events
///
///////////////////////////////////////////////////////////
class EventSystem {
public:
    EventSystem() = default;
    ~EventSystem();

    ///////////////////////////////////////////////////////////
    /// \brief Add an event listener function
    ///
    /// The given function will be called every time the specified
    /// event is sent, and it must be casted into a std::function.
    /// This means the function can be a function pointer, a member
    /// function pointer, lambda, or any other function created
    /// with std::bind.
    ///
    /// A handle to the event listener is returned in case the
    /// listener needs to be removed later.
    ///
    /// \param func The listener function
    ///
    /// \return A handle to the listener function, used to remove the listener
    ///
    /// \see removeListener
    ///
    ///////////////////////////////////////////////////////////
    template <typename E, typename Func> Handle addListener(Func&& func);

    ///////////////////////////////////////////////////////////
    /// \brief Remove an event listener function
    ///
    /// The handle must be acquired from addListener().
    ///
    /// \param handle The handle of the listener to remove
    ///
    /// \see addListener
    ///
    ///////////////////////////////////////////////////////////
    template <typename E> void removeListener(Handle handle);

    ///////////////////////////////////////////////////////////
    /// \brief Send an event to all registered event listener functions
    ///
    /// All listener functions added under the specified event type
    /// will be invoked, but not necessarily in the order the
    /// listeners were added.
    ///
    /// \param event The event data to send
    ///
    ///////////////////////////////////////////////////////////
    template <typename E> void sendEvent(const E& event);

    ///////////////////////////////////////////////////////////
    /// \brief Poll all event queues and invoke listeners
    ///
    /// This function should be called once per frame to process
    /// all pending events.
    ///
    ///////////////////////////////////////////////////////////
    void poll();

private:
    ///////////////////////////////////////////////////////////
    /// \brief Create event queue
    ///////////////////////////////////////////////////////////
    template <typename E> priv::EventQueue<E>& getOrCreateEventQueue();

private:
    HashMap<std::type_index, void*> m_queues; //!< Type erased event queues
    std::vector<priv::EventQueueLifecycleHandler>
        m_lifecycleHandlers; //!< Event queue lifecycle handlers
    std::mutex m_mutex;      //!< Mutex for listeners
};

}

#include <ply/engine/Events.inl>

///////////////////////////////////////////////////////////
/// \class poly::EventDispatcher
/// \ingroup Engine
///
/// EventDispatcher handles adding listeners and sending events
/// for a limited set of event types. Only the event types
/// specified in the system's template parameters will be
/// valid. However, having a limited set of events will not
/// be a constraint most of the time, as there is only a limited
/// number of unique event types that can be created anyway.
///
/// This event system is meant to be convenient for the user
/// to setup, rather than being general purpose like the Scene
/// event system, which can handle any number of event types
/// without specifying which ones it can handle.
///
/// Use EventDispatcher::addListener() to add any listener functions.
/// This function returns a Handle to the listener function,
/// which can be used to remove the listener at a later time
/// if desired. EventDispatcher::sendEvent() will pass the given event
/// object to all existing listener functions.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// struct EventA { int m_a; };
/// struct EventB { float m_b; };
/// struct EventC { bool m_c; };
///
/// class A :
///		public EventDispatcher<EventA, EventB, EventC>
/// { };
///
/// // Create object
/// A a;
///
/// // Add a listener
/// Handle listener = a.addListener<EventA>(
///		[&](const EventA& e)
///		{
///			std::cout << e.m_a << '\n';
///		}
/// );
///
/// // Send a few events
/// a.sendEvent(EventA{ 1 });
/// a.sendEvent(EventA{ 2 });
/// a.sendEvent(EventA{ 3 });
///
/// // These won't do anything because there are no listeners for these events
/// a.sendEvent(EventB{ 3.14f });
/// a.sendEvent(EventB{ 6.28f });
///
/// // Remove the listener
/// a.removeListener<EventA>(listener);
///
/// \endcode
///
///////////////////////////////////////////////////////////