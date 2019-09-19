#include "intention/Evaluator.h"
#include "intention/Everything.h"
#include "intention/RegistNodes.h"

#include <blueprint/Node.h>
#include <blueprint/Pin.h>
#include <blueprint/Connecting.h>
#include <blueprint/CompNode.h>

#include <node0/SceneNode.h>
#include <node0/CompComplex.h>
#include <everything/EvalContext.h>
#include <everything/node/GroupCreate.h>
#include <everything/node/GroupExpression.h>

#include <queue>

namespace itt
{

void Evaluator::OnAddNode(const bp::Node& front)
{
    auto back = Everything::CreateBackFromFront(front);
    if (!back) {
        return;
    }

    m_eval.AddNode(back);
    m_nodes_map.insert({ &front, back });

    if (front.get_type().is_derived_from<Node>()) {
        const_cast<Node&>(static_cast<const Node&>(front)).SetName(back->GetName());
    }

    Everything::UpdatePropBackFromFront(front, *back, *this);

    Update();
}

void Evaluator::OnRemoveNode(const bp::Node& node)
{
    auto itr = m_nodes_map.find(&node);
    if (itr == m_nodes_map.end()) {
        return;
    }

    m_eval.RemoveNode(itr->second);
    m_nodes_map.erase(itr);

    Update();
}

void Evaluator::OnClearAllNodes()
{
    m_eval.ClearAllNodes();
    m_nodes_map.clear();

    Update();
}

void Evaluator::OnNodePropChanged(const bp::NodePtr& node)
{
    auto itr = m_nodes_map.find(node.get());
    assert(itr != m_nodes_map.end());
    Everything::UpdatePropBackFromFront(*node, *itr->second, *this);

    if (node->get_type().is_derived_from<Node>())
    {
        auto& itt_n = std::static_pointer_cast<Node>(node);
        if (itt_n->GetName() != itr->second->GetName()) {
            m_eval.Rename(itr->second->GetName(), itt_n->GetName());
            if (itr->second->GetName() != itt_n->GetName()) {
                itt_n->SetName(itr->second->GetName());
            }
        }
    }

    //m_eval.PropChanged(itr->second);
    m_eval.MakeDirty();

    Update();
}

void Evaluator::OnConnected(const bp::Connecting& conn)
{
    auto f_pin = conn.GetFrom();
    auto t_pin = conn.GetTo();

    auto f_itr = m_nodes_map.find(&f_pin->GetParent());
    auto t_itr = m_nodes_map.find(&t_pin->GetParent());
    if (f_itr == m_nodes_map.end() || t_itr == m_nodes_map.end()) {
        return;
    }

    if (t_itr->first->GetAllInput().size() > t_itr->second->GetImports().size()) {
        t_itr->second->AddInputPorts(t_itr->first->GetAllInput().size() - t_itr->first->GetAllOutput().size());
    }

    m_eval.Connect(
        { f_itr->second, f_pin->GetPosIdx() },
        { t_itr->second, t_pin->GetPosIdx() }
    );

    Update();
}

void Evaluator::OnDisconnecting(const bp::Connecting& conn)
{
    auto f_pin = conn.GetFrom();
    auto t_pin = conn.GetTo();

    auto f_itr = m_nodes_map.find(&f_pin->GetParent());
    auto t_itr = m_nodes_map.find(&t_pin->GetParent());
    if (f_itr == m_nodes_map.end() || t_itr == m_nodes_map.end()) {
        return;
    }

    m_eval.Disconnect(
        { f_itr->second, f_pin->GetPosIdx() },
        { t_itr->second, t_pin->GetPosIdx() }
    );

    Update();
}

void Evaluator::OnRebuildConnection()
{
    std::vector<std::pair<evt::Node::PortAddr, evt::Node::PortAddr>> conns;
    for (auto& itr : m_nodes_map)
    {
        auto& front = itr.first;
        auto& back  = itr.second;
        for (auto& in : front->GetAllInput())
        {
            for (auto& conn : in->GetConnecting())
            {
                auto f_pin = conn->GetFrom();
                auto t_pin = conn->GetTo();

                auto f_itr = m_nodes_map.find(&f_pin->GetParent());
                auto t_itr = m_nodes_map.find(&t_pin->GetParent());
                if (f_itr == m_nodes_map.end() || t_itr == m_nodes_map.end()) {
                    continue;
                }

                if (t_itr->first->GetAllInput().size() > t_itr->second->GetImports().size()) {
                    t_itr->second->AddInputPorts(t_itr->first->GetAllInput().size() - t_itr->first->GetAllOutput().size());
                }

                conns.push_back({
                    { f_itr->second, f_pin->GetPosIdx() },
                    { t_itr->second, t_pin->GetPosIdx() }
                });
            }
        }
    }

    m_eval.RebuildConnections(conns);

    Update();
}

float Evaluator::CalcFloat(const std::string& expr, const evt::Node& node, float expect) const
{
    auto var = m_eval.CalcExpr(expr, evt::EvalContext(m_eval, node));
    switch (var.type)
    {
    case evt::VarType::Bool:
        return var.b ? 1.0f : 0.0f;
    case evt::VarType::Int:
        return static_cast<float>(var.i);
    case evt::VarType::Float:
        return var.f;
    case evt::VarType::Float3:
    {
        auto f3 = static_cast<const float*>(var.p);
        return f3[0];
    }
    case evt::VarType::Double:
        return static_cast<float>(var.d);
    default:
        return expect;
    }
}

int Evaluator::CalcInt(const std::string& expr, const evt::Node& node, int expect) const
{
    auto var = m_eval.CalcExpr(expr, evt::EvalContext(m_eval, node));
    switch (var.type)
    {
    case evt::VarType::Bool:
        return var.b ? 1 : 0;
    case evt::VarType::Int:
        return var.i;
    case evt::VarType::Float:
        return static_cast<int>(var.f);
    case evt::VarType::Float3:
    {
        auto f3 = static_cast<const float*>(var.p);
        return static_cast<int>(f3[0]);
    }
    case evt::VarType::Double:
        return static_cast<int>(var.d);
    default:
        return expect;
    }
}

evt::NodePtr Evaluator::QueryBackNode(const bp::Node& front_node) const
{
    auto itr = m_nodes_map.find(&front_node);
    return itr == m_nodes_map.end() ? nullptr : itr->second;
}

void Evaluator::Update()
{
    m_eval.Update();

    UpdateGroupName();
}

void Evaluator::UpdateGroupName()
{
    for (auto& itr : m_nodes_map)
    {
        auto type = itr.first->get_type();
        if (type == rttr::type::get<node::GroupCreate>())
        {
            assert(itr.second->get_type() == rttr::type::get<evt::node::GroupCreate>());
            auto front = static_cast<const node::GroupCreate*>(itr.first);
            auto back = std::static_pointer_cast<evt::node::GroupCreate>(itr.second);
            const_cast<node::GroupCreate*>(front)->group_name = back->GetGroupName();
        }
        else if (type == rttr::type::get<node::GroupExpression>())
        {
            assert(itr.second->get_type() == rttr::type::get<evt::node::GroupExpression>());
            auto front = const_cast<node::GroupExpression*>(
                static_cast<const node::GroupExpression*>(itr.first)
            );
            auto back = std::static_pointer_cast<evt::node::GroupExpression>(itr.second);
            auto& back_insts = back->GetInstances();
            size_t idx = 0;
            if (!front->inst0.expr_str.empty()) {
                assert(idx < back_insts.size());
                front->inst0.group_name = back_insts[idx++].group_name;
            }
            if (!front->inst1.expr_str.empty()) {
                assert(idx < back_insts.size());
                front->inst1.group_name = back_insts[idx++].group_name;
            }
            if (!front->inst2.expr_str.empty()) {
                assert(idx < back_insts.size());
                front->inst2.group_name = back_insts[idx++].group_name;
            }
            if (!front->inst3.expr_str.empty()) {
                assert(idx < back_insts.size());
                front->inst3.group_name = back_insts[idx++].group_name;
            }
            assert(idx == back_insts.size());
        }
    }
}

}