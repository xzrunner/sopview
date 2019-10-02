#pragma once

#include <SM_Vector.h>
#include <sop/typedef.h>
#include <sop/NodeVar.h>

namespace sop { class Node; }
namespace bp { class Node; class Pin; }

namespace sopv
{

class Evaluator;
struct StrVec3;

class SOP
{
public:
    static void UpdatePropBackFromFront(const bp::Node& front,
        sop::Node& back, const Evaluator& eval);

    static sop::NodePtr CreateBackFromFront(const bp::Node& front);

    static int TypeBackToFront(sop::NodeVarType type);
    static sop::NodeVarType TypeFrontToBack(int pin_type);

private:
    static int ParseExprInt(const std::string& src, const sop::Node& dst,
        size_t idx, int expect, const Evaluator& eval);
    static float ParseExprFloat(const std::string& src, const sop::Node& dst,
        size_t idx, float expect, const Evaluator& eval);
    static sm::vec3 ParseExprFloat3(const StrVec3& src, const sop::Node& dst,
        const sm::ivec3& idx, const sm::vec3& expect, const Evaluator& eval);

}; // SOP

}