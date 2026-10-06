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

        void allocate(const char *key, double value)
        {
            if (key == nullptr)
            {
                throw std::invalid_argument("Ключ не может быть nullptr.");
            }

            std::size_t length = std::strlen(key) + 1;

            if (length > KeySize)
            {
                throw std::length_error("Недопустимая длина ключа.");
            }

            _key = new char[length];
            std::memcpy(_key, key, length);
            _value = value;
        }

        Pair() = default;

        Pair(const char *key, double value)
        {
            allocate(key, value);
        }

        Pair(const Pair &other)
        {
            allocate(other._key, other._value);
        }

        Pair(Pair &&other) noexcept
            : _value(other._value), _key(other._key)
        {
            other._value = 0;
            other._key = nullptr;
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
                other._key = nullptr;
                _value = other._value;
                other._value = 0;
            }
            return *this;
        }

        ~Pair() noexcept
        {
            delete[] _key;
            _key = nullptr;
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
        _pairs = nullptr;
    }

    void free_pairs() noexcept
    {
        free_data();
        _capacity = 0;
        _size = 0;
    }

    std::optional<std::size_t> find(const char *key) const noexcept
    {
        for (std::size_t index = 0; index < _size; ++index)
        {
            if (std::strcmp(key, _pairs[index]->key) == 0)
            {
                return index;
            }
        }

        return std::nullopt;
    }

public:
    PairContainer(const char *key, double value)
        : _capacity(StepSize), _size(1)
    {
        _pairs = new Pair *[StepSize]();

        try
        {
            _pairs[0] = new Pair(key, value);
        }
        catch (...)
        {
            free_pairs();
            throw;
        }
    }

    PairContainer(const PairContainer &other)
        : _capacity(other._capacity), _size(other._size)
    {
        _pairs = new Pair *[other._capacity]();

        try
        {
            for (std::size_t index = 0; index < other._size; ++index)
            {
                if (other._pairs[index] != nullptr)
                {
                    _pairs[index] = new Pair(*other._pairs[index]);
                }
            }
        }
        catch (...)
        {
            free_pairs();
            throw;
        }
    }

    PairContainer(PairContainer &&other) noexcept
        : _capacity(other._capacity), _size(other._size), _pairs(other._pairs)
    {
        other._capacity = 0;
        other._size = 0;
        other._pairs = nullptr;
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
            free_pairs();
            _pairs = other._pairs;
            other._pairs = nullptr;
            _capacity = other._capacity;
            other._capacity = 0;
            _size = other._size;
            other._size = 0;
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
            return _pairs[*index]->value;
        }

        push_back(key, 0);
        return _pairs[_size - 1]->value;
    }

    const double &operator[](const char *key) const
    {
        const auto index = find(key);

        if (index)
        {
            throw std::out_of_range("Ключ не найден.");
        }

        return _pairs[*index]->value;
    }

    void reserve(std::size_t new_capacity)
    {
        if (new_capacity <= _capacity)
        {
            throw std::invalid_argument("Новая вместимость меньше текущей.");
        }

        Pair **new_pairs = new Pair *[new_capacity]();
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

        _pairs[_size + 1] = new Pair(key, value);
        ++_size;
    }
};