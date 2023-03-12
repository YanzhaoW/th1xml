#pragma once

#include <boost/program_options.hpp>

namespace th1xml
{
    using std::runtime_error;

    namespace po = boost::program_options;

    template <typename>
    struct is_bool : std::false_type
    {
    };

    template <>
    struct is_bool<bool> : std::true_type
    {
    };

    template <typename Type>
    class Option
    {
      public:
        using type = Type;
        explicit Option(std::string name, Type defaultValue = {})
            : m_name{ std::move(name) }
            , m_value{ std::move(defaultValue) }
        {
            if (auto end = m_name.find(','))
            {
                m_key = m_name.substr(0, end);
            }
            else
            {
                m_key = m_name;
            }
        }

        void Add(po::options_description& po_desc, const std::string& desc)
        {
            if constexpr (is_bool<Type>::value)
            {
                po_desc.add_options()(m_name.c_str(), desc.c_str());
            }
            else
            {
                po_desc.add_options()(m_name.c_str(), po::value<Type>(), desc.c_str());
            }
        }

        void MakePositional(po::positional_options_description& pos_desc, int option)
        {
            pos_desc.add(m_name.c_str(), option);
        }

        void Retrieve(const po::variables_map& varMap)
        {
            if (varMap.count(m_key) != 0U)
            {

                if constexpr (is_bool<Type>::value)
                {
                    m_value = true;
                }
                else
                {
                    m_value = varMap[m_key].template as<Type>();
                }
            }
            else if (is_required)
            {
                const auto error_msg = "program option " + m_name + " is required.";
                throw runtime_error(error_msg);
            }
        }

        void Set_required(bool p_rq) { is_required = p_rq; }

        [[nodiscard]] auto Get() const { return m_value; }

      private:
        std::string m_name;
        std::string m_key;
        std::string m_desc;
        bool is_required = false;
        Type m_value{};
    };
}; // namespace th1xml
