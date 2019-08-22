#include "intention/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

#define REGIST_NODE_RTTI(name, prop)                          \
	rttr::registration::class_<itt::node::name>("itt::"#name) \
		.constructor<>()                                      \
		prop                                                  \
	;

#define PROP
#define REGIST_NODE_RTTI_DEFAULT(name) REGIST_NODE_RTTI(name, PROP)

RTTR_REGISTRATION
{

// base
rttr::registration::class_<itt::Node>("itt::node")
.property("display", &itt::Node::GetDisplay, &itt::Node::SetDisplay)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo(itt::Node::STR_PROP_DISPLAY))
)
;

// attribute
REGIST_NODE_RTTI(Sort,
.property("key", &itt::node::Sort::key)                          \
(                                                                \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Key")) \
)                                                                \
)

// manipulate
REGIST_NODE_RTTI(Delete,
.property("delete_non_selected", &itt::node::Delete::delete_non_selected)   \
(                                                                           \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("DelNonSelected")) \
)                                                                           \
.property("entity_type", &itt::node::Delete::entity_type)                   \
(                                                                           \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("EntityType"))     \
)                                                                           \
.property("filter_exp", &itt::node::Delete::filter_exp)                     \
(                                                                           \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("FilterExp"))      \
)                                                                           \
)
REGIST_NODE_RTTI(Transform,
.property("translate", &itt::node::Transform::translate)               \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Translate")) \
)                                                                      \
.property("rotate", &itt::node::Transform::rotate)                     \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Rotate"))    \
)                                                                      \
.property("scale", &itt::node::Transform::scale)                       \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Scale"))     \
)                                                                      \
.property("shear", &itt::node::Transform::shear)                       \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Shear"))     \
)                                                                      \
)

// NURBs
REGIST_NODE_RTTI(Carve,
.property("first_u", &itt::node::Carve::first_u)                     \
(                                                                    \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("FirstU"))  \
)                                                                    \
.property("second_u", &itt::node::Carve::second_u)                   \
(                                                                    \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("SecondU")) \
)                                                                    \
.property("first_v", &itt::node::Carve::first_v)                     \
(                                                                    \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("FirstV"))  \
)                                                                    \
.property("second_v", &itt::node::Carve::second_v)                   \
(                                                                    \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("SecondV")) \
)                                                                    \
)

// polygon
REGIST_NODE_RTTI(Add,
.property("points", &itt::node::Add::points)                        \
(                                                                   \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Points")) \
)                                                                   \
)
REGIST_NODE_RTTI(Boolean,
.property("operator", &itt::node::Boolean::op)                        \
(                                                                     \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Operator")) \
)                                                                     \
)
REGIST_NODE_RTTI(Knife,
.property("origin", &itt::node::Knife::origin)                         \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Origin"))    \
)                                                                      \
.property("direction", &itt::node::Knife::direction)                   \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Direction")) \
)                                                                      \
.property("keep", &itt::node::Knife::keep)                             \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Keep"))      \
)
)
REGIST_NODE_RTTI(PolyExtrude,
.property("group_name", &itt::node::PolyExtrude::group_name)           \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("GroupName")) \
)                                                                      \
.property("distance", &itt::node::PolyExtrude::distance)               \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Distance"))  \
)
)

// primitive
REGIST_NODE_RTTI(Box,
.property("size", &itt::node::Box::size)                            \
(                                                                   \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Size"))   \
)                                                                   \
.property("center", &itt::node::Box::center)                        \
(                                                                   \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Center")) \
)                                                                   \
.property("scale", &itt::node::Box::scale)                          \
(                                                                   \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Scale"))  \
)
)
REGIST_NODE_RTTI(Curve,
.property("vertices", &itt::node::Curve::vertices)                        \
(                                                                     \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Vertices")) \
)                                                                     \
)
REGIST_NODE_RTTI(Line,
.property("origin", &itt::node::Line::origin)                          \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Origin"))    \
)                                                                      \
.property("direction", &itt::node::Line::direction)                    \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Direction")) \
)                                                                      \
.property("length", &itt::node::Line::length)                          \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Length"))    \
)                                                                      \
.property("points", &itt::node::Line::points)                          \
(                                                                      \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Points"))    \
)                                                                      \
)
REGIST_NODE_RTTI_DEFAULT(Sphere)

// utility
REGIST_NODE_RTTI(Blast,
.property("group_name", &itt::node::Blast::group_name)                      \
(                                                                           \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("GroupName"))      \
)                                                                           \
.property("group_type", &itt::node::Blast::group_type)                      \
(                                                                           \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("GroupType"))      \
)                                                                           \
.property("delete_non_selected", &itt::node::Blast::delete_non_selected)    \
(                                                                           \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("DelNonSelected")) \
)
)
REGIST_NODE_RTTI_DEFAULT(CopyToPoints)
REGIST_NODE_RTTI(GroupCreate,                                              \
.property("name", &itt::node::GroupCreate::name)                           \
(                                                                          \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Name"))          \
)                                                                          \
.property("type", &itt::node::GroupCreate::type)                           \
(                                                                          \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Type"))          \
)                                                                          \
.property("keep_by_normals", &itt::node::GroupCreate::keep_by_normals)     \
(                                                                          \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("KeepByNormals")) \
)                                                                          \
.property("direction", &itt::node::GroupCreate::direction)                 \
(                                                                          \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Direction"))     \
)                                                                          \
.property("spread_angle", &itt::node::GroupCreate::spread_angle)           \
(                                                                          \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("SpreadAngle"))   \
)                                                                          \
)
REGIST_NODE_RTTI_DEFAULT(Merge)
REGIST_NODE_RTTI(Switch,
.property("selected", &itt::node::Switch::selected)                   \
(                                                                     \
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Selected")) \
)                                                                     \
)

}

namespace itt
{

void nodes_regist_rttr()
{
}

}