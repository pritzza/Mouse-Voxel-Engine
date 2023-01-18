#include <array>
#include <cassert>

template<typename Enum, typename Value>
class EnumArray
{
    using EnumValPair = std::pair<Enum, Value>;

private:
    // enum class must have its last enum be "SIZE"
    constexpr static inline int SIZE{ static_cast<int>(Enum::SIZE) };

    std::array<EnumValPair, SIZE> arr;

public:
    constexpr EnumArray(const std::initializer_list<EnumValPair>& il)
    {
        for (int i = 0; i < this->arr.size(); ++i)
        {
            // get iterator to beginning of initilizer list
            const auto& it{ il.begin() };

            // get EnumValPair at current index
            const EnumValPair pair{ *(it + i) };

            // verify that EnumValPair's ordinal value is equal to index
            const bool ordinalIsIndex{ static_cast<int>(pair.first) == i };

            // verify that EnumValPair's value is equal to nominal value of index
            const bool indexIsNominal{ static_cast<Enum>(i) == pair.first };

            const bool isOrdered{ ordinalIsIndex && indexIsNominal };
            assert(isOrdered);

            arr[i] = pair;
        }
    }

    const constexpr Value& getVal(const Enum id) const 
    {
        const int index{ static_cast<int>(id) };

        return this->arr[index].second;
    }

    // requires type VAL with defined operator==
    constexpr Enum getID(const Value& val) const
    {
        for (auto [id, value] : this->arr)
        {
            if (value == val)
                return id;
        }

        // there is an error if we cannot fine the id for the value you searched
        // every value should have unique ID mapped and visa vers 
        // (atleast that's the point)a
        assert(false);
        return Enum::SIZE;
    }

};