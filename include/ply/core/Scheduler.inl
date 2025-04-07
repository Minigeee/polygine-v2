
namespace ply {

#ifndef DOXYGEN_SKIP
namespace priv {

    ///////////////////////////////////////////////////////////
    template <typename Sig> class TaskState;

    ///////////////////////////////////////////////////////////
    template <typename Ret> class TaskState<Ret()> : public TaskStateWithResult<Ret> {
    public:
        TaskState(std::function<Ret()>&& func, const TaskDependencies& dependencies, int refCount)
            : m_function(std::forward<std::function<Ret()>>(func)) {
            this->m_dependencies = dependencies;
            this->m_refCount = refCount;
            this->m_isDone = false;
        }

        void operator()() override {
            this->m_result = m_function();
            this->m_isDone = true;

            // Delete self if ref count is 0
            if (--this->m_refCount == 0)
                delete this;
        }

    public:
        std::function<Ret()> m_function;
    };

    ///////////////////////////////////////////////////////////
    template <> class TaskState<void()> : public TaskStateWithResult<void> {
    public:
        TaskState(std::function<void()>&& func, const TaskDependencies& dependencies, int refCount)
            : m_function(std::forward<std::function<void()>>(func)) {
            this->m_dependencies = dependencies;
            this->m_refCount = refCount;
            this->m_isDone = false;
        }

        void operator()() override {
            m_function();
            this->m_isDone = true;

            // Delete self if ref count is 0
            if (--this->m_refCount == 0)
                delete this;
        }

    public:
        std::function<void()> m_function;
    };

    ///////////////////////////////////////////////////////////
    template <typename Ret> inline Ret& TaskStateWithResult<Ret>::getResult() {
        return this->m_result;
    }

} // namespace priv
#endif

///////////////////////////////////////////////////////////
template <typename Ret> inline TaskBase<Ret>::TaskBase() : m_state(0) {}

///////////////////////////////////////////////////////////
template <typename Ret>
inline TaskBase<Ret>::TaskBase(TaskHandle handle)
    : m_state((priv::TaskStateWithResult<Ret>*)handle) {}

///////////////////////////////////////////////////////////
template <typename Ret>
inline TaskBase<Ret>::TaskBase(TaskBase<Ret>&& other) : m_state(other.m_state) {
    other.m_state = 0;
}

///////////////////////////////////////////////////////////
template <typename Ret> inline TaskBase<Ret>& TaskBase<Ret>::operator=(TaskBase<Ret>&& other) {
    if (&other != this) {
        // Delete last state
        if (m_state && --m_state->m_refCount == 0)
            delete m_state;

        m_state = other.m_state;
        other.m_state = 0;
    }

    return *this;
}

///////////////////////////////////////////////////////////
template <typename Ret> inline TaskBase<Ret>::~TaskBase() {
    if (m_state && --m_state->m_refCount == 0)
        delete m_state;

    m_state = 0;
}

///////////////////////////////////////////////////////////
template <typename Ret> inline bool TaskBase<Ret>::isDone() const {
    return m_state && m_state->m_isDone;
}

///////////////////////////////////////////////////////////
template <typename Ret> inline void* TaskBase<Ret>::getHandle() const {
    return (void*)m_state;
}

///////////////////////////////////////////////////////////
template <typename Ret> inline Ret& Task<Ret>::getResult() {
    return this->m_state->getResult();
}

///////////////////////////////////////////////////////////
template <typename F, typename Ret>
inline Task<Ret>
Scheduler::addTask(F&& func, const TaskDependencies& dependencies, Scheduler::Priority priority) {
    // Create new task state (2 references: scheduler, task)
    priv::TaskState<Ret()>* state = new priv::TaskState<Ret()>(std::forward<F>(func), dependencies, 2);

    // Add to queue
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue[priority].push_back(state);
    }

    // Notify any threads that are ready
    m_scv.notify_one();

    // Return task object
    Task<Ret> task((void*)state);
    return task;
}

///////////////////////////////////////////////////////////
template <typename F, typename Ret>
inline Task<Ret>
Scheduler::addTask(F&& func, Scheduler::Priority priority) {
    return addTask(std::forward<F>(func), {}, priority);
}

///////////////////////////////////////////////////////////
template <typename F, typename Ret>
inline Task<Ret>
Barrier::add(F&& func, const TaskDependencies& dependencies, Scheduler::Priority priority) {
    // Create new task state (3 references: barrier, scheduler, task)
    priv::TaskState<Ret()>* state = new priv::TaskState<Ret()>(std::forward<F>(func), dependencies, 3);

    // Add to queue
    {
        std::unique_lock<std::mutex> lock(m_scheduler->m_mutex);
        m_scheduler->m_queue[priority].push_back(state);
    }

    // Add to own list
    m_tasks.push_back(state);

    // Notify any threads that are ready
    m_scheduler->m_scv.notify_one();

    // Return task
    Task<Ret> task((void*)state);
    return task;
}

} // namespace ply
