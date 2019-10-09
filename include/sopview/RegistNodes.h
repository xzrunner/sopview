#pragma once

#include "sopview/Node.h"
#include "sopview/ReflectPropTypes.h"

#include <blueprint/Pin.h>

namespace sopv
{

void nodes_regist_rttr();

namespace node
{

#define ITT_DEFINE_NODE(name, prop)          \
class name : public Node                     \
{                                            \
public:                                      \
	name()                                   \
		: Node(#name)                        \
	{                                        \
		InitPins(#name);                     \
	}                                        \
                                             \
	prop                                     \
	RTTR_ENABLE(Node)                        \
};

#define ITT_DEFINE_PROPS_NODE(name, prop)    \
class name : public Node                     \
{                                            \
public:                                      \
	name()                                   \
		: Node(#name, true)                  \
	{                                        \
		InitPins(#name);                     \
	}                                        \
                                             \
	prop                                     \
	RTTR_ENABLE(Node)                        \
};

#define ITT_DEFINE_IMPORT_EXT_NODE(name, prop)                       \
class name : public Node                                             \
{                                                                    \
public:                                                              \
	name()                                                           \
		: Node(#name)                                                \
	{                                                                \
		InitPins(#name);                                             \
        SetExtensibleInputPorts(true);                               \
	}                                                                \
private:                                                             \
    virtual std::string GenInputPinName(size_t idx) const override { \
        return "in" + std::to_string(idx);                           \
    }                                                                \
                                                                     \
public:                                                              \
	prop                                                             \
	RTTR_ENABLE(Node)                                                \
};

#define ITT_NODE_PROP

// base
ITT_DEFINE_PROPS_NODE(Geometry,        \
    std::vector<bp::NodePtr> children; \
)

// attribute
ITT_DEFINE_NODE(Measure,                          \
    MeasureType ms_type = MeasureType::Perimeter; \
    std::string ms_name;                          \
)
ITT_DEFINE_NODE(Sort,                \
    SortKey key = SortKey::NoChange; \
)

// group
ITT_DEFINE_NODE(GroupCreate,                          \
    std::string group_name;                           \
    GroupType   group_type = GroupType::Primitives;   \
    GroupMerge  merge_op   = GroupMerge::Replace;     \
    bool        base_group = false;                   \
    std::string base_group_expr;                      \
    bool        keep_by_normals  = false;             \
    sm::vec3    direction        = sm::vec3(0, 0, 1); \
    float       spread_angle     = 180;               \
)
ITT_DEFINE_NODE(GroupExpression,                    \
    GroupType   group_type = GroupType::Primitives; \
    GroupExprInst inst0;                            \
    GroupExprInst inst1;                            \
    GroupExprInst inst2;                            \
    GroupExprInst inst3;                            \
)

// manipulate
ITT_DEFINE_NODE(Delete,                                      \
    bool             delete_non_selected = false;            \
    DeleteEntityType entity_type = DeleteEntityType::Points; \
    std::string      filter_exp;                             \
)
ITT_DEFINE_NODE(Transform,                            \
    GroupName group_name;                             \
    GroupType group_type = GroupType::GuessFromGroup; \
    StrVec3 translate    = StrVec3("0", "0", "0");    \
    StrVec3 rotate       = StrVec3("0", "0", "0");    \
    StrVec3 scale        = StrVec3("1", "1", "1");    \
    StrVec3 shear        = StrVec3("0", "0", "0");    \
)

// material
ITT_DEFINE_NODE(Color,                           \
    sm::vec3 color = sm::vec3(1.0f, 1.0f, 1.0f); \
)

// NURBs
ITT_DEFINE_NODE(Carve,          \
    std::string first_u  = "0"; \
    std::string second_u = "1"; \
    std::string first_v  = "0"; \
    std::string second_v = "1"; \
)

// polygon
ITT_DEFINE_NODE(Add,              \
    std::vector<sm::vec3> points; \
)
ITT_DEFINE_NODE(Boolean,                             \
    BooleanOperator op = BooleanOperator::Intersect; \
)
ITT_DEFINE_NODE(Fuse,        \
    float distance = 0.001f; \
)
ITT_DEFINE_NODE(Knife,                      \
    sm::vec3 origin;                        \
    sm::vec3 direction = sm::vec3(0, 1, 0); \
    KnifeKeep keep = KnifeKeep::KeepAll;    \
)
ITT_DEFINE_NODE(Normal,                                 \
    GeoAttrType attr_add_norm_to = GeoAttrType::Vertex; \
)
ITT_DEFINE_NODE(PolyExtrude, \
    GroupName group_name;    \
    float distance = 0;      \
)
ITT_DEFINE_NODE(PolyFill, ITT_NODE_PROP)

// primitive
ITT_DEFINE_NODE(Box,                         \
    StrVec3 size   = StrVec3("1", "1", "1"); \
    StrVec3 center = StrVec3("0", "0", "0"); \
    float scale    = 1.0f;                   \
)
ITT_DEFINE_NODE(Curve,              \
    std::vector<sm::vec3> vertices; \
)
ITT_DEFINE_NODE(Line,                          \
    sm::vec3    origin    = sm::vec3(0, 0, 0); \
    sm::vec3    direction = sm::vec3(0, 1, 0); \
    std::string length    = "1";               \
    size_t      points    = 2;                 \
)
ITT_DEFINE_NODE(Primitive,                  \
    sm::vec3 translate = sm::vec3(0, 0, 0); \
    sm::vec3 rotate    = sm::vec3(0, 0, 0); \
    sm::vec3 scale     = sm::vec3(1, 1, 1); \
    sm::vec3 shear     = sm::vec3(0, 0, 0); \
)
ITT_DEFINE_NODE(Sphere, ITT_NODE_PROP)

// utility
ITT_DEFINE_NODE(Blast,                                \
    GroupName group_name;                             \
    GroupType group_type = GroupType::GuessFromGroup; \
    bool delete_non_selected = false;                 \
)
ITT_DEFINE_NODE(CopyToPoints,              \
    GroupName src_group    = GroupName(0); \
    GroupName target_group = GroupName(1); \
    bool use_pt_dir = false;               \
)
ITT_DEFINE_NODE(ForeachPrimBegin, ITT_NODE_PROP)
ITT_DEFINE_NODE(ForeachPrimEnd,      \
    bool do_single_pass     = false; \
    int  single_pass_offset = 0;     \
)
ITT_DEFINE_IMPORT_EXT_NODE(Merge, ITT_NODE_PROP)
ITT_DEFINE_PROPS_NODE(Null, ITT_NODE_PROP)
ITT_DEFINE_IMPORT_EXT_NODE(Switch, \
    std::string selected = "0";    \
)

}

}