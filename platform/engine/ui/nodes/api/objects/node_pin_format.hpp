#ifndef CHERRY_NODES_NODE_PIN_FORMAT
#define CHERRY_NODES_NODE_PIN_FORMAT

#include "../../../../base.hpp"

namespace Cherry
{
    namespace NodeSystem
    {
        enum class PinShape
        {
            Flow,
            Circle,
            Square,
            Grid,
            RoundSquare,
            Diamond
        };

        class PinFormat
        {
        public:
            PinFormat() = default;

            PinFormat(const std::string &id,
                      const std::string &name,
                      const std::string &color,
                      PinShape shape = PinShape::Circle,
                      const std::string &description = "undefined")
                : m_TypeID(id),
                  m_TypeName(name),
                  m_Color(color),
                  m_Shape(shape),
                  m_TypeDescription(description)
            {
            }

            std::string m_TypeID; // Must be unique on the NodeContext
            std::string m_TypeDescription;
            std::string m_TypeName;
            std::string m_Color;
            PinShape m_Shape;
        };

    }
}

#endif // CHERRY_NODES_NODE_PIN_FORMAT