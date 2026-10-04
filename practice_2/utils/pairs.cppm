export module pairs;

import std;

export template<std::size_t KeySize = 256, std::size_t StepSize = 16>
class PairContainer final
{
private:
    class Pair final
    {
    private:
        void allocate(const char* key, double value)
        {
            _key = new char[KeySize];
            std::memcpy(_key, key, std::strlen(key) + 1);
            _value = value;
        }

        void free_pair() noexcept
        {
            delete[] _key;
            _key = nullptr;
            _value = 0;
        }

        void move_pair(Pair&& other) noexcept
        {
            _key = other._key;
            other._key = nullptr;
            _value = other._value;
            other._value = 0;
        }

    public:
        char* _key = nullptr;
        double _value = 0;

        Pair() = default;

        Pair(const char* key, double value)
        {
            allocate(key, value);
        }

        Pair(const Pair& other)
        {
            allocate(other._key, other._value);
        }

        Pair(Pair&& other) noexcept
        {
            move_pair(std::move(other));
        }

        Pair& operator=(const Pair& other)
        {
            if (this != &other)
            {
                Pair tmp(other);
                *this = std::move(tmp);
            }
            return *this;
        }

        Pair& operator=(Pair&& other) noexcept
        {
            if (this != &other)
            {
                free_pair();
                move_pair(std::move(other));
            }
            return *this;
        }

        ~Pair() noexcept
        {
            free_pair();
        }
    };

    std::size_t _capacity = StepSize;
    std::size_t _size = 0;
    Pair** _pairs = nullptr;

public:
    PairContainer(const char* key, double value)
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

        _pairs = new Pair*[_capacity];
        _pairs[0] = new Pair(key, value);
    }
};