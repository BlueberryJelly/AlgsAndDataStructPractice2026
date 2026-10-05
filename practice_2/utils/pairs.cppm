export module pairs;

import std;

export template <std::size_t KeySize = 256, std::size_t StepSize = 16>
class PairContainer final
{
private:
    struct Pair final
    {
        char *_key = nullptr;
        double _value = 0;

        void allocate(const char *key, double value)
        {
            if (key == nullptr)
            {
                return;
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
            : _value(other._value), _key(other._key);
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

    std::size_t _capacity = StepSize;
    std::size_t _size = 0;
    Pair **_pairs = nullptr;

public:
    PairContainer(const char *key, double value)
        : _size(1)
    {
        if (key == nullptr)
        {
            throw std::invalid_argument("Отсутсвует ключь");
        }
        if (std::strlen(key) >= KeySize)
        {
            throw std::length_error("Недопустимая длина ключа");
        }

        _pairs = new Pair *[_capacity];
        _pairs[0] = new Pair(key, value);
    }
};