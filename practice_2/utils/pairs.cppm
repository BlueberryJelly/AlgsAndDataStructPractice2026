export module pairs;

import std;

export template <std::size_t KeySize = 256, std::size_t StepSize = 16>
class PairContainer final
{
    static_assert(KeySize > 0, "KeySize - положительное число.");
    static_assert(StepSize > 0, "StepSize - положительное число.");

private:
    struct Pair final
    {
        char *_key = nullptr;
        double _value = 0;

        void allocate_key(const char *key)
        {
            if (key == nullptr)
            {
                throw std::invalid_argument("Ключ не может быть nullptr.");
            }

            const std::size_t length = std::strlen(key) + 1;

            if (length > KeySize)
            {
                throw std::length_error("Недопустимая длина ключа.");
            }

            _key = new char[length];
            std::memcpy(_key, key, length);
        }

        Pair() = default;

        Pair(const char *key, double value)
            : _value(value)
        {
            allocate_key(key);
        }

        Pair(const Pair &other)
            : _value(other._value)
        {
            if (other._key != nullptr)
            {
                allocate_key(other._key);
            }
        }

        Pair(Pair &&other) noexcept
            : _key(other._key), _value(other._value)
        {
            other._key = nullptr;
            other._value = 0;
        }

        Pair &operator=(const Pair &other)
        {
            if (this != &other)
            {
                Pair tmp(other);
                *this = std::move(tmp);
            }

            return *this;
        }

        Pair &operator=(Pair &&other) noexcept
        {
            if (this != &other)
            {
                delete[] _key;
                _key = other._key;
                _value = other._value;
                other._key = nullptr;
                other._value = 0;
            }

            return *this;
        }

        ~Pair() noexcept
        {
            delete[] _key;
        }
    };

    std::size_t _capacity = 0;
    std::size_t _size = 0;
    Pair **_pairs = nullptr;

    void free_data() noexcept
    {
        for (std::size_t index = 0; index < _size; ++index)
        {
            delete _pairs[index];
        }

        delete[] _pairs;
    }

    void take(PairContainer &other) noexcept
    {
        _pairs = other._pairs;
        _capacity = other._capacity;
        _size = other._size;

        other._pairs = nullptr;
        other._capacity = 0;
        other._size = 0;
    }

    std::optional<std::size_t> find(const char *key) const
    {
        if (key == nullptr)
        {
            throw std::invalid_argument("Ключ не может быть nullptr.");
        }

        for (std::size_t index = 0; index < _size; ++index)
        {
            if (std::strcmp(key, _pairs[index]->_key) == 0)
            {
                return index;
            }
        }

        return std::nullopt;
    }

public:
    PairContainer() = default;

    PairContainer(const char *key, double value)
        : PairContainer()
    {
        push_back(key, value);
    }

    PairContainer(const PairContainer &other)
        : PairContainer()
    {
        *this += other;
    }

    PairContainer(PairContainer &&other) noexcept
    {
        take(other);
    }

    PairContainer &operator=(const PairContainer &other)
    {
        if (this != &other)
        {
            PairContainer tmp(other);
            *this = std::move(tmp);
        }

        return *this;
    }

    PairContainer &operator=(PairContainer &&other) noexcept
    {
        if (this != &other)
        {
            free_data();
            take(other);
        }

        return *this;
    }

    ~PairContainer() noexcept
    {
        free_data();
    }

    Pair &operator[](std::size_t index) noexcept
    {
        return *_pairs[index];
    }

    const Pair &operator[](std::size_t index) const noexcept
    {
        return *_pairs[index];
    }

    double &operator[](const char *key)
    {
        if (const auto index = find(key); index)
        {
            return _pairs[*index]->_value;
        }

        push_back(key, 0);
        return _pairs[_size - 1]->_value;
    }

    const double &operator[](const char *key) const
    {
        const auto index = find(key);

        if (!index)
        {
            throw std::out_of_range("Ключ не найден.");
        }

        return _pairs[*index]->_value;
    }

    void reserve(std::size_t new_capacity)
    {
        if (new_capacity <= _capacity)
        {
            return;
        }

        Pair **new_pairs = new Pair *[new_capacity];

        for (std::size_t index = 0; index < _size; ++index)
        {
            new_pairs[index] = _pairs[index];
        }

        delete[] _pairs;
        _pairs = new_pairs;
        _capacity = new_capacity;
    }

    void push_back(const char *key, double value)
    {
        if (_size == _capacity)
        {
            reserve(_capacity + StepSize);
        }

        _pairs[_size] = new Pair(key, value);
        ++_size;
    }

    PairContainer &operator+=(const PairContainer &other)
    {
        const std::size_t count = other._size;

        reserve(_size + count);

        for (std::size_t index = 0; index < count; ++index)
        {
            push_back(other._pairs[index]->_key, other._pairs[index]->_value);
        }

        return *this;
    }

    friend PairContainer operator+(PairContainer lhs, const PairContainer &rhs)
    {
        lhs += rhs;
        return lhs;
    }
};