#include "intention/SceneTree.h"
#include "intention/Evaluator.h"
#include "intention/Node.h"
#include "intention/RegistNodes.h"
#include "intention/SOP.h"

#include <blueprint/CompNode.h>
#include <blueprint/Node.h>

#include <node0/SceneNode.h>
#include <node0/CompComplex.h>
#include <node0/NodeFlagsHelper.h>
#include <node0/NodeFlags.h>
#include <ns/NodeFactory.h>
#include <sop/node/Geometry.h>

#include <assert.h>

namespace
{

void RebuildBackFromFront(std::shared_ptr<sop::node::Geometry>& dst,
                          const std::shared_ptr<itt::node::Geometry>& src,
                          const itt::Evaluator& eval)
{
    dst->ClearChildren();
    for (auto& c : src->children)
    {
        auto dst_c = eval.QueryBackNode(*c);
        if (!dst_c) {
            continue;
        }
        sop::node::Geometry::AddChild(dst, dst_c);

        //// calc again, for expr which need level info
        //itt::SOP::UpdatePropBackFromFront(*c, *dst_c, eval);
    }

    // calc again, for expr which need level info
    for (auto& c : src->children)
    {
        auto dst_c = eval.QueryBackNode(*c);
        itt::SOP::UpdatePropBackFromFront(*c, *dst_c, eval);
    }
}

}

namespace itt
{

SceneTree::SceneTree()
{
#ifdef ITT_SCENE_TREE_DUMMY_ROOT
    InitDummyRoot();
#endif // ITT_SCENE_TREE_DUMMY_ROOT
}

void SceneTree::AfterLoadFromFile()
{
    SetupCurrNode();
}

bool SceneTree::Add(const n0::SceneNodePtr& node)
{
#ifndef ITT_SCENE_TREE_DUMMY_ROOT
    if (m_path.patrs.empty())
    {
        auto eval = std::make_shared<Evaluator>();
        m_eval_cache.insert({ node, eval });
        m_path.patrs.push_back(PathPart(node, eval));

        return true;
    }
#endif // ITT_SCENE_TREE_DUMMY_ROOT

    assert(!m_path.patrs.empty());

    // reset flags
    ClearNodeDisplayTag();

    // update scene node
    auto& curr = m_path.patrs.back();
    assert(curr.node->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = curr.node->GetSharedComp<n0::CompComplex>();
    ccomplex.AddChild(node);

    if (node->HasUniqueComp<bp::CompNode>())
    {
        auto& bp_node = node->GetUniqueComp<bp::CompNode>().GetNode();
        auto& curr = m_path.patrs.back();

        // update front
        if (curr.node->HasUniqueComp<bp::CompNode>())
        {
            auto bp_parent = curr.node->GetUniqueComp<bp::CompNode>().GetNode();
            if (bp_parent && bp_parent->get_type() == rttr::type::get<node::Geometry>())
            {
                auto geo = std::static_pointer_cast<node::Geometry>(bp_parent);
                geo->children.push_back(bp_node);
            }
        }

        // front eval cb
        m_path.patrs.back().eval->OnAddNode(*bp_node);

        // update back
        if (m_path.patrs.size() > 1)
        {
            auto& prev_eval = m_path.patrs[m_path.patrs.size() - 2].eval;
            auto& curr_node = m_path.patrs.back().node;
            assert(curr_node->HasUniqueComp<bp::CompNode>());
            auto parent = prev_eval->QueryBackNode(*curr_node->GetUniqueComp<bp::CompNode>().GetNode());
            auto child = m_path.patrs.back().eval->QueryBackNode(*bp_node);
            assert(parent->get_type() == rttr::type::get<sop::node::Geometry>());
            sop::node::Geometry::AddChild(std::static_pointer_cast<sop::node::Geometry>(parent), child);
        }

        // prepare ccomplex
        auto type = bp_node->get_type();
        if (type == rttr::type::get<node::Geometry>()) {
            if (!node->HasSharedComp<n0::CompComplex>()) {
                node->AddSharedComp<n0::CompComplex>();
            }
        }

        // update flags
        if (type.is_derived_from<Node>()) {
            auto itt_node = std::static_pointer_cast<Node>(bp_node);
            itt_node->SetDisplay(true);
        }
    }

    return true;
}

bool SceneTree::Remove(const n0::SceneNodePtr& node)
{
    if (m_path.patrs.empty()) {
        return false;
    }

    if (node == m_path.patrs.front().node)
    {
        return Clear();
    }
    else
    {
        // update scene node
        auto& curr = m_path.patrs.back();
        assert(curr.node->HasSharedComp<n0::CompComplex>());
        auto& ccomplex = curr.node->GetSharedComp<n0::CompComplex>();
        bool dirty = ccomplex.RemoveChild(node);

        if (dirty && node->HasUniqueComp<bp::CompNode>())
        {
            // update front
            if (curr.node->HasUniqueComp<bp::CompNode>())
            {
                auto bp_parent = curr.node->GetUniqueComp<bp::CompNode>().GetNode();
                if (bp_parent && bp_parent->get_type() == rttr::type::get<node::Geometry>())
                {
                    auto geo = std::static_pointer_cast<node::Geometry>(bp_parent);
                    auto& bp_node = node->GetUniqueComp<bp::CompNode>().GetNode();
                    for (auto itr = geo->children.begin(); itr != geo->children.end(); )
                    {
                        if (*itr == bp_node) {
                            itr = geo->children.erase(itr);
                        } else {
                            ++itr;
                        }
                    }
                }
            }

            // front eval cb
            auto& bp_node = node->GetUniqueComp<bp::CompNode>().GetNode();
            curr.eval->OnRemoveNode(*bp_node);

            // update back
            if (curr.node->HasUniqueComp<bp::CompNode>() && m_path.patrs.size() > 1)
            {
                auto bp_parent = curr.node->GetUniqueComp<bp::CompNode>().GetNode();
                if (bp_parent && bp_parent->get_type() == rttr::type::get<node::Geometry>())
                {
                    auto geo = std::static_pointer_cast<node::Geometry>(bp_parent);

                    auto& prev_eval = m_path.patrs[m_path.patrs.size() - 2].eval;
                    auto& curr_node = m_path.patrs.back().node;
                    assert(curr_node->HasUniqueComp<bp::CompNode>());
                    auto parent = prev_eval->QueryBackNode(*curr_node->GetUniqueComp<bp::CompNode>().GetNode());
                    auto child = m_path.patrs.back().eval->QueryBackNode(*bp_node);
                    assert(parent->get_type() == rttr::type::get<sop::node::Geometry>());
                    RebuildBackFromFront(std::static_pointer_cast<sop::node::Geometry>(parent), geo, *curr.eval);
                }
            }
        }

        return dirty;
    }
}

bool SceneTree::Clear()
{
    if (m_path.patrs.empty()) {
        return false;
    }

    // update scene node
    auto& curr = m_path.patrs.back();
    assert(curr.node->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = curr.node->GetSharedComp<n0::CompComplex>();
    bool dirty = !ccomplex.GetAllChildren().empty();
	ccomplex.RemoveAllChildren();

    // update front
    if (curr.node->HasUniqueComp<bp::CompNode>())
    {
        auto bp_parent = curr.node->GetUniqueComp<bp::CompNode>().GetNode();
        if (bp_parent && bp_parent->get_type() == rttr::type::get<node::Geometry>())
        {
            auto geo = std::static_pointer_cast<node::Geometry>(bp_parent);
            geo->children.clear();
        }
    }

    // front eval cb
    assert(curr.eval);
    curr.eval->OnClearAllNodes();

    // update back
    if (m_path.patrs.size() > 1)
    {
        auto& prev_eval = m_path.patrs[m_path.patrs.size() - 2].eval;
        auto& curr_node = m_path.patrs.back().node;
        assert(curr_node->HasUniqueComp<bp::CompNode>());
        auto parent = prev_eval->QueryBackNode(*curr_node->GetUniqueComp<bp::CompNode>().GetNode());
        assert(parent->get_type() == rttr::type::get<sop::node::Geometry>());
        std::static_pointer_cast<sop::node::Geometry>(parent)->ClearChildren();
    }

    return dirty;
}

bool SceneTree::ToNextLevel(const n0::SceneNodePtr& node)
{
    if (!node->HasUniqueComp<bp::CompNode>()) {
        return false;
    }

    auto& bp_node = node->GetUniqueComp<bp::CompNode>().GetNode();
    auto type = bp_node->get_type();
    if (type != rttr::type::get<node::Geometry>()) {
        return false;
    }

    assert(IsCurrChild(node));
    auto itr = m_eval_cache.find(node);
    if (itr == m_eval_cache.end())
    {
        auto eval = std::make_shared<Evaluator>();
        if (node->HasSharedComp<n0::CompComplex>())
        {
            for (auto& c : node->GetSharedComp<n0::CompComplex>().GetAllChildren()) {
                if (c->HasUniqueComp<bp::CompNode>()) {
                    auto& bp_node = c->GetUniqueComp<bp::CompNode>().GetNode();
                    eval->OnAddNode(*bp_node, false);
                }
            }

            if (node->HasUniqueComp<bp::CompNode>())
            {
                auto bp_parent = node->GetUniqueComp<bp::CompNode>().GetNode();
                if (bp_parent->get_type() == rttr::type::get<node::Geometry>())
                {
                    auto src = std::static_pointer_cast<node::Geometry>(bp_parent);
                    auto dst = GetCurrEval()->QueryBackNode(*src);
                    assert(dst && dst->get_type() == rttr::type::get<sop::node::Geometry>());
                    auto dst_geo = std::static_pointer_cast<sop::node::Geometry>(dst);
                    RebuildBackFromFront(dst_geo, src, *eval);
                }
            }
        }

        m_eval_cache.insert({ node, eval });
        m_path.patrs.push_back({ node, eval });

        eval->OnRebuildConnection();
    }
    else
    {
        m_path.patrs.push_back({ node, itr->second });
    }

    SetupCurrNode();

    return true;
}

bool SceneTree::SetDepth(size_t depth)
{
    if (depth >= m_path.patrs.size()) {
        return false;
    }

    m_path.patrs.erase(m_path.patrs.begin() + depth + 1, m_path.patrs.end());

    SetupCurrNode();

    return true;
}

void SceneTree::ClearNodeDisplayTag()
{
    if (m_path.patrs.empty()) {
        return;
    }

    auto curr = m_path.patrs.back();
    for (auto& itr : curr.eval->GetAllNodes())
    {
        auto bp_node = itr.first;
        if (bp_node->get_type().is_derived_from<Node>())
        {
            auto itt_node = static_cast<const Node*>(bp_node);
            const_cast<Node*>(itt_node)->SetDisplay(false);
        }
    }
}

n0::SceneNodePtr SceneTree::GetRoot() const
{
#ifdef ITT_SCENE_TREE_DUMMY_ROOT
    return m_path.patrs.size() <= 1 ? nullptr : m_path.patrs[m_path.patrs.size() - 2].node;
#else
    return m_path.patrs.empty() ? nullptr : m_path.patrs.front().node;
#endif // ITT_SCENE_TREE_DUMMY_ROOT
}

void SceneTree::InitDummyRoot()
{
    auto node = ns::NodeFactory::Create2D();
    node->AddSharedComp<n0::CompComplex>();

    auto eval = std::make_shared<Evaluator>();
    m_eval_cache.insert({ node, eval });

    m_path.patrs.push_back(PathPart(node, eval));
}

bool SceneTree::IsCurrChild(const n0::SceneNodePtr& node) const
{
    if (m_path.patrs.empty()) {
        return false;
    }

    auto curr = m_path.patrs.back();
    assert(curr.node->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = curr.node->GetSharedComp<n0::CompComplex>();
    for (auto& child : ccomplex.GetAllChildren()) {
        if (child == node) {
            return true;
        }
    }
    return false;
}

void SceneTree::SetupCurrNode()
{
    if (m_path.patrs.empty()) {
        return;
    }

    auto curr = m_path.patrs.back();
    assert(curr.node->HasSharedComp<n0::CompComplex>());
    auto& ccomplex = curr.node->GetSharedComp<n0::CompComplex>();
    for (auto& c : ccomplex.GetAllChildren())
    {
        n0::NodeFlagsHelper::SetFlag<n0::NodeNotVisible>(*c, false);
        if (c->HasSharedComp<n0::CompComplex>()) {
            for (auto& cc : c->GetSharedComp<n0::CompComplex>().GetAllChildren()) {
                n0::NodeFlagsHelper::SetFlag<n0::NodeNotVisible>(*cc, true);
            }
        }
    }
}

}