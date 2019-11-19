#pragma once

#include "sopview/Node.h"
#include "sopview/ReflectPropTypes.h"

#include <blueprint/Pin.h>

namespace sopv
{

void nodes_regist_rttr();

namespace node
{

#define SOPV_DEFINE_NODE(type, name, prop) \
class type : public Node                   \
{                                          \
public:                                    \
	type()                                 \
		: Node(#type)                      \
	{                                      \
		InitPins(#name);                   \
	}                                      \
                                           \
	prop                                   \
	RTTR_ENABLE(Node)                      \
};

#define SOPV_DEFINE_PROPS_NODE(type, name, prop) \
class type : public Node                         \
{                                                \
public:                                          \
	type()                                       \
		: Node(#type, true)                      \
	{                                            \
		InitPins(#name);                         \
	}                                            \
                                                 \
	prop                                         \
	RTTR_ENABLE(Node)                            \
};

#define SOPV_DEFINE_IMPORT_EXT_NODE(type, name, base, prop)          \
class type : public base                                             \
{                                                                    \
public:                                                              \
	type()                                                           \
		: base(#type)                                                \
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
	RTTR_ENABLE(base)                                                \
};

#define SOPV_NODE_BASE Node
#define SOPV_NODE_PROP

// base
class Geometry : public Node
{
public:
    Geometry(const std::string& title = "Geometry")
		: Node(title, true)
	{
		InitPins("geometry");
	}
    
    std::vector<bp::NodePtr> children;

	RTTR_ENABLE(Node)
};

// attribute
SOPV_DEFINE_NODE(AttributeCreate, attribcreate::2.0,
    GroupName  group_name;                    \
    GroupTypes group_type = GroupTypes::Auto; \
    AttrCreateItem item0;                     \
    AttrCreateItem item1;                     \
    AttrCreateItem item2;                     \
    AttrCreateItem item3;                     \
)
SOPV_DEFINE_NODE(AttributePromote, attribpromote,
    AttributeName attr_name = GeoAttrClass::Point; \
    GeoAttrClass  from_cls  = GeoAttrClass::Point; \
    GeoAttrClass  to_cls    = GeoAttrClass::Point; \
)
SOPV_DEFINE_NODE(AttributeTransfer, attribtransfer,
    bool points_attrs_toggle       = false;                   \
    bool vertices_attrs_toggle     = false;                   \
    bool prims_attrs_toggle        = false;                   \
    bool detail_attrs_toggle       = false;                   \
    AttributeName points_attrs     = GeoAttrClass::Point;     \
    AttributeName vertices_attrs   = GeoAttrClass::Vertex;    \
    AttributeName primitives_attrs = GeoAttrClass::Primitive; \
    AttributeName detail_attrs     = GeoAttrClass::Detail;    \
)
SOPV_DEFINE_NODE(AttributeVOP, attribvop, SOPV_NODE_PROP)
SOPV_DEFINE_NODE(AttributeWrangle, attribwrangle,
    std::string vex_expr; \
)
SOPV_DEFINE_NODE(Measure, measure,
    MeasureType measure_type = MeasureType::Perimeter; \
    bool        override_name = false;                  \
    std::string attr_name;                              \
)
SOPV_DEFINE_NODE(Sort, sort,
    SortKey     key = SortKey::NoChange; \
    bool        point_reverse = false;   \
    std::string point_offset = "0";      \
)

// group
SOPV_DEFINE_NODE(GroupCreate, groupcreate,
    std::string group_name;                         \
    GroupType   group_type = GroupType::Primitives; \
    GroupMerge  merge_op = GroupMerge::Replace;     \
    bool        base_group = false;                 \
    std::string base_group_expr;                    \
    bool        keep_in_bounding = false;           \
    GroupBoundingType bounding_type = GroupBoundingType::Box;
bool        keep_by_normals = false;            \
sm::vec3    direction = sm::vec3(0, 0, 1);      \
float       spread_angle = 180;                 \
)
SOPV_DEFINE_NODE(GroupExpression, groupexpression,
    GroupType   group_type = GroupType::Primitives; \
    size_t inst_num = 0;                            \
    GroupExprInst inst0;                            \
    GroupExprInst inst1;                            \
    GroupExprInst inst2;                            \
    GroupExprInst inst3;                            \
)
SOPV_DEFINE_NODE(GroupPromote, grouppromote,
    GroupName group_name;                      \
    GroupTypes src_type1 = GroupTypes::Auto;   \
    GroupTypes dst_type1 = GroupTypes::Points; \
)

// import
SOPV_DEFINE_NODE(File, file,
    std::string filepath; \
)
SOPV_DEFINE_NODE(ObjectMerge, object_merge,
    std::string obj0; \
    std::string obj1; \
    std::string obj2; \
    std::string obj3; \
)

// manipulate
SOPV_DEFINE_NODE(Delete, delete,
    bool             delete_non_selected = false;            \
    DeleteEntityType entity_type = DeleteEntityType::Points; \
    DeleteOperation  operation = DeleteOperation::Pattern;   \
    std::string      filter_exp;                             \
)
SOPV_DEFINE_NODE(Peak, peak,
    GroupName   group_name;                             \
    GroupType   group_type = GroupType::GuessFromGroup; \
    std::string distance = "0";                         \
    bool        update_norm = true;                     \
)
SOPV_DEFINE_NODE(Transform, xform,
    GroupName group_name;                             \
    GroupType group_type = GroupType::GuessFromGroup; \
    StrVec3 translate = StrVec3("0", "0", "0");       \
    StrVec3 rotate = StrVec3("0", "0", "0");          \
    StrVec3 scale = StrVec3("1", "1", "1");           \
    StrVec3 shear = StrVec3("0", "0", "0");           \
)

// material
SOPV_DEFINE_NODE(Color, color,
    sm::vec3 color = sm::vec3(1.0f, 1.0f, 1.0f); \
)
SOPV_DEFINE_NODE(UVLayout, uvlayout, SOPV_NODE_PROP)
SOPV_DEFINE_NODE(UVQuickShade, uvquickshade,
    std::string image_file; \
)
SOPV_DEFINE_NODE(UVTransform, uvtransform::2.0,
    GroupName group_name;                             \
    GroupType group_type = GroupType::GuessFromGroup; \
    StrVec3   translate = StrVec3("0", "0", "0");    \
    StrVec3   rotate = StrVec3("0", "0", "0");    \
    StrVec3   scale = StrVec3("1", "1", "1");    \
    StrVec3   shear = StrVec3("0", "0", "0");    \
)
SOPV_DEFINE_NODE(UVUnwrap, uvunwrap,
    UVScale  scale = UVScale::Uniform;  \
    sm::vec3 rotate = sm::vec3(0, 0, 0); \
)

// NURBs
SOPV_DEFINE_NODE(Carve, carve,
    bool first_u_toggle = false; \
    bool second_u_toggle = false; \
    bool first_v_toggle = false; \
    bool second_v_toggle = false; \
    std::string first_u = "0";   \
    std::string second_u = "1";   \
    std::string first_v = "0";   \
    std::string second_v = "1";   \
)

// polygon
SOPV_DEFINE_NODE(Add, add,
    size_t top_switch = 0;  \
    size_t poly_switch = 0; \
    size_t pt_num = 0;      \
    bool p0_toggle = false; \
    bool p1_toggle = false; \
    bool p2_toggle = false; \
    bool p3_toggle = false; \
    sm::vec3 p0;            \
    sm::vec3 p1;            \
    sm::vec3 p2;            \
    sm::vec3 p3;            \
)
SOPV_DEFINE_NODE(Boolean, boolean::2.0,
    BooleanGeoType  a_surface = BooleanGeoType::Solid;      \
    BooleanGeoType  b_surface = BooleanGeoType::Solid;      \
    BooleanOperator op = BooleanOperator::Intersect; \
    BooleanSubType  sub_type = BooleanSubType::AMinusB;    \
    bool        ab_seam_edges_toggle = false;               \
    std::string ab_seam_edges;                              \
)
SOPV_DEFINE_NODE(Divide, divide,
    bool avoid_small_angles = false; \
)
SOPV_DEFINE_NODE(Fuse, fuse,
    FuseOperator op          = FuseOperator::Consolidate; \
    float distance           = 0.001f;                    \
    bool  keep_unused_points = false;                     \
    bool  update_point_norm  = true;                      \
)
SOPV_DEFINE_NODE(Knife, knife,
    KnifeKeep knife_op = KnifeKeep::KeepAll;     \
    StrVec3   origin = StrVec3("0", "0", "0"); \
    sm::vec3  direction = sm::vec3(0, 1, 0);      \
)
SOPV_DEFINE_NODE(Normal, normal,
    GeoAttrClassType attr_add_norm_to = GeoAttrClassType::Vertex; \
)
SOPV_DEFINE_NODE(PolyExtrude, polyextrude::2.0,
    GroupName group_name;            \
    PolyExtrudeExtrusion extrusion = PolyExtrudeExtrusion::PrimEdgeNorm; \
    float distance = 0;              \
    bool output_front = true;        \
    bool output_back  = false;       \
    bool output_side  = true;        \
    bool front_group_toggle = false; \
    bool back_group_toggle  = false; \
    bool side_group_toggle  = false; \
    std::string front_group;         \
    std::string back_group;          \
    std::string side_group;          \
)
SOPV_DEFINE_NODE(PolyFill, polyfill,
    PolyFillMode fill_mode       = PolyFillMode::QuadrilateralGrid; \
    bool         smooth_toggle   = true;                            \
    float        smooth_strength = 50.0f;                           \
)
SOPV_DEFINE_NODE(PolyFrame, polyframe,
    GroupType      entity_type = GroupType::Primitives;     \
    PolyFrameStyle frame_style = PolyFrameStyle::TwoEdges;  \
    bool           normal_toggle    = true;                 \
    bool           tangent_toggle   = true;                 \
    bool           bitangent_toggle = false;                \
    std::string    normal_name;                             \
    std::string    tangent_name;                            \
    std::string    bitangent_name;                          \
)

// primitive
SOPV_DEFINE_NODE(Box, box,
    StrVec3   size   = StrVec3("1", "1", "1"); \
    StrVec3   center = StrVec3("0", "0", "0"); \
    float     scale  = 1.0f;                   \
    bool      divis_toggle = false;            \
    sm::ivec3 divis  = sm::ivec3(3, 3, 3);     \
)
SOPV_DEFINE_NODE(Curve, curve,
    std::vector<sm::vec3> vertices; \
)
SOPV_DEFINE_NODE(Grid, grid,
    sm::vec2 size    = sm::vec2(10, 10); \
    size_t   rows    = 10;               \
    size_t   columns = 10;               \
)
SOPV_DEFINE_NODE(Line, line,
    sm::vec3    origin    = sm::vec3(0, 0, 0); \
    sm::vec3    direction = sm::vec3(0, 1, 0); \
    std::string length    = "1";               \
    size_t      points    = 2;                 \
)
SOPV_DEFINE_NODE(Primitive, primitive,
    GroupName group_name;                      \
    bool trans_toggle     = false;             \
    sm::vec3 translate    = sm::vec3(0, 0, 0); \
    sm::vec3 rotate       = sm::vec3(0, 0, 0); \
    sm::vec3 scale        = sm::vec3(1, 1, 1); \
    sm::vec3 shear        = sm::vec3(0, 0, 0); \
    sm::vec3 pivot_trans  = sm::vec3(0, 0, 0); \
    sm::vec3 pivot_rotate = sm::vec3(0, 0, 0); \
    std::string vec_attrs_to_trans;
)
SOPV_DEFINE_NODE(Sphere, sphere, SOPV_NODE_PROP)
// primitive extern
SOPV_DEFINE_NODE(Dungeon, dungeon,
    sm::vec2 size     = sm::vec2(1, 1);         \
    sm::vec2 split_sz = sm::vec2(0.5f, 0.5f);   \
    sm::vec2 min_sz   = sm::vec2(0.25f, 0.25f); \
    uint32_t seed     = 0;                      \
)

// rigging
SOPV_DEFINE_NODE(Lattice, lattice, SOPV_NODE_PROP)

// utility
SOPV_DEFINE_NODE(Blast, blast,
    GroupName group_name;                             \
    GroupType group_type = GroupType::GuessFromGroup; \
    bool del_non_selected  = false;                   \
    bool del_unused_groups = false;                   \
)
SOPV_DEFINE_NODE(CopyToPoints, copytopoints,
    GroupName   src_group    = GroupName(0); \
    GroupName   target_group = GroupName(1); \
    std::string target_group_str;            \
    bool        show_guide   = true;         \
    bool        trans_dir    = true;         \
    bool        copy_attr    = true;         \
)
SOPV_DEFINE_NODE(ForeachBegin, block_begin,
    ForeachBeginMethod method = ForeachBeginMethod::Input; \
    std::string        block_path;                         \
)
SOPV_DEFINE_NODE(ForeachEnd, block_end,
    ForeachIterMethod    iter    = ForeachIterMethod::ByPiecesOrPoints; \
    ForeachGatherMethod  gather  = ForeachGatherMethod::Merge;          \
    ForeachPieceElements element = ForeachPieceElements::Primitives;    \
    bool        piece_attrib_toggle = false;                            \
    std::string piece_attrib;                                           \
    bool        use_max_iter = false;                                   \
    size_t      max_iter = 10;                                          \
    std::string block_path;                                             \
    std::string template_path;                                          \
    bool        do_single_pass     = false;                             \
    int         single_pass_offset = 0;                                 \
)
SOPV_DEFINE_IMPORT_EXT_NODE(Merge, merge, SOPV_NODE_BASE, SOPV_NODE_PROP)
SOPV_DEFINE_PROPS_NODE(Null, null, SOPV_NODE_PROP)
SOPV_DEFINE_NODE(Output, output,
    int output_idx = -1; \
)
SOPV_DEFINE_NODE(Python, python,
    std::string code; \
)
SOPV_DEFINE_IMPORT_EXT_NODE(Split, split, Geometry,
    GroupName group_name; \
)
SOPV_DEFINE_IMPORT_EXT_NODE(Switch, switch, SOPV_NODE_BASE,
    std::string selected = "0"; \
)

}

}