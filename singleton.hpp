
template <typename T>
class Singleton
{
    private:
    static Singleton* element;
    T data;

    // Disallow instantiation outside of the class.
    Singleton(T other);

    public:
    // Forbidden operations
    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

    // Allowed
    static Singleton *Instance(T temp);
    void Destroy();
};
